#include "Kmplete/Core/json_writer.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Json writer", "[core][json][writer]")
{
    /* Reference
    {
        "Group1":
        {
            "Prop1": 11,
            "Prop2": true,
            "Prop3": "string",
            "Prop4": [22, 33],
            "Prop5": [ {"arrProp": 11.0} ]
        },
        "Group2": [1, 2, 3],
        "Group3": 
        {
            "Group4":
            {
                "Prop1": [[42, 949], [44, 101, 202, 303, 404]]
            }
        }
    }
    */

    rapidjson::Document document;
    Kmplete::JsonWriter writer(document);

    REQUIRE(writer.StartObject("Group1"));
        REQUIRE(writer.SetInt("Prop1", 11));
        REQUIRE(writer.SetBool("Prop2", true));
        REQUIRE(writer.SetString("Prop3", "string"));
        REQUIRE(writer.StartArray("Prop4"));
            REQUIRE(writer.SetInt(0, 22));
            REQUIRE(writer.SetInt(1, 33));
        REQUIRE(writer.EndArray()); // Prop4
        REQUIRE(writer.StartArray("Prop5"));
            REQUIRE(writer.StartObject(0));
                REQUIRE(writer.SetDouble("arrProp", 11.0));
            REQUIRE(writer.EndObject());
        REQUIRE(writer.EndArray()); // Prop5
    REQUIRE(writer.EndObject()); // Group1
    REQUIRE(writer.StartArray("Group2"));
        REQUIRE(writer.SetInt(0, 1));
        REQUIRE(writer.SetInt(1, 2));
        REQUIRE(writer.SetInt(2, 3));
    REQUIRE(writer.EndArray()); // Group2
    REQUIRE(writer.StartObject("Group3"));
        REQUIRE(writer.StartObject("Group4"));
            REQUIRE(writer.StartArray("Prop1"));
                REQUIRE(writer.StartArray(0));
                    REQUIRE(writer.SetInt(0, 42));
                    REQUIRE(writer.SetInt(1, 949));
                REQUIRE(writer.EndArray());
                REQUIRE(writer.StartArray(1));
                    REQUIRE(writer.SetInt(0, 44));
                    REQUIRE(writer.SetInt(1, 101));
                    REQUIRE(writer.SetInt(2, 202));
                    REQUIRE(writer.SetInt(3, 303));
                    REQUIRE(writer.SetInt(4, 404));
                REQUIRE(writer.EndArray());
            REQUIRE(writer.EndArray()); // Prop1
        REQUIRE(writer.EndObject()); // Group4
    REQUIRE(writer.EndObject()); // Group3
}
//--------------------------------------------------------------------------