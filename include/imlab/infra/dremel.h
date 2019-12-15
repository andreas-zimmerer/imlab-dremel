// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_INFRA_DREMEL_H_
#define INCLUDE_IMLAB_INFRA_DREMEL_H_
//---------------------------------------------------------------------------
#include <optional>
#include <utility>
#include <vector>
#include <string>
#include <tuple>
#include <google/protobuf/message.h>
//---------------------------------------------------------------------------
namespace imlab {
namespace dremel {
//---------------------------------------------------------------------------
using TID = uint64_t;
using namespace google::protobuf;

class FieldWriter;

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

/// The base class for a FieldWriter.
/// FieldWriters will form a tree-like structure that resembles the structure of a record type.
/// They are used for shredding a nested type into a columnar format.
/// Thus, a FieldWriter exposes a write() function that will insert a new row into one or more columns.
/// The tree-like structure consists of ComplexFieldWriter (inner node) and AtomicFieldWriter (leaf node).
/// The only reason to call `write()` on an inner node is to indicate a "missing field", and thus `write()`
/// only takes a `value` parameter when called on leaf nodes.
///
/// Every FieldWriter is furthermore associated with a definition level.
/// Handling of definition levels will be done for the caller. As long as the tree of FieldWriters is correct,
/// there is no need to manually handle definition levels.
class FieldWriter {
    friend class ComplexFieldWriter;
 public:
    /// Creates a new FieldWriter at the given definition level.
    /// When writing values, the given definition level will be used.
    /// The given repetition level will never be used for writing, but serves as a reference point.
    explicit FieldWriter(unsigned definition_level, unsigned repetition_level, unsigned field_id)
        : _definition_level(definition_level), _repetition_level(repetition_level), _field_id(field_id) {}
    /// Writes a 'null' value with the given repetition level to all columns underneath this writer.
    /// The definition level is determined by this writer.
    void write(unsigned repetition_level) { write(repetition_level, _definition_level - 1 /*minus itself*/); }
    /// Gets the definition level of this FieldWriter
    unsigned get_definition_level() { return _definition_level; }
    /// Gets the repetition level of this FieldWriter as a point of reference.
    unsigned get_repetition_level() { return _repetition_level; }
    /// Get the field number of the underlying Protobuf field
    unsigned get_field_id() { return _field_id; }
 protected:
    virtual void write(unsigned repetition_level, unsigned definition_level) = 0;
    const unsigned _repetition_level;
    const unsigned _definition_level;
    const unsigned _field_id;
};

/// A FieldWriter for a "complex" field; in other words an inner node.
/// The only task is to propagate the write call down to the actual columns,
/// represented by AtomicFieldWriters.
class ComplexFieldWriter : public FieldWriter {
 public:
    /// Creates a new ComplexFieldWriter at the given definition level and its child writers.
    ComplexFieldWriter(unsigned definition_level, unsigned repetition_level, unsigned field_id, std::vector<FieldWriter*> child_writers)
        : FieldWriter(definition_level, repetition_level, field_id), _child_writers(std::move(child_writers)) {}
    /// Returns the corresponding FieldWriter for the field with the given field_id
    std::optional<FieldWriter*> find_child_writer(unsigned field_id) {
        auto it = std::find_if(_child_writers.begin(), _child_writers.end(), [&](FieldWriter* c) { return c->get_field_id() == field_id; });
        return (it != _child_writers.end())? std::optional<FieldWriter*>(*it) : std::nullopt;
    };
 protected:
    void write(unsigned repetition_level, unsigned definition_level) override {
        for (auto& child : _child_writers) {
            child->write(repetition_level, definition_level);
        }
    }
 private:
    std::vector<FieldWriter*> _child_writers;
};

/// A FieldWriter that is actually associated with one (and only one!) column.
/// Its task is to write a given value into the respective column.
template<typename T>
class AtomicFieldWriter : public FieldWriter {
 public:
    /// Creates a new AtomicFieldWriter that is directly associated with a DremelColumn at a given definition level.
    AtomicFieldWriter(unsigned definition_level, unsigned repetition_level, unsigned field_id, DremelColumn<T>* column)
        : FieldWriter(definition_level, repetition_level, field_id), _column(column) {}
    /// Writes an explicitly given value into the column with the given repetition level.
    /// The definition level is the definition level of this writer.
    void write_value(T value, unsigned repetition_level) {
      _column->insert({ value, repetition_level, _definition_level });
    }
 protected:
    void write(unsigned repetition_level, unsigned definition_level) override {
      _column->insert({ std::nullopt, repetition_level, definition_level });
    }
 private:
    DremelColumn<T>* _column;
};

//---------------------------------------------------------------------------

// Protobuf does not provide a templated version to get the value of a field via reflection.
// Only functions like GetInt32() are provided.
// Same goes for the repeated versions like GetRepeatedInt32().
template<typename T>
T GetValue(const Reflection* ref, const Message& msg, const FieldDescriptor* field);
template<typename T>
T GetRepeatedValue(const Reflection* ref, const Message& msg, const FieldDescriptor* field, int index);


/// Represents one single value in a Protobuf message.
/// It's not always easy to work with raw Protobuf fields.
/// This class tries to unify the work with repeated vs non-repeated fields
/// and also getting the value out of a field.
/// When the underlying field is "repeated", this class represents only ONE of the values.
class ProtoFieldValue {
 public:
    ProtoFieldValue(const Message& msg, const FieldDescriptor* field)
        : _ref(msg.GetReflection()), _msg(msg), _field(field), _index(0) {}
    ProtoFieldValue(const Message& msg, const FieldDescriptor* field, const unsigned index)
        : _ref(msg.GetReflection()), _msg(msg), _field(field), _index(index) {}

    /// Get the underlying FieldDescriptor of this ProtoFieldValue
    [[nodiscard]] const FieldDescriptor* GetField() const { return _field; }

    /// Checks whether the field is empty or not.
    [[nodiscard]] bool HasValue() const {
        if (_field->is_repeated()) {
            auto size = _ref->FieldSize(_msg, _field);
            return size > 0 && _index < size;
        } else {
            return _ref->HasField(_msg, _field);
        }
    }

    /// Checks whether the underlying field is atomic or a combination of other values.
    [[nodiscard]] bool IsAtomic() const {
        return _field->type() != FieldDescriptor::Type::TYPE_MESSAGE
            && _field->type() != FieldDescriptor::Type::TYPE_GROUP;
    }

    /// Get the value of the field. Only works if HasValue() is true.
    template<typename T>
    T GetFieldValue() const {
        if (_field->is_repeated()) {
            return GetRepeatedValue<T>(_ref, _msg, _field, _index);
        } else {
            return GetValue<T>(_ref, _msg, _field);
        }
    }

 private:
    const Reflection* _ref;
    const Message& _msg;
    const FieldDescriptor* _field;
    const unsigned _index;
};

/// Given a table, this function inserts a new record into this table
/// by dissecting it into a columnar format.
/// The value being inserted is a Protobuf message.
void DissectRecord(TableBase& table, Message& msg);

void AssembleRecord();

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_INFRA_DREMEL_H_
//---------------------------------------------------------------------------
