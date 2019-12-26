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
    static R AssembleRecordInternal(RecordFSM& fsm, std::vector<FieldReader*>& readers) {
        R record {};
        auto* ref = record.GetReflection();
        auto* last_reader = ; // TODO
        auto* reader = readers[0];

        while (reader != *readers.end() /*TODO reader.hasdata()*/) {
            auto value = reader->fetch();
            if (!value.is_null()) {
                MoveToLevel(/*TODO tree level of reader*/, last_reader, reader);
                // TODO: append readers value to record
            } else {
                MoveToLevel(GetFullDefinitionLevel(reader->field()), last_reader, reader);
            }

            auto* next_field = fsm.NextField(reader->field(), value.repetition_level);
            reader = *std::find_if(readers.begin(), readers.end(), [&](const auto& r) { return r.field() == next_field; });
            ReturnToLevel(/*TODO tree level of reader*/, last_reader);
        }

        ReturnToLevel(0, last_reader);
        // TODO: end all nested records

        return record;
    }

 protected:
    static void MoveToLevel(unsigned newLevel, FieldReader* lastReader, FieldReader* nextReader) {

    }

    static void ReturnToLevel(unsigned newLevel, FieldReader* lastReader) {

    }
};

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_DREMEL_ASSEMBLING_H_
//---------------------------------------------------------------------------
