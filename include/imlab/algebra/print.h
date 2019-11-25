// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_ALGEBRA_PRINT_H_
#define INCLUDE_IMLAB_ALGEBRA_PRINT_H_
// ---------------------------------------------------------------------------
#include <memory>
#include <utility>
#include <vector>
#include "./operator.h"
// ---------------------------------------------------------------------------
namespace imlab {
// ---------------------------------------------------------------------------
class Print: public Operator {
 protected:
    // Child operator
    std::unique_ptr<Operator> child_;

    // Required ius
    std::vector<const IU*> required_ius_;
    // Consumer
    Operator *consumer_;
    // The ostream;
    std::ostream &out_;

 public:
    // Constructor
    explicit Print(std::unique_ptr<Operator> child, std::ostream& out)
        : child_(std::move(child)), out_(out) {}

    // Collect all IUs produced by the operator
    std::vector<const IU*> CollectIUs() override;

    // Prepare the operator
    void Prepare(const std::vector<const IU*> &required, Operator* consumer) override;
    // Produce all tuples
    void Produce(std::ostream& _o) override;
    // Consume tuple
    void Consume(std::ostream&_o, const Operator* child) override;
};
// ---------------------------------------------------------------------------
}  // namespace imlab
// ---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_ALGEBRA_PRINT_H_
// ---------------------------------------------------------------------------

