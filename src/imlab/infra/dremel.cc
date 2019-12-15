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

}  // namespace dremel
}  // namespace imlab
