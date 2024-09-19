#include "Kmplete/Core/application.h"
#include "Kmplete/Core/filesystem.h"

#include <catch2/catch_test_macros.hpp>

namespace Kmplete
{
    class TestApplication : public Application
    {
        KMP_NODISCARD std::string GetApplicationName() const KMP_NOEXCEPT override 
        { 
            return std::string("TestApplication"); 
        }

        void Run() override {}
    };
}
//--------------------------------------------------------------------------

TEST_CASE("Test application initialize", "[core][application]")
{
    const auto application = Kmplete::CreateUPtr<Kmplete::TestApplication>();

    REQUIRE(application);
    REQUIRE(application->Initialize("", "Kmplete_unit_tests_settings.json"));
    REQUIRE(!Kmplete::Filesystem::GetApplicationPathCRef().empty());

    application->Finalize();
    const auto settingsPath = Kmplete::Filesystem::GetApplicationPath().append("Kmplete_unit_tests_settings.json");
    REQUIRE(Kmplete::Filesystem::FilePathIsValid(settingsPath));
    REQUIRE(Kmplete::Filesystem::PathExists(settingsPath));
}
//--------------------------------------------------------------------------