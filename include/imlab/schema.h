#ifndef INCLUDE_IMLAB_SCHEMA_H_
#define INCLUDE_IMLAB_SCHEMA_H_

#include <vector>
#include <unordered_map>
#include "imlab/infra/hash.h"
#include "imlab/infra/types.h"

namespace imlab {
namespace tpcc {

template<typename Tuple>
class TableBase {
 public:
    virtual uint64_t insert_tuple(const Tuple& tuple) = 0;
    virtual std::optional<Tuple> read_tuple(const uint64_t tid) = 0;
    virtual void update_tuple(const uint64_t tid, const Tuple& tuple) = 0;
    virtual void delete_tuple(const uint64_t tid) = 0;
    uint64_t get_size() { return size; }
 protected:
    uint64_t size = 0;
};

enum Relation {
    kwarehouse,
    kdistrict,
    kcustomer,
    khistory,
    kneworder,
    korder,
    korderline,
    kitem,
    kstock,
};

struct warehouseTuple {
    Integer w_id;
    Varchar<10> w_name;
    Varchar<20> w_street_1;
    Varchar<20> w_street_2;
    Varchar<20> w_city;
    Char<2> w_state;
    Char<9> w_zip;
    Numeric<4, 4> w_tax;
    Numeric<12, 2> w_ytd;
};
class warehouseTable : public TableBase<warehouseTuple> {
 public:
    uint64_t insert_tuple(const warehouseTuple& tuple) override;
    std::optional<warehouseTuple> read_tuple(const uint64_t tid) override;
    void update_tuple(const uint64_t tid, const warehouseTuple& tuple) override;
    void delete_tuple(const uint64_t tid) override;

    std::unordered_map<Key<Integer>, uint64_t> primary_key;

 private:
    std::vector<bool> deleted;
    std::vector<Integer> w_id;
    std::vector<Varchar<10>> w_name;
    std::vector<Varchar<20>> w_street_1;
    std::vector<Varchar<20>> w_street_2;
    std::vector<Varchar<20>> w_city;
    std::vector<Char<2>> w_state;
    std::vector<Char<9>> w_zip;
    std::vector<Numeric<4, 4>> w_tax;
    std::vector<Numeric<12, 2>> w_ytd;
};

struct districtTuple {
    Integer d_id;
    Integer d_w_id;
    Varchar<10> d_name;
    Varchar<20> d_street_1;
    Varchar<20> d_street_2;
    Varchar<20> d_city;
    Char<2> d_state;
    Char<9> d_zip;
    Numeric<4, 4> d_tax;
    Numeric<12, 2> d_ytd;
    Integer d_next_o_id;
};
class districtTable : public TableBase<districtTuple> {
 public:
    uint64_t insert_tuple(const districtTuple& tuple) override;
    std::optional<districtTuple> read_tuple(const uint64_t tid) override;
    void update_tuple(const uint64_t tid, const districtTuple& tuple) override;
    void delete_tuple(const uint64_t tid) override;

    std::unordered_map<Key<Integer, Integer>, uint64_t> primary_key;

 private:
    std::vector<bool> deleted;
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
};

struct customerTuple {
    Integer c_id;
    Integer c_d_id;
    Integer c_w_id;
    Varchar<16> c_first;
    Char<2> c_middle;
    Varchar<16> c_last;
    Varchar<20> c_street_1;
    Varchar<20> c_street_2;
    Varchar<20> c_city;
    Char<2> c_state;
    Char<9> c_zip;
    Char<16> c_phone;
    Timestamp c_since;
    Char<2> c_credit;
    Numeric<12, 2> c_credit_lim;
    Numeric<4, 4> c_discount;
    Numeric<12, 2> c_balance;
    Numeric<12, 2> c_ytd_paymenr;
    Numeric<4, 0> c_payment_cnt;
    Numeric<4, 0> c_delivery_cnt;
    Varchar<500> c_data;
};
class customerTable : public TableBase<customerTuple> {
 public:
    uint64_t insert_tuple(const customerTuple& tuple) override;
    std::optional<customerTuple> read_tuple(const uint64_t tid) override;
    void update_tuple(const uint64_t tid, const customerTuple& tuple) override;
    void delete_tuple(const uint64_t tid) override;

    std::unordered_map<Key<Integer, Integer, Integer>, uint64_t> primary_key;

 private:
    std::vector<bool> deleted;
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
};

struct historyTuple {
    Integer h_c_id;
    Integer h_c_d_id;
    Integer h_c_w_id;
    Integer h_d_id;
    Integer h_w_id;
    Timestamp h_date;
    Numeric<6, 2> h_amount;
    Varchar<24> h_data;
};
class historyTable : public TableBase<historyTuple> {
 public:
    uint64_t insert_tuple(const historyTuple& tuple) override;
    std::optional<historyTuple> read_tuple(const uint64_t tid) override;
    void update_tuple(const uint64_t tid, const historyTuple& tuple) override;
    void delete_tuple(const uint64_t tid) override;

 private:
    std::vector<bool> deleted;
    std::vector<Integer> h_c_id;
    std::vector<Integer> h_c_d_id;
    std::vector<Integer> h_c_w_id;
    std::vector<Integer> h_d_id;
    std::vector<Integer> h_w_id;
    std::vector<Timestamp> h_date;
    std::vector<Numeric<6, 2>> h_amount;
    std::vector<Varchar<24>> h_data;
};

struct neworderTuple {
    Integer no_o_id;
    Integer no_d_id;
    Integer no_w_id;
};
class neworderTable : public TableBase<neworderTuple> {
 public:
    uint64_t insert_tuple(const neworderTuple& tuple) override;
    std::optional<neworderTuple> read_tuple(const uint64_t tid) override;
    void update_tuple(const uint64_t tid, const neworderTuple& tuple) override;
    void delete_tuple(const uint64_t tid) override;

    std::unordered_map<Key<Integer, Integer, Integer>, uint64_t> primary_key;

 private:
    std::vector<bool> deleted;
    std::vector<Integer> no_o_id;
    std::vector<Integer> no_d_id;
    std::vector<Integer> no_w_id;
};

struct orderTuple {
    Integer o_id;
    Integer o_d_id;
    Integer o_w_id;
    Integer o_c_id;
    Timestamp o_entry_d;
    Integer o_carrier_id;
    Numeric<2, 0> o_ol_cnt;
    Numeric<1, 0> o_all_local;
};
class orderTable : public TableBase<orderTuple> {
 public:
    uint64_t insert_tuple(const orderTuple& tuple) override;
    std::optional<orderTuple> read_tuple(const uint64_t tid) override;
    void update_tuple(const uint64_t tid, const orderTuple& tuple) override;
    void delete_tuple(const uint64_t tid) override;

    std::unordered_map<Key<Integer, Integer, Integer>, uint64_t> primary_key;

 private:
    std::vector<bool> deleted;
    std::vector<Integer> o_id;
    std::vector<Integer> o_d_id;
    std::vector<Integer> o_w_id;
    std::vector<Integer> o_c_id;
    std::vector<Timestamp> o_entry_d;
    std::vector<Integer> o_carrier_id;
    std::vector<Numeric<2, 0>> o_ol_cnt;
    std::vector<Numeric<1, 0>> o_all_local;
};

struct orderlineTuple {
    Integer ol_o_id;
    Integer ol_d_id;
    Integer ol_w_id;
    Integer ol_number;
    Integer ol_i_id;
    Integer ol_supply_w_id;
    Timestamp ol_delivery_d;
    Numeric<2, 0> ol_quantity;
    Numeric<6, 2> ol_amount;
    Char<24> ol_dist_info;
};
class orderlineTable : public TableBase<orderlineTuple> {
 public:
    uint64_t insert_tuple(const orderlineTuple& tuple) override;
    std::optional<orderlineTuple> read_tuple(const uint64_t tid) override;
    void update_tuple(const uint64_t tid, const orderlineTuple& tuple) override;
    void delete_tuple(const uint64_t tid) override;

    std::unordered_map<Key<Integer, Integer, Integer, Integer>, uint64_t> primary_key;

 private:
    std::vector<bool> deleted;
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
};

struct itemTuple {
    Integer i_id;
    Integer i_im_id;
    Varchar<24> i_name;
    Numeric<5, 2> i_price;
    Varchar<50> i_data;
};
class itemTable : public TableBase<itemTuple> {
 public:
    uint64_t insert_tuple(const itemTuple& tuple) override;
    std::optional<itemTuple> read_tuple(const uint64_t tid) override;
    void update_tuple(const uint64_t tid, const itemTuple& tuple) override;
    void delete_tuple(const uint64_t tid) override;

    std::unordered_map<Key<Integer>, uint64_t> primary_key;

 private:
    std::vector<bool> deleted;
    std::vector<Integer> i_id;
    std::vector<Integer> i_im_id;
    std::vector<Varchar<24>> i_name;
    std::vector<Numeric<5, 2>> i_price;
    std::vector<Varchar<50>> i_data;
};

struct stockTuple {
    Integer s_i_id;
    Integer s_w_id;
    Numeric<4, 0> s_quantity;
    Char<24> s_dist_01;
    Char<24> s_dist_02;
    Char<24> s_dist_03;
    Char<24> s_dist_04;
    Char<24> s_dist_05;
    Char<24> s_dist_06;
    Char<24> s_dist_07;
    Char<24> s_dist_08;
    Char<24> s_dist_09;
    Char<24> s_dist_10;
    Numeric<8, 0> s_ytd;
    Numeric<4, 0> s_order_cnt;
    Numeric<4, 0> s_remote_cnt;
    Varchar<50> s_data;
};
class stockTable : public TableBase<stockTuple> {
 public:
    uint64_t insert_tuple(const stockTuple& tuple) override;
    std::optional<stockTuple> read_tuple(const uint64_t tid) override;
    void update_tuple(const uint64_t tid, const stockTuple& tuple) override;
    void delete_tuple(const uint64_t tid) override;

    std::unordered_map<Key<Integer, Integer>, uint64_t> primary_key;

 private:
    std::vector<bool> deleted;
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
};

}  // namespace tpcc
}  // namespace imlab
#endif  // INCLUDE_IMLAB_SCHEMA_H_
