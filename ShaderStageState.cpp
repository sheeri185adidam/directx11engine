#include "ShaderStageState.h"
#include "ShaderStage.h"
#include <cassert>

ShaderStageState::ShaderStageState(ShaderStage* stage)
  : stage_(stage)
{
  assert(stage_ != nullptr);
  constant_buffers_ = new std::vector<ID3D11Buffer*>(ShaderStageState::ConstantBuffersMax());
  shader_resource_views_ = new std::vector<ID3D11ShaderResourceView*>(ShaderStageState::ShaderResourceViewsMax());
  samplers_ = new std::vector<ID3D11SamplerState*>(ShaderStageState::SamplersMax());
}

ShaderStageState::~ShaderStageState()
{
  constant_buffers_->clear();
  delete constant_buffers_;
  constant_buffers_ = nullptr;

  shader_resource_views_->clear();
  delete shader_resource_views_;
  shader_resource_views_ = nullptr;

  samplers_->clear();
  delete samplers_;
  samplers_ = nullptr;
}

bool ShaderStageState::ApplyUpdate()
{
  if (!updated_)
    return false;

  if (!stage_->BindShader(shader_))
    return false;

  if (!stage_->BindConstantBuffers(0, ConstantBuffersMax() - 1, &((*constant_buffers_)[0])))
    return false;

  if (!stage_->BindShaderResourceViews(0, ShaderResourceViewsMax() - 1, &((*shader_resource_views_)[0])))
    return false;

  if (!stage_->BindSamplers(0, SamplersMax() - 1, &((*samplers_)[0])))
    return false;

  return true;
}

bool ShaderStageState::SetShader(Shader* shader)
{
  if (shader == nullptr)
    return false;

  shader_ = shader;
  updated_ = true;
  return true;
}

bool ShaderStageState::SetConstantBuffer(uint32_t slot, ID3D11Buffer* buffer)
{
  if (buffer == nullptr)
    return false;

  if (slot >= ConstantBuffersMax())
    return false;

  RemoveConstantBuffers();

  (*constant_buffers_)[slot] = buffer;
  updated_ = true;
  return true;
}

bool ShaderStageState::SetConstantBuffers(uint32_t start_slot, uint32_t count, ID3D11Buffer **buffers)
{
  if (buffers == nullptr)
    return false;

  if (start_slot + count >= ConstantBuffersMax())
    return false;

  RemoveConstantBuffers();

  const auto begin = constant_buffers_->begin() + start_slot;
  for(uint32_t i = 0; i < count; ++i)
  {
    *(begin + i) = buffers[i];
  }

  updated_ = true;
  return true;
}

void ShaderStageState::RemoveConstantBuffer(uint32_t slot)
{
  if (slot >= ConstantBuffersMax())
    return;

  (*constant_buffers_)[slot] = nullptr;
  updated_ = true;
}

void ShaderStageState::RemoveConstantBuffers()
{
  for (auto& element : *constant_buffers_)
  {
    element = nullptr;
  }
  updated_ = true;
}

bool ShaderStageState::SetShaderResourveView(uint32_t slot, ID3D11ShaderResourceView* view)
{
  if (view == nullptr)
    return false;

  if (slot >= ShaderResourceViewsMax())
    return false;

  RemoveShaderResourveViews();

  (*shader_resource_views_)[slot] = view;
  updated_ = true;
  return true;
}

bool ShaderStageState::SetShaderResourceViews(uint32_t start_slot, uint32_t count, ID3D11ShaderResourceView **views)
{
  if (views == nullptr)
    return false;

  if (start_slot + count >= ShaderResourceViewsMax())
    return false;

  RemoveShaderResourveViews();

  const auto begin = shader_resource_views_->begin() + start_slot;
  for (uint32_t i = 0; i < count; ++i)
  {
    *(begin + i) = views[i];
  }

  updated_ = true;
  return true;
}

void ShaderStageState::RemoveShaderResourveView(uint32_t slot)
{
  if (slot >= ShaderResourceViewsMax())
    return;

  (*shader_resource_views_)[slot] = nullptr;
  updated_ = true;
}

void ShaderStageState::RemoveShaderResourveViews()
{
  for (auto& view : *shader_resource_views_)
  {
    view = nullptr;
  }

  updated_ = true;
}

bool ShaderStageState::SetSampler(uint32_t slot, ID3D11SamplerState * sampler)
{
  if (sampler == nullptr)
    return false;

  if (slot >= SamplersMax())
    return false;

  RemoveSamplers();

  (*samplers_)[slot] = sampler;
  updated_ = true;
  return true;
}

bool ShaderStageState::SetSamplers(uint32_t start_slot, uint32_t count, ID3D11SamplerState **samplers)
{
  if (samplers == nullptr)
    return false;

  if (start_slot + count >= SamplersMax())
    return false;

  RemoveSamplers();

  const auto begin = samplers_->begin() + start_slot;
  for (uint32_t i = 0; i < count; ++i)
  {
    *(begin + i) = samplers[i];
  }

  updated_ = true;
  return true;
}

void ShaderStageState::RemoveSampler(uint32_t slot)
{
  if (slot >= SamplersMax())
    return;

  (*samplers_)[slot] = nullptr;
  updated_ = true;
}

void ShaderStageState::RemoveSamplers()
{
  for (auto& sampler : *samplers_)
  {
    sampler = nullptr;
  }

  updated_ = true;
}