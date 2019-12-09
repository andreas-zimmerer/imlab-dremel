// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <sstream>
#include "imlab/test/data.h"
#include "database.h"
#include "gtest/gtest.h"
#include "imlab/schema.h"

namespace {

TEST(DatabaseLoadingTest, LoadDremelDocumentTableSmall) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestDocumentSmall);
    db.LoadDocumentTable(in);
}

TEST(DatabaseLoadingTest, LoadDremelDocumentTableLarge) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestDocumentLarge);
    db.LoadDocumentTable(in);
}

}  // namespace
