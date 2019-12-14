// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "database.h"
#include "rapidjson/document.h"
#include "../tools/protobuf/gen/schema.pb.h"
#include <rapidjson/istreamwrapper.h>

namespace imlab {

void Database::LoadDocumentTable(std::istream &in) {
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

        documentTable.insert(document);
    }
}

}  // namespace imlab
