#pragma once

#include "Kmplete/Graphics/camera.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class OrthographicCamera : public Camera
        {
            KMP_LOG_CLASSNAME(OrthographicCamera)

        public:
            KMP_API explicit OrthographicCamera(Type type) noexcept;
            KMP_API explicit OrthographicCamera(const Math::Point3F& position, Type type) noexcept;

            KMP_API void SetOrthographicParameters(float left, float right, float top, float bottom, float zNear, float zFar) noexcept;
            KMP_API void SetLeft(float left) noexcept;
            KMP_API void SetRight(float right) noexcept;
            KMP_API void SetTop(float top) noexcept;
            KMP_API void SetBottom(float bottom) noexcept;

            KMP_API void SetScale(float scale) noexcept;
            KMP_NODISCARD KMP_API float GetScale() const noexcept;

            KMP_API void SetApplyAspectRatioFix(bool applyAspect) noexcept;
            KMP_NODISCARD KMP_API bool IsAspectRatioFixApplied() const noexcept;

        private:
            void _UpdateProjectionMatrix() override;

        private:
            float _left;
            float _right;
            float _top;
            float _bottom;

            float _scale;
            bool _applyAspectRatioFix;
        };
        //--------------------------------------------------------------------------
    }
}