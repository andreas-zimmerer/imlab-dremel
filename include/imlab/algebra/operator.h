// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_ALGEBRA_OPERATOR_H_
#define INCLUDE_IMLAB_ALGEBRA_OPERATOR_H_
// ---------------------------------------------------------------------------
#include <vector>
#include "imlab/algebra/iu.h"
// ---------------------------------------------------------------------------
namespace imlab {
// ---------------------------------------------------------------------------
class Operator {
 public:
    // Collect all IUs produced by the operator
    virtual std::vector<const IU*> CollectIUs();

    // Prepare the operator
    virtual void Prepare(const std::vector<const IU*> &required, Operator* parent);
    // Produce all tuples
    virtual void Produce();
    // Consume tuple
    virtual void Consume(const Operator* child);
};
// ---------------------------------------------------------------------------
}  // namespace imlab
// ---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_ALGEBRA_OPERATOR_H_
// ---------------------------------------------------------------------------

