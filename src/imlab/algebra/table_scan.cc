// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/algebra/table_scan.h"
#include "imlab/database.h"
#include "imlab/infra/types.h"

namespace imlab {

    std::vector<const IU *> TableScan::CollectIUs() {
        const std::string table_name = table_;
        if (table_name == "warehouse") return tpcc::warehouseTable::CollectIUs();
        if (table_name == "district") return tpcc::districtTable::CollectIUs();
        if (table_name == "customer") return tpcc::customerTable::CollectIUs();
        if (table_name == "history") return tpcc::historyTable::CollectIUs();
        if (table_name == "neworder") return tpcc::neworderTable::CollectIUs();
        if (table_name == "order") return tpcc::orderTable::CollectIUs();
        if (table_name == "orderline") return tpcc::orderlineTable::CollectIUs();
        if (table_name == "item") return tpcc::itemTable::CollectIUs();
        if (table_name == "stock") return tpcc::stockTable::CollectIUs();
        return {};
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
        _o << "for (auto&[_, tid] : " << table_ << ".primary_key) {" << std::endl;
        for (auto &iu : required_ius_) {
            _o << "    auto " << iu->table << "_" << iu->column << " = " << iu->column << ".get_" << iu->column
               << "(tid).value();" << std::endl;
        }

        consumer_->Consume(_o, this);

        _o << "}" << std::endl;
    }

}  // namespace imlab
