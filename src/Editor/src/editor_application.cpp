#include "editor_application.h"
#include "Kmplete/Core/entry_point.h"

namespace Kmplete
{
    Application* CreateApplication()
    {
        return new EditorApplication();
    }
    //--------------------------------------------------------------------------

    EditorApplication::EditorApplication()
        : WindowApplication()
    {}
    //--------------------------------------------------------------------------

    EditorApplication::~EditorApplication()
    {
    }
    //--------------------------------------------------------------------------

    std::string EditorApplication::GetApplicationName() const
    {
        return std::string("Kmplete Editor");
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::Initialize(const std::string& configPath)
    {
        if (!WindowApplication::Initialize(configPath))
        {
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Run()
    {
    }
    //--------------------------------------------------------------------------
}