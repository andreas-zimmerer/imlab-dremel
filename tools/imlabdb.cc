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
    auto load_db_end = std::chrono::steady_clock::now();
    auto load_db_duration = std::chrono::duration_cast<std::chrono::milliseconds>(load_db_end - load_db_begin).count();
    std::cout << " [done in " << load_db_duration << " ms]" << std::endl;

    // Starting REPL
    std::cout << "Starting SQL interpreter - close with Ctrl+D" << std::endl;
    std::cout << ">>> " << std::flush;
    std::string line {};
    while (std::getline(std::cin, line))
    {
        // TODO Interpret line
        // TODO Execute SQL statemnt

        // Print prompt for next input
        std::cout << ">>> " << std::flush;
    }
    return 0;
}
