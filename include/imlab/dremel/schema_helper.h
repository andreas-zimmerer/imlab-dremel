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

/// Gets the lowest common ancestor of two fields.
inline const FieldDescriptor* GetCommonAncestor(const FieldDescriptor* field1, const FieldDescriptor* field2) {
    if (field1 == nullptr || field2 == nullptr) {
        return nullptr;
    }

    std::vector<const FieldDescriptor*> field1_path {};
    std::vector<const FieldDescriptor*> field2_path {};

    // Collect the path of FieldDescriptors that lead to both fields (in reverse order!)
    auto* parent1 = field1;
    do {
        field1_path.push_back(parent1);
        parent1 = GetFieldDescriptor(parent1->containing_type());
    } while (parent1 != nullptr);
    auto* parent2 = field2;
    do {
        field2_path.push_back(parent2);
        parent2 = GetFieldDescriptor(parent2->containing_type());
    } while (parent2 != nullptr);

    if (*field1_path.rbegin() != *field2_path.rbegin()) {
        // They have nothing in common -> only message itself
        return nullptr;
    }

    // Index of lowest common ancestor counted from the end
    unsigned common_ancestor_index = 0;
    // Now we have two vectors of paths to the two fields in reversed order (root is at the end).
    for (unsigned i = 0; i < std::min(field1_path.size(), field2_path.size()); i++) {
        if (*(field1_path.rbegin() + i) == *(field2_path.rbegin() + i)) {
            common_ancestor_index = i;
        } else {
            break;
        }
    }

    // One of the fields is a leaf. Can happen when both fields are equal or one of them is an inner node above the other.
    bool one_is_leaf = (common_ancestor_index == field1_path.size() - 1)
                    || (common_ancestor_index == field2_path.size() - 1);

    // If we know that one if the fields is a leaf in the path, but both fields are not equal,
    // one of them is actually an inner node that is on the path of the other field.
    if (one_is_leaf && field1 != field2 && field1_path.size() < field2_path.size()) {
        return *(field1_path.rbegin() + common_ancestor_index);
    }

    // At this point we have the lowest field in the tree that is common for both fields.
    // Now we have to handle certain situations regarding field ordering.
    //  1) If the first field after the common ancestor has a smaller index than the second field
    //     (was declared before the second field), everything is fine because both fields belong to
    //     exactly this ancestor within the message.
    //  2) Else, although the two fields do have a common ancestor type, they do not belong to the same
    //     ancestor in the actual message. This is for example the case if field 1 comes after field 2
    //     when their parent type is repeated. Although they have the same parent type, they don't belong
    //     to the exact same parent, but to a repeated version. Because of that, the parent of the parent
    //     is their actual common ancestor.
    if (!one_is_leaf && (*(field1_path.rbegin() + common_ancestor_index + 1))->index() < (*(field2_path.rbegin() + common_ancestor_index + 1))->index()) {
        return *(field1_path.rbegin() + common_ancestor_index);
    } else {
        auto f = field1_path.rbegin() + common_ancestor_index;  // common_ancestor could also be the field itself if fields are equal.
        while (f != field1_path.rbegin() && !(*f)->is_repeated()) {
            f--;
        }
        if (f == field1_path.rbegin()) {
            return nullptr;  // this case happens when both fields need to be in different messages.
        }
        return *(--f);  // the field above.
    }
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
