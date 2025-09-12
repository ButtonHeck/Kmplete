## ~/src/Engine/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)


# Unit Tests

set(Kmplete_UnitTests_CORE
    ${CMAKE_CURRENT_LIST_DIR}/Core/uuid_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/settings_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/settings_manager_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/assertion_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/rng_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/timer_tests.cpp
)
source_group("Core" FILES ${Kmplete_UnitTests_CORE})

set(Kmplete_UnitTests_APPLICATION
    ${CMAKE_CURRENT_LIST_DIR}/Application/application_tests.cpp
)
source_group("Application" FILES ${Kmplete_UnitTests_APPLICATION})

add_executable(Kmplete_UnitTests
    ${Kmplete_UnitTests_CORE}
    ${Kmplete_UnitTests_APPLICATION}
)


SetupBoost(Kmplete_UnitTests)

target_include_directories(Kmplete_UnitTests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"

    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    PRIVATE "${GLFW_SOURCE_DIR}/include"    
)

set(KmpleteUnitTestsCopySettingsScriptName "CopySettings$<CONFIG>.cmake")
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${KmpleteUnitTestsCopySettingsScriptName}"
    CONTENT
    [=[
    if (NOT EXISTS "$<TARGET_FILE_DIR:Kmplete_UnitTests>/Kmplete_settings_unit_tests.json")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E copy
        "${resources_folder}/json/Kmplete_settings_tests_example.json"
        "$<TARGET_FILE_DIR:Kmplete_UnitTests>/Kmplete_settings_unit_tests.json")
    endif() 
    ]=]
)

add_custom_command(
    TARGET Kmplete_UnitTests
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -D resources_folder=${KmpleteResourcesFolder} -P "${KmpleteUnitTestsCopySettingsScriptName}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

SetupUnitTestsTargetProperties(
    Kmplete_UnitTests 
    Kmplete/Tests
    Catch2::Catch2WithMain Kmplete
)


# Window Application Tests

set(Kmplete_WindowApplicationTests_WINDOW
    ${CMAKE_CURRENT_LIST_DIR}/Window/window_backend_tests.cpp
)
source_group("Window" FILES ${Kmplete_WindowApplicationTests_WINDOW})

set(Kmplete_WindowApplicationTests_APPLICATION
    ${CMAKE_CURRENT_LIST_DIR}/Application/window_application_tests.cpp
)
source_group("Application" FILES ${Kmplete_WindowApplicationTests_APPLICATION})

add_executable(Kmplete_WindowApplicationTests
    ${Kmplete_WindowApplicationTests_WINDOW}
    ${Kmplete_WindowApplicationTests_APPLICATION}
)


target_include_directories(Kmplete_WindowApplicationTests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"
    
    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    PRIVATE "${GLFW_SOURCE_DIR}/include"
)

set(KmpleteWindowApplicationTestsSettingsFile "Kmplete_window_application_tests_settings.json")
target_compile_definitions(Kmplete_WindowApplicationTests
    PRIVATE KMP_TEST_ICON_PATH=\"${KmpleteResourcesFolder}/icons/test_icon64.png\"
    PRIVATE KMP_TEST_SETTINGS_JSON=\"${KmpleteWindowApplicationTestsSettingsFile}\"
)

set(KmpleteWindowAppTestsCopySettingsScriptName "CopyWindowTestsSettings$<CONFIG>.cmake")
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${KmpleteWindowAppTestsCopySettingsScriptName}"
    CONTENT
    [=[
    if (NOT EXISTS "$<TARGET_FILE_DIR:Kmplete_WindowApplicationTests>/${KmpleteWindowApplicationTestsSettingsFile}")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E copy
        "${resources_folder}/json/Kmplete_settings_example.json"
        "$<TARGET_FILE_DIR:Kmplete_WindowApplicationTests>/${KmpleteWindowApplicationTestsSettingsFile}")
    endif() 
    ]=]
)

add_custom_command(
    TARGET Kmplete_WindowApplicationTests
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -D resources_folder=${KmpleteResourcesFolder} -P "${KmpleteWindowAppTestsCopySettingsScriptName}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

SetupUnitTestsTargetProperties(
    Kmplete_WindowApplicationTests 
    Kmplete/Tests
    Catch2::Catch2WithMain Kmplete FileDialogsLib
)


# Locale tests

set(Kmplete_LocaleTests_LOCALIZATION
    ${CMAKE_CURRENT_LIST_DIR}/Localization/localization_manager_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Localization/localization_base_tests.cpp
)
source_group("Localization" FILES ${Kmplete_LocaleTests_LOCALIZATION})

add_executable(Kmplete_LocaleTests
    ${Kmplete_LocaleTests_LOCALIZATION}
)


target_include_directories(Kmplete_LocaleTests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"
    PUBLIC "${spdlog_SOURCE_DIR}/include"
)


# Locale tests - localization

set(KmpleteTestsDomain "Tests")
target_compile_definitions(Kmplete_LocaleTests
    PRIVATE KMP_TR_DOMAIN_TESTS=\"${KmpleteTestsDomain}\"
)
get_target_property(exeDir Kmplete_LocaleTests RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(Kmplete_LocaleTests_copy_translations
    COMMAND ${CMAKE_COMMAND} -E echo "Copying translations for Kmplete_LocaleTests done"
)
add_custom_command(TARGET Kmplete_LocaleTests_copy_translations PRE_BUILD
    COMMAND ${CMAKE_COMMAND}
    "-D DEST_DIR=${exeDir}" "-D SRC_DIRS=${CMAKE_CURRENT_LIST_DIR}/locale" 
    -P "${CMAKE_SOURCE_DIR}/cmake/CopyTranslations.cmake"
)
set_target_properties(Kmplete_LocaleTests_copy_translations PROPERTIES
    FOLDER Kmplete/Tests
)

add_dependencies(Kmplete_LocaleTests Kmplete_LocaleTests_copy_translations)

CreateTranslationHelperTargets(
    Kmplete_LocaleTests 
    Kmplete/Tests
    "--mode=Update --input_directories=\"${CMAKE_CURRENT_LIST_DIR}/Localization\" --extensions=.cpp .h --recursive --output_directory=\"${CMAKE_CURRENT_LIST_DIR}/locale\" --output_file_name=${KmpleteTestsDomain}"
    "--mode=Compile --output_directory=\"${CMAKE_CURRENT_LIST_DIR}/locale\" --output_file_name=${KmpleteTestsDomain}"
)

SetupUnitTestsTargetProperties(
    Kmplete_LocaleTests 
    Kmplete/Tests
    Catch2::Catch2WithMain Kmplete
)
