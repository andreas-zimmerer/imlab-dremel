// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <algorithm>
#include <sstream>
#include "imlab/algebra/inner_join.h"
#include "imlab/schemac/schema_compiler.h"

namespace imlab {

    std::vector<const IU *> InnerJoin::CollectIUs() {
        const auto &ius_left = left_child_->CollectIUs();
        const auto &ius_right = right_child_->CollectIUs();

        std::vector<const IU *> ius_combined{};
        ius_combined.reserve(ius_left.size() + ius_right.size());
        ius_combined.insert(ius_combined.end(), ius_left.begin(), ius_left.end());
        ius_combined.insert(ius_combined.end(), ius_right.begin(), ius_right.end());

        return ius_combined;
    }

    void InnerJoin::Prepare(const std::vector<const IU *> &required, Operator *consumer) {
        required_ius_ = required;
        consumer_ = consumer;

        std::vector<const IU *> required_from_left_child {};
        std::vector<const IU *> required_from_right_child {};

        // First of all, we of course need the columns that are required for the join.
        // Searching the vectors before insert to maintain uniqueness (is this required?)
        for (auto &p : hash_predicates_) {
            if (std::find(required_from_left_child.begin(), required_from_left_child.end(), p.first) ==
                required_from_left_child.end()) {
                required_from_left_child.push_back(p.first);
            }
            if (std::find(required_from_right_child.begin(), required_from_right_child.end(), p.second) ==
                required_from_right_child.end()) {
                required_from_right_child.push_back(p.second);
            }
        }

        // Now we also need to sort out where we can get the required columns from our parent from.
        // Because our parents requires a certain set of columns and they can either come from the left or right
        // join relation, this is a little trickier.
        const auto &left_child_collected_ius = left_child_->CollectIUs();
        const auto &right_child_collected_ius = right_child_->CollectIUs();

        for (auto &r_iu : required) {
            if (std::find(left_child_collected_ius.begin(), left_child_collected_ius.end(), r_iu) !=
                left_child_collected_ius.end()) {
                if (std::find(required_from_left_child.begin(), required_from_left_child.end(), r_iu) ==
                    required_from_left_child.end()) {
                    required_from_left_child.push_back(r_iu);
                }
            } else if (std::find(right_child_collected_ius.begin(), right_child_collected_ius.end(), r_iu) !=
                       right_child_collected_ius.end()) {
                if (std::find(required_from_right_child.begin(), required_from_right_child.end(), r_iu) ==
                    required_from_right_child.end()) {
                    required_from_right_child.push_back(r_iu);
                }
            } else {
                // If we got here, our parent requested an IU that we can not provide -> Error
                std::cout << "Required column not found in neither of the join relations: " << r_iu->table << "->" << r_iu->column << std::endl;
            }
        }

        left_child_->Prepare(required_from_left_child, this);
        right_child_->Prepare(required_from_right_child, this);
    }

    std::string InnerJoin::GenerateHashmapName() {
        const char *left_relation = hash_predicates_[0].first->table;
        const char *right_relation = hash_predicates_[0].second->table;

        std::stringstream ss {};
        ss << left_relation << "_" << right_relation << "_hashmap";
        return ss.str();
    }

    void InnerJoin::Produce(std::ostream &_o) {
        const auto &left_child_collected_ius = left_child_->CollectIUs();

        _o << "LazyMultiMap<Key<";
        for (auto &p : hash_predicates_) {  // all hash predicates from the left side
            _o << schemac::SchemaCompiler::generateTypeName(p.first->type) << "/*" << p.first->column << "*/"
               << ((&p != &*hash_predicates_.end() - 1) ? ", " : "");
        }
        _o << ">, std::tuple<";
        bool first_value = true;
        for (auto r : required_ius_) {  // if the value comes from the left side, it should be added to the hash table
            if (std::find(left_child_collected_ius.begin(), left_child_collected_ius.end(), r) != left_child_collected_ius.end()) {
                _o << (first_value ? "" : ", ") << schemac::SchemaCompiler::generateTypeName(r->type) << "/*" << r->column << "*/";
                first_value = false;
            }
        }
        _o << ">> " << GenerateHashmapName() << ";" << std::endl << std::endl;

        left_child_->Produce(_o);

        _o << std::endl;
        _o << GenerateHashmapName() << ".finalize();" << std::endl;
        _o << std::endl;

        right_child_->Produce(_o);
    }

    void InnerJoin::Consume(std::ostream &_o, const Operator *child) {
        const auto &left_child_collected_ius = left_child_->CollectIUs();

        if (child == left_child_.get()) {
            // Print:
            // [hashmapname].insert({Key([left_predicates, ...]), [values, required_from_left_values]});

            _o << GenerateHashmapName() << ".insert({Key(";
            for (auto& p : hash_predicates_) {
                _o << p.first->table << "_" << p.first->column << ((&p != &*hash_predicates_.end() - 1) ? ", " : "");
            }
            _o << "), std::make_tuple(";
            bool first_value = true;
            for (auto& r : required_ius_) {  // again, only if tuple comes from left side
                if (std::find(left_child_collected_ius.begin(), left_child_collected_ius.end(), r) != left_child_collected_ius.end()) {
                    _o << (first_value ? "" : ", ") << r->table << "_" << r->column;
                    first_value = false;
                }
            }
            _o << ")});" << std::endl;

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
            _o << "auto matches = " << GenerateHashmapName() << ".equal_range(Key(";
            for (auto& p : hash_predicates_) {
                _o << p.second->table << "_" << p.second->column << ((&p != &*hash_predicates_.end() - 1) ? ", " : "");
            }
            _o << "));" << std::endl;
            _o << "for (auto it = matches.first; it != matches.second; ++it) {" << std::endl;
            // Now we want to get out the required values from the left side
            // The values from the right side should already be in scope
            unsigned idx = 0;
            for (auto& r : required_ius_) {  // again, only if tuple comes from left side
                if (std::find(left_child_collected_ius.begin(), left_child_collected_ius.end(), r) != left_child_collected_ius.end()) {
                    _o << "auto& " << r->table << "_" << r->column << " = std::get<" << idx << ">(it->value);";
                    idx++;
                }
            }
            _o << std::endl << std::endl;

            consumer_->Consume(_o, this);
            _o << "}" << std::endl;
        }
    }

}  // namespace imlab
