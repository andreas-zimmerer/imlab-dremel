// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#ifndef INCLUDE_DATABASE_H_
#define INCLUDE_DATABASE_H_

#include <istream>
#include "../tools/protobuf/gen/schema.h"

namespace imlab {

class Database {
 public:
    /// Load data from JSON into documentTable.
    /// The underlying file format of the istream should be JSON.
    /// The JSON should be an array at the top level.
    void LoadDocumentTable(std::istream& in);

    imlab::schema::DocumentTable documentTable;
};

}  // namespace imlab

#endif  // INCLUDE_DATABASE_H_
