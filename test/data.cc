// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/test/data.h"

namespace imlab_test {

const char* const kTestDocumentPaperLarge = R"RAWJSON(
[
  {
    "DocId": 10,
    "Links": {
      "Forward": [
        20,
        40,
        60
      ]
    },
    "Name": [
      {
        "Language": [
          {
            "Code": "en-us",
            "Country": "us"
          },
          {
            "Code": "en"
          }
        ],
        "Url": "http://A"
      },
      {
        "Url": "http://B"
      },
      {
        "Language": [
          {
            "Code": "en-gb",
            "Country": "gb"
          }
        ]
      }
    ]
  }
]
)RAWJSON";

const char* const kTestDocumentPaperSmall = R"RAWJSON(
[
  {
    "DocId": 20,
    "Links": {
      "Backward": [
        10,
        30
      ],
      "Forward": [
        80
      ]
    },
    "Name": [
      {
        "Url": "http://C"
      }
    ]
  }
]
)RAWJSON";

const char* const kTestDocumentEmpty = R"RAWJSON(
[
  {
    "DocId": 30
  }
]
)RAWJSON";

}  // namespace imlab_test
