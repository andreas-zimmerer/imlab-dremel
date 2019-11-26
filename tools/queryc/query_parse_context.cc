// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#include "imlab/queryc/query_parse_context.h"
#include "./gen/query_parser.h"
#include <sstream>
#include <set>
#include "imlab/infra/error.h"
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
    // A list of operators to apply
    std::vector<TableScan> scans {};
    std::vector<Selection> selections {};
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
        }

        // SELECTIONS
        auto& select_iu = (iu_1)? *iu_1 : *iu_2;
        auto& select_value_raw = (iu_1)? column2 : column1;
        auto select_value = SchemaCompiler::generateTypeName(select_iu->type) + "::castString(\"" + select_value_raw + "\", " + std::to_string(select_value_raw.length()) + ")";
        selection_attr.emplace_back(select_iu, select_value);
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


    // The final statement of the query is a "print()".
    // Take the uppermost join and print the requested columns of the result.
    Print print(std::make_unique<Selection>(std::move(selections[0])));
    this->query.op = std::move(print);

    // We must call the Prepare function at the end because this internally connects
    // the query components with raw pointers. They should be stable, so no object
    // moving is allowed afterwards...
    this->query.op->Prepare(columns_to_print, nullptr);
}
// ---------------------------------------------------------------------------------------------------
