#include "PixelShaderStage.h"

PixelShaderStage::PixelShaderStage(ID3D11DeviceContext* context) : ShaderStage(context)
{
}

bool PixelShaderStage::BindShader(Shader* shader)
{
  const auto pixel_shader = dynamic_cast<PixelShader*>(shader);
  if (pixel_shader == nullptr)
    return  false;

  context_->PSSetShader(pixel_shader->PixelShaderPtr(), nullptr, 0);
  return true;
}

bool PixelShaderStage::BindConstantBuffers(uint32_t start_slot, uint32_t count, ID3D11Buffer * const *buffers)
{
  if (buffers == nullptr || count == 0)
    return false;
  context_->PSSetConstantBuffers(start_slot, count, buffers);
  return true;
}

bool PixelShaderStage::BindSamplers(uint32_t start_slot, uint32_t count, ID3D11SamplerState *const *samplers)
{
  if (samplers == nullptr)
    return false;
  context_->PSSetSamplers(start_slot, count, samplers);
  return true;
}

bool PixelShaderStage::BindShaderResourceViews(uint32_t start_slot, uint32_t count, ID3D11ShaderResourceView * const *views)
{
  if (views == nullptr)
    return false;
  context_->PSSetShaderResources(start_slot, count, views);
  return true;
}