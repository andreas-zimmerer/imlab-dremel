// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_INFRA_DREMEL_H_
#define INCLUDE_IMLAB_INFRA_DREMEL_H_
//---------------------------------------------------------------------------
#include <vector>
#include <string>
#include <optional>
//---------------------------------------------------------------------------
namespace imlab {
//---------------------------------------------------------------------------

template<typename T>
class TableBase<T> {
 public:
    virtual uint64_t insert(T tuple) = 0;
    uint64_t get_size() { return size; }
 protected:
    uint64_t size = 0;
};

/// A row in Dremel consists of
///  * a value
///  * a repetition level
///  * a definition level
template<typename T>
struct DremelRow<T> {
    std::optional<T> value;
    unsigned repetition_level;
    unsigned definition_level;
};

/// A column in the Dremel format.
/// What's special about a Dremel column is that not only values are stored,
/// but also repetition and definition levels for every value.
/// Values can also be NULL. This is required for reconstructing the object.
///
/// Usually, a column is stored a little more efficient in Dremel,
/// e.g. only required repetition and definition levels are stored while others
/// are inferred. Also, r and d levels only take up as many bits as they actually
/// need.
template<typename T>
class DremelColumn<T> {
 public:
    /// Creates a new column with a human-readable identifier.
    explicit DremelColumn(std::string identifier) : identifier(identifier) {}

    /// Returns the human-readable identifier for this column.
    std::string get_identifier() { return identifier; }

    /// Insert a new value into the column with a given repetition and definition level.
    /// Returns the TID of the inserted value.
    uint64_t insert(DremelRow<T> row) {
        rows.push_back(row);
        return rows.size() - 1;
    }

    /// Retrieves a value together with its repetition and definition levels for a given TID.
    DremelRow<T> get(uint64_t tid) {
        return rows[tid];
    }

 protected:
    std::string identifier;
    std::vector<DremelRow<T>> rows;
};
//---------------------------------------------------------------------------
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_INFRA_DREMEL_H_
//---------------------------------------------------------------------------
