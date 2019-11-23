// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------

#include <signal.h>
#include <atomic>
#include <cstdint>
#include <chrono>  // NOLINT
#include <iostream>
#include <fstream>
#include "benchmark/benchmark.h"
#include "imlab/database.h"
#include "imlab/schema.h"
#include "imlab/infra/types.h"
#include <unistd.h>
#include <sys/wait.h>

namespace {
const int32_t kWarehouses = 5;

// Uniform random number
int32_t URand(int32_t min, int32_t max) {
    return (random() % (max - min + 1)) + min;
}

// Uniform random number
int32_t URandExcept(int32_t min, int32_t max, int32_t v) {
    if (max <= min)
        return min;
    int32_t r = (random() % (max - min)) + min;
    return (r >= v) ? (r + 1) : r;
}

// Non-uniform random number
int32_t NURand(int32_t A, int32_t x, int32_t y) {
    return ((((random() % A) | (random() % (y - x + 1) + x)) + 42) % (y - x + 1)) + x;
}

// Place a random order
void RandomNewOrder(imlab::Database &db) {
    Timestamp now(0);
    auto w_id = Integer(URand(1, kWarehouses));
    auto d_id = Integer(URand(1, 10));
    auto c_id = Integer(NURand(1023, 1, 3000));
    auto ol_cnt = Integer(URand(5, 15));

    std::array<Integer, 15> supware;
    std::array<Integer, 15> itemid;
    std::array<Integer, 15> qty;
    for (auto i = Integer(0); i < ol_cnt; i += Integer(1)) {
        supware[i.value] = (URand(1, 100) > 1) ? w_id : Integer(URandExcept(1, kWarehouses, w_id.value));
        itemid[i.value] = Integer(NURand(8191, 1, 100000));
        qty[i.value] = Integer(URand(1, 10));
    }

    db.NewOrder(w_id, d_id, c_id, ol_cnt, supware, itemid, qty, now);
}

// Place a random delivery
void RandomDelivery(imlab::Database &db) {
    Timestamp now(0);
    db.Delivery(Integer(URand(1, kWarehouses)), Integer(URand(1, 10)), now);
}

imlab::Database LoadDatabase() {
    imlab::Database database;

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

//---------------------------------------------------------------------------
void RandomMixNeworderDeliveryBenchmark(benchmark::State &state) {
    auto database = LoadDatabase();

    database.Print();
    std::cout.flush();

    unsigned number_of_delivery = 0;
    unsigned number_of_neworder = 0;

    for (auto _ : state) {
        if (URand(1, 100) <= 10) {
            RandomDelivery(database);
            number_of_delivery++;
        } else {
            RandomNewOrder(database);
            number_of_neworder++;
        }
    }

    // Statistics
    state.SetItemsProcessed(state.iterations());

    database.Print();
    std::cout.flush();

    std::cout << std::endl;
    std::cout << "Number of Delivery Queries: " << number_of_delivery << std::endl;
    std::cout << "Number of Neworder Queries: " << number_of_neworder << std::endl;
    std::cout.flush();
}

//---------------------------------------------------------------------------

std::atomic<bool> olapRunning;

static void SIGCHLD_handler(int /*sig*/) {
    int status;
    pid_t childPid = wait(&status);
    // now the child with process id childPid is dead
    olapRunning = false;
}

void RandomMixNeworderDeliveryBenchmarkWithOlap(benchmark::State &state) {
    auto database = LoadDatabase();

    unsigned number_of_delivery = 0;
    unsigned number_of_neworder = 0;
    unsigned number_of_olap = 0;

    for (auto _ : state) {
        if (URand(1, 100) <= 10) {
            RandomDelivery(database);
            number_of_delivery++;
        } else {
            RandomNewOrder(database);
            number_of_neworder++;
        }
    }

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = SIGCHLD_handler;
    sigaction(SIGCHLD, &sa, NULL);

    // This loop contains all the magic:
    // The loop itself is executed n times (where is is supposed to be 1.000.000)
    // Each iteration corresponds to the execution of one OLTP query.
    // Meanwhile, if no OLAP query is running, a new snapshot is created with "fork()"
    // and the OLAP query runs in the child process.
    // Execution of the OLTP queries just continues in the parent.
    for (auto _ : state) {
        // Whenever we notice that the OLAP query is done,
        // we create a new one until all of our OLTP-Mix queries are done as well.
        if (!olapRunning) {
            olapRunning = true;
            number_of_olap++;
            pid_t pid = fork();
            if (pid) { // parent
                // parent just continues
            } else { // forked child
                // The child runs the analytical query.
                // There should always only be one child with one analytical query.
                auto result = database.AnalyticalQuerySTL();
                std::cout << "Result of OLAP query: " << result << std::endl;
                return; // child is finished
            }
        }

        // Run one OLTP query from the mix in the parent
        if (URand(1, 100) <= 10) {
            RandomDelivery(database);
            number_of_delivery++;
        } else {
            RandomNewOrder(database);
            number_of_neworder++;
        }
    }

    // Statistics
    state.SetItemsProcessed(state.iterations());

    std::cout << std::endl;
    std::cout << "Number of Delivery Queries: " << number_of_delivery << std::endl;
    std::cout << "Number of Neworder Queries: " << number_of_neworder << std::endl;
    std::cout << "Number of OLAP Queries:     " << number_of_olap << std::endl;
    std::cout.flush();
}
//---------------------------------------------------------------------------
}  // namespace

BENCHMARK(RandomMixNeworderDeliveryBenchmark)
->Iterations(10000);
BENCHMARK(RandomMixNeworderDeliveryBenchmarkWithOlap)
->Iterations(10000);

int main(int argc, char** argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
