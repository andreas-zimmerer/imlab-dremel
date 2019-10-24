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
%token <int>            INTEGER_VALUE    "integer_value"
%token <std::string>    IDENTIFIER       "identifier"
%token LCB              "left_curly_brackets"
%token RCB              "right_curly_brackets"
%token SEMICOLON        "semicolon"
%token INTEGER          "integer"
%token CHAR             "char"
%token COMMA            "comma"
%token FOO              "foo"
%token BAR              "bar"
%token EOF 0            "eof"
// ---------------------------------------------------------------------------------------------------
%type <std::vector<imlab::schemac::SomeDeclaration>> some_declaration_list;
%type <imlab::schemac::SomeDeclaration> some_declaration;
%type <imlab::schemac::Type> some_type;
// ---------------------------------------------------------------------------------------------------
%%

%start foo_statement_list;

foo_statement_list:
    foo_statement_list foo_statment
 |  %empty
    ;

foo_statment:
    FOO IDENTIFIER LCB some_declaration_list RCB SEMICOLON         { sc.defineFoo($2, $4); }
    ;

some_declaration_list:
    some_declaration_list COMMA some_declaration        { $1.push_back($3); std::swap($$, $1); }
 |  some_declaration                                    { $$ = std::vector<imlab::schemac::SomeDeclaration> { $1 }; }
 |  %empty                                              {}
    ;

some_declaration:
    IDENTIFIER some_type                                { $$ = imlab::schemac::SomeDeclaration($1, $2); }

some_type:
    INTEGER                                             { $$ = Type::Integer(); }
 |  CHAR LCB INTEGER_VALUE RCB                          { $$ = Type::Char($3); }

%%
// ---------------------------------------------------------------------------------------------------
// Define error function
void imlab::schemac::SchemaParser::error(const location_type& l, const std::string& m) {
    sc.Error(l.begin.line, l.begin.column, m);
}
// ---------------------------------------------------------------------------------------------------

