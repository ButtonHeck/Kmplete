## ~/cmake/Vulkan.cmake

############################################################################
# CMake module for finding Vulkan and its components
############################################################################

find_package(Vulkan REQUIRED COMPONENTS
    glslc
    shaderc_combined
)
