// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------

#include <cstdint>
#include <chrono>  // NOLINT
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "tbb/tbb.h"
#include "../tools/protobuf/gen/schema.h"
#include "imlab/dremel/record_fsm.h"
#include "imlab/dremel/shredding.h"
#include "database.h"
#include "benchmark/benchmark.h"
#include "../include/database.h"

namespace {
using namespace imlab::dremel;

// FieldDescriptors:
const auto* DocId_Field = Document::descriptor()->FindFieldByName("DocId");
const auto* Links_Backward_Field = Document_Links::descriptor()->FindFieldByName("Backward");
const auto* Links_Forward_Field = Document_Links::descriptor()->FindFieldByName("Forward");
const auto* Name_Language_Code_Field = Document_Name_Language::descriptor()->FindFieldByName("Code");
const auto* Name_Language_Country_Field = Document_Name_Language::descriptor()->FindFieldByName("Country");
const auto* Name_Url_Field = Document_Name::descriptor()->FindFieldByName("Url");

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
void BM_Load_Generated_Dataset(benchmark::State &state) {
    // Generate some example data
    system("cd ../data/dremel && python3 generate_dremel_data.py 10240 1024 > /dev/null");  // ~ 100 MiB

    imlab::Database db;
    std::fstream dremel_file("../data/dremel/data.json", std::fstream::in);

    for (auto _ : state) {
        db.LoadDocumentTable(dremel_file);
    }

    state.SetItemsProcessed(db.DocumentTable.size());
    state.SetBytesProcessed(102400 * 1024);  // only rough estimation
}

/// Load a record into the database and retrieve it.
/// Measure the time it takes for the get()-operation. Involves:
///  * building the FSM to get the appropriate fields.
///  * assemble record
void BM_Assemble_Document(benchmark::State &state) {
    imlab::Database db {};

    // Large Document record from Dremel paper.
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
    Document_Name_Language* document_name_3_language_1 = document_name_3->add_language();
    document_name_3_language_1->set_code("en-gb");
    document_name_3_language_1->set_country("gb");

    db.DocumentTable.insert(document);

    // FieldDescriptors of fields we want to retrieve.
    const auto* DocId_Field = Document::descriptor()->FindFieldByName("DocId");
    const auto& Links_Field = Document::descriptor()->FindFieldByName("links");
    const auto* Links_Backward_Field = Document_Links::descriptor()->FindFieldByName("Backward");
    const auto* Links_Forward_Field = Document_Links::descriptor()->FindFieldByName("Forward");
    const auto* Name_Language_Code_Field = Document_Name_Language::descriptor()->FindFieldByName("Code");
    const auto* Name_Language_Country_Field = Document_Name_Language::descriptor()->FindFieldByName("Country");
    const auto* Name_Url_Field = Document_Name::descriptor()->FindFieldByName("Url");

    for (auto _ : state) {
        const auto& record = db.DocumentTable.get(0, {
            DocId_Field,
            Links_Backward_Field,
            Links_Forward_Field,
            Name_Language_Code_Field,
            Name_Language_Country_Field,
            Name_Url_Field
        });
    }

    state.SetItemsProcessed(state.iterations());
}

/// Measures the time it takes to asseble all records from a randomly generated dataset.
/// You can pass the average record size in byte as an parameter of this benchmark.
void BM_Assembly_Generated_Dataset(benchmark::State &state) {
    uint64_t number_of_records = 100 * 1024 * 1024 / state.range(0);  // ~ 100 MiB
    uint64_t average_record_size = state.range(0);

    // Generate some example data
    system(("cd ../data/dremel && python3 generate_dremel_data.py " + std::to_string(number_of_records) + " " + std::to_string(average_record_size) + " > /dev/null").c_str());

    imlab::Database db;
    std::fstream dremel_file("../data/dremel/data.json", std::fstream::in);
    db.LoadDocumentTable(dremel_file);
    assert(db.DocumentTable.size() == number_of_records);

    for (auto _ : state) {
        const auto& documents_read = db.DocumentTable.get_range(0, db.DocumentTable.size(), {
                DocId_Field,
                Links_Backward_Field,
                Links_Forward_Field,
                Name_Language_Code_Field,
                Name_Language_Country_Field,
                Name_Url_Field
        });
    }

    state.SetItemsProcessed(number_of_records);
    state.SetBytesProcessed(number_of_records * average_record_size);  // only rough estimation
}

/// Measures the time it takes to asseble all records from a randomly generated dataset.
/// You can pass the average record size in byte as an parameter of this benchmark.
    void BM_Assembly_Generated_Dataset_Multithreaded(benchmark::State &state) {
        uint64_t number_of_records = 100 * 1024 * 1024 / state.range(0);  // ~ 100 MiB
        uint64_t average_record_size = state.range(0);

        // Generate some example data
        system(("cd ../data/dremel && python3 generate_dremel_data.py " + std::to_string(number_of_records) + " " + std::to_string(average_record_size) + " > /dev/null").c_str());

        imlab::Database db;
        std::fstream dremel_file("../data/dremel/data.json", std::fstream::in);
        db.LoadDocumentTable(dremel_file);
        assert(db.DocumentTable.size() == number_of_records);

        for (auto _ : state) {
            tbb::parallel_for(tbb::blocked_range<size_t>(0, db.DocumentTable.size()), [&](const tbb::blocked_range<size_t>& index_range) {
                const auto &documents_read = db.DocumentTable.get_range(index_range.begin(), index_range.end(), {
                        DocId_Field,
                        Links_Backward_Field,
                        Links_Forward_Field,
                        Name_Language_Code_Field,
                        Name_Language_Country_Field,
                        Name_Url_Field
                });
            });
        }

        state.SetItemsProcessed(number_of_records);
        state.SetBytesProcessed(number_of_records * average_record_size);  // only rough estimation
    }

}  // namespace

BENCHMARK(BM_Construct_Complete_FSM);
BENCHMARK(BM_Shredding_nLanguage)->RangeMultiplier(2)->Range(1, 1024);
BENCHMARK(BM_Load_Generated_Dataset)->Iterations(2)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_Assemble_Document);
BENCHMARK(BM_Assembly_Generated_Dataset)->Unit(benchmark::kMillisecond)->RangeMultiplier(2)->Range(64, 4096);
BENCHMARK(BM_Assembly_Generated_Dataset_Multithreaded)->Unit(benchmark::kMillisecond)->RangeMultiplier(2)->Range(64, 4096);

int main(int argc, char** argv) {
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
