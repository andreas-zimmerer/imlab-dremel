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

template <> size_t Database::Size<tpcc::kCustomer>()    { /* TODO */ return 0; }
template <> size_t Database::Size<tpcc::kDistrict>()    { /* TODO */ return 0; }
template <> size_t Database::Size<tpcc::kHistory>()     { /* TODO */ return 0; }
template <> size_t Database::Size<tpcc::kItem>()        { /* TODO */ return 0; }
template <> size_t Database::Size<tpcc::kNewOrder>()    { /* TODO */ return 0; }
template <> size_t Database::Size<tpcc::kOrder>()       { /* TODO */ return 0; }
template <> size_t Database::Size<tpcc::kOrderLine>()   { /* TODO */ return 0; }
template <> size_t Database::Size<tpcc::kStock>()       { /* TODO */ return 0; }
template <> size_t Database::Size<tpcc::kWarehouse>()   { /* TODO */ return 0; }

}  // namespace imlab
