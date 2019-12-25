// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_DREMEL_RECORD_FSM_H_
#define INCLUDE_IMLAB_DREMEL_RECORD_FSM_H_
//---------------------------------------------------------------------------
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <google/protobuf/message.h>
#include "imlab/infra/hash.h"
//---------------------------------------------------------------------------
namespace imlab {
namespace dremel {
//---------------------------------------------------------------------------
using namespace google::protobuf;

struct Field {
    std::string identifier;
    unsigned max_repetition_level;
};

/// A finite state machine for assembling a shredded record.
class RecordFSM {
 public:
    explicit RecordFSM(const std::vector<Field>& fields) {
        assert(fields.size() > 0);
        _start_state = fields[0].identifier;
        ConstructRecordFSM(fields);
    };

    /// Yields the next field of the FSM for a given state.
    std::optional<std::string> NextField(const std::string& field, unsigned repetition_level) {
        const auto& it = _transitions.find(std::make_pair(field, repetition_level));
        if (it != _transitions.end()) {
            return {it->second};
        } else {
            return std::nullopt;
        }
    }

    std::string GenerateFsmGraph() { return GenerateGraphviz(); }

 protected:
    /// Constructs the state transitions of the FSM given a list of fields that should be included.
    void ConstructRecordFSM(const std::vector<Field>& fields) {
        for (unsigned i = 0; i < fields.size(); i++) {
            auto& field = fields[i];
            auto maxRepetitionLevel = field.max_repetition_level;
            // Either next element or nullopt if we are at the last field.
            auto barrier = (i == fields.size() - 1)? std::nullopt : std::make_optional(fields[i+1]);
            // "common repetition level of barrier and field" -> Minimum of both.
            // If we are at the end, repetition level is 0
            auto barrierLevel = std::min(maxRepetitionLevel, barrier.has_value()? barrier.value().max_repetition_level : 0);

            // Insert a default reflexive transition with the repetition level of this field.
            // It might be overridden later on.
            // This is not explicitly mentioned in the paper.
            _transitions.insert_or_assign(std::make_pair(field.identifier, maxRepetitionLevel), field.identifier);

            // Work on all backward edges of the FSM.
            // To refer to the paper, this is an edge like from "Name.Url" to "Name.Language.Code" in Figure 4.
            for (int u = i - 1; u >= 0; u--) {
                auto& preField = fields[u];
                if (preField.max_repetition_level > barrierLevel) { // TODO: we probably need the common repetition level here (wrong in paper)
                    // Get common repetition level of preField and field
                    auto backLevel = std::min(preField.max_repetition_level, field.max_repetition_level);
                    // Insert a transition into the FSM
                    _transitions.insert_or_assign(std::make_pair(field.identifier, backLevel), preField.identifier);
                }
            }

            // Handle transitions with multiple repetition levels to one destination.
            for (unsigned level = barrierLevel + 1; level <= maxRepetitionLevel; level++) {
                // If we don't have a transition from (field,level) yet, we copy it from (field,level-1).
                if (_transitions.find(std::make_pair(field.identifier, level)) == _transitions.end()) {
                    const auto& pre = _transitions.find(std::make_pair(field.identifier, level - 1));
                    if (pre == _transitions.end()) {
                        assert(false);
                    }
                    _transitions.insert_or_assign(std::make_pair(field.identifier, level), field.identifier); // this is different from the paper
                }
            }

            // Handle forward transition to the next field.
            for (unsigned level = 0; level <= barrierLevel; level++) {
                if (barrier) {
                    _transitions.insert_or_assign(std::make_pair(field.identifier, level), barrier.value().identifier);
                } else {
                    // "end state" is encoded as "no transition" -> explicitly delete transition
                    _transitions.erase(std::make_pair(field.identifier, level));
                }
            }
        }
    }

    /// Generates code for graphviz to draw a the FSM.
    /// Only meaningful for visually displaying the FSM in papers or so.
    std::string GenerateGraphviz() {
        std::stringstream ss {};
        ss << "digraph finite_state_machine {" << std::endl;
        ss << "    size=\"8,5\"" << std::endl;
        ss << "    node [shape = rect, style = rounded];" << std::endl;
        ss << std::endl;

        // Set of all nodes
        std::unordered_set<std::string> nodes {};

        // Map of transitions with (source, target) -> [repetition_level, ...]
        std::unordered_multimap<std::pair<std::string, std::string>, unsigned> edges {};

        for (const auto& [key, value] : _transitions) {
            auto& [source, level] = key;
            auto& target = value;
            nodes.emplace(source);
            nodes.emplace(target);
            edges.emplace(std::make_pair(source, target), level);
        }

        for (auto& source : nodes) {
            for (auto& target : nodes) {
                if (edges.find(std::make_pair(source, target)) != edges.end()) {
                    auto source_name = source;
                    std::replace(source_name.begin(), source_name.end(), '.', '_');
                    auto target_name = target;
                    std::replace(target_name.begin(), target_name.end(), '.', '_');

                    ss << "    " << source_name << " -> " << target_name << " [label=\"[";
                    // Print repetition levels as labels on edges:
                    auto level_range = edges.equal_range(std::make_pair(source, target));
                    for (auto &level_it = level_range.first; level_it != level_range.second; level_it++) {
                        ss << level_it->second << ",";
                    }
                    ss << "]\"];" << std::endl;
                }
            }
        }

        ss << "}" << std::endl;
        return ss.str();
    }

 private:
    /// Maps field identifier and repetition_level to a new field identifier.
    /// TODO: not sure if we really want string identifiers here...
    /// TODO: is it possible to use references here?
    std::unordered_map<std::pair<std::string, unsigned>, std::string> _transitions {};
    std::string _start_state;
};

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_DREMEL_RECORD_FSM_H_
//---------------------------------------------------------------------------
