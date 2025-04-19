## ~/src/Engine/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)


# Unit Tests

set(Kmplete_UnitTests_CORE
    ${CMAKE_CURRENT_LIST_DIR}/Core/platform_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/filesystem_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/uuid_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/settings_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/settings_manager_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/macro_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/assertion_tests.cpp
)
source_group("Core" FILES ${Kmplete_UnitTests_CORE})

set(Kmplete_UnitTests_APPLICATION
    ${CMAKE_CURRENT_LIST_DIR}/Application/application_tests.cpp
)
source_group("Application" FILES ${Kmplete_UnitTests_APPLICATION})

set(Kmplete_UnitTests_UTILS
    ${CMAKE_CURRENT_LIST_DIR}/Utils/function_utils_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/string_utils_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/string_id_tests.cpp
)
source_group("Utils" FILES ${Kmplete_UnitTests_UTILS})

set(Kmplete_UnitTests_JSON
    ${CMAKE_CURRENT_LIST_DIR}/Json/json_reader_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Json/json_reader_writer_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Json/json_writer_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Json/json_document_tests.cpp
)
source_group("Json" FILES ${Kmplete_UnitTests_JSON})

add_executable(Kmplete_UnitTests
    ${Kmplete_UnitTests_CORE}
    ${Kmplete_UnitTests_APPLICATION}
    ${Kmplete_UnitTests_UTILS}
    ${Kmplete_UnitTests_JSON}
)


SetupBoost(Kmplete_UnitTests)
SetupCompilerOptions(Kmplete_UnitTests)

target_include_directories(Kmplete_UnitTests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"

    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    PRIVATE "${GLFW_SOURCE_DIR}/include"    
)

target_link_libraries(Kmplete_UnitTests
    PRIVATE Kmplete
    PRIVATE Catch2::Catch2WithMain
)

set_target_properties(Kmplete_UnitTests PROPERTIES 
    FOLDER Kmplete/Engine/Tests
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


# File Dialogs Tests

set(Kmplete_FileDialogsTests_CORE
    ${CMAKE_CURRENT_LIST_DIR}/Core/file_dialogs_tests.cpp
)
source_group("Core" FILES ${Kmplete_FileDialogsTests_CORE})

add_executable(Kmplete_FileDialogsTests
    ${Kmplete_FileDialogsTests_CORE}
)


SetupCompilerOptions(Kmplete_FileDialogsTests)

target_include_directories(Kmplete_FileDialogsTests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"
    
    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    PRIVATE "${portable_file_dialogs_SOURCE_DIR}"
)

target_link_libraries(Kmplete_FileDialogsTests
    PRIVATE Kmplete
    PRIVATE Catch2::Catch2WithMain
)

set_target_properties(Kmplete_FileDialogsTests PROPERTIES 
    FOLDER Kmplete/Engine/Tests
)


# Window Application Tests

set(Kmplete_WindowApplicationTests_CORE
    ${CMAKE_CURRENT_LIST_DIR}/Core/window_backend_tests.cpp
)
source_group("Core" FILES ${Kmplete_WindowApplicationTests_CORE})

set(Kmplete_WindowApplicationTests_APPLICATION
    ${CMAKE_CURRENT_LIST_DIR}/Application/window_application_tests.cpp
)
source_group("Application" FILES ${Kmplete_WindowApplicationTests_APPLICATION})

add_executable(Kmplete_WindowApplicationTests
    ${Kmplete_WindowApplicationTests_CORE}
    ${Kmplete_WindowApplicationTests_APPLICATION}
)


SetupCompilerOptions(Kmplete_WindowApplicationTests)

target_include_directories(Kmplete_WindowApplicationTests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"
    
    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    PRIVATE "${GLFW_SOURCE_DIR}/include"
)

target_link_libraries(Kmplete_WindowApplicationTests
    PRIVATE Kmplete
    PRIVATE Catch2::Catch2WithMain
)

set(KmpleteWindowApplicationTestsSettingsFile "Kmplete_window_application_tests_settings.json")
target_compile_definitions(Kmplete_WindowApplicationTests
    PRIVATE KMP_TEST_ICON_PATH=\"${KmpleteResourcesFolder}/icons/test_icon64.png\"
    PRIVATE KMP_TEST_SETTINGS_JSON=\"${KmpleteWindowApplicationTestsSettingsFile}\"
)

set_target_properties(Kmplete_WindowApplicationTests PROPERTIES 
    FOLDER Kmplete/Engine/Tests
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


# Locale tests

set(Kmplete_LocaleTests_LOCALIZATION
    ${CMAKE_CURRENT_LIST_DIR}/Localization/localization_manager_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Localization/localization_base_tests.cpp
)
source_group("Localization" FILES ${Kmplete_LocaleTests_LOCALIZATION})

add_executable(Kmplete_LocaleTests
    ${Kmplete_LocaleTests_LOCALIZATION}
)


SetupCompilerOptions(Kmplete_LocaleTests)

target_include_directories(Kmplete_LocaleTests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"
    
    PUBLIC "${spdlog_SOURCE_DIR}/include"
)

target_link_libraries(Kmplete_LocaleTests
    PRIVATE Kmplete
    PRIVATE Catch2::Catch2WithMain
)

set_target_properties(Kmplete_LocaleTests PROPERTIES 
    FOLDER Kmplete/Engine/Tests/Localization
)

# Locale tests - localization

set(KmpleteTestsDomain "Tests")
target_compile_definitions(Kmplete_LocaleTests
    PRIVATE KMP_TR_DOMAIN_TESTS=\"${KmpleteTestsDomain}\"
)
get_target_property(exeDir Kmplete_LocaleTests RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(Kmplete_LocaleTests_copy_translations
    COMMAND ${CMAKE_COMMAND} -E echo "Copying translations for Kmplete_Editor done"
)
add_custom_command(TARGET Kmplete_LocaleTests_copy_translations PRE_BUILD
    COMMAND ${CMAKE_COMMAND}
    "-D DEST_DIR=${exeDir}" "-D SRC_DIRS=${CMAKE_CURRENT_LIST_DIR}/locale" 
    -P "${CMAKE_SOURCE_DIR}/cmake/CopyTranslations.cmake"
)
set_target_properties(Kmplete_LocaleTests_copy_translations PROPERTIES
    FOLDER Kmplete/Engine/Tests/Localization
)

add_dependencies(Kmplete_LocaleTests Kmplete_LocaleTests_copy_translations)

CreateTranslationHelperTargets(Kmplete_LocaleTests Kmplete/Engine/Tests/Localization po_update.sh po_compile.sh ${KmpleteTestsDomain} ${CMAKE_CURRENT_LIST_DIR}/scripts)


# Common stuff

add_custom_target(BuildAllTests)
add_dependencies(BuildAllTests
    Kmplete_UnitTests
    Kmplete_FileDialogsTests
    Kmplete_WindowApplicationTests
    Kmplete_LocaleTests
)
set_target_properties(BuildAllTests PROPERTIES
    FOLDER Kmplete/Engine/Tests
)

if(MSVC)
    set_target_properties(Kmplete_UnitTests PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
    set_target_properties(Kmplete_FileDialogsTests PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
    set_target_properties(Kmplete_WindowApplicationTests PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
    set_target_properties(Kmplete_LocaleTests PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(Kmplete_UnitTests)
catch_discover_tests(Kmplete_FileDialogsTests)
catch_discover_tests(Kmplete_WindowApplicationTests)
catch_discover_tests(Kmplete_LocaleTests)
