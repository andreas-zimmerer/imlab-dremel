#include <iostream>
#include <fstream>

#include "gflags/gflags.h"
#include "imlab/algebra/inner_join.h"
#include "imlab/algebra/iu.h"
#include "imlab/algebra/operator.h"
#include "imlab/algebra/print.h"
#include "imlab/algebra/selection.h"
#include "imlab/algebra/table_scan.h"

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

    // TODO
}

