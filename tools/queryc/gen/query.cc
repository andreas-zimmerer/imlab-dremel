
#include "../../../include/imlab/schema.h"
#include "../../../include/imlab/database.h"
#include "../../../include/imlab/schemac/schema_parse_context.h"
#include <iostream>
#include <iomanip>

namespace imlab {

    extern "C" void Run(imlab::Database &db) {
        std::cout << "| " << std::left << std::setw(16) << "c_first";
        std::cout << "| " << std::left << std::setw(16) << "c_last";
        std::cout << "| " << std::left << std::setw(16) << "o_all_local";
        std::cout << "| " << std::left << std::setw(16) << "ol_amount";
        std::cout << std::endl;
        std::unordered_multimap < Key < Integer/*o_w_id*/, Integer/*o_d_id*/, Integer/*o_id*/>, std::tuple < Varchar <
                                                                                                16 >/*c_first*/,
                Varchar < 16 >/*c_last*/, Numeric < 1, 0 >/*o_all_local*/>> order_orderline_hashmap;
        std::unordered_multimap < Key < Integer/*c_w_id*/, Integer/*c_d_id*/, Integer/*c_id*/>, std::tuple < Varchar <
                                                                                                16 >/*c_first*/,
                Varchar < 16 >/*c_last*/>> customer_order_hashmap;

        for (auto&[_, tid] : db.customerTable.primary_key) {
            auto customer_c_w_id = db.customerTable.get_c_w_id(tid).value();
            auto customer_c_d_id = db.customerTable.get_c_d_id(tid).value();
            auto customer_c_id = db.customerTable.get_c_id(tid).value();
            auto customer_c_first = db.customerTable.get_c_first(tid).value();
            auto customer_c_last = db.customerTable.get_c_last(tid).value();
            if (customer_c_id == Integer(322) && customer_c_w_id == Integer(1) && customer_c_d_id == Integer(1) &&
                true) {
                customer_order_hashmap.insert({Key(customer_c_w_id, customer_c_d_id, customer_c_id),
                                               std::make_tuple(customer_c_first, customer_c_last)});
            }
        }

        for (auto&[_, tid] : db.orderTable.primary_key) {
            auto order_o_w_id = db.orderTable.get_o_w_id(tid).value();
            auto order_o_d_id = db.orderTable.get_o_d_id(tid).value();
            auto order_o_c_id = db.orderTable.get_o_c_id(tid).value();
            auto order_o_id = db.orderTable.get_o_id(tid).value();
            auto order_o_all_local = db.orderTable.get_o_all_local(tid).value();

            auto matches = customer_order_hashmap.equal_range(Key(order_o_w_id, order_o_d_id, order_o_c_id));
            for (auto it = matches.first; it != matches.second; ++it) {
                auto &customer_c_first = std::get<0>(it->second);
                auto &customer_c_last = std::get<1>(it->second);
                order_orderline_hashmap.insert({Key(order_o_w_id, order_o_d_id, order_o_id),
                                                std::make_tuple(customer_c_first, customer_c_last, order_o_all_local)});
            }
        }

        for (auto&[_, tid] : db.orderlineTable.primary_key) {
            auto orderline_ol_w_id = db.orderlineTable.get_ol_w_id(tid).value();
            auto orderline_ol_d_id = db.orderlineTable.get_ol_d_id(tid).value();
            auto orderline_ol_o_id = db.orderlineTable.get_ol_o_id(tid).value();
            auto orderline_ol_amount = db.orderlineTable.get_ol_amount(tid).value();

            auto matches = order_orderline_hashmap.equal_range(
                    Key(orderline_ol_w_id, orderline_ol_d_id, orderline_ol_o_id));
            for (auto it = matches.first; it != matches.second; ++it) {
                auto &customer_c_first = std::get<0>(it->second);
                auto &customer_c_last = std::get<1>(it->second);
                auto &order_o_all_local = std::get<2>(it->second);
                std::cout << "| " << customer_c_first;
                std::cout << "| " << customer_c_last;
                std::cout << "| " << order_o_all_local;
                std::cout << "| " << orderline_ol_amount;
                std::cout << std::endl;
            }
        }

    }

}  // namespace imlab
