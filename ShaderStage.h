#pragma once

#include "Shader.h"
#include "ShaderStageState.h"

class ShaderStage
{
public:
  virtual ~ShaderStage() = default;
  virtual ShaderStageState* State() { return &state_; }

protected:
  ShaderStage(ID3D11DeviceContext* context)
    : context_(context)
    , state_(this)
  {
  }

  // all shader stages must implement these methods
  virtual bool BindShader(Shader* shader) = 0;
  virtual bool BindConstantBuffers(uint32_t start_slot, uint32_t count, ID3D11Buffer * const *buffers) = 0;
  virtual bool BindSamplers(uint32_t start_slot, uint32_t count, ID3D11SamplerState *const *samplers) = 0;
  virtual bool BindShaderResourceViews(uint32_t start_slot, uint32_t count, ID3D11ShaderResourceView * const *views) = 0;

  // only computer shader stage supports Unordered Access View(UAV)
  virtual bool BindUnorderedAccessViews(uint32_t start_slot, uint32_t count,
    ID3D11UnorderedAccessView * const *views, const uint32_t* uav_counters)
  {
    return false;
  }

  ID3D11DeviceContext* context_;
  ShaderStageState state_;
  friend class ShaderStageState;
};
