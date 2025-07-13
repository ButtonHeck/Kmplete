## ~/src/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

set(KmpleteResourcesFolder ${CMAKE_CURRENT_LIST_DIR}/Resources)

if(${KMPLETE_BUILD_TESTS})
    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
endif()

add_subdirectory(VersionLib)
add_subdirectory(BaseLib)
add_subdirectory(LoggerLib)
add_subdirectory(UtilsLib)
add_subdirectory(FilesystemLib)
add_subdirectory(JsonLib)
add_subdirectory(ProfilerLib)
add_subdirectory(Engine)
add_subdirectory(Editor)
add_subdirectory(Translator)
add_subdirectory(TranslatorInterfaceLib)

if(${KMPLETE_BUILD_TESTS})
    add_custom_target(_BuildAllTests)
    add_dependencies(_BuildAllTests
        BaseLib_UnitTests
        FilesystemLib_UnitTests
        JsonLib_UnitTests
        UtilsLib_UnitTests
        Kmplete_UnitTests
        Kmplete_FileDialogsTests
        Kmplete_WindowApplicationTests
        Kmplete_LocaleTests
    )
    set_target_properties(_BuildAllTests PROPERTIES
        FOLDER Kmplete/Tests
    )
endif()