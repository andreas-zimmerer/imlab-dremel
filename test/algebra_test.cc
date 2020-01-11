// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <sstream>
#include "imlab/algebra/table_scan.h"
#include "imlab/algebra/inner_join.h"
#include "imlab/algebra/selection.h"
#include "imlab/algebra/print.h"
#include "gtest/gtest.h"

using TableScan = imlab::TableScan;
using Selection = imlab::Selection;
using InnerJoin = imlab::InnerJoin;
using Print = imlab::Print;

namespace {
/*
TEST(IUPropagation, CollectIUsTableScan) {
    TableScan s("Document");

    const auto& ius = s.CollectFields();

    ASSERT_EQ(std::string(ius[0]), "DocId");
    ASSERT_EQ(std::string(ius[1]), "Links.Backward");
    ASSERT_EQ(std::string(ius[2]), "Links.Forward");
}

TEST(IUPropagation, CollectIUsSelection) {
    TableScan s("Document");
    std::vector<std::pair<const IU*, std::string>> predicates {std::make_pair(s.CollectIUs()[0], "foobar")};
    Selection sel(std::make_unique<TableScan>(s), predicates);

    const auto& ius = sel.CollectIUs();

    ASSERT_EQ(std::string(ius[0]->column), "DocId");
}

TEST(IUPropagation, CollectIUsTableJoin) {
    TableScan s1("Document");
    TableScan s2("Document");
    std::vector<std::pair<const IU*, const IU*>> predicates {std::make_pair(s1.CollectIUs()[0], s2.CollectIUs()[0])};
    InnerJoin j(std::make_unique<TableScan>(s1), std::make_unique<TableScan>(s2), predicates);

    const auto& ius = j.CollectIUs();

    ASSERT_EQ(ius.size(), s1.CollectIUs().size() + s2.CollectIUs().size());
}

TEST(IUPropagation, CollectIUsPrint) {
    TableScan s("Document");
    Print p(std::make_unique<TableScan>(s));

    const auto& ius = p.CollectIUs();

    ASSERT_EQ(ius.size(), 0);
}
*/
}  // namespace
