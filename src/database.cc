// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "database.h"
#include "rapidjson/document.h"
#include "../include/imlab/schema.h"
#include <rapidjson/istreamwrapper.h>

namespace imlab {

void Database::LoadDocumentTable(std::istream &in) {
    rapidjson::IStreamWrapper stream(in);
    rapidjson::Document d;
    d.ParseStream(stream);

    for (const auto &document : d.GetArray()) {
        schema::Document document_record {};

        const auto &docId = document["DocId"]; //  DocId is required
        const auto &docId_value = docId.GetInt();
        document_record.DocId = Integer(docId_value);

        if (document.HasMember("Links")) { //  Links is optional
            const auto &links = document["Links"];
            schema::Document::Links document_links {};

            if (links.HasMember("Backward")) {
                for (const auto &backward : links["Backward"].GetArray()) { //  Backward is repeated
                    const auto &backward_value = backward.GetInt();
                    document_links.Backward.push_back(Integer(backward_value));
                }
            }

            if (links.HasMember("Forward")) {
                for (const auto &forward : links["Forward"].GetArray()) { //  Forward is repeated
                    const auto &forward_value = forward.GetInt();
                    document_links.Forward.push_back(Integer(forward_value));
                }
            }

            document_record.links = document_links;
        }

        if (document.HasMember("Name")) {
            for (const auto &name : document["Name"].GetArray()) { // Name is repeated
                schema::Document::Name document_name{};

                if (name.HasMember("Language")) {
                    for (const auto &language : name["Language"].GetArray()) { // Language repeated
                        schema::Document::Name::Language document_name_language{};

                        const auto &code = language["Code"]; //  Code is required
                        const auto &code_value = code.GetString();
                        document_name_language.Code = Varchar<30>::build(code_value);

                        if (language.HasMember("Country")) { //  Country is optional
                            const auto &country = language["Country"];
                            const auto &country_value = country.GetString();
                            document_name_language.Country = Varchar<30>::build(code_value);
                        }

                        document_name.language.push_back(document_name_language);
                    }
                }

                if (name.HasMember("Url")) { //  Url is optional
                    const auto &url = name["Url"];
                     const auto &url_value = url.GetString();
                    document_name.Url = Varchar<30>::build(url_value);
                }

                document_record.name.push_back(document_name);
            }
        }

        documentTable.insert(document_record);
    }
}

}  // namespace imlab
