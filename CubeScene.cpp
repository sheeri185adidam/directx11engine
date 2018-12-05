#include "CubeScene.h"
#include "practice.h"
#include "ShaderManager.h"
#include "ShaderStage.h"

using namespace DirectX;

CubeScene::CubeScene(): m_indexCount(0)
{
}

CubeScene::~CubeScene()
{
  SAFE_RELEASE(m_pVertexBuffer);
  SAFE_RELEASE(m_pIndexBuffer);
  SAFE_RELEASE(m_pInputLayout);
  SAFE_RELEASE(m_pInputLayoutExtended);
  SAFE_RELEASE(m_pConstantBuffer);
}

bool CubeScene::Initialize(Renderer* renderer)
{
  auto device = renderer->Device();

  // initialize world, view, and projection matrices
  m_World = renderer->GetWorldMatrix();
  m_View = renderer->GetViewMatrix();
  m_Projection = renderer->GetProjectionMatrix();

  // Create vertex buffer
  VertexPositionNormal vertices[] =
  {
      {DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
      {DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
      { DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
      { DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },

      { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f) },
      { DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f) },
      { DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f) },
      { DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f) },

      { DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f) },
      { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f) },
      { DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f) },
      { DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f) },

      { DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
      { DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
      { DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
      { DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },

      { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f) },
      { DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f) },
      { DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f) },
      { DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f) },

      { DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
      { DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
      { DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
      { DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
  };

  // Create vertex buffer:
  CD3D11_BUFFER_DESC vDesc(
    sizeof(VertexPositionNormal) * 24,
    D3D11_BIND_VERTEX_BUFFER,
    D3D11_USAGE_IMMUTABLE
  );

  D3D11_SUBRESOURCE_DATA vData;
  ZeroMemory(&vData, sizeof(vData));
  vData.pSysMem = vertices;
  vData.SysMemPitch = 0;
  vData.SysMemSlicePitch = 0;

  HRESULT hr = device->CreateBuffer(&vDesc, &vData, &m_pVertexBuffer);
  if (FAILED(hr))
    return false;

  // Create index buffer:
  WORD CubeIndices[] =
  {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
  };

  m_indexCount = ARRAYSIZE(CubeIndices);

  CD3D11_BUFFER_DESC iDesc(
    sizeof(WORD) * 36,
    D3D11_BIND_INDEX_BUFFER,
    D3D11_USAGE_IMMUTABLE
  );

  D3D11_SUBRESOURCE_DATA iData;
  ZeroMemory(&iData, sizeof(D3D11_SUBRESOURCE_DATA));
  iData.pSysMem = CubeIndices;
  iData.SysMemPitch = 0;
  iData.SysMemSlicePitch = 0;

  hr = device->CreateBuffer(&iDesc, &iData, &m_pIndexBuffer);

  // Create the constant buffer
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(ConstantBufferStruct);
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bd.CPUAccessFlags = 0;
  hr = device->CreateBuffer(&bd, nullptr, &m_pConstantBuffer);
  if (FAILED(hr))
    return false;

  D3D11_INPUT_ELEMENT_DESC layout[] =
  {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  const auto source = ShaderManager::Instance().GetShaderSource("Cube_VS");
  if (source == nullptr)
    return false;

  ID3D11InputLayout* input_layout = nullptr;
  hr = device->CreateInputLayout(layout, ARRAYSIZE(layout), source->Blob()->GetBufferPointer(), source->Blob()->GetBufferSize(), &input_layout);
  m_pInputLayout = input_layout;
  return !FAILED(hr);
}

bool CubeScene::Update(Renderer* renderer, float elapsed)
{
  // Rotate the cube.
  static float t = 0.0f;
  static ULONGLONG timeStart = 0;
  ULONGLONG timeCur = GetTickCount64();
  if (timeStart == 0)
  {
    timeStart = timeCur;
  }

  t = (timeCur - timeStart) / 1000.0f;

  // Rotate cube around the origin
  m_World = DirectX::XMMatrixRotationY(t);

  // Setup our lighting parameters
  DirectX::XMFLOAT4 vLightDirs[2] =
  {
      DirectX::XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f),
      DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f),
  };

  // Rotate the second light around the origin
  DirectX::XMMATRIX mRotate = DirectX::XMMatrixRotationY(-2.0f * t);
  XMVECTOR vLightDir = XMLoadFloat4(&vLightDirs[1]);
  vLightDir = DirectX::XMVector3Transform(vLightDir, mRotate);
  DirectX::XMStoreFloat4(&vLightDirs[1], vLightDir);

  m_lightDirs[0] = vLightDirs[0];
  m_lightDirs[1] = vLightDirs[1];

  // update member constant buffer
  // The transpose is used below because HLSL stores matrices in column-major order.
  // To make sure wold matrix is used inside shader as it is here, we transpose.
  // To disable HLSL to use column-major matrix storage, set D3D10_SHADER_PACK_MATRIX_ROW_MAJOR during
  // shader compilation
  m_constantBufferData.world = XMMatrixTranspose(m_World);
  m_constantBufferData.view = XMMatrixTranspose(m_View);
  m_constantBufferData.projection = XMMatrixTranspose(m_Projection);
  m_constantBufferData.vLightDir[0] = m_lightDirs[0];
  m_constantBufferData.vLightDir[1] = m_lightDirs[1];
  m_constantBufferData.vLightColor[0] = m_lightColors[0];
  m_constantBufferData.vLightColor[1] = m_lightColors[1];
  m_constantBufferData.vOutputColor = XMFLOAT4(0, 0, 0, 0);
  return true;
}

bool CubeScene::Render(Renderer* renderer)
{
  auto pipeline = renderer->RenderingPipeline();
  auto ia_stage = pipeline->GetInputAssemblerStage();
  auto context = renderer->Context();

  // Set up the IA stage by setting the input topology and layout.
  UINT stride = sizeof(VertexPositionNormal);
  UINT offset = 0;
  ia_stage->BindVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
  ia_stage->BindIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
  ia_stage->SetInputLayout(m_pInputLayout);
  ia_stage->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  context->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_constantBufferData, 0, 0);

  const auto vertex_shader = ShaderManager::Instance().GetShader("Cube_VS");
  auto vs_stage = pipeline->GetVertexShaderStage();
  vs_stage->State()->SetShader(vertex_shader);
  vs_stage->State()->SetConstantBuffers(0, 1, &m_pConstantBuffer);

  const auto pixel_shader = ShaderManager::Instance().GetShader("Cube_PS");
  auto ps_stage = pipeline->GetPixelShaderStage();
  ps_stage->State()->SetShader(pixel_shader);
  ps_stage->State()->SetConstantBuffers(0, 1, &m_pConstantBuffer);

  renderer->DrawIndexed(m_indexCount, 0, 0);

  // Render each light
  const auto pixel_shader_solid = ShaderManager::Instance().GetShader("Cube_PSSolid");
  for (int m = 0; m < 2; m++)
  {
    XMMATRIX mLight = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&m_lightDirs[m]));
    XMMATRIX mLightScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
    mLight = mLightScale * mLight;

    // Update the world variable to reflect the current light
    m_constantBufferData.world = XMMatrixTranspose(mLight);
    m_constantBufferData.vOutputColor = m_lightColors[m];
    renderer->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_constantBufferData, 0, 0);

    ps_stage->State()->SetShader(pixel_shader_solid);

    renderer->DrawIndexed(36, 0, 0);
  }

  return true;
}