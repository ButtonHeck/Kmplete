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

add_subdirectory(VersionLib)
add_subdirectory(BaseLib)
add_subdirectory(LoggerLib)
add_subdirectory(UtilsLib)
add_subdirectory(FilesystemLib)
add_subdirectory(FileDialogsLib)
add_subdirectory(JsonLib)
add_subdirectory(MathLib)
add_subdirectory(ProfilerLib)
add_subdirectory(Engine)
add_subdirectory(ImGuiUtilsLib)
add_subdirectory(Editor)
add_subdirectory(Translator)
add_subdirectory(TranslatorInterfaceLib)
add_subdirectory(AssetsCompiler)
add_subdirectory(AssetsInterfaceLib)
add_subdirectory(AssetsCompilerInterfaceLib)
add_subdirectory(TimeLib)

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