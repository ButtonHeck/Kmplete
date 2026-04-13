#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        class Camera
        {
            KMP_LOG_CLASSNAME(Camera)

        public:
            using MovementMask = UInt8;
            using MovementMaskBits = UInt8;

            static constexpr MovementMaskBits NotMoving =    0b0000'0000;
            static constexpr MovementMaskBits MoveLeft =     0b0000'0001;
            static constexpr MovementMaskBits MoveRight =    0b0000'0010;
            static constexpr MovementMaskBits MoveUp =       0b0000'0100;
            static constexpr MovementMaskBits MoveDown =     0b0000'1000;
            static constexpr MovementMaskBits MoveForward =  0b0001'0000;
            static constexpr MovementMaskBits MoveBackward = 0b0010'0000;

            enum class Type {
                FirstPerson,
                LookAt
            };

            enum class Projection {
                Perspective,
                Orthographic
            };

        public:
            KMP_API Camera(Type type, Projection projection) noexcept;
            KMP_API Camera(const Math::Point3F& position, Type type, Projection projection) noexcept;
            KMP_API virtual ~Camera() = default;

            KMP_API virtual void Update(float delta);

            KMP_API void Move(MovementMaskBits direction, bool moving) noexcept;
            KMP_NODISCARD KMP_API bool IsMoving() const noexcept;

            KMP_API void SetTranslation(const Math::Point3F& translation) noexcept;
            KMP_API void Translate(const Math::Point3F& delta) noexcept;
            KMP_NODISCARD KMP_API const Math::Point3F& GetPosition() const noexcept;

            KMP_API void SetRotation(const Math::Vec3F& rotation) noexcept;
            KMP_API void Rotate(const Math::Vec3F& delta) noexcept;
            KMP_NODISCARD KMP_API const Math::Vec3F& GetRotation() const noexcept;

            KMP_API void SetMovementSpeed(float movementSpeed) noexcept;
            KMP_NODISCARD KMP_API float GetMovementSpeed() const noexcept;

            KMP_API void SetRotationSpeed(float rotationSpeed) noexcept;
            KMP_NODISCARD KMP_API float GetRotationSpeed() const noexcept;

            KMP_API void SetZNear(float zNear) noexcept;
            KMP_NODISCARD KMP_API float GetZNear() const noexcept;

            KMP_API void SetZFar(float zFar) noexcept;
            KMP_NODISCARD KMP_API float GetZFar() const noexcept;

            KMP_API void SetAspectRatio(float aspectRatio) noexcept;
            KMP_NODISCARD KMP_API float GetAspectRatio() const noexcept;

            KMP_NODISCARD KMP_API const Math::Mat4& GetViewMatrix() const noexcept;
            KMP_NODISCARD KMP_API const Math::Mat4& GetProjectionMatrix() const noexcept;

            KMP_NODISCARD KMP_API const Math::Vec3F& GetFront() const noexcept;
            KMP_NODISCARD KMP_API const Math::Vec3F& GetRight() const noexcept;
            KMP_NODISCARD KMP_API const Math::Vec3F& GetUp() const noexcept;

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