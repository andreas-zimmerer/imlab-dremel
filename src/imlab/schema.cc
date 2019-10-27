#include "imlab/schema.h"

namespace imlab {
namespace tpcc {

uint64_t warehouseTable::insert_tuple(const warehouseTuple& tuple) {
    w_id.push_back(tuple.w_id);
    w_name.push_back(tuple.w_name);
    w_street_1.push_back(tuple.w_street_1);
    w_street_2.push_back(tuple.w_street_2);
    w_city.push_back(tuple.w_city);
    w_state.push_back(tuple.w_state);
    w_zip.push_back(tuple.w_zip);
    w_tax.push_back(tuple.w_tax);
    w_ytd.push_back(tuple.w_ytd);

    auto insert_pos = size;
    primary_key[Key(tuple.w_id)] = insert_pos;

    size++;
    return insert_pos;
}

warehouseTuple warehouseTable::read_tuple(const uint64_t tid) {
}

void warehouseTable::update_tuple(const uint64_t tid, const warehouseTuple& tuple) {
}

void warehouseTable::delete_tuple(const uint64_t tid) {
}


uint64_t districtTable::insert_tuple(const districtTuple& tuple) {
    d_id.push_back(tuple.d_id);
    d_w_id.push_back(tuple.d_w_id);
    d_name.push_back(tuple.d_name);
    d_street_1.push_back(tuple.d_street_1);
    d_street_2.push_back(tuple.d_street_2);
    d_city.push_back(tuple.d_city);
    d_state.push_back(tuple.d_state);
    d_zip.push_back(tuple.d_zip);
    d_tax.push_back(tuple.d_tax);
    d_ytd.push_back(tuple.d_ytd);
    d_next_o_id.push_back(tuple.d_next_o_id);

    auto insert_pos = size;
    primary_key[Key(tuple.d_w_id, tuple.d_id)] = insert_pos;

    size++;
    return insert_pos;
}

districtTuple districtTable::read_tuple(const uint64_t tid) {
}

void districtTable::update_tuple(const uint64_t tid, const districtTuple& tuple) {
}

void districtTable::delete_tuple(const uint64_t tid) {
}


uint64_t customerTable::insert_tuple(const customerTuple& tuple) {
    c_id.push_back(tuple.c_id);
    c_d_id.push_back(tuple.c_d_id);
    c_w_id.push_back(tuple.c_w_id);
    c_first.push_back(tuple.c_first);
    c_middle.push_back(tuple.c_middle);
    c_last.push_back(tuple.c_last);
    c_street_1.push_back(tuple.c_street_1);
    c_street_2.push_back(tuple.c_street_2);
    c_city.push_back(tuple.c_city);
    c_state.push_back(tuple.c_state);
    c_zip.push_back(tuple.c_zip);
    c_phone.push_back(tuple.c_phone);
    c_since.push_back(tuple.c_since);
    c_credit.push_back(tuple.c_credit);
    c_credit_lim.push_back(tuple.c_credit_lim);
    c_discount.push_back(tuple.c_discount);
    c_balance.push_back(tuple.c_balance);
    c_ytd_paymenr.push_back(tuple.c_ytd_paymenr);
    c_payment_cnt.push_back(tuple.c_payment_cnt);
    c_delivery_cnt.push_back(tuple.c_delivery_cnt);
    c_data.push_back(tuple.c_data);

    auto insert_pos = size;
    primary_key[Key(tuple.c_w_id, tuple.c_d_id, tuple.c_id)] = insert_pos;

    size++;
    return insert_pos;
}

customerTuple customerTable::read_tuple(const uint64_t tid) {
}

void customerTable::update_tuple(const uint64_t tid, const customerTuple& tuple) {
}

void customerTable::delete_tuple(const uint64_t tid) {
}


uint64_t historyTable::insert_tuple(const historyTuple& tuple) {
    h_c_id.push_back(tuple.h_c_id);
    h_c_d_id.push_back(tuple.h_c_d_id);
    h_c_w_id.push_back(tuple.h_c_w_id);
    h_d_id.push_back(tuple.h_d_id);
    h_w_id.push_back(tuple.h_w_id);
    h_date.push_back(tuple.h_date);
    h_amount.push_back(tuple.h_amount);
    h_data.push_back(tuple.h_data);

    auto insert_pos = size;
    size++;
    return insert_pos;
}

historyTuple historyTable::read_tuple(const uint64_t tid) {
}

void historyTable::update_tuple(const uint64_t tid, const historyTuple& tuple) {
}

void historyTable::delete_tuple(const uint64_t tid) {
}


uint64_t neworderTable::insert_tuple(const neworderTuple& tuple) {
    no_o_id.push_back(tuple.no_o_id);
    no_d_id.push_back(tuple.no_d_id);
    no_w_id.push_back(tuple.no_w_id);

    auto insert_pos = size;
    primary_key[Key(tuple.no_w_id, tuple.no_d_id, tuple.no_o_id)] = insert_pos;

    size++;
    return insert_pos;
}

neworderTuple neworderTable::read_tuple(const uint64_t tid) {
}

void neworderTable::update_tuple(const uint64_t tid, const neworderTuple& tuple) {
}

void neworderTable::delete_tuple(const uint64_t tid) {
}


uint64_t orderTable::insert_tuple(const orderTuple& tuple) {
    o_id.push_back(tuple.o_id);
    o_d_id.push_back(tuple.o_d_id);
    o_w_id.push_back(tuple.o_w_id);
    o_c_id.push_back(tuple.o_c_id);
    o_entry_d.push_back(tuple.o_entry_d);
    o_carrier_id.push_back(tuple.o_carrier_id);
    o_ol_cnt.push_back(tuple.o_ol_cnt);
    o_all_local.push_back(tuple.o_all_local);

    auto insert_pos = size;
    primary_key[Key(tuple.o_w_id, tuple.o_d_id, tuple.o_id)] = insert_pos;

    size++;
    return insert_pos;
}

orderTuple orderTable::read_tuple(const uint64_t tid) {
}

void orderTable::update_tuple(const uint64_t tid, const orderTuple& tuple) {
}

void orderTable::delete_tuple(const uint64_t tid) {
}


uint64_t orderlineTable::insert_tuple(const orderlineTuple& tuple) {
    ol_o_id.push_back(tuple.ol_o_id);
    ol_d_id.push_back(tuple.ol_d_id);
    ol_w_id.push_back(tuple.ol_w_id);
    ol_number.push_back(tuple.ol_number);
    ol_i_id.push_back(tuple.ol_i_id);
    ol_supply_w_id.push_back(tuple.ol_supply_w_id);
    ol_delivery_d.push_back(tuple.ol_delivery_d);
    ol_quantity.push_back(tuple.ol_quantity);
    ol_amount.push_back(tuple.ol_amount);
    ol_dist_info.push_back(tuple.ol_dist_info);

    auto insert_pos = size;
    primary_key[Key(tuple.ol_w_id, tuple.ol_d_id, tuple.ol_o_id, tuple.ol_number)] = insert_pos;

    size++;
    return insert_pos;
}

orderlineTuple orderlineTable::read_tuple(const uint64_t tid) {
}

void orderlineTable::update_tuple(const uint64_t tid, const orderlineTuple& tuple) {
}

void orderlineTable::delete_tuple(const uint64_t tid) {
}


uint64_t itemTable::insert_tuple(const itemTuple& tuple) {
    i_id.push_back(tuple.i_id);
    i_im_id.push_back(tuple.i_im_id);
    i_name.push_back(tuple.i_name);
    i_price.push_back(tuple.i_price);
    i_data.push_back(tuple.i_data);

    auto insert_pos = size;
    primary_key[Key(tuple.i_id)] = insert_pos;

    size++;
    return insert_pos;
}

itemTuple itemTable::read_tuple(const uint64_t tid) {
}

void itemTable::update_tuple(const uint64_t tid, const itemTuple& tuple) {
}

void itemTable::delete_tuple(const uint64_t tid) {
}


uint64_t stockTable::insert_tuple(const stockTuple& tuple) {
    s_i_id.push_back(tuple.s_i_id);
    s_w_id.push_back(tuple.s_w_id);
    s_quantity.push_back(tuple.s_quantity);
    s_dist_01.push_back(tuple.s_dist_01);
    s_dist_02.push_back(tuple.s_dist_02);
    s_dist_03.push_back(tuple.s_dist_03);
    s_dist_04.push_back(tuple.s_dist_04);
    s_dist_05.push_back(tuple.s_dist_05);
    s_dist_06.push_back(tuple.s_dist_06);
    s_dist_07.push_back(tuple.s_dist_07);
    s_dist_08.push_back(tuple.s_dist_08);
    s_dist_09.push_back(tuple.s_dist_09);
    s_dist_10.push_back(tuple.s_dist_10);
    s_ytd.push_back(tuple.s_ytd);
    s_order_cnt.push_back(tuple.s_order_cnt);
    s_remote_cnt.push_back(tuple.s_remote_cnt);
    s_data.push_back(tuple.s_data);

    auto insert_pos = size;
    primary_key[Key(tuple.s_w_id, tuple.s_i_id)] = insert_pos;

    size++;
    return insert_pos;
}

stockTuple stockTable::read_tuple(const uint64_t tid) {
}

void stockTable::update_tuple(const uint64_t tid, const stockTuple& tuple) {
}

void stockTable::delete_tuple(const uint64_t tid) {
}


}  // namespace tpcc
}  // namespace imlab
