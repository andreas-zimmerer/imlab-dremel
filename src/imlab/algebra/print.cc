// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/algebra/print.h"

namespace imlab {

    std::vector<const google::protobuf::FieldDescriptor*> Print::CollectFields() {
        return {};
    }

    void Print::Prepare(const std::vector<const google::protobuf::FieldDescriptor*> &required, Operator* consumer) {
        required_fields_ = required;
        consumer_ = consumer;

        child_->Prepare(required_fields_, this);
    }

    void Print::Produce(std::ostream& _o) {
        // Create a lock for the output (because otherwise the prints might be interleaved)
        _o << "std::mutex cout_lock;" << std::endl << std::endl;

        child_->Produce(_o);
    }

    void Print::Consume(std::ostream& _o, const Operator* child) {
        // Print:
        // cout_lock.lock();
        // out_ << [table]_[column] ...;
        // cout_lock.unlock();

        _o << "cout_lock.lock();" << std::endl;
        _o << "std::cout";
        for (auto& iu : required_fields_) {
            _o << " << record.DebugString(); << std::endl";
        }
        _o << "cout_lock.unlock();" << std::endl;
    }

}  // namespace imlab
