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
    header_ << "#ifndef INCLUDE_IMLAB_SCHEMA_H_" << std::endl;
    header_ << "#define INCLUDE_IMLAB_SCHEMA_H_" << std::endl;
    header_ << std::endl;

    header_ << "#include <vector>" << std::endl;
    header_ << "#include <unordered_map>" << std::endl;
    header_ << "#include \"imlab/infra/hash.h\"" << std::endl;
    header_ << std::endl;

    for(auto& table : schema.tables) {
        generateTableStruct(table);
        header_ << std::endl;
    }

    header_ << "#endif  // INCLUDE_IMLAB_SCHEMA_H_" << std::endl;
}

void SchemaCompiler::generateTableStruct(Table &table) {
    header_ << "struct " << table.id << "{" << std::endl;
    for(auto& column : table.columns) {
        header_ << "    std::vector<" << generateTypeName(column.type) << "> " << column.id << ";" << std::endl;
    }

    if(table.primary_key.size() > 0) {
        header_ << std::endl;
        header_ << "    std::unordered_map<Key<";
        for(auto& column : table.primary_key) {
            header_ << generateTypeName(column.type) << ",";
        }
        header_ << ">> primary_key;" << std::endl;
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
