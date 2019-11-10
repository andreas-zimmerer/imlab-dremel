// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <sstream>
#include "imlab/test/data.h"
#include "imlab/database.h"
#include "gtest/gtest.h"
#include "imlab/schema.h"

namespace {

TEST(DatabaseLoadingTest, LoadWarehouse) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestWarehouse);
    db.LoadWarehouse(in);
    ASSERT_EQ(db.Size<imlab::tpcc::kwarehouse>(), 5);
}

TEST(DatabaseLoadingTest, LoadDistrict) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestDistrict);
    db.LoadDistrict(in);
    ASSERT_EQ(db.Size<imlab::tpcc::kdistrict>(), 4);
}

TEST(DatabaseLoadingTest, LoadHistory) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestHistory);
    db.LoadHistory(in);
    ASSERT_EQ(db.Size<imlab::tpcc::khistory>(), 4);
}

TEST(DatabaseLoadingTest, LoadItem) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestItem);
    db.LoadItem(in);
    ASSERT_EQ(db.Size<imlab::tpcc::kitem>(), 4);
}

TEST(DatabaseLoadingTest, LoadNewOrder) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestNewOrder);
    db.LoadNewOrder(in);
    ASSERT_EQ(db.Size<imlab::tpcc::kneworder>(), 4);
}

TEST(DatabaseLoadingTest, LoadOrder) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestOrder);
    db.LoadOrder(in);
    ASSERT_EQ(db.Size<imlab::tpcc::korder>(), 4);
}

TEST(DatabaseLoadingTest, LoadOrderLine) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestOrderLine);
    db.LoadOrderLine(in);
    ASSERT_EQ(db.Size<imlab::tpcc::korderline>(), 4);
}

TEST(DatabaseLoadingTest, LoadStock) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestStock);
    db.LoadStock(in);
    ASSERT_EQ(db.Size<imlab::tpcc::kstock>(), 4);
}

TEST(DatabaseLoadingTest, LoadCustomer) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestCustomer);
    db.LoadCustomer(in);
    ASSERT_EQ(db.Size<imlab::tpcc::kcustomer>(), 2);
}

}  // namespace
