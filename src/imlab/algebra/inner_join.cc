// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/algebra/inner_join.h"
#include <algorithm>
#include <sstream>

namespace imlab {

    std::vector<const IU*> InnerJoin::CollectIUs() {
        const auto& ius_left = left_child_->CollectIUs();
        const auto& ius_right = right_child_->CollectIUs();

        std::vector<const IU*> ius_combined {};
        ius_combined.reserve(ius_left.size() + ius_right.size());
        ius_combined.insert(ius_combined.end(), ius_left.begin(), ius_left.end());
        ius_combined.insert(ius_combined.end(), ius_right.begin(), ius_right.end());

        return ius_combined;
    }

    void InnerJoin::Prepare(const std::vector<const IU*> &required, Operator* consumer) {
        std::cout << "Preparing Join" << std::endl;

        required_ius_ = required;
        consumer_ = consumer;

        std::vector<const IU*> required_from_left_child;
        std::vector<const IU*> required_from_right_child;

        // First of all, we of course need the columns that are required for the join.
        // Searching the vectors before insert to maintain uniqueness (is this required?)
        for (auto& p : hash_predicates_) {
            if (std::find(required_from_left_child.begin(), required_from_left_child.end(), p.first) == required_from_left_child.end()) {
                required_from_left_child.push_back(p.first);
            }
            if (std::find(required_from_right_child.begin(), required_from_right_child.end(), p.second) == required_from_right_child.end()) {
                required_from_right_child.push_back(p.second);
            }
        }

        // Now we also need to sort out where we can get the required columns from our parent from.
        // Because our parents requires a certain set of columns and they can either come from the left or right
        // join relation, this is a little trickier.
        const auto& left_child_collected_ius = left_child_->CollectIUs();
        const auto& right_child_collected_ius = right_child_->CollectIUs();
        for (auto& r_iu : required_ius_) {
            if (std::find(left_child_collected_ius.begin(), left_child_collected_ius.end(), r_iu) != left_child_collected_ius.end()) {
                required_from_left_child.push_back(r_iu);
            } else if (std::find(right_child_collected_ius.begin(), right_child_collected_ius.end(), r_iu) != right_child_collected_ius.end()) {
                required_from_right_child.push_back(r_iu);
            } else {
                // If we got here, our parent requested an IU that we can not provide -> Error
            }
        }

        left_child_->Prepare(required_from_left_child, this);
        right_child_->Prepare(required_from_right_child, this);
    }

    std::string InnerJoin::GenerateHashmapName() {
        const char* left_relation = hash_predicates_[0].first->table;
        const char* right_relation = hash_predicates_[0].second->table;

        std::stringstream ss;
        ss << left_relation << "_" << right_relation << "_hashmap";
        return ss.str();
    }

    void InnerJoin::Produce(std::ostream& _o) {
        // TODO: print hash table
        _o << "std::unordered_multimap<, > " << GenerateHashmapName() << ";" << std::endl;

        left_child_->Produce(_o);
        right_child_->Produce(_o);
    }

    void InnerJoin::Consume(std::ostream& _o, const Operator* child) {
        if (child == left_child_.get()) {
            // Print:
            // [hashmapname].insert({Key([left_predicates, ...]), [values, required_from_left_values]});
            _o << GenerateHashmapName() << ".insert({" << "/*TODO*/" << ", " << "});" << std::endl;
        } else {
            // Print:
            // auto matches = [hashmapname].equal_range(Key([right_predicates]));
            // for (auto it = matches.first; it != matches.second; ++it) {
            //     auto& [table]_[column] = std::get<i>(it->value);
            //     [repeat for every required_from_left_values]
            //
            //     [parent.consume()]
            // }
            _o << std::endl;
            _o << "auto matches = " << GenerateHashmapName() << ".equal_range(Key(" << "TODO" << "));" << std::endl;
            _o << "for (auto it = matches.first; it != matches.second; ++it) {" << std::endl;
            // TODO

            consumer_->Consume(_o, this);
            _o << "}" << std::endl;
        }
    }

}  // namespace imlab
