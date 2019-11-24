
#include "imlab/schema.h"
#include "imlab/schemac/schema_parse_context.h"

namespace imlab {
namespace tpcc {

// ------------------------------------------------
// Generated sources for table warehouse
// ------------------------------------------------
const std::vector<IU> warehouseTable::ius {
    IU("warehouse", "w_id", imlab::schemac::Type::Integer()),
    IU("warehouse", "w_name", imlab::schemac::Type::Varchar(10)),
    IU("warehouse", "w_street_1", imlab::schemac::Type::Varchar(20)),
    IU("warehouse", "w_street_2", imlab::schemac::Type::Varchar(20)),
    IU("warehouse", "w_city", imlab::schemac::Type::Varchar(20)),
    IU("warehouse", "w_state", imlab::schemac::Type::Char(2)),
    IU("warehouse", "w_zip", imlab::schemac::Type::Char(9)),
    IU("warehouse", "w_tax", imlab::schemac::Type::Numeric(4, 4)),
    IU("warehouse", "w_ytd", imlab::schemac::Type::Numeric(12, 2)),
};

std::vector<const IU*> warehouseTable::CollectIUs() {
    std::vector<const IU*> iu_ptrs {};
    iu_ptrs.reserve(ius.size());
    for (auto& iu : ius) {
        iu_ptrs.push_back(&iu);
    }
    return iu_ptrs;
}

uint64_t warehouseTable::insert(
        const Integer w_id,
        const Varchar<10> w_name,
        const Varchar<20> w_street_1,
        const Varchar<20> w_street_2,
        const Varchar<20> w_city,
        const Char<2> w_state,
        const Char<9> w_zip,
        const Numeric<4, 4> w_tax,
        const Numeric<12, 2> w_ytd) {
    // search for free insert position
    size_t insert_pos = this->first_free_pos;
    for (; insert_pos < this->deleted_tuples.size(); insert_pos++) {
        if (this->deleted_tuples[insert_pos]) {
            break;
        }
    }
    // Now we know that the next free slot is definitely not earlier than the current position
    this->first_free_pos = insert_pos + 1;

    if (insert_pos == deleted_tuples.size()) {
        // insert new values at the end and increase vectors
        this->w_id.push_back(w_id);
        this->w_name.push_back(w_name);
        this->w_street_1.push_back(w_street_1);
        this->w_street_2.push_back(w_street_2);
        this->w_city.push_back(w_city);
        this->w_state.push_back(w_state);
        this->w_zip.push_back(w_zip);
        this->w_tax.push_back(w_tax);
        this->w_ytd.push_back(w_ytd);
        this->deleted_tuples.push_back(false);
    } else {
        // we found a free spot and can insert the new tuple there
        this->w_id[insert_pos] = w_id;
        this->w_name[insert_pos] = w_name;
        this->w_street_1[insert_pos] = w_street_1;
        this->w_street_2[insert_pos] = w_street_2;
        this->w_city[insert_pos] = w_city;
        this->w_state[insert_pos] = w_state;
        this->w_zip[insert_pos] = w_zip;
        this->w_tax[insert_pos] = w_tax;
        this->w_ytd[insert_pos] = w_ytd;
        this->deleted_tuples[insert_pos] = false;
    }

    this->primary_key[Key(w_id)] = insert_pos;

    this->size++;
    return insert_pos;
}

void warehouseTable::remove(const uint64_t tid) {
    // The table has a primary index we need to update.
    auto w_id_key = w_id[tid];

    this->primary_key.erase(Key(w_id_key));
    this->deleted_tuples[tid] = true;
    this->size--;

    // Update first_free_pos to speed up following inserts
    if (tid < this->first_free_pos) {
        this->first_free_pos = tid;
    }
}


std::optional<Integer> warehouseTable::get_w_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->w_id[tid];
}

std::optional<Varchar<10>> warehouseTable::get_w_name(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->w_name[tid];
}

std::optional<Varchar<20>> warehouseTable::get_w_street_1(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->w_street_1[tid];
}

std::optional<Varchar<20>> warehouseTable::get_w_street_2(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->w_street_2[tid];
}

std::optional<Varchar<20>> warehouseTable::get_w_city(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->w_city[tid];
}

std::optional<Char<2>> warehouseTable::get_w_state(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->w_state[tid];
}

std::optional<Char<9>> warehouseTable::get_w_zip(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->w_zip[tid];
}

std::optional<Numeric<4, 4>> warehouseTable::get_w_tax(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->w_tax[tid];
}

std::optional<Numeric<12, 2>> warehouseTable::get_w_ytd(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->w_ytd[tid];
}


void warehouseTable::update_w_id(const uint64_t tid, const Integer w_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto w_id_key = this->w_id[tid];

    this->primary_key.erase(Key(w_id_key));

    // Update value
    this->w_id[tid] = w_id;
    w_id_key = w_id;

    // update index on primary key
    this->primary_key[Key(w_id_key)] = tid;
}

void warehouseTable::update_w_name(const uint64_t tid, const Varchar<10> w_name) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->w_name[tid] = w_name;
}

void warehouseTable::update_w_street_1(const uint64_t tid, const Varchar<20> w_street_1) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->w_street_1[tid] = w_street_1;
}

void warehouseTable::update_w_street_2(const uint64_t tid, const Varchar<20> w_street_2) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->w_street_2[tid] = w_street_2;
}

void warehouseTable::update_w_city(const uint64_t tid, const Varchar<20> w_city) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->w_city[tid] = w_city;
}

void warehouseTable::update_w_state(const uint64_t tid, const Char<2> w_state) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->w_state[tid] = w_state;
}

void warehouseTable::update_w_zip(const uint64_t tid, const Char<9> w_zip) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->w_zip[tid] = w_zip;
}

void warehouseTable::update_w_tax(const uint64_t tid, const Numeric<4, 4> w_tax) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->w_tax[tid] = w_tax;
}

void warehouseTable::update_w_ytd(const uint64_t tid, const Numeric<12, 2> w_ytd) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->w_ytd[tid] = w_ytd;
}



// ------------------------------------------------
// Generated sources for table district
// ------------------------------------------------
const std::vector<IU> districtTable::ius {
    IU("district", "d_id", imlab::schemac::Type::Integer()),
    IU("district", "d_w_id", imlab::schemac::Type::Integer()),
    IU("district", "d_name", imlab::schemac::Type::Varchar(10)),
    IU("district", "d_street_1", imlab::schemac::Type::Varchar(20)),
    IU("district", "d_street_2", imlab::schemac::Type::Varchar(20)),
    IU("district", "d_city", imlab::schemac::Type::Varchar(20)),
    IU("district", "d_state", imlab::schemac::Type::Char(2)),
    IU("district", "d_zip", imlab::schemac::Type::Char(9)),
    IU("district", "d_tax", imlab::schemac::Type::Numeric(4, 4)),
    IU("district", "d_ytd", imlab::schemac::Type::Numeric(12, 2)),
    IU("district", "d_next_o_id", imlab::schemac::Type::Integer()),
};

std::vector<const IU*> districtTable::CollectIUs() {
    std::vector<const IU*> iu_ptrs {};
    iu_ptrs.reserve(ius.size());
    for (auto& iu : ius) {
        iu_ptrs.push_back(&iu);
    }
    return iu_ptrs;
}

uint64_t districtTable::insert(
        const Integer d_id,
        const Integer d_w_id,
        const Varchar<10> d_name,
        const Varchar<20> d_street_1,
        const Varchar<20> d_street_2,
        const Varchar<20> d_city,
        const Char<2> d_state,
        const Char<9> d_zip,
        const Numeric<4, 4> d_tax,
        const Numeric<12, 2> d_ytd,
        const Integer d_next_o_id) {
    // search for free insert position
    size_t insert_pos = this->first_free_pos;
    for (; insert_pos < this->deleted_tuples.size(); insert_pos++) {
        if (this->deleted_tuples[insert_pos]) {
            break;
        }
    }
    // Now we know that the next free slot is definitely not earlier than the current position
    this->first_free_pos = insert_pos + 1;

    if (insert_pos == deleted_tuples.size()) {
        // insert new values at the end and increase vectors
        this->d_id.push_back(d_id);
        this->d_w_id.push_back(d_w_id);
        this->d_name.push_back(d_name);
        this->d_street_1.push_back(d_street_1);
        this->d_street_2.push_back(d_street_2);
        this->d_city.push_back(d_city);
        this->d_state.push_back(d_state);
        this->d_zip.push_back(d_zip);
        this->d_tax.push_back(d_tax);
        this->d_ytd.push_back(d_ytd);
        this->d_next_o_id.push_back(d_next_o_id);
        this->deleted_tuples.push_back(false);
    } else {
        // we found a free spot and can insert the new tuple there
        this->d_id[insert_pos] = d_id;
        this->d_w_id[insert_pos] = d_w_id;
        this->d_name[insert_pos] = d_name;
        this->d_street_1[insert_pos] = d_street_1;
        this->d_street_2[insert_pos] = d_street_2;
        this->d_city[insert_pos] = d_city;
        this->d_state[insert_pos] = d_state;
        this->d_zip[insert_pos] = d_zip;
        this->d_tax[insert_pos] = d_tax;
        this->d_ytd[insert_pos] = d_ytd;
        this->d_next_o_id[insert_pos] = d_next_o_id;
        this->deleted_tuples[insert_pos] = false;
    }

    this->primary_key[Key(d_w_id, d_id)] = insert_pos;

    this->size++;
    return insert_pos;
}

void districtTable::remove(const uint64_t tid) {
    // The table has a primary index we need to update.
    auto d_w_id_key = d_w_id[tid];
    auto d_id_key = d_id[tid];

    this->primary_key.erase(Key(d_w_id_key, d_id_key));
    this->deleted_tuples[tid] = true;
    this->size--;

    // Update first_free_pos to speed up following inserts
    if (tid < this->first_free_pos) {
        this->first_free_pos = tid;
    }
}


std::optional<Integer> districtTable::get_d_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->d_id[tid];
}

std::optional<Integer> districtTable::get_d_w_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->d_w_id[tid];
}

std::optional<Varchar<10>> districtTable::get_d_name(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->d_name[tid];
}

std::optional<Varchar<20>> districtTable::get_d_street_1(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->d_street_1[tid];
}

std::optional<Varchar<20>> districtTable::get_d_street_2(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->d_street_2[tid];
}

std::optional<Varchar<20>> districtTable::get_d_city(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->d_city[tid];
}

std::optional<Char<2>> districtTable::get_d_state(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->d_state[tid];
}

std::optional<Char<9>> districtTable::get_d_zip(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->d_zip[tid];
}

std::optional<Numeric<4, 4>> districtTable::get_d_tax(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->d_tax[tid];
}

std::optional<Numeric<12, 2>> districtTable::get_d_ytd(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->d_ytd[tid];
}

std::optional<Integer> districtTable::get_d_next_o_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->d_next_o_id[tid];
}


void districtTable::update_d_id(const uint64_t tid, const Integer d_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto d_w_id_key = this->d_w_id[tid];
    auto d_id_key = this->d_id[tid];

    this->primary_key.erase(Key(d_w_id_key, d_id_key));

    // Update value
    this->d_id[tid] = d_id;
    d_id_key = d_id;

    // update index on primary key
    this->primary_key[Key(d_w_id_key, d_id_key)] = tid;
}

void districtTable::update_d_w_id(const uint64_t tid, const Integer d_w_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto d_w_id_key = this->d_w_id[tid];
    auto d_id_key = this->d_id[tid];

    this->primary_key.erase(Key(d_w_id_key, d_id_key));

    // Update value
    this->d_w_id[tid] = d_w_id;
    d_w_id_key = d_w_id;

    // update index on primary key
    this->primary_key[Key(d_w_id_key, d_id_key)] = tid;
}

void districtTable::update_d_name(const uint64_t tid, const Varchar<10> d_name) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->d_name[tid] = d_name;
}

void districtTable::update_d_street_1(const uint64_t tid, const Varchar<20> d_street_1) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->d_street_1[tid] = d_street_1;
}

void districtTable::update_d_street_2(const uint64_t tid, const Varchar<20> d_street_2) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->d_street_2[tid] = d_street_2;
}

void districtTable::update_d_city(const uint64_t tid, const Varchar<20> d_city) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->d_city[tid] = d_city;
}

void districtTable::update_d_state(const uint64_t tid, const Char<2> d_state) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->d_state[tid] = d_state;
}

void districtTable::update_d_zip(const uint64_t tid, const Char<9> d_zip) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->d_zip[tid] = d_zip;
}

void districtTable::update_d_tax(const uint64_t tid, const Numeric<4, 4> d_tax) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->d_tax[tid] = d_tax;
}

void districtTable::update_d_ytd(const uint64_t tid, const Numeric<12, 2> d_ytd) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->d_ytd[tid] = d_ytd;
}

void districtTable::update_d_next_o_id(const uint64_t tid, const Integer d_next_o_id) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->d_next_o_id[tid] = d_next_o_id;
}



// ------------------------------------------------
// Generated sources for table customer
// ------------------------------------------------
const std::vector<IU> customerTable::ius {
    IU("customer", "c_id", imlab::schemac::Type::Integer()),
    IU("customer", "c_d_id", imlab::schemac::Type::Integer()),
    IU("customer", "c_w_id", imlab::schemac::Type::Integer()),
    IU("customer", "c_first", imlab::schemac::Type::Varchar(16)),
    IU("customer", "c_middle", imlab::schemac::Type::Char(2)),
    IU("customer", "c_last", imlab::schemac::Type::Varchar(16)),
    IU("customer", "c_street_1", imlab::schemac::Type::Varchar(20)),
    IU("customer", "c_street_2", imlab::schemac::Type::Varchar(20)),
    IU("customer", "c_city", imlab::schemac::Type::Varchar(20)),
    IU("customer", "c_state", imlab::schemac::Type::Char(2)),
    IU("customer", "c_zip", imlab::schemac::Type::Char(9)),
    IU("customer", "c_phone", imlab::schemac::Type::Char(16)),
    IU("customer", "c_since", imlab::schemac::Type::Timestamp()),
    IU("customer", "c_credit", imlab::schemac::Type::Char(2)),
    IU("customer", "c_credit_lim", imlab::schemac::Type::Numeric(12, 2)),
    IU("customer", "c_discount", imlab::schemac::Type::Numeric(4, 4)),
    IU("customer", "c_balance", imlab::schemac::Type::Numeric(12, 2)),
    IU("customer", "c_ytd_paymenr", imlab::schemac::Type::Numeric(12, 2)),
    IU("customer", "c_payment_cnt", imlab::schemac::Type::Numeric(4, 0)),
    IU("customer", "c_delivery_cnt", imlab::schemac::Type::Numeric(4, 0)),
    IU("customer", "c_data", imlab::schemac::Type::Varchar(500)),
};

std::vector<const IU*> customerTable::CollectIUs() {
    std::vector<const IU*> iu_ptrs {};
    iu_ptrs.reserve(ius.size());
    for (auto& iu : ius) {
        iu_ptrs.push_back(&iu);
    }
    return iu_ptrs;
}

uint64_t customerTable::insert(
        const Integer c_id,
        const Integer c_d_id,
        const Integer c_w_id,
        const Varchar<16> c_first,
        const Char<2> c_middle,
        const Varchar<16> c_last,
        const Varchar<20> c_street_1,
        const Varchar<20> c_street_2,
        const Varchar<20> c_city,
        const Char<2> c_state,
        const Char<9> c_zip,
        const Char<16> c_phone,
        const Timestamp c_since,
        const Char<2> c_credit,
        const Numeric<12, 2> c_credit_lim,
        const Numeric<4, 4> c_discount,
        const Numeric<12, 2> c_balance,
        const Numeric<12, 2> c_ytd_paymenr,
        const Numeric<4, 0> c_payment_cnt,
        const Numeric<4, 0> c_delivery_cnt,
        const Varchar<500> c_data) {
    // search for free insert position
    size_t insert_pos = this->first_free_pos;
    for (; insert_pos < this->deleted_tuples.size(); insert_pos++) {
        if (this->deleted_tuples[insert_pos]) {
            break;
        }
    }
    // Now we know that the next free slot is definitely not earlier than the current position
    this->first_free_pos = insert_pos + 1;

    if (insert_pos == deleted_tuples.size()) {
        // insert new values at the end and increase vectors
        this->c_id.push_back(c_id);
        this->c_d_id.push_back(c_d_id);
        this->c_w_id.push_back(c_w_id);
        this->c_first.push_back(c_first);
        this->c_middle.push_back(c_middle);
        this->c_last.push_back(c_last);
        this->c_street_1.push_back(c_street_1);
        this->c_street_2.push_back(c_street_2);
        this->c_city.push_back(c_city);
        this->c_state.push_back(c_state);
        this->c_zip.push_back(c_zip);
        this->c_phone.push_back(c_phone);
        this->c_since.push_back(c_since);
        this->c_credit.push_back(c_credit);
        this->c_credit_lim.push_back(c_credit_lim);
        this->c_discount.push_back(c_discount);
        this->c_balance.push_back(c_balance);
        this->c_ytd_paymenr.push_back(c_ytd_paymenr);
        this->c_payment_cnt.push_back(c_payment_cnt);
        this->c_delivery_cnt.push_back(c_delivery_cnt);
        this->c_data.push_back(c_data);
        this->deleted_tuples.push_back(false);
    } else {
        // we found a free spot and can insert the new tuple there
        this->c_id[insert_pos] = c_id;
        this->c_d_id[insert_pos] = c_d_id;
        this->c_w_id[insert_pos] = c_w_id;
        this->c_first[insert_pos] = c_first;
        this->c_middle[insert_pos] = c_middle;
        this->c_last[insert_pos] = c_last;
        this->c_street_1[insert_pos] = c_street_1;
        this->c_street_2[insert_pos] = c_street_2;
        this->c_city[insert_pos] = c_city;
        this->c_state[insert_pos] = c_state;
        this->c_zip[insert_pos] = c_zip;
        this->c_phone[insert_pos] = c_phone;
        this->c_since[insert_pos] = c_since;
        this->c_credit[insert_pos] = c_credit;
        this->c_credit_lim[insert_pos] = c_credit_lim;
        this->c_discount[insert_pos] = c_discount;
        this->c_balance[insert_pos] = c_balance;
        this->c_ytd_paymenr[insert_pos] = c_ytd_paymenr;
        this->c_payment_cnt[insert_pos] = c_payment_cnt;
        this->c_delivery_cnt[insert_pos] = c_delivery_cnt;
        this->c_data[insert_pos] = c_data;
        this->deleted_tuples[insert_pos] = false;
    }

    this->primary_key[Key(c_w_id, c_d_id, c_id)] = insert_pos;

    this->size++;
    return insert_pos;
}

void customerTable::remove(const uint64_t tid) {
    // The table has a primary index we need to update.
    auto c_w_id_key = c_w_id[tid];
    auto c_d_id_key = c_d_id[tid];
    auto c_id_key = c_id[tid];

    this->primary_key.erase(Key(c_w_id_key, c_d_id_key, c_id_key));
    this->deleted_tuples[tid] = true;
    this->size--;

    // Update first_free_pos to speed up following inserts
    if (tid < this->first_free_pos) {
        this->first_free_pos = tid;
    }
}


std::optional<Integer> customerTable::get_c_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_id[tid];
}

std::optional<Integer> customerTable::get_c_d_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_d_id[tid];
}

std::optional<Integer> customerTable::get_c_w_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_w_id[tid];
}

std::optional<Varchar<16>> customerTable::get_c_first(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_first[tid];
}

std::optional<Char<2>> customerTable::get_c_middle(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_middle[tid];
}

std::optional<Varchar<16>> customerTable::get_c_last(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_last[tid];
}

std::optional<Varchar<20>> customerTable::get_c_street_1(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_street_1[tid];
}

std::optional<Varchar<20>> customerTable::get_c_street_2(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_street_2[tid];
}

std::optional<Varchar<20>> customerTable::get_c_city(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_city[tid];
}

std::optional<Char<2>> customerTable::get_c_state(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_state[tid];
}

std::optional<Char<9>> customerTable::get_c_zip(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_zip[tid];
}

std::optional<Char<16>> customerTable::get_c_phone(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_phone[tid];
}

std::optional<Timestamp> customerTable::get_c_since(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_since[tid];
}

std::optional<Char<2>> customerTable::get_c_credit(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_credit[tid];
}

std::optional<Numeric<12, 2>> customerTable::get_c_credit_lim(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_credit_lim[tid];
}

std::optional<Numeric<4, 4>> customerTable::get_c_discount(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_discount[tid];
}

std::optional<Numeric<12, 2>> customerTable::get_c_balance(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_balance[tid];
}

std::optional<Numeric<12, 2>> customerTable::get_c_ytd_paymenr(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_ytd_paymenr[tid];
}

std::optional<Numeric<4, 0>> customerTable::get_c_payment_cnt(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_payment_cnt[tid];
}

std::optional<Numeric<4, 0>> customerTable::get_c_delivery_cnt(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_delivery_cnt[tid];
}

std::optional<Varchar<500>> customerTable::get_c_data(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->c_data[tid];
}


void customerTable::update_c_id(const uint64_t tid, const Integer c_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto c_w_id_key = this->c_w_id[tid];
    auto c_d_id_key = this->c_d_id[tid];
    auto c_id_key = this->c_id[tid];

    this->primary_key.erase(Key(c_w_id_key, c_d_id_key, c_id_key));

    // Update value
    this->c_id[tid] = c_id;
    c_id_key = c_id;

    // update index on primary key
    this->primary_key[Key(c_w_id_key, c_d_id_key, c_id_key)] = tid;
}

void customerTable::update_c_d_id(const uint64_t tid, const Integer c_d_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto c_w_id_key = this->c_w_id[tid];
    auto c_d_id_key = this->c_d_id[tid];
    auto c_id_key = this->c_id[tid];

    this->primary_key.erase(Key(c_w_id_key, c_d_id_key, c_id_key));

    // Update value
    this->c_d_id[tid] = c_d_id;
    c_d_id_key = c_d_id;

    // update index on primary key
    this->primary_key[Key(c_w_id_key, c_d_id_key, c_id_key)] = tid;
}

void customerTable::update_c_w_id(const uint64_t tid, const Integer c_w_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto c_w_id_key = this->c_w_id[tid];
    auto c_d_id_key = this->c_d_id[tid];
    auto c_id_key = this->c_id[tid];

    this->primary_key.erase(Key(c_w_id_key, c_d_id_key, c_id_key));

    // Update value
    this->c_w_id[tid] = c_w_id;
    c_w_id_key = c_w_id;

    // update index on primary key
    this->primary_key[Key(c_w_id_key, c_d_id_key, c_id_key)] = tid;
}

void customerTable::update_c_first(const uint64_t tid, const Varchar<16> c_first) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_first[tid] = c_first;
}

void customerTable::update_c_middle(const uint64_t tid, const Char<2> c_middle) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_middle[tid] = c_middle;
}

void customerTable::update_c_last(const uint64_t tid, const Varchar<16> c_last) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_last[tid] = c_last;
}

void customerTable::update_c_street_1(const uint64_t tid, const Varchar<20> c_street_1) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_street_1[tid] = c_street_1;
}

void customerTable::update_c_street_2(const uint64_t tid, const Varchar<20> c_street_2) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_street_2[tid] = c_street_2;
}

void customerTable::update_c_city(const uint64_t tid, const Varchar<20> c_city) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_city[tid] = c_city;
}

void customerTable::update_c_state(const uint64_t tid, const Char<2> c_state) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_state[tid] = c_state;
}

void customerTable::update_c_zip(const uint64_t tid, const Char<9> c_zip) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_zip[tid] = c_zip;
}

void customerTable::update_c_phone(const uint64_t tid, const Char<16> c_phone) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_phone[tid] = c_phone;
}

void customerTable::update_c_since(const uint64_t tid, const Timestamp c_since) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_since[tid] = c_since;
}

void customerTable::update_c_credit(const uint64_t tid, const Char<2> c_credit) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_credit[tid] = c_credit;
}

void customerTable::update_c_credit_lim(const uint64_t tid, const Numeric<12, 2> c_credit_lim) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_credit_lim[tid] = c_credit_lim;
}

void customerTable::update_c_discount(const uint64_t tid, const Numeric<4, 4> c_discount) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_discount[tid] = c_discount;
}

void customerTable::update_c_balance(const uint64_t tid, const Numeric<12, 2> c_balance) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_balance[tid] = c_balance;
}

void customerTable::update_c_ytd_paymenr(const uint64_t tid, const Numeric<12, 2> c_ytd_paymenr) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_ytd_paymenr[tid] = c_ytd_paymenr;
}

void customerTable::update_c_payment_cnt(const uint64_t tid, const Numeric<4, 0> c_payment_cnt) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_payment_cnt[tid] = c_payment_cnt;
}

void customerTable::update_c_delivery_cnt(const uint64_t tid, const Numeric<4, 0> c_delivery_cnt) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_delivery_cnt[tid] = c_delivery_cnt;
}

void customerTable::update_c_data(const uint64_t tid, const Varchar<500> c_data) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->c_data[tid] = c_data;
}



// ------------------------------------------------
// Generated sources for table history
// ------------------------------------------------
const std::vector<IU> historyTable::ius {
    IU("history", "h_c_id", imlab::schemac::Type::Integer()),
    IU("history", "h_c_d_id", imlab::schemac::Type::Integer()),
    IU("history", "h_c_w_id", imlab::schemac::Type::Integer()),
    IU("history", "h_d_id", imlab::schemac::Type::Integer()),
    IU("history", "h_w_id", imlab::schemac::Type::Integer()),
    IU("history", "h_date", imlab::schemac::Type::Timestamp()),
    IU("history", "h_amount", imlab::schemac::Type::Numeric(6, 2)),
    IU("history", "h_data", imlab::schemac::Type::Varchar(24)),
};

std::vector<const IU*> historyTable::CollectIUs() {
    std::vector<const IU*> iu_ptrs {};
    iu_ptrs.reserve(ius.size());
    for (auto& iu : ius) {
        iu_ptrs.push_back(&iu);
    }
    return iu_ptrs;
}

uint64_t historyTable::insert(
        const Integer h_c_id,
        const Integer h_c_d_id,
        const Integer h_c_w_id,
        const Integer h_d_id,
        const Integer h_w_id,
        const Timestamp h_date,
        const Numeric<6, 2> h_amount,
        const Varchar<24> h_data) {
    // search for free insert position
    size_t insert_pos = this->first_free_pos;
    for (; insert_pos < this->deleted_tuples.size(); insert_pos++) {
        if (this->deleted_tuples[insert_pos]) {
            break;
        }
    }
    // Now we know that the next free slot is definitely not earlier than the current position
    this->first_free_pos = insert_pos + 1;

    if (insert_pos == deleted_tuples.size()) {
        // insert new values at the end and increase vectors
        this->h_c_id.push_back(h_c_id);
        this->h_c_d_id.push_back(h_c_d_id);
        this->h_c_w_id.push_back(h_c_w_id);
        this->h_d_id.push_back(h_d_id);
        this->h_w_id.push_back(h_w_id);
        this->h_date.push_back(h_date);
        this->h_amount.push_back(h_amount);
        this->h_data.push_back(h_data);
        this->deleted_tuples.push_back(false);
    } else {
        // we found a free spot and can insert the new tuple there
        this->h_c_id[insert_pos] = h_c_id;
        this->h_c_d_id[insert_pos] = h_c_d_id;
        this->h_c_w_id[insert_pos] = h_c_w_id;
        this->h_d_id[insert_pos] = h_d_id;
        this->h_w_id[insert_pos] = h_w_id;
        this->h_date[insert_pos] = h_date;
        this->h_amount[insert_pos] = h_amount;
        this->h_data[insert_pos] = h_data;
        this->deleted_tuples[insert_pos] = false;
    }

    this->size++;
    return insert_pos;
}

void historyTable::remove(const uint64_t tid) {
    this->deleted_tuples[tid] = true;
    this->size--;

    // Update first_free_pos to speed up following inserts
    if (tid < this->first_free_pos) {
        this->first_free_pos = tid;
    }
}


std::optional<Integer> historyTable::get_h_c_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->h_c_id[tid];
}

std::optional<Integer> historyTable::get_h_c_d_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->h_c_d_id[tid];
}

std::optional<Integer> historyTable::get_h_c_w_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->h_c_w_id[tid];
}

std::optional<Integer> historyTable::get_h_d_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->h_d_id[tid];
}

std::optional<Integer> historyTable::get_h_w_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->h_w_id[tid];
}

std::optional<Timestamp> historyTable::get_h_date(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->h_date[tid];
}

std::optional<Numeric<6, 2>> historyTable::get_h_amount(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->h_amount[tid];
}

std::optional<Varchar<24>> historyTable::get_h_data(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->h_data[tid];
}


void historyTable::update_h_c_id(const uint64_t tid, const Integer h_c_id) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->h_c_id[tid] = h_c_id;
}

void historyTable::update_h_c_d_id(const uint64_t tid, const Integer h_c_d_id) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->h_c_d_id[tid] = h_c_d_id;
}

void historyTable::update_h_c_w_id(const uint64_t tid, const Integer h_c_w_id) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->h_c_w_id[tid] = h_c_w_id;
}

void historyTable::update_h_d_id(const uint64_t tid, const Integer h_d_id) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->h_d_id[tid] = h_d_id;
}

void historyTable::update_h_w_id(const uint64_t tid, const Integer h_w_id) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->h_w_id[tid] = h_w_id;
}

void historyTable::update_h_date(const uint64_t tid, const Timestamp h_date) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->h_date[tid] = h_date;
}

void historyTable::update_h_amount(const uint64_t tid, const Numeric<6, 2> h_amount) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->h_amount[tid] = h_amount;
}

void historyTable::update_h_data(const uint64_t tid, const Varchar<24> h_data) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->h_data[tid] = h_data;
}



// ------------------------------------------------
// Generated sources for table neworder
// ------------------------------------------------
const std::vector<IU> neworderTable::ius {
    IU("neworder", "no_o_id", imlab::schemac::Type::Integer()),
    IU("neworder", "no_d_id", imlab::schemac::Type::Integer()),
    IU("neworder", "no_w_id", imlab::schemac::Type::Integer()),
};

std::vector<const IU*> neworderTable::CollectIUs() {
    std::vector<const IU*> iu_ptrs {};
    iu_ptrs.reserve(ius.size());
    for (auto& iu : ius) {
        iu_ptrs.push_back(&iu);
    }
    return iu_ptrs;
}

uint64_t neworderTable::insert(
        const Integer no_o_id,
        const Integer no_d_id,
        const Integer no_w_id) {
    // search for free insert position
    size_t insert_pos = this->first_free_pos;
    for (; insert_pos < this->deleted_tuples.size(); insert_pos++) {
        if (this->deleted_tuples[insert_pos]) {
            break;
        }
    }
    // Now we know that the next free slot is definitely not earlier than the current position
    this->first_free_pos = insert_pos + 1;

    if (insert_pos == deleted_tuples.size()) {
        // insert new values at the end and increase vectors
        this->no_o_id.push_back(no_o_id);
        this->no_d_id.push_back(no_d_id);
        this->no_w_id.push_back(no_w_id);
        this->deleted_tuples.push_back(false);
    } else {
        // we found a free spot and can insert the new tuple there
        this->no_o_id[insert_pos] = no_o_id;
        this->no_d_id[insert_pos] = no_d_id;
        this->no_w_id[insert_pos] = no_w_id;
        this->deleted_tuples[insert_pos] = false;
    }

    this->primary_key[Key(no_w_id, no_d_id, no_o_id)] = insert_pos;

    this->size++;
    return insert_pos;
}

void neworderTable::remove(const uint64_t tid) {
    // The table has a primary index we need to update.
    auto no_w_id_key = no_w_id[tid];
    auto no_d_id_key = no_d_id[tid];
    auto no_o_id_key = no_o_id[tid];

    this->primary_key.erase(Key(no_w_id_key, no_d_id_key, no_o_id_key));
    this->deleted_tuples[tid] = true;
    this->size--;

    // Update first_free_pos to speed up following inserts
    if (tid < this->first_free_pos) {
        this->first_free_pos = tid;
    }
}


std::optional<Integer> neworderTable::get_no_o_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->no_o_id[tid];
}

std::optional<Integer> neworderTable::get_no_d_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->no_d_id[tid];
}

std::optional<Integer> neworderTable::get_no_w_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->no_w_id[tid];
}


void neworderTable::update_no_o_id(const uint64_t tid, const Integer no_o_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto no_w_id_key = this->no_w_id[tid];
    auto no_d_id_key = this->no_d_id[tid];
    auto no_o_id_key = this->no_o_id[tid];

    this->primary_key.erase(Key(no_w_id_key, no_d_id_key, no_o_id_key));

    // Update value
    this->no_o_id[tid] = no_o_id;
    no_o_id_key = no_o_id;

    // update index on primary key
    this->primary_key[Key(no_w_id_key, no_d_id_key, no_o_id_key)] = tid;
}

void neworderTable::update_no_d_id(const uint64_t tid, const Integer no_d_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto no_w_id_key = this->no_w_id[tid];
    auto no_d_id_key = this->no_d_id[tid];
    auto no_o_id_key = this->no_o_id[tid];

    this->primary_key.erase(Key(no_w_id_key, no_d_id_key, no_o_id_key));

    // Update value
    this->no_d_id[tid] = no_d_id;
    no_d_id_key = no_d_id;

    // update index on primary key
    this->primary_key[Key(no_w_id_key, no_d_id_key, no_o_id_key)] = tid;
}

void neworderTable::update_no_w_id(const uint64_t tid, const Integer no_w_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto no_w_id_key = this->no_w_id[tid];
    auto no_d_id_key = this->no_d_id[tid];
    auto no_o_id_key = this->no_o_id[tid];

    this->primary_key.erase(Key(no_w_id_key, no_d_id_key, no_o_id_key));

    // Update value
    this->no_w_id[tid] = no_w_id;
    no_w_id_key = no_w_id;

    // update index on primary key
    this->primary_key[Key(no_w_id_key, no_d_id_key, no_o_id_key)] = tid;
}



// ------------------------------------------------
// Generated sources for table order
// ------------------------------------------------
const std::vector<IU> orderTable::ius {
    IU("order", "o_id", imlab::schemac::Type::Integer()),
    IU("order", "o_d_id", imlab::schemac::Type::Integer()),
    IU("order", "o_w_id", imlab::schemac::Type::Integer()),
    IU("order", "o_c_id", imlab::schemac::Type::Integer()),
    IU("order", "o_entry_d", imlab::schemac::Type::Timestamp()),
    IU("order", "o_carrier_id", imlab::schemac::Type::Integer()),
    IU("order", "o_ol_cnt", imlab::schemac::Type::Numeric(2, 0)),
    IU("order", "o_all_local", imlab::schemac::Type::Numeric(1, 0)),
};

std::vector<const IU*> orderTable::CollectIUs() {
    std::vector<const IU*> iu_ptrs {};
    iu_ptrs.reserve(ius.size());
    for (auto& iu : ius) {
        iu_ptrs.push_back(&iu);
    }
    return iu_ptrs;
}

uint64_t orderTable::insert(
        const Integer o_id,
        const Integer o_d_id,
        const Integer o_w_id,
        const Integer o_c_id,
        const Timestamp o_entry_d,
        const Integer o_carrier_id,
        const Numeric<2, 0> o_ol_cnt,
        const Numeric<1, 0> o_all_local) {
    // search for free insert position
    size_t insert_pos = this->first_free_pos;
    for (; insert_pos < this->deleted_tuples.size(); insert_pos++) {
        if (this->deleted_tuples[insert_pos]) {
            break;
        }
    }
    // Now we know that the next free slot is definitely not earlier than the current position
    this->first_free_pos = insert_pos + 1;

    if (insert_pos == deleted_tuples.size()) {
        // insert new values at the end and increase vectors
        this->o_id.push_back(o_id);
        this->o_d_id.push_back(o_d_id);
        this->o_w_id.push_back(o_w_id);
        this->o_c_id.push_back(o_c_id);
        this->o_entry_d.push_back(o_entry_d);
        this->o_carrier_id.push_back(o_carrier_id);
        this->o_ol_cnt.push_back(o_ol_cnt);
        this->o_all_local.push_back(o_all_local);
        this->deleted_tuples.push_back(false);
    } else {
        // we found a free spot and can insert the new tuple there
        this->o_id[insert_pos] = o_id;
        this->o_d_id[insert_pos] = o_d_id;
        this->o_w_id[insert_pos] = o_w_id;
        this->o_c_id[insert_pos] = o_c_id;
        this->o_entry_d[insert_pos] = o_entry_d;
        this->o_carrier_id[insert_pos] = o_carrier_id;
        this->o_ol_cnt[insert_pos] = o_ol_cnt;
        this->o_all_local[insert_pos] = o_all_local;
        this->deleted_tuples[insert_pos] = false;
    }

    this->primary_key[Key(o_w_id, o_d_id, o_id)] = insert_pos;

    this->size++;
    return insert_pos;
}

void orderTable::remove(const uint64_t tid) {
    // The table has a primary index we need to update.
    auto o_w_id_key = o_w_id[tid];
    auto o_d_id_key = o_d_id[tid];
    auto o_id_key = o_id[tid];

    this->primary_key.erase(Key(o_w_id_key, o_d_id_key, o_id_key));
    this->deleted_tuples[tid] = true;
    this->size--;

    // Update first_free_pos to speed up following inserts
    if (tid < this->first_free_pos) {
        this->first_free_pos = tid;
    }
}


std::optional<Integer> orderTable::get_o_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->o_id[tid];
}

std::optional<Integer> orderTable::get_o_d_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->o_d_id[tid];
}

std::optional<Integer> orderTable::get_o_w_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->o_w_id[tid];
}

std::optional<Integer> orderTable::get_o_c_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->o_c_id[tid];
}

std::optional<Timestamp> orderTable::get_o_entry_d(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->o_entry_d[tid];
}

std::optional<Integer> orderTable::get_o_carrier_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->o_carrier_id[tid];
}

std::optional<Numeric<2, 0>> orderTable::get_o_ol_cnt(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->o_ol_cnt[tid];
}

std::optional<Numeric<1, 0>> orderTable::get_o_all_local(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->o_all_local[tid];
}


void orderTable::update_o_id(const uint64_t tid, const Integer o_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto o_w_id_key = this->o_w_id[tid];
    auto o_d_id_key = this->o_d_id[tid];
    auto o_id_key = this->o_id[tid];

    this->primary_key.erase(Key(o_w_id_key, o_d_id_key, o_id_key));

    // Update value
    this->o_id[tid] = o_id;
    o_id_key = o_id;

    // update index on primary key
    this->primary_key[Key(o_w_id_key, o_d_id_key, o_id_key)] = tid;
}

void orderTable::update_o_d_id(const uint64_t tid, const Integer o_d_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto o_w_id_key = this->o_w_id[tid];
    auto o_d_id_key = this->o_d_id[tid];
    auto o_id_key = this->o_id[tid];

    this->primary_key.erase(Key(o_w_id_key, o_d_id_key, o_id_key));

    // Update value
    this->o_d_id[tid] = o_d_id;
    o_d_id_key = o_d_id;

    // update index on primary key
    this->primary_key[Key(o_w_id_key, o_d_id_key, o_id_key)] = tid;
}

void orderTable::update_o_w_id(const uint64_t tid, const Integer o_w_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto o_w_id_key = this->o_w_id[tid];
    auto o_d_id_key = this->o_d_id[tid];
    auto o_id_key = this->o_id[tid];

    this->primary_key.erase(Key(o_w_id_key, o_d_id_key, o_id_key));

    // Update value
    this->o_w_id[tid] = o_w_id;
    o_w_id_key = o_w_id;

    // update index on primary key
    this->primary_key[Key(o_w_id_key, o_d_id_key, o_id_key)] = tid;
}

void orderTable::update_o_c_id(const uint64_t tid, const Integer o_c_id) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->o_c_id[tid] = o_c_id;
}

void orderTable::update_o_entry_d(const uint64_t tid, const Timestamp o_entry_d) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->o_entry_d[tid] = o_entry_d;
}

void orderTable::update_o_carrier_id(const uint64_t tid, const Integer o_carrier_id) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->o_carrier_id[tid] = o_carrier_id;
}

void orderTable::update_o_ol_cnt(const uint64_t tid, const Numeric<2, 0> o_ol_cnt) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->o_ol_cnt[tid] = o_ol_cnt;
}

void orderTable::update_o_all_local(const uint64_t tid, const Numeric<1, 0> o_all_local) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->o_all_local[tid] = o_all_local;
}



// ------------------------------------------------
// Generated sources for table orderline
// ------------------------------------------------
const std::vector<IU> orderlineTable::ius {
    IU("orderline", "ol_o_id", imlab::schemac::Type::Integer()),
    IU("orderline", "ol_d_id", imlab::schemac::Type::Integer()),
    IU("orderline", "ol_w_id", imlab::schemac::Type::Integer()),
    IU("orderline", "ol_number", imlab::schemac::Type::Integer()),
    IU("orderline", "ol_i_id", imlab::schemac::Type::Integer()),
    IU("orderline", "ol_supply_w_id", imlab::schemac::Type::Integer()),
    IU("orderline", "ol_delivery_d", imlab::schemac::Type::Timestamp()),
    IU("orderline", "ol_quantity", imlab::schemac::Type::Numeric(2, 0)),
    IU("orderline", "ol_amount", imlab::schemac::Type::Numeric(6, 2)),
    IU("orderline", "ol_dist_info", imlab::schemac::Type::Char(24)),
};

std::vector<const IU*> orderlineTable::CollectIUs() {
    std::vector<const IU*> iu_ptrs {};
    iu_ptrs.reserve(ius.size());
    for (auto& iu : ius) {
        iu_ptrs.push_back(&iu);
    }
    return iu_ptrs;
}

uint64_t orderlineTable::insert(
        const Integer ol_o_id,
        const Integer ol_d_id,
        const Integer ol_w_id,
        const Integer ol_number,
        const Integer ol_i_id,
        const Integer ol_supply_w_id,
        const Timestamp ol_delivery_d,
        const Numeric<2, 0> ol_quantity,
        const Numeric<6, 2> ol_amount,
        const Char<24> ol_dist_info) {
    // search for free insert position
    size_t insert_pos = this->first_free_pos;
    for (; insert_pos < this->deleted_tuples.size(); insert_pos++) {
        if (this->deleted_tuples[insert_pos]) {
            break;
        }
    }
    // Now we know that the next free slot is definitely not earlier than the current position
    this->first_free_pos = insert_pos + 1;

    if (insert_pos == deleted_tuples.size()) {
        // insert new values at the end and increase vectors
        this->ol_o_id.push_back(ol_o_id);
        this->ol_d_id.push_back(ol_d_id);
        this->ol_w_id.push_back(ol_w_id);
        this->ol_number.push_back(ol_number);
        this->ol_i_id.push_back(ol_i_id);
        this->ol_supply_w_id.push_back(ol_supply_w_id);
        this->ol_delivery_d.push_back(ol_delivery_d);
        this->ol_quantity.push_back(ol_quantity);
        this->ol_amount.push_back(ol_amount);
        this->ol_dist_info.push_back(ol_dist_info);
        this->deleted_tuples.push_back(false);
    } else {
        // we found a free spot and can insert the new tuple there
        this->ol_o_id[insert_pos] = ol_o_id;
        this->ol_d_id[insert_pos] = ol_d_id;
        this->ol_w_id[insert_pos] = ol_w_id;
        this->ol_number[insert_pos] = ol_number;
        this->ol_i_id[insert_pos] = ol_i_id;
        this->ol_supply_w_id[insert_pos] = ol_supply_w_id;
        this->ol_delivery_d[insert_pos] = ol_delivery_d;
        this->ol_quantity[insert_pos] = ol_quantity;
        this->ol_amount[insert_pos] = ol_amount;
        this->ol_dist_info[insert_pos] = ol_dist_info;
        this->deleted_tuples[insert_pos] = false;
    }

    this->primary_key[Key(ol_w_id, ol_d_id, ol_o_id, ol_number)] = insert_pos;

    this->size++;
    return insert_pos;
}

void orderlineTable::remove(const uint64_t tid) {
    // The table has a primary index we need to update.
    auto ol_w_id_key = ol_w_id[tid];
    auto ol_d_id_key = ol_d_id[tid];
    auto ol_o_id_key = ol_o_id[tid];
    auto ol_number_key = ol_number[tid];

    this->primary_key.erase(Key(ol_w_id_key, ol_d_id_key, ol_o_id_key, ol_number_key));
    this->deleted_tuples[tid] = true;
    this->size--;

    // Update first_free_pos to speed up following inserts
    if (tid < this->first_free_pos) {
        this->first_free_pos = tid;
    }
}


std::optional<Integer> orderlineTable::get_ol_o_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->ol_o_id[tid];
}

std::optional<Integer> orderlineTable::get_ol_d_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->ol_d_id[tid];
}

std::optional<Integer> orderlineTable::get_ol_w_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->ol_w_id[tid];
}

std::optional<Integer> orderlineTable::get_ol_number(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->ol_number[tid];
}

std::optional<Integer> orderlineTable::get_ol_i_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->ol_i_id[tid];
}

std::optional<Integer> orderlineTable::get_ol_supply_w_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->ol_supply_w_id[tid];
}

std::optional<Timestamp> orderlineTable::get_ol_delivery_d(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->ol_delivery_d[tid];
}

std::optional<Numeric<2, 0>> orderlineTable::get_ol_quantity(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->ol_quantity[tid];
}

std::optional<Numeric<6, 2>> orderlineTable::get_ol_amount(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->ol_amount[tid];
}

std::optional<Char<24>> orderlineTable::get_ol_dist_info(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->ol_dist_info[tid];
}


void orderlineTable::update_ol_o_id(const uint64_t tid, const Integer ol_o_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto ol_w_id_key = this->ol_w_id[tid];
    auto ol_d_id_key = this->ol_d_id[tid];
    auto ol_o_id_key = this->ol_o_id[tid];
    auto ol_number_key = this->ol_number[tid];

    this->primary_key.erase(Key(ol_w_id_key, ol_d_id_key, ol_o_id_key, ol_number_key));

    // Update value
    this->ol_o_id[tid] = ol_o_id;
    ol_o_id_key = ol_o_id;

    // update index on primary key
    this->primary_key[Key(ol_w_id_key, ol_d_id_key, ol_o_id_key, ol_number_key)] = tid;
}

void orderlineTable::update_ol_d_id(const uint64_t tid, const Integer ol_d_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto ol_w_id_key = this->ol_w_id[tid];
    auto ol_d_id_key = this->ol_d_id[tid];
    auto ol_o_id_key = this->ol_o_id[tid];
    auto ol_number_key = this->ol_number[tid];

    this->primary_key.erase(Key(ol_w_id_key, ol_d_id_key, ol_o_id_key, ol_number_key));

    // Update value
    this->ol_d_id[tid] = ol_d_id;
    ol_d_id_key = ol_d_id;

    // update index on primary key
    this->primary_key[Key(ol_w_id_key, ol_d_id_key, ol_o_id_key, ol_number_key)] = tid;
}

void orderlineTable::update_ol_w_id(const uint64_t tid, const Integer ol_w_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto ol_w_id_key = this->ol_w_id[tid];
    auto ol_d_id_key = this->ol_d_id[tid];
    auto ol_o_id_key = this->ol_o_id[tid];
    auto ol_number_key = this->ol_number[tid];

    this->primary_key.erase(Key(ol_w_id_key, ol_d_id_key, ol_o_id_key, ol_number_key));

    // Update value
    this->ol_w_id[tid] = ol_w_id;
    ol_w_id_key = ol_w_id;

    // update index on primary key
    this->primary_key[Key(ol_w_id_key, ol_d_id_key, ol_o_id_key, ol_number_key)] = tid;
}

void orderlineTable::update_ol_number(const uint64_t tid, const Integer ol_number) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto ol_w_id_key = this->ol_w_id[tid];
    auto ol_d_id_key = this->ol_d_id[tid];
    auto ol_o_id_key = this->ol_o_id[tid];
    auto ol_number_key = this->ol_number[tid];

    this->primary_key.erase(Key(ol_w_id_key, ol_d_id_key, ol_o_id_key, ol_number_key));

    // Update value
    this->ol_number[tid] = ol_number;
    ol_number_key = ol_number;

    // update index on primary key
    this->primary_key[Key(ol_w_id_key, ol_d_id_key, ol_o_id_key, ol_number_key)] = tid;
}

void orderlineTable::update_ol_i_id(const uint64_t tid, const Integer ol_i_id) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->ol_i_id[tid] = ol_i_id;
}

void orderlineTable::update_ol_supply_w_id(const uint64_t tid, const Integer ol_supply_w_id) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->ol_supply_w_id[tid] = ol_supply_w_id;
}

void orderlineTable::update_ol_delivery_d(const uint64_t tid, const Timestamp ol_delivery_d) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->ol_delivery_d[tid] = ol_delivery_d;
}

void orderlineTable::update_ol_quantity(const uint64_t tid, const Numeric<2, 0> ol_quantity) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->ol_quantity[tid] = ol_quantity;
}

void orderlineTable::update_ol_amount(const uint64_t tid, const Numeric<6, 2> ol_amount) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->ol_amount[tid] = ol_amount;
}

void orderlineTable::update_ol_dist_info(const uint64_t tid, const Char<24> ol_dist_info) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->ol_dist_info[tid] = ol_dist_info;
}



// ------------------------------------------------
// Generated sources for table item
// ------------------------------------------------
const std::vector<IU> itemTable::ius {
    IU("item", "i_id", imlab::schemac::Type::Integer()),
    IU("item", "i_im_id", imlab::schemac::Type::Integer()),
    IU("item", "i_name", imlab::schemac::Type::Varchar(24)),
    IU("item", "i_price", imlab::schemac::Type::Numeric(5, 2)),
    IU("item", "i_data", imlab::schemac::Type::Varchar(50)),
};

std::vector<const IU*> itemTable::CollectIUs() {
    std::vector<const IU*> iu_ptrs {};
    iu_ptrs.reserve(ius.size());
    for (auto& iu : ius) {
        iu_ptrs.push_back(&iu);
    }
    return iu_ptrs;
}

uint64_t itemTable::insert(
        const Integer i_id,
        const Integer i_im_id,
        const Varchar<24> i_name,
        const Numeric<5, 2> i_price,
        const Varchar<50> i_data) {
    // search for free insert position
    size_t insert_pos = this->first_free_pos;
    for (; insert_pos < this->deleted_tuples.size(); insert_pos++) {
        if (this->deleted_tuples[insert_pos]) {
            break;
        }
    }
    // Now we know that the next free slot is definitely not earlier than the current position
    this->first_free_pos = insert_pos + 1;

    if (insert_pos == deleted_tuples.size()) {
        // insert new values at the end and increase vectors
        this->i_id.push_back(i_id);
        this->i_im_id.push_back(i_im_id);
        this->i_name.push_back(i_name);
        this->i_price.push_back(i_price);
        this->i_data.push_back(i_data);
        this->deleted_tuples.push_back(false);
    } else {
        // we found a free spot and can insert the new tuple there
        this->i_id[insert_pos] = i_id;
        this->i_im_id[insert_pos] = i_im_id;
        this->i_name[insert_pos] = i_name;
        this->i_price[insert_pos] = i_price;
        this->i_data[insert_pos] = i_data;
        this->deleted_tuples[insert_pos] = false;
    }

    this->primary_key[Key(i_id)] = insert_pos;

    this->size++;
    return insert_pos;
}

void itemTable::remove(const uint64_t tid) {
    // The table has a primary index we need to update.
    auto i_id_key = i_id[tid];

    this->primary_key.erase(Key(i_id_key));
    this->deleted_tuples[tid] = true;
    this->size--;

    // Update first_free_pos to speed up following inserts
    if (tid < this->first_free_pos) {
        this->first_free_pos = tid;
    }
}


std::optional<Integer> itemTable::get_i_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->i_id[tid];
}

std::optional<Integer> itemTable::get_i_im_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->i_im_id[tid];
}

std::optional<Varchar<24>> itemTable::get_i_name(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->i_name[tid];
}

std::optional<Numeric<5, 2>> itemTable::get_i_price(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->i_price[tid];
}

std::optional<Varchar<50>> itemTable::get_i_data(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->i_data[tid];
}


void itemTable::update_i_id(const uint64_t tid, const Integer i_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto i_id_key = this->i_id[tid];

    this->primary_key.erase(Key(i_id_key));

    // Update value
    this->i_id[tid] = i_id;
    i_id_key = i_id;

    // update index on primary key
    this->primary_key[Key(i_id_key)] = tid;
}

void itemTable::update_i_im_id(const uint64_t tid, const Integer i_im_id) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->i_im_id[tid] = i_im_id;
}

void itemTable::update_i_name(const uint64_t tid, const Varchar<24> i_name) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->i_name[tid] = i_name;
}

void itemTable::update_i_price(const uint64_t tid, const Numeric<5, 2> i_price) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->i_price[tid] = i_price;
}

void itemTable::update_i_data(const uint64_t tid, const Varchar<50> i_data) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->i_data[tid] = i_data;
}



// ------------------------------------------------
// Generated sources for table stock
// ------------------------------------------------
const std::vector<IU> stockTable::ius {
    IU("stock", "s_i_id", imlab::schemac::Type::Integer()),
    IU("stock", "s_w_id", imlab::schemac::Type::Integer()),
    IU("stock", "s_quantity", imlab::schemac::Type::Numeric(4, 0)),
    IU("stock", "s_dist_01", imlab::schemac::Type::Char(24)),
    IU("stock", "s_dist_02", imlab::schemac::Type::Char(24)),
    IU("stock", "s_dist_03", imlab::schemac::Type::Char(24)),
    IU("stock", "s_dist_04", imlab::schemac::Type::Char(24)),
    IU("stock", "s_dist_05", imlab::schemac::Type::Char(24)),
    IU("stock", "s_dist_06", imlab::schemac::Type::Char(24)),
    IU("stock", "s_dist_07", imlab::schemac::Type::Char(24)),
    IU("stock", "s_dist_08", imlab::schemac::Type::Char(24)),
    IU("stock", "s_dist_09", imlab::schemac::Type::Char(24)),
    IU("stock", "s_dist_10", imlab::schemac::Type::Char(24)),
    IU("stock", "s_ytd", imlab::schemac::Type::Numeric(8, 0)),
    IU("stock", "s_order_cnt", imlab::schemac::Type::Numeric(4, 0)),
    IU("stock", "s_remote_cnt", imlab::schemac::Type::Numeric(4, 0)),
    IU("stock", "s_data", imlab::schemac::Type::Varchar(50)),
};

std::vector<const IU*> stockTable::CollectIUs() {
    std::vector<const IU*> iu_ptrs {};
    iu_ptrs.reserve(ius.size());
    for (auto& iu : ius) {
        iu_ptrs.push_back(&iu);
    }
    return iu_ptrs;
}

uint64_t stockTable::insert(
        const Integer s_i_id,
        const Integer s_w_id,
        const Numeric<4, 0> s_quantity,
        const Char<24> s_dist_01,
        const Char<24> s_dist_02,
        const Char<24> s_dist_03,
        const Char<24> s_dist_04,
        const Char<24> s_dist_05,
        const Char<24> s_dist_06,
        const Char<24> s_dist_07,
        const Char<24> s_dist_08,
        const Char<24> s_dist_09,
        const Char<24> s_dist_10,
        const Numeric<8, 0> s_ytd,
        const Numeric<4, 0> s_order_cnt,
        const Numeric<4, 0> s_remote_cnt,
        const Varchar<50> s_data) {
    // search for free insert position
    size_t insert_pos = this->first_free_pos;
    for (; insert_pos < this->deleted_tuples.size(); insert_pos++) {
        if (this->deleted_tuples[insert_pos]) {
            break;
        }
    }
    // Now we know that the next free slot is definitely not earlier than the current position
    this->first_free_pos = insert_pos + 1;

    if (insert_pos == deleted_tuples.size()) {
        // insert new values at the end and increase vectors
        this->s_i_id.push_back(s_i_id);
        this->s_w_id.push_back(s_w_id);
        this->s_quantity.push_back(s_quantity);
        this->s_dist_01.push_back(s_dist_01);
        this->s_dist_02.push_back(s_dist_02);
        this->s_dist_03.push_back(s_dist_03);
        this->s_dist_04.push_back(s_dist_04);
        this->s_dist_05.push_back(s_dist_05);
        this->s_dist_06.push_back(s_dist_06);
        this->s_dist_07.push_back(s_dist_07);
        this->s_dist_08.push_back(s_dist_08);
        this->s_dist_09.push_back(s_dist_09);
        this->s_dist_10.push_back(s_dist_10);
        this->s_ytd.push_back(s_ytd);
        this->s_order_cnt.push_back(s_order_cnt);
        this->s_remote_cnt.push_back(s_remote_cnt);
        this->s_data.push_back(s_data);
        this->deleted_tuples.push_back(false);
    } else {
        // we found a free spot and can insert the new tuple there
        this->s_i_id[insert_pos] = s_i_id;
        this->s_w_id[insert_pos] = s_w_id;
        this->s_quantity[insert_pos] = s_quantity;
        this->s_dist_01[insert_pos] = s_dist_01;
        this->s_dist_02[insert_pos] = s_dist_02;
        this->s_dist_03[insert_pos] = s_dist_03;
        this->s_dist_04[insert_pos] = s_dist_04;
        this->s_dist_05[insert_pos] = s_dist_05;
        this->s_dist_06[insert_pos] = s_dist_06;
        this->s_dist_07[insert_pos] = s_dist_07;
        this->s_dist_08[insert_pos] = s_dist_08;
        this->s_dist_09[insert_pos] = s_dist_09;
        this->s_dist_10[insert_pos] = s_dist_10;
        this->s_ytd[insert_pos] = s_ytd;
        this->s_order_cnt[insert_pos] = s_order_cnt;
        this->s_remote_cnt[insert_pos] = s_remote_cnt;
        this->s_data[insert_pos] = s_data;
        this->deleted_tuples[insert_pos] = false;
    }

    this->primary_key[Key(s_w_id, s_i_id)] = insert_pos;

    this->size++;
    return insert_pos;
}

void stockTable::remove(const uint64_t tid) {
    // The table has a primary index we need to update.
    auto s_w_id_key = s_w_id[tid];
    auto s_i_id_key = s_i_id[tid];

    this->primary_key.erase(Key(s_w_id_key, s_i_id_key));
    this->deleted_tuples[tid] = true;
    this->size--;

    // Update first_free_pos to speed up following inserts
    if (tid < this->first_free_pos) {
        this->first_free_pos = tid;
    }
}


std::optional<Integer> stockTable::get_s_i_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_i_id[tid];
}

std::optional<Integer> stockTable::get_s_w_id(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_w_id[tid];
}

std::optional<Numeric<4, 0>> stockTable::get_s_quantity(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_quantity[tid];
}

std::optional<Char<24>> stockTable::get_s_dist_01(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_dist_01[tid];
}

std::optional<Char<24>> stockTable::get_s_dist_02(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_dist_02[tid];
}

std::optional<Char<24>> stockTable::get_s_dist_03(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_dist_03[tid];
}

std::optional<Char<24>> stockTable::get_s_dist_04(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_dist_04[tid];
}

std::optional<Char<24>> stockTable::get_s_dist_05(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_dist_05[tid];
}

std::optional<Char<24>> stockTable::get_s_dist_06(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_dist_06[tid];
}

std::optional<Char<24>> stockTable::get_s_dist_07(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_dist_07[tid];
}

std::optional<Char<24>> stockTable::get_s_dist_08(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_dist_08[tid];
}

std::optional<Char<24>> stockTable::get_s_dist_09(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_dist_09[tid];
}

std::optional<Char<24>> stockTable::get_s_dist_10(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_dist_10[tid];
}

std::optional<Numeric<8, 0>> stockTable::get_s_ytd(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_ytd[tid];
}

std::optional<Numeric<4, 0>> stockTable::get_s_order_cnt(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_order_cnt[tid];
}

std::optional<Numeric<4, 0>> stockTable::get_s_remote_cnt(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_remote_cnt[tid];
}

std::optional<Varchar<50>> stockTable::get_s_data(const uint64_t tid) {
    if (this->deleted_tuples[tid] == true) {
        return std::nullopt;
    }
    return this->s_data[tid];
}


void stockTable::update_s_i_id(const uint64_t tid, const Integer s_i_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto s_w_id_key = this->s_w_id[tid];
    auto s_i_id_key = this->s_i_id[tid];

    this->primary_key.erase(Key(s_w_id_key, s_i_id_key));

    // Update value
    this->s_i_id[tid] = s_i_id;
    s_i_id_key = s_i_id;

    // update index on primary key
    this->primary_key[Key(s_w_id_key, s_i_id_key)] = tid;
}

void stockTable::update_s_w_id(const uint64_t tid, const Integer s_w_id) {
    // The attribute is part of an index. Thus, we need to update the index as well.
    auto s_w_id_key = this->s_w_id[tid];
    auto s_i_id_key = this->s_i_id[tid];

    this->primary_key.erase(Key(s_w_id_key, s_i_id_key));

    // Update value
    this->s_w_id[tid] = s_w_id;
    s_w_id_key = s_w_id;

    // update index on primary key
    this->primary_key[Key(s_w_id_key, s_i_id_key)] = tid;
}

void stockTable::update_s_quantity(const uint64_t tid, const Numeric<4, 0> s_quantity) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_quantity[tid] = s_quantity;
}

void stockTable::update_s_dist_01(const uint64_t tid, const Char<24> s_dist_01) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_dist_01[tid] = s_dist_01;
}

void stockTable::update_s_dist_02(const uint64_t tid, const Char<24> s_dist_02) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_dist_02[tid] = s_dist_02;
}

void stockTable::update_s_dist_03(const uint64_t tid, const Char<24> s_dist_03) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_dist_03[tid] = s_dist_03;
}

void stockTable::update_s_dist_04(const uint64_t tid, const Char<24> s_dist_04) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_dist_04[tid] = s_dist_04;
}

void stockTable::update_s_dist_05(const uint64_t tid, const Char<24> s_dist_05) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_dist_05[tid] = s_dist_05;
}

void stockTable::update_s_dist_06(const uint64_t tid, const Char<24> s_dist_06) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_dist_06[tid] = s_dist_06;
}

void stockTable::update_s_dist_07(const uint64_t tid, const Char<24> s_dist_07) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_dist_07[tid] = s_dist_07;
}

void stockTable::update_s_dist_08(const uint64_t tid, const Char<24> s_dist_08) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_dist_08[tid] = s_dist_08;
}

void stockTable::update_s_dist_09(const uint64_t tid, const Char<24> s_dist_09) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_dist_09[tid] = s_dist_09;
}

void stockTable::update_s_dist_10(const uint64_t tid, const Char<24> s_dist_10) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_dist_10[tid] = s_dist_10;
}

void stockTable::update_s_ytd(const uint64_t tid, const Numeric<8, 0> s_ytd) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_ytd[tid] = s_ytd;
}

void stockTable::update_s_order_cnt(const uint64_t tid, const Numeric<4, 0> s_order_cnt) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_order_cnt[tid] = s_order_cnt;
}

void stockTable::update_s_remote_cnt(const uint64_t tid, const Numeric<4, 0> s_remote_cnt) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_remote_cnt[tid] = s_remote_cnt;
}

void stockTable::update_s_data(const uint64_t tid, const Varchar<50> s_data) {
    // Because the attribute is not part of any indexes, we can simply overwrite it
    this->s_data[tid] = s_data;
}




}  // namespace tpcc
}  // namespace imlab
