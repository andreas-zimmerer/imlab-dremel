#include <iostream>
#include <fstream>
#include "gflags/gflags.h"
#include "imlab/algebra/inner_join.h"
#include "imlab/algebra/iu.h"
#include "imlab/algebra/operator.h"
#include "imlab/algebra/print.h"
#include "imlab/algebra/selection.h"
#include "imlab/algebra/table_scan.h"
#include "imlab/schemac/schema_parse_context.h"

using namespace imlab;

DEFINE_string(out_cc, "", "Header path");
DEFINE_string(out_h, "", "Implementation path");

static bool ValidateWritable(const char *flagname, const std::string &value) {
    std::ofstream out(value);
    return out.good();
}

static bool ValidateReadable(const char *flagname, const std::string &value) {
    std::ifstream in(value);
    return in.good();
}

DEFINE_validator(out_cc, &ValidateWritable);
DEFINE_validator(out_h, &ValidateWritable);

int main(int argc, char *argv[]) {
    gflags::SetUsageMessage("queryc --out_cc <H> --out_h <CC>");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::ofstream out_h(FLAGS_out_h, std::ofstream::trunc);
    std::ofstream out_cc(FLAGS_out_cc, std::ofstream::trunc);

    // THE QUERY
    // select c_first, c_last, o_all_local, ol_amount
    // from customer, "order", orderline
    // where o_w_id = c_w_id
    // and o_d_id = c_d_id
    // and o_c_id = c_id
    // and o_w_id = ol_w_id
    // and o_d_id = ol_d_id
    // and o_id = ol_o_id
    // and c_id = 322
    // and c_w_id = 1
    // and c_d_id = 1

    IU iu_customer_c_id{"customer", "c_id", schemac::Type::Integer()};
    IU iu_customer_c_w_id{"customer", "c_w_id", schemac::Type::Integer()};
    IU iu_customer_c_d_id{"customer", "c_d_id", schemac::Type::Integer()};
    IU iu_customer_c_first{"customer", "c_first", schemac::Type::Varchar(16)};
    IU iu_customer_c_last{"customer", "c_last", schemac::Type::Varchar(16)};
    IU iu_order_o_w_id{"order", "o_w_id", schemac::Type::Integer()};
    IU iu_order_o_d_id{"order", "o_d_id", schemac::Type::Integer()};
    IU iu_order_o_c_id{"order", "o_c_id", schemac::Type::Integer()};
    IU iu_order_o_id{"order", "o_id", schemac::Type::Integer()};
    IU iu_order_o_all_local{"order", "o_all_local", schemac::Type::Numeric(1, 0)};
    IU iu_orderline_ol_w_id{"orderline", "ol_w_id", schemac::Type::Integer()};
    IU iu_orderline_ol_d_id{"orderline", "ol_d_id", schemac::Type::Integer()};
    IU iu_orderline_ol_o_id{"orderline", "ol_o_id", schemac::Type::Integer()};
    IU iu_orderline_ol_amount{"orderline", "ol_amount", schemac::Type::Numeric(6, 2)};

    TableScan customer_table_scan("customer");
    TableScan order_table_scan("order");
    TableScan orderline_table_scan("orderline");
    Selection customer_select(std::make_unique<TableScan>(customer_table_scan), {
        {&iu_customer_c_id,   "Integer(322)"},
        {&iu_customer_c_w_id, "Integer(1)"},
        {&iu_customer_c_d_id, "Integer(1)"}});
    InnerJoin customer_order_join(std::make_unique<Selection>(std::move(customer_select)),
                                  std::make_unique<TableScan>(order_table_scan), {
        {&iu_customer_c_w_id, &iu_order_o_w_id},
        {&iu_customer_c_d_id, &iu_order_o_w_id},
        {&iu_customer_c_id,   &iu_order_o_c_id}});
    InnerJoin order_orderline_join(std::make_unique<InnerJoin>(std::move(customer_order_join)),
                                   std::make_unique<TableScan>(orderline_table_scan), {
        {&iu_order_o_w_id, &iu_orderline_ol_w_id},
        {&iu_order_o_d_id, &iu_orderline_ol_d_id},
        {&iu_order_o_id,   &iu_orderline_ol_o_id}});
    Print query(std::make_unique<InnerJoin>(std::move(order_orderline_join)), std::cout);

    query.Prepare({&iu_customer_c_first, &iu_customer_c_last, &iu_order_o_all_local, &iu_orderline_ol_amount}, nullptr);

    out_h << R"HEADER(
#ifndef INCLUDE_IMLAB_COMPILED_QUERY_H_
#define INCLUDE_IMLAB_COMPILED_QUERY_H_

namespace imlab {

    void RunCompiledQuery();

}  // namespace imlab
#endif  // INCLUDE_IMLAB_COMPILED_QUERY_H_
)HEADER";

    out_cc << R"IMPL(
#include "imlab/schema.h"
#include "imlab/schemac/schema_parse_context.h"

namespace imlab {

    void RunCompiledQuery() {
)IMPL";
    query.Produce(out_cc);
    out_cc << R"IMPL(
    }

}  // namespace imlab
)IMPL";
}
