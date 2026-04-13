#include "Kmplete/Graphics/orthographic_camera.h"
#include "Kmplete/Log/log.h"


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
            , _scale(1.0f)
            , _applyAspectRatioFix(false)
        {}
        //--------------------------------------------------------------------------

        OrthographicCamera::OrthographicCamera(const Math::Point3F& position, Type type) noexcept
            : Camera(position, type, Projection::Orthographic)
            , _left(0.0f)
            , _right(0.0f)
            , _top(0.0f)
            , _bottom(0.0f)
            , _scale(1.0f)
            , _applyAspectRatioFix(false)
        {}
        //--------------------------------------------------------------------------

        void OrthographicCamera::SetOrthographicParameters(float left, float right, float top, float bottom, float zNear, float zFar) noexcept
        {
            _left = left;
            _right = right;
            _top = top;
            _bottom = bottom;
            _zNear = zNear;
            _zFar = zFar;
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

        void OrthographicCamera::SetScale(float scale) noexcept
        {
            if (scale < 0.25f)
            {
                KMP_LOG_WARN("scale cannot be set less than 0.25 (given {}), clamping will be applied", scale);
                scale = 0.25f;
            }

            _scale = scale;
            _UpdateProjectionMatrix();
        }
        //--------------------------------------------------------------------------

        float OrthographicCamera::GetScale() const noexcept
        {
            return _scale;
        }
        //--------------------------------------------------------------------------

        void OrthographicCamera::SetApplyAspectRatioFix(bool applyAspect) noexcept
        {
            _applyAspectRatioFix = applyAspect;
            _UpdateProjectionMatrix();
        }
        //--------------------------------------------------------------------------

        bool OrthographicCamera::IsAspectRatioFixApplied() const noexcept
        {
            return _applyAspectRatioFix;
        }
        //--------------------------------------------------------------------------

        void OrthographicCamera::_UpdateProjectionMatrix()
        {
            const auto scaledLeft = _left * _scale;
            const auto scaledRight = _right * _scale;
            const auto scaledBottom = _bottom * _scale;
            const auto scaledTop = _top * _scale;

            if (_applyAspectRatioFix)
            {
                _projectionMatrix = glm::orthoLH(scaledLeft, scaledRight, scaledBottom / _aspectRatio, scaledTop / _aspectRatio, _zNear, _zFar);
            }
            else
            {
                _projectionMatrix = glm::orthoLH(scaledLeft, scaledRight, scaledBottom, scaledTop, _zNear, _zFar);
            }
        }
        //--------------------------------------------------------------------------
    }
}