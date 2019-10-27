// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
%skeleton "lalr1.cc"
%require "3.0.4"
// ---------------------------------------------------------------------------------------------------
// Write a parser header file
%defines
// Define the parser class name
%define parser_class_name {SchemaParser}
// Create the parser in our namespace
%define api.namespace { imlab::schemac }
// Use C++ variant to store the values and get better type warnings (compared to "union")
%define api.value.type variant
// With variant-based values, symbols are handled as a whole in the scanner
%define api.token.constructor
// Prefix all tokens
%define api.token.prefix {SCHEMA_}
// Check if variants are constructed and destroyed properly
%define parse.assert
// Trace the parser
%define parse.trace
// Use verbose parser errors
%define parse.error verbose
// Enable location tracking.
%locations
// Pass the compiler as parameter to yylex/yyparse.
%param { imlab::schemac::SchemaParseContext &sc }
// ---------------------------------------------------------------------------------------------------
// Added to the header file and parser implementation before bison definitions.
// We include string for string tokens and forward declare the SchemaParseContext.
%code requires {
#include <string>
#include "imlab/schemac/schema_parse_context.h"
}
// ---------------------------------------------------------------------------------------------------
// Import the compiler header in the implementation file
%code {
imlab::schemac::SchemaParser::symbol_type yylex(imlab::schemac::SchemaParseContext& sc);
}
// ---------------------------------------------------------------------------------------------------
// Token definitions
%token SEMICOLON        "semicolon"
%token COMMA            "comma"
%token LCB              "left_curly_brackets"
%token RCB              "right_curly_brackets"
%token LPAR             "left_parentheses"
%token RPAR             "right_parentheses"
%token EQUAL            "equal"
%token INTEGER          "integer"
%token TIMESTAMP        "timestamp"
%token NUMERIC          "numeric"
%token CHAR             "char"
%token VARCHAR          "varchar"
%token NOTNULL          "not_null"
%token CREATE           "create"
%token TABLE            "table"
%token WITH             "with"
%token KEY_INDEX_TYPE   "key_index_type"
%token INDEX_TYPE_UNORDERED_MAP         "index_type_unordered_map"
%token INDEX_TYPE_STL_MAP               "index_type_stl_map"
%token INDEX_TYPE_STX_MAP               "index_type_stx_map"
%token PRIMARY_KEY      "primary_key"
%token <int>            INTEGER_VALUE    "integer_value"
%token <std::string>    IDENTIFIER       "identifier"
%token EOF 0            "eof"
// ---------------------------------------------------------------------------------------------------
%type <imlab::schemac::Schema> schema;
%type <imlab::schemac::Table> table;
%type <std::vector<imlab::schemac::Table>> table_list;
%type <imlab::schemac::Column> column;
%type <std::vector<imlab::schemac::Column>> column_list;
%type <std::vector<std::string>> primary_key;
%type <imlab::schemac::IndexType> with_key_index_type;
%type <imlab::schemac::IndexType> key_index_type;
%type <imlab::schemac::Index> index;
%type <std::vector<imlab::schemac::Index>> index_list;
%type <imlab::schemac::Type> type;
%type <std::vector<std::string>> identifier_list;
// ---------------------------------------------------------------------------------------------------
%%

%start create_schema;

create_schema:
    schema                                              { sc.createSchema($1); }

schema:
    table_list                                          { $$ = imlab::schemac::Schema {$1}; }
 |  %empty                                              {}
    ;

table_list:
    table_list table                                    { $1.push_back($2); std::swap($$, $1); }
 |  table                                               { $$ = std::vector<imlab::schemac::Table> { $1 }; }
 |  %empty                                              {}
    ;

table:
    CREATE TABLE IDENTIFIER LPAR column_list primary_key RPAR with_key_index_type SEMICOLON         { $$ = sc.createTable($3, $5, $6, $8); }

column_list:
    column_list COMMA column                            { $1.push_back($3); std::swap($$, $1); }
 |  column                                              { $$ = std::vector<imlab::schemac::Column> { $1 }; }
 |  %empty                                              {}
    ;

column:
    IDENTIFIER type NOTNULL                             { $$ = imlab::schemac::Column {$1, $2}; }

primary_key:
    COMMA PRIMARY_KEY LPAR identifier_list RPAR         { $$ = $4; }
 |  %empty                                              {}

with_key_index_type:
    WITH LPAR KEY_INDEX_TYPE EQUAL key_index_type RPAR  { $$ = $5; }
 |  %empty                                              {}
    ;

key_index_type:
    INDEX_TYPE_UNORDERED_MAP                            { $$ = imlab::schemac::IndexType::kSTLUnorderedMap; }
 |  INDEX_TYPE_STL_MAP                                  { $$ = imlab::schemac::IndexType::kSTLMap; }
 |  INDEX_TYPE_STX_MAP                                  { $$ = imlab::schemac::IndexType::kSTXMap; }

index_list:
    index_list index                                    { $1.push_back($2); std::swap($$, $1); }
 |  index                                               { $$ = std::vector<imlab::schemac::Index> { $1 }; }
 |  %empty                                              {}
    ;

index:
    %empty                                              { $$ = imlab::schemac::Index {}; }

type:
    INTEGER                                             { $$ = Type::Integer(); }
 |  TIMESTAMP                                           { $$ = Type::Timestamp(); }
 |  NUMERIC LPAR INTEGER_VALUE COMMA INTEGER_VALUE RPAR { $$ = Type::Numeric($3, $5); }
 |  CHAR LPAR INTEGER_VALUE RPAR                        { $$ = Type::Char($3); }
 |  VARCHAR LPAR INTEGER_VALUE RPAR                     { $$ = Type::Varchar($3); }

identifier_list:
        identifier_list COMMA IDENTIFIER                { $1.push_back($3); std::swap($$, $1); }
 |  IDENTIFIER                                          { $$ = std::vector<std::string> { $1 }; }
 |  %empty                                              {}
    ;

%%
// ---------------------------------------------------------------------------------------------------
// Define error function
void imlab::schemac::SchemaParser::error(const location_type& l, const std::string& m) {
    sc.Error(l.begin.line, l.begin.column, m);
}
// ---------------------------------------------------------------------------------------------------

