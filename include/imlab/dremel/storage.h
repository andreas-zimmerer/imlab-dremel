// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_DREMEL_STORAGE_H_
#define INCLUDE_IMLAB_DREMEL_STORAGE_H_
//---------------------------------------------------------------------------
#include <optional>
#include <vector>
#include <tuple>
#include <string>
#include <google/protobuf/descriptor.h>
#include "./schema_helper.h"
//---------------------------------------------------------------------------
namespace imlab {
namespace dremel {
//---------------------------------------------------------------------------
using namespace google::protobuf;
using TID = uint64_t;

class FieldWriter;  // Forward-declare

class TableBase {
 public:
    uint64_t size() { return _size; }

    virtual FieldWriter* record_writer() = 0;
 protected:
    uint64_t _size = 0;
};

/// A row in Dremel consists of
///  * a value that might be null
///  * a repetition level
///  * a definition level
template<typename T>
struct DremelRow {
    std::optional<T> value;
    unsigned repetition_level;
    unsigned definition_level;
};

template<typename T>
inline bool operator==(const DremelRow<T>& lhs, const DremelRow<T>& rhs){
    return lhs.value == rhs.value
        && lhs.repetition_level == rhs.repetition_level
        && lhs.definition_level == rhs.definition_level;
}

/// A column in the Dremel format.
/// What's special about a Dremel column is that not only values are stored,
/// but also repetition and definition levels for every value.
/// Values can also be NULL.
///
/// Usually, a column is stored a little more efficient in Dremel,
/// e.g. only required repetition and definition levels are stored while others
/// are inferred. Also, r and d levels only take up as many bits as they actually
/// need.
///
/// \tparam T The type of the values stored in the column.
template<typename T>
class DremelColumn {
 public:
    /// Creates a new column in the Dremel format.
    ///
    /// \tparam field The field of the message that is stored in this column.
    explicit DremelColumn(const FieldDescriptor* field)
        : _field(field), _max_definition_level(GetDefinitionLevel(field)) {}

    /// Returns the field of the record which is stored in this column.
    const FieldDescriptor* field() { return _field; }

    /// Insert a new value into the column with a given repetition and definition level.
    /// Returns the TID of the inserted value.
    TID insert(DremelRow<T> row) {
        _rows.push_back({ row.value.value_or(T{}), row.repetition_level, row.definition_level });
        return _rows.size() - 1;
    }

    /// Retrieves a value together with its repetition and definition levels for a given TID.
    DremelRow<T> get(TID tid) {
        auto& [value, r, d] = _rows[tid];
        // Null values are stored implicitly if the definition
        // level is smaller than the maximum definition level.
        if (d < _max_definition_level) {
            return { std::nullopt, r, d};
        } else {
            return { value, r, d};
        }
    }

    /// Returns the number of elements in this column.
    uint64_t size() { return _rows.size(); }

 protected:
    const FieldDescriptor* _field;

    const std::size_t _max_definition_level;

    std::vector<std::tuple<T, unsigned, unsigned>> _rows;
};

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_DREMEL_STORAGE_H_
//---------------------------------------------------------------------------
