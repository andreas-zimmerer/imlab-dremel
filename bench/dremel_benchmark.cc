// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------

#include <cstdint>
#include <chrono>  // NOLINT
#include <iostream>
#include <fstream>
#include "../tools/protobuf/gen/schema.h"
#include "imlab/dremel/record_fsm.h"
#include "imlab/dremel/shredding.h"
#include "benchmark/benchmark.h"

namespace {
using namespace imlab::dremel;

void BM_Construct_Complete_FSM(benchmark::State &state) {
    auto* DocId = Document::descriptor()->FindFieldByName("DocId");
    auto* Links_Backward = Document_Links::descriptor()->FindFieldByName("Backward");
    auto* Links_Forward = Document_Links::descriptor()->FindFieldByName("Forward");
    auto* Name_Language_Code = Document_Name_Language::descriptor()->FindFieldByName("Code");
    auto* Name_Language_Country = Document_Name_Language::descriptor()->FindFieldByName("Country");
    auto* Name_Url = Document_Name::descriptor()->FindFieldByName("Url");

    for (auto _ : state) {
        RecordFSM fsm { std::vector<const FieldDescriptor*>{DocId, Links_Backward, Links_Forward, Name_Language_Code, Name_Language_Country, Name_Url} };
    }

    state.SetItemsProcessed(state.iterations());
}

void BM_Shredding(benchmark::State &state) {
    imlab::schema::DocumentTable document_table {};

    // Large record from paper:
    Document document {};
    document.set_docid(10);
    Document_Links* document_links = document.mutable_links();
    document_links->add_forward(20);
    document_links->add_forward(40);
    document_links->add_forward(60);
    Document_Name* document_name_1 = document.add_name();
    Document_Name_Language* document_name_1_language_1 = document_name_1->add_language();
    document_name_1_language_1->set_code("en-us");
    document_name_1_language_1->set_country("us");
    Document_Name_Language* document_name_1_language_2 = document_name_1->add_language();
    document_name_1_language_2->set_code("en");
    document_name_1->set_url("http://A");
    Document_Name* document_name_2 = document.add_name();
    document_name_2->set_url("http://B");
    Document_Name* document_name_3 = document.add_name();
    Document_Name_Language* document_name_3_language_1 = document_name_1->add_language();
    document_name_3_language_1->set_code("en-gb");
    document_name_3_language_1->set_country("gb");

    for (auto _ : state) {
        Shredder::DissectRecord(dynamic_cast<TableBase&>(document_table), document);
    }

    state.SetItemsProcessed(state.iterations());
}

}  // namespace

BENCHMARK(BM_Construct_Complete_FSM);
BENCHMARK(BM_Shredding);

int main(int argc, char** argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
