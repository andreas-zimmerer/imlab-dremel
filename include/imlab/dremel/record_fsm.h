// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------
#ifndef INCLUDE_IMLAB_DREMEL_RECORD_FSM_H_
#define INCLUDE_IMLAB_DREMEL_RECORD_FSM_H_
//---------------------------------------------------------------------------
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include "./schema_helper.h"
#include "imlab/infra/hash.h"
#include <google/protobuf/descriptor.h>
//---------------------------------------------------------------------------
namespace imlab {
namespace dremel {
//---------------------------------------------------------------------------
using namespace google::protobuf;

/// A finite state machine for assembling a shredded record.
class RecordFSM {
 public:
    explicit RecordFSM(const std::vector<const FieldDescriptor*>& fields) { ConstructRecordFSM(fields); };

    /// Yields the next field of the FSM for a given state or nullptr if we are at the end.
    const FieldDescriptor* NextField(const FieldDescriptor* field, unsigned repetition_level) {
        const auto& it = _transitions.find(std::make_pair(field, repetition_level));
        if (it != _transitions.end()) {
            return it->second;
        } else {
            // There is no matching transition.
            return nullptr;
        }
    }

    /// Generates a human-readable form of the FSM.
    std::string GenerateFsmGraph() { return GenerateGraphviz(); }

 protected:
    /// Constructs the state transitions of the FSM given a list of fields that should be included.
    void ConstructRecordFSM(const std::vector<const FieldDescriptor*>& fields) {
        for (int i = 0; i < fields.size(); i++) {
            auto& field = fields[i];
            auto maxRepetitionLevel = GetMaxRepetitionLevel(field);
            auto barrier = (i != fields.size() - 1)? fields[i+1] : nullptr;
            auto barrierLevel = (barrier != nullptr)? GetCommonRepetitionLevel(field, barrier) : 0;

            // Work on all backward edges of the FSM.
            // To refer to the paper, this is an edge like from "Name.Url" to "Name.Language.Code" in Figure 4.
            for (int u = i - 1; u >= 0; u--) {
                auto& preField = fields[u];
                if (GetMaxRepetitionLevel(preField) > barrierLevel) {
                    // Get common repetition level of preField and field
                    auto backLevel = GetCommonRepetitionLevel(preField, field);
                    // Insert a transition into the FSM
                    _transitions.insert_or_assign(std::make_pair(field, backLevel), preField);
                }
            }

            // Handle reflexive transitions.
            for (unsigned level = barrierLevel + 1; level <= maxRepetitionLevel; level++) {
                if (_transitions.find(std::make_pair(field, level)) == _transitions.end()) {
                    // Here, the paper seems to be wrong.
                    // According to the paper, we should copy the transition from (field, level-1) to (field, level),
                    // but this does not work.
                    // Instead, we need to insert a reflexive edge here.
                    _transitions.insert_or_assign(std::make_pair(field, level), field);
                }
            }

            // Handle forward transition to the next field.
            for (unsigned level = 0; level <= barrierLevel; level++) {
                _transitions.insert_or_assign(std::make_pair(field, level), barrier);
            }
        }
    }

    /// Generates code for graphviz to draw a the FSM.
    /// Only meaningful for visually displaying the FSM in papers or so.
    std::string GenerateGraphviz() {
        std::stringstream ss {};
        ss << "digraph finite_state_machine {" << std::endl;
        ss << "    size=\"8,5\"" << std::endl;
        ss << "    node [shape = doublecircle]; \"end\" [label=\"\"];" << std::endl;
        ss << "    node [shape = rect, style = rounded];" << std::endl;
        ss << std::endl;

        // Set of all nodes
        std::unordered_set<std::string> nodes {};

        // Map of transitions with (source, target) -> [repetition_level, ...]
        std::unordered_multimap<std::pair<std::string, std::string>, unsigned> edges {};

        for (const auto [key, target] : _transitions) {
            auto [source, level] = key;
            nodes.emplace(source->full_name());
            if (target != nullptr) {  // end state
                nodes.emplace(target->full_name());
                edges.emplace(std::make_pair(source->full_name(), target->full_name()), level);
            } else {
                nodes.emplace("end");
                edges.emplace(std::make_pair(source->full_name(), "end"), level);
            }
        }

        for (auto& source : nodes) {
            for (auto& target : nodes) {
                if (edges.find(std::make_pair(source, target)) != edges.end()) {
                    ss << "    \"" << source << "\" -> \"" << target << "\" [label=\"[";
                    // Print repetition levels as labels on edges:
                    std::vector<unsigned> repetition_levels {};
                    auto level_range = edges.equal_range(std::make_pair(source, target));
                    for (auto &level_it = level_range.first; level_it != level_range.second; level_it++) {
                        repetition_levels.push_back(level_it->second);
                    }
                    std::sort(repetition_levels.begin(), repetition_levels.end());
                    for (int i = 0; i < repetition_levels.size(); i++) {
                        ss << repetition_levels[i] << ((i != repetition_levels.size() - 1)? ", " : "");
                    }
                    ss << "]\"];" << std::endl;
                }
            }
        }

        ss << "}" << std::endl;
        return ss.str();
    }

 private:
    /// Maps a field and repetition_level to a new field.
    std::unordered_map<std::pair<const FieldDescriptor*, unsigned>, const FieldDescriptor*> _transitions {};
};

//---------------------------------------------------------------------------
}  // namespace dremel
}  // namespace imlab
//---------------------------------------------------------------------------
#endif  // INCLUDE_IMLAB_DREMEL_RECORD_FSM_H_
//---------------------------------------------------------------------------
