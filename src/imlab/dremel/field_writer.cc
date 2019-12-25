// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#include "imlab/dremel/field_writer.h"
//---------------------------------------------------------------------------
namespace imlab {
namespace dremel {
//---------------------------------------------------------------------------

template<> int32_t GetValue<int32_t>(const Reflection* ref, const Message& msg, const FieldDescriptor* field) {
    return ref->GetInt32(msg, field);
}
template<> int64_t GetValue<int64_t>(const Reflection* ref, const Message& msg, const FieldDescriptor* field) {
    return ref->GetInt64(msg, field);
}
template<> uint32_t GetValue<uint32_t>(const Reflection* ref, const Message& msg, const FieldDescriptor* field) {
    return ref->GetUInt32(msg, field);
}
template<> uint64_t GetValue<uint64_t>(const Reflection* ref, const Message& msg, const FieldDescriptor* field) {
    return ref->GetUInt64(msg, field);
}
template<> double GetValue<double>(const Reflection* ref, const Message& msg, const FieldDescriptor* field) {
    return ref->GetDouble(msg, field);
}
template<> float GetValue<float>(const Reflection* ref, const Message& msg, const FieldDescriptor* field) {
    return ref->GetFloat(msg, field);
}
template<> bool GetValue<bool>(const Reflection* ref, const Message& msg, const FieldDescriptor* field) {
    return ref->GetBool(msg, field);
}
template<> std::string GetValue<std::string>(const Reflection* ref, const Message& msg, const FieldDescriptor* field) {
    return ref->GetString(msg, field);
}
template<> const Message& GetValue<const Message&>(const Reflection* ref, const Message& msg, const FieldDescriptor* field) {
    return ref->GetMessage(msg, field);
}
// ---------------------------------------------------------------------------
template<> int32_t GetRepeatedValue<int32_t>(const Reflection* ref, const Message& msg, const FieldDescriptor* field, int index) {
    return ref->GetRepeatedInt32(msg, field, index);
}
template<> int64_t GetRepeatedValue<int64_t>(const Reflection* ref, const Message& msg, const FieldDescriptor* field, int index) {
    return ref->GetRepeatedInt64(msg, field, index);
}
template<> uint32_t GetRepeatedValue<uint32_t>(const Reflection* ref, const Message& msg, const FieldDescriptor* field, int index) {
    return ref->GetRepeatedUInt32(msg, field, index);
}
template<> uint64_t GetRepeatedValue<uint64_t>(const Reflection* ref, const Message& msg, const FieldDescriptor* field, int index) {
    return ref->GetRepeatedUInt64(msg, field, index);
}
template<> double GetRepeatedValue<double>(const Reflection* ref, const Message& msg, const FieldDescriptor* field, int index) {
    return ref->GetRepeatedDouble(msg, field, index);
}
template<> float GetRepeatedValue<float>(const Reflection* ref, const Message& msg, const FieldDescriptor* field, int index) {
    return ref->GetRepeatedFloat(msg, field, index);
}
template<> bool GetRepeatedValue<bool>(const Reflection* ref, const Message& msg, const FieldDescriptor* field, int index) {
    return ref->GetRepeatedBool(msg, field, index);
}
template<> std::string GetRepeatedValue<std::string>(const Reflection* ref, const Message& msg, const FieldDescriptor* field, int index) {
    return ref->GetRepeatedString(msg, field, index);
}
template<> const Message& GetRepeatedValue<const Message&>(const Reflection* ref, const Message& msg, const FieldDescriptor* field, int index) {
    return ref->GetRepeatedMessage(msg, field, index);
}

void WriteField(FieldWriter* writer, const ProtoFieldValue& value, unsigned repetition_level) {
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
