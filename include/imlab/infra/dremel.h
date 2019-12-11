// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_INFRA_DREMEL_H_
#define INCLUDE_IMLAB_INFRA_DREMEL_H_
//---------------------------------------------------------------------------
#include <utility>
#include <vector>
#include <string>
#include <optional>
//---------------------------------------------------------------------------
namespace imlab {
//---------------------------------------------------------------------------

class TableBase {
 public:
    uint64_t get_size() { return size; }
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
/// \tparam max_definition_level The number of optional and repeated fields in the record path.
///                              Note that 'required' fields should not be counted!
template<typename T, std::size_t max_definition_level>
class DremelColumn {
 public:
    /// Creates a new column with a human-readable identifier.
    explicit DremelColumn(std::string identifier) : identifier(std::move(identifier)) {}

    /// Returns the human-readable identifier for this column.
    std::string& get_identifier() { return identifier; }

    /// Insert a new value into the column with a given repetition and definition level.
    /// Returns the TID of the inserted value.
    uint64_t insert(DremelRow<T> row) {
        rows.push_back({ row.value.value_or(nullptr), row.repetition_level, row.definition_level });
        return rows.size() - 1;
    }

    /// Retrieves a value together with its repetition and definition levels for a given TID.
    DremelRow<T> get(uint64_t tid) {
        auto& [value, r, d] = rows[tid];
        if (d < max_definition_level) {
          return { std::nullopt, r, d};
        } else {
          return { value, r, d};
        }
    }

 protected:
    /// Human-readable identifier of this column.
    std::string identifier;

    /// Null values are stored implicitly if the definition
    /// level is smaller than the maximum definition level.
    std::vector<std::tuple<T, unsigned, unsigned>> rows;
};
//---------------------------------------------------------------------------
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_INFRA_DREMEL_H_
//---------------------------------------------------------------------------
