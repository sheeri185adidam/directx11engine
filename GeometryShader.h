#pragma once
#include "Shader.h"
#include <cassert>
#include "ShaderSource.h"

class GeometryShader : Shader
{
public:
  GeometryShader() = default;

  GeometryShader(ID3D11Device* device, const ShaderSource* source)
  {
    assert(source->IsCompiled());
    CreateInternal(device, source);
  }

  virtual ~GeometryShader()
  {
    if (shader_ != nullptr)
    {
      shader_->Release();
      shader_ = nullptr;
    }
  }

  bool Create(ID3D11Device* device, const ShaderSource* source) override
  {
    return CreateInternal(device, source);
  }

  bool CreateInternal(ID3D11Device* device, const ShaderSource* source)
  {
    if (device == nullptr || source == nullptr)
      return false;

    if (source->Type() != ShaderType::GEOMETRY_SHADER)
      return false;

    ID3D11GeometryShader* shader = nullptr;
    auto blob = source->Blob();
    const auto result = device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
    if (FAILED(result))
      return false;
    shader_ = shader;
    name_ = source->Name();
    return true;
  }

  bool IsCreated() override { return shader_ != nullptr; }

  ID3D11GeometryShader* GeometryShaderPtr() const { return shader_; }

private:
  ID3D11GeometryShader* shader_ = nullptr;
};
