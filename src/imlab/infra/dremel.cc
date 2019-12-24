// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#include "imlab/infra/dremel.h"
#include <unordered_set>
#include "imlab/infra/types.h"

namespace imlab {
namespace dremel {

using namespace google::protobuf;

// ---------------------------------------------------------------------------
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
// ---------------------------------------------------------------------------

/// Writes a given value into the table with a FieldWriter at a given repetition level.
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


/// Decodes a given Protobuf message into a vector of individual fields to simplify handling them.
std::vector<ProtoFieldValue> DecodeMessage(const google::protobuf::Message* msg) {
    const Descriptor* desc = msg->GetDescriptor();
    const Reflection* ref = msg->GetReflection();

    std::vector<ProtoFieldValue> fields;
    for (int i = 0; i < desc->field_count(); i++) {
        const FieldDescriptor *field = desc->field(i);

        if (!field->is_repeated()) {
            // A non-repeated field simply gets a ProtoFieldValue.
            fields.emplace_back(*msg, field);
        } else {
            if (ref->FieldSize(*msg, field) == 0) {
                // Create a placeholder field for the empty repetition.
                fields.emplace_back(*msg, field);
            } else {
                // For every value in the repeated field, there will be a corresponding ProtoFieldValue.
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    fields.emplace_back(*msg, field, i);
                }
            }
        }
    }
    return fields;
}


void DissectRecordInternal(const google::protobuf::Message* msg, ComplexFieldWriter* writer, unsigned repetition_level) {
    // A set of messages (their field_ids) that have already been processed. Needed for computing repetition levels.
    std::unordered_set<unsigned> seen_fields {};

    // Get a list of all fields in this message (not recursive, just flat!)
    const auto& fields = DecodeMessage(msg);

    for (auto& field : fields) {
        // Find the corresponding FieldWriter for this field.
        // .value() only fails, if the Protobuf message does not correspond to the table format.
        const auto child_writer = writer->find_child_writer(field.GetField()->number()).value();

        // Compute new repetition level.
        unsigned child_repetition_level = repetition_level;
        if (seen_fields.find(child_writer->get_field_id()) != seen_fields.end()) {
            // We already saw that field.
            // Now we can't simply take the definition level of the child_writer as the repetition level.
            // Why? Because the definition level is "the number of optional and repeated fields in the path"
            // and repetition levels is just "the number of repeated fields in the path".
            child_repetition_level = child_writer->get_repetition_level();
        } else {
            // We have a new field that we didn't see before.
            seen_fields.insert(child_writer->get_field_id());
        }

        if (field.IsAtomic() || !field.HasValue()) {
            // This is not really depicted in the algorithm from the Dremel paper.
            // However, we also need to write an entry, if a field is not set (even if it's not atomic).
            WriteField(child_writer, field, child_repetition_level);
        } else if (field.HasValue()) {
            // Descend deeper into the message.
            // Because the field is not atomic, we know that it corresponds to a ComplexFieldWriter.
            DissectRecordInternal(&field.GetFieldValue<const Message&>(),
                dynamic_cast<ComplexFieldWriter*>(child_writer),
                child_repetition_level);
        }
    }
}

void DissectRecord(TableBase& table, google::protobuf::Message& msg) {
    // For the top level we can be 100% sure that this is a ComplexFieldWriter
    DissectRecordInternal(&msg, dynamic_cast<ComplexFieldWriter*>(table.get_record_writer()), 0);
}

// ---------------------------------------------------------------------------

void RecordFSM::ConstructRecordFSM(const std::vector<Field>& fields) {
    for (unsigned i = 0; i < fields.size(); i++) {
        auto& field = fields[i];
        auto maxRepetitionLevel = field.max_repetition_level;
        // Either next element or nullopt if we are at the last field.
        auto barrier = (i == fields.size() - 1)? std::nullopt : std::make_optional(fields[i+1]);
        // "common repetition level of barrier and field" -> Minimum of both.
        // If we are at the end, repetition level is 0
        auto barrierLevel = std::min(maxRepetitionLevel, barrier.has_value()? barrier.value().max_repetition_level : 0);

        // Insert a default reflexive transition with the repetition level of this field.
        // It might be overridden later on.
        // This is not explicitly mentioned in the paper.
        _transitions.insert_or_assign(std::make_pair(field.identifier, maxRepetitionLevel), field.identifier);

        // Work on all backward edges of the FSM.
        // To refer to the paper, this is an edge like from "Name.Url" to "Name.Language.Code" in Figure 4.
        for (unsigned u = 0; u < i; u++) {
            auto& preField = fields[u];
            if (preField.max_repetition_level > barrierLevel) {
                // Get common repetition level of preField and field
                auto backLevel = std::min(preField.max_repetition_level, field.max_repetition_level);
                // Insert a transition into the FSM
                _transitions.insert_or_assign(std::make_pair(field.identifier, backLevel), preField.identifier);
            }
        }

        // Handle transitions with multiple repetition levels to one destination.
        for (unsigned level = barrierLevel + 1; level <= maxRepetitionLevel; level++) {
            // If we don't have a transition from (field,level) yet, we copy it from (field,level-1).
            if (_transitions.find(std::make_pair(field.identifier, level)) == _transitions.end()) {
                const auto& pre = _transitions.find(std::make_pair(field.identifier, level - 1));
                if (pre == _transitions.end()) {
                    assert(false);
                }
                _transitions.insert_or_assign(std::make_pair(field.identifier, level - 1), pre->second);
            }
        }

        // Handle forward transition to the next field.
        if (barrier) { // TODO: ?? But only insert the transitions if the barrier is not the last element.
            for (unsigned level = 0; level <= barrierLevel; level++) {
                _transitions.insert_or_assign(std::make_pair(field.identifier, level), barrier.value().identifier);
            }
        }
    }
}

std::string RecordFSM::GenerateGraphviz() {
    std::stringstream ss {};
    ss << "digraph finite_state_machine {" << std::endl;
    ss << "    size=\"8,5\"" << std::endl;
    ss << "    node [shape = rect, style = rounded];" << std::endl;
    ss << std::endl;

    // Set of all nodes
    std::unordered_set<std::string> nodes {};

    // Map of transitions with (source, target) -> [repetition_level, ...]
    std::unordered_multimap<std::pair<std::string, std::string>, unsigned> edges {};

    for (const auto& [key, value] : _transitions) {
        auto& [source, level] = key;
        auto& target = value;
        nodes.emplace(source);
        nodes.emplace(target);
        edges.emplace(std::make_pair(source, target), level);
    }

    for (auto& source : nodes) {
        for (auto& target : nodes) {
            if (edges.find(std::make_pair(source, target)) != edges.end()) {
                auto source_name = source;
                std::replace(source_name.begin(), source_name.end(), '.', '_');
                auto target_name = target;
                std::replace(target_name.begin(), target_name.end(), '.', '_');

                ss << "    " << source_name << " -> " << target_name << " [label=\"[";
                // Print repetition levels as labels on edges:
                auto level_range = edges.equal_range(std::make_pair(source, target));
                for (auto &level_it = level_range.first; level_it != level_range.second; level_it++) {
                    ss << level_it->second << ",";
                }
                ss << "]\"];" << std::endl;
            }
        }
    }

    ss << "}" << std::endl;
    return ss.str();
}

}  // namespace dremel
}  // namespace imlab
