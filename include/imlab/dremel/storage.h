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
//---------------------------------------------------------------------------
namespace imlab {
namespace dremel {
//---------------------------------------------------------------------------
using TID = uint64_t;

class FieldWriter;  // Forward-declare

class TableBase {
 public:
    uint64_t get_size() { return size; }

    virtual FieldWriter* get_record_writer() = 0;
 protected:
    uint64_t size = 0;
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
    /// \tparam indentifier A human readable identifier for this column.
    /// \tparam max_definition_level The number of optional and repeated fields in the record path.
    ///                              Note that 'required' fields should not be counted!
    explicit DremelColumn(std::string identifier, std::size_t max_definition_level)
        : identifier(std::move(identifier)), max_definition_level(max_definition_level) {}

    /// Returns the human-readable identifier for this column.
    const std::string& get_identifier() { return identifier; }

    /// Insert a new value into the column with a given repetition and definition level.
    /// Returns the TID of the inserted value.
    TID insert(DremelRow<T> row) {
        rows.push_back({ row.value.value_or(T{}), row.repetition_level, row.definition_level });
        return rows.size() - 1;
    }

    /// Retrieves a value together with its repetition and definition levels for a given TID.
    DremelRow<T> get(TID tid) {
        auto& [value, r, d] = rows[tid];
        if (d < max_definition_level) {
            return { std::nullopt, r, d};
        } else {
            return { value, r, d};
        }
    }

    /// Returns the number of elements in this column.
    uint64_t get_size() { return rows.size(); }

 protected:
    /// Human-readable identifier of this column.
    const std::string identifier;

    /// The maximum definition level values can have in this column.
    /// Defined as the number of optional and repeated fields in the record path.
    const std::size_t max_definition_level;

    /// Null values are stored implicitly if the definition
    /// level is smaller than the maximum definition level.
    std::vector<std::tuple<T, unsigned, unsigned>> rows;
};

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_DREMEL_STORAGE_H_
//---------------------------------------------------------------------------
