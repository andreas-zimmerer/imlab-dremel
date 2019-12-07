// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <sstream>
#include "imlab/test/data.h"
#include "database.h"
#include "gtest/gtest.h"
#include "imlab/schema.h"

namespace {

TEST(DatabaseLoadingTest, LoadDremelDocumentSmall) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestDocumentSmall);
    db.LoadDremel(in);
}

TEST(DatabaseLoadingTest, LoadDremelDocumentLarge) {
    imlab::Database db;
    std::stringstream in(imlab_test::kTestDocumentLarge);
    db.LoadDremel(in);
}

}  // namespace
