// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_DREMEL_SHREDDING_H_
#define INCLUDE_IMLAB_DREMEL_SHREDDING_H_
//---------------------------------------------------------------------------
#include "./storage.h"
#include "./field_writer.h"
#include <google/protobuf/message.h>
//---------------------------------------------------------------------------
namespace imlab {
namespace dremel {
//---------------------------------------------------------------------------

class Shredder {
 public:
    /// Given a table, this function inserts a new record into this table
    /// by dissecting it into a columnar format.
    /// The value being inserted is a Protobuf message.
    static void DissectRecord(TableBase& table, Message& msg) {
        // For the top level we can be 100% sure that this is a ComplexFieldWriter
        DissectRecordInternal(&msg, dynamic_cast<ComplexFieldWriter*>(table.record_writer()), 0);
    }

 private:
    /// Decodes a given Protobuf message into a vector of individual fields to simplify handling them.
    static std::vector<ProtoFieldValue> DecodeMessage(const google::protobuf::Message* msg) {
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
                    for (int u = 0; u < ref->FieldSize(*msg, field); u++) {
                        fields.emplace_back(*msg, field, u);
                    }
                }
            }
        }
        return fields;
    }

    static void DissectRecordInternal(const google::protobuf::Message* msg, ComplexFieldWriter* writer, unsigned repetition_level) {
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
};

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_DREMEL_SHREDDING_H_
//---------------------------------------------------------------------------
