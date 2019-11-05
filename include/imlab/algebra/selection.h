// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_ALGEBRA_SELECTION_H_
#define INCLUDE_IMLAB_ALGEBRA_SELECTION_H_
// ---------------------------------------------------------------------------
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include "imlab/infra/types.h"
#include "imlab/algebra/operator.h"
// ---------------------------------------------------------------------------
namespace imlab {
// ---------------------------------------------------------------------------
class Selection: public Operator {
 protected:
    // Child operator
    std::unique_ptr<Operator> child_;
    // Predicates
    std::vector<std::pair<const IU*, std::string>> predicates_;

    // Required ius
    std::vector<const IU*> required_ius_;
    // Consumer
    Operator *consumer_;

 public:
    // Constructor
    Selection(std::unique_ptr<Operator> child, std::vector<std::pair<const IU*, std::string>> predicates)
        : child_(std::move(child)), predicates_(std::move(predicates)) {}

    // Collect all IUs produced by the operator
    std::vector<const IU*> CollectIUs() override;

    // Prepare the operator
    void Prepare(const std::vector<const IU*> &required, Operator* consumer) override;
    // Produce all tuples
    void Produce() override;
    // Consume tuple
    void Consume(const Operator* child) override;
};
// ---------------------------------------------------------------------------
}  // namespace imlab
// ---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_ALGEBRA_SELECTION_H_
// ---------------------------------------------------------------------------

