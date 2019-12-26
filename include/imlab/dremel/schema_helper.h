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

/// Converts a given Descriptor to a FieldDescriptor.
/// Each Descriptor, except for the root, also has a FieldDescriptor associated with it,
/// but it's kind of hard to get there.
/// Returns a pointer to the FieldDescriptor or nullptr if we are at the root.
const FieldDescriptor* GetFieldDescriptor(const Descriptor* desc) {
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
unsigned GetMaxRepetitionLevel(const FieldDescriptor* desc) {
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
unsigned GetCommonRepetitionLevel(const FieldDescriptor* field1, const FieldDescriptor* field2) {
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
unsigned GetDefinitionLevel(const FieldDescriptor* desc) {
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
unsigned GetFullDefinitionLevel(const FieldDescriptor* desc) {
    unsigned full_definition_level = 0;

    auto* field = desc;
    while (field != nullptr) {
        full_definition_level++;
        field = GetFieldDescriptor(field->containing_type());
    }
    return full_definition_level;
}

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_DREMEL_SCHEMA_HELPER_H_
//---------------------------------------------------------------------------
