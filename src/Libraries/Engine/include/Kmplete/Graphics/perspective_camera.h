#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/camera.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class PerspectiveCamera : public Camera
        {
        public:
            KMP_API explicit PerspectiveCamera(Type type, float fov) noexcept;
            KMP_API explicit PerspectiveCamera(const Math::Point3F& position, Type type, float fov) noexcept;

            KMP_API void SetFOV(float fov) noexcept;
            KMP_API void SetFOVDelta(float delta) noexcept;
            KMP_NODISCARD KMP_API float GetFOV() const noexcept;

        private:
            void _UpdateProjectionMatrix() override;

        private:
            float _fov;
        };
        //--------------------------------------------------------------------------
    }
}