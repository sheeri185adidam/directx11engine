#include "VertexShaderStage.h"

VertexShaderStage::VertexShaderStage(ID3D11DeviceContext* context) : ShaderStage(context)
{
}

bool VertexShaderStage::BindShader(Shader* shader)
{
  const auto vertex_shader = dynamic_cast<VertexShader*>(shader);
  if (vertex_shader == nullptr)
    return  false;

  context_->VSSetShader(vertex_shader->VertexShaderPtr(), nullptr, 0);
  return true;
}

bool VertexShaderStage::BindConstantBuffers(uint32_t start_slot, uint32_t count, ID3D11Buffer * const *buffers)
{
  if (buffers == nullptr || count == 0)
    return false;
  context_->VSSetConstantBuffers(start_slot, count, buffers);
  return true;
}

bool VertexShaderStage::BindSamplers(uint32_t start_slot, uint32_t count, ID3D11SamplerState *const *samplers)
{
  if (samplers == nullptr)
    return false;
  context_->VSSetSamplers(start_slot, count, samplers);
  return true;
}

bool VertexShaderStage::BindShaderResourceViews(uint32_t start_slot, uint32_t count, ID3D11ShaderResourceView * const *views)
{
  if (views == nullptr)
    return false;
  context_->VSSetShaderResources(start_slot, count, views);
  return true;
}