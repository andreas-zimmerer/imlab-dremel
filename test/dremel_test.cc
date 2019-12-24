// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <sstream>
#include "imlab/test/data.h"
#include "database.h"
#include "imlab/infra/dremel.h"
#include "gtest/gtest.h"
#include "gtest/gtest_prod.h"

namespace {
using namespace imlab::dremel;

// Checking the contents of the private vectors and print contents of columns.
class TestClass : public imlab::schema::DocumentTable {
    FRIEND_TEST(DremelTest, ShreddingDocumentRecordSmall);
    FRIEND_TEST(DremelTest, ShreddingDocumentRecordLarge);

    template<typename T>
    void _print(DremelColumn<T> column) {
        std::cout << column.get_identifier() << std::endl;
        for (uint64_t tid = 0; tid < column.get_size(); tid++) {
            const auto& row = column.get(tid);
            if (row.value.has_value())
                std::cout << "| " << row.value.value() << " | " << row.repetition_level << " | " << row.definition_level << " |" << std::endl;
            else
                std::cout << "| " << "NULL" << " | " << row.repetition_level << " | " << row.definition_level << " |" << std::endl;
        }
        std::cout << std::endl;
    }

    void print() {
        _print(DocId);
        _print(Links_Backward);
        _print(Links_Forward);
        _print(Name_Language_Code);
        _print(Name_Language_Country);
        _print(Name_Url);
    }
};

// This test corresponds to the example from the Dremel paper in Figure 2 and Figure 3.
TEST(DremelTest, ShreddingDocumentRecordSmall) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestDocumentSmall);

    db.LoadDocumentTable(in);

    auto* tc = reinterpret_cast<TestClass*>(&db.documentTable);
    tc->print();

    ASSERT_EQ(db.documentTable.get_size(), 1);
    ASSERT_EQ(tc->DocId.get_size(), 1);
    ASSERT_EQ(tc->DocId.get(0), (DremelRow<Integer>{{Integer(20)}, 0, 0}));
    ASSERT_EQ(tc->Links_Backward.get_size(), 2);
    ASSERT_EQ(tc->Links_Backward.get(0), (DremelRow<Integer>{{Integer(10)}, 0, 2}));
    ASSERT_EQ(tc->Links_Backward.get(1), (DremelRow<Integer>{{Integer(30)}, 1, 2}));
    ASSERT_EQ(tc->Links_Forward.get_size(), 1);
    ASSERT_EQ(tc->Links_Forward.get(0), (DremelRow<Integer>{{Integer(80)}, 0, 2}));
    ASSERT_EQ(tc->Name_Language_Code.get_size(), 1);
    ASSERT_EQ(tc->Name_Language_Code.get(0), (DremelRow<Varchar<30>>{std::nullopt, 0, 1}));
    ASSERT_EQ(tc->Name_Language_Country.get_size(), 1);
    ASSERT_EQ(tc->Name_Language_Country.get(0), (DremelRow<Varchar<30>>{std::nullopt, 0, 1}));
    ASSERT_EQ(tc->Name_Url.get_size(), 1);
    ASSERT_EQ(tc->Name_Url.get(0), (DremelRow<Varchar<30>>{{Varchar<30>::build("http://C")}, 0, 2}));
}

// This test corresponds to the example from the Dremel paper in Figure 2 and Figure 3.
TEST(DremelTest, ShreddingDocumentRecordLarge) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestDocumentLarge);

    db.LoadDocumentTable(in);

    auto* tc = reinterpret_cast<TestClass*>(&db.documentTable);
    tc->print();

    ASSERT_EQ(db.documentTable.get_size(), 1);
    ASSERT_EQ(tc->DocId.get_size(), 1);
    ASSERT_EQ(tc->DocId.get(0), (DremelRow<Integer>{{Integer(10)}, 0, 0}));
    ASSERT_EQ(tc->Links_Backward.get_size(), 1);
    ASSERT_EQ(tc->Links_Backward.get(0), (DremelRow<Integer>{std::nullopt, 0, 1}));
    ASSERT_EQ(tc->Links_Forward.get_size(), 3);
    ASSERT_EQ(tc->Links_Forward.get(0), (DremelRow<Integer>{{Integer(20)}, 0, 2}));
    ASSERT_EQ(tc->Links_Forward.get(1), (DremelRow<Integer>{{Integer(40)}, 1, 2}));
    ASSERT_EQ(tc->Links_Forward.get(2), (DremelRow<Integer>{{Integer(60)}, 1, 2}));
    ASSERT_EQ(tc->Name_Language_Code.get_size(), 4);
    ASSERT_EQ(tc->Name_Language_Code.get(0), (DremelRow<Varchar<30>>{{Varchar<30>::build("en-us")}, 0, 2}));
    ASSERT_EQ(tc->Name_Language_Code.get(1), (DremelRow<Varchar<30>>{{Varchar<30>::build("en")}, 2, 2}));
    ASSERT_EQ(tc->Name_Language_Code.get(2), (DremelRow<Varchar<30>>{std::nullopt, 1, 1}));
    ASSERT_EQ(tc->Name_Language_Code.get(3), (DremelRow<Varchar<30>>{{Varchar<30>::build("en-gb")}, 1, 2}));
    ASSERT_EQ(tc->Name_Language_Country.get_size(), 4);
    ASSERT_EQ(tc->Name_Language_Country.get(0), (DremelRow<Varchar<30>>{{Varchar<30>::build("us")}, 0, 3}));
    ASSERT_EQ(tc->Name_Language_Country.get(1), (DremelRow<Varchar<30>>{std::nullopt, 2, 2}));
    ASSERT_EQ(tc->Name_Language_Country.get(2), (DremelRow<Varchar<30>>{std::nullopt, 1, 1}));
    ASSERT_EQ(tc->Name_Language_Country.get(3), (DremelRow<Varchar<30>>{{Varchar<30>::build("gb")}, 1, 3}));
    ASSERT_EQ(tc->Name_Url.get_size(), 3);
    ASSERT_EQ(tc->Name_Url.get(0), (DremelRow<Varchar<30>>{{Varchar<30>::build("http://A")}, 0, 2}));
    ASSERT_EQ(tc->Name_Url.get(1), (DremelRow<Varchar<30>>{{Varchar<30>::build("http://B")}, 1, 2}));
    ASSERT_EQ(tc->Name_Url.get(2), (DremelRow<Varchar<30>>{std::nullopt, 1, 1}));
}

// This test corresponds to the example from the Dremel paper in Figure 4.
TEST(DremelTest, ConstructCompleteFSM) {
    Field DocId {"DocId", 0 };
    Field Links_Backward {"Links.Backward", 1 };
    Field Links_Forward {"Links.Forward", 1 };
    Field Name_Language_Code {"Name.Language.Code", 2 };
    Field Name_Language_Country {"Name.Language.Country", 2 };
    Field Name_Url {"Name.Url", 1 };

    RecordFSM fsm {std::vector<Field>{DocId, Links_Backward, Links_Forward, Name_Language_Code, Name_Language_Country, Name_Url} };

    std::cout << "Resulting FSM:" << std::endl << fsm.GenerateTikzGraph() << std::endl;

    ASSERT_EQ(fsm.NextField("DocId", 0).value(), "Links.Backward");
    ASSERT_EQ(fsm.NextField("Links.Backward", 1).value(), "Links.Backward");
    ASSERT_EQ(fsm.NextField("Links.Backward", 0).value(), "Links.Forward");
    ASSERT_EQ(fsm.NextField("Links.Forward", 1).value(), "Links.Forward");
    ASSERT_EQ(fsm.NextField("Links.Forward", 0).value(), "Name.Language.Code");
    ASSERT_EQ(fsm.NextField("Name.Language.Code", 0).value(), "Name.Language.Country");
    ASSERT_EQ(fsm.NextField("Name.Language.Code", 1).value(), "Name.Language.Country");
    ASSERT_EQ(fsm.NextField("Name.Language.Code", 2).value(), "Name.Language.Country");
    ASSERT_EQ(fsm.NextField("Name.Language.Country", 2).value(), "Name.Language.Code");
    ASSERT_EQ(fsm.NextField("Name.Language.Country", 1).value(), "Name.Url");
    ASSERT_EQ(fsm.NextField("Name.Language.Country", 2).value(), "Name.Url");
    ASSERT_EQ(fsm.NextField("Name.Url", 1).value(), "Name.Language.Code");
    ASSERT_EQ(fsm.NextField("Name.Url", 0).has_value(), false);  // end state
}

// This test corresponds to the example from the Dremel paper in Figure 5.
TEST(DremelTest, ConstructPartialFSM) {
    Field DocId {"DocId", 0 };
    Field Name_Language_Country {"Name.Language.Country", 2 };

    RecordFSM fsm {std::vector<Field>{DocId, Name_Language_Country} };

    std::cout << "Resulting FSM:" << std::endl << fsm.GenerateTikzGraph() << std::endl;

    ASSERT_EQ(fsm.NextField("DocId", 0).value(), "Name.Language.Country");
    ASSERT_EQ(fsm.NextField("Name.Language.Country", 1).value(), "Name.Language.Country");
    ASSERT_EQ(fsm.NextField("Name.Language.Country", 2).value(), "Name.Language.Country");
    ASSERT_EQ(fsm.NextField("Name.Language.Country", 0).has_value(), false);  // end state
}

}  // namespace
