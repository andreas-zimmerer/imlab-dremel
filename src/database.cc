// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "database.h"
#include "rapidjson/document.h"
#include <rapidjson/istreamwrapper.h>

namespace imlab {

void Database::LoadDremel(std::istream &in) {
    rapidjson::IStreamWrapper stream(in);
    rapidjson::Document d;
    d.ParseStream(stream);
}

}  // namespace imlab
