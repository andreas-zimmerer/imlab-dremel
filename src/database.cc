// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/database.h"
#include <array>
#include "imlab/infra/error.h"
#include "imlab/infra/types.h"

namespace imlab {

void Database::LoadWarehouse(std::istream &in) {
    std::string tmp = "";

    while(in.peek() != EOF) {
        std::getline(in, tmp,'|');
        auto w_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp,'|');
        auto w_name = Varchar<10>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp,'|');
        auto w_street_1 = Varchar<20>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp,'|');
        auto w_street_2 = Varchar<20>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp,'|');
        auto w_city = Varchar<20>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp,'|');
        auto w_state = Char<2>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp,'|');
        auto w_zip = Char<9>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp,'|');
        auto w_tax = Numeric<4, 4>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp,'\n');
        auto w_ytd = Numeric<12, 2>::castString(tmp.c_str(), tmp.size());

        warehouseTable.append_row(w_id, w_name, w_street_1, w_street_2, w_city,
                w_state, w_zip, w_tax, w_ytd);
    }
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

    auto w_tax = warehouseTable.w_tax[warehouseTable.index_prim_key.at(Key(w_id))];
    auto c_discount = customerTable.c_discount[customerTable.index_prim_key.at(Key(w_id, d_id, c_id))];
    auto o_id = districtTable.d_next_o_id[districtTable.index_prim_key.at(Key(w_id, d_id))];
    auto d_tax = districtTable.d_tax[districtTable.index_prim_key.at(Key(w_id, d_id))];

    districtTable.d_next_o_id[districtTable.index_prim_key.at(Key(w_id, d_id))] = o_id + Integer(1);

    int all_local = 1;
    for(int index = 0; index < items.value; index++) {
        if(w_id != supware[index])
            all_local = 0;
    }

    // TODO: inserts

    for(int index = 0; index < items.value; index++) {
        auto i_price = itemTable.i_price[itemTable.index_prim_key.at(Key(itemid[index]))];
        auto s_quantity = stockTable.s_quantity[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))];
        auto s_remote_cnt = stockTable.s_remote_cnt[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))];
        auto s_order_cnt = stockTable.s_order_cnt[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))];
        auto s_dist = [&](Integer d_id) {
           switch(d_id.value) {
               case 1: return stockTable.s_dist_01[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))];
               case 2: return stockTable.s_dist_02[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))];
               case 3: return stockTable.s_dist_03[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))];
               case 4: return stockTable.s_dist_04[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))];
               case 5: return stockTable.s_dist_05[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))];
               case 6: return stockTable.s_dist_06[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))];
               case 7: return stockTable.s_dist_07[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))];
               case 8: return stockTable.s_dist_08[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))];
               case 9: return stockTable.s_dist_09[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))];
               case 10: return stockTable.s_dist_10[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))];
           }
        }(d_id);

        if (s_quantity > Numeric<4, 0>(qty[index])) {
            stockTable.s_quantity[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))] = s_quantity - Numeric<4, 0>(qty[index]);
        } else {
            stockTable.s_quantity[stockTable.index_prim_key.at(Key(supware[index], itemid[index]))] = s_quantity + Numeric<4, 0>(91) - Numeric<4, 0>(qty[index]);
        }

        if (supware[index] != w_id) {
            stockTable.s_remote_cnt[stockTable.index_prim_key.at(Key(w_id, itemid[index]))] = s_remote_cnt + Numeric<4, 0>(1);
        } else {
            stockTable.s_remote_cnt[stockTable.index_prim_key.at(Key(w_id, itemid[index]))] = s_order_cnt + Numeric<4, 0>(1);
        }

        Numeric<6, 2> ol_amount = (Numeric<5, 2>(qty[index]) * i_price * (Numeric<4, 4>(1) + w_tax + d_tax).castS<5>() * (Numeric<4, 4>(1) - c_discount)
                .castP2().castP2().castS<5>()).castS<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>();

        // TODO last insert
    }
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
