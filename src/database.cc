// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/database.h"
#include <array>
#include <iostream>
#include "imlab/infra/error.h"
#include "imlab/infra/types.h"
#include "imlab/infra/hash_table.h"
#include "imlab/infra/data_loader.h"

namespace imlab {

    void Database::LoadWarehouse(std::istream &in) {
        while (in.peek() != EOF) {
            auto[w_id, w_name, w_street_1, w_street_2, w_city, w_state, w_zip, w_tax, w_ytd]
            = parseLine<Integer, Varchar<10>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Numeric<4, 4>, Numeric<12, 2>>(in);
            warehouseTable.insert(w_id, w_name, w_street_1, w_street_2, w_city,
                                  w_state, w_zip, w_tax, w_ytd);
        }
    }

    void Database::LoadDistrict(std::istream &in) {
        while (in.peek() != EOF) {
            auto[d_id, d_w_id, d_name, d_street_1, d_street_2, d_city, d_state, d_zip, d_tax, d_ytd, d_next_o_id]
            = parseLine<Integer, Integer, Varchar<10>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Numeric<4, 4>, Numeric<12, 2>, Integer>(in);
            districtTable.insert(d_id, d_w_id, d_name, d_street_1, d_street_2, d_city,
                                 d_state, d_zip, d_tax, d_ytd, d_next_o_id);
        }
    }

    void Database::LoadCustomer(std::istream &in) {
        while (in.peek() != EOF) {
            auto[c_id, c_d_id, c_w_id, c_first, c_middle, c_last, c_street_1, c_street_2, c_city, c_state, c_zip, c_phone, c_since, c_credit, c_credit_lim, c_discount, c_balance, c_ytd_paymenr, c_payment_cnt, c_delivery_cnt, c_data]
            = parseLine<Integer, Integer, Integer, Varchar<16>, Char<2>, Varchar<16>, Varchar<20>, Varchar<20>, Varchar<20>, Char<2>, Char<9>, Char<16>, Timestamp, Char<2>, Numeric<12, 2>, Numeric<4, 4>, Numeric<12, 2>, Numeric<12, 2>, Numeric<4, 0>, Numeric<4, 0>, Varchar<500>>(in);
            customerTable.insert(c_id, c_d_id, c_w_id, c_first, c_middle, c_last,
                                 c_street_1, c_street_2, c_city, c_state, c_zip, c_phone, c_since,
                                 c_credit, c_credit_lim, c_discount, c_balance, c_ytd_paymenr,
                                 c_payment_cnt, c_delivery_cnt, c_data);
        }
    }

    void Database::LoadHistory(std::istream &in) {
        while (in.peek() != EOF) {
            auto[h_c_id, h_c_d_id, h_c_w_id, h_d_id, h_w_id, h_date, h_amount, h_data] = parseLine<Integer, Integer, Integer, Integer, Integer, Timestamp, Numeric<6, 2>, Varchar<24>>(in);
            historyTable.insert(h_c_id, h_c_d_id, h_c_w_id, h_d_id, h_w_id, h_date,
                                h_amount, h_data);
        }
    }

    void Database::LoadNewOrder(std::istream &in) {
        while (in.peek() != EOF) {
            auto[no_o_id, no_d_id, no_w_id] = parseLine<Integer, Integer, Integer>(in);
            neworderTable.insert(no_o_id, no_d_id, no_w_id);
        }
    }

    void Database::LoadOrder(std::istream &in) {
        while (in.peek() != EOF) {
            auto[o_id, o_d_id, o_w_id, o_c_id, o_entry_d, o_carrier_id, o_ol_cnt, o_all_local] = parseLine<Integer, Integer, Integer, Integer, Timestamp, Integer, Numeric<2, 0>, Numeric<1, 0>>(in);
            orderTable.insert(o_id, o_d_id, o_w_id, o_c_id, o_entry_d, o_carrier_id,
                              o_ol_cnt, o_all_local);
        }
    }

    void Database::LoadOrderLine(std::istream &in) {
        while (in.peek() != EOF) {
            auto[ol_o_id, ol_d_id, ol_w_id, ol_number, ol_i_id, ol_supply_w_id, ol_delivery_d, ol_quantity, ol_amount, ol_dist_info]
            = parseLine<Integer, Integer, Integer, Integer, Integer, Integer, Timestamp, Numeric<2, 0>, Numeric<6, 2>, Char<24>>(in);
            orderlineTable.insert(ol_o_id, ol_d_id, ol_w_id, ol_number, ol_i_id,
                                  ol_supply_w_id, ol_delivery_d, ol_quantity, ol_amount, ol_dist_info);
        }
    }

    void Database::LoadItem(std::istream &in) {
        while (in.peek() != EOF) {
            auto[i_id, i_im_id, i_name, i_price, i_data] = parseLine<Integer, Integer, Varchar<24>, Numeric<5, 2>, Varchar<50>>(in);
            itemTable.insert(i_id, i_im_id, i_name, i_price, i_data);
        }
    }

    void Database::LoadStock(std::istream &in) {
        while (in.peek() != EOF) {
            auto[s_i_id, s_w_id, s_quantity, s_dist_01, s_dist_02, s_dist_03, s_dist_04, s_dist_05, s_dist_06, s_dist_07, s_dist_08, s_dist_09, s_dist_10, s_ytd, s_order_cnt, s_remote_cnt, s_data]
            = parseLine<Integer, Integer, Numeric<4, 0>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Char<24>, Numeric<8, 0>, Numeric<4, 0>, Numeric<4, 0>, Varchar<50>>(in);
            stockTable.insert(s_i_id, s_w_id, s_quantity, s_dist_01, s_dist_02,
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
        // select w_tax from warehouse w where w.w_id=w_id;
        // select c_discount from customer c where c_w_id=w_id and c_d_id=d_id and c.c_id=c_id;
        // select d_next_o_id as o_id,d_tax from district d where d_w_id=w_id and d.d_id=d_id;
        auto w_tax = warehouseTable.get_w_tax(warehouseTable.primary_key.at(Key(w_id))).value();
        auto c_discount = customerTable.get_c_discount(customerTable.primary_key.at(Key(w_id, d_id, c_id))).value();
        auto o_id = districtTable.get_d_next_o_id(districtTable.primary_key.at(Key(w_id, d_id))).value();
        auto d_tax = districtTable.get_d_tax(districtTable.primary_key.at(Key(w_id, d_id))).value();

        // update district set d_next_o_id=o_id+1 where d_w_id=w_id and district.d_id=d_id;
        districtTable.update_d_next_o_id(districtTable.primary_key.at(Key(w_id, d_id)), o_id + Integer(1));

        // var integer all_local = 1;
        // forsequence (index between 0 and items-1) {
        //    if (w_id<>supware[index])
        //       all_local=0;
        // }
        int all_local = 1;
        for (int index = 0; index < items.value; index++) {
            if (w_id != supware[index])
                all_local = 0;
        }

        // insert into "order" values (o_id,d_id,w_id,c_id,datetime,0,items,all_local);
        // insert into neworder values (o_id,d_id,w_id);
        orderTable.insert(o_id, d_id, w_id, c_id, datetime, Integer(0), Numeric<2, 0>(items), Numeric<1, 0>(all_local));
        neworderTable.insert(o_id, d_id, w_id);

        // forsequence (index between 0 and items-1) {
        for (int index = 0; index < items.value; index++) {
            // select i_price from item where i_id=itemid[index];
            auto i_price = itemTable.get_i_price(itemTable.primary_key.at(Key(itemid[index]))).value();

            // select s_quantity,s_remote_cnt,s_order_cnt,case d_id when 1 then s_dist_01 when 2 then s_dist_02 when 3 then s_dist_03
            // when 4 then s_dist_04 when 5 then s_dist_05 when 6 then s_dist_06 when 7 then s_dist_07 when 8 then s_dist_08
            // when 9 then s_dist_09 when 10 then s_dist_10 end as s_dist from stock where s_w_id=supware[index] and s_i_id=itemid[index];
            auto s_quantity = stockTable.get_s_quantity(
                    stockTable.primary_key.at(Key(supware[index], itemid[index]))).value();
            auto s_remote_cnt = stockTable.get_s_remote_cnt(
                    stockTable.primary_key.at(Key(supware[index], itemid[index]))).value();
            auto s_order_cnt = stockTable.get_s_order_cnt(
                    stockTable.primary_key.at(Key(supware[index], itemid[index]))).value();
            auto s_dist = [&](Integer d_id) {
                switch (d_id.value) {
                    case 1:
                        return stockTable.get_s_dist_01(
                                stockTable.primary_key.at(Key(supware[index], itemid[index]))).value();
                    case 2:
                        return stockTable.get_s_dist_02(
                                stockTable.primary_key.at(Key(supware[index], itemid[index]))).value();
                    case 3:
                        return stockTable.get_s_dist_03(
                                stockTable.primary_key.at(Key(supware[index], itemid[index]))).value();
                    case 4:
                        return stockTable.get_s_dist_04(
                                stockTable.primary_key.at(Key(supware[index], itemid[index]))).value();
                    case 5:
                        return stockTable.get_s_dist_05(
                                stockTable.primary_key.at(Key(supware[index], itemid[index]))).value();
                    case 6:
                        return stockTable.get_s_dist_06(
                                stockTable.primary_key.at(Key(supware[index], itemid[index]))).value();
                    case 7:
                        return stockTable.get_s_dist_07(
                                stockTable.primary_key.at(Key(supware[index], itemid[index]))).value();
                    case 8:
                        return stockTable.get_s_dist_08(
                                stockTable.primary_key.at(Key(supware[index], itemid[index]))).value();
                    case 9:
                        return stockTable.get_s_dist_09(
                                stockTable.primary_key.at(Key(supware[index], itemid[index]))).value();
                    case 10:
                        return stockTable.get_s_dist_10(
                                stockTable.primary_key.at(Key(supware[index], itemid[index]))).value();
                }
            }(d_id);

            // if (s_quantity>qty[index]) {
            //    update stock set s_quantity=s_quantity-qty[index] where s_w_id=supware[index] and s_i_id=itemid[index];
            // } else {
            //    update stock set s_quantity=s_quantity+91-qty[index] where s_w_id=supware[index] and s_i_id=itemid[index];
            // }
            if (s_quantity > Numeric<4, 0>(qty[index])) {
                stockTable.update_s_quantity(stockTable.primary_key.at(Key(supware[index], itemid[index])),
                                             s_quantity - Numeric<4, 0>(qty[index]));
            } else {
                stockTable.update_s_quantity(stockTable.primary_key.at(Key(supware[index], itemid[index])),
                                             s_quantity + Numeric<4, 0>(91) - Numeric<4, 0>(qty[index]));
            }

            // if (supware[index]<>w_id) {
            //    update stock set s_remote_cnt=s_remote_cnt+1 where s_w_id=w_id and s_i_id=itemid[index];
            // } else {
            //    update stock set s_order_cnt=s_order_cnt+1 where s_w_id=w_id and s_i_id=itemid[index];
            // }
            if (supware[index] != w_id) {
                stockTable.update_s_remote_cnt(stockTable.primary_key.at(Key(w_id, itemid[index])),
                                               s_remote_cnt + Numeric<4, 0>(1));
            } else {
                stockTable.update_s_remote_cnt(stockTable.primary_key.at(Key(w_id, itemid[index])),
                                               s_order_cnt + Numeric<4, 0>(1));
            }

            // var numeric(6,2) ol_amount=qty[index]*i_price*(1.0+w_tax+d_tax)*(1.0-c_discount);
            Numeric<6, 2> ol_amount = (Numeric<5, 2>(qty[index]) * i_price *
                                       (Numeric<4, 4>(1) + w_tax + d_tax).castS<5>() * (Numeric<4, 4>(1) - c_discount)
                                               .castP2().castP2().castS<5>()).castS<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>();

            // insert into orderline values (o_id,d_id,w_id,index+1,itemid[index],supware[index],0,qty[index],ol_amount,s_dist);
            orderlineTable.insert(o_id, d_id, w_id, Integer(index + 1), itemid[index], supware[index], Timestamp(0),
                                  Numeric<2, 0>(qty[index]), ol_amount, s_dist);
        }
    }


    void Database::Delivery(
            Integer w_id,
            Integer o_carrier_id,
            Timestamp datetime) {
        // forsequence (d_id between 1 and 10) {
        for (Integer d_id = Integer(1); d_id < Integer(10); d_id = d_id + Integer(1)) {
            // select min(no_o_id) as o_id from neworder where no_w_id=w_id and no_d_id=d_id order by no_o_id else { continue; } -- ignore this district if no row found
            std::vector<Integer> select_from_neworder{};
            for (auto&[_, tid] : neworderTable.primary_key) {
                if (neworderTable.get_no_w_id(tid) == w_id && neworderTable.get_no_d_id(tid) == Integer(d_id)) {
                    select_from_neworder.push_back(neworderTable.get_no_o_id(tid).value());
                }
            }
            if (select_from_neworder.size() == 0) {
                continue;
            }
            auto o_id = *std::min_element(select_from_neworder.begin(), select_from_neworder.end());

            // delete from neworder where no_w_id=w_id and no_d_id=d_id and no_o_id=o_id;
            neworderTable.remove(neworderTable.primary_key.at(Key(w_id, d_id, o_id)));

            // select o_ol_cnt,o_c_id from "order" o where o_w_id=w_id and o_d_id=d_id and o.o_id=o_id;
            auto o_ol_cnt = orderTable.get_o_ol_cnt(orderTable.primary_key.at(Key(w_id, d_id, o_id))).value();
            auto o_c_id = orderTable.get_o_c_id(orderTable.primary_key.at(Key(w_id, d_id, o_id))).value();

            // update "order" set o_carrier_id=o_carrier_id where o_w_id=w_id and o_d_id=d_id and "order".o_id=o_id;
            orderTable.update_o_carrier_id(orderTable.primary_key.at(Key(w_id, d_id, o_id)), o_carrier_id);

            // var numeric(6,2) ol_total=0;
            auto ol_total = Numeric<6, 2>(0);
            // forsequence (ol_number between 1 and o_ol_cnt) {
            for (Integer ol_number = Integer(1);
                 Numeric<2, 0>(ol_number) < o_ol_cnt; ol_number = ol_number + Integer(1)) {
                // select ol_amount from orderline where ol_w_id=w_id and ol_d_id=d_id and ol_o_id=o_id and orderline.ol_number=ol_number;
                auto ol_amount = orderlineTable.get_ol_amount(
                        orderlineTable.primary_key.at(Key(w_id, d_id, o_id, ol_number))).value();
                // ol_total=ol_total+ol_amount;
                ol_total = ol_total + ol_amount;
                // update orderline set ol_delivery_d=datetime where ol_w_id=w_id and ol_d_id=d_id and ol_o_id=o_id and orderline.ol_number=ol_number;
                orderlineTable.update_ol_delivery_d(orderlineTable.primary_key.at(Key(w_id, d_id, o_id, ol_number)),
                                                    datetime);
            }

            // update customer set c_balance=c_balance+ol_total where c_w_id=w_id and c_d_id=d_id and c_id=o_c_id;
            customerTable.update_c_balance(customerTable.primary_key.at(Key(w_id, d_id, o_c_id)),
                                           customerTable.get_c_balance(
                                                   customerTable.primary_key.at(Key(w_id, d_id, o_c_id))).value() +
                                           ol_total.castS<12>());
        }
    }


    Numeric<12, 2> Database::AnalyticalQuerySTL() {
        // select sum(ol_quantity*ol_amount-c_balance*o_ol_cnt)
        //   from customer, "order", orderline
        // where o_w_id = c_w_id
        //   and o_d_id = c_d_id
        //   and o_c_id = c_id
        //   and o_w_id = ol_w_id
        //   and o_d_id = ol_d_id
        //   and o_id = ol_o_id
        //   and c_last like 'B%'

        Numeric<12, 4> sum = Numeric<12, 4>(0);

        std::unordered_multimap<Key<Integer /*c_w_id*/, Integer /*c_d_id*/, Integer /*c_id*/>, Numeric<12, 2> /*c_balance*/> join_customer_order;
        std::unordered_multimap<Key<Integer /*o_w_id*/, Integer /*o_d_id*/, Integer /*o_id*/>, std::tuple<Numeric<12, 2> /*c_balance*/, Numeric<2, 0> /*o_ol_cnt*/>> join_order_orderline;

        for (auto&[_, c_tid] : customerTable.primary_key) {
            auto c_last = customerTable.get_c_last(c_tid).value();
            if (*c_last.begin() == 'B') {
                auto c_w_id = customerTable.get_c_w_id(c_tid).value();
                auto c_d_id = customerTable.get_c_d_id(c_tid).value();
                auto c_id = customerTable.get_c_id(c_tid).value();
                auto c_balance = customerTable.get_c_balance(c_tid).value();
                join_customer_order.insert({Key(c_w_id, c_d_id, c_id), c_balance});
            }
        }

        for (auto&[_, o_tid] : orderTable.primary_key) {
            auto o_w_id = orderTable.get_o_w_id(o_tid).value();
            auto o_d_id = orderTable.get_o_d_id(o_tid).value();
            auto o_c_id = orderTable.get_o_c_id(o_tid).value();

            auto o_ol_cnt = orderTable.get_o_ol_cnt(o_tid).value();
            auto o_id = orderTable.get_o_id(o_tid).value();

            auto matches = join_customer_order.equal_range(Key(o_w_id, o_d_id, o_c_id));
            for (auto it = matches.first; it != matches.second; ++it) {
                auto c_balance = it->second;

                join_order_orderline.insert({Key(o_w_id, o_d_id, o_id), std::make_tuple(c_balance, o_ol_cnt)});
            }
        }

        for (auto&[_, ol_tid] : orderlineTable.primary_key) {
            auto ol_w_id = orderlineTable.get_ol_w_id(ol_tid).value();
            auto ol_d_id = orderlineTable.get_ol_d_id(ol_tid).value();
            auto ol_o_id = orderlineTable.get_ol_o_id(ol_tid).value();

            auto ol_quantity = orderlineTable.get_ol_quantity(ol_tid).value();
            auto ol_amount = orderlineTable.get_ol_amount(ol_tid).value();

            auto matches = join_order_orderline.equal_range(Key(ol_w_id, ol_d_id, ol_o_id));
            for (auto it = matches.first; it != matches.second; ++it) {
                auto c_balance = std::get<0>(it->second);
                auto o_ol_cnt = std::get<1>(it->second);

                sum = sum + ((ol_quantity.castP2().castS<6>() * ol_amount).castS<12>() -
                             (c_balance * o_ol_cnt.castP2().castS<12>()));
            }
        }

        return sum.castM2<12>();
    }


    Numeric<12, 2> Database::AnalyticalQueryLHT() {
        // select sum(ol_quantity*ol_amount-c_balance*o_ol_cnt)
        //   from customer, "order", orderline
        // where o_w_id = c_w_id
        //   and o_d_id = c_d_id
        //   and o_c_id = c_id
        //   and o_w_id = ol_w_id
        //   and o_d_id = ol_d_id
        //   and o_id = ol_o_id
        //   and c_last like 'B%'

        Numeric<12, 4> sum = Numeric<12, 4>(0);

        LazyMultiMap<Key<Integer /*c_w_id*/, Integer /*c_d_id*/, Integer /*c_id*/>, Numeric<12, 2> /*c_balance*/> join_customer_order;
        LazyMultiMap<Key<Integer /*o_w_id*/, Integer /*o_d_id*/, Integer /*o_id*/>, std::tuple<Numeric<12, 2> /*c_balance*/, Numeric<2, 0> /*o_ol_cnt*/>> join_order_orderline;

        for (auto&[_, c_tid] : customerTable.primary_key) {
            auto c_last = customerTable.get_c_last(c_tid).value();
            if (*c_last.begin() == 'B') {
                auto c_w_id = customerTable.get_c_w_id(c_tid).value();
                auto c_d_id = customerTable.get_c_d_id(c_tid).value();
                auto c_id = customerTable.get_c_id(c_tid).value();
                auto c_balance = customerTable.get_c_balance(c_tid).value();
                join_customer_order.insert({Key(c_w_id, c_d_id, c_id), c_balance});
            }
        }

        join_customer_order.finalize();

        for (auto&[_, o_tid] : orderTable.primary_key) {
            auto o_w_id = orderTable.get_o_w_id(o_tid).value();
            auto o_d_id = orderTable.get_o_d_id(o_tid).value();
            auto o_c_id = orderTable.get_o_c_id(o_tid).value();

            auto o_ol_cnt = orderTable.get_o_ol_cnt(o_tid).value();
            auto o_id = orderTable.get_o_id(o_tid).value();

            auto matches = join_customer_order.equal_range(Key(o_w_id, o_d_id, o_c_id));
            for (auto it = matches.first; it != matches.second; ++it) {
                auto c_balance = it->value;

                join_order_orderline.insert({Key(o_w_id, o_d_id, o_id), std::make_tuple(c_balance, o_ol_cnt)});
            }
        }

        join_order_orderline.finalize();

        for (auto&[_, ol_tid] : orderlineTable.primary_key) {
            auto ol_w_id = orderlineTable.get_ol_w_id(ol_tid).value();
            auto ol_d_id = orderlineTable.get_ol_d_id(ol_tid).value();
            auto ol_o_id = orderlineTable.get_ol_o_id(ol_tid).value();

            auto ol_quantity = orderlineTable.get_ol_quantity(ol_tid).value();
            auto ol_amount = orderlineTable.get_ol_amount(ol_tid).value();

            auto matches = join_order_orderline.equal_range(Key(ol_w_id, ol_d_id, ol_o_id));
            for (auto it = matches.first; it != matches.second; ++it) {
                auto c_balance = std::get<0>(it->value);
                auto o_ol_cnt = std::get<1>(it->value);

                sum = sum + ((ol_quantity.castP2().castS<6>() * ol_amount).castS<12>() -
                             (c_balance * o_ol_cnt.castP2().castS<12>()));
            }
        }

        return sum.castM2<12>();
    }


    template<>
    size_t Database::Size<tpcc::kcustomer>() { return customerTable.get_size(); }

    template<>
    size_t Database::Size<tpcc::kdistrict>() { return districtTable.get_size(); }

    template<>
    size_t Database::Size<tpcc::khistory>() { return historyTable.get_size(); }

    template<>
    size_t Database::Size<tpcc::kitem>() { return itemTable.get_size(); }

    template<>
    size_t Database::Size<tpcc::kneworder>() { return neworderTable.get_size(); }

    template<>
    size_t Database::Size<tpcc::korder>() { return orderTable.get_size(); }

    template<>
    size_t Database::Size<tpcc::korderline>() { return orderlineTable.get_size(); }

    template<>
    size_t Database::Size<tpcc::kstock>() { return stockTable.get_size(); }

    template<>
    size_t Database::Size<tpcc::kwarehouse>() { return warehouseTable.get_size(); }

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
