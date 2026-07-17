## ~/src/Sandboxes/CMakeLists.txt

############################################################################
# CMakeLists for all sandbox applications
############################################################################

cmake_minimum_required(VERSION 3.24)

set(KmpleteSandboxResourcesFolder ${CMAKE_CURRENT_LIST_DIR}/Resources)

add_subdirectory(Triangle)
add_subdirectory(Texture)
add_subdirectory(MultiplePipelines)
add_subdirectory(InstancedRendering)
add_subdirectory(UniformBuffers)
add_subdirectory(StorageBuffers)
add_subdirectory(PushConstants)
add_subdirectory(DrawIndirect)
add_subdirectory(PostProcessing)