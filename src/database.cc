// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <dlfcn.h>
#include <fstream>
#include "database.h"
#include "rapidjson/document.h"
#include <rapidjson/istreamwrapper.h>
#include "imlab/queryc/query_compiler.h"

namespace imlab {
    using QueryCompiler = imlab::queryc::QueryCompiler;

void Database::DecodeJson(std::istream& in, const std::function<void (Document&)>& handler) {
    rapidjson::IStreamWrapper stream(in);
    rapidjson::Document d;
    d.ParseStream(stream);

    for (const auto &json : d.GetArray()) {
        Document document {};

        const auto &docId = json["DocId"];  // DocId is required
        const auto &docId_value = docId.GetInt();
        document.set_docid(docId_value);

        if (json.HasMember("Links")) {  // Links is optional
            const auto &links = json["Links"];
            Document_Links* document_links = document.mutable_links();

            if (links.HasMember("Backward")) {
                for (const auto &backward : links["Backward"].GetArray()) {  // Backward is repeated
                    const auto &backward_value = backward.GetInt();
                    document_links->add_backward(backward_value);
                }
            }

            if (links.HasMember("Forward")) {
                for (const auto &forward : links["Forward"].GetArray()) {  // Forward is repeated
                    const auto &forward_value = forward.GetInt();
                    document_links->add_forward(forward_value);
                }
            }
        }

        if (json.HasMember("Name")) {
            for (const auto &name : json["Name"].GetArray()) {  // Name is repeated
                Document_Name* document_name = document.add_name();

                if (name.HasMember("Language")) {
                    for (const auto &language : name["Language"].GetArray()) {  // Language repeated
                        Document_Name_Language* document_name_language = document_name->add_language();

                        const auto &code = language["Code"];  // Code is required
                        const auto &code_value = code.GetString();
                        document_name_language->set_code(code_value);

                        if (language.HasMember("Country")) {  // Country is optional
                            const auto &country = language["Country"];
                            const auto &country_value = country.GetString();
                            document_name_language->set_country(country_value);
                        }
                    }
                }

                if (name.HasMember("Url")) {  // Url is optional
                    const auto &url = name["Url"];
                    const auto &url_value = url.GetString();
                    document_name->set_url(url_value);
                }
            }
        }

        handler(document);
    }
}

/// Executes a compiled query.
void Database::LoadDocumentTable(std::istream &in) {
    DecodeJson(in, [&](auto& d) { documentTable.insert(d); });
}

void ExecuteQuery(imlab::Database &db, const std::string &dylib_path) {
    void *handle = dlopen(dylib_path.c_str(), RTLD_NOW);
    if (!handle) {
        std::cerr << "error: " << dlerror() << std::endl;
        exit(1);
    }
    auto run_query = reinterpret_cast<void (*)(imlab::Database &)>(dlsym(handle, "Run"));
    if (!run_query) {
        std::cerr << "error: " << dlerror() << std::endl;
        exit(1);
    }

    run_query(db);

    if (dlclose(handle)) {
        std::cerr << "error: " << dlerror() << std::endl;
        exit(1);
    }
}

QueryStats Database::RunQuery(Query& query) {
    //---------------------------------------------------------------------------------------
    auto code_generation_begin = std::chrono::steady_clock::now();

    std::ofstream query_out_h("../tools/queryc/gen/query.h", std::ofstream::trunc);
    std::ofstream query_out_cc("../tools/queryc/gen/query.cc", std::ofstream::trunc);
    QueryCompiler compiler {query_out_h, query_out_cc};

    compiler.Compile(query);

    query_out_h.close();
    query_out_cc.close();

    auto code_generation_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - code_generation_begin).count();
    //---------------------------------------------------------------------------------------

    //---------------------------------------------------------------------------------------
    auto code_compilation_begin = std::chrono::steady_clock::now();

    auto err = system("c++ ../tools/queryc/gen/query.cc -o ../tools/queryc/gen/query.so -std=c++17 -shared -fPIC -rdynamic");
    if (err) {
        throw std::runtime_error("Unable to compile query.");
    }

    auto code_compilation_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - code_compilation_begin).count();
    //---------------------------------------------------------------------------------------

    //---------------------------------------------------------------------------------------
    auto query_execution_begin = std::chrono::steady_clock::now();

    ExecuteQuery(*this, "../tools/queryc/gen/query.so");

    auto query_execution_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - query_execution_begin).count();
    //---------------------------------------------------------------------------------------

    return QueryStats {
        code_generation_duration,
        code_compilation_duration,
        query_execution_duration
    };
}

}  // namespace imlab
