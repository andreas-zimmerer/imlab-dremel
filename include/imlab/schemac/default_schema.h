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
            "warehouse",
            std::vector<Column> {
                {"w_id", Type::Integer()},
                {"w_name", Type::Varchar(10)},
                {"w_street_1", Type::Varchar(20)},
                {"w_street_2", Type::Varchar(20)},
                {"w_city", Type::Varchar(20)},
                {"w_state", Type::Char(2)},
                {"w_zip", Type::Char(9)},
                {"w_tax", Type::Numeric(4, 4)},
                {"w_ytd", Type::Numeric(12, 2)}
            },
            std::vector<Column> {
                {"w_id", Type::Integer()}
            },
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
