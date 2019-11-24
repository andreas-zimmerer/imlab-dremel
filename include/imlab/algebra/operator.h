// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_ALGEBRA_OPERATOR_H_
#define INCLUDE_IMLAB_ALGEBRA_OPERATOR_H_
// ---------------------------------------------------------------------------
#include <vector>
#include <ostream>
#include <iostream>
#include "imlab/algebra/iu.h"
// ---------------------------------------------------------------------------
namespace imlab {
// ---------------------------------------------------------------------------
class Operator {
 public:
    // Collect all IUs produced by the operator
    virtual std::vector<const IU*> CollectIUs() = 0;

    // Prepare the operator
    virtual void Prepare(const std::vector<const IU*> &required, Operator* parent) = 0;
    // Produce all tuples
    virtual void Produce(std::ostream& _o) = 0;
    // Consume tuple
    virtual void Consume(std::ostream&_o, const Operator* child) = 0;

    virtual ~Operator() = default;
};
// ---------------------------------------------------------------------------
}  // namespace imlab
// ---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_ALGEBRA_OPERATOR_H_
// ---------------------------------------------------------------------------

