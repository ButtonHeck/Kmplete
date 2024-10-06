#include "Kmplete/Json/json_writer.h"
#include "Kmplete/Json/json_reader.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Json Reader/Writer overwriting", "[json][reader][writer]")
{
    const char* BasicJson = R"rjs(
    {
        "Group1": [11, 22, 33],
        "Group2": [[9, 8, 7, 6]]
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(BasicJson);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorNone);
    REQUIRE(document.IsObject());

    Kmplete::JsonReader reader(document);
    Kmplete::JsonWriter writer(document);

    // check if reader still can read document's content
    REQUIRE(reader.StartArray("Group1") == 3);
    REQUIRE(reader.EndArray());

    // check if writer still can write document's content (append to Group1 array)
    auto newValue = 100;
    REQUIRE(writer.StartArray("Group1", false));
        REQUIRE(writer.SetInt(10, newValue));
    REQUIRE(writer.EndArray());

    // writer has set at index 10, but expect it pushed_back
    REQUIRE(reader.StartArray("Group1") == 4);
        REQUIRE(reader.GetInt(10) == 0);
        REQUIRE(reader.GetInt(3) == newValue);
    REQUIRE(reader.EndArray());

    // writer overwrite Group1 array
    newValue = 1000;
    REQUIRE(writer.StartArray("Group1"));
        REQUIRE(writer.SetInt(1000, newValue));
    REQUIRE(writer.EndArray());

    // reader read overwritten Group1 array
    REQUIRE(reader.StartArray("Group1") == 1);
        REQUIRE(reader.GetInt(0) == newValue);
    REQUIRE(reader.EndArray());

    // writer write Group1 object instead of array
    REQUIRE(writer.StartObject("Group1"));
        REQUIRE(writer.SetString("ObjStr", "str"));
    REQUIRE(writer.EndObject());

    // reader read Group1 object instead of array
    REQUIRE(reader.StartObject("Group1"));
        REQUIRE(reader.GetString("ObjStr") == std::string("str"));
    REQUIRE(reader.EndObject());


    // writer check append for Group2/0 array
    newValue = 654;
    REQUIRE(writer.StartArray("Group2", false));
        REQUIRE(writer.StartArray(0, false));
            REQUIRE(writer.SetInt(50, newValue));
        REQUIRE(writer.EndArray());
    REQUIRE(writer.EndArray());

    // reader check read appended Group2/0 array
    REQUIRE(reader.StartArray("Group2"));
        REQUIRE(reader.StartArray(0) == 5);
            REQUIRE(reader.GetInt(4) == newValue);
        REQUIRE(reader.EndArray());
    REQUIRE(reader.EndArray());

    // writer overwrite Group2/0 array
    REQUIRE(writer.StartArray("Group2", false));
        REQUIRE(writer.StartArray(0));
            REQUIRE(writer.SetInt(0, 12));
            REQUIRE(writer.SetInt(1, 13));
        REQUIRE(writer.EndArray());
    REQUIRE(writer.EndArray());

    // reader check new Group2/0 array
    REQUIRE(reader.StartArray("Group2"));
        REQUIRE(reader.StartArray(0) == 2);
            REQUIRE(reader.GetInt(0) == 12);
            REQUIRE(reader.GetInt(1) == 13);
        REQUIRE(reader.EndArray());
    REQUIRE(reader.EndArray());
}
//--------------------------------------------------------------------------