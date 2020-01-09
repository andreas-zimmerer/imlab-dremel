// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#ifndef INCLUDE_DATABASE_H_
#define INCLUDE_DATABASE_H_

#include <istream>
#include <functional>
#include <imlab/algebra/query.h>
#include "../tools/protobuf/gen/schema.h"

namespace imlab {

struct QueryStats {
    long code_generation_duration;
    long code_compilation_duration;
    long query_execution_duration;
};

class Database {
 public:
    /// Load data from JSON into documentTable.
    /// The underlying file format of the istream should be JSON.
    /// The JSON should be an array at the top level.
    void LoadDocumentTable(std::istream& in);

    static void DecodeJson(std::istream& in, const std::function<void (Document&)>& handler);

    QueryStats RunQuery(Query& query);

    imlab::schema::DocumentTable documentTable;
};

}  // namespace imlab

#endif  // INCLUDE_DATABASE_H_
