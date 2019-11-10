// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#include "imlab/schemac/schema_compiler.h"
#include "imlab/schemac/schema_parse_context.h"
#include <sstream>
#include <algorithm>
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

std::string generateTypeName(Type &type) {
    std::stringstream ss;
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

void generateTableHeader(Table &table,  std::ostream& header_) {
    header_ << "class " << table.id << "Table" << " : public TableBase {" << std::endl;

    // Insert
    header_ << " public:" << std::endl;
    header_ << "    uint64_t insert(";
    for (auto& column : table.columns) {
        header_ << "const " << generateTypeName(column.type) << " " << column.id << ((&column != &*table.columns.end() - 1)? ", " : "");
    }
    header_ << ");" << std::endl;
    header_ << std::endl;

    // Get
    for (auto& column : table.columns) {
        header_ << "    std::optional<" << generateTypeName(column.type) << "> get_" << column.id << "(const uint64_t tid);" << std::endl;
    }
    header_ << std::endl;

    // Update
    for (auto& column : table.columns) {
        header_ << "    void update_" << column.id << "(const uint64_t tid, const " << generateTypeName(column.type) << " " << column.id <<");" << std::endl;
    }
    header_ << std::endl;

    // Remove
    header_ << "    void remove(const uint64_t tid);" << std::endl;
    header_ << std::endl;

    // Primary key
    if (table.primary_key.size() > 0) {
        header_ << "    // Primary Key for: ";
        for (auto& column : table.primary_key) {
            header_ << column.id << ", ";
        }
        header_ << std::endl;
        header_ << "    std::unordered_map<Key<";
        for (auto& column : table.primary_key) {
            header_ << generateTypeName(column.type) << ((&column != &*table.primary_key.end() - 1)? ", " : "");
        }
        header_ << ">, uint64_t> primary_key;" << std::endl;
        header_ << std::endl;
    }

    header_ << " private:" << std::endl;
    header_ << "    std::vector<bool> deleted_tuples;" << std::endl;
    header_ << "    size_t first_free_pos = 0;" << std::endl;
    header_ << std::endl;

    // Vectors for columns
    for (auto& column : table.columns) {
        header_ << "    std::vector<" << generateTypeName(column.type) << "> " << column.id << ";" << std::endl;
    }

    header_ << "};" << std::endl;
}

void SchemaCompiler::createHeader(Schema &schema) {
    header_ << R"HEADER(
#ifndef INCLUDE_IMLAB_SCHEMA_H_
#define INCLUDE_IMLAB_SCHEMA_H_

#include <vector>
#include <unordered_map>
#include "imlab/infra/hash.h"
#include "imlab/infra/types.h"

namespace imlab {
namespace tpcc {
)HEADER";

    header_ << R"HEADER(
class TableBase {
 public:
    uint64_t get_size() { return size; }
 protected:
    uint64_t size = 0;
};

)HEADER";

    header_ << "enum Relation {" << std::endl;
    for (auto& table : schema.tables) {
        header_ << "    k" << table.id << "," << std::endl;
    }
    header_ << "};" << std::endl;
    header_ << std::endl;

    for (auto& table : schema.tables) {
        generateTableHeader(table, header_);
        header_ << std::endl;
    }

    header_ << R"HEADER(
}  // namespace tpcc
}  // namespace imlab
#endif  // INCLUDE_IMLAB_SCHEMA_H_
)HEADER";
}

void generateInsertMethod(Table &table, std::ostream &impl_) {
    impl_ << "uint64_t " << table.id << "Table" << "::" << "insert(";
    for (auto& column : table.columns) {
        impl_ << std::endl << "        ";
        impl_ << "const " << generateTypeName(column.type) << " " << column.id << ((&column != &*table.columns.end() - 1)? ", " : "");
    }
    impl_ << ") {" << std::endl;

    impl_ << R"IMPL(    // search for free insert position
    size_t insert_pos = this->first_free_pos;
    for (; insert_pos < this->deleted_tuples.size(); insert_pos++) {
        if (this->deleted_tuples[insert_pos]) {
            break;
        }
    }
    // Now we know that the next free slot is definitely not earlier than the current position
    this->first_free_pos = insert_pos + 1;

)IMPL";

    impl_ << "    if (insert_pos == deleted_tuples.size()) {" << std::endl;
    impl_ << "        // insert new values at the end and increase vectors" << std::endl;
    for (auto& column : table.columns) {
        impl_ << "        this->" << column.id << ".push_back(" << column.id << ");" << std::endl;
    }
    impl_ << "        this->deleted_tuples.push_back(false);" << std::endl;
    impl_ << "    } else {" << std::endl;
    impl_ << "        // we found a free spot and can insert the new tuple there" << std::endl;
    for (auto& column : table.columns) {
        impl_ << "        this->" << column.id << "[insert_pos] = " << column.id << ";" << std::endl;
    }
    impl_ << "        this->deleted_tuples[insert_pos] = false;" << std::endl;
    impl_ << "    }" << std::endl;
    impl_ << std::endl;



    // update primary key index if exists
    if (table.primary_key.size() > 0) {
        impl_ << "    " << "this->primary_key[Key(";
        for (auto& column : table.primary_key) {
            impl_ << column.id << ((&column != &*table.primary_key.end() - 1)? ", " : "");
        }
        impl_ << ")] = insert_pos;" << std::endl;
        impl_ << std::endl;
    }
    impl_ << "    this->size++;" << std::endl;
    impl_ << "    return insert_pos;" << std::endl;
    impl_ << "}" << std::endl;
}

void generateRemoveMethod(Table &table, std::ostream& impl_) {
    impl_ << "void " << table.id << "Table" << "::" << "remove(const uint64_t tid) {" << std::endl;

    // Of course, we need to update all indexes, but for simplicity, we will only care about primary indexes here
    if (table.primary_key.size() > 0) {
        impl_ << "    // The table has a primary index we need to update." << std::endl;
        for (auto& key_column : table.primary_key) {
            impl_ << "    auto " << key_column.id << "_key = " << key_column.id << "[tid];" << std::endl;
        }
        impl_ << std::endl;
        impl_ << "    this->primary_key.erase(Key(";
        for (auto& key_column : table.primary_key) {
            impl_ << key_column.id << "_key" << ((&key_column != &*table.primary_key.end() - 1)? ", " : "");
        }
        impl_ << "));" << std::endl;
    }

    // Now remove the tuple
    // TODO: we could also shrink the vectors if the deletion happened at the end
    impl_ << "    this->deleted_tuples[tid] = true;" << std::endl;
    impl_ << "    this->size--;" << std::endl;
    impl_ << std::endl;
    impl_ << "    // Update first_free_pos to speed up following inserts" << std::endl;
    impl_ << "    if (tid < this->first_free_pos) {" << std::endl;
    impl_ << "        this->first_free_pos = tid;" << std::endl;
    impl_ << "    }" << std::endl;
    impl_ << "}" << std::endl;
    impl_ << std::endl;
}

void generateGetMethods(Table &table, std::ostream& impl_) {
    for (auto& column : table.columns) {
        impl_ << "std::optional<" << generateTypeName(column.type) << "> " << table.id << "Table" << "::" << "get_" << column.id << "(const uint64_t tid) {" << std::endl;
        impl_ << "    if (this->deleted_tuples[tid] == true) {" << std::endl;
        impl_ << "        return std::nullopt;" << std::endl;
        impl_ << "    }" << std::endl;
        impl_ << "    return this->" << column.id << "[tid];" << std::endl;
        impl_ << "}" << std::endl;
        impl_ << std::endl;
    }
}

void generateUpdateMethods(Table &table, std::ostream& impl_) {
    for (auto& column : table.columns) {
        // Method header
        impl_ << "void " << table.id << "Table" << "::" << "update_" << column.id << "(const uint64_t tid, const "
              << generateTypeName(column.type) << " " << column.id << ") {" << std::endl;

        // If the field is not part of any indexes, it's super easy for us. Otherwise, we need to update the index as well.
        // For simplicity, we will only care about primary keys for now.
        if (std::find_if(table.primary_key.begin(), table.primary_key.end(), [&](const Column& c) { return c.id.compare(column.id) == 0; }) == table.primary_key.end()) {
            impl_ << "    // Because the attribute is not part of any indexes, we can simply overwrite it" << std::endl;
            impl_ << "    this->" << column.id << "[tid] = " << column.id << ";" << std::endl;
        } else {
            impl_ << "    // The attribute is part of an index. Thus, we need to update the index as well." << std::endl;
            for (auto& key_column : table.primary_key) {
                impl_ << "    auto " << key_column.id << "_key = this->" << key_column.id << "[tid];" << std::endl;
            }
            impl_ << std::endl;
            impl_ << "    this->primary_key.erase(Key(";
            for (auto& key_column : table.primary_key) {
                impl_ << key_column.id << "_key" << ((&key_column != &*table.primary_key.end() - 1)? ", " : "");
            }
            impl_ << "));" << std::endl;
            impl_ << std::endl;
            impl_ << "    // Update value" << std::endl;
            impl_ << "    this->" << column.id << "[tid] = " << column.id << ";" << std::endl;
            impl_ << "    " << column.id << "_key = " << column.id << ";" << std::endl;
            impl_ << std::endl;
            impl_ << "    // update index on primary key" << std::endl;
            impl_ << "    this->primary_key[Key(";
            for (auto& key_column : table.primary_key) {
                impl_ << key_column.id << "_key" << ((&key_column != &*table.primary_key.end() - 1)? ", " : "");
            }
            impl_ << ")] = tid;" << std::endl;
        }
        impl_ << "}" << std::endl;
        impl_ << std::endl;
    }
}

void generateTableSource(Table &table, std::ostream& impl_) {
    impl_ << "// ------------------------------------------------" << std::endl;
    impl_ << "// Generated sources for table " << table.id << std::endl;
    impl_ << "// ------------------------------------------------" << std::endl;

    generateInsertMethod(table, impl_);
    impl_ << std::endl;

    generateRemoveMethod(table, impl_);
    impl_ << std::endl;

    generateGetMethods(table, impl_);
    impl_ << std::endl;

    generateUpdateMethods(table, impl_);
    impl_ << std::endl;
}

void SchemaCompiler::createSource(Schema &schema) {
    impl_ << R"IMPL(
#include "imlab/schema.h"

namespace imlab {
namespace tpcc {

)IMPL";

    for (auto& table : schema.tables) {
        generateTableSource(table, impl_);
        impl_ << std::endl;
    }

    impl_ << R"IMPL(
}  // namespace tpcc
}  // namespace imlab
)IMPL";
}
// ---------------------------------------------------------------------------------------------------
