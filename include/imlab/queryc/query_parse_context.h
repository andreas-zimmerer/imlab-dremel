// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_QUERYC_QUERY_PARSE_CONTEXT_H_
#define INCLUDE_IMLAB_QUERYC_QUERY_PARSE_CONTEXT_H_
// ---------------------------------------------------------------------------------------------------
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../schemac/schema_parse_context.h"
#include "../algebra/query.h"
// ---------------------------------------------------------------------------------------------------
namespace imlab {
namespace queryc {
// ---------------------------------------------------------------------------------------------------
struct QueryParser;
struct QueryCompiler;
// ---------------------------------------------------------------------------------------------------
// Query parse context
class QueryParseContext {
    friend QueryParser;

 public:
    // Constructor
    explicit QueryParseContext(const schemac::Schema &schema, bool trace_scanning = false, bool trace_parsing = false);
    // Destructor
    virtual ~QueryParseContext();

    // Parse an istream
    Query& Parse(std::istream &in);

    // Throw an error
    void Error(uint32_t line, uint32_t column, const std::string &err);
    // Throw an error
    void Error(const std::string &m);

 private:
    // Begin a scan
    void beginScan(std::istream &in);
    // End a scan
    void endScan();

    // Only need for storing the result -> is there a nicer way?
    Query query;

    // The schema the query is created for
    schemac::Schema schema;

    // create a table
    void CreateSqlQuery(const std::vector<std::string> &select_columns,
                        const std::vector<std::string> &relations,
                        const std::vector<std::pair<std::string, std::string>> &where_predicates = {});

    // Trace the scanning
    bool trace_scanning_;
    // Trace the parsing
    bool trace_parsing_;
};
// ---------------------------------------------------------------------------------------------------
}  // namespace queryc
}  // namespace imlab
// ---------------------------------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_QUERYC_QUERY_PARSE_CONTEXT_H_
// ---------------------------------------------------------------------------------------------------

