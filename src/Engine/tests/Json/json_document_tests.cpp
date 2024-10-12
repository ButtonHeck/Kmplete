#include "Kmplete/Json/json_document.h"
#include "Kmplete/Core/filesystem.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Json document from empty rapidjson document", "[json][reader][writer][document]")
{
    const char* EmptyStr = "";

    rapidjson::Document document;
    document.Parse(EmptyStr);

    Kmplete::JsonDocument jsonDocument(std::move(document));
    REQUIRE(jsonDocument.HasError());

    const auto childrenDocuments = jsonDocument.GetChildren();
    REQUIRE(childrenDocuments.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("Json document from minimal rapidjson document", "[json][reader][writer][document]")
{
    const char* EmptyJsonStr = "{}";

    rapidjson::Document document;
    document.Parse(EmptyJsonStr);

    Kmplete::JsonDocument jsonDocument(std::move(document));
    REQUIRE_FALSE(jsonDocument.HasError());

    auto childrenDocuments = jsonDocument.GetChildren();
    REQUIRE(childrenDocuments.empty());

    childrenDocuments = jsonDocument.GetChildren(false);
    REQUIRE(childrenDocuments.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("Json document from simple rapidjson document", "[json][reader][writer][document]")
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

    Kmplete::JsonDocument jsonDocument(std::move(document));
    REQUIRE_FALSE(jsonDocument.HasError());

    auto childrenDocuments = jsonDocument.GetChildren();
    REQUIRE(childrenDocuments.empty());

    childrenDocuments = jsonDocument.GetChildren(false);
    REQUIRE(childrenDocuments.size() == 4);

    REQUIRE_FALSE(jsonDocument.StartGetArray(0));
    REQUIRE_FALSE(jsonDocument.StartGetArray(-1));
    REQUIRE_FALSE(jsonDocument.StartGetArray(""));
    REQUIRE_FALSE(jsonDocument.StartGetArray("Hello"));

    REQUIRE_FALSE(jsonDocument.StartGetObject(0));
    REQUIRE_FALSE(jsonDocument.StartGetObject(-1));
    REQUIRE_FALSE(jsonDocument.StartGetObject(""));
    REQUIRE_FALSE(jsonDocument.StartGetObject("Hello"));

    REQUIRE(jsonDocument.GetInt("Hello") == 321);
    REQUIRE(jsonDocument.GetInt("Hello", 123) == 321);
    REQUIRE(jsonDocument.GetString("Hello") == std::string(""));
    REQUIRE(jsonDocument.GetString("Hello", "test") == std::string("test"));
    REQUIRE(jsonDocument.GetInt(0, 100) == 100);
    REQUIRE(jsonDocument.GetBool("Var") == true);
    REQUIRE(jsonDocument.GetString("Str") == std::string("string"));

    REQUIRE(jsonDocument.GetInt("Ptr", 10) == 10);
    REQUIRE(jsonDocument.SetInt("Ptr", 999));
    REQUIRE(jsonDocument.GetInt("Ptr", 10) == 999);

    REQUIRE_FALSE(jsonDocument.SetInt(0, -10));
    childrenDocuments = jsonDocument.GetChildren(false);
    REQUIRE(childrenDocuments.size() == 4);

    // Create new int
    REQUIRE(jsonDocument.SetInt("BrandNewInt", -10));
    childrenDocuments = jsonDocument.GetChildren(false);
    REQUIRE(childrenDocuments.size() == 5);
    REQUIRE(jsonDocument.GetInt("BrandNewInt") == -10);

    // replace BrandNewInt with an object with same name
    REQUIRE(jsonDocument.StartSetObject("BrandNewInt"));
    REQUIRE(jsonDocument.EndSetObject());
    childrenDocuments = jsonDocument.GetChildren(false);
    REQUIRE(childrenDocuments.size() == 5);
    childrenDocuments = jsonDocument.GetChildren();
    REQUIRE(childrenDocuments.size() == 1);
    REQUIRE(jsonDocument.GetInt("BrandNewInt") == 0);

    // replace BrandNewInt with an array
    REQUIRE(jsonDocument.StartSetArray("BrandNewInt", false));
    REQUIRE(jsonDocument.EndSetArray());
    childrenDocuments = jsonDocument.GetChildren();
    REQUIRE(childrenDocuments.empty());
    childrenDocuments = jsonDocument.GetChildren(false);
    REQUIRE(childrenDocuments.size() == 5);

    // check document string
    auto documentString = jsonDocument.ToString(); //pretty
    REQUIRE(!documentString.empty());
    documentString = jsonDocument.ToString(false); // oneline
    REQUIRE(!documentString.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("Json document from invalid rapidjson document", "[json][reader][writer][document]")
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

    Kmplete::JsonDocument jsonDocument(std::move(document));
    REQUIRE(jsonDocument.HasError());
}
//--------------------------------------------------------------------------

TEST_CASE("Json document add children documents positive", "[json][reader][writer][document]")
{
    Kmplete::JsonDocument rootDoc;
    REQUIRE_FALSE(rootDoc.HasError());
    auto childrenDocuments = rootDoc.GetChildren();
    REQUIRE(childrenDocuments.empty());

    // assure lifetime of child1 and child2 doesn't affect rootDoc (rapidjson allocators didn't mess up)
    std::string rootDocStringBeforeScopeExit = "";
    {
        Kmplete::JsonDocument child1;
        REQUIRE(child1.SetInt("Int1", 10));
        REQUIRE(child1.SetInt("Int2", 20));
        REQUIRE(child1.SetInt("Int3", 30));

        Kmplete::JsonDocument child2;
        REQUIRE(child2.StartSetArray("StringArray"));
        REQUIRE(child2.SetString(0, "zero"));
        REQUIRE(child2.SetString(1, "one"));
        REQUIRE(child2.SetString(2, "two"));
        REQUIRE(child2.EndSetArray());

        // assure lifetime of ChildStr doesn't affect rootDoc
        {
            const std::string ChildStr = std::string("Child1");
            REQUIRE(rootDoc.AddChildDocument(ChildStr, child1));
            REQUIRE(rootDoc.AddChildDocument("Child2", child2));
            childrenDocuments = rootDoc.GetChildren();
            REQUIRE(childrenDocuments.size() == 2);

            rootDocStringBeforeScopeExit = rootDoc.ToString();
        }
    }

    const auto rootDocStringAfterScopeExit = rootDoc.ToString();
    REQUIRE(rootDocStringBeforeScopeExit == rootDocStringAfterScopeExit);

    REQUIRE(rootDoc.StartGetObject("Child1"));
        REQUIRE(rootDoc.GetInt("Int1") == 10);
        REQUIRE(rootDoc.GetInt("Int2") == 20);
        REQUIRE(rootDoc.GetInt("Int3") == 30);
    REQUIRE(rootDoc.EndGetObject());

    REQUIRE(rootDoc.StartGetObject("Child2"));
        REQUIRE(rootDoc.StartGetArray("StringArray") == 3);
            REQUIRE(rootDoc.GetString(0) == std::string("zero"));
            REQUIRE(rootDoc.GetString(1) == std::string("one"));
            REQUIRE(rootDoc.GetString(2) == std::string("two"));
        REQUIRE(rootDoc.EndGetArray());
    REQUIRE(rootDoc.EndGetObject());
}
//--------------------------------------------------------------------------

TEST_CASE("Json document add children (default constructed)", "[json][reader][writer][document]")
{
    Kmplete::JsonDocument rootDoc;
    REQUIRE_FALSE(rootDoc.HasError());
    auto childrenDocuments = rootDoc.GetChildren();
    REQUIRE(childrenDocuments.empty());

    Kmplete::JsonDocument defaultCreatedChild;
    REQUIRE(rootDoc.AddChildDocument("Child", defaultCreatedChild));
    childrenDocuments = rootDoc.GetChildren();
    REQUIRE(childrenDocuments.size() == 1);
}
//--------------------------------------------------------------------------

TEST_CASE("Json document add children (invalid child)", "[json][reader][writer][document]")
{
    Kmplete::JsonDocument rootDoc;
    REQUIRE_FALSE(rootDoc.HasError());
    auto childrenDocuments = rootDoc.GetChildren();
    REQUIRE(childrenDocuments.empty());

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

    Kmplete::JsonDocument invalidChildDoc(std::move(document));
    REQUIRE(invalidChildDoc.HasError());

    REQUIRE_FALSE(rootDoc.AddChildDocument("Child", invalidChildDoc));
    childrenDocuments = rootDoc.GetChildren();
    REQUIRE(childrenDocuments.empty());
}
//--------------------------------------------------------------------------

TEST_CASE("Json document add children (empty name)", "[json][reader][writer][document]")
{
    Kmplete::JsonDocument rootDoc;
    REQUIRE_FALSE(rootDoc.HasError());
    auto childrenDocuments = rootDoc.GetChildren();
    REQUIRE(childrenDocuments.empty());

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

    Kmplete::JsonDocument childDoc(std::move(document));
    REQUIRE_FALSE(childDoc.HasError());

    REQUIRE_FALSE(rootDoc.AddChildDocument("", childDoc));
}
//--------------------------------------------------------------------------

TEST_CASE("Json document add children (overwrite existing object)", "[json][reader][writer][document]")
{
    Kmplete::JsonDocument rootDoc;
    REQUIRE(rootDoc.StartSetObject("Obj1"));
        REQUIRE(rootDoc.SetBool("Bool", true));
        REQUIRE(rootDoc.SetInt("Int", 33));
        REQUIRE(rootDoc.SetString("String", "some string"));
    REQUIRE(rootDoc.EndSetObject());

    auto childrenDocuments = rootDoc.GetChildren();
    REQUIRE(childrenDocuments.size() == 1);

    const char* BasicJsonStr = R"rjs(
    {
        "Hello": 321,
        "Var": true,
        "Str": "string"
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(BasicJsonStr);

    Kmplete::JsonDocument childDoc(std::move(document));
    REQUIRE_FALSE(childDoc.HasError());

    REQUIRE_FALSE(rootDoc.AddChildDocument("Obj1", childDoc, false));

    REQUIRE(rootDoc.AddChildDocument("Obj1", childDoc));
    const auto rootDocStr = rootDoc.ToString();
    childrenDocuments = rootDoc.GetChildren();
    REQUIRE(childrenDocuments.size() == 1);
    REQUIRE(rootDoc.StartGetObject("Obj1"));
        REQUIRE_FALSE(rootDoc.GetBool("Bool") == true);
        REQUIRE_FALSE(rootDoc.GetInt("Int") == 33);
        REQUIRE_FALSE(rootDoc.GetString("String") == std::string("some string"));
        REQUIRE(rootDoc.GetInt("Hello") == 321);
        REQUIRE(rootDoc.GetBool("Var") == true);
        REQUIRE(rootDoc.GetString("Str") == std::string("string"));
    REQUIRE(rootDoc.EndGetObject());
}
//--------------------------------------------------------------------------

TEST_CASE("Json document add children during filling another object", "[json][reader][writer][document]")
{
    Kmplete::JsonDocument rootDoc;

    const char* BasicJsonStr = R"rjs(
    {
        "Hello": 321,
        "Var": true,
        "Str": "string"
    }
    )rjs";

    rapidjson::Document document;
    document.Parse(BasicJsonStr);

    Kmplete::JsonDocument childDoc(std::move(document));
    REQUIRE_FALSE(childDoc.HasError());

    REQUIRE(rootDoc.StartSetObject("Obj1"));
        REQUIRE(rootDoc.SetBool("Bool", true));
        REQUIRE(rootDoc.SetInt("Int", 33));
        REQUIRE(rootDoc.AddChildDocument("Obj2", childDoc)); // add child in between Obj1 set
        REQUIRE(rootDoc.SetString("String", "some string"));
    REQUIRE(rootDoc.EndSetObject());

    auto childrenDocuments = rootDoc.GetChildren();
    REQUIRE(childrenDocuments.size() == 2);
}
//--------------------------------------------------------------------------

TEST_CASE("Json document save to file and read from file", "[json][reader][writer][document]")
{
    REQUIRE(Kmplete::Filesystem::Initialize());
    const auto settingsPath = Kmplete::Filesystem::GetApplicationPath().append("json_document_test_temp.json");
    const auto settingsPathStr = Kmplete::Filesystem::ToGenericString(settingsPath);
    const auto nonExistSettingsPath = Kmplete::Filesystem::GetApplicationPath().append("non-exist.json");

    Kmplete::JsonDocument rootDoc;
    REQUIRE(rootDoc.StartSetObject("Obj1"));
        REQUIRE(rootDoc.SetBool("Bool", true));
        REQUIRE(rootDoc.SetInt("Int", 33));
        REQUIRE(rootDoc.SetString("String", "some string"));
    REQUIRE(rootDoc.EndSetObject());

    REQUIRE(rootDoc.Save(settingsPath));

    Kmplete::JsonDocument loadedDoc(settingsPath);
    REQUIRE_FALSE(loadedDoc.HasError());
    auto childrenDocuments = loadedDoc.GetChildren();
    REQUIRE(childrenDocuments.size() == 1);

    REQUIRE(loadedDoc.StartSetObject("Obj2"));
    REQUIRE(loadedDoc.EndSetObject());
    childrenDocuments = loadedDoc.GetChildren();
    REQUIRE(childrenDocuments.size() == 2);

    loadedDoc.SetFilename("");
    REQUIRE_FALSE(loadedDoc.Save());
    REQUIRE(loadedDoc.Save(settingsPathStr));

    Kmplete::JsonDocument loadedEmptyDoc;
    REQUIRE_FALSE(loadedEmptyDoc.Load());
    REQUIRE_FALSE(loadedEmptyDoc.Load(nonExistSettingsPath));
}
//--------------------------------------------------------------------------