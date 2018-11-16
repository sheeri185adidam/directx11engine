#pragma once
#include "ShaderStage.h"
#include "PixelShader.h"

class PixelShaderStage : public ShaderStage
{
public:
  PixelShaderStage(ID3D11DeviceContext* context): ShaderStage(context)
  {
  }

  virtual ~PixelShaderStage() = default;

  bool BindShader(Shader* shader) override
  {
    const auto pixel_shader = dynamic_cast<PixelShader*>(shader);
    if (pixel_shader == nullptr)
      return  false;

    context_->PSSetShader(pixel_shader->PixelShaderPtr(), nullptr, 0);
    return true;
  }

  bool BindConstantBuffers(uint32_t start_slot, uint32_t count, ID3D11Buffer * const *buffers) override
  {
    if (buffers == nullptr || count == 0)
      return false;
    context_->PSSetConstantBuffers(start_slot, count, buffers);
    return true;
  }

  bool BindSamplers(uint32_t start_slot, uint32_t count, ID3D11SamplerState *const *samplers) override
  {
    if (samplers == nullptr)
      return false;
    context_->PSSetSamplers(start_slot, count, samplers);
    return true;
  }

  bool BindShaderResourceViews(uint32_t start_slot, uint32_t count, ID3D11ShaderResourceView * const *views) override
  {
    if (views == nullptr)
      return false;
    context_->PSSetShaderResources(start_slot, count, views);
    return true;
  }
};
