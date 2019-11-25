// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#include "imlab/queryc/query_compiler.h"
#include "imlab/queryc/query_parse_context.h"
#include <sstream>
// ---------------------------------------------------------------------------------------------------
using QueryCompiler = imlab::queryc::QueryCompiler;
// ---------------------------------------------------------------------------------------------------

void QueryCompiler::Compile(Query &query) {
    header_ << R"HEADER(
#ifndef INCLUDE_IMLAB_COMPILED_QUERY_H_
#define INCLUDE_IMLAB_COMPILED_QUERY_H_

namespace imlab {

    void Run(imlab::Database& db);

}  // namespace imlab
#endif  // INCLUDE_IMLAB_COMPILED_QUERY_H_
)HEADER";

    impl_ << R"IMPL(
#include "../../../include/imlab/schema.h"
#include "../../../include/imlab/database.h"
#include "../../../include/imlab/schemac/schema_parse_context.h"
#include <iostream>
#include <iomanip>

namespace imlab {

    extern "C" void Run(imlab::Database& db) {
)IMPL";
    query.GenerateCode(impl_);
    impl_ << R"IMPL(
    }

}  // namespace imlab
)IMPL";
}
// ---------------------------------------------------------------------------------------------------
