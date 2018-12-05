#pragma once
#include <cstdint>
#include <d3d11.h>
#include "Shader.h"
#include <vector>

class ShaderStageState
{
public:
  // shader
  virtual bool SetShader(Shader* shader) = 0;
  virtual Shader* GetShader(Shader* shader) = 0;

  // constant buffers
  virtual bool SetConstantBuffer(uint32_t slot, ID3D11Buffer* buffer) = 0;
  virtual bool SetConstantBuffers(uint32_t start_slot, uint32_t count, ID3D11Buffer **buffers) = 0;
  virtual void RemoveConstantBuffer(uint32_t slot) = 0;
  virtual void RemoveConstantBuffers() = 0;

  // shader resource views
  virtual bool SetShaderResourveView(uint32_t slot, ID3D11ShaderResourceView* view)=0;
  virtual bool SetShaderResourceViews(uint32_t start_slot, uint32_t count, ID3D11ShaderResourceView **views) = 0;
  virtual void RemoveShaderResourveView(uint32_t slot) = 0;
  virtual void RemoveShaderResourveViews() = 0;

  // samplers
  virtual bool SetSampler(uint32_t slot, ID3D11SamplerState * sampler) = 0;
  virtual bool SetSamplers(uint32_t start_slot, uint32_t count, ID3D11SamplerState **samplers) = 0;
  virtual void RemoveSampler(uint32_t slot) = 0;
  virtual void RemoveSamplers() = 0;

  // unordered access views
  
protected:
  virtual uint32_t ConstantBuffersMax() { return D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT; }
  virtual uint32_t SamplersMax() { return D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT; }
  virtual uint32_t ShaderResourceViewsMax() { return D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT; }
  virtual uint32_t UnorderedAccessViewsMax() { return 0; }

private:
  Shader* shader_;
  std::vector<ID3D11Buffer*> constant_buffers_;
  bool updated_ = true;
};
