// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#include "imlab/schemac/schema_parse_context.h"
#include "./gen/schema_parser.h"
#include <sstream>
#include <unordered_set>
#include "imlab/infra/error.h"
// ---------------------------------------------------------------------------------------------------
using SchemaParseContext = imlab::schemac::SchemaParseContext;
using Table = imlab::schemac::Table;
using Schema = imlab::schemac::Schema;
using Type = imlab::schemac::Type;
// ---------------------------------------------------------------------------------------------------
const char *Type::Name() const {
    switch (tclass) {
        case kInteger:      return "Integer";
        case kTimestamp:    return "Timestamp";
        case kNumeric:      return "Numeric";
        case kChar:         return "Character";
        case kVarchar:      return "Varchar";
        default:            return "Unknown";
    }
}
// ---------------------------------------------------------------------------------------------------
// Constructor
SchemaParseContext::SchemaParseContext(bool trace_scanning, bool trace_parsing)
    : trace_scanning_(trace_scanning), trace_parsing_(trace_parsing) {}
// ---------------------------------------------------------------------------------------------------
// Destructor
SchemaParseContext::~SchemaParseContext() {}
// ---------------------------------------------------------------------------------------------------
// Parse a string
Schema SchemaParseContext::Parse(std::istream &in) {
    beginScan(in);
    imlab::schemac::SchemaParser parser(*this);
    parser.set_debug_level(trace_parsing_);
    parser.parse();
    endScan();

    return this->schema;
}
// ---------------------------------------------------------------------------------------------------
// Yield an error
void SchemaParseContext::Error(const std::string& m) {
    throw SchemaCompilationError(m);
}
// ---------------------------------------------------------------------------------------------------
// Yield an error
void SchemaParseContext::Error(uint32_t line, uint32_t column, const std::string &err) {
    std::stringstream ss;
    ss << "[ l=" << line << " c=" << column << " ] " << err << std::endl;
    throw SchemaCompilationError(ss.str());
}
// ---------------------------------------------------------------------------------------------------
// Define a table
void SchemaParseContext::createTable(const std::string &id,
                                     const std::vector<Column> &columns,
                                     const std::vector<std::string> &primary_keys,
                                     const IndexType &indexType) {

    // map given column identifiers to real columns
    std::vector<Column> prim_keys_columns {};
    for (auto& prim_key : primary_keys) {
        auto c_it = std::find_if(columns.begin(), columns.end(), [&](const Column& c) { return c.id == prim_key; });
        if (c_it == columns.end()) {
            std::stringstream ss;
            ss << "Column name " << prim_key << " not found in table " << id << " while creating primary index.";
            Error(ss.str());
        } else {
            prim_keys_columns.push_back(*c_it);
        }
    }

    this->schema.tables.emplace_back(Table {id, columns, prim_keys_columns, indexType});
    if (prim_keys_columns.size() > 0) {
        this->schema.indexes.emplace_back(Index{id, id, prim_keys_columns, indexType});
    }
}

void SchemaParseContext::createIndex(const std::string &id,
                                     const std::string &table,
                                     const std::vector<std::string> &columns,
                                     const IndexType &indexType) {

    // map given table identifier to real table reference
    auto indexed_table_it = std::find_if(this->schema.tables.begin(), this->schema.tables.end(),
            [&](const Table& t) { return t.id.compare(table) == 0; });
    if (indexed_table_it == this->schema.tables.end()) {
        std::stringstream ss;
        ss << "Table with name " << table << " not found! Could not create index.";
        Error(ss.str());
    }
    auto& indexed_table = *indexed_table_it;

    // map given column identifiers to real columns
    std::vector<Column> indexed_columns {};
    for (auto& column_id : columns) {
        auto c_it = std::find_if(indexed_table.columns.begin(), indexed_table.columns.end(), [&](const Column& c) { return c.id == column_id; });
        if (c_it == indexed_table.columns.end()) {
            std::stringstream ss;
            ss << "Column name " << column_id << " not found in table " << indexed_table.id << " while creating index.";
            Error(ss.str());
        } else {
            indexed_columns.push_back(*c_it);
        }
    }

    this->schema.indexes.emplace_back(Index {id, indexed_table.id, indexed_columns, indexType});
}
// ---------------------------------------------------------------------------------------------------
