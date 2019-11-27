// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <sstream>
#include "imlab/algebra/table_scan.h"
#include "imlab/algebra/inner_join.h"
#include "imlab/algebra/selection.h"
#include "imlab/algebra/print.h"
#include "gtest/gtest.h"

using IU = imlab::IU;
using TableScan = imlab::TableScan;
using Selection = imlab::Selection;
using InnerJoin = imlab::InnerJoin;
using Print = imlab::Print;

namespace {

TEST(IUPropagation, CollectIUsTableScan) {
    TableScan s("neworder");

    const auto& ius = s.CollectIUs();

    ASSERT_EQ(std::string(ius[0]->column), "no_o_id");
    ASSERT_EQ(std::string(ius[1]->column), "no_d_id");
    ASSERT_EQ(std::string(ius[2]->column), "no_w_id");
}

TEST(IUPropagation, CollectIUsSelection) {
    TableScan s("neworder");
    std::vector<std::pair<const IU*, std::string>> predicates {std::make_pair(s.CollectIUs()[0], "foobar")};
    Selection sel (std::make_unique<TableScan>(s), predicates);

    const auto& ius = sel.CollectIUs();

    ASSERT_EQ(std::string(ius[0]->column), "no_o_id");
    ASSERT_EQ(std::string(ius[1]->column), "no_d_id");
    ASSERT_EQ(std::string(ius[2]->column), "no_w_id");
}

TEST(IUPropagation, CollectIUsTableJoin) {
    TableScan s1("neworder");
    TableScan s2("item");
    std::vector<std::pair<const IU*, const IU*>> predicates {std::make_pair(s1.CollectIUs()[0], s2.CollectIUs()[0])};
    InnerJoin j(std::make_unique<TableScan>(s1), std::make_unique<TableScan>(s2), predicates);

    const auto& ius = j.CollectIUs();

    ASSERT_EQ(ius.size(), s1.CollectIUs().size() + s2.CollectIUs().size());
}

TEST(IUPropagation, CollectIUsPrint) {
    TableScan s("neworder");
    Print p(std::make_unique<TableScan>(s));

    const auto& ius = p.CollectIUs();

    ASSERT_EQ(ius.size(), 0);
}

}  // namespace
