#include "Kmplete/Core/json_reader.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Rapidjson empty string", "[core][json][reader]")
{
    const char* EmptyStr = "";

    rapidjson::Document document;
    document.Parse(EmptyStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorDocumentEmpty);
}

TEST_CASE("Rapidjson empty json", "[core][json][reader]")
{
    const char* EmptyJsonStr = "{}";

    rapidjson::Document document;
    document.Parse(EmptyJsonStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorNone);
    REQUIRE(document.IsObject());
    REQUIRE_FALSE(document.HasMember("Hello"));
}

TEST_CASE("Rapidjson basic json", "[core][json][reader]")
{
    const char* BasicJsonStr = R"rjs(
    {
        "Hello": 321,
        "Var": true,
        "Str": "string",
        "Ptr": null
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(BasicJsonStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorNone);
    REQUIRE(document.IsObject());

    REQUIRE(document.HasMember("Hello"));
    REQUIRE(document["Hello"].IsInt());
    REQUIRE(document["Hello"] == 321);

    REQUIRE(document.HasMember("Var"));
    REQUIRE(document["Var"].IsBool());
    REQUIRE(document["Var"] == true);

    REQUIRE(document.HasMember("Str"));
    REQUIRE(document["Str"].IsString());
    REQUIRE(document["Str"] == "string");

    REQUIRE(document.HasMember("Ptr"));
    REQUIRE(document["Ptr"].IsNull());
    REQUIRE_FALSE(document["Ptr"] == 0);
}

TEST_CASE("Rapidjson duplicate keys (same type)", "[core][json][reader]")
{
    const char* JsonStr = R"rjs(
    {
        "Node1": true,
        "Node1": false
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(JsonStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorNone);
    REQUIRE(document.IsObject());

    REQUIRE(document.HasMember("Node1"));
    REQUIRE(document["Node1"].IsBool());
    REQUIRE(document["Node1"] == true);
}

TEST_CASE("Rapidjson duplicate keys (different types)", "[core][json][reader]")
{
    const char* JsonStr = R"rjs(
    {
        "Node1": 94,
        "Node1": false
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(JsonStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorNone);
    REQUIRE(document.IsObject());

    REQUIRE(document.HasMember("Node1"));
    REQUIRE(document["Node1"].IsInt());
    REQUIRE(document["Node1"] == 94);
}

TEST_CASE("Rapidjson arrays", "[core][json][reader]")
{
    const char* JsonWithArrayStr = R"rjs(
    {
        "IntArr": [1, 2, 3, 4],
        "StrArr": ["str1", "str2", "str3"],
        "AllArr": [1, "str2", false]
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(JsonWithArrayStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorNone);
    REQUIRE(document.IsObject());

    REQUIRE(document.HasMember("IntArr"));
    REQUIRE((document["IntArr"].IsArray() && 
        (document["IntArr"].Size() == 4) &&
        document["IntArr"][0].IsInt() && (document["IntArr"][0].GetInt() == 1) &&
        document["IntArr"][1].IsInt() && (document["IntArr"][1].GetInt() == 2) &&
        document["IntArr"][2].IsInt() && (document["IntArr"][2].GetInt() == 3) &&
        document["IntArr"][3].IsInt() && (document["IntArr"][3].GetInt() == 4)));

    REQUIRE(document.HasMember("StrArr"));
    REQUIRE((document["StrArr"].IsArray() &&
        (document["StrArr"].Size() == 3) &&
        document["StrArr"][0].IsString() && (strcmp(document["StrArr"][0].GetString(), "str1") == 0) &&
        document["StrArr"][1].IsString() && (strcmp(document["StrArr"][1].GetString(), "str2") == 0) &&
        document["StrArr"][2].IsString() && (strcmp(document["StrArr"][2].GetString(), "str3") == 0)));

    REQUIRE(document.HasMember("AllArr"));
    REQUIRE((document["AllArr"].IsArray() &&
        (document["AllArr"].Size() == 3) &&
        document["AllArr"][0].IsInt() && (document["AllArr"][0].GetInt() == 1) &&
        document["AllArr"][1].IsString() && (strcmp(document["AllArr"][1].GetString(), "str2") == 0) &&
        document["AllArr"][2].IsBool() && (document["AllArr"][2].GetBool() == false)));
}
//--------------------------------------------------------------------------

TEST_CASE("Rapidjson malformed json (not closing tag)", "[core][json][reader]")
{
    const char* MalformedJsonStr = R"rjs(
    {
        "Hello": 321,
        "Var": true,
        "Str": "string",
        "Ptr": null
    )rjs";

    rapidjson::Document document;
    document.Parse(MalformedJsonStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorObjectMissCommaOrCurlyBracket);
}

TEST_CASE("Rapidjson malformed json (missing colon)", "[core][json][reader]")
{
    const char* MalformedJsonStr = R"rjs(
    {
        "Hello" 321
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(MalformedJsonStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorObjectMissColon);
    REQUIRE_FALSE(document.IsObject());
}

TEST_CASE("Rapidjson malformed json (type error)", "[core][json][reader]")
{
    const char* MalformedJsonStr = R"rjs(
    {
        "Hello": string
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(MalformedJsonStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorValueInvalid);
    REQUIRE_FALSE(document.IsObject());
}

TEST_CASE("Rapidjson malformed json (non singular root)", "[core][json][reader]")
{
    const char* MalformedJsonStr = R"rjs(
    {
        "Hello": "string"
    },
    {
        "World": 321
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(MalformedJsonStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorDocumentRootNotSingular);
    REQUIRE_FALSE(document.IsObject());
}

TEST_CASE("Rapidjson malformed json (array with different objects)", "[core][json][reader]")
{
    const char* MalformedJsonStr = R"rjs(
    {
        "SomeArray": [ {"Prop1": 0}, {"Prop1": 'a'} ]
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(MalformedJsonStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorValueInvalid);
    REQUIRE_FALSE(document.IsObject());
}

TEST_CASE("Rapidjson malformed json (root with name)", "[core][json][reader]")
{
    const char* MalformedJsonStr = R"rjs(
    "RootNode": {
        "SubNode": 12
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(MalformedJsonStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorDocumentRootNotSingular);
    REQUIRE_FALSE(document.IsObject());
}

TEST_CASE("Rapidjson malformed json (unnamed subnode)", "[core][json][reader]")
{
    const char* MalformedJsonStr = R"rjs(
    {
        { "SubNode1": 12 }
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(MalformedJsonStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorObjectMissName);
    REQUIRE_FALSE(document.IsObject());
}
//--------------------------------------------------------------------------

TEST_CASE("Json reader positive", "[core][json][reader]")
{
    const char* BasicJson = R"rjs(
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
    )rjs";

    rapidjson::Document document;
    document.Parse(BasicJson);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorNone);
    REQUIRE(document.IsObject());

    Kmplete::JsonReader reader(document);

    REQUIRE_FALSE(reader.StartObject(""));
    REQUIRE(reader.StartObject("Group1"));
        REQUIRE(reader.GetBool("Prop1") == false);
        REQUIRE(reader.GetBool("Prop1", true) == true);
        REQUIRE(reader.GetString("Prop1").empty());
        REQUIRE(reader.GetString("Prop1", "default") == "default");
        REQUIRE(reader.GetInt("Prop1") == 11);
        REQUIRE(reader.GetInt("Prop1", 345345) == 11);
        REQUIRE(reader.GetInt("NonExistProp") == 0);
        REQUIRE(reader.GetInt("NonExistProp", 64) == 64);
        REQUIRE(reader.GetDouble("", 13.0) == 13.0);
        REQUIRE(reader.GetDouble("") == 0.0);
        REQUIRE_FALSE(reader.StartObject(3));
        REQUIRE(reader.StartArray("NonExistArray") == 0);
        REQUIRE_FALSE(reader.EndArray());

        REQUIRE(reader.StartArray("Prop4") == 2);
            REQUIRE(reader.GetInt(1) == 33);
            REQUIRE(reader.GetInt(0) == 22);
            REQUIRE(reader.GetInt(11) == 0);
            REQUIRE(reader.GetInt(11, 46) == 46);
            REQUIRE(reader.GetInt("NamedValue", 100) == 100);
        REQUIRE(reader.EndArray()); // Prop4

        REQUIRE(reader.StartArray("Prop4") == 2);
        REQUIRE(reader.EndObject());

        REQUIRE(reader.StartArray("Prop5") == 1);
            REQUIRE_FALSE(reader.StartObject(""));
            REQUIRE(reader.StartObject(0));
            REQUIRE(reader.EndObject());
            REQUIRE(reader.StartObject(0));
                REQUIRE(reader.GetDouble("arrProp") == 11.0);
                REQUIRE(reader.GetDouble("arrProp", 4456.0) == 11.0);
                REQUIRE(reader.GetDouble("ArrPROP", 98.0) == 98.0);
            REQUIRE(reader.EndObject());
            REQUIRE_FALSE(reader.StartObject(1));
        REQUIRE(reader.EndArray()); // Prop5
        REQUIRE_FALSE(reader.StartObject("Group2"));
    REQUIRE(reader.EndObject()); // Group1

    REQUIRE_FALSE(reader.StartObject("Group2"));
    REQUIRE(reader.StartArray("Group2"));
        REQUIRE(reader.GetInt("someInt", -19) == -19);
        REQUIRE(reader.GetInt(0) == 1);
        REQUIRE(reader.GetInt(1) == 2);
        REQUIRE(reader.GetInt(2) == 3);
        REQUIRE(reader.GetInt(3) == 0);
        REQUIRE(reader.EndArray());
    REQUIRE_FALSE(reader.EndObject()); // Group2

    REQUIRE(reader.StartObject("Group3"));
        REQUIRE_FALSE(reader.StartArray("Group4"));
        REQUIRE(reader.GetString("Group4") == std::string(""));
        REQUIRE(reader.StartObject("Group4"));
            REQUIRE(reader.StartArray("Prop1") == 2);
                REQUIRE(reader.StartArray(0) == 2);
                REQUIRE(reader.EndArray());
                REQUIRE(reader.StartArray(1) == 5);
                REQUIRE(reader.EndArray());
            REQUIRE(reader.EndArray()); // Prop1
        REQUIRE(reader.EndObject()); // Group4
    REQUIRE(reader.EndObject()); // Group3
}
//--------------------------------------------------------------------------

TEST_CASE("Json reader negative", "[core][json][reader]")
{
    const char* BasicJson = R"rjs(
    {
        "Group1":
        {
            "Prop1": 11
        },
        "Group2": [1, 2, 3]
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(BasicJson);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorNone);
    REQUIRE(document.IsObject());

    Kmplete::JsonReader reader(document);

    REQUIRE_FALSE(reader.StartObject(""));
    REQUIRE(reader.StartObject("Group1"));
        REQUIRE(reader.StartObject("") == false);
        REQUIRE(reader.StartObject(-1) == false);
        REQUIRE(reader.StartObject(0) == false);

        REQUIRE(reader.StartArray("") == 0);
        REQUIRE(reader.StartArray(-1) == 0);
        REQUIRE(reader.StartArray(0) == 0);

        REQUIRE(reader.GetBool("") == false);
        REQUIRE(reader.GetBool(-1) == false);
        REQUIRE(reader.GetBool(0) == false);

        REQUIRE(reader.GetDouble("") == 0.0);
        REQUIRE(reader.GetDouble(-1) == 0.0);
        REQUIRE(reader.GetDouble(0) == 0.0);

        REQUIRE(reader.GetInt("") == 0);
        REQUIRE(reader.GetInt(-1) == 0);
        REQUIRE(reader.GetInt(0) == 0);

        REQUIRE(reader.GetInt64("") == 0);
        REQUIRE(reader.GetInt64(-1) == 0);
        REQUIRE(reader.GetInt64(0) == 0);

        REQUIRE(reader.GetString("") == std::string(""));
        REQUIRE(reader.GetString(-1) == std::string(""));
        REQUIRE(reader.GetString(0) == std::string(""));

        REQUIRE(reader.GetUInt("") == 0u);
        REQUIRE(reader.GetUInt(-1) == 0u);
        REQUIRE(reader.GetUInt(0) == 0u);

        REQUIRE(reader.GetUInt64("") == 0u);
        REQUIRE(reader.GetUInt64(-1) == 0u);
        REQUIRE(reader.GetUInt64(0) == 0u);
    REQUIRE(reader.EndObject()); // Group1


    REQUIRE(reader.StartArray("Group2") == 3);
        REQUIRE(reader.StartObject("") == false);
        REQUIRE(reader.StartObject("ValidName") == false);
        REQUIRE(reader.StartObject(-1) == false);

        REQUIRE(reader.StartArray("") == 0);
        REQUIRE(reader.StartArray("ValidName") == 0);
        REQUIRE(reader.StartArray(-1) == 0);

        REQUIRE(reader.GetBool("") == false);
        REQUIRE(reader.GetBool("ValidName") == false);
        REQUIRE(reader.GetBool(-1) == false);

        REQUIRE(reader.GetDouble("") == 0.0);
        REQUIRE(reader.GetDouble("ValidName") == 0.0);
        REQUIRE(reader.GetDouble(-1) == 0.0);

        REQUIRE(reader.GetInt("") == 0);
        REQUIRE(reader.GetInt("ValidName") == 0);
        REQUIRE(reader.GetInt(-1) == 0);

        REQUIRE(reader.GetInt64("") == 0);
        REQUIRE(reader.GetInt64("ValidName") == 0);
        REQUIRE(reader.GetInt64(-1) == 0);

        REQUIRE(reader.GetString("") == std::string(""));
        REQUIRE(reader.GetString("ValidName") == std::string(""));
        REQUIRE(reader.GetString(-1) == std::string(""));

        REQUIRE(reader.GetUInt("") == 0u);
        REQUIRE(reader.GetUInt("ValidName") == 0u);
        REQUIRE(reader.GetUInt(-1) == 0u);

        REQUIRE(reader.GetUInt64("") == 0u);
        REQUIRE(reader.GetUInt64("ValidName") == 0u);
        REQUIRE(reader.GetUInt64(-1) == 0u);
    REQUIRE(reader.EndArray()); // Group2
}
//--------------------------------------------------------------------------