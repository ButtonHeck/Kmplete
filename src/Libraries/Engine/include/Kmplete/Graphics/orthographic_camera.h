#pragma once

#include "Kmplete/Graphics/camera.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class OrthographicCamera : public Camera
        {
        public:
            KMP_API explicit OrthographicCamera(Type type) noexcept;
            KMP_API explicit OrthographicCamera(const Math::Point3F& position, Type type) noexcept;

            KMP_API void SetOrthographicParameters(float left, float right, float top, float bottom) noexcept;
            KMP_API void SetLeft(float left) noexcept;
            KMP_API void SetRight(float right) noexcept;
            KMP_API void SetTop(float top) noexcept;
            KMP_API void SetBottom(float bottom) noexcept;

        private:
            void _UpdateProjectionMatrix() override;

        private:
            float _left;
            float _right;
            float _top;
            float _bottom;
        };
        //--------------------------------------------------------------------------
    }
}