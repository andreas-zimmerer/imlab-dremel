// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_QUERYC_QUERY_COMPILER_H_
#define INCLUDE_IMLAB_QUERYC_QUERY_COMPILER_H_
// ---------------------------------------------------------------------------------------------------
#include <ostream>
#include <string>
#include <sstream>

#include "imlab/queryc/query_parse_context.h"
#include "imlab/algebra/operator.h"
// ---------------------------------------------------------------------------------------------------
namespace imlab {
namespace queryc {
// ---------------------------------------------------------------------------------------------------
class QueryCompiler {
 public:
    // Constructor
    QueryCompiler(std::ostream &header, std::ostream &impl)
        : header_(header), impl_(impl) {}
    // Compile a query
    void Compile(Operator &query);

 private:
    // Output stream for the header
    std::ostream &header_;
    // Output stream for the implementation
    std::ostream &impl_;
};
// ---------------------------------------------------------------------------------------------------
}  // namespace queryc
}  // namespace imlab
// ---------------------------------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_QUERYC_QUERY_COMPILER_H_
// ---------------------------------------------------------------------------------------------------

