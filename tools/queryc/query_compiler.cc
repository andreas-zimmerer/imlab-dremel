// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#include "imlab/queryc/query_compiler.h"
#include "imlab/queryc/query_parse_context.h"
#include <sstream>
// ---------------------------------------------------------------------------------------------------
// Compile a query
void QueryCompiler::Compile(Operator &query) {
    out_h << R"HEADER(
#ifndef INCLUDE_IMLAB_COMPILED_QUERY_H_
#define INCLUDE_IMLAB_COMPILED_QUERY_H_

namespace imlab {

    void Run(imlab::Database& db);

}  // namespace imlab
#endif  // INCLUDE_IMLAB_COMPILED_QUERY_H_
)HEADER";

    out_cc << R"IMPL(
#include "../../../include/imlab/schema.h"
#include "../../../include/imlab/database.h"
#include "../../../include/imlab/schemac/schema_parse_context.h"
#include <iostream>
#include <iomanip>

namespace imlab {

    extern "C" void Run(imlab::Database& db) {
)IMPL";
    query.Produce(out_cc);
    out_cc << R"IMPL(
    }

}  // namespace imlab
)IMPL";
}
// ---------------------------------------------------------------------------------------------------
