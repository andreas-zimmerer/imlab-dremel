// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#include "imlab/schemac/schema_compiler.h"
#include "imlab/schemac/schema_parse_context.h"
#include <sstream>
// ---------------------------------------------------------------------------------------------------
using Column = imlab::schemac::Column;
using SchemaCompiler = imlab::schemac::SchemaCompiler;
using SchemaParseContext = imlab::schemac::SchemaParseContext;
using Schema = imlab::schemac::Schema;
using Table = imlab::schemac::Table;
using Type = imlab::schemac::Type;
// ---------------------------------------------------------------------------------------------------
// Compile a schema
void SchemaCompiler::Compile(Schema &schema) {
    createHeader(schema);
}

void SchemaCompiler::createHeader(Schema &schema) {
    header_ << "#include <vector>" << std::endl << std::endl;

    for(auto& table : schema.tables) {
        generateTableStruct(table);
        header_ << std::endl;
    }
}

void SchemaCompiler::generateTableStruct(Table &table) {
    header_ << "struct " << table.id << "{" << std::endl;
    for(auto& column : table.columns) {
        header_ << "    std::vector<" << generateTypeName(column.type) << "> " << column.id << "," << std::endl;
    }
    header_ << "}" << std::endl;
}

std::string SchemaCompiler::generateTypeName(Type &type) {
    std::stringstream ss;
    switch(type.tclass) {
        case Type::kNumeric:
            ss << type.Name() << "<" << type.length << ", " << type.precision << ">"; break;
        case Type::kChar:
        case Type::kVarchar:
            ss << type.Name() << "<" << type.length << ">"; break;
        case Type::kInteger:
        case Type::kTimestamp:
            ss << type.Name(); break;
    }
    return ss.str();
}
// ---------------------------------------------------------------------------------------------------
