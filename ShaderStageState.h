#pragma once
#include <cstdint>
#include <d3d11.h>
#include <vector>

class Shader;
class ShaderStage;

/// <summary>A shader stage state class.</summary>
class ShaderStageState
{
public:
  using ConstantBufferContainerType = std::vector<ID3D11Buffer*>;
  using ShaderResourceViewContainerType = std::vector<ID3D11ShaderResourceView*>;
  using SamplerContainerType = std::vector<ID3D11SamplerState*>;

  ShaderStageState(ShaderStage* stage);
  virtual ~ShaderStageState();

  virtual bool UpdateRequired() { return updated_; }
  virtual bool ApplyUpdate();

  // shader
  virtual bool SetShader(Shader* shader);
  virtual Shader* GetShader(Shader* shader) { return shader_; }

  // constant buffers
  virtual bool SetConstantBuffer(uint32_t slot, ID3D11Buffer* buffer);
  virtual bool SetConstantBuffers(uint32_t start_slot, uint32_t count, ID3D11Buffer **buffers);
  virtual void RemoveConstantBuffer(uint32_t slot);
  virtual void RemoveConstantBuffers();

  // shader resource views
  virtual bool SetShaderResourveView(uint32_t slot, ID3D11ShaderResourceView* view);
  virtual bool SetShaderResourceViews(uint32_t start_slot, uint32_t count, ID3D11ShaderResourceView **views);
  virtual void RemoveShaderResourveView(uint32_t slot);
  virtual void RemoveShaderResourveViews();

  // samplers
  virtual bool SetSampler(uint32_t slot, ID3D11SamplerState * sampler);
  virtual bool SetSamplers(uint32_t start_slot, uint32_t count, ID3D11SamplerState **samplers);
  virtual void RemoveSampler(uint32_t slot);
  virtual void RemoveSamplers();

  // unordered access views - NOT IMPLEMENTED YET
  
protected:
  virtual uint32_t ConstantBuffersMax() { return D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT; }
  virtual uint32_t SamplersMax() { return D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; }
  virtual uint32_t ShaderResourceViewsMax() { return D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; }
  virtual uint32_t UnorderedAccessViewsMax() { return 0; }

  Shader* shader_ = nullptr;
  ShaderStage* stage_ = nullptr;
  bool updated_ = true;
  ConstantBufferContainerType* constant_buffers_ = nullptr;
  ShaderResourceViewContainerType* shader_resource_views_ = nullptr;
  SamplerContainerType* samplers_ = nullptr;
};
