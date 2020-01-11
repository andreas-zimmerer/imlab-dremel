// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_ALGEBRA_INNER_JOIN_H_
#define INCLUDE_IMLAB_ALGEBRA_INNER_JOIN_H_
// ---------------------------------------------------------------------------
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "./operator.h"
// ---------------------------------------------------------------------------
namespace imlab {
// ---------------------------------------------------------------------------
class InnerJoin: public Operator {
 protected:
    // Left child operator
    std::unique_ptr<Operator> left_child_;
    // Right child operator
    std::unique_ptr<Operator> right_child_;
    // Hash preodicates
    std::vector<std::pair<const google::protobuf::FieldDescriptor*, const google::protobuf::FieldDescriptor*>> hash_predicates_;

    // Required ius
    std::vector<const google::protobuf::FieldDescriptor*> required_fields_;
    // Consumer
    Operator *consumer_;

 public:
    // Constructor
    InnerJoin(std::unique_ptr<Operator> left,
            std::unique_ptr<Operator> right,
            std::vector<std::pair<const google::protobuf::FieldDescriptor*, const google::protobuf::FieldDescriptor*>> predicates)
        : left_child_(std::move(left)), right_child_(std::move(right)), hash_predicates_(predicates) {}

    // Collect all IUs produced by the operator
    std::vector<const google::protobuf::FieldDescriptor*> CollectFields() override;

    // Prepare the operator
    void Prepare(const std::vector<const google::protobuf::FieldDescriptor*> &required, Operator* consumer) override;
    // Produce all tuples
    void Produce(std::ostream& _o) override;
    // Consume tuple
    void Consume(std::ostream& _o, const Operator* child) override;

 private:
    std::string GenerateHashmapName();
};
// ---------------------------------------------------------------------------
}  // namespace imlab
// ---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_ALGEBRA_INNER_JOIN_H_
// ---------------------------------------------------------------------------

