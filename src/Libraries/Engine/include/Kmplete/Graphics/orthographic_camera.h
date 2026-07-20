#pragma once

#include "Kmplete/Graphics/camera.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"


namespace Kmplete
{
    namespace Graphics
    {
        //! Implementation of an orthographic projection camera
        class KMP_API OrthographicCamera : public Camera
        {
            KMP_LOG_CLASSNAME(OrthographicCamera)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            explicit OrthographicCamera(Type type) noexcept;
            explicit OrthographicCamera(const Math::Point3F& position, Type type) noexcept;
            ~OrthographicCamera() = default;

            void SetOrthographicParameters(float left, float right, float top, float bottom, float zNear, float zFar) noexcept;
            void SetLeft(float left) noexcept;
            void SetRight(float right) noexcept;
            void SetTop(float top) noexcept;
            void SetBottom(float bottom) noexcept;

            void SetScale(float scale) noexcept;
            void SetScaleDelta(float delta) noexcept;
            KMP_NODISCARD float GetScale() const noexcept;

            void SetApplyAspectRatioFix(bool applyAspect) noexcept;
            KMP_NODISCARD bool IsAspectRatioFixApplied() const noexcept;

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