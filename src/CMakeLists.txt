## ~/src/CMakeLists.txt

############################################################################
# Main CMakeLists file for all Kmplete targets and its' tests. All main targets
# are placed into "Kmplete/[Type]" folder, where [Type] is either "Libraries" or
# "Tools" depending on the target purpose.
# All test targets are placed into "Kmplete/Tests" folder and additional
# "_BuildAllTests" target added to build all tests at once
############################################################################

cmake_minimum_required(VERSION 3.20)

set(KmpleteResourcesFolder ${CMAKE_CURRENT_LIST_DIR}/Resources)
set(KmpleteInstallEditorComponentName KmpleteInstallEditor)

if(${KMPLETE_BUILD_TESTS})
    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
endif()

add_subdirectory(Libraries/VersionLib)
add_subdirectory(Libraries/BaseLib)
add_subdirectory(Libraries/LoggerLib)
add_subdirectory(Libraries/UtilsLib)
add_subdirectory(Libraries/FilesystemLib)
add_subdirectory(Libraries/FileDialogsLib)
add_subdirectory(Libraries/JsonLib)
add_subdirectory(Libraries/MathLib)
add_subdirectory(Libraries/ProfilerLib)
add_subdirectory(Libraries/Engine)
add_subdirectory(Libraries/ImGuiUtilsLib)
add_subdirectory(Libraries/TranslatorInterfaceLib)
add_subdirectory(Libraries/AssetsInterfaceLib)
add_subdirectory(Libraries/AssetsCompilerInterfaceLib)
add_subdirectory(Libraries/TimeLib)

add_subdirectory(Tools/Editor)
add_subdirectory(Tools/Translator)
add_subdirectory(Tools/AssetsCompiler)

add_subdirectory(Sandboxes/TriangleOpenGL)
add_subdirectory(Sandboxes/TriangleVulkan)

if(${KMPLETE_BUILD_TESTS})
    add_custom_target(_BuildAllTests)
    add_dependencies(_BuildAllTests
        BaseLib_UnitTests
        FilesystemLib_UnitTests
        FileDialogsLib_UnitTests
        JsonLib_UnitTests
        UtilsLib_UnitTests
        MathLib_UnitTests
        TimeLib_UnitTests

        Kmplete_UnitTests
        Kmplete_WindowApplicationTests
        Kmplete_LocaleTests
        TestWindowApplication
    )
    set_target_properties(_BuildAllTests PROPERTIES
        FOLDER Kmplete/Tests
    )
endif()