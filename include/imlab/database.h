// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#ifndef INCLUDE_IMLAB_DATABASE_H_
#define INCLUDE_IMLAB_DATABASE_H_

#include <istream>
#include <array>
#include <unordered_map>
#include <vector>
#include "imlab/infra/hash.h"
#include "imlab/schema.h"

namespace imlab {

struct WarehouseTable {
    uint64_t size = 0;

    std::vector<Integer> w_id;
    std::vector<Varchar<10>> w_name;
    std::vector<Varchar<20>> w_street_1;
    std::vector<Varchar<20>> w_street_2;
    std::vector<Varchar<20>> w_city;
    std::vector<Char<2>> w_state;
    std::vector<Char<9>> w_zip;
    std::vector<Numeric<4, 4>> w_tax;
    std::vector<Numeric<12, 2>> w_ytd;

    std::unordered_map<Key<Integer>, uint64_t> index_prim_key;

    void append_row(Integer w_id_val, Varchar<10> w_name_val, Varchar<20> w_street_1_val,
            Varchar<20> w_street_2_val, Varchar<20> w_city_val, Char<2> w_state_val,
            Char<9> w_zip_val, Numeric<4, 4> w_tax_val, Numeric<12, 2> w_ytd_val) {

        w_id.push_back(w_id_val);
        w_name.push_back(w_name_val);
        w_street_1.push_back(w_street_1_val);
        w_street_2.push_back(w_street_2_val);
        w_city.push_back(w_city_val);
        w_state.push_back(w_state_val);
        w_zip.push_back(w_zip_val);
        w_tax.push_back(w_tax_val);
        w_ytd.push_back(w_ytd_val);

        index_prim_key[Key(w_id_val)] = size;

        size++;

        assert(w_id.size() == size
        && w_name.size() == size
        && w_street_1.size() == size
        && w_street_2.size() == size
        && w_city.size() == size
        && w_state.size() == size
        && w_zip.size() == size
        && w_tax.size() == size
        && w_ytd.size() == size
        && index_prim_key.size() == size);
    }
};
struct DistrictTable {
    uint64_t size = 0;

    std::vector<Integer> d_id;
    std::vector<Integer> d_w_id;
    std::vector<Varchar<10>> d_name;
    std::vector<Varchar<20>> d_street_1;
    std::vector<Varchar<20>> d_street_2;
    std::vector<Varchar<20>> d_city;
    std::vector<Char<2>> d_state;
    std::vector<Char<9>> d_zip;
    std::vector<Numeric<4, 4>> d_tax;
    std::vector<Numeric<12, 2>> d_ytd;
    std::vector<Integer> d_next_o_id;

    std::unordered_map<Key<Integer, Integer>, uint64_t> index_prim_key;

    void append_row(Integer d_id_val, Integer d_w_id_val, Varchar<10> d_name_val,
            Varchar<20> d_street_1_val, Varchar<20> d_street_2_val, Varchar<20> d_city_val,
            Char<2> d_state_val, Char<9> d_zip_val, Numeric<4, 4> d_tax_val,
            Numeric<12, 2> d_ytd_val, Integer d_next_o_id_val) {

        d_id.push_back(d_id_val);
        d_w_id.push_back(d_w_id_val);
        d_name.push_back(d_name_val);
        d_street_1.push_back(d_street_1_val);
        d_street_2.push_back(d_street_2_val);
        d_city.push_back(d_city_val);
        d_state.push_back(d_state_val);
        d_zip.push_back(d_zip_val);
        d_tax.push_back(d_tax_val);
        d_ytd.push_back(d_ytd_val);
        d_next_o_id.push_back(d_next_o_id_val);

        index_prim_key[Key(d_w_id_val, d_id_val)] = size;

        size++;

        assert(d_id.size() == size
        && d_w_id.size() == size
        && d_name.size() == size
        && d_street_1.size() == size
        && d_street_2.size() == size
        && d_city.size() == size
        && d_state.size() == size
        && d_zip.size() == size
        && d_tax.size() == size
        && d_ytd.size() == size
        && d_next_o_id.size() == size
        && index_prim_key.size() == size);
    }
};
struct CustomerTable {
    uint64_t size = 0;

    std::vector<Integer> c_id;
    std::vector<Integer> c_d_id;
    std::vector<Integer> c_w_id;
    std::vector<Varchar<16>> c_first;
    std::vector<Char<2>> c_middle;
    std::vector<Varchar<16>> c_last;
    std::vector<Varchar<20>> c_street_1;
    std::vector<Varchar<20>> c_street_2;
    std::vector<Varchar<20>> c_city;
    std::vector<Char<2>> c_state;
    std::vector<Char<9>> c_zip;
    std::vector<Char<16>> c_phone;
    std::vector<Timestamp> c_since;
    std::vector<Char<2>> c_credit;
    std::vector<Numeric<12, 2>> c_credit_lim;
    std::vector<Numeric<4, 4>> c_discount;
    std::vector<Numeric<12, 2>> c_balance;
    std::vector<Numeric<12, 2>> c_ytd_paymenr;
    std::vector<Numeric<4, 0>> c_payment_cnt;
    std::vector<Numeric<4, 0>> c_delivery_cnt;
    std::vector<Varchar<500>> c_data;

    std::unordered_map<Key<Integer, Integer, Integer>, uint64_t> index_prim_key;
    std::unordered_map<Key<Integer, Integer, Varchar<16>, Varchar<16>>, uint64_t> index_wdl;

    void append_row(Integer c_id_val, Integer c_d_id_val, Integer c_w_id_val,
            Varchar<16> c_first_val, Char<2> c_middle_val, Varchar<16> c_last_val,
            Varchar<20> c_street_1_val, Varchar<20> c_street_2_val, Varchar<20> c_city_val,
            Char<2> c_state_val, Char<9> c_zip_val, Char<16> c_phone_val, Timestamp c_since_val,
            Char<2> c_credit_val, Numeric<12, 2> c_credit_lim_val, Numeric<4, 4> c_discount_val,
            Numeric<12, 2> c_balance_val, Numeric<12, 2> c_ytd_paymenr_val,
            Numeric<4, 0> c_payment_cnt_val, Numeric<4, 0> c_delivery_cnt_val, Varchar<500> c_data_val) {

        c_id.push_back(c_id_val);
        c_d_id.push_back(c_d_id_val);
        c_w_id.push_back(c_w_id_val);
        c_first.push_back(c_first_val);
        c_middle.push_back(c_middle_val);
        c_last.push_back(c_last_val);
        c_street_1.push_back(c_street_1_val);
        c_street_2.push_back(c_street_2_val);
        c_city.push_back(c_city_val);
        c_state.push_back(c_state_val);
        c_zip.push_back(c_zip_val);
        c_phone.push_back(c_phone_val);
        c_since.push_back(c_since_val);
        c_credit.push_back(c_credit_val);
        c_credit_lim.push_back(c_credit_lim_val);
        c_discount.push_back(c_discount_val);
        c_balance.push_back(c_balance_val);
        c_ytd_paymenr.push_back(c_ytd_paymenr_val);
        c_payment_cnt.push_back(c_payment_cnt_val);
        c_delivery_cnt.push_back(c_delivery_cnt_val);

        index_prim_key[Key(c_w_id_val, c_d_id_val, c_id_val)] = size;
        index_wdl[Key(c_w_id_val, c_d_id_val, c_last_val, c_first_val)] = size;

        size++;

        assert(c_id.size() == size
        && c_d_id.size() == size
        && c_w_id.size() == size
        && c_first.size() == size
        && c_middle.size() == size
        && c_last.size() == size
        && c_street_1.size() == size
        && c_street_2.size() == size
        && c_city.size() == size
        && c_state.size() == size
        && c_zip.size() == size
        && c_phone.size() == size
        && c_since.size() == size
        && c_credit.size() == size
        && c_credit_lim.size() == size
        && c_discount.size() == size
        && c_balance.size() == size
        && c_ytd_paymenr.size() == size
        && c_payment_cnt.size() == size
        && c_delivery_cnt.size() == size
        && index_prim_key.size() == size
        && index_wdl.size() == size);
    }
};
struct HistoryTable {
    uint64_t size = 0;

    std::vector<Integer> h_c_id;
    std::vector<Integer> h_c_d_id;
    std::vector<Integer> h_c_w_id;
    std::vector<Integer> h_d_id;
    std::vector<Integer> h_w_id;
    std::vector<Timestamp> h_date;
    std::vector<Numeric<6, 2>> h_amount;
    std::vector<Varchar<24>> h_data;

    void append_row(Integer h_c_id_val, Integer h_c_d_id_val, Integer h_c_w_id_val,
            Integer h_d_id_val, Integer h_w_id_val, Timestamp h_date_val,
            Numeric<6, 2> h_amount_val, Varchar<24> h_data_val) {
        h_c_id.push_back(h_c_id_val);
        h_c_d_id.push_back(h_c_d_id_val);
        h_c_w_id.push_back(h_c_w_id_val);
        h_d_id.push_back(h_d_id_val);
        h_w_id.push_back(h_w_id_val);
        h_date.push_back(h_date_val);
        h_amount.push_back(h_amount_val);
        h_data.push_back(h_data_val);

        size++;

        assert(h_c_id.size() == size
        && h_c_d_id.size() == size
        && h_c_w_id.size() == size
        && h_d_id.size() == size
        && h_w_id.size() == size
        && h_date.size() == size
        && h_amount.size() == size
        && h_data.size() == size);
    }
};
struct NeworderTable {
    uint64_t size = 0;

    std::vector<Integer> no_o_id;
    std::vector<Integer> no_d_id;
    std::vector<Integer> no_w_id;

    std::unordered_map<Key<Integer, Integer, Integer>, uint64_t> index_prim_key;

    void append_row(Integer no_o_id_val, Integer no_d_id_val, Integer no_w_id_val) {
        no_o_id.push_back(no_o_id_val);
        no_d_id.push_back(no_d_id_val);
        no_w_id.push_back(no_w_id_val);

        index_prim_key[Key(no_o_id_val, no_d_id_val, no_w_id_val)] = size;

        size++;

        assert(no_o_id.size() == size
        && no_d_id.size() == size
        && no_w_id.size() == size
        && index_prim_key.size() == size);
    }
};
struct OrderTable {
    uint64_t size = 0;

    std::vector<Integer> o_id;
    std::vector<Integer> o_d_id;
    std::vector<Integer> o_w_id;
    std::vector<Integer> o_c_id;
    std::vector<Timestamp> o_entry_d;
    std::vector<Integer> o_carrier_id;
    std::vector<Numeric<2, 0>> o_ol_cnt;
    std::vector<Numeric<1, 0>> o_all_local;

    std::unordered_map<Key<Integer, Integer, Integer>, uint64_t> index_prim_key;
    std::unordered_map<Key<Integer, Integer, Integer, Integer>, uint64_t> index_wdc;

    void append_row(Integer o_id_val, Integer o_d_id_val, Integer o_w_id_val,
            Integer o_c_id_val, Timestamp o_entry_d_val, Integer o_carrier_id_val,
            Numeric<2, 0> o_ol_cnt_val, Numeric<1, 0> o_all_local_val) {
        o_id.push_back(o_id_val);
        o_d_id.push_back(o_d_id_val);
        o_w_id.push_back(o_w_id_val);
        o_c_id.push_back(o_c_id_val);
        o_entry_d.push_back(o_entry_d_val);
        o_carrier_id.push_back(o_carrier_id_val);
        o_ol_cnt.push_back(o_ol_cnt_val);
        o_all_local.push_back(o_all_local_val);

        index_prim_key[Key(o_w_id_val, o_d_id_val, o_id_val)] = size;
        index_wdc[Key(o_w_id_val, o_d_id_val, o_c_id_val, o_id_val)] = size;

        size++;

        assert(o_id.size() == size
        && o_d_id.size() == size
        && o_w_id.size() == size
        && o_c_id.size() == size
        && o_entry_d.size() == size
        && o_carrier_id.size() == size
        && o_ol_cnt.size() == size
        && o_all_local.size() == size
        && index_prim_key.size() == size
        && index_wdc.size() == size);
    }
};
struct OrderlineTable {
    uint64_t size = 0;

    std::vector<Integer> ol_o_id;
    std::vector<Integer> ol_d_id;
    std::vector<Integer> ol_w_id;
    std::vector<Integer> ol_number;
    std::vector<Integer> ol_i_id;
    std::vector<Integer> ol_supply_w_id;
    std::vector<Timestamp> ol_delivery_d;
    std::vector<Numeric<2, 0>> ol_quantity;
    std::vector<Numeric<6, 2>> ol_amount;
    std::vector<Char<24>> ol_dist_info;

    std::unordered_map<Key<Integer, Integer, Integer, Integer>, uint64_t> index_prim_key;

    void append_row(Integer ol_o_id_val, Integer ol_d_id_val, Integer ol_w_id_val,
            Integer ol_number_val, Integer ol_i_id_val, Integer ol_supply_w_id_val,
            Timestamp ol_delivery_d_val, Numeric<2, 0> ol_quantity_val,
            Numeric<6, 2> ol_amount_val, Char<24> ol_dist_info_val) {
        ol_o_id.push_back(ol_o_id_val);
        ol_d_id.push_back(ol_d_id_val);
        ol_w_id.push_back(ol_w_id_val);
        ol_number.push_back(ol_number_val);
        ol_i_id.push_back(ol_i_id_val);
        ol_supply_w_id.push_back(ol_supply_w_id_val);
        ol_delivery_d.push_back(ol_delivery_d_val);
        ol_quantity.push_back(ol_quantity_val);
        ol_amount.push_back(ol_amount_val);
        ol_dist_info.push_back(ol_dist_info_val);

        index_prim_key[Key(ol_w_id_val, ol_d_id_val, ol_o_id_val, ol_number_val)] = size;

        size++;

        assert(ol_i_id.size() == size
        && ol_d_id.size() == size
        && ol_w_id.size() == size
        && ol_number.size() == size
        && ol_i_id.size() == size
        && ol_supply_w_id.size() == size
        && ol_delivery_d.size() == size
        && ol_quantity.size() == size
        && ol_amount.size() == size
        && ol_dist_info.size() == size
        && index_prim_key.size() == size);
    }
};
struct ItemTable {
    uint64_t size = 0;

    std::vector<Integer> i_id;
    std::vector<Integer> i_im_id;
    std::vector<Varchar<24>> i_name;
    std::vector<Numeric<5, 2>> i_price;
    std::vector<Varchar<50>> i_data;

    std::unordered_map<Key<Integer>, uint64_t> index_prim_key;

    void append_row(Integer i_id_val, Integer i_im_id_val, Varchar<24> i_name_val,
            Numeric<5, 2> i_price_val, Varchar<50> i_data_val) {
        i_id.push_back(i_id_val);
        i_im_id.push_back(i_im_id_val);
        i_name.push_back(i_name_val);
        i_price.push_back(i_price_val);
        i_data.push_back(i_data_val);

        index_prim_key[Key(i_id_val)] = size;

        size++;

        assert(i_id.size() == size
        && i_im_id.size() == size
        && i_name.size() == size
        && i_price.size() == size
        && i_data.size() == size
        && index_prim_key.size() == size);
    }
};
struct StockTable {
    uint64_t size = 0;

    std::vector<Integer> s_i_id;
    std::vector<Integer> s_w_id;
    std::vector<Numeric<4, 0>> s_quantity;
    std::vector<Char<24>> s_dist_01;
    std::vector<Char<24>> s_dist_02;
    std::vector<Char<24>> s_dist_03;
    std::vector<Char<24>> s_dist_04;
    std::vector<Char<24>> s_dist_05;
    std::vector<Char<24>> s_dist_06;
    std::vector<Char<24>> s_dist_07;
    std::vector<Char<24>> s_dist_08;
    std::vector<Char<24>> s_dist_09;
    std::vector<Char<24>> s_dist_10;
    std::vector<Numeric<8, 0>> s_ytd;
    std::vector<Numeric<4, 0>> s_order_cnt;
    std::vector<Numeric<4, 0>> s_remote_cnt;
    std::vector<Varchar<50>> s_data;

    std::unordered_map<Key<Integer, Integer>, uint64_t> index_prim_key;

    void append_row(Integer s_i_id_val, Integer s_w_id_val, Numeric<4, 0> s_quantity_val,
            Char<24> s_dist_01_val, Char<24> s_dist_02_val, Char<24> s_dist_03_val,
            Char<24> s_dist_04_val, Char<24> s_dist_05_val, Char<24> s_dist_06_val,
            Char<24> s_dist_07_val, Char<24> s_dist_08_val, Char<24> s_dist_09_val,
            Char<24> s_dist_10_val, Numeric<8, 0> s_ytd_val, Numeric<4, 0> s_order_cnt_val,
            Numeric<4, 0> s_remote_cnt_val, Varchar<50> s_data_val) {
        s_i_id.push_back(s_i_id_val);
        s_w_id.push_back(s_w_id_val);
        s_quantity.push_back(s_quantity_val);
        s_dist_01.push_back(s_dist_01_val);
        s_dist_02.push_back(s_dist_02_val);
        s_dist_03.push_back(s_dist_03_val);
        s_dist_04.push_back(s_dist_04_val);
        s_dist_05.push_back(s_dist_05_val);
        s_dist_06.push_back(s_dist_06_val);
        s_dist_07.push_back(s_dist_07_val);
        s_dist_08.push_back(s_dist_08_val);
        s_dist_09.push_back(s_dist_09_val);
        s_dist_10.push_back(s_dist_10_val);
        s_ytd.push_back(s_ytd_val);
        s_order_cnt.push_back(s_order_cnt_val);
        s_remote_cnt.push_back(s_remote_cnt_val);
        s_data.push_back(s_data_val);

        index_prim_key[Key(s_w_id_val, s_i_id_val)] = size;

        size++;

        assert(s_i_id.size() == size
        && s_w_id.size() == size
        && s_quantity.size() == size
        && s_dist_01.size() == size
        && s_dist_02.size() == size
        && s_dist_03.size() == size
        && s_dist_04.size() == size
        && s_dist_05.size() == size
        && s_dist_06.size() == size
        && s_dist_07.size() == size
        && s_dist_08.size() == size
        && s_dist_09.size() == size
        && s_dist_10.size() == size
        && s_ytd.size() == size
        && s_order_cnt.size() == size
        && s_remote_cnt.size() == size
        && s_data.size() == size
        && index_prim_key.size() == size);
    }
};


class Database {
 public:
    // Load the TPCC data.
    // Call these methods with std::ifstreams to load your data.
    void LoadCustomer(std::istream &in);
    void LoadDistrict(std::istream &in);
    void LoadHistory(std::istream &in);
    void LoadItem(std::istream &in);
    void LoadNewOrder(std::istream &in);
    void LoadOrder(std::istream &in);
    void LoadOrderLine(std::istream &in);
    void LoadStock(std::istream &in);
    void LoadWarehouse(std::istream &in);

    // Tables
    WarehouseTable warehouseTable;
    DistrictTable districtTable;
    CustomerTable customerTable;
    HistoryTable historyTable;
    NeworderTable neworderTable;
    OrderTable orderTable;
    OrderlineTable orderlineTable;
    ItemTable itemTable;
    StockTable stockTable;

    // Place a new order.
    void NewOrder(
        Integer w_id,
        Integer d_id,
        Integer c_id,
        Integer items,
        std::array<Integer, 15> &supware,
        std::array<Integer, 15> &itemid,
        std::array<Integer, 15> &qty,
        Timestamp datetime);

    // Returns the number of tuples in a relation.
    template <tpcc::Relation> size_t Size();
};

}  // namespace imlab

#endif  // INCLUDE_IMLAB_DATABASE_H_
