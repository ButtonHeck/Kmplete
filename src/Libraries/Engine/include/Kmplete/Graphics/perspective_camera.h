#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/camera.h"
#include "Kmplete/Profile/profiler_fwd.h"


namespace Kmplete
{
    namespace Graphics
    {
        //! Implementation of a perspective projection camera
        class PerspectiveCamera : public Camera
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            KMP_API explicit PerspectiveCamera(Type type, float fov) noexcept;
            KMP_API explicit PerspectiveCamera(const Math::Point3F& position, Type type, float fov) noexcept;
            ~PerspectiveCamera() = default;

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