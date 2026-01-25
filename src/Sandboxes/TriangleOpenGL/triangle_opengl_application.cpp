#include "triangle_opengl_application.h"


namespace Kmplete
{
    TriangleOpenGLApplication::TriangleOpenGLApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    TriangleOpenGLApplication::~TriangleOpenGLApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void TriangleOpenGLApplication::_Initialize()
    {
        _mainWindow.SetTitle(_applicationName.c_str());

        _mainFrameListener.reset(new MainFrameListener(*_frameListenerManager.get(), _mainWindow));

        _LoadSettings();
    }
    //--------------------------------------------------------------------------

    void TriangleOpenGLApplication::_Finalize()
    {
        _SaveSettings();

        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------

    void TriangleOpenGLApplication::_SaveSettings() const
    {
    }
    //--------------------------------------------------------------------------

    void TriangleOpenGLApplication::_LoadSettings()
    {
    }
    //--------------------------------------------------------------------------
}