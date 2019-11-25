// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <cstdint>
#include <chrono>  // NOLINT
#include <iostream>
#include <fstream>
#include <chrono>
#include "imlab/database.h"
#include "imlab/schema.h"
#include "imlab/infra/types.h"
#include "../tools/queryc/gen/query.h"

imlab::Database loadDatabase() {
    imlab::Database database {};

    std::fstream warehouse_file ("../data/tpcc_5w/tpcc_warehouse.tbl", std::fstream::in);
    database.LoadWarehouse(warehouse_file);
    std::fstream district_file ("../data/tpcc_5w/tpcc_district.tbl", std::fstream::in);
    database.LoadDistrict(district_file);
    std::fstream customer_file ("../data/tpcc_5w/tpcc_customer.tbl", std::fstream::in);
    database.LoadCustomer(customer_file);
    std::fstream history_file ("../data/tpcc_5w/tpcc_history.tbl", std::fstream::in);
    database.LoadHistory(history_file);
    std::fstream neworder_file ("../data/tpcc_5w/tpcc_neworder.tbl", std::fstream::in);
    database.LoadNewOrder(neworder_file);
    std::fstream order_file ("../data/tpcc_5w/tpcc_order.tbl", std::fstream::in);
    database.LoadOrder(order_file);
    std::fstream orderline_file ("../data/tpcc_5w/tpcc_orderline.tbl", std::fstream::in);
    database.LoadOrderLine(orderline_file);
    std::fstream item_file ("../data/tpcc_5w/tpcc_item.tbl", std::fstream::in);
    database.LoadItem(item_file);
    std::fstream stock_file ("../data/tpcc_5w/tpcc_stock.tbl", std::fstream::in);
    database.LoadStock(stock_file);

    return database;
}

int main(int argc, char *argv[]) {
    std::cout << "Loading database..." << std::flush;
    auto load_db_begin = std::chrono::steady_clock::now();
    auto db = loadDatabase();
    auto load_db_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - load_db_begin).count();
    std::cout << " [done in " << load_db_duration << " ms]" << std::endl;

    // Starting REPL
    std::cout << "Starting SQL interpreter - close with Ctrl+D" << std::endl;
    std::cout << "To enable statistics, enter \"enable_stats\"" << std::endl;
    std::cout << ">>> " << std::flush;
    bool enable_stats = false;
    std::string line {};
    while (std::getline(std::cin, line))
    {
        if (line == "enable_stats") {
            enable_stats = true;
        } else {
            auto parse_query_begin = std::chrono::steady_clock::now();
            auto parse_query_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - parse_query_begin).count();

            auto code_generation_begin = std::chrono::steady_clock::now();
            auto code_generation_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - code_generation_begin).count();

            auto code_compilation_begin = std::chrono::steady_clock::now();
            auto code_compilation_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - code_compilation_begin).count();

            auto query_execution_begin = std::chrono::steady_clock::now();
            auto query_execution_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - query_execution_begin).count();

            if (enable_stats) {
                std::cout << "-----" << std::endl;
                std::cout << "Parsing SQL:     " << parse_query_duration << " ms" << std::endl;
                std::cout << "Generating code: " << code_generation_duration << " ms" << std::endl;
                std::cout << "Compiling query: " << code_compilation_duration << " ms" << std::endl;
                std::cout << "Query execution: " << query_execution_duration << " ms" << std::endl;
            }
        }

        // Print prompt for next input
        std::cout << ">>> " << std::flush;
    }
    return 0;
}
