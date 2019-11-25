// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
#include "imlab/queryc/query_parse_context.h"
#include "./gen/query_parser.h"
#include <sstream>
#include <unordered_set>
#include "imlab/infra/error.h"
#include "imlab/schema.h"
#include "imlab/algebra/table_scan.h"
#include "imlab/algebra/print.h"
#include "imlab/algebra/inner_join.h"
#include "imlab/algebra/selection.h"
// ---------------------------------------------------------------------------------------------------
using namespace imlab;
using QueryParseContext = imlab::queryc::QueryParseContext;
// ---------------------------------------------------------------------------------------------------

// Constructor
QueryParseContext::QueryParseContext(const schemac::Schema& schema, bool trace_scanning, bool trace_parsing)
    : schema(schema), trace_scanning_(trace_scanning), trace_parsing_(trace_parsing) {}
// ---------------------------------------------------------------------------------------------------
// Destructor
QueryParseContext::~QueryParseContext() {}
// ---------------------------------------------------------------------------------------------------
// Parse a string
Query& QueryParseContext::Parse(std::istream &in) {
    beginScan(in);
    imlab::queryc::QueryParser parser(*this);
    parser.set_debug_level(trace_parsing_);
    parser.parse();
    endScan();

    return this->query;
}
// ---------------------------------------------------------------------------------------------------
// Yield an error
void QueryParseContext::Error(const std::string& m) {
    throw QueryCompilationError(m);
}
// ---------------------------------------------------------------------------------------------------
// Yield an error
void QueryParseContext::Error(uint32_t line, uint32_t column, const std::string &err) {
    std::stringstream ss;
    ss << "[ l=" << line << " c=" << column << " ] " << err << std::endl;
    throw QueryCompilationError(ss.str());
}
// ---------------------------------------------------------------------------------------------------
// Define a table
void QueryParseContext::CreateSqlQuery(const std::vector<std::string> &select_columns,
                                       const std::vector<std::string> &relations,
                                       const std::vector<std::pair<std::string, std::string>> &where_predicates) {

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

    const IU& iu_customer_c_id = tpcc::customerTable::IUs[0];
    const IU& iu_customer_c_w_id = tpcc::customerTable::IUs[2];
    const IU& iu_customer_c_d_id = tpcc::customerTable::IUs[1];
    const IU& iu_customer_c_first = tpcc::customerTable::IUs[3];
    const IU& iu_customer_c_last = tpcc::customerTable::IUs[5];
    const IU& iu_order_o_w_id = tpcc::orderTable::IUs[2];
    const IU& iu_order_o_d_id = tpcc::orderTable::IUs[1];
    const IU& iu_order_o_c_id = tpcc::orderTable::IUs[3];
    const IU& iu_order_o_id = tpcc::orderTable::IUs[0];
    const IU& iu_order_o_all_local = tpcc::orderTable::IUs[7];
    const IU& iu_orderline_ol_w_id = tpcc::orderlineTable::IUs[2];
    const IU& iu_orderline_ol_d_id = tpcc::orderlineTable::IUs[1];
    const IU& iu_orderline_ol_o_id = tpcc::orderlineTable::IUs[0];
    const IU& iu_orderline_ol_amount = tpcc::orderlineTable::IUs[8];

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
                                          {&iu_customer_c_d_id, &iu_order_o_d_id},
                                          {&iu_customer_c_id,   &iu_order_o_c_id}});
    InnerJoin order_orderline_join(std::make_unique<InnerJoin>(std::move(customer_order_join)),
                                   std::make_unique<TableScan>(orderline_table_scan), {
                                           {&iu_order_o_w_id, &iu_orderline_ol_w_id},
                                           {&iu_order_o_d_id, &iu_orderline_ol_d_id},
                                           {&iu_order_o_id,   &iu_orderline_ol_o_id}});
    Print print(std::make_unique<InnerJoin>(std::move(order_orderline_join)));

    print.Prepare({&iu_customer_c_first, &iu_customer_c_last, &iu_order_o_all_local, &iu_orderline_ol_amount}, nullptr);

    this->query.op = std::move(print);
}
// ---------------------------------------------------------------------------------------------------
