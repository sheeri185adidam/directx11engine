#include "PixelShader.h"
#include <cassert>

PixelShader::PixelShader(ID3D11Device* device, const ShaderSource* source)
{
  assert(source->IsCompiled());
  CreateInternal(device, source);
}

PixelShader::~PixelShader()
{
  if (shader_ != nullptr)
  {
    shader_->Release();
    shader_ = nullptr;
  }
}

bool PixelShader::Create(ID3D11Device* device, const ShaderSource* source)
{
  return CreateInternal(device, source);
}

bool PixelShader::CreateInternal(ID3D11Device* device, const ShaderSource* source)
{
  if (device == nullptr || source == nullptr)
    return false;

  if (source->Type() != ShaderType::PIXEL_SHADER)
    return false;

  ID3D11PixelShader* shader = nullptr;
  auto blob = source->Blob();
  const auto result = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
  if (FAILED(result))
    return false;
  shader_ = shader;
  name_ = source->Name();
  return true;
}