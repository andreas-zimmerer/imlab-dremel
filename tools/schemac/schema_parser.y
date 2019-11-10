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
%token QUOTE            "quote"
%token INTEGER          "integer"
%token TIMESTAMP        "timestamp"
%token NUMERIC          "numeric"
%token CHAR             "char"
%token VARCHAR          "varchar"
%token NOTNULL          "not_null"
%token CREATE           "create"
%token TABLE            "table"
%token INDEX            "index"
%token ON               "on"
%token WITH             "with"
%token KEY_INDEX_TYPE   "key_index_type"
%token INDEX_TYPE       "index_type"
%token INDEX_TYPE_UNORDERED_MAP         "index_type_unordered_map"
%token INDEX_TYPE_STL_MAP               "index_type_stl_map"
%token INDEX_TYPE_STX_MAP               "index_type_stx_map"
%token PRIMARY_KEY      "primary_key"
%token <int>            INTEGER_VALUE    "integer_value"
%token <std::string>    IDENTIFIER       "identifier"
%token EOF 0            "eof"
// ---------------------------------------------------------------------------------------------------
%type <imlab::schemac::Column> column;
%type <std::vector<imlab::schemac::Column>> column_list;
%type <std::vector<std::string>> primary_key;
%type <imlab::schemac::IndexType> with_index_type;
%type <imlab::schemac::IndexType> index_type;
%type <imlab::schemac::Type> type;
%type <std::vector<std::string>> identifier_list;
%type <std::string> identifier;
// ---------------------------------------------------------------------------------------------------
%%

%start schema;

schema:
    sql_schema_statement_list                               {}
    ;

sql_schema_statement_list:
    sql_schema_statement_list sql_schema_statement          {}
 |  sql_schema_statement                                    {}
 |  %empty                                                  {}
    ;

sql_schema_statement:
    create_table_statement                                  {}
 |  create_index_statement                                  {}
    ;

create_table_statement:
    CREATE TABLE identifier LPAR column_list primary_key RPAR SEMICOLON                             { sc.createTable($3, $5, $6); }
 |  CREATE TABLE identifier LPAR column_list primary_key RPAR with_index_type SEMICOLON             { sc.createTable($3, $5, $6, $8); }
    ;

create_index_statement:
    CREATE INDEX identifier ON identifier LPAR identifier_list RPAR SEMICOLON                       { sc.createIndex($3, $5, $7); }
 |  CREATE INDEX identifier ON identifier LPAR identifier_list RPAR with_index_type SEMICOLON       { sc.createIndex($3, $5, $7, $9); }
    ;

column_list:
    column_list COMMA column                            { $1.push_back($3); std::swap($$, $1); }
 |  column                                              { $$ = std::vector<imlab::schemac::Column> { $1 }; }
 |  %empty                                              {}
    ;

column:
    identifier type NOTNULL                             { $$ = imlab::schemac::Column {$1, $2}; }
    ;

primary_key:
    COMMA PRIMARY_KEY LPAR identifier_list RPAR         { $$ = $4; }
 |  %empty                                              {}
    ;

with_index_type:
    WITH LPAR KEY_INDEX_TYPE EQUAL index_type RPAR  { $$ = $5; }
 |  WITH LPAR INDEX_TYPE EQUAL index_type RPAR  { $$ = $5; }
    ;

index_type:
    INDEX_TYPE_UNORDERED_MAP                            { $$ = imlab::schemac::IndexType::kSTLUnorderedMap; }
 |  INDEX_TYPE_STL_MAP                                  { $$ = imlab::schemac::IndexType::kSTLMap; }
 |  INDEX_TYPE_STX_MAP                                  { $$ = imlab::schemac::IndexType::kSTXMap; }
    ;

type:
    INTEGER                                             { $$ = Type::Integer(); }
 |  TIMESTAMP                                           { $$ = Type::Timestamp(); }
 |  NUMERIC LPAR INTEGER_VALUE COMMA INTEGER_VALUE RPAR { $$ = Type::Numeric($3, $5); }
 |  CHAR LPAR INTEGER_VALUE RPAR                        { $$ = Type::Char($3); }
 |  VARCHAR LPAR INTEGER_VALUE RPAR                     { $$ = Type::Varchar($3); }
    ;

identifier_list:
    identifier_list COMMA identifier                    { $1.push_back($3); std::swap($$, $1); }
 |  identifier                                          { $$ = std::vector<std::string> { $1 }; }
 |  %empty                                              {}
    ;

identifier:
    QUOTE IDENTIFIER QUOTE                              { $$ = $2; }
 |  IDENTIFIER                                          { $$ = $1; }
    ;

%%
// ---------------------------------------------------------------------------------------------------
// Define error function
void imlab::schemac::SchemaParser::error(const location_type& l, const std::string& m) {
    sc.Error(l.begin.line, l.begin.column, m);
}
// ---------------------------------------------------------------------------------------------------

