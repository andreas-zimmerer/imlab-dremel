// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_SCHEMAC_DEFAULT_SCHEMA_H_
#define INCLUDE_IMLAB_SCHEMAC_DEFAULT_SCHEMA_H_
// ---------------------------------------------------------------------------------------------------
#include <vector>
#include "imlab/schemac/schema_parse_context.h"
// ---------------------------------------------------------------------------------------------------
using Type = imlab::schemac::Type;
// ---------------------------------------------------------------------------------------------------
namespace imlab {
namespace schemac {
// ---------------------------------------------------------------------------------------------------
const Schema defaultSchema {
    std::vector<Table> {
        Table {
            "Document",
            std::vector<Column> {
                {"DocId", Type::Integer()},
                {"Links.Backward", Type::Integer()},
                {"Links.Forward", Type::Integer()},
                {"Name.Url", Type::Varchar(30)},
                {"Name.Language.Code", Type::Varchar(30)},
                {"Name.Language.Country", Type::Varchar(30)}
            },
            std::vector<Column> {},
            IndexType::kSTLUnorderedMap
        }
    },
    std::vector<Index> {}
};
// ---------------------------------------------------------------------------------------------------
    }  // namespace schemac
}  // namespace imlab
// ---------------------------------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_SCHEMAC_DEFAULT_SCHEMA_H_
// ---------------------------------------------------------------------------------------------------
