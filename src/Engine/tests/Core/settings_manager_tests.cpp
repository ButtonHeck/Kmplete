#include "Kmplete/Core/settings_manager.h"
#include "Kmplete/Filesystem/filesystem.h"

#include <catch2/catch_test_macros.hpp>


TEST_CASE("SettingsManager basic test", "[core][settings][manager]")
{
    const auto settingsFilepath = Kmplete::Filesystem::GetCurrentFilepath().append("Kmplete_settings_unit_tests.json");
    Kmplete::SettingsManager settingsManager(settingsFilepath);

    REQUIRE(!settingsManager.GetFilepath().empty());
    REQUIRE(settingsManager.LoadSettings());

    auto settings = settingsManager.GetSettings("NonExisting");
    REQUIRE_FALSE(settings);

    settings = settingsManager.GetSettings("ObjA");
    REQUIRE(settings);
    REQUIRE(settings->get().GetInt("PropA") == 12);

    settings = settingsManager.GetSettings("ObjB");
    REQUIRE(settings);
    REQUIRE(settings->get().GetInt("PropA") == 0);
    REQUIRE(settings->get().GetBool("PropB") == true);

    settings = settingsManager.GetSettings("ObjC");
    REQUIRE(settings);
    REQUIRE(settings->get().GetString("PropA") == "hello");
    REQUIRE(settings->get().GetDouble("PropB") == -44.55);
    REQUIRE(settings->get().GetInt("PropC") == 8);

    REQUIRE(settingsManager.SaveSettings());
}
//--------------------------------------------------------------------------

TEST_CASE("SettingsManager read/write and back", "[core][settings][manager]")
{
    const auto settingsFilepath = Kmplete::Filesystem::GetCurrentFilepath().append("Kmplete_settings_unit_tests.json");
    Kmplete::SettingsManager settingsManager(settingsFilepath);

    REQUIRE(!settingsManager.GetFilepath().empty());
    REQUIRE(settingsManager.LoadSettings());

    auto settings = settingsManager.GetSettings("ObjA");
    REQUIRE(settings);
    REQUIRE(settings->get().SaveInt("PropA", 999));

    const auto swapSettingsPath = Kmplete::Filesystem::GetCurrentFilepath().append("Kmplete_settings_unit_tests_swap.json");
    settingsManager.SetFilepath(swapSettingsPath);
    REQUIRE(settingsManager.SaveSettings());


    Kmplete::SettingsManager swapSettingsManager(swapSettingsPath);
    REQUIRE(!swapSettingsManager.GetFilepath().empty());
    REQUIRE(swapSettingsManager.LoadSettings());

    settings = swapSettingsManager.GetSettings("ObjA");
    REQUIRE(settings);
    REQUIRE(settings->get().GetInt("PropA") == 999);

    settings = swapSettingsManager.GetSettings("ObjB");
    REQUIRE(settings); // didn't change ObjB but expect it to be in swapSettings json file
}
//--------------------------------------------------------------------------