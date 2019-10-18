// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/database.h"
#include <array>
#include "imlab/infra/error.h"
#include "imlab/infra/types.h"

namespace imlab {

void Database::LoadWarehouse(std::istream &in) {
    // TODO
}

void Database::LoadDistrict(std::istream &in) {
    // TODO
}

void Database::LoadCustomer(std::istream &in) {
    // TODO
}

void Database::LoadHistory(std::istream &in) {
    // TODO
}

void Database::LoadNewOrder(std::istream &in) {
    // TODO
}

void Database::LoadOrder(std::istream &in) {
    // TODO
}

void Database::LoadOrderLine(std::istream &in) {
    // TODO
}

void Database::LoadItem(std::istream &in) {
    // TODO
}

void Database::LoadStock(std::istream& in) {
    // TODO
}

void Database::NewOrder(
        Integer w_id,
        Integer d_id,
        Integer c_id,
        Integer items,
        std::array<Integer, 15> &supware,
        std::array<Integer, 15> &itemid,
        std::array<Integer, 15> &qty,
        Timestamp datetime) {
    // TODO
}

template <> size_t Database::Size<tpcc::kCustomer>()    { return customerTable.c_id.size(); }
template <> size_t Database::Size<tpcc::kDistrict>()    { return districtTable.d_id.size(); }
template <> size_t Database::Size<tpcc::kHistory>()     { return historyTable.h_c_id.size(); }
template <> size_t Database::Size<tpcc::kItem>()        { return itemTable.i_id.size(); }
template <> size_t Database::Size<tpcc::kNewOrder>()    { return neworderTable.no_o_id.size(); }
template <> size_t Database::Size<tpcc::kOrder>()       { return orderTable.o_id.size(); }
template <> size_t Database::Size<tpcc::kOrderLine>()   { return orderlineTable.ol_o_id.size(); }
template <> size_t Database::Size<tpcc::kStock>()       { return stockTable.s_i_id.size(); }
template <> size_t Database::Size<tpcc::kWarehouse>()   { return warehouseTable.w_id.size(); }

}  // namespace imlab
