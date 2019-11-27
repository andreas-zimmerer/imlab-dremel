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
        // out_ << [table]_[column];
        // [repeat for every...]
        // out_ << std::endl;

        for (auto& iu : required_ius_) {
            _o << "std::cout << \" | \" << std::right << std::setw(20) << " << iu->table << "_" << iu->column << ";" << std::endl;
        }
        _o << "std::cout << \" |\" << std::endl;";
    }

}  // namespace imlab
