#pragma once
#include "Shader.h"
#include "ShaderSource.h"

class PixelShader : public Shader
{
public:
  PixelShader() = default;

  PixelShader(ID3D11Device* device, const ShaderSource* source);

  virtual ~PixelShader();

  bool Create(ID3D11Device* device, const ShaderSource* source) override;

  bool CreateInternal(ID3D11Device* device, const ShaderSource* source);

  bool IsCreated() override { return shader_ != nullptr; }

  ID3D11PixelShader* PixelShaderPtr() const { return shader_; }

private:
  ID3D11PixelShader* shader_ = nullptr;
};
