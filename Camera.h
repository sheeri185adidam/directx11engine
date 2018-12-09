#pragma once

#include "Helpers.h"

class Camera
{
public:
  Camera()
  {
    position_ = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
    target_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    up_ = VectorToFloat(DirectX::XMVectorAdd(FloatToVector(position_), FloatToVector(DirectX::XMFLOAT3(0, 1, 0))));
    CalculateViewMatrix();

    field_of_view_ = 0.0f;
    viewport_width_ = 0.0f;
    viewport_height_ = 0.0f;
    near_plane_ = 0.0f;
    far_plane_ = 0.0f;

    XMStoreFloat4x4(&projection_, DirectX::XMMatrixIdentity());
    XMStoreFloat4x4(&ortho_, DirectX::XMMatrixIdentity());
  }

  Camera(const Camera& camera) = default;

  virtual void Move(DirectX::XMFLOAT3 position)
  {
    const auto translation = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    position_ = VectorToFloat(XMVector3Transform(FloatToVector(position_), translation));
    target_ = VectorToFloat(XMVector3Transform(FloatToVector(target_), translation));
    up_ = VectorToFloat(XMVector3Transform(FloatToVector(up_), translation));
    CalculateViewMatrix();
  }

  virtual void Rotate(DirectX::XMFLOAT3 axis, float degrees)
  {
    if (degrees == 0.0f)
      return;

    if (DirectX::XMVector3Equal(FloatToVector(axis), DirectX::XMVectorZero()))
      return;

    auto look_at_target = VectorToFloat(
      DirectX::XMVectorSubtract(FloatToVector(target_), FloatToVector(position_))
    );

    auto look_at_up = VectorToFloat(
      DirectX::XMVectorSubtract(FloatToVector(up_), FloatToVector(position_))
    );

    look_at_target = VectorToFloat(XMVector3Transform(FloatToVector(look_at_target),
      DirectX::XMMatrixRotationAxis(FloatToVector(axis), DirectX::XMConvertToRadians(degrees))));
    look_at_up = VectorToFloat(XMVector3Transform(FloatToVector(look_at_up),
      DirectX::XMMatrixRotationAxis(FloatToVector(axis), DirectX::XMConvertToRadians(degrees))));

    target_ = VectorToFloat(DirectX::XMVectorAdd(FloatToVector(position_), FloatToVector(look_at_target)));
    up_ = VectorToFloat(DirectX::XMVectorAdd(FloatToVector(position_), FloatToVector(look_at_up)));

    CalculateViewMatrix();
  }

  virtual void Target(DirectX::XMFLOAT3 target)
  {
    if (DirectX::XMVector3Equal(FloatToVector(target), FloatToVector(position_)))
      return;

    if (DirectX::XMVector3Equal(FloatToVector(target), FloatToVector(target_)))
      return;

    const auto old_look_at_target = VectorToFloat(DirectX::XMVectorSubtract(FloatToVector(target_), FloatToVector(position_)));
    const auto new_look_at_target = VectorToFloat(DirectX::XMVectorSubtract(FloatToVector(target), FloatToVector(position_)));
    const auto angle = DirectX::XMConvertToDegrees(DirectX::XMVectorGetX(
      DirectX::XMVector3AngleBetweenNormals(DirectX::XMVector3Normalize(FloatToVector(old_look_at_target)),
        DirectX::XMVector3Normalize(FloatToVector(new_look_at_target)))));

    if (angle != 0.0f && angle != 360.0f && angle != 180.0f)
    {
      const auto axis = DirectX::XMVector3Cross(FloatToVector(old_look_at_target), FloatToVector(new_look_at_target));
      Rotate(VectorToFloat(axis), angle);
    }
    target_ = target;
    CalculateViewMatrix();
  }

  virtual void Position(DirectX::XMFLOAT3& position)
  {
    const auto move_vector = VectorToFloat(DirectX::XMVectorSubtract(FloatToVector(position), FloatToVector(position_)));
    const auto target = target_;
    Move(move_vector);
    Target(target);
  }

  virtual void OnViewportResize(float width, float height)
  {
    viewport_width_ = width;
    viewport_height_ = height;
    CalculateProjectionMatrix();
    CalculateOrthogonalMatrix();
  }

  virtual void FieldOfView(float angle)
  {
    field_of_view_ = angle;
    CalculateProjectionMatrix();
  }

  virtual void NearPlane(float plane)
  {
    near_plane_ = plane;
    CalculateProjectionMatrix();
  }

  virtual void FarPlane(float plane)
  {
    far_plane_ = plane;
    CalculateProjectionMatrix();
    CalculateOrthogonalMatrix();
  }

  virtual DirectX::XMFLOAT4X4 ViewMatrix() const { return view_; }

  virtual DirectX::XMFLOAT4X4 OrthogonalMatrix() const { return ortho_; }

  virtual DirectX::XMFLOAT4X4 ProjectionMatrix() const { return projection_; }

protected:
  virtual void CalculateViewMatrix()
  {
    XMStoreFloat4x4(&view_,
      DirectX::XMMatrixLookAtLH(XMLoadFloat3(&position_), XMLoadFloat3(&target_), XMLoadFloat3(&up_)));
  }

  virtual void CalculateProjectionMatrix()
  {
    XMStoreFloat4x4(&projection_, DirectX::XMMatrixPerspectiveFovLH(field_of_view_, viewport_width_ / viewport_height_, near_plane_, far_plane_));
  }

  virtual void CalculateOrthogonalMatrix()
  {
    XMStoreFloat4x4(&ortho_, DirectX::XMMatrixOrthographicLH(viewport_width_, viewport_height_, 0.0f, far_plane_));
  }

  DirectX::XMFLOAT4X4 ortho_{};
  DirectX::XMFLOAT4X4 view_{};
  DirectX::XMFLOAT4X4 projection_{};

  DirectX::XMFLOAT3 position_{};
  DirectX::XMFLOAT3 target_{};
  DirectX::XMFLOAT3 up_{};

  float field_of_view_;
  float viewport_width_;
  float viewport_height_;
  float near_plane_;
  float far_plane_;
};
