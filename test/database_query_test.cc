// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <sstream>
#include <fstream>
#include "imlab/test/data.h"
#include "imlab/database.h"
#include "gtest/gtest.h"
#include "imlab/schema.h"

namespace {

TEST(DatabaseQueryTest, NewOrderQuery) {
    imlab::Database db;
    std::stringstream in_warehouse(imlab_test::kTestWarehouse);
    db.LoadWarehouse(in_warehouse);
    std::stringstream in_customer(imlab_test::kTestCustomer);
    db.LoadCustomer(in_customer);
    std::stringstream in_district(imlab_test::kTestDistrict);
    db.LoadDistrict(in_district);
    std::stringstream in_item(imlab_test::kTestItem);
    db.LoadItem(in_item);
    std::stringstream in_stock(imlab_test::kTestStock);
    db.LoadStock(in_stock);

    auto w_id = Integer(4);
    auto d_id = Integer(1);
    auto c_id = Integer(1);
    auto items = Integer(1);
    auto supware = std::array<Integer, 15>{ Integer(4) };
    auto itemid = std::array<Integer, 15>{ Integer(1) };
    auto qty = std::array<Integer, 15>{ Integer(20) };
    auto datetime = Timestamp(123456789);

    db.NewOrder(w_id, d_id, c_id, items, supware, itemid, qty, datetime);

    ASSERT_EQ(db.Size<imlab::tpcc::korder>(), 1);
    ASSERT_EQ(db.Size<imlab::tpcc::kneworder>(), 1);
    ASSERT_EQ(db.Size<imlab::tpcc::korderline>(), items.value);
}

// TODO adjust test data to make this work
TEST(DISABLED_DatabaseQueryTest, Delivery) {
    imlab::Database db;
    std::stringstream in_neworder(imlab_test::kTestNewOrder);
    db.LoadNewOrder(in_neworder);
    std::stringstream in_order(imlab_test::kTestOrder);
    db.LoadOrder(in_order);
    std::stringstream in_orderline(imlab_test::kTestOrderLine);
    db.LoadOrderLine(in_orderline);
    std::stringstream in_customer(imlab_test::kTestCustomer);
    db.LoadCustomer(in_customer);

    auto w_id = Integer(4);
    auto o_carrier_id = Integer(999);
    auto datetime = Timestamp(123456789);

    db.Delivery(w_id, o_carrier_id, datetime);

    ASSERT_EQ(db.Size<imlab::tpcc::kneworder>(), 3);
}

// Test depends on real data -> not executed in CI
TEST(DatabaseQueryTest, AnalyticalQuerySTL) {
    imlab::Database db;
    std::fstream customer_file("../data/tpcc_5w/tpcc_customer.tbl", std::fstream::in);
    db.LoadCustomer(customer_file);
    std::fstream order_file("../data/tpcc_5w/tpcc_order.tbl", std::fstream::in);
    db.LoadOrder(order_file);
    std::fstream orderline_file("../data/tpcc_5w/tpcc_orderline.tbl", std::fstream::in);
    db.LoadOrderLine(orderline_file);

    auto sum = db.AnalyticalQuerySTL();

    auto result = Numeric<12, 2>(136787200125);
    ASSERT_EQ(sum, result);
}

// Test depends on real data -> not executed in CI
TEST(DatabaseQueryTest, AnalyticalQueryLHT) {
    imlab::Database db;
    std::fstream customer_file("../data/tpcc_5w/tpcc_customer.tbl", std::fstream::in);
    db.LoadCustomer(customer_file);
    std::fstream order_file("../data/tpcc_5w/tpcc_order.tbl", std::fstream::in);
    db.LoadOrder(order_file);
    std::fstream orderline_file("../data/tpcc_5w/tpcc_orderline.tbl", std::fstream::in);
    db.LoadOrderLine(orderline_file);

    auto sum = db.AnalyticalQueryLHT();

    auto result = Numeric<12, 2>(136787200125);
    ASSERT_EQ(sum, result);
}

}  // namespace
