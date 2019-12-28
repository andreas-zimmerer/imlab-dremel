// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_DREMEL_ASSEMBLING_H_
#define INCLUDE_IMLAB_DREMEL_ASSEMBLING_H_
//---------------------------------------------------------------------------
#include <vector>
#include "./storage.h"
#include "./field_reader.h"
#include "./record_fsm.h"
#include "./schema_helper.h"
#include <google/protobuf/message.h>
//---------------------------------------------------------------------------
namespace imlab {
namespace dremel {
//---------------------------------------------------------------------------
using namespace google::protobuf;
using TID = uint64_t;

template<class R>
class Assembler {
    static_assert(std::is_base_of<Message, R>::value, "R must be derived from google::protobuf::Message");

 public:
    /// Assembles a shredded record from a column-store table.
    R AssembleRecord(RecordFSM& fsm, std::vector<IFieldReader*>& readers) {
        R record {};  // Record that is assembled
        msg_stack = { &record };
        last_reader = nullptr;
        reader = *readers.begin();

        while (true) {
            auto value = reader->ReadNext();
            if (!value.is_null()) {
                MoveToLevel();

                value.AppendToRecord(msg_stack[msg_stack.size() - 1]);
            } else {
                MoveToLevel();
            }

            auto* next_field = fsm.NextField((reader)->field(), (reader)->Peek().repetition_level());
            auto next_reader = std::find_if(readers.begin(), readers.end(), [&](const auto& r) { return r->field() == next_field; });
            if (next_reader == readers.end()) {
                break;
            }
            reader = *next_reader;
            ReturnToLevel();
        }

        //ReturnToLevel(0, last_reader, *reader);
        // TODO: end all nested records

        return record;
    }

 protected:
    void MoveToLevel() {
        const auto* common_ancestor = GetCommonAncestor(last_reader? last_reader->field() : nullptr, reader->field());
        auto common_ancestor_level = GetFullDefinitionLevel(common_ancestor);

        // Unwind message stack.
        for (int i = 0; i < msg_stack.size() - common_ancestor_level - 1/*don't pop root*/; i++) {
            msg_stack.pop_back();
        }

        // Re-build message stack accordingly.
        std::vector<const FieldDescriptor*> parents {};
        auto* parent_field = GetFieldDescriptor(reader->field()->containing_type());
        for (int i = 0; i < GetFullDefinitionLevel(reader->field()) - common_ancestor_level - 1; i++) {
            parents.push_back(parent_field);
            parent_field = GetFieldDescriptor(parent_field->containing_type());
        }
        for (unsigned i = parents.size(); i > 0; i--) {
            Message* parent_msg = msg_stack[msg_stack.size() - 1];
            auto* field = parents[i - 1];
            auto* ref = parent_msg->GetReflection();
            Message* new_msg = (field->is_repeated())? ref->AddMessage(parent_msg, field) : ref->MutableMessage(parent_msg, field);
            msg_stack.push_back(new_msg);
        }

        last_reader = reader;
    }

    void ReturnToLevel() {
        //last_reader = reader;
    }

    IFieldReader* last_reader;
    IFieldReader* reader;
    std::vector<Message*> msg_stack;
};

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_DREMEL_ASSEMBLING_H_
//---------------------------------------------------------------------------
