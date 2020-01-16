// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <sstream>
#include <fstream>
#include "imlab/test/data.h"
#include "database.h"
#include "imlab/dremel/record_fsm.h"
#include "imlab/dremel/schema_helper.h"
#include "gtest/gtest.h"
#include "gtest/gtest_prod.h"
#include <google/protobuf/util/message_differencer.h>

namespace {
using namespace imlab::dremel;

// FieldDescriptors:
const auto* DocId_Field = Document::descriptor()->FindFieldByName("DocId");
const auto& Links_Field = Document::descriptor()->FindFieldByName("links");
const auto* Links_Backward_Field = Document_Links::descriptor()->FindFieldByName("Backward");
const auto* Links_Forward_Field = Document_Links::descriptor()->FindFieldByName("Forward");
const auto* Name_Field = Document::descriptor()->FindFieldByName("name");
const auto* Name_Language_Field = Document_Name::descriptor()->FindFieldByName("language");
const auto* Name_Language_Code_Field = Document_Name_Language::descriptor()->FindFieldByName("Code");
const auto* Name_Language_Country_Field = Document_Name_Language::descriptor()->FindFieldByName("Country");
const auto* Name_Url_Field = Document_Name::descriptor()->FindFieldByName("Url");


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
        _print(DocId_Column);
        _print(Links_Backward_Column);
        _print(Links_Forward_Column);
        _print(Name_Language_Code_Column);
        _print(Name_Language_Country_Column);
        _print(Name_Url_Column);
    }
};

// ---------------------------------------------------------------------------

TEST(DremelSchemaHelperTest, RepetitionLevelDocId) {
    auto repetition_level = GetMaxRepetitionLevel(DocId_Field);
    ASSERT_EQ(repetition_level, 0);
}

TEST(DremelSchemaHelperTest, RepetitionLevelLinksBackward) {
    auto repetition_level = GetMaxRepetitionLevel(Links_Backward_Field);
    ASSERT_EQ(repetition_level, 1);
}

TEST(DremelSchemaHelperTest, RepetitionLevelLinksForward) {
    auto repetition_level = GetMaxRepetitionLevel(Links_Forward_Field);
    ASSERT_EQ(repetition_level, 1);
}

TEST(DremelSchemaHelperTest, RepetitionLevelNameLanguageCode) {
    auto repetition_level = GetMaxRepetitionLevel(Name_Language_Code_Field);
    ASSERT_EQ(repetition_level, 2);
}

TEST(DremelSchemaHelperTest, RepetitionLevelNameLanguageCountry) {
    auto repetition_level = GetMaxRepetitionLevel(Name_Language_Country_Field);
    ASSERT_EQ(repetition_level, 2);
}

TEST(DremelSchemaHelperTest, RepetitionLevelNameUrl) {
    auto repetition_level = GetMaxRepetitionLevel(Name_Url_Field);
    ASSERT_EQ(repetition_level, 1);
}

// ---------------------------------------------------------------------------

// Trivial case where both fields are repeated by themselves
TEST(DremelSchemaHelperTest, CommonAncestorLinks) {
    auto common_ancestor = GetCommonAncestor(Links_Backward_Field, Links_Forward_Field);
    ASSERT_EQ(common_ancestor, Links_Field);
}

// Trivial case where both fields are non-repeated.
TEST(DremelSchemaHelperTest, CommonAncestorLanguage) {
    auto common_ancestor = GetCommonAncestor(Name_Language_Code_Field, Name_Language_Country_Field);
    ASSERT_EQ(common_ancestor, Name_Language_Field);
}

// Interesting case: For (code,country) it's easy because they belong under the same "language".
// But if we have (country,code), they don't belong to the same "language" and hence we need to go a level up to "name".
TEST(DremelSchemaHelperTest, CommonAncestorLanguageReverse) {
    auto common_ancestor = GetCommonAncestor(Name_Language_Country_Field, Name_Language_Code_Field);
    ASSERT_EQ(common_ancestor, Name_Field);
}

// Trivial case where we have just one repeated field.
TEST(DremelSchemaHelperTest, CommonAncestorEqualFieldsRepeated) {
    auto common_ancestor = GetCommonAncestor(Links_Backward_Field, Links_Backward_Field);
    ASSERT_EQ(common_ancestor, Links_Field);
}

// Interesting case: Both fields are equal, but because there can only be just one "code" in each "language",
// they can't belong to the same "language" and hence we need to go up to "name".
TEST(DremelSchemaHelperTest, CommonAncestorEqualFieldsNonRepeated) {
    auto common_ancestor = GetCommonAncestor(Name_Language_Code_Field, Name_Language_Code_Field);
    ASSERT_EQ(common_ancestor, Name_Field);
}

// Interesting case: First Links_Forward, then Links_Backward -> can't exist in one message so
// they have to be in different messages. Common ancestor is thus nullptr.
TEST(DremelSchemaHelperTest, CommonAncestorDifferentMessage) {
    auto common_ancestor = GetCommonAncestor(Links_Forward_Field, Links_Backward_Field);
    ASSERT_EQ(common_ancestor, nullptr);
}

// Interesting case: Field 1 is an inner node that contains field 2 as a leaf.
TEST(DremelSchemaHelperTest, CommonAncestorContainingField) {
    auto common_ancestor = GetCommonAncestor(Name_Language_Field, Name_Language_Code_Field);
    ASSERT_EQ(common_ancestor, Name_Language_Field);
}

// Interesting case: Field 2 is an inner node that contains field 1 as a leaf.
TEST(DremelSchemaHelperTest, CommonAncestorContainingFieldReverse) {
    auto common_ancestor = GetCommonAncestor(Name_Language_Code_Field, Name_Language_Field);
    ASSERT_EQ(common_ancestor, Name_Field);
}

// Interesting case: Field 2 is an inner node that contains field 1 as a leaf, but we need to go up to the root.
TEST(DremelSchemaHelperTest, CommonAncestorContainingFieldReverseRoot) {
    auto common_ancestor = GetCommonAncestor(Name_Language_Field, Name_Field);
    ASSERT_EQ(common_ancestor, nullptr);
}

// Somewhat interesting: Both fields have nothing in common and the only common ancestor is the message itself.
TEST(DremelSchemaHelperTest, CommonRepetitionLevelRoot) {
    auto common_ancestor = GetCommonAncestor(Links_Backward_Field, Name_Language_Code_Field);
    ASSERT_EQ(common_ancestor, nullptr);
}

// ---------------------------------------------------------------------------

// This test corresponds to the example from the Dremel paper in Figure 2 and Figure 3.
TEST(DremelTest, ShreddingDocumentRecordSmall) {
    imlab::Database db {};
    std::stringstream in(imlab_test::kTestDocumentPaperSmall);

    db.LoadDocumentTable(in);

    auto* tc = reinterpret_cast<TestClass*>(&db.DocumentTable);
    tc->print();

    ASSERT_EQ(db.DocumentTable.size(), 1);
    ASSERT_EQ(tc->DocId_Column.size(), 1);
    ASSERT_EQ(tc->DocId_Column.get(0), (DremelRow<int64_t>{{20}, 0, 0}));
    ASSERT_EQ(tc->Links_Backward_Column.size(), 2);
    ASSERT_EQ(tc->Links_Backward_Column.get(0), (DremelRow<int64_t>{{10}, 0, 2}));
    ASSERT_EQ(tc->Links_Backward_Column.get(1), (DremelRow<int64_t>{{30}, 1, 2}));
    ASSERT_EQ(tc->Links_Forward_Column.size(), 1);
    ASSERT_EQ(tc->Links_Forward_Column.get(0), (DremelRow<int64_t>{{80}, 0, 2}));
    ASSERT_EQ(tc->Name_Language_Code_Column.size(), 1);
    ASSERT_EQ(tc->Name_Language_Code_Column.get(0), (DremelRow<std::string>{std::nullopt, 0, 1}));
    ASSERT_EQ(tc->Name_Language_Country_Column.size(), 1);
    ASSERT_EQ(tc->Name_Language_Country_Column.get(0), (DremelRow<std::string>{std::nullopt, 0, 1}));
    ASSERT_EQ(tc->Name_Url_Column.size(), 1);
    ASSERT_EQ(tc->Name_Url_Column.get(0), (DremelRow<std::string>{{"http://C"}, 0, 2}));
}

// This test corresponds to the example from the Dremel paper in Figure 2 and Figure 3.
TEST(DremelTest, ShreddingDocumentRecordLarge) {
    imlab::Database db {};
    std::stringstream in(imlab_test::kTestDocumentPaperLarge);

    db.LoadDocumentTable(in);

    auto* tc = reinterpret_cast<TestClass*>(&db.DocumentTable);
    tc->print();

    ASSERT_EQ(db.DocumentTable.size(), 1);
    ASSERT_EQ(tc->DocId_Column.size(), 1);
    ASSERT_EQ(tc->DocId_Column.get(0), (DremelRow<int64_t>{{10}, 0, 0}));
    ASSERT_EQ(tc->Links_Backward_Column.size(), 1);
    ASSERT_EQ(tc->Links_Backward_Column.get(0), (DremelRow<int64_t>{std::nullopt, 0, 1}));
    ASSERT_EQ(tc->Links_Forward_Column.size(), 3);
    ASSERT_EQ(tc->Links_Forward_Column.get(0), (DremelRow<int64_t>{{20}, 0, 2}));
    ASSERT_EQ(tc->Links_Forward_Column.get(1), (DremelRow<int64_t>{{40}, 1, 2}));
    ASSERT_EQ(tc->Links_Forward_Column.get(2), (DremelRow<int64_t>{{60}, 1, 2}));
    ASSERT_EQ(tc->Name_Language_Code_Column.size(), 4);
    ASSERT_EQ(tc->Name_Language_Code_Column.get(0), (DremelRow<std::string>{{"en-us"}, 0, 2}));
    ASSERT_EQ(tc->Name_Language_Code_Column.get(1), (DremelRow<std::string>{{"en"}, 2, 2}));
    ASSERT_EQ(tc->Name_Language_Code_Column.get(2), (DremelRow<std::string>{std::nullopt, 1, 1}));
    ASSERT_EQ(tc->Name_Language_Code_Column.get(3), (DremelRow<std::string>{{"en-gb"}, 1, 2}));
    ASSERT_EQ(tc->Name_Language_Country_Column.size(), 4);
    ASSERT_EQ(tc->Name_Language_Country_Column.get(0), (DremelRow<std::string>{{"us"}, 0, 3}));
    ASSERT_EQ(tc->Name_Language_Country_Column.get(1), (DremelRow<std::string>{std::nullopt, 2, 2}));
    ASSERT_EQ(tc->Name_Language_Country_Column.get(2), (DremelRow<std::string>{std::nullopt, 1, 1}));
    ASSERT_EQ(tc->Name_Language_Country_Column.get(3), (DremelRow<std::string>{{"gb"}, 1, 3}));
    ASSERT_EQ(tc->Name_Url_Column.size(), 3);
    ASSERT_EQ(tc->Name_Url_Column.get(0), (DremelRow<std::string>{{"http://A"}, 0, 2}));
    ASSERT_EQ(tc->Name_Url_Column.get(1), (DremelRow<std::string>{{"http://B"}, 1, 2}));
    ASSERT_EQ(tc->Name_Url_Column.get(2), (DremelRow<std::string>{std::nullopt, 1, 1}));
}

// ---------------------------------------------------------------------------

// This test corresponds to the example from the Dremel paper in Figure 4.
TEST(DremelTest, ConstructCompleteFSM) {
    RecordFSM fsm {{
        DocId_Field,
        Links_Backward_Field,
        Links_Forward_Field,
        Name_Language_Code_Field,
        Name_Language_Country_Field,
        Name_Url_Field
    }};

    std::cout << fsm.GenerateFsmGraph() << std::endl;

    ASSERT_EQ(fsm.NextField(DocId_Field, 0), Links_Backward_Field);
    ASSERT_EQ(fsm.NextField(Links_Backward_Field, 1), Links_Backward_Field);
    ASSERT_EQ(fsm.NextField(Links_Backward_Field, 0), Links_Forward_Field);
    ASSERT_EQ(fsm.NextField(Links_Forward_Field, 1), Links_Forward_Field);
    ASSERT_EQ(fsm.NextField(Links_Forward_Field, 0), Name_Language_Code_Field);
    ASSERT_EQ(fsm.NextField(Name_Language_Code_Field, 0), Name_Language_Country_Field);
    ASSERT_EQ(fsm.NextField(Name_Language_Code_Field, 1), Name_Language_Country_Field);
    ASSERT_EQ(fsm.NextField(Name_Language_Code_Field, 2), Name_Language_Country_Field);
    ASSERT_EQ(fsm.NextField(Name_Language_Country_Field, 2), Name_Language_Code_Field);
    ASSERT_EQ(fsm.NextField(Name_Language_Country_Field, 1), Name_Url_Field);
    ASSERT_EQ(fsm.NextField(Name_Language_Country_Field, 0), Name_Url_Field);
    ASSERT_EQ(fsm.NextField(Name_Url_Field, 1), Name_Language_Code_Field);
    ASSERT_EQ(fsm.NextField(Name_Url_Field, 0), nullptr);  // end state
}

// This test corresponds to the example from the Dremel paper in Figure 5.
TEST(DremelTest, ConstructPartialFSM) {
    RecordFSM fsm {{
        DocId_Field,
        Name_Language_Country_Field
    }};

    std::cout << fsm.GenerateFsmGraph() << std::endl;

    ASSERT_EQ(fsm.NextField(DocId_Field, 0), Name_Language_Country_Field);
    ASSERT_EQ(fsm.NextField(Name_Language_Country_Field, 1), Name_Language_Country_Field);
    ASSERT_EQ(fsm.NextField(Name_Language_Country_Field, 2), Name_Language_Country_Field);
    ASSERT_EQ(fsm.NextField(Name_Language_Country_Field, 0), nullptr);  // end state
}

// Tests another example from the paper (Figure 5)
TEST(DremelTest, InsertAndGetLargeRecordWithPartialFSM) {
    imlab::Database db {};
    std::stringstream in(imlab_test::kTestDocumentPaperLarge);

    db.LoadDocumentTable(in);
    auto record = db.DocumentTable.get(0, {
        DocId_Field,
        Name_Language_Country_Field
    });

    // Expected Result
    Document document {};
    document.set_docid(10);
    auto* name_1 = document.add_name();
    auto* lang_1 = name_1->add_language();
    lang_1->set_country("us");
    auto* lang_2 = name_1->add_language();
    auto* name_2 = document.add_name();
    auto* lang_3 = name_2->add_language();
    lang_3->set_country("gb");

    ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(document, record))
        << "\nExpected:\n\n" << document.DebugString() << "\nBut got:\n\n" << record.DebugString();
}

// Tests another example from the paper (Figure 5)
TEST(DremelTest, InsertAndGetSmallRecordWithPartialFSM) {
    imlab::Database db {};
    std::stringstream in(imlab_test::kTestDocumentPaperSmall);

    db.LoadDocumentTable(in);
    auto record = db.DocumentTable.get(0, {
        DocId_Field,
        Name_Language_Country_Field
    });

    // Expected Result:
    Document document {};
    document.set_docid(20);
    document.add_name();

    ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(document, record))
        << "\nExpected:\n\n" << document.DebugString() << "\nBut got:\n\n" << record.DebugString();
}

TEST(DremelTest, InsertAndGetEmptyRecordWithPartialFSM) {
    imlab::Database db {};
    std::stringstream in(imlab_test::kTestDocumentEmpty);

    db.LoadDocumentTable(in);
    auto record = db.DocumentTable.get(0, {
        DocId_Field,
        Name_Language_Country_Field
    });

    // Expected Result:
    Document document {};
    document.set_docid(30);

    ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(document, record))
        << "\nExpected:\n\n" << document.DebugString() << "\nBut got:\n\n" << record.DebugString();
}

TEST(DremelTest, InsertAndGetLargeRecordRoundtrip) {
    Document document {};
    std::stringstream in(imlab_test::kTestDocumentPaperLarge);
    imlab::Database::DecodeJson(in, [&](auto& d) { document = d; });

    imlab::Database db {};

    db.DocumentTable.insert(document);
    const auto& record = db.DocumentTable.get(0, {
        DocId_Field,
        Links_Backward_Field,
        Links_Forward_Field,
        Name_Language_Code_Field,
        Name_Language_Country_Field,
        Name_Url_Field
    });

    ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(document, record))
        << "\nExpected:\n\n" << document.DebugString() << "\nBut got:\n\n" << record.DebugString();
}

TEST(DremelTest, InsertAndGetSmallRecordRoundtrip) {
    Document document {};
    std::stringstream in(imlab_test::kTestDocumentPaperSmall);
    imlab::Database::DecodeJson(in, [&](auto& d) { document = d; });

    imlab::Database db {};

    db.DocumentTable.insert(document);
    const auto& record = db.DocumentTable.get(0, {
        DocId_Field,
        Links_Backward_Field,
        Links_Forward_Field,
        Name_Language_Code_Field,
        Name_Language_Country_Field,
        Name_Url_Field
    });

    ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(document, record))
        << "\nExpected:\n\n" << document.DebugString() << "\nBut got:\n\n" << record.DebugString();
}

TEST(DremelTest, InsertAndGetEmptyRecordRoundtrip) {
    Document document {};
    std::stringstream in(imlab_test::kTestDocumentEmpty);
    imlab::Database::DecodeJson(in, [&](auto& d) { document = d; });

    imlab::Database db {};

    db.DocumentTable.insert(document);
    const auto& record = db.DocumentTable.get(0, {
        DocId_Field,
        Links_Backward_Field,
        Links_Forward_Field,
        Name_Language_Code_Field,
        Name_Language_Country_Field,
        Name_Url_Field
    });

    ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(document, record))
        << "\nExpected:\n\n" << document.DebugString() << "\nBut got:\n\n" << record.DebugString();
}

// Load a bunch of randomly generated record into the DB and retrieve them individually.
TEST(DremelTest, InsertAndGetIndividual) {
    imlab::Database db {};
    std::vector<Document> documents {};

    system("cd ../data/dremel && python3 generate_dremel_data.py 500 500");
    std::fstream dremel_file("../data/dremel/generated_data_500_500.json", std::fstream::in);
    imlab::Database::DecodeJson(dremel_file, [&](auto& d) {
        db.DocumentTable.insert(d);
        documents.push_back(d);
    });

    for (unsigned i = 0; i < documents.size(); i++) {
        const auto& record = db.DocumentTable.get(i, {
            DocId_Field,
            Links_Backward_Field,
            Links_Forward_Field,
            Name_Language_Code_Field,
            Name_Language_Country_Field,
            Name_Url_Field
        });

        ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(documents[i], record))
            << "\nExpected (" << i + 1 << "):\n\n" << documents[i].DebugString() << "\nBut got:\n\n" << record.DebugString();
    }
}

// Load a bunch of randomly generated record into the DB and retrieve them in one get_range request.
TEST(DremelTest, InsertAndGetFullRange) {
    imlab::Database db {};
    std::vector<Document> documents {};

    system("cd ../data/dremel && python3 generate_dremel_data.py 500 500");
    std::fstream dremel_file("../data/dremel/generated_data_500_500.json", std::fstream::in);
    imlab::Database::DecodeJson(dremel_file, [&](auto& d) {
        db.DocumentTable.insert(d);
        documents.push_back(d);
    });

    const auto& documents_read = db.DocumentTable.get_range(0, documents.size(), {
        DocId_Field,
        Links_Backward_Field,
        Links_Forward_Field,
        Name_Language_Code_Field,
        Name_Language_Country_Field,
        Name_Url_Field
    });

    ASSERT_EQ(documents.size(), documents_read.size());

    for (unsigned i = 0; i < documents.size(); i++) {
        ASSERT_TRUE(google::protobuf::util::MessageDifferencer::Equals(documents[i], documents_read[i]))
            << "\nExpected (" << i + 1 << "):\n\n" << documents[i].DebugString() << "\nBut got:\n\n" << documents_read[i].DebugString();
    }
}

}  // namespace
