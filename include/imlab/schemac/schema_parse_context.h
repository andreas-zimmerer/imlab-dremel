// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_SCHEMAC_SCHEMA_PARSE_CONTEXT_H_
#define INCLUDE_IMLAB_SCHEMAC_SCHEMA_PARSE_CONTEXT_H_
// ---------------------------------------------------------------------------------------------------
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
// ---------------------------------------------------------------------------------------------------
namespace imlab {
namespace schemac {
// ---------------------------------------------------------------------------------------------------
struct SchemaParser;
struct SchemaCompiler;
// ---------------------------------------------------------------------------------------------------
// A single type
// Required by test/schemac/schema_parser_test.cc
struct Type {
    // Type class
    enum Class: uint8_t {
        kInteger,
        kTimestamp,
        kNumeric,
        kChar,
        kVarchar,
    };
    // The type class
    Class tclass;
    // The type argument (if any)
    union {
        struct {
            uint32_t length;
            uint32_t precision;
        };
    };

    // Get type name
    const char *Name() const;

    // Static methods to construct a column
    static Type Integer();
    static Type Timestamp();
    static Type Numeric(unsigned length, unsigned precision);
    static Type Char(unsigned length);
    static Type Varchar(unsigned length);
};
// ---------------------------------------------------------------------------------------------------
// An index type
enum IndexType: uint8_t {
    kSTLUnorderedMap,
    kSTLMap,
    kSTXMap
};
// ---------------------------------------------------------------------------------------------------
// A single column
// Required by test/schemac/schema_parser_test.cc
struct Column {
    // Name of the column
    std::string id;
    // Type of the column
    Type type;
};
// ---------------------------------------------------------------------------------------------------
// A single table
// Required by test/schemac/schema_parser_test.cc
struct Table {
    // Name of the table
    std::string id;
    // Columns
    std::vector<Column> columns;
    // Primary key
    std::vector<Column> primary_key;
    // Index type
    IndexType index_type;
};
// ---------------------------------------------------------------------------------------------------
// A single index
// Required by test/schemac/schema_parser_test.cc
struct Index {
    // Name of the index
    std::string id;
    // Name of the indexed table
    std::string table_id;
    // Columns
    std::vector<Column> columns;
    // Index type
    IndexType index_type;
};
// ---------------------------------------------------------------------------------------------------
// A complete schema
// Required by test/schemac/schema_parser_test.cc
struct Schema {
    // Tables
    std::vector<Table> tables;
    // Indexes
    std::vector<Index> indexes;
};
// ---------------------------------------------------------------------------------------------------
// Schema parse context
class SchemaParseContext {
    friend SchemaParser;

 public:
    // Constructor
    explicit SchemaParseContext(bool trace_scanning = false, bool trace_parsing = false);
    // Destructor
    virtual ~SchemaParseContext();

    // Parse an istream
    Schema Parse(std::istream &in);

    // Throw an error
    void Error(uint32_t line, uint32_t column, const std::string &err);
    // Throw an error
    void Error(const std::string &m);

 private:
    // Begin a scan
    void beginScan(std::istream &in);
    // End a scan
    void endScan();

    // Only need for storing the result -> is there a nicer way?
    void createSchema(const Schema &schema);
    Schema schema;

    // create a table
    Table createTable(const std::string &id, std::vector<Column> &columns, std::vector<std::string> &primary_keys, IndexType &indexType);

    // Trace the scanning
    bool trace_scanning_;
    // Trace the parsing
    bool trace_parsing_;
};
// ---------------------------------------------------------------------------------------------------
}  // namespace schemac
}  // namespace imlab
// ---------------------------------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_SCHEMAC_SCHEMA_PARSE_CONTEXT_H_
// ---------------------------------------------------------------------------------------------------

