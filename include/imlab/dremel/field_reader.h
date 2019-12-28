// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_DREMEL_FIELD_READER_H_
#define INCLUDE_IMLAB_DREMEL_FIELD_READER_H_
//---------------------------------------------------------------------------
#include "./storage.h"
#include "./schema_helper.h"
#include <google/protobuf/message.h>
//---------------------------------------------------------------------------
namespace imlab {
namespace dremel {
//---------------------------------------------------------------------------
using namespace google::protobuf;

class FieldValue;

class IFieldReader {
    friend FieldValue;
 public:
    explicit IFieldReader(uint64_t index) : _current_index(index) {}
    virtual FieldValue Read(uint64_t index) = 0;
    virtual FieldValue ReadNext() = 0;
    virtual FieldValue Peek() = 0;
    virtual const FieldDescriptor* field() = 0;

 protected:
    virtual unsigned GetRepetitionLevel(FieldValue* field_value) = 0;
    virtual void AppendToRecord(FieldValue* field_value, Message* msg) = 0;
    virtual bool is_null(FieldValue* field_value) = 0;

    uint64_t _current_index;
};

class FieldValue {
 public:
    FieldValue(IFieldReader* reader, uint64_t index) : _reader(reader), _index(index) {}
    unsigned repetition_level() { return _reader->GetRepetitionLevel(this); }
    uint64_t index() { return _index; }
    bool is_null() { return _reader->is_null(this); }
    void AppendToRecord(Message* msg) { _reader->AppendToRecord(this, msg); }

 private:
    uint64_t _index;
    IFieldReader* _reader;
};

template<typename T>
void AppendToRecordGeneric(T value, const FieldDescriptor* field, Message* msg);

template<> inline void AppendToRecordGeneric(Integer value, const FieldDescriptor* field, Message* msg) {
    if (field->is_repeated()) {
        AddValue(msg, field, (int64_t)value.value);
    } else {
        SetValue(msg, field, (int64_t)value.value);
    }
}

template<> inline void AppendToRecordGeneric(Varchar<30> value, const FieldDescriptor* field, Message* msg) {
    if (field->is_repeated()) {
        AddValue(msg, field, (std::string)value.value);
    } else {
        SetValue(msg, field, (std::string)value.value);
    }
}

template<typename T>
class FieldReader: public IFieldReader {
 public:
    explicit FieldReader(DremelColumn<T>* column, uint64_t index)
        : IFieldReader(index), _column(column) {}

    FieldValue Read(uint64_t index) override { return { this, index }; }

    FieldValue ReadNext() { return Read(_current_index++); }

    FieldValue Peek() { return Read(_current_index + 1); }

    const FieldDescriptor* field() override { return _column->field(); };

 protected:
    unsigned GetRepetitionLevel(FieldValue* field_value) override {
        if (field_value->index() < _column->size()) {
            const auto &row = _column->get(field_value->index());
            return row.repetition_level;
        }
        return 0;
    }

    inline void AppendToRecord(FieldValue* field_value, Message* msg) override {
        const auto& row = _column->get(field_value->index());
        AppendToRecordGeneric(row.value.value(), _column->field(), msg);
    }

    bool is_null(FieldValue* field_value) {
        if (field_value->index() < _column->size()) {
            const auto &row = _column->get(field_value->index());
            return !row.value.has_value();
        }
        return true;
    }

 private:
    DremelColumn<T>* _column;
};

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_DREMEL_FIELD_READER_H_
//---------------------------------------------------------------------------
