// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#include "imlab/infra/dremel.h"
#include <unordered_set>
#include "imlab/infra/types.h"

using namespace google::protobuf;

namespace imlab {
namespace dremel {

void WriteField(FieldWriter* writer, unsigned repetition_level, const FieldDescriptor* field, const Reflection* ref, const Message* msg) {
    switch (field->cpp_type()) {
        case FieldDescriptor::CPPTYPE_INT32:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                        Integer(ref->GetRepeatedInt32(*msg, field, i)), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                    Integer(ref->GetInt32(*msg, field)), repetition_level);
            }
            break;
        case FieldDescriptor::CPPTYPE_INT64:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                        Integer(ref->GetRepeatedInt64(*msg, field, i)), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                    Integer(ref->GetInt64(*msg, field)), repetition_level);
            }
            break;
        case FieldDescriptor::CPPTYPE_UINT32:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                        Integer(ref->GetRepeatedUInt32(*msg, field, i)), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                    Integer(ref->GetUInt32(*msg, field)), repetition_level);
            }
            break;
        case FieldDescriptor::CPPTYPE_UINT64:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                        Integer(ref->GetRepeatedUInt64(*msg, field, i)), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                    Integer(ref->GetUInt64(*msg, field)), repetition_level);
            }
            break;
        case FieldDescriptor::CPPTYPE_DOUBLE:  // UNSUPPORTED
            break;
        case FieldDescriptor::CPPTYPE_FLOAT:  // UNSUPPORTED
            break;
        case FieldDescriptor::CPPTYPE_BOOL:  // UNSUPPORTED
            break;
        case FieldDescriptor::CPPTYPE_ENUM:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                        Integer(ref->GetRepeatedEnumValue(*msg, field, i)), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<Integer>*>(writer)->write_value(
                    Integer(ref->GetEnumValue(*msg, field)), repetition_level);
            }
            break;
        case FieldDescriptor::CPPTYPE_STRING:
            if (field->is_repeated()) {
                for (int i = 0; i < ref->FieldSize(*msg, field); i++) {
                    dynamic_cast<AtomicFieldWriter<Varchar<30>>*>(writer)->write_value(
                        Varchar<30>::build(ref->GetRepeatedString(*msg, field, i).c_str()), repetition_level);
                }
            } else {
                dynamic_cast<AtomicFieldWriter<Varchar<30>>*>(writer)->write_value(
                    Varchar<30>::build(ref->GetString(*msg, field).c_str()), repetition_level);
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
        if (field->type() != FieldDescriptor::Type::TYPE_MESSAGE && field->type() != FieldDescriptor::Type::TYPE_GROUP) {
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
