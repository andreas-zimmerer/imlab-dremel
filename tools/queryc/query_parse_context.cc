// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#include "imlab/queryc/query_parse_context.h"
#include "./gen/query_parser.h"
#include <sstream>
#include <unordered_set>
#include "imlab/infra/error.h"
// ---------------------------------------------------------------------------------------------------
// Constructor
QueryParseContext::QueryParseContext(bool trace_scanning, bool trace_parsing)
    : trace_scanning_(trace_scanning), trace_parsing_(trace_parsing) {}
// ---------------------------------------------------------------------------------------------------
// Destructor
QueryParseContext::~QueryParseContext() {}
// ---------------------------------------------------------------------------------------------------
// Parse a string
Operator QueryParseContext::Parse(std::istream &in) {
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
    std::stringstream ss;
    ss << "[ l=" << line << " c=" << column << " ] " << err << std::endl;
    throw QueryCompilationError(ss.str());
}
// ---------------------------------------------------------------------------------------------------
// Define a table
void QueryParseContext::CreateSqlQuery(const std::vector<std::string> &select_columns,
                                       const std::vector<std::string> &relations,
                                       const std::vector<std::pair<std::string, std::string>> &where_predicates = {}) {

}
// ---------------------------------------------------------------------------------------------------
