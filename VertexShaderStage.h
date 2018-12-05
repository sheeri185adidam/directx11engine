#pragma once

#include "ShaderStage.h"
#include "VertexShader.h"

class VertexShaderStage : public ShaderStage
{
public:
  VertexShaderStage(ID3D11DeviceContext* context);
  virtual ~VertexShaderStage() = default;

protected:
  bool BindShader(Shader* shader) override;
  bool BindConstantBuffers(uint32_t start_slot, uint32_t count, ID3D11Buffer * const *buffers) override;
  bool BindSamplers(uint32_t start_slot, uint32_t count, ID3D11SamplerState *const *samplers) override;
  bool BindShaderResourceViews(uint32_t start_slot, uint32_t count, ID3D11ShaderResourceView * const *views) override;
};
