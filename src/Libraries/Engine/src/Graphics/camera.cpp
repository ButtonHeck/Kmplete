#include "Kmplete/Graphics/camera.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    namespace Graphics
    {
        Camera::Camera(Type type, Projection projection) noexcept
            : Camera({0.0f, 0.0f, 0.0f}, type, projection)
        {}
        //--------------------------------------------------------------------------

        Camera::Camera(const Math::Point3F& position, Type type, Projection projection) noexcept
            : _type(type)
            , _projection(projection)
            , _position(position)
            , _rotation(Math::Vec3F(0.0f, 0.0f, 0.0f))
            , _movementSpeed(1.0f)
            , _rotationSpeed(1.0f)
            , _flipY(false)
            , _movementMask(NotMoving)
            , _zNear(0.0f)
            , _zFar(1.0f)
            , _front(Math::UnitVectorZ)
            , _right(Math::UnitVectorX)
            , _up(Math::UnitVectorY)
            , _viewMatrix(Math::Mat4())
            , _projectionMatrix(Math::Mat4())
        {}
        //--------------------------------------------------------------------------

        void Camera::Update(float delta)
        {
            if (IsMoving())
            {
                const auto moveSpeed = _movementSpeed * delta;

                if (_movementMask & MoveForward)
                {
                    _position += _front * moveSpeed;
                }
                if (_movementMask & MoveBackward)
                {
                    _position -= _front * moveSpeed;
                }
                if (_movementMask & MoveLeft)
                {
                    _position -= _right * moveSpeed;
                }
                if (_movementMask & MoveRight)
                {
                    _position += _right * moveSpeed;
                }
                if (_movementMask & MoveUp)
                {
                    _position += _up * moveSpeed;
                }
                if (_movementMask & MoveDown)
                {
                    _position -= _up * moveSpeed;
                }

                KMP_LOG_DEBUG("position - x:{} y:{} z:{}", int(_position.x), int(_position.y), int(_position.z));
            }

            _UpdateViewMatrix();
        }
        //--------------------------------------------------------------------------

        void Camera::Move(MovementMaskBits direction, bool moving) noexcept
        {
            if (moving)
            {
                _movementMask |= direction;
            }
            else
            {
                _movementMask &= ~direction;
            }
        }
        //--------------------------------------------------------------------------

        bool Camera::IsMoving() const noexcept
        {
            return _movementMask != NotMoving;
        }
        //--------------------------------------------------------------------------

        void Camera::SetTranslation(const Math::Point3F& translation) noexcept
        {
            _position = translation;
            _UpdateViewMatrix();
        }
        //--------------------------------------------------------------------------

        void Camera::Translate(const Math::Point3F& delta) noexcept
        {
            _position += delta;
            _UpdateViewMatrix();
        }
        //--------------------------------------------------------------------------

        const Math::Point3F& Camera::GetPosition() const noexcept
        {
            return _position;
        }
        //--------------------------------------------------------------------------

        void Camera::SetRotation(const Math::Vec3F& rotation) noexcept
        {
            _rotation = rotation;
            _UpdateViewVectors();
            _UpdateViewMatrix();
        }
        //--------------------------------------------------------------------------

        void Camera::Rotate(const Math::Vec3F& delta) noexcept
        {
            _rotation += delta;
            _UpdateViewVectors();
            _UpdateViewMatrix();
        }
        //--------------------------------------------------------------------------

        const Math::Vec3F& Camera::GetRotation() const noexcept
        {
            return _rotation;
        }
        //--------------------------------------------------------------------------

        void Camera::SetMovementSpeed(float movementSpeed) noexcept
        {
            _movementSpeed = movementSpeed;
        }
        //--------------------------------------------------------------------------

        void Camera::SetRotationSpeed(float rotationSpeed) noexcept
        {
            _rotationSpeed = rotationSpeed;
        }
        //--------------------------------------------------------------------------

        void Camera::SetFlipY(bool flipY) noexcept
        {
            _flipY = flipY;
            _UpdateProjectionMatrix();
        }
        //--------------------------------------------------------------------------

        bool Camera::IsFlipY() const noexcept
        {
            return _flipY;
        }
        //--------------------------------------------------------------------------

        void Camera::SetZNear(float zNear) noexcept
        {
            _zNear = zNear;
            _UpdateProjectionMatrix();
        }
        //--------------------------------------------------------------------------

        float Camera::GetZNear() const noexcept
        {
            return _zNear;
        }
        //--------------------------------------------------------------------------

        void Camera::SetZFar(float zFar) noexcept
        {
            _zFar = zFar;
            _UpdateProjectionMatrix();
        }
        //--------------------------------------------------------------------------

        float Camera::GetZFar() const noexcept
        {
            return _zFar;
        }
        //--------------------------------------------------------------------------

        const Math::Mat4& Camera::GetViewMatrix() const noexcept
        {
            return _viewMatrix;
        }
        //--------------------------------------------------------------------------

        const Math::Mat4& Camera::GetProjectionMatrix() const noexcept
        {
            return _projectionMatrix;
        }
        //--------------------------------------------------------------------------

        const Math::Vec3F& Camera::GetFront() const noexcept
        {
            return _front;
        }
        //--------------------------------------------------------------------------

        void Camera::_UpdateViewMatrix()
        {
            _viewMatrix = glm::lookAtLH(_position, _position + _front, _up);
        }
        //--------------------------------------------------------------------------

        void Camera::_UpdateViewVectors()
        {
            _front.x = glm::cos(glm::radians(_rotation.x)) * glm::sin(glm::radians(_rotation.y));
            _front.y = glm::sin(glm::radians(_rotation.x));
            _front.z = glm::cos(glm::radians(_rotation.x)) * glm::cos(glm::radians(_rotation.y));
            _front = glm::normalize(_front);

            _right = glm::normalize(glm::cross(Math::UnitVectorY, _front));
            _up = glm::normalize(glm::cross(_front, _right));
        }
        //--------------------------------------------------------------------------
    }
}