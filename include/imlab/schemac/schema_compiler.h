// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_SCHEMAC_SCHEMA_COMPILER_H_
#define INCLUDE_IMLAB_SCHEMAC_SCHEMA_COMPILER_H_
// ---------------------------------------------------------------------------------------------------
#include "imlab/schemac/schema_parse_context.h"
#include <ostream>
// ---------------------------------------------------------------------------------------------------
namespace imlab {
namespace schemac {
// ---------------------------------------------------------------------------------------------------
class SchemaCompiler {
 public:
    // Constructor
    SchemaCompiler(std::ostream &header, std::ostream &impl)
        : header_(header), impl_(impl) {}
    // Compile a schema
    void Compile(Schema &schema);

 private:
    // Output stream for the header
    std::ostream &header_;
    // Output stream for the implementation
    std::ostream &impl_;
};
// ---------------------------------------------------------------------------------------------------
}  // namespace schemac
}  // namespace imlab
// ---------------------------------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_SCHEMAC_SCHEMA_COMPILER_H_
// ---------------------------------------------------------------------------------------------------

