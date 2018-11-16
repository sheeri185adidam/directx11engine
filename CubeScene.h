#pragma once
#include "Scene.h"
#include <DirectXMath.h>

typedef struct _vertexPositionColor
{
  DirectX::XMFLOAT3 pos;
  DirectX::XMFLOAT3 normal;
} VertexPositionNormal;

class CubeScene: public Scene
{
public:
  CubeScene();
  virtual ~CubeScene();
  bool Initialize(Renderer* renderer) override;
  bool Update(Renderer* renderer, float elapsed) override;
  bool Render(Renderer* renderer) override;

private:
  typedef struct _constantBufferStruct {
    DirectX::XMMATRIX world;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
    DirectX::XMFLOAT4 vLightDir[2];
    DirectX::XMFLOAT4 vLightColor[2];
    DirectX::XMFLOAT4 vOutputColor;
  } ConstantBufferStruct;

  // Assert that the constant buffer remains 16-byte aligned.
  static_assert((sizeof(ConstantBufferStruct) % 16) == 0, "Constant Buffer size must be 16-byte aligned");

  ConstantBufferStruct m_constantBufferData{};
  unsigned int  m_indexCount;

  DirectX::XMFLOAT4 m_lightDirs[2]{};
  DirectX::XMFLOAT4 m_lightColors[2]
    = { DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),
          DirectX::XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f),
  };

  ID3D11Buffer*            m_pVertexBuffer = nullptr;
  ID3D11Buffer*            m_pIndexBuffer = nullptr;
  ID3D11InputLayout*       m_pInputLayout = nullptr;
  ID3D11InputLayout*       m_pInputLayoutExtended = nullptr;
  ID3D11Buffer*            m_pConstantBuffer = nullptr;
};
