// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#include "imlab/queryc/query_parse_context.h"
#include "./gen/query_parser.h"
#include <sstream>
#include <set>
#include "imlab/infra/error.h"
#include "imlab/infra/hash.h"
#include "imlab/schemac/schema_parse_context.h"
#include "imlab/schemac/schema_compiler.h"
#include "imlab/algebra/table_scan.h"
#include "imlab/algebra/print.h"
#include "imlab/algebra/inner_join.h"
#include "imlab/algebra/selection.h"
// ---------------------------------------------------------------------------------------------------
using namespace imlab;
using namespace imlab::schemac;
using QueryParseContext = imlab::queryc::QueryParseContext;
// ---------------------------------------------------------------------------------------------------

// Constructor
QueryParseContext::QueryParseContext(const schemac::Schema& schema, bool trace_scanning, bool trace_parsing)
    : schema(schema), trace_scanning_(trace_scanning), trace_parsing_(trace_parsing) {}
// ---------------------------------------------------------------------------------------------------
// Destructor
QueryParseContext::~QueryParseContext() {}
// ---------------------------------------------------------------------------------------------------
// Parse a string
Query& QueryParseContext::Parse(std::istream &in) {
    beginScan(in);
    imlab::queryc::QueryParser parser(*this);
    parser.set_debug_level(trace_parsing_);
    parser.parse();
    endScan();

    return this->query;
}
// ---------------------------------------------------------------------------------------------------
// Yield an error
void QueryParseContext::Error(const std::string& m) {
    throw QueryCompilationError(m);
}
// ---------------------------------------------------------------------------------------------------
// Yield an error
void QueryParseContext::Error(uint32_t line, uint32_t column, const std::string &err) {
    std::stringstream ss {};
    ss << "[ l=" << line << " c=" << column << " ] " << err << std::endl;
    throw QueryCompilationError(ss.str());
}
// ---------------------------------------------------------------------------------------------------
// Define a table
void QueryParseContext::CreateSqlQuery(const std::vector<std::string> &select_columns,
                                       const std::vector<std::string> &relations,
                                       const std::vector<std::pair<std::string, std::string>> &where_predicates) {
    if (select_columns.size() == 0) {
        throw QueryCompilationError("You need to provide at least one column name in the SELECT clause.");
    }
    if (relations.size() == 0) {
        throw QueryCompilationError("You need to provide at least one table in the FROM clause.");
    }

    // A list of operators to apply
    std::vector<TableScan> scans {};
    std::vector<Selection> selections {};
    std::vector<InnerJoin> joins {};
    std::vector<const IU*> columns_to_print {};

    // Helper structures that collect all the info we get during the processing step
    std::vector<Table*> involved_tables {};
    std::set<const IU*> involved_ius {};
    std::vector<std::pair<const IU*, std::string>> selection_attr {};
    std::vector<std::pair<const IU*, const IU*>> join_attr {};

    // Get all the involved tables, e.g. resolve all strings after the "from ..." clause in the query.
    // A table also means that we create a table scan for it.
    // So basically there is a 1:1 mapping between "scans" and "involved_tables".
    involved_tables.reserve(relations.size());
    for (auto& r : relations) {
        auto it_r = std::find_if(schema.tables.begin(), schema.tables.end(), [&](const auto& t) { return t.id == r; });
        if (it_r == schema.tables.end()) {
            std::stringstream ss {};
            ss << "Table '" << r << "' not found.";
            throw QueryCompilationError(ss.str());
        } else {
            involved_tables.push_back(&*it_r);
            scans.emplace_back(it_r->id.c_str());
        }
    }

    // Now collect all IUs that are needed for the query.
    // The first set of IUs (= table + column + type) is obtained from the column enumeration after the "select...".
    // The second set of IUs are the ones that are needed for predicates (joins or selections) after the "where...".
    // The first set is just used for the print statement; the second set actually needs some processing afterwards.

    // Resolves a column name to an IU by searching all tables that are used for this query.
    auto find_iu_by_column = [&](const std::string& column) -> std::optional<const IU*> {
        for (auto& s : scans) {
            const auto& ius = s.CollectIUs();
            auto it_iu = std::find_if(ius.begin(), ius.end(), [&](const auto& iu) { return std::string(iu->column) == column; });
            if (it_iu != ius.end()) {
                return *it_iu;
            }
        }
        return {};
    };
    // Gather all the IUs from the "select..." clause.
    for (auto& column : select_columns) {
        auto iu = find_iu_by_column(column);
        if (!iu) {
            std::stringstream ss {};
            ss << "Column '" << column << "' not found.";
            throw QueryCompilationError(ss.str());
        } else {
            involved_ius.insert(*iu);
            columns_to_print.push_back(*iu);
        }
    }
    // Gather all IUs from the predicates after the "where..."
    for (auto& [column1, column2] : where_predicates) {
        auto iu_1 = find_iu_by_column(column1);
        if (iu_1) {
            involved_ius.insert(*iu_1);
        }
        auto iu_2 = find_iu_by_column(column2);
        if (iu_2) {
            involved_ius.insert(*iu_2);
        }

        // Now things get interesting:
        // - if we found IUs for both identifiers, we have a join
        // - if we found an IU for just one of them, we have a selection -> we need to get the type

        // JOIN
        if (iu_1 && iu_2) {
            join_attr.emplace_back(*iu_1, *iu_2);
            continue;
        }

        // SELECTIONS
        if ((iu_1 && !iu_2) || (!iu_1 && iu_2)) {
            auto &select_iu = (iu_1) ? *iu_1 : *iu_2;
            auto &select_value_raw = (iu_1) ? column2 : column1;
            auto select_value =
                    SchemaCompiler::generateTypeName(select_iu->type) + "::castString(\"" + select_value_raw + "\", " +
                    std::to_string(select_value_raw.length()) + ")";
            selection_attr.emplace_back(select_iu, select_value);
            continue;
        }

        // If we got here, neither iu_1 nor iu_2 reference a valid IU.
        std::stringstream ss {};
        ss << "Column '" << column1 << "' and '" << column2 << "' not found.";
        throw QueryCompilationError(ss.str());
    }

    // Now, we have all the table scans and a number of selection attributes and a number of join attributes.
    // We need to sort them out and actually build a query tree.

    // For every table(-scan), create a selection (might be empty, however)
    for (unsigned i = 0; i < scans.size(); i++) {
        auto& scan = scans[i];
        auto& table = involved_tables[i];

        std::vector<std::pair<const IU*, std::string>> predicates {};
        for (auto& selection : selection_attr) {
            if (table->id == std::string(selection.first->table)) {
                predicates.push_back(selection);
            }
        }
        Selection s(std::make_unique<TableScan>(scan), predicates);
        selections.push_back(std::move(s));
    }

    // The trickier part is actually generating joins:
    // The requirement is "Build left-deep join trees based on the order of the relations in the from clause."
    // Thus, we can simply use the order from the from clause and assume a lot of things that would not work for real queries.
    Operator* left_operator = &selections[0];
    for (auto it = selections.begin() + 1; it != selections.end(); it++) {
        Selection* right_operator = &*it;

        const auto& left_ius = left_operator->CollectIUs();
        const auto& right_ius = right_operator->CollectIUs();

        // Now look through all join predicates and see if some of them are applicable to this join
        std::vector<std::pair<const IU*, const IU*>> applicable_join_predicates {};
        for (auto& attr : join_attr) {
            if (std::find(left_ius.begin(), left_ius.end(), attr.first) != left_ius.end()
                && std::find(right_ius.begin(), right_ius.end(), attr.second) != right_ius.end()) {
                applicable_join_predicates.emplace_back(attr.first, attr.second);
            } else if (std::find(left_ius.begin(), left_ius.end(), attr.second) != left_ius.end()
                       && std::find(right_ius.begin(), right_ius.end(), attr.first) != right_ius.end()) {
                applicable_join_predicates.emplace_back(attr.second, attr.first);
            }
        }

        if (applicable_join_predicates.size() == 0) {
            // Hmm, bad: we would need a cross product...
            // Of course, in real databases this is not necessarily the case because we can influence the join order.
            // But here we have the strong asumption that the query string already contains a meaningful join order.
            throw QueryCompilationError("Cross-products are not allowed.");
        }

        // Now that we have everything, we can create a Join
        if (left_operator == &selections[0]) {
            InnerJoin j (std::make_unique<Selection>(std::move(*static_cast<Selection*>(left_operator))),
                         std::make_unique<Selection>(std::move(*right_operator)),
                         applicable_join_predicates);
            joins.push_back(std::move(j));
        } else {
            InnerJoin j (std::make_unique<InnerJoin>(std::move(*static_cast<InnerJoin*>(left_operator))),
                         std::make_unique<Selection>(std::move(*right_operator)),
                         applicable_join_predicates);
            joins.push_back(std::move(j));
        }

        left_operator = &joins[joins.size() - 1];
    }

    // The final statement of the query is a "print()".
    // Take the uppermost join and print the requested columns of the result.
    if (joins.size() == 0 && selections.size() == 1) {
        Print print(std::make_unique<Selection>(std::move(selections[0])));
        this->query.op = std::move(print);
    } else if (joins.size() > 0) {
        Print print(std::make_unique<InnerJoin>(std::move(joins[joins.size() - 1])));
        this->query.op = std::move(print);
    } else {
        // Strange query: multiple tables, but no joins...
        throw QueryCompilationError("Cross-products are not allowed.");
    }

    // We must call the Prepare function at the end because this internally connects
    // the query components with raw pointers. They should be stable, so no object
    // moving is allowed afterwards...
    this->query.op->Prepare(columns_to_print, nullptr);
}
// ---------------------------------------------------------------------------------------------------
