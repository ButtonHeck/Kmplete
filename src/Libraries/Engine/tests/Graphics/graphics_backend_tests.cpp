#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/OpenGL/opengl_graphics_backend.h"
#include "Kmplete/Window/window_backend.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Base/pointers.h"

#include <catch2/catch_test_macros.hpp>
#include <GLFW/glfw3.h>


using namespace Kmplete;
using namespace Kmplete::Graphics;


TEST_CASE("Graphics backend initialization error type", "[graphics]")
{
    auto windowBackend = Kmplete::WindowBackend::Create();
    windowBackend->SetGraphicsBackendType(GraphicsBackendType::Unknown);
    auto& mainWindow = windowBackend->CreateMainWindow();

    UPtr<GraphicsBackend> backend;
    backend = GraphicsBackend::Create(mainWindow);
    REQUIRE(backend == nullptr);
}
//--------------------------------------------------------------------------

TEST_CASE("Graphics backend initialization opengl", "[graphics]")
{
    auto windowBackend = Kmplete::WindowBackend::Create();
    windowBackend->SetGraphicsBackendType(GraphicsBackendType::OpenGL);
    auto& mainWindow = windowBackend->CreateMainWindow();

    UPtr<GraphicsBackend> backend;
    REQUIRE_NOTHROW(backend = GraphicsBackend::Create(mainWindow));
    REQUIRE(backend);
}
//--------------------------------------------------------------------------