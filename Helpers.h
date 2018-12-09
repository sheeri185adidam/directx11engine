#pragma once
#include <DirectXMath.h>

inline DirectX::XMVECTOR FloatToVector(const DirectX::XMFLOAT3& val)
{
  return XMLoadFloat3(&val);
}

inline DirectX::XMFLOAT3 VectorToFloat(const DirectX::XMVECTOR& vec)
{
  DirectX::XMFLOAT3 val{};
  XMStoreFloat3(&val, vec);
  return val;
}
