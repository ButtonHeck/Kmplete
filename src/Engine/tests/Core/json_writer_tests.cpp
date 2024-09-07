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

    REQUIRE(writer.StartObject()); // Root
        REQUIRE(writer.StartObject("Group1"));
        REQUIRE_FALSE(writer.ParseToDocument());
            REQUIRE(writer.SaveInt("Prop1", 11));
            REQUIRE(writer.SaveBool("Prop2", true));
            REQUIRE(writer.SaveString("Prop3", "string"));
            REQUIRE(writer.StartArray("Prop4"));
                REQUIRE(writer.SaveInt(22));
                REQUIRE(writer.SaveInt(33));
            REQUIRE(writer.EndArray()); // Prop4
            REQUIRE(writer.StartArray("Prop5"));
                REQUIRE(writer.StartObject());
                    REQUIRE(writer.SaveDouble("arrProp", 11.0));
                REQUIRE(writer.EndObject());
            REQUIRE(writer.EndArray()); // Prop5
        REQUIRE(writer.EndObject()); // Group1
        REQUIRE(writer.StartArray("Group2"));
            REQUIRE_FALSE(writer.ParseToDocument());
            REQUIRE(writer.SaveInt(1));
            REQUIRE(writer.SaveInt(2));
            REQUIRE(writer.SaveInt(3));
        REQUIRE(writer.EndArray()); // Group2
        REQUIRE_FALSE(writer.ParseToDocument());
        REQUIRE(writer.StartObject("Group3"));
            REQUIRE(writer.StartObject("Group4"));
                REQUIRE(writer.StartArray("Prop1"));
                    REQUIRE(writer.StartArray());
                        REQUIRE(writer.SaveInt(42));
                        REQUIRE(writer.SaveInt(949));
                    REQUIRE(writer.EndArray());
                    REQUIRE(writer.StartArray());
                        REQUIRE(writer.SaveInt(44));
                        REQUIRE(writer.SaveInt(101));
                        REQUIRE(writer.SaveInt(202));
                        REQUIRE(writer.SaveInt(303));
                        REQUIRE(writer.SaveInt(404));
                    REQUIRE(writer.EndArray());
                REQUIRE(writer.EndArray()); // Prop1
            REQUIRE(writer.EndObject()); // Group4
        REQUIRE(writer.EndObject()); // Group3
    REQUIRE(writer.EndObject()); // Root
    REQUIRE(writer.ParseToDocument());
}
//--------------------------------------------------------------------------