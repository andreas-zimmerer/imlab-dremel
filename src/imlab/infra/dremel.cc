// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#include "imlab/infra/dremel.h"
#include <unordered_set>

using namespace google::protobuf;

namespace imlab {
namespace dremel {

void WriteField(FieldWriter* writer, unsigned repetition_level, const FieldDescriptor* field, const Reflection* ref, const Message* msg) {
    switch (field->cpp_type()) {
        case FieldDescriptor::CPPTYPE_INT32:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<int32_t>*>(writer)->write_value(
                        ref->GetRepeatedInt32(*msg, field, i), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<int32_t>*>(writer)->write_value(
                    ref->GetInt32(*msg, field), repetition_level);
            }
            break;
        case FieldDescriptor::CPPTYPE_INT64:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<int64_t>*>(writer)->write_value(
                        ref->GetRepeatedInt64(*msg, field, i), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<int64_t>*>(writer)->write_value(
                    ref->GetInt64(*msg, field), repetition_level);
            }
            break;
        case FieldDescriptor::CPPTYPE_UINT32:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<uint32_t>*>(writer)->write_value(
                        ref->GetRepeatedUInt32(*msg, field, i), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<uint32_t>*>(writer)->write_value(
                    ref->GetUInt32(*msg, field), repetition_level);
            }
            break;
        case FieldDescriptor::CPPTYPE_UINT64:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<uint64_t>*>(writer)->write_value(
                        ref->GetRepeatedUInt64(*msg, field, i), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<uint64_t>*>(writer)->write_value(
                    ref->GetUInt64(*msg, field), repetition_level);
            }
            break;
        case FieldDescriptor::CPPTYPE_DOUBLE:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<double>*>(writer)->write_value(
                        ref->GetRepeatedDouble(*msg, field, i), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<double>*>(writer)->write_value(
                    ref->GetDouble(*msg, field), repetition_level);
            }
            break;
        case FieldDescriptor::CPPTYPE_FLOAT:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<float>*>(writer)->write_value(
                        ref->GetRepeatedFloat(*msg, field, i), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<float>*>(writer)->write_value(
                    ref->GetFloat(*msg, field), repetition_level);
            }
            break;
        case FieldDescriptor::CPPTYPE_BOOL:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<bool>*>(writer)->write_value(
                        ref->GetRepeatedBool(*msg, field, i), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<bool>*>(writer)->write_value(
                    ref->GetBool(*msg, field), repetition_level);
            }
            break;
        case FieldDescriptor::CPPTYPE_ENUM:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<int>*>(writer)->write_value(
                        ref->GetRepeatedEnumValue(*msg, field, i), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<int>*>(writer)->write_value(
                    ref->GetEnumValue(*msg, field), repetition_level);
            }
            break;
        case FieldDescriptor::CPPTYPE_STRING:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<std::string>*>(writer)->write_value(
                        ref->GetRepeatedString(*msg, field, i), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<std::string>*>(writer)->write_value(
                    ref->GetString(*msg, field), repetition_level);
            }
            break;
        case FieldDescriptor::CPPTYPE_MESSAGE:break;
    }
}


void DissectRecordInternal(const google::protobuf::Message* msg, ComplexFieldWriter* writer, unsigned repetition_level) {
    // A set of messages (their field_ids) that have already been processed. Needed for computing repetition levels.
    std::unordered_set<unsigned> seen_fields {};

    const Descriptor* desc = msg->GetDescriptor();
    const Reflection* ref = msg->GetReflection();
    for (int i = 0; i < desc->field_count(); i++) {
        const FieldDescriptor* field = desc->field(i);

        // Find the corresponding FieldWriter for this message.
        const auto child_writer = writer->find_child_writer(field->number());
        if (child_writer == std::nullopt) {
            // ERROR: message format does not comply to the table.
        }

        // Compute new repetition level.
        unsigned child_repetition_level = repetition_level;
        if (seen_fields.find(child_writer.value()->get_field_id()) != seen_fields.end()) {
            // We already saw that field -> repetition level is current level of the FieldWriter.
            child_repetition_level = child_writer.value()->get_definition_level();
        } else {
            // We have a new field that we didn't see before.
            seen_fields.insert(child_writer.value()->get_field_id());
        }

        // Write into columns.
        if (!ref->HasField(*msg, field)) {
            child_writer.value()->write(child_repetition_level);
        } else if (field->type() != FieldDescriptor::Type::TYPE_MESSAGE) {
            WriteField(child_writer.value(), repetition_level, field, ref, msg);
        } else {
            // Descend deeper into the message.
            if (field->is_repeated()) {
                for (int m = 0; m < ref->FieldSize(*msg, field); m++) {
                    DissectRecordInternal(&(ref->GetRepeatedMessage(*msg, field, m)), dynamic_cast<ComplexFieldWriter*>(child_writer.value()), child_repetition_level);
                }
            } else {
                DissectRecordInternal(&(ref->GetMessage(*msg, field)), dynamic_cast<ComplexFieldWriter*>(child_writer.value()), child_repetition_level);
            }
        }
    }
}

void DissectRecord(TableBase& table, google::protobuf::Message& msg) {
    // For the top level we can be 100% sure that this is a ComplexFieldWriter
    DissectRecordInternal(&msg, dynamic_cast<ComplexFieldWriter*>(table.get_record_writer()), 0);
}

}  // namespace dremel
}  // namespace imlab
