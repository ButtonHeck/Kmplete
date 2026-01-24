#include "triangle_application.h"


namespace Kmplete
{
    TriangleApplication::TriangleApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
        , _mainWindow(_windowBackend->GetMainWindow())
        , _mainFrameListener(nullptr)
    {
        _Initialize();
    }
    //--------------------------------------------------------------------------

    TriangleApplication::~TriangleApplication()
    {
        _Finalize();
    }
    //--------------------------------------------------------------------------

    void TriangleApplication::_Initialize()
    {
        _mainWindow.SetTitle(_applicationName.c_str());

        _mainFrameListener.reset(new MainFrameListener(*_frameListenerManager.get(), _mainWindow));

        _LoadSettings();
    }
    //--------------------------------------------------------------------------

    void TriangleApplication::_Finalize()
    {
        _SaveSettings();

        _mainFrameListener.reset();
    }
    //--------------------------------------------------------------------------

    void TriangleApplication::_SaveSettings() const
    {
    }
    //--------------------------------------------------------------------------

    void TriangleApplication::_LoadSettings()
    {
    }
    //--------------------------------------------------------------------------
}