#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Core/filesystem.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("SettingsManager basic test", "[core][json][settings][manager]")
{
    REQUIRE(Kmplete::Filesystem::Initialize());

    const auto settingsPath = Kmplete::Filesystem::GetApplicationPath().append("Kmplete_settings_tests.json");
    Kmplete::SettingsManager settingsManager(settingsPath);

    REQUIRE(!settingsManager.GetFilename().empty());
    REQUIRE(settingsManager.Initialize());

    auto settings = settingsManager.GetSettings("NonExisting");
    REQUIRE_FALSE(settings);

    settings = settingsManager.GetSettings("ObjA");
    REQUIRE(settings);
    REQUIRE(settings.use_count() == 2); // one inside settingsManager and settings ptr from GetSettings("ObjA")
    REQUIRE(settings->GetInt("PropA") == 12);

    settings = settingsManager.GetSettings("ObjB");
    REQUIRE(settings);
    REQUIRE(settings.use_count() == 2);
    REQUIRE(settings->GetInt("PropA") == 0);
    REQUIRE(settings->GetBool("PropB") == true);

    settings = settingsManager.GetSettings("ObjC");
    REQUIRE(settings);
    REQUIRE(settings.use_count() == 2);
    REQUIRE(settings->GetString("PropA") == std::string("hello"));
    REQUIRE(settings->GetDouble("PropB") == -44.55);
    REQUIRE(settings->GetInt("PropC") == 8);
}
//--------------------------------------------------------------------------

TEST_CASE("SettingsManager read/write and back", "[core][json][settings][manager]")
{
    REQUIRE(Kmplete::Filesystem::Initialize());

    const auto settingsPath = Kmplete::Filesystem::GetApplicationPath().append("Kmplete_settings_tests.json");
    Kmplete::SettingsManager settingsManager(settingsPath);

    REQUIRE(!settingsManager.GetFilename().empty());
    REQUIRE(settingsManager.Initialize());

    auto settings = settingsManager.GetSettings("ObjA");
    REQUIRE(settings->StartSaveObject());
    REQUIRE(settings->SaveInt("PropA", 999));
    REQUIRE(settings->EndSaveObject());

    const auto swapSettingsPath = Kmplete::Filesystem::GetApplicationPath().append("Kmplete_settings_tests_swap.json");
    settingsManager.SetFilename(swapSettingsPath);
    REQUIRE(settingsManager.SaveSettings());


    Kmplete::SettingsManager swapSettingsManager(swapSettingsPath);
    REQUIRE(swapSettingsManager.Initialize());

    settings = swapSettingsManager.GetSettings("ObjA");
    REQUIRE(settings);
    REQUIRE(settings.use_count() == 2);
    REQUIRE(settings->GetInt("PropA") == 999);

    settings = swapSettingsManager.GetSettings("ObjB");
    REQUIRE_FALSE(settings);
}
//--------------------------------------------------------------------------