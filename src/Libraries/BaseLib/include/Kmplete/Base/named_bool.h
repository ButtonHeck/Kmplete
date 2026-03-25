#pragma once

namespace Kmplete
{
    //! Reader-friendly syntactic sugar for declaring "false" values with a description,
    //! e.g. a code line below
    //! ImGuiUtils::ContextVulkan(_mainWindow.GetImplPointer(), GraphicsBackendTypeToString(_graphicsBackend.GetType()), true, false, initInfo);
    //! may be rewritten as
    //! ImGuiUtils::ContextVulkan(_mainWindow.GetImplPointer(), GraphicsBackendTypeToString(_graphicsBackend.GetType()), "docking"_true, "viewports"_false, initInfo);
    consteval bool operator ""_false(const char*, std::size_t)
    {
        return false;
    }
    //--------------------------------------------------------------------------

    //! Same as ""_false but for "true" values
    consteval bool operator ""_true(const char*, std::size_t)
    {
        return true;
    }
    //--------------------------------------------------------------------------
}