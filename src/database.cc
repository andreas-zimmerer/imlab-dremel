// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/database.h"
#include <array>
#include "imlab/infra/error.h"
#include "imlab/infra/types.h"
#include "../include/imlab/infra/types.h"
#include "../include/imlab/database.h"

namespace imlab {

void Database::LoadWarehouse(std::istream &in) {
    std::string tmp = "";

    while (in.peek() != EOF) {
        std::getline(in, tmp, '|');
        auto w_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto w_name = Varchar<10>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto w_street_1 = Varchar<20>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto w_street_2 = Varchar<20>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto w_city = Varchar<20>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto w_state = Char<2>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto w_zip = Char<9>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto w_tax = Numeric<4, 4>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '\n');
        auto w_ytd = Numeric<12, 2>::castString(tmp.c_str(), tmp.size());

        warehouseTable.append_row(w_id, w_name, w_street_1, w_street_2, w_city,
                                  w_state, w_zip, w_tax, w_ytd);
    }
}

void Database::LoadDistrict(std::istream &in) {
    std::string tmp = "";

    while (in.peek() != EOF) {
        std::getline(in, tmp, '|');
        auto d_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto d_w_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto d_name = Varchar<10>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto d_street_1 = Varchar<20>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto d_street_2 = Varchar<20>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto d_city = Varchar<20>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto d_state = Char<2>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto d_zip = Char<9>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto d_tax = Numeric<4, 4>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto d_ytd = Numeric<12, 2>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '\n');
        auto d_next_o_id = Integer::castString(tmp.c_str(), tmp.size());

        districtTable.append_row(d_id, d_w_id, d_name, d_street_1, d_street_2, d_city,
                                 d_state, d_zip, d_tax, d_ytd, d_next_o_id);
    }
}

void Database::LoadCustomer(std::istream &in) {
    std::string tmp = "";

    while (in.peek() != EOF) {
        std::getline(in, tmp, '|');
        auto c_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_d_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_w_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_first = Varchar<16>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_middle = Char<2>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_last = Varchar<16>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_street_1 = Varchar<20>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_street_2 = Varchar<20>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_city = Varchar<20>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_state = Char<2>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_zip = Char<9>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_phone = Char<16>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_since = Timestamp::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_credit = Char<2>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_credit_lim = Numeric<12, 2>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_discount = Numeric<4, 4>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_balance = Numeric<12, 2>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_ytd_paymenr = Numeric<12, 2>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_payment_cnt = Numeric<4, 0>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto c_delivery_cnt = Numeric<4, 0>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '\n');
        auto c_data = Varchar<500>::castString(tmp.c_str(), tmp.size());

        customerTable.append_row(c_id, c_d_id, c_w_id, c_first, c_middle, c_last,
                c_street_1, c_street_2, c_city, c_state, c_zip, c_phone, c_since,
                c_credit, c_credit_lim, c_discount, c_balance, c_ytd_paymenr,
                c_payment_cnt, c_delivery_cnt, c_data);
    }
}

void Database::LoadHistory(std::istream &in) {
    std::string tmp = "";

    while (in.peek() != EOF) {
        std::getline(in, tmp, '|');
        auto h_c_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto h_c_d_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto h_c_w_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto h_d_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto h_w_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto h_date = Timestamp::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto h_amount = Numeric<6, 2>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '\n');
        auto h_data = Varchar<24>::castString(tmp.c_str(), tmp.size());

        historyTable.append_row(h_c_id, h_c_d_id, h_c_w_id, h_d_id, h_w_id, h_date,
                h_amount, h_data);
    }
}

void Database::LoadNewOrder(std::istream &in) {
    std::string tmp = "";

    while (in.peek() != EOF) {
        std::getline(in, tmp, '|');
        auto no_o_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto no_d_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '\n');
        auto no_w_id = Integer::castString(tmp.c_str(), tmp.size());

        neworderTable.append_row(no_o_id, no_d_id, no_w_id);
    }
}

void Database::LoadOrder(std::istream &in) {
    std::string tmp = "";

    while (in.peek() != EOF) {
        std::getline(in, tmp, '|');
        auto o_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto o_d_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto o_w_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto o_c_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto o_entry_d = Timestamp::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto o_carrier_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto o_ol_cnt = Numeric<2, 0>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '\n');
        auto o_all_local = Numeric<1, 0>::castString(tmp.c_str(), tmp.size());

        orderTable.append_row(o_id, o_d_id, o_w_id, o_c_id, o_entry_d, o_carrier_id,
                o_ol_cnt, o_all_local);
    }
}

void Database::LoadOrderLine(std::istream &in) {
    std::string tmp = "";

    while (in.peek() != EOF) {
        std::getline(in, tmp, '|');
        auto ol_o_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto ol_d_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto ol_w_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto ol_number = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto ol_i_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto ol_supply_w_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto ol_delivery_d = Timestamp::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto ol_quantity = Numeric<2, 0>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto ol_amount = Numeric<6, 2>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '\n');
        auto ol_dist_info = Char<24>::castString(tmp.c_str(), tmp.size());

        orderlineTable.append_row(ol_o_id, ol_d_id, ol_w_id, ol_number, ol_i_id,
                ol_supply_w_id, ol_delivery_d, ol_quantity, ol_amount, ol_dist_info);
    }
}

void Database::LoadItem(std::istream &in) {
    std::string tmp = "";

    while (in.peek() != EOF) {
        std::getline(in, tmp, '|');
        auto i_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto i_im_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto i_name = Varchar<24>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto i_price = Numeric<5, 2>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '\n');
        auto i_data = Varchar<50>::castString(tmp.c_str(), tmp.size());

        itemTable.append_row(i_id, i_im_id, i_name, i_price, i_data);
    }
}

void Database::LoadStock(std::istream &in) {
    std::string tmp = "";

    while (in.peek() != EOF) {
        std::getline(in, tmp, '|');
        auto s_i_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_w_id = Integer::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_quantity = Numeric<4, 0>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_dist_01 = Char<24>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_dist_02 = Char<24>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_dist_03 = Char<24>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_dist_04 = Char<24>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_dist_05 = Char<24>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_dist_06 = Char<24>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_dist_07 = Char<24>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_dist_08 = Char<24>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_dist_09 = Char<24>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_dist_10 = Char<24>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_ytd = Numeric<8, 0>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_order_cnt = Numeric<4, 0>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '|');
        auto s_remote_cnt = Numeric<4, 0>::castString(tmp.c_str(), tmp.size());
        std::getline(in, tmp, '\n');
        auto s_data = Varchar<50>::castString(tmp.c_str(), tmp.size());

        stockTable.append_row(s_i_id, s_w_id, s_quantity, s_dist_01, s_dist_02,
                              s_dist_03, s_dist_04, s_dist_05, s_dist_06, s_dist_07, s_dist_08,
                              s_dist_09, s_dist_10, s_ytd, s_order_cnt, s_remote_cnt, s_data);
    }
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

    auto t = warehouseTable.index_prim_key.at(Key(w_id));

    auto w_tax = warehouseTable.w_tax[warehouseTable.index_prim_key.at(Key(w_id))];
    auto c_discount = customerTable.c_discount[customerTable.index_prim_key.at(Key(w_id, d_id, c_id))];
    auto o_id = districtTable.d_next_o_id[districtTable.index_prim_key.at(Key(w_id, d_id))];
    auto d_tax = districtTable.d_tax[districtTable.index_prim_key.at(Key(w_id, d_id))];

    districtTable.d_next_o_id[districtTable.index_prim_key.at(Key(w_id, d_id))] = o_id + Integer(1);

    int all_local = 1;
    for (int index = 0; index < items.value; index++) {
        if (w_id != supware[index])
            all_local = 0;
    }

    orderTable.append_row(o_id, d_id, w_id, c_id, datetime, Integer(0), Numeric<2, 0>(items), Numeric<1, 0>(all_local));
    neworderTable.append_row(o_id, d_id, w_id);

    for (int index = 0; index < items.value; index++) {
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

        orderlineTable.append_row(o_id, d_id, w_id, Integer(index + 1), itemid[index], supware[index], Timestamp(0), Numeric<2, 0>(qty[index]), ol_amount, s_dist);
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
