#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Graphics/camera.h"
#include "Kmplete/Profile/profiler_fwd.h"


namespace Kmplete
{
    namespace Graphics
    {
        //! Implementation of a perspective projection camera
        class KMP_API PerspectiveCamera : public Camera
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            explicit PerspectiveCamera(Type type, float fov) noexcept;
            explicit PerspectiveCamera(const Math::Point3F& position, Type type, float fov) noexcept;
            ~PerspectiveCamera() = default;

            void SetFOV(float fov) noexcept;
            void SetFOVDelta(float delta) noexcept;
            KMP_NODISCARD float GetFOV() const noexcept;

        private:
            void _UpdateProjectionMatrix() override;

        private:
            float _fov;
        };
        //--------------------------------------------------------------------------
    }
}