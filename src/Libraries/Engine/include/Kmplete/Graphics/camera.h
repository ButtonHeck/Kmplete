#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Profile/profiler_fwd.h"


namespace Kmplete
{
    namespace Graphics
    {
        //! Base class for cameras, uses Euler angles approach for view matrices calculation.
        //! Projection matrices should be calculated in implementation subclasses
        class KMP_API Camera
        {
            KMP_LOG_CLASSNAME(Camera)
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            using MovementMask = UInt8;
            using MovementMaskBits = UInt8;

            static constexpr MovementMaskBits NotMoving =    0;
            static constexpr MovementMaskBits MoveLeft =     1 << 0;
            static constexpr MovementMaskBits MoveRight =    1 << 1;
            static constexpr MovementMaskBits MoveUp =       1 << 2;
            static constexpr MovementMaskBits MoveDown =     1 << 3;
            static constexpr MovementMaskBits MoveForward =  1 << 4;
            static constexpr MovementMaskBits MoveBackward = 1 << 5;

            enum class Type {
                FirstPerson,
                LookAt
            };

            enum class Projection {
                Perspective,
                Orthographic
            };

        public:
            Camera(Type type, Projection projection) noexcept;
            Camera(const Math::Point3F& position, Type type, Projection projection) noexcept;
            virtual ~Camera() = default;

            virtual void Update(float delta);

            void Move(MovementMaskBits direction, bool moving) noexcept;
            KMP_NODISCARD bool IsMoving() const noexcept;

            void SetTranslation(const Math::Point3F& translation) noexcept;
            void Translate(const Math::Point3F& delta) noexcept;
            KMP_NODISCARD const Math::Point3F& GetPosition() const noexcept;

            void SetRotation(const Math::Vec3F& rotation) noexcept;
            void Rotate(const Math::Vec3F& delta) noexcept;
            KMP_NODISCARD const Math::Vec3F& GetRotation() const noexcept;

            void SetMovementSpeed(float movementSpeed) noexcept;
            KMP_NODISCARD float GetMovementSpeed() const noexcept;

            void SetRotationSpeed(float rotationSpeed) noexcept;
            KMP_NODISCARD float GetRotationSpeed() const noexcept;

            void SetZNear(float zNear) noexcept;
            KMP_NODISCARD float GetZNear() const noexcept;

            void SetZFar(float zFar) noexcept;
            KMP_NODISCARD float GetZFar() const noexcept;

            void SetAspectRatio(float aspectRatio) noexcept;
            KMP_NODISCARD float GetAspectRatio() const noexcept;

            KMP_NODISCARD const Math::Mat4& GetViewMatrix() const noexcept;
            KMP_NODISCARD const Math::Mat4& GetProjectionMatrix() const noexcept;

            KMP_NODISCARD const Math::Vec3F& GetFront() const noexcept;
            KMP_NODISCARD const Math::Vec3F& GetRight() const noexcept;
            KMP_NODISCARD const Math::Vec3F& GetUp() const noexcept;

        protected:
            virtual void _UpdateProjectionMatrix() = 0;

        private:
            void _UpdateViewMatrix();
            void _UpdateViewVectors();

        protected:
            Type _type;
            Projection _projection;

            Math::Point3F _position;
            Math::Vec3F _rotation;
            float _movementSpeed;
            float _rotationSpeed;
            MovementMask _movementMask;
            float _zNear;
            float _zFar;
            float _aspectRatio;

            Math::Vec3F _front;
            Math::Vec3F _right;
            Math::Vec3F _up;

            Math::Mat4 _viewMatrix;
            Math::Mat4 _projectionMatrix;
        };
        //--------------------------------------------------------------------------
    }
}