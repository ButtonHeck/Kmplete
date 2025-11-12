#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/OpenGL/opengl_graphics_backend.h"
#include "Kmplete/Base/pointers.h"

#include <catch2/catch_test_macros.hpp>
#include <GLFW/glfw3.h>


using namespace Kmplete;

TEST_CASE("Graphics backend initialization error type", "[graphics]")
{
    UPtr<GraphicsBackend> backend;
    backend = GraphicsBackend::Create(GraphicsBackendType::Unknown);
    REQUIRE(backend == nullptr);
}
//--------------------------------------------------------------------------

TEST_CASE("Graphics backend initialization opengl", "[graphics]")
{
    REQUIRE(glfwInit());

    UPtr<GraphicsBackend> backend;
    REQUIRE_THROWS(backend = GraphicsBackend::Create(GraphicsBackendType::OpenGL));

    const auto window = glfwCreateWindow(100, 100, "", nullptr, nullptr);
    REQUIRE(window);
    glfwMakeContextCurrent(window);

    REQUIRE_NOTHROW(backend = GraphicsBackend::Create(GraphicsBackendType::OpenGL));
    REQUIRE(backend);
}
//--------------------------------------------------------------------------