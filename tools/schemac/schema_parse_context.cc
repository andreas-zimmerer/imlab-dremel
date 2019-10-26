// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#include "imlab/schemac/schema_parse_context.h"
#include "./gen/schema_parser.h"
#include <sstream>
#include <unordered_set>
#include "imlab/infra/error.h"
// ---------------------------------------------------------------------------------------------------
using SchemaParseContext = imlab::schemac::SchemaParseContext;
using Table = imlab::schemac::Table;
using Schema = imlab::schemac::Schema;
using Type = imlab::schemac::Type;
// ---------------------------------------------------------------------------------------------------
Type Type::Integer()    { Type t; t.tclass = kInteger; return t; }
Type Type::Timestamp()  { Type t; t.tclass = kTimestamp; return t; }
Type Type::Numeric(unsigned length, unsigned precision) {
    Type t;
    t.tclass = kNumeric;
    t.length = length;
    t.precision = precision;
    return t;
}
Type Type::Char(unsigned length) {
    Type t;
    t.tclass = kChar;
    t.length = length;
    return t;
}
Type Type::Varchar(unsigned length) {
    Type t;
    t.tclass = kVarchar;
    t.length = length;
    return t;
}
// ---------------------------------------------------------------------------------------------------
const char *Type::Name() const {
    switch (tclass) {
        case kInteger:      return "Integer";
        case kTimestamp:    return "Timestamp";
        case kNumeric:      return "Numeric";
        case kChar:         return "Character";
        case kVarchar:      return "Varchar";
        default:            return "Unknown";
    }
}
// ---------------------------------------------------------------------------------------------------
// Constructor
SchemaParseContext::SchemaParseContext(bool trace_scanning, bool trace_parsing)
    : trace_scanning_(trace_scanning), trace_parsing_(trace_parsing) {}
// ---------------------------------------------------------------------------------------------------
// Destructor
SchemaParseContext::~SchemaParseContext() {}
// ---------------------------------------------------------------------------------------------------
// Parse a string
Schema SchemaParseContext::Parse(std::istream &in) {
    beginScan(in);
    imlab::schemac::SchemaParser parser(*this);
    parser.set_debug_level(trace_parsing_);
    parser.parse();
    endScan();

    return this->schema;
}
// ---------------------------------------------------------------------------------------------------
// Yield an error
void SchemaParseContext::Error(const std::string& m) {
    throw SchemaCompilationError(m);
}
// ---------------------------------------------------------------------------------------------------
// Yield an error
void SchemaParseContext::Error(uint32_t line, uint32_t column, const std::string &err) {
    std::stringstream ss;
    ss << "[ l=" << line << " c=" << column << " ] " << err << std::endl;
    throw SchemaCompilationError(ss.str());
}
// ---------------------------------------------------------------------------------------------------
// Define a table
void SchemaParseContext::createSchema(const Schema &schema) {
    this->schema = schema;
}
// ---------------------------------------------------------------------------------------------------

