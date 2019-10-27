#include "imlab/schema.h"

namespace imlab {
namespace tpcc {

uint64_t warehouseTable::insert_tuple(const warehouseTuple& tuple) {
    auto insert_pos = size;
    w_id.push_back(tuple.w_id);
    w_name.push_back(tuple.w_name);
    w_street_1.push_back(tuple.w_street_1);
    w_street_2.push_back(tuple.w_street_2);
    w_city.push_back(tuple.w_city);
    w_state.push_back(tuple.w_state);
    w_zip.push_back(tuple.w_zip);
    w_tax.push_back(tuple.w_tax);
    w_ytd.push_back(tuple.w_ytd);
    deleted.push_back(false);

    primary_key[Key(tuple.w_id)] = insert_pos;

    size++;
    return insert_pos;
}

std::optional<warehouseTuple> warehouseTable::read_tuple(const uint64_t tid) {
    if (deleted[tid] == true) {
        return std::nullopt;
    }

    return warehouseTuple {
        w_id[tid],
        w_name[tid],
        w_street_1[tid],
        w_street_2[tid],
        w_city[tid],
        w_state[tid],
        w_zip[tid],
        w_tax[tid],
        w_ytd[tid],
    };
}

void warehouseTable::update_tuple(const uint64_t tid, const warehouseTuple& tuple) {
    // delete the entry from the primary key index
    auto w_id_old = w_id[tid];
    primary_key.erase(Key(w_id_old));

    w_id[tid] = tuple.w_id;
    w_name[tid] = tuple.w_name;
    w_street_1[tid] = tuple.w_street_1;
    w_street_2[tid] = tuple.w_street_2;
    w_city[tid] = tuple.w_city;
    w_state[tid] = tuple.w_state;
    w_zip[tid] = tuple.w_zip;
    w_tax[tid] = tuple.w_tax;
    w_ytd[tid] = tuple.w_ytd;

    // update index on primary key
    primary_key[Key(tuple.w_id)] = tid;

}

void warehouseTable::delete_tuple(const uint64_t tid) {
    deleted[tid] = true;
}


uint64_t districtTable::insert_tuple(const districtTuple& tuple) {
    auto insert_pos = size;
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
    deleted.push_back(false);

    primary_key[Key(tuple.d_w_id, tuple.d_id)] = insert_pos;

    size++;
    return insert_pos;
}

std::optional<districtTuple> districtTable::read_tuple(const uint64_t tid) {
    if (deleted[tid] == true) {
        return std::nullopt;
    }

    return districtTuple {
        d_id[tid],
        d_w_id[tid],
        d_name[tid],
        d_street_1[tid],
        d_street_2[tid],
        d_city[tid],
        d_state[tid],
        d_zip[tid],
        d_tax[tid],
        d_ytd[tid],
        d_next_o_id[tid],
    };
}

void districtTable::update_tuple(const uint64_t tid, const districtTuple& tuple) {
    // delete the entry from the primary key index
    auto d_w_id_old = d_w_id[tid];
    auto d_id_old = d_id[tid];
    primary_key.erase(Key(d_w_id_old, d_id_old));

    d_id[tid] = tuple.d_id;
    d_w_id[tid] = tuple.d_w_id;
    d_name[tid] = tuple.d_name;
    d_street_1[tid] = tuple.d_street_1;
    d_street_2[tid] = tuple.d_street_2;
    d_city[tid] = tuple.d_city;
    d_state[tid] = tuple.d_state;
    d_zip[tid] = tuple.d_zip;
    d_tax[tid] = tuple.d_tax;
    d_ytd[tid] = tuple.d_ytd;
    d_next_o_id[tid] = tuple.d_next_o_id;

    // update index on primary key
    primary_key[Key(tuple.d_w_id, tuple.d_id)] = tid;

}

void districtTable::delete_tuple(const uint64_t tid) {
    deleted[tid] = true;
}


uint64_t customerTable::insert_tuple(const customerTuple& tuple) {
    auto insert_pos = size;
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
    deleted.push_back(false);

    primary_key[Key(tuple.c_w_id, tuple.c_d_id, tuple.c_id)] = insert_pos;

    size++;
    return insert_pos;
}

std::optional<customerTuple> customerTable::read_tuple(const uint64_t tid) {
    if (deleted[tid] == true) {
        return std::nullopt;
    }

    return customerTuple {
        c_id[tid],
        c_d_id[tid],
        c_w_id[tid],
        c_first[tid],
        c_middle[tid],
        c_last[tid],
        c_street_1[tid],
        c_street_2[tid],
        c_city[tid],
        c_state[tid],
        c_zip[tid],
        c_phone[tid],
        c_since[tid],
        c_credit[tid],
        c_credit_lim[tid],
        c_discount[tid],
        c_balance[tid],
        c_ytd_paymenr[tid],
        c_payment_cnt[tid],
        c_delivery_cnt[tid],
        c_data[tid],
    };
}

void customerTable::update_tuple(const uint64_t tid, const customerTuple& tuple) {
    // delete the entry from the primary key index
    auto c_w_id_old = c_w_id[tid];
    auto c_d_id_old = c_d_id[tid];
    auto c_id_old = c_id[tid];
    primary_key.erase(Key(c_w_id_old, c_d_id_old, c_id_old));

    c_id[tid] = tuple.c_id;
    c_d_id[tid] = tuple.c_d_id;
    c_w_id[tid] = tuple.c_w_id;
    c_first[tid] = tuple.c_first;
    c_middle[tid] = tuple.c_middle;
    c_last[tid] = tuple.c_last;
    c_street_1[tid] = tuple.c_street_1;
    c_street_2[tid] = tuple.c_street_2;
    c_city[tid] = tuple.c_city;
    c_state[tid] = tuple.c_state;
    c_zip[tid] = tuple.c_zip;
    c_phone[tid] = tuple.c_phone;
    c_since[tid] = tuple.c_since;
    c_credit[tid] = tuple.c_credit;
    c_credit_lim[tid] = tuple.c_credit_lim;
    c_discount[tid] = tuple.c_discount;
    c_balance[tid] = tuple.c_balance;
    c_ytd_paymenr[tid] = tuple.c_ytd_paymenr;
    c_payment_cnt[tid] = tuple.c_payment_cnt;
    c_delivery_cnt[tid] = tuple.c_delivery_cnt;
    c_data[tid] = tuple.c_data;

    // update index on primary key
    primary_key[Key(tuple.c_w_id, tuple.c_d_id, tuple.c_id)] = tid;

}

void customerTable::delete_tuple(const uint64_t tid) {
    deleted[tid] = true;
}


uint64_t historyTable::insert_tuple(const historyTuple& tuple) {
    auto insert_pos = size;
    h_c_id.push_back(tuple.h_c_id);
    h_c_d_id.push_back(tuple.h_c_d_id);
    h_c_w_id.push_back(tuple.h_c_w_id);
    h_d_id.push_back(tuple.h_d_id);
    h_w_id.push_back(tuple.h_w_id);
    h_date.push_back(tuple.h_date);
    h_amount.push_back(tuple.h_amount);
    h_data.push_back(tuple.h_data);
    deleted.push_back(false);

    size++;
    return insert_pos;
}

std::optional<historyTuple> historyTable::read_tuple(const uint64_t tid) {
    if (deleted[tid] == true) {
        return std::nullopt;
    }

    return historyTuple {
        h_c_id[tid],
        h_c_d_id[tid],
        h_c_w_id[tid],
        h_d_id[tid],
        h_w_id[tid],
        h_date[tid],
        h_amount[tid],
        h_data[tid],
    };
}

void historyTable::update_tuple(const uint64_t tid, const historyTuple& tuple) {
    h_c_id[tid] = tuple.h_c_id;
    h_c_d_id[tid] = tuple.h_c_d_id;
    h_c_w_id[tid] = tuple.h_c_w_id;
    h_d_id[tid] = tuple.h_d_id;
    h_w_id[tid] = tuple.h_w_id;
    h_date[tid] = tuple.h_date;
    h_amount[tid] = tuple.h_amount;
    h_data[tid] = tuple.h_data;
}

void historyTable::delete_tuple(const uint64_t tid) {
    deleted[tid] = true;
}


uint64_t neworderTable::insert_tuple(const neworderTuple& tuple) {
    auto insert_pos = size;
    no_o_id.push_back(tuple.no_o_id);
    no_d_id.push_back(tuple.no_d_id);
    no_w_id.push_back(tuple.no_w_id);
    deleted.push_back(false);

    primary_key[Key(tuple.no_w_id, tuple.no_d_id, tuple.no_o_id)] = insert_pos;

    size++;
    return insert_pos;
}

std::optional<neworderTuple> neworderTable::read_tuple(const uint64_t tid) {
    if (deleted[tid] == true) {
        return std::nullopt;
    }

    return neworderTuple {
        no_o_id[tid],
        no_d_id[tid],
        no_w_id[tid],
    };
}

void neworderTable::update_tuple(const uint64_t tid, const neworderTuple& tuple) {
    // delete the entry from the primary key index
    auto no_w_id_old = no_w_id[tid];
    auto no_d_id_old = no_d_id[tid];
    auto no_o_id_old = no_o_id[tid];
    primary_key.erase(Key(no_w_id_old, no_d_id_old, no_o_id_old));

    no_o_id[tid] = tuple.no_o_id;
    no_d_id[tid] = tuple.no_d_id;
    no_w_id[tid] = tuple.no_w_id;

    // update index on primary key
    primary_key[Key(tuple.no_w_id, tuple.no_d_id, tuple.no_o_id)] = tid;

}

void neworderTable::delete_tuple(const uint64_t tid) {
    deleted[tid] = true;
}


uint64_t orderTable::insert_tuple(const orderTuple& tuple) {
    auto insert_pos = size;
    o_id.push_back(tuple.o_id);
    o_d_id.push_back(tuple.o_d_id);
    o_w_id.push_back(tuple.o_w_id);
    o_c_id.push_back(tuple.o_c_id);
    o_entry_d.push_back(tuple.o_entry_d);
    o_carrier_id.push_back(tuple.o_carrier_id);
    o_ol_cnt.push_back(tuple.o_ol_cnt);
    o_all_local.push_back(tuple.o_all_local);
    deleted.push_back(false);

    primary_key[Key(tuple.o_w_id, tuple.o_d_id, tuple.o_id)] = insert_pos;

    size++;
    return insert_pos;
}

std::optional<orderTuple> orderTable::read_tuple(const uint64_t tid) {
    if (deleted[tid] == true) {
        return std::nullopt;
    }

    return orderTuple {
        o_id[tid],
        o_d_id[tid],
        o_w_id[tid],
        o_c_id[tid],
        o_entry_d[tid],
        o_carrier_id[tid],
        o_ol_cnt[tid],
        o_all_local[tid],
    };
}

void orderTable::update_tuple(const uint64_t tid, const orderTuple& tuple) {
    // delete the entry from the primary key index
    auto o_w_id_old = o_w_id[tid];
    auto o_d_id_old = o_d_id[tid];
    auto o_id_old = o_id[tid];
    primary_key.erase(Key(o_w_id_old, o_d_id_old, o_id_old));

    o_id[tid] = tuple.o_id;
    o_d_id[tid] = tuple.o_d_id;
    o_w_id[tid] = tuple.o_w_id;
    o_c_id[tid] = tuple.o_c_id;
    o_entry_d[tid] = tuple.o_entry_d;
    o_carrier_id[tid] = tuple.o_carrier_id;
    o_ol_cnt[tid] = tuple.o_ol_cnt;
    o_all_local[tid] = tuple.o_all_local;

    // update index on primary key
    primary_key[Key(tuple.o_w_id, tuple.o_d_id, tuple.o_id)] = tid;

}

void orderTable::delete_tuple(const uint64_t tid) {
    deleted[tid] = true;
}


uint64_t orderlineTable::insert_tuple(const orderlineTuple& tuple) {
    auto insert_pos = size;
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
    deleted.push_back(false);

    primary_key[Key(tuple.ol_w_id, tuple.ol_d_id, tuple.ol_o_id, tuple.ol_number)] = insert_pos;

    size++;
    return insert_pos;
}

std::optional<orderlineTuple> orderlineTable::read_tuple(const uint64_t tid) {
    if (deleted[tid] == true) {
        return std::nullopt;
    }

    return orderlineTuple {
        ol_o_id[tid],
        ol_d_id[tid],
        ol_w_id[tid],
        ol_number[tid],
        ol_i_id[tid],
        ol_supply_w_id[tid],
        ol_delivery_d[tid],
        ol_quantity[tid],
        ol_amount[tid],
        ol_dist_info[tid],
    };
}

void orderlineTable::update_tuple(const uint64_t tid, const orderlineTuple& tuple) {
    // delete the entry from the primary key index
    auto ol_w_id_old = ol_w_id[tid];
    auto ol_d_id_old = ol_d_id[tid];
    auto ol_o_id_old = ol_o_id[tid];
    auto ol_number_old = ol_number[tid];
    primary_key.erase(Key(ol_w_id_old, ol_d_id_old, ol_o_id_old, ol_number_old));

    ol_o_id[tid] = tuple.ol_o_id;
    ol_d_id[tid] = tuple.ol_d_id;
    ol_w_id[tid] = tuple.ol_w_id;
    ol_number[tid] = tuple.ol_number;
    ol_i_id[tid] = tuple.ol_i_id;
    ol_supply_w_id[tid] = tuple.ol_supply_w_id;
    ol_delivery_d[tid] = tuple.ol_delivery_d;
    ol_quantity[tid] = tuple.ol_quantity;
    ol_amount[tid] = tuple.ol_amount;
    ol_dist_info[tid] = tuple.ol_dist_info;

    // update index on primary key
    primary_key[Key(tuple.ol_w_id, tuple.ol_d_id, tuple.ol_o_id, tuple.ol_number)] = tid;

}

void orderlineTable::delete_tuple(const uint64_t tid) {
    deleted[tid] = true;
}


uint64_t itemTable::insert_tuple(const itemTuple& tuple) {
    auto insert_pos = size;
    i_id.push_back(tuple.i_id);
    i_im_id.push_back(tuple.i_im_id);
    i_name.push_back(tuple.i_name);
    i_price.push_back(tuple.i_price);
    i_data.push_back(tuple.i_data);
    deleted.push_back(false);

    primary_key[Key(tuple.i_id)] = insert_pos;

    size++;
    return insert_pos;
}

std::optional<itemTuple> itemTable::read_tuple(const uint64_t tid) {
    if (deleted[tid] == true) {
        return std::nullopt;
    }

    return itemTuple {
        i_id[tid],
        i_im_id[tid],
        i_name[tid],
        i_price[tid],
        i_data[tid],
    };
}

void itemTable::update_tuple(const uint64_t tid, const itemTuple& tuple) {
    // delete the entry from the primary key index
    auto i_id_old = i_id[tid];
    primary_key.erase(Key(i_id_old));

    i_id[tid] = tuple.i_id;
    i_im_id[tid] = tuple.i_im_id;
    i_name[tid] = tuple.i_name;
    i_price[tid] = tuple.i_price;
    i_data[tid] = tuple.i_data;

    // update index on primary key
    primary_key[Key(tuple.i_id)] = tid;

}

void itemTable::delete_tuple(const uint64_t tid) {
    deleted[tid] = true;
}


uint64_t stockTable::insert_tuple(const stockTuple& tuple) {
    auto insert_pos = size;
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
    deleted.push_back(false);

    primary_key[Key(tuple.s_w_id, tuple.s_i_id)] = insert_pos;

    size++;
    return insert_pos;
}

std::optional<stockTuple> stockTable::read_tuple(const uint64_t tid) {
    if (deleted[tid] == true) {
        return std::nullopt;
    }

    return stockTuple {
        s_i_id[tid],
        s_w_id[tid],
        s_quantity[tid],
        s_dist_01[tid],
        s_dist_02[tid],
        s_dist_03[tid],
        s_dist_04[tid],
        s_dist_05[tid],
        s_dist_06[tid],
        s_dist_07[tid],
        s_dist_08[tid],
        s_dist_09[tid],
        s_dist_10[tid],
        s_ytd[tid],
        s_order_cnt[tid],
        s_remote_cnt[tid],
        s_data[tid],
    };
}

void stockTable::update_tuple(const uint64_t tid, const stockTuple& tuple) {
    // delete the entry from the primary key index
    auto s_w_id_old = s_w_id[tid];
    auto s_i_id_old = s_i_id[tid];
    primary_key.erase(Key(s_w_id_old, s_i_id_old));

    s_i_id[tid] = tuple.s_i_id;
    s_w_id[tid] = tuple.s_w_id;
    s_quantity[tid] = tuple.s_quantity;
    s_dist_01[tid] = tuple.s_dist_01;
    s_dist_02[tid] = tuple.s_dist_02;
    s_dist_03[tid] = tuple.s_dist_03;
    s_dist_04[tid] = tuple.s_dist_04;
    s_dist_05[tid] = tuple.s_dist_05;
    s_dist_06[tid] = tuple.s_dist_06;
    s_dist_07[tid] = tuple.s_dist_07;
    s_dist_08[tid] = tuple.s_dist_08;
    s_dist_09[tid] = tuple.s_dist_09;
    s_dist_10[tid] = tuple.s_dist_10;
    s_ytd[tid] = tuple.s_ytd;
    s_order_cnt[tid] = tuple.s_order_cnt;
    s_remote_cnt[tid] = tuple.s_remote_cnt;
    s_data[tid] = tuple.s_data;

    // update index on primary key
    primary_key[Key(tuple.s_w_id, tuple.s_i_id)] = tid;

}

void stockTable::delete_tuple(const uint64_t tid) {
    deleted[tid] = true;
}


}  // namespace tpcc
}  // namespace imlab
