// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/algebra/table_scan.h"
#include "imlab/infra/types.h"
#include "imlab/schema.h"

namespace imlab {

    std::vector<const IU *> TableScan::CollectIUs() {
        const std::string table_name = table_;

        const std::vector<IU>& IUs = [&]() -> const std::vector<IU>& {
            if (table_name == "warehouse") return tpcc::warehouseTable::IUs;
            if (table_name == "district") return tpcc::districtTable::IUs;
            if (table_name == "customer") return tpcc::customerTable::IUs;
            if (table_name == "history") return tpcc::historyTable::IUs;
            if (table_name == "neworder") return tpcc::neworderTable::IUs;
            if (table_name == "order") return tpcc::orderTable::IUs;
            if (table_name == "orderline") return tpcc::orderlineTable::IUs;
            if (table_name == "item") return tpcc::itemTable::IUs;
            if (table_name == "stock") return tpcc::stockTable::IUs;
        }();

        std::vector<const IU*> refs;
        refs.reserve(IUs.size());
        for (auto& iu : IUs) {
            refs.push_back(&iu);
        }
        return refs;
    }

    void TableScan::Prepare(const std::vector<const IU *> &required, Operator *consumer) {
        std::cout << "Preparing Table Scan" << std::endl;

        required_ius_ = required;
        consumer_ = consumer;
    }

    void TableScan::Produce(std::ostream &_o) {
        // Print:
        // for (auto&[_, tid] : [table].primary_key) {
        //     auto [table]_[column] = [table].get_[column](tid).value();
        //     [... repeat for every required required_ius_]
        //
        //     [parent.consume(_o, this)]
        // }

        _o << std::endl;
        _o << "for (auto&[_, tid] : db." << table_ << "Table.primary_key) {" << std::endl;
        for (auto &iu : required_ius_) {
            _o << "    auto " << iu->table << "_" << iu->column << " = db." << iu->table << "Table.get_" << iu->column
               << "(tid).value();" << std::endl;
        }

        consumer_->Consume(_o, this);

        _o << "}" << std::endl;
    }

}  // namespace imlab
