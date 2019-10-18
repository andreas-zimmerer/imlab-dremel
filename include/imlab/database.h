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
};
struct CustomerTable {
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
};
struct HistoryTable {
    std::vector<Integer> h_c_id;
    std::vector<Integer> h_c_d_id;
    std::vector<Integer> h_c_w_id;
    std::vector<Integer> h_d_id;
    std::vector<Integer> h_w_id;
    std::vector<Timestamp> h_date;
    std::vector<Numeric<6, 2>> h_amount;
    std::vector<Varchar<24>> h_data;
};
struct NeworderTable {
    std::vector<Integer> no_o_id;
    std::vector<Integer> no_d_id;
    std::vector<Integer> no_w_id;

    std::unordered_map<Key<Integer, Integer, Integer>, uint64_t> index_prim_key;
};
struct OrderTable {
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
};
struct OrderlineTable {
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
};
struct ItemTable {
    std::vector<Integer> i_id;
    std::vector<Integer> i_im_id;
    std::vector<Varchar<24>> i_name;
    std::vector<Numeric<5, 2>> i_price;
    std::vector<Varchar<50>> i_data;

    std::unordered_map<Key<Integer>, uint64_t> index_prim_key;
};
struct StockTable {
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
