// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/algebra/print.h"

namespace imlab {

    std::vector<const IU*> Print::CollectIUs() {
        return {};
    }

    void Print::Prepare(const std::vector<const IU*> &required, Operator* consumer) {
        required_ius_ = required;
        consumer_ = consumer;

        child_->Prepare(required_ius_, this);
    }

    void Print::Produce(std::ostream& _o) {
        // Create a lock for the output (because otherwise the prints might be interleaved)
        _o << "std::mutex cout_lock;" << std::endl << std::endl;

        // Print column names
        for (auto& iu : required_ius_) {
            _o << "std::cout << \" | \" << std::right << std::setw(20) << \"" << iu->column << "\";" << std::endl;
        }
        _o << "std::cout << \" |\" << std::endl;" << std::endl;
        for (auto& iu : required_ius_) {
            _o << "std::cout << \" | --------------------\";" << std::endl;
        }
        _o << "std::cout << \" |\" << std::endl;" << std::endl << std::endl;
        child_->Produce(_o);
    }

    void Print::Consume(std::ostream& _o, const Operator* child) {
        // Print:
        // cout_lock.lock();
        // out_ << [table]_[column] ...;
        // cout_lock.unlock();

        _o << "cout_lock.lock();" << std::endl;
        _o << "std::cout";
        for (auto& iu : required_ius_) {
            _o << " << \" | \" << std::right << std::setw(20) << " << iu->table << "_" << iu->column;
        }
        _o << " << \" |\" << std::endl;" << std::endl;
        _o << "cout_lock.unlock();" << std::endl;
    }

}  // namespace imlab
