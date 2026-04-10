#include "Kmplete/Graphics/orthographic_camera.h"


namespace Kmplete
{
    namespace Graphics
    {
        OrthographicCamera::OrthographicCamera(Type type) noexcept
            : Camera(type, Projection::Orthographic)
            , _left(0.0f)
            , _right(0.0f)
            , _top(0.0f)
            , _bottom(0.0f)
        {}
        //--------------------------------------------------------------------------

        OrthographicCamera::OrthographicCamera(const Math::Point3F& position, Type type) noexcept
            : Camera(position, type, Projection::Orthographic)
            , _left(0.0f)
            , _right(0.0f)
            , _top(0.0f)
            , _bottom(0.0f)
        {}
        //--------------------------------------------------------------------------

        void OrthographicCamera::SetOrthographicParameters(float left, float right, float top, float bottom) noexcept
        {
            _left = left;
            _right = right;
            _top = top;
            _bottom = bottom;
            _UpdateProjectionMatrix();
        }
        //--------------------------------------------------------------------------

        void OrthographicCamera::SetLeft(float left) noexcept
        {
            _left = left;
            _UpdateProjectionMatrix();
        }
        //--------------------------------------------------------------------------

        void OrthographicCamera::SetRight(float right) noexcept
        {
            _right = right;
            _UpdateProjectionMatrix();
        }
        //--------------------------------------------------------------------------

        void OrthographicCamera::SetTop(float top) noexcept
        {
            _top = top;
            _UpdateProjectionMatrix();
        }
        //--------------------------------------------------------------------------

        void OrthographicCamera::SetBottom(float bottom) noexcept
        {
            _bottom = bottom;
            _UpdateProjectionMatrix();
        }
        //--------------------------------------------------------------------------

        void OrthographicCamera::_UpdateProjectionMatrix()
        {
            _projectionMatrix = glm::orthoLH(_left, _right, _bottom, _top, _zNear, _zFar);
        }
        //--------------------------------------------------------------------------
    }
}