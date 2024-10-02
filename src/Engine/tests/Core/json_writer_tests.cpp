#include "Kmplete/Core/json_writer.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Json writer positive", "[core][json][writer]")
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
                    // emulate randomized set order
                    REQUIRE(writer.SetInt(4, 404));
                    REQUIRE(writer.SetInt(0, 44));
                    REQUIRE(writer.SetInt(3, 303));
                    REQUIRE(writer.SetInt(1, 101));
                    REQUIRE(writer.SetInt(2, 202));
                REQUIRE(writer.EndArray());
            REQUIRE(writer.EndArray()); // Prop1
        REQUIRE(writer.EndObject()); // Group4
    REQUIRE(writer.EndObject()); // Group3
}
//--------------------------------------------------------------------------

TEST_CASE("Json writer negative", "[core][json][writer]")
{
    rapidjson::Document document;
    Kmplete::JsonWriter writer(document);

    REQUIRE(writer.StartObject("Group1"));
        REQUIRE_FALSE(writer.EndArray());

        REQUIRE_FALSE(writer.StartArray(0));
        REQUIRE_FALSE(writer.StartArray(-1));
        REQUIRE_FALSE(writer.StartArray(""));

        REQUIRE_FALSE(writer.StartObject(0));
        REQUIRE_FALSE(writer.StartObject(-1));
        REQUIRE_FALSE(writer.StartObject(""));

        REQUIRE_FALSE(writer.SetBool(0, true));
        REQUIRE_FALSE(writer.SetBool(-1, true));
        REQUIRE_FALSE(writer.SetBool("", true));

        REQUIRE_FALSE(writer.SetInt(0, 5));
        REQUIRE_FALSE(writer.SetInt(-1, 5));
        REQUIRE_FALSE(writer.SetInt("", 5));

        REQUIRE_FALSE(writer.SetInt64(0, 565684548));
        REQUIRE_FALSE(writer.SetInt64(-1, 565684548));
        REQUIRE_FALSE(writer.SetInt64("", 565684548));

        REQUIRE_FALSE(writer.SetUInt(0, 234u));
        REQUIRE_FALSE(writer.SetUInt(-1, 234u));
        REQUIRE_FALSE(writer.SetUInt("", 234u));

        REQUIRE_FALSE(writer.SetUInt64(0, 234u));
        REQUIRE_FALSE(writer.SetUInt64(-1, 234u));
        REQUIRE_FALSE(writer.SetUInt64("", 234u));

        REQUIRE_FALSE(writer.SetDouble(0, 1.0));
        REQUIRE_FALSE(writer.SetDouble(-1, 1.0));
        REQUIRE_FALSE(writer.SetDouble("", 1.0));

        REQUIRE_FALSE(writer.SetString(0, "str"));
        REQUIRE_FALSE(writer.SetString(-1, "str"));
        REQUIRE_FALSE(writer.SetString("", "str"));
    REQUIRE(writer.EndObject()); // Group1


    REQUIRE_FALSE(writer.StartObject(0));
    REQUIRE_FALSE(writer.StartObject(-1));
    REQUIRE_FALSE(writer.StartObject(""));
    REQUIRE_FALSE(writer.EndObject());
    REQUIRE_FALSE(writer.EndArray());


    REQUIRE_FALSE(writer.StartArray(0));
    REQUIRE_FALSE(writer.StartArray(-1));
    REQUIRE_FALSE(writer.StartArray(""));
    REQUIRE_FALSE(writer.EndArray());
    REQUIRE_FALSE(writer.EndObject());


    REQUIRE(writer.StartArray("Array"));
        REQUIRE_FALSE(writer.StartArray("InternalArray"));
        REQUIRE_FALSE(writer.StartArray(-1));
        REQUIRE(writer.StartArray(0));
        REQUIRE(writer.EndArray());

        REQUIRE_FALSE(writer.StartObject("InternalObject"));
        REQUIRE_FALSE(writer.StartObject(-1));
        REQUIRE(writer.StartObject(0)); // override array 0
        REQUIRE(writer.EndObject());

        REQUIRE(writer.SetBool(0, true));
        REQUIRE_FALSE(writer.SetBool(-1, true));
        REQUIRE_FALSE(writer.SetBool("", true));
        REQUIRE_FALSE(writer.SetBool("ValidName", true));

        REQUIRE(writer.SetInt(0, 5));
        REQUIRE_FALSE(writer.SetInt(-1, 5));
        REQUIRE_FALSE(writer.SetInt("", 5));
        REQUIRE_FALSE(writer.SetInt("ValidName", 5));

        REQUIRE(writer.SetInt64(0, 565684548));
        REQUIRE_FALSE(writer.SetInt64(-1, 565684548));
        REQUIRE_FALSE(writer.SetInt64("", 565684548));
        REQUIRE_FALSE(writer.SetInt64("ValidName", 565684548));

        REQUIRE(writer.SetUInt(0, 234u));
        REQUIRE_FALSE(writer.SetUInt(-1, 234u));
        REQUIRE_FALSE(writer.SetUInt("", 234u));
        REQUIRE_FALSE(writer.SetUInt("ValidName", 234u));

        REQUIRE(writer.SetUInt64(0, 234u));
        REQUIRE_FALSE(writer.SetUInt64(-1, 234u));
        REQUIRE_FALSE(writer.SetUInt64("", 234u));
        REQUIRE_FALSE(writer.SetUInt64("ValidName", 234u));

        REQUIRE(writer.SetDouble(0, 1.0));
        REQUIRE_FALSE(writer.SetDouble(-1, 1.0));
        REQUIRE_FALSE(writer.SetDouble("", 1.0));
        REQUIRE_FALSE(writer.SetDouble("ValidName", 1.0));

        REQUIRE(writer.SetString(0, "str"));
        REQUIRE_FALSE(writer.SetString(-1, "str"));
        REQUIRE_FALSE(writer.SetString("", "str"));
        REQUIRE_FALSE(writer.SetString("ValidName", "str"));
    REQUIRE(writer.EndArray());
}
//--------------------------------------------------------------------------