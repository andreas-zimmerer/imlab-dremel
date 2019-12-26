// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------

#include <cstdint>
#include <chrono>  // NOLINT
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "../tools/protobuf/gen/schema.h"
#include "imlab/dremel/record_fsm.h"
#include "imlab/dremel/shredding.h"
#include "database.h"
#include "benchmark/benchmark.h"

namespace {
using namespace imlab::dremel;

/// Constructs a complete finite state machine for a "Document" record.
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

/// Shreds a document into columnar representation.
/// The size of the record to be shredded is determined by an additional benchmark parameter.
/// The parameter determines the number of "Document.Name.Language"
void BM_Shredding_nLanguage(benchmark::State &state) {
    imlab::schema::DocumentTable document_table {};

    // Large record from paper:
    Document document {};
    document.set_docid(0);
    Document_Name* document_name = document.add_name();
    for (int i = 0; i < state.range(0); i++) {
        Document_Name_Language* document_name_language = document_name->add_language();
        document_name_language->set_code("en-us");
    }

    for (auto _ : state) {
        Shredder::DissectRecord(dynamic_cast<TableBase&>(document_table), document);
    }

    state.SetItemsProcessed(state.iterations());
}

/// Loads a large, randomly generated dataset into the database.
/// Involves:
///  * reading from disk
///  * reading JSON
///  * constructing Protobuf records
///  * shred records into columnar format
void BM_Load_Full_Dataset(benchmark::State &state) {
    // Generate some example data
    system("python3 ../data/dremel/generate_dremel_data.py > ../data/dremel/dremel_data.json");

    imlab::Database db;
    std::fstream dremel_file("../data/dremel/dremel_data.json", std::fstream::in);

    for (auto _ : state) {
        db.LoadDocumentTable(dremel_file);
    }

    state.SetItemsProcessed(db.documentTable.get_size());
}

}  // namespace

BENCHMARK(BM_Construct_Complete_FSM);
BENCHMARK(BM_Shredding_nLanguage)->RangeMultiplier(2)->Range(1, 1024);
BENCHMARK(BM_Load_Full_Dataset)->Iterations(1)->Unit(benchmark::kMillisecond);

int main(int argc, char** argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
