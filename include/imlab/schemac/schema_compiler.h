// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_SCHEMAC_SCHEMA_COMPILER_H_
#define INCLUDE_IMLAB_SCHEMAC_SCHEMA_COMPILER_H_
// ---------------------------------------------------------------------------------------------------
#include "imlab/schemac/schema_parse_context.h"
#include <ostream>
#include <string>
#include <sstream>
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

    static std::string generateTypeName(const Type &type) {
        std::stringstream ss {};
        switch (type.tclass) {
            case Type::kNumeric:
                ss << "Numeric<" << type.length << ", " << type.precision << ">"; break;
            case Type::kChar:
                ss << "Char<" << type.length << ">"; break;
            case Type::kVarchar:
                ss << "Varchar<" << type.length << ">"; break;
            case Type::kInteger:
                ss << "Integer"; break;
            case Type::kTimestamp:
                ss << "Timestamp"; break;
        }
        return ss.str();
    }

    static std::string generateSchemacTypeName(const Type &type) {
        std::stringstream ss {};
        switch (type.tclass) {
            case Type::kNumeric:
                ss << "schemac::Type::Numeric(" << type.length << ", " << type.precision << ")"; break;
            case Type::kChar:
                ss << "schemac::Type::Char(" << type.length << ")"; break;
            case Type::kVarchar:
                ss << "schemac::Type::Varchar(" << type.length << ")"; break;
            case Type::kInteger:
                ss << "schemac::Type::Integer()"; break;
            case Type::kTimestamp:
                ss << "schemac::Type::Timestamp()"; break;
        }
        return ss.str();
    }

 private:
    // Output stream for the header
    std::ostream &header_;
    // Output stream for the implementation
    std::ostream &impl_;

    void createHeader(Schema &schema);

    void createSource(Schema &schema);
};
// ---------------------------------------------------------------------------------------------------
}  // namespace schemac
}  // namespace imlab
// ---------------------------------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_SCHEMAC_SCHEMA_COMPILER_H_
// ---------------------------------------------------------------------------------------------------

