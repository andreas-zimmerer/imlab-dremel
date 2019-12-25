// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/algebra/table_scan.h"
#include "imlab/schemac/schema_compiler.h"
#include "imlab/infra/types.h"
#include "../tools/protobuf/gen/schema.h"

namespace imlab {

    std::vector<const IU *> TableScan::CollectIUs() {
        const std::string table_name = table_;

        if (table_name == "Document") return schema::DocumentTable::get_ius();

        return {};
    }

    void TableScan::Prepare(const std::vector<const IU *> &required, Operator *consumer) {
        required_ius_ = required;
        consumer_ = consumer;
    }

    void TableScan::Produce(std::ostream &_o) {
        // With TBB, we will actually emit:
        //
        // tbb::parallel_for(tbb::blocked_range<size_t>(0, [table].get_size()), [&](const tbb::blocked_range<size_t>& index_range) {
        //     for(size_t i=r.begin(); i!=r.end(); ++i, ++tuple_it) {
        //         auto& [_, tid] = *([table].primary_key.begin() + i);
        //
        //         auto [table]_[column] = [table].get_[column](tid).value();
        //         [... repeat for every required required_ius_]
        //
        //         [parent.consume(_o, this)]
        //     }
        // });

        _o << "tbb::parallel_for(tbb::blocked_range<size_t>(0, db." << table_ << "Table.get_size()), [&](const tbb::blocked_range<size_t>& index_range) {" << std::endl;
        _o << "    for(size_t i = index_range.begin(); i != index_range.end(); ++i) {" << std::endl;
        for (auto &iu : required_ius_) {
            _o << "    auto " << iu->table << "_" << iu->column << " = db." << iu->table << "Table.get_" << iu->column
               << "(i).value();" << std::endl;
        }

        consumer_->Consume(_o, this);

        _o << "    }" << std::endl;
        _o << "});" << std::endl;
    }

}  // namespace imlab
