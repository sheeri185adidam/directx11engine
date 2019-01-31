#include "GeometryShaderStage.h"
#include "GeometryShader.h"

GeometryShaderStage::GeometryShaderStage(ID3D11DeviceContext * context) 
  : ShaderStage(context)
{
}

bool GeometryShaderStage::BindShader(Shader * shader)
{
  const auto geometry_shader = dynamic_cast<GeometryShader*>(shader);
  if (geometry_shader == nullptr)
    return  false;

  context_->GSSetShader(geometry_shader->GeometryShaderPtr(), nullptr, 0);
  return true;
}

bool GeometryShaderStage::BindConstantBuffers(uint32_t start_slot, uint32_t count, ID3D11Buffer * const * buffers)
{
  if (buffers == nullptr || count == 0)
    return false;

  context_->GSSetConstantBuffers(start_slot, count, buffers);
  return true;
}

bool GeometryShaderStage::BindSamplers(uint32_t start_slot, uint32_t count, ID3D11SamplerState * const * samplers)
{
  if (samplers == nullptr)
    return false;

  context_->GSSetSamplers(start_slot, count, samplers);
  return true;
}

bool GeometryShaderStage::BindShaderResourceViews(uint32_t start_slot, uint32_t count, ID3D11ShaderResourceView * const * views)
{
  if (views == nullptr)
    return false;

  context_->GSSetShaderResources(start_slot, count, views);
  return true;
}
