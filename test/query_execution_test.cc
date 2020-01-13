// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <sstream>
#include <fstream>
#include "database.h"
#include "gtest/gtest.h"
#include "gtest/gtest_prod.h"
#include <imlab/algebra/selection.h>
#include "imlab/algebra/table_scan.h"

namespace {
    using namespace imlab;
    using namespace imlab::dremel;

class QueryExecutionTest : public ::testing::Test {
 protected:
    void SetUp() override {
        system("python3 ../data/dremel/generate_dremel_data.py");
        std::fstream dremel_file("../data/dremel/data.json", std::fstream::in);
        imlab::Database::DecodeJson(dremel_file, [&](auto& d) {
            db.DocumentTable.insert(d);
            documents.push_back(d);
        });
    }

    imlab::Database db {};
    std::vector<Document> documents {};
};

TEST_F(QueryExecutionTest, FullScan) {
    TableScan scan("Document");
    Print print(std::make_unique<TableScan>(std::move(scan)));
    print.Prepare(imlab::schema::DocumentTable::fields(), nullptr);
    Query query {std::move(print)};

    db.RunQuery(query);
}

TEST_F(QueryExecutionTest, Selection) {
    TableScan scan("Document");
    Selection sel(std::make_unique<TableScan>(std::move(scan)), {});  // TODO: some IUs
    Print print(std::make_unique<Selection>(std::move(sel)));
    print.Prepare({imlab::schema::DocumentTable::fields()}, nullptr);
    Query query {std::move(print)};

    db.RunQuery(query);
}

}  // namespace
