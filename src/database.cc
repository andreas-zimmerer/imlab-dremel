// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/database.h"
#include <array>
#include <iostream>
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

        warehouseTable.insert_tuple({w_id, w_name, w_street_1, w_street_2, w_city,
                                  w_state, w_zip, w_tax, w_ytd});
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

        districtTable.insert_tuple({d_id, d_w_id, d_name, d_street_1, d_street_2, d_city,
                                 d_state, d_zip, d_tax, d_ytd, d_next_o_id});
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

        customerTable.insert_tuple({c_id, c_d_id, c_w_id, c_first, c_middle, c_last,
                c_street_1, c_street_2, c_city, c_state, c_zip, c_phone, c_since,
                c_credit, c_credit_lim, c_discount, c_balance, c_ytd_paymenr,
                c_payment_cnt, c_delivery_cnt, c_data});
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

        historyTable.insert_tuple({h_c_id, h_c_d_id, h_c_w_id, h_d_id, h_w_id, h_date,
                h_amount, h_data});
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

        neworderTable.insert_tuple({no_o_id, no_d_id, no_w_id});
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

        orderTable.insert_tuple({o_id, o_d_id, o_w_id, o_c_id, o_entry_d, o_carrier_id,
                o_ol_cnt, o_all_local});
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

        orderlineTable.insert_tuple({ol_o_id, ol_d_id, ol_w_id, ol_number, ol_i_id,
                ol_supply_w_id, ol_delivery_d, ol_quantity, ol_amount, ol_dist_info});
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

        itemTable.insert_tuple({i_id, i_im_id, i_name, i_price, i_data});
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

        stockTable.insert_tuple({s_i_id, s_w_id, s_quantity, s_dist_01, s_dist_02,
                              s_dist_03, s_dist_04, s_dist_05, s_dist_06, s_dist_07, s_dist_08,
                              s_dist_09, s_dist_10, s_ytd, s_order_cnt, s_remote_cnt, s_data});
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
    auto w_tax = warehouseTable.read_tuple(warehouseTable.primary_key.at(Key(w_id))).w_tax;
    auto c_discount = customerTable.read_tuple(customerTable.primary_key.at(Key(w_id, d_id, c_id))).c_discount;
    auto o_id = districtTable.read_tuple(districtTable.primary_key.at(Key(w_id, d_id))).d_next_o_id;
    auto d_tax = districtTable.read_tuple(districtTable.primary_key.at(Key(w_id, d_id))).d_tax;

    districtTable.update_tuple(districtTable.primary_key.at(Key(w_id, d_id)), tpcc::districtTuple {.d_next_o_id = o_id + Integer(1)});

    int all_local = 1;
    for (int index = 0; index < items.value; index++) {
        if (w_id != supware[index])
            all_local = 0;
    }

    orderTable.insert_tuple({o_id, d_id, w_id, c_id, datetime, Integer(0), Numeric<2, 0>(items), Numeric<1, 0>(all_local)});
    neworderTable.insert_tuple({o_id, d_id, w_id});

    for (int index = 0; index < items.value; index++) {
        auto i_price = itemTable.read_tuple(itemTable.primary_key.at(Key(itemid[index]))).i_price;

        auto stockTableTuple = stockTable.read_tuple(stockTable.primary_key.at(Key(supware[index], itemid[index])));
        auto s_quantity = stockTableTuple.s_quantity;
        auto s_remote_cnt = stockTableTuple.s_remote_cnt;
        auto s_order_cnt = stockTableTuple.s_order_cnt;
        auto s_dist = [&](Integer d_id) {
            switch (d_id.value) {
                case 1: return stockTableTuple.s_dist_01;
                case 2: return stockTableTuple.s_dist_02;
                case 3: return stockTableTuple.s_dist_03;
                case 4: return stockTableTuple.s_dist_04;
                case 5: return stockTableTuple.s_dist_05;
                case 6: return stockTableTuple.s_dist_06;
                case 7: return stockTableTuple.s_dist_07;
                case 8: return stockTableTuple.s_dist_08;
                case 9: return stockTableTuple.s_dist_09;
                case 10: return stockTableTuple.s_dist_10;
           }
        }(d_id);

        if (s_quantity > Numeric<4, 0>(qty[index])) {
            stockTable.update_tuple(stockTable.primary_key.at(Key(supware[index], itemid[index])), tpcc::stockTuple {.s_quantity = s_quantity - Numeric<4, 0>(qty[index])});
        } else {
            stockTable.update_tuple(stockTable.primary_key.at(Key(supware[index], itemid[index])), tpcc::stockTuple {.s_quantity = s_quantity + Numeric<4, 0>(91) - Numeric<4, 0>(qty[index])});
        }

        if (supware[index] != w_id) {
            stockTable.update_tuple(stockTable.primary_key.at(Key(w_id, itemid[index])), tpcc::stockTuple {.s_remote_cnt = s_remote_cnt + Numeric<4, 0>(1)});
        } else {
            stockTable.update_tuple(stockTable.primary_key.at(Key(w_id, itemid[index])), tpcc::stockTuple {.s_remote_cnt = s_order_cnt + Numeric<4, 0>(1)});
        }

        Numeric<6, 2> ol_amount = (Numeric<5, 2>(qty[index]) * i_price * (Numeric<4, 4>(1) + w_tax + d_tax).castS<5>() * (Numeric<4, 4>(1) - c_discount)
                .castP2().castP2().castS<5>()).castS<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>();

        orderlineTable.insert_tuple({o_id, d_id, w_id, Integer(index + 1), itemid[index], supware[index], Timestamp(0), Numeric<2, 0>(qty[index]), ol_amount, s_dist});
    }
}

template <> size_t Database::Size<tpcc::kcustomer>()    { return customerTable.get_size(); }
template <> size_t Database::Size<tpcc::kdistrict>()    { return districtTable.get_size(); }
template <> size_t Database::Size<tpcc::khistory>()     { return historyTable.get_size(); }
template <> size_t Database::Size<tpcc::kitem>()        { return itemTable.get_size(); }
template <> size_t Database::Size<tpcc::kneworder>()    { return neworderTable.get_size(); }
template <> size_t Database::Size<tpcc::korder>()       { return orderTable.get_size(); }
template <> size_t Database::Size<tpcc::korderline>()   { return orderlineTable.get_size(); }
template <> size_t Database::Size<tpcc::kstock>()       { return stockTable.get_size(); }
template <> size_t Database::Size<tpcc::kwarehouse>()   { return warehouseTable.get_size(); }

void Database::Print() {
    std::cout << "-------------\n"
              << "Warehouse: " << Size<imlab::tpcc::kwarehouse>() << "\n"
              << "District: " << Size<imlab::tpcc::kdistrict>() << "\n"
              << "Customer: " << Size<imlab::tpcc::kcustomer>() << "\n"
              << "History: " << Size<imlab::tpcc::khistory>() << "\n"
              << "Neworder: " << Size<imlab::tpcc::kneworder>() << "\n"
              << "Order: " << Size<imlab::tpcc::korder>() << "\n"
              << "Orderline: " << Size<imlab::tpcc::korderline>() << "\n"
              << "Item: " << Size<imlab::tpcc::kitem>() << "\n"
              << "Stock: " << Size<imlab::tpcc::kstock>() << "\n";
}
}  // namespace imlab
