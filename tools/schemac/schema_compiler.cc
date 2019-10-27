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
    createSource(schema);
}

void SchemaCompiler::createHeader(Schema &schema) {
    header_ << "#ifndef INCLUDE_IMLAB_SCHEMA_H_" << std::endl;
    header_ << "#define INCLUDE_IMLAB_SCHEMA_H_" << std::endl;
    header_ << std::endl;

    header_ << "#include <vector>" << std::endl;
    header_ << "#include <unordered_map>" << std::endl;
    header_ << "#include \"imlab/infra/hash.h\"" << std::endl;
    header_ << "#include \"imlab/infra/types.h\"" << std::endl;
    header_ << std::endl;

    header_ << "namespace imlab {" << std::endl;
    header_ << std::endl;

    header_ << "template<typename... Types>" << std::endl;
    header_ << "class TableBase {" << std::endl;
    header_ << " public:" << std::endl;
    header_ << "    uint64_t insert_tuple(const std::tuple<Types ...>& tuple);" << std::endl;
    header_ << "    std::tuple<Types ...> read_tuple(const uint64_t tid);" << std::endl;
    header_ << "    void update_tuple(const uint64_t tid, const std::tuple<Types ...>& tuple);" << std::endl;
    header_ << "    void delete_tuple(const uint64_t tid);" << std::endl;
    header_ << "    uint64_t get_size() { return size; }" << std::endl;
    header_ << " private:" << std::endl;
    header_ << "    uint64_t size = 0;" << std::endl;
    header_ << "};" << std::endl;
    header_ << std::endl;

    header_ << "enum Relation {" << std::endl;
    for(auto& table : schema.tables) {
        header_ << "    k" << table.id << "," << std::endl;
    }
    header_ << "};" << std::endl;
    header_ << std::endl;

    for(auto& table : schema.tables) {
        generateTableClassHeader(table);
        header_ << std::endl;
    }

    header_ << "}  // namespace imlab" << std::endl;
    header_ << "#endif  // INCLUDE_IMLAB_SCHEMA_H_" << std::endl;
}

void SchemaCompiler::generateTableClassHeader(Table &table) {
    header_ << "class " << table.id << "Table" << " : public TableBase<";
    for(auto& column : table.columns) {
        header_ << generateTypeName(column.type) << ((&column != &*table.columns.end() - 1)? ", " : "");
    }
    header_ << "> {" << std::endl;

    // vectors for columns
    for(auto& column : table.columns) {
        header_ << "    std::vector<" << generateTypeName(column.type) << "> " << column.id << ";" << std::endl;
    }

    // primary key
    if(table.primary_key.size() > 0) {
        header_ << std::endl;
        header_ << "    std::unordered_map<Key<";
        for(auto& column : table.primary_key) {
            header_ << generateTypeName(column.type) << ((&column != &*table.primary_key.end() - 1)? ", " : "");
        }
        header_ << ">, uint64_t> primary_key;" << std::endl;
    }

    header_ << "};" << std::endl;
}

void SchemaCompiler::createSource(Schema &schema) {
    impl_ << "#include \"schema.h\"" << std::endl;
    impl_ << std::endl;

    for(auto& table : schema.tables) {
        generateTableClassSource(table);
        impl_ << std::endl;
    }
}

void SchemaCompiler::generateTableClassSource(Table &table) {
    // functions: insert
    impl_ << "uint64_t " << table.id << "Table" << "::" << "insert_tuple(std::tuple<";
    for(auto& column : table.columns) {
        impl_ << generateTypeName(column.type) << ((&column != &*table.columns.end() - 1)? ", " : "");
    }
    impl_ << "> tuple) {" << std::endl;

    auto index = 0;
    for(auto& column : table.columns) {
        impl_ << "    " << column.id << ".push_back(" << "std::get<" << index << ">(tuple)" << ");" << std::endl;
        index++;
    }
    impl_ << std::endl;
    impl_ << "    auto insert_pos = size;" << std::endl;

    // update primary key index if exists
    if(table.primary_key.size() > 0) {
        impl_ << "    " << "primary_key[Key(";
        auto index = 0;
        for(auto& column : table.primary_key) {
            impl_ << "std::get<" << index << ">(tuple),";
            index++;
        }
        impl_ << ")] = insert_pos;" << std::endl;
        impl_ << std::endl;
    }
    impl_ << "    size++;" << std::endl;
    impl_ << "    return insert_pos;" << std::endl;

    impl_ << "}" << std::endl;
    impl_ << std::endl;

    // functions: read
    impl_ << "std::tuple<";
    for(auto& column : table.columns) {
        impl_ << generateTypeName(column.type) << ((&column != &*table.columns.end() - 1)? ", " : "");
    }
    impl_ << "> " << table.id << "Table" << "::" << "read_tuple(const uint64_t tid) {" << std::endl;

    impl_ << "}" << std::endl;
    impl_ << std::endl;

    // functions: update
    impl_ << "void " << table.id << "Table" << "::" << "update_tuple(const uint64_t tid, std::tuple<";
    for(auto& column : table.columns) {
        impl_ << generateTypeName(column.type) << ((&column != &*table.columns.end() - 1)? ", " : "");
    }
    impl_ << "> tuple) {" << std::endl;

    impl_ << "}" << std::endl;
    impl_ << std::endl;

    // functions: delete
    impl_ << "void " << table.id << "Table" << "::" << "delete_tuple(const uint64_t tid) {" << std::endl;

    impl_ << "}" << std::endl;
    impl_ << std::endl;
}

std::string SchemaCompiler::generateTypeName(Type &type) {
    std::stringstream ss;
    switch(type.tclass) {
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
// ---------------------------------------------------------------------------------------------------
