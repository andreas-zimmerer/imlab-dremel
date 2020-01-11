// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/algebra/selection.h"
#include <algorithm>

namespace imlab {

    std::vector<const google::protobuf::FieldDescriptor*> Selection::CollectFields() {
        const auto& child_ius = child_->CollectFields();
        return child_ius;
    }

    void Selection::Prepare(const std::vector<const google::protobuf::FieldDescriptor*> &required, Operator* consumer) {
        required_fields_ = required;
        consumer_ = consumer;

        std::vector<const google::protobuf::FieldDescriptor*>& required_from_child = required_fields_;
        for (auto& p : predicates_) {
            if (std::find(required_from_child.begin(), required_from_child.end(), p.first) == required_from_child.end()) {
                required_from_child.push_back(p.first);
            }
        }
        child_->Prepare(required_from_child, this);
    }

    void Selection::Produce(std::ostream& _o) {
        child_->Produce(_o);
    }

    void Selection::Consume(std::ostream& _o, const Operator* child) {
        // print:
        // if (predicate == "..." && ...) {
        //     [parent.consume()]
        // }

        _o << "if (";
        for (auto& [IU, p] : predicates_) {
           // _o << IU->table << "_" << IU->column << " == " << p << " && ";
        }
        _o << "true) {" << std::endl;

        consumer_->Consume(_o, this);

        _o << "}" << std::endl;
    }

}  // namespace imlab
