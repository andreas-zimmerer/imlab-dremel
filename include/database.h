// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#ifndef INCLUDE_DATABASE_H_
#define INCLUDE_DATABASE_H_

#include <istream>
#include <array>
#include <unordered_map>
#include <vector>
#include "imlab/infra/hash.h"
#include "imlab/infra/dremel.h"
#include "imlab/schema.h"

namespace imlab {

class Database {
 public:
    void LoadDremel(std::istream& in);
    imlab::schema::DocumentTable documentTable;
};

}  // namespace imlab

#endif  // INCLUDE_DATABASE_H_
