// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <sstream>
#include "imlab/queryc/query_parse_context.h"
#include "imlab/schemac/default_schema.h"
#include "gtest/gtest.h"

using QueryParseContext = imlab::queryc::QueryParseContext;

namespace {

TEST(QueryParseContextTest, ParseSimpleSelect) {
    std::istringstream in("select w_id from warehouse;");
    QueryParseContext qpc {imlab::schemac::defaultSchema};
    auto& query = qpc.Parse(in);
    auto& print = *query.op;
}

}  // namespace
