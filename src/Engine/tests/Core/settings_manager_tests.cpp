#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Core/filesystem.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("SettingsManager basic test", "[core][settings][manager]")
{
    REQUIRE(Kmplete::Filesystem::Initialize());

    const auto settingsPath = Kmplete::Filesystem::GetApplicationPath().append("Kmplete_settings_unit_tests.json");
    Kmplete::SettingsManager settingsManager(settingsPath);

    REQUIRE(!settingsManager.GetFilename().empty());
    REQUIRE(settingsManager.LoadSettings());

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

    REQUIRE(settingsManager.SaveSettings());
}
//--------------------------------------------------------------------------

TEST_CASE("SettingsManager read/write and back", "[core][settings][manager]")
{
    REQUIRE(Kmplete::Filesystem::Initialize());

    const auto settingsPath = Kmplete::Filesystem::GetApplicationPath().append("Kmplete_settings_unit_tests.json");
    Kmplete::SettingsManager settingsManager(settingsPath);

    REQUIRE(!settingsManager.GetFilename().empty());
    REQUIRE(settingsManager.LoadSettings());

    auto settings = settingsManager.GetSettings("ObjA");
    REQUIRE(settings->SaveInt("PropA", 999));

    const auto swapSettingsPath = Kmplete::Filesystem::GetApplicationPath().append("Kmplete_settings_unit_tests_swap.json");
    settingsManager.SetFilename(swapSettingsPath);
    REQUIRE(settingsManager.SaveSettings());


    Kmplete::SettingsManager swapSettingsManager(swapSettingsPath);
    REQUIRE(!swapSettingsManager.GetFilename().empty());
    REQUIRE(swapSettingsManager.LoadSettings());

    settings = swapSettingsManager.GetSettings("ObjA");
    REQUIRE(settings);
    REQUIRE(settings.use_count() == 2);
    REQUIRE(settings->GetInt("PropA") == 999);

    settings = swapSettingsManager.GetSettings("ObjB");
    REQUIRE(settings); // didn't change ObjB but expect it to be in swapSettings json file
}
//--------------------------------------------------------------------------