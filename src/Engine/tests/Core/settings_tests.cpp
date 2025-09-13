#include "Kmplete/Core/settings.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("Settings minimal saving", "[core][settings]")
{
    Kmplete::Settings settings("TestSettings");

    REQUIRE(settings.GetName() == "TestSettings");
    REQUIRE_FALSE(settings.EndLoadObject());
    REQUIRE_FALSE(settings.EndSaveObject());
    REQUIRE_FALSE(settings.StartSaveObject(""));
    REQUIRE_FALSE(settings.StartSaveArray(""));
}
//--------------------------------------------------------------------------

TEST_CASE("Settings normal saving", "[core][settings]")
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

    Kmplete::Settings settings("TestSettings");

    REQUIRE(settings.StartSaveObject("Group1"));
        REQUIRE(settings.SaveInt("Prop1", 11));
        REQUIRE(settings.SaveBool("Prop2", true));
        REQUIRE(settings.SaveString("Prop3", "string"));
        REQUIRE(settings.StartSaveArray("Prop4"));
            REQUIRE(settings.SaveInt(0, 22));
            REQUIRE(settings.SaveInt(1, 33));
        REQUIRE(settings.EndSaveArray()); // Prop4
        REQUIRE(settings.StartSaveArray("Prop5"));
            REQUIRE(settings.StartSaveObject(0));
                REQUIRE(settings.SaveDouble("arrProp", 11.0));
            REQUIRE(settings.EndSaveObject());
        REQUIRE(settings.EndSaveArray()); // Prop5
    REQUIRE(settings.EndSaveObject()); // Group1
    REQUIRE(settings.StartSaveArray("Group2"));
        REQUIRE(settings.SaveInt(0, 1));
        REQUIRE(settings.SaveInt(1, 2));
        REQUIRE(settings.SaveInt(3, 3));
    REQUIRE(settings.EndSaveArray()); // Group2
    REQUIRE(settings.StartSaveObject("Group3"));
        REQUIRE(settings.StartSaveObject("Group4"));
            REQUIRE(settings.StartSaveArray("Prop1"));
                REQUIRE(settings.StartSaveArray(0));
                    REQUIRE(settings.SaveInt(0, 42));
                    REQUIRE(settings.SaveInt(1, 949));
                REQUIRE(settings.EndSaveArray());
                REQUIRE(settings.StartSaveArray(1));
                    REQUIRE(settings.SaveInt(0, 44));
                    REQUIRE(settings.SaveInt(1, 101));
                    REQUIRE(settings.SaveInt(2, 202));
                    REQUIRE(settings.SaveInt(3, 303));
                    REQUIRE(settings.SaveInt(4, 404));
                REQUIRE(settings.EndSaveArray());
            REQUIRE(settings.EndSaveArray()); // Prop1
        REQUIRE(settings.EndSaveObject()); // Group4
    REQUIRE(settings.EndSaveObject()); // Group3
}
//--------------------------------------------------------------------------

TEST_CASE("Settings normal loading", "[core][settings]")
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

    const auto jsonDocument = Kmplete::CreatePtr<Kmplete::JsonDocument>(std::move(document));
    REQUIRE(jsonDocument);

    Kmplete::Settings settings("TestSettings", jsonDocument);
    REQUIRE(settings.GetName() == "TestSettings");

    REQUIRE_FALSE(settings.StartLoadObject("Obj"));
    REQUIRE_FALSE(settings.StartLoadObject(0));
    REQUIRE_FALSE(settings.EndLoadObject());
    REQUIRE_FALSE(settings.EndLoadArray());
    REQUIRE_FALSE(settings.StartLoadArray(0));
    REQUIRE_FALSE(settings.StartLoadArray("Group1"));

    REQUIRE(settings.StartLoadObject("Group1"));
        REQUIRE(settings.GetInt("Prop1") == 11);
        REQUIRE(settings.GetInt("Prop1", 33) == 11);
        REQUIRE(settings.GetInt("P1", 33) == 33);
        REQUIRE(settings.GetString("Prop1", "def") == "def");
        REQUIRE(settings.GetBool("Prop2") == true);
        REQUIRE(settings.GetString("Prop3") == "string");
        REQUIRE(settings.GetInt("Prop4") == 0);
        REQUIRE(settings.StartLoadArray("Prop4") == 2);
            REQUIRE(settings.GetString(0) == "");
            REQUIRE(settings.GetInt(0) == 22);
            REQUIRE(settings.GetInt(1) == 33);
            REQUIRE(settings.GetInt(1, 66) == 33);
            REQUIRE(settings.GetInt(2, -19) == -19);
        REQUIRE(settings.EndLoadArray()); // Prop4
        REQUIRE_FALSE(settings.EndLoadArray());
        REQUIRE(settings.StartLoadArray(1) == 0);
        REQUIRE(settings.StartLoadArray("dkgdf") == 0);
        REQUIRE(settings.StartLoadArray("Prop5") == 1);
            REQUIRE(settings.StartLoadObject(0));
                REQUIRE(settings.GetDouble("arrProp") == 11.0);
            REQUIRE(settings.EndLoadObject());
            REQUIRE_FALSE(settings.StartLoadObject(1));
        REQUIRE(settings.EndLoadArray()); // Prop5
    REQUIRE(settings.EndLoadObject()); // Group1
    REQUIRE_FALSE(settings.StartLoadObject("Group2"));
    REQUIRE(settings.StartLoadArray("Group2") == 3);
        REQUIRE(settings.GetInt(0, 222) == 1);
        REQUIRE(settings.GetInt(1) == 2);
        REQUIRE(settings.GetInt(2) == 3);
    REQUIRE(settings.EndLoadArray()); // Group2
    REQUIRE(settings.StartLoadObject("Group3"));
        REQUIRE(settings.StartLoadObject("Group4"));
            REQUIRE(settings.StartLoadArray("Prop1") == 2);
                REQUIRE(settings.StartLoadArray("Prop1") == 0);
                REQUIRE(settings.StartLoadArray(0) == 2);
                    REQUIRE(settings.GetInt(1) == 949);
                    REQUIRE(settings.GetInt(0) == 42);
                REQUIRE(settings.EndLoadArray());
                REQUIRE(settings.StartLoadArray(1) == 5);
                    REQUIRE(settings.GetInt(4) == 404);
                    REQUIRE(settings.GetInt(3) == 303);
                    REQUIRE(settings.GetInt(2) == 202);
                    REQUIRE(settings.GetInt(1) == 101);
                    REQUIRE(settings.GetInt(0) == 44);
                REQUIRE(settings.EndLoadArray());
            REQUIRE(settings.EndLoadArray()); // Prop1
        REQUIRE(settings.EndLoadObject()); // Group4
    REQUIRE(settings.EndLoadObject()); // Group3
}
//--------------------------------------------------------------------------

TEST_CASE("Settings loading malformed json", "[core][settings]")
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
    REQUIRE(document.HasParseError());

    const auto jsonDocument = Kmplete::CreatePtr<Kmplete::JsonDocument>(std::move(document));
    REQUIRE(jsonDocument);

    Kmplete::Settings settings("SettingsWithError", jsonDocument);

    REQUIRE_FALSE(settings.GetInt("Hello") == 321);
    REQUIRE_FALSE(settings.GetBool("Var") == true);
    REQUIRE_FALSE(settings.GetString("Str") == "string");
}
//--------------------------------------------------------------------------

TEST_CASE("Settings loading document with null value", "[core][settings]")
{
    const char* BasicJsonStr = R"rjs(
    {
        "Hello": 321,
        "Ptr": null
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(BasicJsonStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorNone);

    const auto jsonDocument = Kmplete::CreatePtr<Kmplete::JsonDocument>(std::move(document));
    REQUIRE(jsonDocument);

    Kmplete::Settings settings("SettingsWithNull", jsonDocument);

    REQUIRE(settings.GetInt("Hello", 99) == 321);
    REQUIRE(settings.GetInt("Ptr", 99) == 99);
}
//--------------------------------------------------------------------------

TEST_CASE("Settings loading duplicate values", "[core][settings]")
{
    const char* BasicJsonStr = R"rjs(
    {
        "Hello": 321,
        "Hello": 123
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(BasicJsonStr);
    const auto error = document.GetParseError();

    REQUIRE(error == rapidjson::kParseErrorNone);

    const auto jsonDocument = Kmplete::CreatePtr<Kmplete::JsonDocument>(std::move(document));
    REQUIRE(jsonDocument);

    Kmplete::Settings settings("SettingsWithDups", jsonDocument);

    REQUIRE(settings.GetInt("Hello") == 321);
}
//--------------------------------------------------------------------------