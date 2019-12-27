// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_DREMEL_FIELD_WRITER_H_
#define INCLUDE_IMLAB_DREMEL_FIELD_WRITER_H_
//---------------------------------------------------------------------------
#include <vector>
#include "./storage.h"
#include "./schema_helper.h"
#include "imlab/infra/hash.h"
#include "imlab/infra/types.h"
#include <google/protobuf/descriptor.h>
//---------------------------------------------------------------------------
namespace imlab {
namespace dremel {
//---------------------------------------------------------------------------
using namespace google::protobuf;

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
    [[nodiscard]] T GetFieldValue() const {
        if (_field->is_repeated()) {
            return GetRepeatedValue<T>(_msg, _field, _index);
        } else {
            return GetValue<T>(_msg, _field);
        }
    }

 private:
    const Reflection* _ref;
    const Message& _msg;
    const FieldDescriptor* _field;
    const unsigned _index;
};


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
    explicit FieldWriter(const FieldDescriptor* field)
        : _definition_level(GetDefinitionLevel(field)), _repetition_level(GetMaxRepetitionLevel(field)), _field_id(field->number()) {}
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
    ComplexFieldWriter(const FieldDescriptor* field, std::vector<FieldWriter*> child_writers)
        : FieldWriter(field), _child_writers(std::move(child_writers)) {}
    /// Returns the corresponding FieldWriter for the field with the given field_number
    std::optional<FieldWriter*> find_child_writer(unsigned field_number) {
        auto it = std::find_if(_child_writers.begin(), _child_writers.end(), [&](FieldWriter* c) { return c->get_field_id() == field_number; });
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
    /// Creates a new AtomicFieldWriter that is directly associated with a DremelColumn.
    explicit AtomicFieldWriter(DremelColumn<T>* column)
        : FieldWriter(column->field()), _column(column) {}
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


/// Writes a given value into the table with a FieldWriter at a given repetition level.
inline void WriteField(FieldWriter* writer, const ProtoFieldValue& value, unsigned repetition_level) {
    if (!value.HasValue()) {
        writer->write(repetition_level);
        return;
    }

    switch (value.GetField()->cpp_type()) {
        case FieldDescriptor::CPPTYPE_INT32:
            dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                Integer(value.GetFieldValue<int32_t>()), repetition_level);
            break;
        case FieldDescriptor::CPPTYPE_INT64:
            dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                Integer(value.GetFieldValue<int64_t>()), repetition_level);
            break;
        case FieldDescriptor::CPPTYPE_UINT32:
            dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                Integer(value.GetFieldValue<uint32_t>()), repetition_level);
            break;
        case FieldDescriptor::CPPTYPE_UINT64:
            dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                Integer(value.GetFieldValue<uint64_t>()), repetition_level);
            break;
        case FieldDescriptor::CPPTYPE_STRING:
            dynamic_cast<AtomicFieldWriter<Varchar<30>>*>(writer)->write_value(
                Varchar<30>::build(value.GetFieldValue<std::string>().c_str()), repetition_level);
            break;
        case FieldDescriptor::CPPTYPE_DOUBLE:  // UNSUPPRTED
        case FieldDescriptor::CPPTYPE_FLOAT:  // UNSUPPRTED
        case FieldDescriptor::CPPTYPE_BOOL:  // UNSUPPRTED
        case FieldDescriptor::CPPTYPE_ENUM:  // UNSUPPRTED
        case FieldDescriptor::CPPTYPE_MESSAGE:  // INVALID
            break;
    }
}

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_DREMEL_FIELD_WRITER_H_
//---------------------------------------------------------------------------
