#include "Kmplete/Graphics/perspective_camera.h"


namespace Kmplete
{
    namespace Graphics
    {
        PerspectiveCamera::PerspectiveCamera(Type type, float fov) noexcept
            : Camera(type, Projection::Perspective)
            , _fov(fov)
        {
            _UpdateProjectionMatrix();
        }
        //--------------------------------------------------------------------------

        PerspectiveCamera::PerspectiveCamera(const Math::Point3F& position, Type type, float fov) noexcept
            : Camera(position, type, Projection::Perspective)
            , _fov(fov)
        {
            _UpdateProjectionMatrix();
        }
        //--------------------------------------------------------------------------

        void PerspectiveCamera::SetFOV(float fov) noexcept
        {
            _fov = fov;
            _UpdateProjectionMatrix();
        }
        //--------------------------------------------------------------------------

        void PerspectiveCamera::SetFOVDelta(float delta) noexcept
        {
            SetFOV(_fov + delta);
        }
        //--------------------------------------------------------------------------

        float PerspectiveCamera::GetFOV() const noexcept
        {
            return _fov;
        }
        //--------------------------------------------------------------------------

        void PerspectiveCamera::_UpdateProjectionMatrix()
        {
            _projectionMatrix = glm::perspectiveLH(glm::radians(_fov), _aspectRatio, _zNear, _zFar);
            _projectionMatrix[1][1] *= -1.0f;
        }
        //--------------------------------------------------------------------------
    }
}