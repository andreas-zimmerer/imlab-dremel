// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#include "imlab/dremel/schema_helper.h"

#include <utility>
//---------------------------------------------------------------------------
namespace imlab {
namespace dremel {
//---------------------------------------------------------------------------

template<> int32_t GetValue<int32_t>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetInt32(msg, field);
}
template<> int64_t GetValue<int64_t>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetInt64(msg, field);
}
template<> uint32_t GetValue<uint32_t>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetUInt32(msg, field);
}
template<> uint64_t GetValue<uint64_t>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetUInt64(msg, field);
}
template<> double GetValue<double>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetDouble(msg, field);
}
template<> float GetValue<float>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetFloat(msg, field);
}
template<> bool GetValue<bool>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetBool(msg, field);
}
template<> std::string GetValue<std::string>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetString(msg, field);
}
template<> const Message& GetValue<const Message&>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetMessage(msg, field);
}
// ---------------------------------------------------------------------------
template<> int32_t GetRepeatedValue<int32_t>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedInt32(msg, field, index);
}
template<> int64_t GetRepeatedValue<int64_t>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedInt64(msg, field, index);
}
template<> uint32_t GetRepeatedValue<uint32_t>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedUInt32(msg, field, index);
}
template<> uint64_t GetRepeatedValue<uint64_t>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedUInt64(msg, field, index);
}
template<> double GetRepeatedValue<double>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedDouble(msg, field, index);
}
template<> float GetRepeatedValue<float>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedFloat(msg, field, index);
}
template<> bool GetRepeatedValue<bool>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedBool(msg, field, index);
}
template<> std::string GetRepeatedValue<std::string>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedString(msg, field, index);
}
template<> const Message& GetRepeatedValue<const Message&>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedMessage(msg, field, index);
}
//---------------------------------------------------------------------------
template<> void SetValue(Message* msg, const FieldDescriptor* field, int32_t value) {
    msg->GetReflection()->SetInt32(msg, field, value);
}
template<> void SetValue(Message* msg, const FieldDescriptor* field, int64_t value) {
    msg->GetReflection()->SetInt64(msg, field, value);
}
template<> void SetValue(Message* msg, const FieldDescriptor* field, uint32_t value) {
    msg->GetReflection()->SetUInt32(msg, field, value);
}
template<> void SetValue(Message* msg, const FieldDescriptor* field, uint64_t value) {
    msg->GetReflection()->SetUInt64(msg, field, value);
}
template<> void SetValue(Message* msg, const FieldDescriptor* field, double value) {
    msg->GetReflection()->SetDouble(msg, field, value);
}
template<> void SetValue(Message* msg, const FieldDescriptor* field, float value) {
    msg->GetReflection()->SetFloat(msg, field, value);
}
template<> void SetValue(Message* msg, const FieldDescriptor* field, bool value) {
    msg->GetReflection()->SetBool(msg, field, value);
}
template<> void SetValue(Message* msg, const FieldDescriptor* field, std::string value) {
    msg->GetReflection()->SetString(msg, field, std::move(value));
}
//---------------------------------------------------------------------------
template<> void AddValue(Message* msg, const FieldDescriptor* field, int32_t value) {
    msg->GetReflection()->AddInt32(msg, field, value);
}
template<> void AddValue(Message* msg, const FieldDescriptor* field, int64_t value) {
    msg->GetReflection()->AddInt64(msg, field, value);
}
template<> void AddValue(Message* msg, const FieldDescriptor* field, uint32_t value) {
    msg->GetReflection()->AddUInt32(msg, field, value);
}
template<> void AddValue(Message* msg, const FieldDescriptor* field, uint64_t value) {
    msg->GetReflection()->AddUInt64(msg, field, value);
}
template<> void AddValue(Message* msg, const FieldDescriptor* field, double value) {
    msg->GetReflection()->AddDouble(msg, field, value);
}
template<> void AddValue(Message* msg, const FieldDescriptor* field, float value) {
    msg->GetReflection()->AddFloat(msg, field, value);
}
template<> void AddValue(Message* msg, const FieldDescriptor* field, bool value) {
    msg->GetReflection()->AddBool(msg, field, value);
}
template<> void AddValue(Message* msg, const FieldDescriptor* field, std::string value) {
    msg->GetReflection()->AddString(msg, field, std::move(value));
}

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
