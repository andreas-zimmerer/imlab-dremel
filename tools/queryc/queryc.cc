#include <iostream>
#include <fstream>

#include "gflags/gflags.h"
#include "imlab/algebra/inner_join.h"
#include "imlab/algebra/operator.h"
#include "imlab/algebra/print.h"
#include "imlab/algebra/selection.h"
#include "imlab/algebra/table_scan.h"
#include "../tools/protobuf/gen/schema.h"
#include "imlab/schemac/schema_parse_context.h"
#include "imlab/queryc/query_parse_context.h"
#include "imlab/queryc/query_compiler.h"
#include "imlab/schemac/default_schema.h"

using QueryCompiler = imlab::queryc::QueryCompiler;
using QueryParseContext = imlab::queryc::QueryParseContext;

DEFINE_string(out_cc, "", "Output directory");
DEFINE_string(out_h, "", "Output directory");
DEFINE_string(in, "" , "SQL schema");

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
DEFINE_validator(in, &ValidateReadable);

int main(int argc, char *argv[]) {
    gflags::SetUsageMessage("queryc --in <SCHEMA> --out_cc <H> --out_h <CC>");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::ifstream in(FLAGS_in);
    std::ofstream out_h(FLAGS_out_h, std::ofstream::trunc);
    std::ofstream out_cc(FLAGS_out_cc, std::ofstream::trunc);

    QueryParseContext parse_context {imlab::schemac::defaultSchema};
    auto& query = parse_context.Parse(in);

    // Produce output files
    QueryCompiler compiler {out_h, out_cc};
    compiler.Compile(query);
}
