// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/algebra/table_scan.h"
#include "imlab/schemac/schema_compiler.h"
#include "imlab/infra/types.h"
#include "../tools/protobuf/gen/schema.h"

namespace imlab {

    std::vector<const google::protobuf::FieldDescriptor*> TableScan::CollectFields() {
        const std::string table_name = table_;

        if (table_name == "Document") return schema::DocumentTable::fields();

        return {};
    }

    void TableScan::Prepare(const std::vector<const google::protobuf::FieldDescriptor*> &required, Operator *consumer) {
        required_fields_ = required;
        consumer_ = consumer;
    }

    void TableScan::Produce(std::ostream &_o) {
        // With TBB, we will actually emit:
        //
        // tbb::parallel_for(tbb::blocked_range<size_t>(0, [table].size()), [&](const tbb::blocked_range<size_t>& index_range) {
        //     for(size_t i=r.begin(); i!=r.end(); ++i, ++tuple_it) {
        //         auto& record = [table].get(i, required_ius);
        //
        //         [parent.consume(_o, this)]
        //     }
        // });

        _o << "tbb::parallel_for(tbb::blocked_range<size_t>(0, db." << table_ << "Table.size()), [&](const tbb::blocked_range<size_t>& index_range) {" << std::endl;
        _o << "    for(size_t i = index_range.begin(); i != index_range.end(); ++i) {" << std::endl;
        _o << "        auto& record = db." << table_ << "Table.get(i, {";
        for (auto& field : required_fields_) {
            _o << field->full_name() << ", ";
        }
        _o << "});" << std::endl;

        consumer_->Consume(_o, this);

        _o << "    }" << std::endl;
        _o << "});" << std::endl;
    }

}  // namespace imlab
