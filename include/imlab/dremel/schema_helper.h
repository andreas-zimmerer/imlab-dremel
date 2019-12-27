// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_DREMEL_SCHEMA_HELPER_H_
#define INCLUDE_IMLAB_DREMEL_SCHEMA_HELPER_H_
//---------------------------------------------------------------------------
#include <google/protobuf/message.h>
//---------------------------------------------------------------------------
namespace imlab {
namespace dremel {
//---------------------------------------------------------------------------
using namespace google::protobuf;

// Protobuf does not provide a templated version to get/set the value of a field via reflection.
// Only functions like GetInt32() are provided. Same goes for the repeated versions like GetRepeatedInt32().
template<typename T>
T GetValue(const Message& msg, const FieldDescriptor* field);
template<typename T>
T GetRepeatedValue(const Message& msg, const FieldDescriptor* field, int index);
template<typename T>
void SetValue(Message* msg, const FieldDescriptor* field, T value);
template<typename T>
void AddValue(Message* msg, const FieldDescriptor* field, T value);

/// Converts a given Descriptor to a FieldDescriptor.
/// Each Descriptor, except for the root, also has a FieldDescriptor associated with it,
/// but it's kind of hard to get there.
/// Returns a pointer to the FieldDescriptor or nullptr if we are at the root.
inline const FieldDescriptor* GetFieldDescriptor(const Descriptor* desc) {
    auto* parent = desc->containing_type();
    if (parent == nullptr) {  // root
        return nullptr;
    }
    for (int i = 0; i < parent->field_count(); i++) {
        if (parent->field(i)->message_type() == desc) {
            return parent->field(i);
        }
    }
    assert(false);
}

/// Computes the repetition level for a given field.
/// The repetition level is the number of REPEATED fields in the path.
inline unsigned GetMaxRepetitionLevel(const FieldDescriptor* desc) {
    unsigned repetition_level = 0;

    auto* field = desc;
    while (field != nullptr) {
        if (field->is_repeated()) {
            repetition_level++;
        }
        field = GetFieldDescriptor(field->containing_type());
    }
    return repetition_level;
}

/// Computes the repetition level of the lowest common ancestor of two fields.
inline unsigned GetCommonRepetitionLevel(const FieldDescriptor* field1, const FieldDescriptor* field2) {
    std::vector<const Descriptor*> field1_path {};
    std::vector<const Descriptor*> field2_path {};

    auto* parent1 = field1->containing_type();
    while (parent1 != nullptr) {
        field1_path.push_back(parent1);
        parent1 = parent1->containing_type();
    }
    auto* parent2 = field2->containing_type();
    while (parent2 != nullptr) {
        field2_path.push_back(parent2);
        parent2 = parent2->containing_type();
    }

    // Index of lowest common ancestor with respect to field1_path
    unsigned common_ancestor_index = 0;
    // Now we have two vectors of paths to the two fields in reversed order (root is at the end).
    for (unsigned i = 1; i <= std::min(field1_path.size(), field2_path.size()); i++) {
        if (field1_path[field1_path.size() - i] == field2_path[field2_path.size() - i]) {
            common_ancestor_index = field1_path.size() - i;
        }
    }
    return GetMaxRepetitionLevel(GetFieldDescriptor(field1_path[common_ancestor_index]));
}

/// Computes the definition level of a given field.
/// The definition level is the number of optional and repeated fields in the path.
inline unsigned GetDefinitionLevel(const FieldDescriptor* desc) {
    unsigned definition_level = 0;

    auto* field = desc;
    while (field != nullptr) {
        if (field->is_repeated() || field->is_optional()) {
            definition_level++;
        }
        field = GetFieldDescriptor(field->containing_type());
    }
    return definition_level;
}

/// In contrast to a "normal" definition level, the full definition level takes
/// all fields in the path into account.
inline unsigned GetFullDefinitionLevel(const FieldDescriptor* desc) {
    unsigned full_definition_level = 0;

    auto* field = desc;
    while (field != nullptr) {
        full_definition_level++;
        field = GetFieldDescriptor(field->containing_type());
    }
    return full_definition_level;
}



// ---------------------------------------------------------------------------
// Type-specific implementations of
// GetValue<>(), GetRepeatedValue<>(), SetValue<>(), AddValue<>()
// ---------------------------------------------------------------------------
template<> inline int32_t GetValue<int32_t>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetInt32(msg, field);
}
template<> inline int64_t GetValue<int64_t>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetInt64(msg, field);
}
template<> inline uint32_t GetValue<uint32_t>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetUInt32(msg, field);
}
template<> inline uint64_t GetValue<uint64_t>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetUInt64(msg, field);
}
template<> inline double GetValue<double>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetDouble(msg, field);
}
template<> inline float GetValue<float>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetFloat(msg, field);
}
template<> inline bool GetValue<bool>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetBool(msg, field);
}
template<> inline std::string GetValue<std::string>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetString(msg, field);
}
template<> inline const Message& GetValue<const Message&>(const Message& msg, const FieldDescriptor* field) {
    return msg.GetReflection()->GetMessage(msg, field);
}
// ---------------------------------------------------------------------------
template<> inline int32_t GetRepeatedValue<int32_t>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedInt32(msg, field, index);
}
template<> inline int64_t GetRepeatedValue<int64_t>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedInt64(msg, field, index);
}
template<> inline uint32_t GetRepeatedValue<uint32_t>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedUInt32(msg, field, index);
}
template<> inline uint64_t GetRepeatedValue<uint64_t>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedUInt64(msg, field, index);
}
template<> inline double GetRepeatedValue<double>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedDouble(msg, field, index);
}
template<> inline float GetRepeatedValue<float>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedFloat(msg, field, index);
}
template<> inline bool GetRepeatedValue<bool>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedBool(msg, field, index);
}
template<> inline std::string GetRepeatedValue<std::string>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedString(msg, field, index);
}
template<> inline const Message& GetRepeatedValue<const Message&>(const Message& msg, const FieldDescriptor* field, int index) {
    return msg.GetReflection()->GetRepeatedMessage(msg, field, index);
}
//---------------------------------------------------------------------------
template<> inline void SetValue(Message* msg, const FieldDescriptor* field, int32_t value) {
    msg->GetReflection()->SetInt32(msg, field, value);
}
template<> inline void SetValue(Message* msg, const FieldDescriptor* field, int64_t value) {
    msg->GetReflection()->SetInt64(msg, field, value);
}
template<> inline void SetValue(Message* msg, const FieldDescriptor* field, uint32_t value) {
    msg->GetReflection()->SetUInt32(msg, field, value);
}
template<> inline void SetValue(Message* msg, const FieldDescriptor* field, uint64_t value) {
    msg->GetReflection()->SetUInt64(msg, field, value);
}
template<> inline void SetValue(Message* msg, const FieldDescriptor* field, double value) {
    msg->GetReflection()->SetDouble(msg, field, value);
}
template<> inline void SetValue(Message* msg, const FieldDescriptor* field, float value) {
    msg->GetReflection()->SetFloat(msg, field, value);
}
template<> inline void SetValue(Message* msg, const FieldDescriptor* field, bool value) {
    msg->GetReflection()->SetBool(msg, field, value);
}
template<> inline void SetValue(Message* msg, const FieldDescriptor* field, std::string value) {
    msg->GetReflection()->SetString(msg, field, std::move(value));
}
//---------------------------------------------------------------------------
template<> inline void AddValue(Message* msg, const FieldDescriptor* field, int32_t value) {
    msg->GetReflection()->AddInt32(msg, field, value);
}
template<> inline void AddValue(Message* msg, const FieldDescriptor* field, int64_t value) {
    msg->GetReflection()->AddInt64(msg, field, value);
}
template<> inline void AddValue(Message* msg, const FieldDescriptor* field, uint32_t value) {
    msg->GetReflection()->AddUInt32(msg, field, value);
}
template<> inline void AddValue(Message* msg, const FieldDescriptor* field, uint64_t value) {
    msg->GetReflection()->AddUInt64(msg, field, value);
}
template<> inline void AddValue(Message* msg, const FieldDescriptor* field, double value) {
    msg->GetReflection()->AddDouble(msg, field, value);
}
template<> inline void AddValue(Message* msg, const FieldDescriptor* field, float value) {
    msg->GetReflection()->AddFloat(msg, field, value);
}
template<> inline void AddValue(Message* msg, const FieldDescriptor* field, bool value) {
    msg->GetReflection()->AddBool(msg, field, value);
}
template<> inline void AddValue(Message* msg, const FieldDescriptor* field, std::string value) {
    msg->GetReflection()->AddString(msg, field, std::move(value));
}

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_DREMEL_SCHEMA_HELPER_H_
//---------------------------------------------------------------------------
