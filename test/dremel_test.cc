// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <sstream>
#include "imlab/test/data.h"
#include "database.h"
#include "imlab/dremel/record_fsm.h"
#include "imlab/dremel/schema_helper.h"
#include "gtest/gtest.h"
#include "gtest/gtest_prod.h"
#include "../include/database.h"

namespace {
using namespace imlab::dremel;

// Checking the contents of the private vectors and print contents of columns.
class TestClass : public imlab::schema::DocumentTable {
    FRIEND_TEST(DremelTest, ShreddingDocumentRecordSmall);
    FRIEND_TEST(DremelTest, ShreddingDocumentRecordLarge);

    template<typename T>
    void _print(DremelColumn<T> column) {
        std::cout << column.field()->full_name() << std::endl;
        for (uint64_t tid = 0; tid < column.size(); tid++) {
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

// ---------------------------------------------------------------------------

TEST(DremelTest, RepetitionLevelDocId) {
    const auto& desc = Document::descriptor()->FindFieldByName("DocId");
    auto repetition_level = GetMaxRepetitionLevel(desc);
    ASSERT_EQ(repetition_level, 0);
}

TEST(DremelTest, RepetitionLevelLinksBackward) {
    const auto& desc = Document_Links::descriptor()->FindFieldByName("Backward");
    auto repetition_level = GetMaxRepetitionLevel(desc);
    ASSERT_EQ(repetition_level, 1);
}

TEST(DremelTest, RepetitionLevelLinksForward) {
    const auto& desc = Document_Links::descriptor()->FindFieldByName("Forward");
    auto repetition_level = GetMaxRepetitionLevel(desc);
    ASSERT_EQ(repetition_level, 1);
}

TEST(DremelTest, RepetitionLevelNameLanguageCode) {
    const auto& desc = Document_Name_Language::descriptor()->FindFieldByName("Code");
    auto repetition_level = GetMaxRepetitionLevel(desc);
    ASSERT_EQ(repetition_level, 2);
}

TEST(DremelTest, RepetitionLevelNameLanguageCountry) {
    const auto& desc = Document_Name_Language::descriptor()->FindFieldByName("Country");
    auto repetition_level = GetMaxRepetitionLevel(desc);
    ASSERT_EQ(repetition_level, 2);
}

TEST(DremelTest, RepetitionLevelNameUrl) {
    const auto& desc = Document_Name::descriptor()->FindFieldByName("Url");
    auto repetition_level = GetMaxRepetitionLevel(desc);
    ASSERT_EQ(repetition_level, 1);
}

// ---------------------------------------------------------------------------

TEST(DremelTest, CommonAncestorLinks) {
    const auto& links_backward_desc = Document_Links::descriptor()->FindFieldByName("Backward");
    const auto& links_forward_desc = Document_Links::descriptor()->FindFieldByName("Forward");
    const auto& links_desc = Document::descriptor()->FindFieldByName("links");
    auto common_ancestor = GetCommonAncestor(links_backward_desc, links_forward_desc);
    ASSERT_EQ(common_ancestor, links_desc);
}

TEST(DremelTest, CommonAncestorLanguage) {
    const auto& name_language_country_desc = Document_Name_Language::descriptor()->FindFieldByName("Country");
    const auto& name_language_code_desc = Document_Name_Language::descriptor()->FindFieldByName("Code");
    const auto& name_language_desc = Document_Name::descriptor()->FindFieldByName("language");
    auto common_ancestor = GetCommonAncestor(name_language_country_desc, name_language_code_desc);
    ASSERT_EQ(common_ancestor, name_language_desc);
}

TEST(DremelTest, CommonAncestorEqualFieldsRepeated) {
    const auto& links_backward_desc = Document_Links::descriptor()->FindFieldByName("Backward");
    const auto& links_forward_desc = Document_Links::descriptor()->FindFieldByName("Backward");
    const auto& links_desc = Document::descriptor()->FindFieldByName("links");
    auto common_ancestor = GetCommonAncestor(links_backward_desc, links_forward_desc);
    ASSERT_EQ(common_ancestor, links_desc);
}

TEST(DremelTest, CommonAncestorEqualFieldsNonRepeated) {
    const auto& name_language_code_1_desc = Document_Name_Language::descriptor()->FindFieldByName("Code");
    const auto& name_language_code_2_desc = Document_Name_Language::descriptor()->FindFieldByName("Code");
    const auto& name_desc = Document::descriptor()->FindFieldByName("name");
    auto common_ancestor = GetCommonAncestor(name_language_code_1_desc, name_language_code_2_desc);
    ASSERT_EQ(common_ancestor, name_desc);
}

TEST(DremelTest, CommonRepetitionLevelRoot) {
    const auto& links_backward_desc = Document_Links::descriptor()->FindFieldByName("Backward");
    const auto& name_language_code_desc = Document_Name_Language::descriptor()->FindFieldByName("Code");
    auto common_ancestor = GetCommonAncestor(links_backward_desc, name_language_code_desc);
    ASSERT_EQ(common_ancestor, nullptr/*root*/);
}

// ---------------------------------------------------------------------------

// This test corresponds to the example from the Dremel paper in Figure 2 and Figure 3.
TEST(DremelTest, ShreddingDocumentRecordSmall) {
    imlab::Database db {};
    std::stringstream in(imlab_test::kTestDocumentSmall);

    db.LoadDocumentTable(in);

    auto* tc = reinterpret_cast<TestClass*>(&db.documentTable);
    tc->print();

    ASSERT_EQ(db.documentTable.size(), 1);
    ASSERT_EQ(tc->DocId.size(), 1);
    ASSERT_EQ(tc->DocId.get(0), (DremelRow<Integer>{{Integer(20)}, 0, 0}));
    ASSERT_EQ(tc->Links_Backward.size(), 2);
    ASSERT_EQ(tc->Links_Backward.get(0), (DremelRow<Integer>{{Integer(10)}, 0, 2}));
    ASSERT_EQ(tc->Links_Backward.get(1), (DremelRow<Integer>{{Integer(30)}, 1, 2}));
    ASSERT_EQ(tc->Links_Forward.size(), 1);
    ASSERT_EQ(tc->Links_Forward.get(0), (DremelRow<Integer>{{Integer(80)}, 0, 2}));
    ASSERT_EQ(tc->Name_Language_Code.size(), 1);
    ASSERT_EQ(tc->Name_Language_Code.get(0), (DremelRow<Varchar<30>>{std::nullopt, 0, 1}));
    ASSERT_EQ(tc->Name_Language_Country.size(), 1);
    ASSERT_EQ(tc->Name_Language_Country.get(0), (DremelRow<Varchar<30>>{std::nullopt, 0, 1}));
    ASSERT_EQ(tc->Name_Url.size(), 1);
    ASSERT_EQ(tc->Name_Url.get(0), (DremelRow<Varchar<30>>{{Varchar<30>::build("http://C")}, 0, 2}));
}

// This test corresponds to the example from the Dremel paper in Figure 2 and Figure 3.
TEST(DremelTest, ShreddingDocumentRecordLarge) {
    imlab::Database db {};
    std::stringstream in(imlab_test::kTestDocumentLarge);

    db.LoadDocumentTable(in);

    auto* tc = reinterpret_cast<TestClass*>(&db.documentTable);
    tc->print();

    ASSERT_EQ(db.documentTable.size(), 1);
    ASSERT_EQ(tc->DocId.size(), 1);
    ASSERT_EQ(tc->DocId.get(0), (DremelRow<Integer>{{Integer(10)}, 0, 0}));
    ASSERT_EQ(tc->Links_Backward.size(), 1);
    ASSERT_EQ(tc->Links_Backward.get(0), (DremelRow<Integer>{std::nullopt, 0, 1}));
    ASSERT_EQ(tc->Links_Forward.size(), 3);
    ASSERT_EQ(tc->Links_Forward.get(0), (DremelRow<Integer>{{Integer(20)}, 0, 2}));
    ASSERT_EQ(tc->Links_Forward.get(1), (DremelRow<Integer>{{Integer(40)}, 1, 2}));
    ASSERT_EQ(tc->Links_Forward.get(2), (DremelRow<Integer>{{Integer(60)}, 1, 2}));
    ASSERT_EQ(tc->Name_Language_Code.size(), 4);
    ASSERT_EQ(tc->Name_Language_Code.get(0), (DremelRow<Varchar<30>>{{Varchar<30>::build("en-us")}, 0, 2}));
    ASSERT_EQ(tc->Name_Language_Code.get(1), (DremelRow<Varchar<30>>{{Varchar<30>::build("en")}, 2, 2}));
    ASSERT_EQ(tc->Name_Language_Code.get(2), (DremelRow<Varchar<30>>{std::nullopt, 1, 1}));
    ASSERT_EQ(tc->Name_Language_Code.get(3), (DremelRow<Varchar<30>>{{Varchar<30>::build("en-gb")}, 1, 2}));
    ASSERT_EQ(tc->Name_Language_Country.size(), 4);
    ASSERT_EQ(tc->Name_Language_Country.get(0), (DremelRow<Varchar<30>>{{Varchar<30>::build("us")}, 0, 3}));
    ASSERT_EQ(tc->Name_Language_Country.get(1), (DremelRow<Varchar<30>>{std::nullopt, 2, 2}));
    ASSERT_EQ(tc->Name_Language_Country.get(2), (DremelRow<Varchar<30>>{std::nullopt, 1, 1}));
    ASSERT_EQ(tc->Name_Language_Country.get(3), (DremelRow<Varchar<30>>{{Varchar<30>::build("gb")}, 1, 3}));
    ASSERT_EQ(tc->Name_Url.size(), 3);
    ASSERT_EQ(tc->Name_Url.get(0), (DremelRow<Varchar<30>>{{Varchar<30>::build("http://A")}, 0, 2}));
    ASSERT_EQ(tc->Name_Url.get(1), (DremelRow<Varchar<30>>{{Varchar<30>::build("http://B")}, 1, 2}));
    ASSERT_EQ(tc->Name_Url.get(2), (DremelRow<Varchar<30>>{std::nullopt, 1, 1}));
}

// ---------------------------------------------------------------------------

// This test corresponds to the example from the Dremel paper in Figure 4.
TEST(DremelTest, ConstructCompleteFSM) {
    auto* DocId = Document::descriptor()->FindFieldByName("DocId");
    auto* Links_Backward = Document_Links::descriptor()->FindFieldByName("Backward");
    auto* Links_Forward = Document_Links::descriptor()->FindFieldByName("Forward");
    auto* Name_Language_Code = Document_Name_Language::descriptor()->FindFieldByName("Code");
    auto* Name_Language_Country = Document_Name_Language::descriptor()->FindFieldByName("Country");
    auto* Name_Url = Document_Name::descriptor()->FindFieldByName("Url");

    RecordFSM fsm { std::vector<const FieldDescriptor*>{DocId, Links_Backward, Links_Forward, Name_Language_Code, Name_Language_Country, Name_Url} };

    std::cout << fsm.GenerateFsmGraph() << std::endl;

    ASSERT_EQ(fsm.NextField(DocId, 0), Links_Backward);
    ASSERT_EQ(fsm.NextField(Links_Backward, 1), Links_Backward);
    ASSERT_EQ(fsm.NextField(Links_Backward, 0), Links_Forward);
    ASSERT_EQ(fsm.NextField(Links_Forward, 1), Links_Forward);
    ASSERT_EQ(fsm.NextField(Links_Forward, 0), Name_Language_Code);
    ASSERT_EQ(fsm.NextField(Name_Language_Code, 0), Name_Language_Country);
    ASSERT_EQ(fsm.NextField(Name_Language_Code, 1), Name_Language_Country);
    ASSERT_EQ(fsm.NextField(Name_Language_Code, 2), Name_Language_Country);
    ASSERT_EQ(fsm.NextField(Name_Language_Country, 2), Name_Language_Code);
    ASSERT_EQ(fsm.NextField(Name_Language_Country, 1), Name_Url);
    ASSERT_EQ(fsm.NextField(Name_Language_Country, 0), Name_Url);
    ASSERT_EQ(fsm.NextField(Name_Url, 1), Name_Language_Code);
    ASSERT_EQ(fsm.NextField(Name_Url, 0), nullptr);  // end state
}

// This test corresponds to the example from the Dremel paper in Figure 5.
TEST(DremelTest, ConstructPartialFSM) {
    auto* DocId = Document::descriptor()->FindFieldByName("DocId");
    auto* Name_Language_Country = Document_Name_Language::descriptor()->FindFieldByName("Country");

    RecordFSM fsm {std::vector<const FieldDescriptor*>{DocId, Name_Language_Country} };

    std::cout << fsm.GenerateFsmGraph() << std::endl;

    ASSERT_EQ(fsm.NextField(DocId, 0), Name_Language_Country);
    ASSERT_EQ(fsm.NextField(Name_Language_Country, 1), Name_Language_Country);
    ASSERT_EQ(fsm.NextField(Name_Language_Country, 2), Name_Language_Country);
    ASSERT_EQ(fsm.NextField(Name_Language_Country, 0), nullptr);  // end state
}

TEST(DremelTest, InsertAndGetLargeRecordWithPartialFSM) {
    auto* DocId = Document::descriptor()->FindFieldByName("DocId");
    auto* Name_Language_Country = Document_Name_Language::descriptor()->FindFieldByName("Country");

    imlab::Database db {};
    std::stringstream in(imlab_test::kTestDocumentLarge);

    db.LoadDocumentTable(in);
    auto record = db.documentTable.get(0, {DocId, Name_Language_Country});

    std::cout << record.DebugString() << std::endl;

    // TODO assert
}

}  // namespace
