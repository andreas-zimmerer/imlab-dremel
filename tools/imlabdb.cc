// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#include <chrono>  // NOLINT
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "database.h"
#include "imlab/schemac/schema_parse_context.h"
#include "imlab/queryc/query_parse_context.h"
#include "imlab/queryc/query_compiler.h"
// ---------------------------------------------------------------------------
using SchemaParseContext = imlab::schemac::SchemaParseContext;
using QueryParseContext = imlab::queryc::QueryParseContext;
using QueryCompiler = imlab::queryc::QueryCompiler;
// ---------------------------------------------------------------------------

imlab::Database loadDatabase() {
    imlab::Database database{};

    std::fstream dremel_file("../data/dremel/dremel_data.json", std::fstream::in);
    database.LoadDocumentTable(dremel_file);

    return database;
}

int main(int argc, char *argv[]) {
    // Load schema and database content

    auto load_schema_begin = std::chrono::steady_clock::now();
    std::cout << "Loading schema information..." << std::flush;
    std::ifstream schema_file("../data/schema.sql");
    SchemaParseContext schema_parse_context;
    auto schema = schema_parse_context.Parse(schema_file);
    auto load_schema_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - load_schema_begin).count();
    std::cout << " [done in " << load_schema_duration << " ms]" << std::endl;

    auto load_db_begin = std::chrono::steady_clock::now();
    std::cout << "Loading database..." << std::flush;
    auto db = loadDatabase();
    auto load_db_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - load_db_begin).count();
    std::cout << " [done in " << load_db_duration << " ms]" << std::endl;

    // Prepare sql query parser
    QueryParseContext query_parse_context {schema};

    // Starting REPL
    std::cout << "Starting SQL interpreter - close with Ctrl+D" << std::endl;
    std::cout << "To enable statistics, enter \"enable_stats\"" << std::endl;
    std::cout << ">>> " << std::flush;
    bool enable_stats = false;
    std::string line{};
    while (std::getline(std::cin, line)) {
        if (line == "enable_stats") {
            enable_stats = true;
        } else {
            try {
                //---------------------------------------------------------------------------------------
                auto parse_query_begin = std::chrono::steady_clock::now();

                std::istringstream in_stream(line);
                auto &query = query_parse_context.Parse(in_stream);

                auto parse_query_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::steady_clock::now() - parse_query_begin).count();
                //---------------------------------------------------------------------------------------

                auto stats = db.RunQuery(query);

                if (enable_stats) {
                    std::cout << "-----" << std::endl;
                    std::cout << "Parsing SQL:     " << parse_query_duration << " ms" << std::endl;
                    std::cout << "Generating code: " << stats.code_generation_duration << " ms" << std::endl;
                    std::cout << "Compiling query: " << stats.code_compilation_duration << " ms" << std::endl;
                    std::cout << "Query execution: " << stats.query_execution_duration << " ms" << std::endl;
                }
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }

        // Print prompt for next input
        std::cout << ">>> " << std::flush;
    }
    return 0;
}
