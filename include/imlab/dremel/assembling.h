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

        // Init all the things
        msg_stack = { &record };
        last_read_field = nullptr;
        currently_read_field = readers[0]->field();
        field_reader_map.clear();
        for (auto& r : readers) {
            field_reader_map[r->field()] = r;
        }

        while (currently_read_field != nullptr) {
            auto value = field_reader_map.at(currently_read_field)->ReadNext();
            if (!value.is_null()) {
                MoveToLevel(currently_read_field);
                value.AppendToRecord(msg_stack[msg_stack.size() - 1]);
            } else {
                auto* new_level = currently_read_field;
                // TODO: probably this is not the correct new_level
                for (int i = 0; i < GetFullDefinitionLevel(currently_read_field) - value.definition_level(); i++) {
                    new_level = GetFieldDescriptor(new_level->containing_type());
                }
                MoveToLevel(new_level);
            }

            currently_read_field = fsm.NextField(currently_read_field, field_reader_map.at(currently_read_field)->Peek().repetition_level());

            ReturnToLevel();
        }

        ReturnToLevel();

        return record;
    }

 protected:
    void MoveToLevel(const FieldDescriptor* new_field) {
        const auto* common_ancestor = GetCommonAncestor(last_read_field, currently_read_field);
        auto common_ancestor_level = GetFullDefinitionLevel(common_ancestor);

        // Unwind message stack: End nested records up to the level of the lowest common ancestor.
        int elements_to_remove = msg_stack.size() - common_ancestor_level - 1/*don't pop root*/;
        for (int i = 0; i < elements_to_remove; i++) {
            msg_stack.pop_back();
        }
        assert(GetFieldDescriptor(msg_stack[msg_stack.size() - 1]->GetDescriptor()) == common_ancestor);

        if (GetFullDefinitionLevel(new_field) < common_ancestor_level) {
            last_read_field = new_field;
            return;
        }

        // Re-build message stack accordingly: Start nested records from the level of the lowest common ancestor.
        // First, gather which fields we need to add to our stack (reverse order), then convert them to messages.
        std::vector<const FieldDescriptor*> parents {};
        // The target_field_type is "where we want to end up".
        auto* target_field_type = (new_field->type() == FieldDescriptor::TYPE_GROUP || new_field->type() == FieldDescriptor::TYPE_MESSAGE)?
                                  new_field->message_type() : new_field->containing_type();
        //target_field_type = new_field->containing_type();

        while (target_field_type != msg_stack[msg_stack.size() - 1]->GetDescriptor()) {
            parents.push_back(GetFieldDescriptor(target_field_type));
            target_field_type = GetFieldDescriptor(target_field_type)->containing_type();
        }
        for (auto field = parents.rbegin(); field != parents.rend(); field++) {
            Message* parent_msg = msg_stack[msg_stack.size() - 1];
            Message* child_msg = ((*field)->is_repeated())? parent_msg->GetReflection()->AddMessage(parent_msg, *field)
                                                          : parent_msg->GetReflection()->MutableMessage(parent_msg, *field);
            msg_stack.push_back(child_msg);
        }

        last_read_field = new_field;
    }

    void ReturnToLevel() {
        //return;

        const auto* common_ancestor = GetCommonAncestor(last_read_field, currently_read_field);
        auto common_ancestor_level = GetFullDefinitionLevel(common_ancestor);

        // Unwind message stack: End nested records up to the level of the lowest common ancestor.
        int elements_to_remove = msg_stack.size() - common_ancestor_level - 1/*don't pop root*/;
        for (int i = 0; i < elements_to_remove; i++) {
            msg_stack.pop_back();
        }
        assert(GetFieldDescriptor(msg_stack[msg_stack.size() - 1]->GetDescriptor()) == common_ancestor);

        last_read_field = common_ancestor;
        return;
    }

    // In the original paper, the following two FieldDescriptors are actually FieldReaders.
    // But it doesn't really matter and I found it easier to work with FieldDescriptors here.
    const FieldDescriptor* last_read_field;
    const FieldDescriptor* currently_read_field;
    // This stack resembles the nested structure of a message.
    std::vector<Message*> msg_stack;
    // We also create a mapping between fields and FieldReaders
    std::unordered_map<const FieldDescriptor*, IFieldReader*> field_reader_map;
};

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_DREMEL_ASSEMBLING_H_
//---------------------------------------------------------------------------
