// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
%skeleton "lalr1.cc"
%require "3.0.4"
// ---------------------------------------------------------------------------------------------------
// Write a parser header file
%defines
// Define the parser class name
%define parser_class_name {QueryParser}
// Create the parser in our namespace
%define api.namespace { imlab::queryc }
// Use C++ variant to store the values and get better type warnings (compared to "union")
%define api.value.type variant
// With variant-based values, symbols are handled as a whole in the scanner
%define api.token.constructor
// Prefix all tokens
%define api.token.prefix {QUERY_}
// Check if variants are constructed and destroyed properly
%define parse.assert
// Trace the parser
%define parse.trace
// Use verbose parser errors
%define parse.error verbose
// Enable location tracking.
%locations
// Pass the compiler as parameter to yylex/yyparse.
%param { imlab::queryc::QueryParseContext &sc }
// In order to prevent naming conflicts, the .l files contain a prefix parameter. Prefix needs to be set here accordingly.
%name-prefix "query"
// ---------------------------------------------------------------------------------------------------
// Added to the header file and parser implementation before bison definitions.
// We include string for string tokens and forward declare the QueryParseContext.
%code requires {
#include <string>
#include "imlab/queryc/query_parse_context.h"
}
// ---------------------------------------------------------------------------------------------------
// Import the compiler header in the implementation file
%code {
imlab::queryc::QueryParser::symbol_type yylex(imlab::queryc::QueryParseContext& sc);
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
%token SQUOTE           "squote"
%token SELECT           "select"
%token FROM             "from"
%token WHERE            "where"
%token AND              "and"
%token <int>            INTEGER_VALUE    "integer_value"
%token <std::string>    IDENTIFIER       "identifier"
%token <std::string>    STRING_VALUE     "string_value"
%token EOF 0            "eof"
// ---------------------------------------------------------------------------------------------------
%type <std::vector<std::string>> identifier_list;
%type <std::string> identifier;
%type <std::vector<std::pair<std::string, std::string>>> condition_list;
%type <std::pair<std::string, std::string>> condition;
%type <std::string> constant;
// ---------------------------------------------------------------------------------------------------
%%

%start sql_query;

sql_query:
    SELECT identifier_list FROM identifier_list                                 { sc.CreateSqlQuery($2, $4); }
 |  SELECT identifier_list FROM identifier_list SEMICOLON                       { sc.CreateSqlQuery($2, $4); }
 |  SELECT identifier_list FROM identifier_list WHERE condition_list            { sc.CreateSqlQuery($2, $4, $6); }
 |  SELECT identifier_list FROM identifier_list WHERE condition_list SEMICOLON  { sc.CreateSqlQuery($2, $4, $6); }
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

condition_list:
    condition_list AND condition                        { $1.push_back($3); std::swap($$, $1); }
 |  condition                                           { $$ = std::vector<std::pair<std::string, std::string>> { $1 }; }
 |  %empty                                              {}
    ;

condition:
    identifier EQUAL identifier                         { $$ = {$1, $3}; }
 |  identifier EQUAL constant                           { $$ = {$1, $3}; }
    ;

constant:
    STRING_VALUE                                        { $$ = $1; }
 |  INTEGER_VALUE                                       { $$ = $1; }
    ;

%%
// ---------------------------------------------------------------------------------------------------
// Define error function
void imlab::queryc::QueryParser::error(const location_type& l, const std::string& m) {
    sc.Error(l.begin.line, l.begin.column, m);
}
// ---------------------------------------------------------------------------------------------------

