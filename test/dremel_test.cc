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
#include <google/protobuf/util/message_differencer.h>

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

TEST(DremelSchemaHelperTest, RepetitionLevelDocId) {
    const auto& desc = Document::descriptor()->FindFieldByName("DocId");
    auto repetition_level = GetMaxRepetitionLevel(desc);
    ASSERT_EQ(repetition_level, 0);
}

TEST(DremelSchemaHelperTest, RepetitionLevelLinksBackward) {
    const auto& desc = Document_Links::descriptor()->FindFieldByName("Backward");
    auto repetition_level = GetMaxRepetitionLevel(desc);
    ASSERT_EQ(repetition_level, 1);
}

TEST(DremelSchemaHelperTest, RepetitionLevelLinksForward) {
    const auto& desc = Document_Links::descriptor()->FindFieldByName("Forward");
    auto repetition_level = GetMaxRepetitionLevel(desc);
    ASSERT_EQ(repetition_level, 1);
}

TEST(DremelSchemaHelperTest, RepetitionLevelNameLanguageCode) {
    const auto& desc = Document_Name_Language::descriptor()->FindFieldByName("Code");
    auto repetition_level = GetMaxRepetitionLevel(desc);
    ASSERT_EQ(repetition_level, 2);
}

TEST(DremelSchemaHelperTest, RepetitionLevelNameLanguageCountry) {
    const auto& desc = Document_Name_Language::descriptor()->FindFieldByName("Country");
    auto repetition_level = GetMaxRepetitionLevel(desc);
    ASSERT_EQ(repetition_level, 2);
}

TEST(DremelSchemaHelperTest, RepetitionLevelNameUrl) {
    const auto& desc = Document_Name::descriptor()->FindFieldByName("Url");
    auto repetition_level = GetMaxRepetitionLevel(desc);
    ASSERT_EQ(repetition_level, 1);
}

// ---------------------------------------------------------------------------

// Trivial case where both fields are repeated by themselves
TEST(DremelSchemaHelperTest, CommonAncestorLinks) {
    const auto& links_backward_desc = Document_Links::descriptor()->FindFieldByName("Backward");
    const auto& links_forward_desc = Document_Links::descriptor()->FindFieldByName("Forward");
    const auto& links_desc = Document::descriptor()->FindFieldByName("links");
    auto common_ancestor = GetCommonAncestor(links_backward_desc, links_forward_desc);
    ASSERT_EQ(common_ancestor, links_desc);
}

// Trivial case where both fields are non-repeated.
TEST(DremelSchemaHelperTest, CommonAncestorLanguage) {
    const auto& name_language_code_desc = Document_Name_Language::descriptor()->FindFieldByName("Code");
    const auto& name_language_country_desc = Document_Name_Language::descriptor()->FindFieldByName("Country");
    const auto& name_language_desc = Document_Name::descriptor()->FindFieldByName("language");
    auto common_ancestor = GetCommonAncestor(name_language_code_desc, name_language_country_desc);
    ASSERT_EQ(common_ancestor, name_language_desc);
}

// Interesting case: For (code,country) it's easy because they belong under the same "language".
// But if we have (country,code), they don't belong to the same "language" and hence we need to go a level up to "name".
TEST(DremelSchemaHelperTest, CommonAncestorLanguageReverse) {
    const auto& name_language_country_desc = Document_Name_Language::descriptor()->FindFieldByName("Country");
    const auto& name_language_code_desc = Document_Name_Language::descriptor()->FindFieldByName("Code");
    const auto& name_desc = Document::descriptor()->FindFieldByName("name");
    auto common_ancestor = GetCommonAncestor(name_language_country_desc, name_language_code_desc);
    ASSERT_EQ(common_ancestor, name_desc);
}

// Trivial case where we have just one repeated field.
TEST(DremelSchemaHelperTest, CommonAncestorEqualFieldsRepeated) {
    const auto& links_backward_desc = Document_Links::descriptor()->FindFieldByName("Backward");
    const auto& links_forward_desc = Document_Links::descriptor()->FindFieldByName("Backward");
    const auto& links_desc = Document::descriptor()->FindFieldByName("links");
    auto common_ancestor = GetCommonAncestor(links_backward_desc, links_forward_desc);
    ASSERT_EQ(common_ancestor, links_desc);
}

// Interesting case: Both fields are equal, but because there can only be just one "code" in each "language",
// they can't belong to the same "language" and hence we need to go up to "name".
TEST(DremelSchemaHelperTest, CommonAncestorEqualFieldsNonRepeated) {
    const auto& name_language_code_1_desc = Document_Name_Language::descriptor()->FindFieldByName("Code");
    const auto& name_language_code_2_desc = Document_Name_Language::descriptor()->FindFieldByName("Code");
    const auto& name_desc = Document::descriptor()->FindFieldByName("name");
    auto common_ancestor = GetCommonAncestor(name_language_code_1_desc, name_language_code_2_desc);
    ASSERT_EQ(common_ancestor, name_desc);
}

// Interesting case: First Links_Forward, then Links_Backward -> can't exist in one message so
// they have to be in different messages. Common ancestor is thus nullptr.
TEST(DremelSchemaHelperTest, CommonAncestorDifferentMessage) {
    const auto& links_forward_desc = Document_Links::descriptor()->FindFieldByName("Forward");
    const auto& links_backward_desc = Document_Links::descriptor()->FindFieldByName("Backward");
    auto common_ancestor = GetCommonAncestor(links_forward_desc, links_backward_desc);
    ASSERT_EQ(common_ancestor, nullptr);
}

// Interesting case: Field 1 is an inner node that contains field 2 as a leaf.
TEST(DremelSchemaHelperTest, CommonAncestorContainingField) {
    const auto& name_language_desc = Document_Name::descriptor()->FindFieldByName("language");
    const auto& name_language_code_desc = Document_Name_Language::descriptor()->FindFieldByName("Code");
    auto common_ancestor = GetCommonAncestor(name_language_desc, name_language_code_desc);
    ASSERT_EQ(common_ancestor, name_language_desc);
}

// Interesting case: Field 2 is an inner node that contains field 1 as a leaf.
TEST(DremelSchemaHelperTest, CommonAncestorContainingFieldReverse) {
    const auto& name_language_code_desc = Document_Name_Language::descriptor()->FindFieldByName("Code");
    const auto& name_language_desc = Document_Name::descriptor()->FindFieldByName("language");
    const auto& name_desc = Document::descriptor()->FindFieldByName("name");
    auto common_ancestor = GetCommonAncestor(name_language_code_desc, name_language_desc);
    ASSERT_EQ(common_ancestor, name_desc);
}

// Interesting case: Field 2 is an inner node that contains field 1 as a leaf, but we need to go up to the root.
TEST(DremelSchemaHelperTest, CommonAncestorContainingFieldReverseRoot) {
    const auto& name_language_desc = Document_Name::descriptor()->FindFieldByName("language");
    const auto& name_desc = Document::descriptor()->FindFieldByName("name");
    auto common_ancestor = GetCommonAncestor(name_language_desc, name_desc);
    ASSERT_EQ(common_ancestor, nullptr);
}

// Somewhat interesting: Both fields have nothing in common and the only common ancestor is the message itself.
TEST(DremelSchemaHelperTest, CommonRepetitionLevelRoot) {
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

TEST(DremelTest, InsertAndGetSmallRecordWithPartialFSM) {
    auto* DocId = Document::descriptor()->FindFieldByName("DocId");
    auto* Name_Language_Country = Document_Name_Language::descriptor()->FindFieldByName("Country");

    imlab::Database db {};
    std::stringstream in(imlab_test::kTestDocumentSmall);

    db.LoadDocumentTable(in);
    auto record = db.documentTable.get(0, {DocId, Name_Language_Country});

    std::cout << record.DebugString() << std::endl;

    // TODO assert
}

TEST(DremelTest, InsertAndGetRoundtrip) {
    // Large record from paper:
    Document document {};
    document.set_docid(10);
    Document_Links* document_links = document.mutable_links();
    document_links->add_forward(20);
    document_links->add_forward(40);
    document_links->add_forward(60);
    Document_Name* document_name_1 = document.add_name();
    Document_Name_Language* document_name_1_language_1 = document_name_1->add_language();
    document_name_1_language_1->set_code("en-us");
    document_name_1_language_1->set_country("us");
    Document_Name_Language* document_name_1_language_2 = document_name_1->add_language();
    document_name_1_language_2->set_code("en");
    document_name_1->set_url("http://A");
    Document_Name* document_name_2 = document.add_name();
    document_name_2->set_url("http://B");
    Document_Name* document_name_3 = document.add_name();
    Document_Name_Language* document_name_3_language_1 = document_name_3->add_language();
    document_name_3_language_1->set_code("en-gb");
    document_name_3_language_1->set_country("gb");

    auto* DocId = Document::descriptor()->FindFieldByName("DocId");
    auto* Links_Backward = Document_Links::descriptor()->FindFieldByName("Backward");
    auto* Links_Forward = Document_Links::descriptor()->FindFieldByName("Forward");
    auto* Name_Language_Code = Document_Name_Language::descriptor()->FindFieldByName("Code");
    auto* Name_Language_Country = Document_Name_Language::descriptor()->FindFieldByName("Country");
    auto* Name_Url = Document_Name::descriptor()->FindFieldByName("Url");

    imlab::Database db {};

    db.documentTable.insert(document);
    auto record = db.documentTable.get(0, {DocId, Links_Backward, Links_Forward, Name_Language_Code, Name_Language_Country, Name_Url});

    ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(document, record))
        << "\nExpected Message:\n\n" << document.DebugString() << "\nBut got:\n\n" << record.DebugString();
}

}  // namespace
