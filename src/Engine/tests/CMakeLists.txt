## ~/src/Engine/tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)


# Unit Tests

set(Kmplete_Engine_Unit_Tests_CORE
    ${CMAKE_CURRENT_LIST_DIR}/Core/platform_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/filesystem_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/uuid_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/settings_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/settings_manager_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/macro_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Core/assertion_tests.cpp
)
source_group("Core" FILES ${Kmplete_Engine_Unit_Tests_CORE})

set(Kmplete_Engine_Unit_Tests_APPLICATION
    ${CMAKE_CURRENT_LIST_DIR}/Application/application_tests.cpp
)
source_group("Application" FILES ${Kmplete_Engine_Unit_Tests_APPLICATION})

set(Kmplete_Engine_Unit_Tests_UTILS
    ${CMAKE_CURRENT_LIST_DIR}/Utils/function_utils_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/string_utils_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/string_id_tests.cpp
)
source_group("Utils" FILES ${Kmplete_Engine_Unit_Tests_UTILS})

set(Kmplete_Engine_Unit_Tests_JSON
    ${CMAKE_CURRENT_LIST_DIR}/Json/json_reader_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Json/json_reader_writer_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Json/json_writer_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Json/json_document_tests.cpp
)
source_group("Json" FILES ${Kmplete_Engine_Unit_Tests_JSON})

add_executable(Kmplete_Engine_Unit_Tests
    ${Kmplete_Engine_Unit_Tests_CORE}
    ${Kmplete_Engine_Unit_Tests_APPLICATION}
    ${Kmplete_Engine_Unit_Tests_UTILS}
    ${Kmplete_Engine_Unit_Tests_JSON}
)


SetupBoost(Kmplete_Engine_Unit_Tests)
SetupCompilerOptions(Kmplete_Engine_Unit_Tests)

target_include_directories(Kmplete_Engine_Unit_Tests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"

    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    PRIVATE "${GLFW_SOURCE_DIR}/include"    
)

target_link_libraries(Kmplete_Engine_Unit_Tests
    PRIVATE Kmplete_Engine
    PRIVATE Catch2::Catch2WithMain
)

set_target_properties(Kmplete_Engine_Unit_Tests PROPERTIES 
    FOLDER Kmplete/Engine/Tests
)

set(KmpleteUnitTestsCopySettingsScriptName "CopySettings$<CONFIG>.cmake")
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${KmpleteUnitTestsCopySettingsScriptName}"
    CONTENT
    [=[
    if (NOT EXISTS "$<TARGET_FILE_DIR:Kmplete_Engine_Unit_Tests>/Kmplete_settings_unit_tests.json")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E copy
        "${resources_folder}/json/Kmplete_settings_tests_example.json"
        "$<TARGET_FILE_DIR:Kmplete_Engine_Unit_Tests>/Kmplete_settings_unit_tests.json")
    endif() 
    ]=]
)

add_custom_command(
    TARGET Kmplete_Engine_Unit_Tests
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -D resources_folder=${KmpleteResourcesFolder} -P "${KmpleteUnitTestsCopySettingsScriptName}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)


# File Dialogs Tests

set(Kmplete_Engine_File_Dialogs_Tests_CORE
    ${CMAKE_CURRENT_LIST_DIR}/Core/file_dialogs_tests.cpp
)
source_group("Core" FILES ${Kmplete_Engine_File_Dialogs_Tests_CORE})

add_executable(Kmplete_Engine_File_Dialogs_Tests
    ${Kmplete_Engine_File_Dialogs_Tests_CORE}
)


SetupCompilerOptions(Kmplete_Engine_File_Dialogs_Tests)

target_include_directories(Kmplete_Engine_File_Dialogs_Tests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"
    
    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    PRIVATE "${portable_file_dialogs_SOURCE_DIR}"
)

target_link_libraries(Kmplete_Engine_File_Dialogs_Tests
    PRIVATE Kmplete_Engine
    PRIVATE Catch2::Catch2WithMain
)

set_target_properties(Kmplete_Engine_File_Dialogs_Tests PROPERTIES 
    FOLDER Kmplete/Engine/Tests
)


# Window Application Tests

set(Kmplete_Engine_Window_Application_Tests_CORE
    ${CMAKE_CURRENT_LIST_DIR}/Core/window_backend_tests.cpp
)
source_group("Core" FILES ${Kmplete_Engine_Window_Application_Tests_CORE})

set(Kmplete_Engine_Window_Application_Tests_APPLICATION
    ${CMAKE_CURRENT_LIST_DIR}/Application/window_application_tests.cpp
)
source_group("Application" FILES ${Kmplete_Engine_Window_Application_Tests_APPLICATION})

add_executable(Kmplete_Engine_Window_Application_Tests
    ${Kmplete_Engine_Window_Application_Tests_CORE}
    ${Kmplete_Engine_Window_Application_Tests_APPLICATION}
)


SetupCompilerOptions(Kmplete_Engine_Window_Application_Tests)

target_include_directories(Kmplete_Engine_Window_Application_Tests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"
    
    PUBLIC "${spdlog_SOURCE_DIR}/include"
    PUBLIC "${RapidJSON_SOURCE_DIR}/include"
    PRIVATE "${GLFW_SOURCE_DIR}/include"
)

target_link_libraries(Kmplete_Engine_Window_Application_Tests
    PRIVATE Kmplete_Engine
    PRIVATE Catch2::Catch2WithMain
)

target_compile_definitions(Kmplete_Engine_Window_Application_Tests
    PRIVATE KMP_TEST_ICON_PATH=\"${KmpleteResourcesFolder}/icons/test_icon64.png\"
)

set_target_properties(Kmplete_Engine_Window_Application_Tests PROPERTIES 
    FOLDER Kmplete/Engine/Tests
)


# Locale tests

set(Kmplete_Engine_Locale_Tests_LOCALIZATION
    ${CMAKE_CURRENT_LIST_DIR}/Localization/localization_manager_tests.cpp
)
source_group("Localization" FILES ${Kmplete_Engine_Locale_Tests_LOCALIZATION})

add_executable(Kmplete_Engine_Locale_Tests
    ${Kmplete_Engine_Locale_Tests_LOCALIZATION}
)


SetupCompilerOptions(Kmplete_Engine_Locale_Tests)

target_include_directories(Kmplete_Engine_Locale_Tests
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include"
    
    PUBLIC "${spdlog_SOURCE_DIR}/include"
)

target_link_libraries(Kmplete_Engine_Locale_Tests
    PRIVATE Kmplete_Engine
    PRIVATE Catch2::Catch2WithMain
)

set_target_properties(Kmplete_Engine_Locale_Tests PROPERTIES 
    FOLDER Kmplete/Engine/Tests/Localization
)

# Locale tests - localization

set(KmpleteTestsDomain "Tests")
target_compile_definitions(Kmplete_Engine_Locale_Tests
    PRIVATE KMP_TR_DOMAIN_TESTS=\"${KmpleteTestsDomain}\"
)
get_target_property(exeDir Kmplete_Engine_Locale_Tests RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(Kmplete_Engine_Locale_Tests_copy_translations
    COMMAND ${CMAKE_COMMAND} -E echo "Copying translations for Kmplete_Editor done"
)
add_custom_command(TARGET Kmplete_Engine_Locale_Tests_copy_translations PRE_BUILD
    COMMAND ${CMAKE_COMMAND}
    "-D DEST_DIR=${exeDir}" "-D SRC_DIRS=${CMAKE_CURRENT_LIST_DIR}/locale" 
    -P "${CMAKE_SOURCE_DIR}/cmake/CopyTranslations.cmake"
)
set_target_properties(Kmplete_Engine_Locale_Tests_copy_translations PROPERTIES
    FOLDER Kmplete/Engine/Tests/Localization
)

add_dependencies(Kmplete_Engine_Locale_Tests Kmplete_Engine_Locale_Tests_copy_translations)

CreateTranslationHelperTargets(Kmplete_Engine_Locale_Tests Kmplete/Engine/Tests/Localization po_update.sh po_compile.sh ${KmpleteTestsDomain} ${CMAKE_CURRENT_LIST_DIR}/scripts)


# Common stuff

add_custom_target(Kmplete_Engine_build_all_tests)
add_dependencies(Kmplete_Engine_build_all_tests
    Kmplete_Engine_Unit_Tests
    Kmplete_Engine_File_Dialogs_Tests
    Kmplete_Engine_Window_Application_Tests
    Kmplete_Engine_Locale_Tests
)
set_target_properties(Kmplete_Engine_build_all_tests PROPERTIES
    FOLDER Kmplete/Engine/Tests
)

if(MSVC)
    set_target_properties(Kmplete_Engine_Unit_Tests PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
    set_target_properties(Kmplete_Engine_File_Dialogs_Tests PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
    set_target_properties(Kmplete_Engine_Window_Application_Tests PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
    set_target_properties(Kmplete_Engine_Locale_Tests PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>
    )
endif()

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(Kmplete_Engine_Unit_Tests)
catch_discover_tests(Kmplete_Engine_File_Dialogs_Tests)
catch_discover_tests(Kmplete_Engine_Window_Application_Tests)
catch_discover_tests(Kmplete_Engine_Locale_Tests)
