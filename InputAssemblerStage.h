#pragma once

#include <cstdint>
#include <d3d11.h>
#include <vector>

class Pipeline;

class InputAssemblerStage
{
public:
  using VertexBufferContainerType = std::vector<ID3D11Buffer*>;

  InputAssemblerStage();
  InputAssemblerStage(ID3D11DeviceContext* context);
  virtual ~InputAssemblerStage();

  virtual void ApplyState();
  virtual void SetVertexBuffer(uint32_t slot, ID3D11Buffer* buffer, uint32_t stride, uint32_t offset);
  virtual void SetVertexBuffers(uint32_t start_slot, uint32_t count, ID3D11Buffer** buffers, uint32_t* strides, uint32_t* offsets);
  virtual void SetIndexBuffer(ID3D11Buffer *buffer, DXGI_FORMAT format, uint32_t offset);
  virtual void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
  virtual void SetInputLayout(ID3D11InputLayout *layout);
  virtual void RemoveBuffers();

protected:
  virtual uint32_t VertexBuffersMax() { return D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; }
  virtual bool IsInitialized() { return context_ != nullptr; }
  virtual void Updated(bool update) { updated_ = update; }
  virtual bool IsUpdated() { return updated_; }

  bool updated_ = true;
  ID3D11DeviceContext* context_ = nullptr;
  VertexBufferContainerType* vertex_buffers_ = nullptr;
  ID3D11Buffer* index_buffer_ = nullptr;
  ID3D11InputLayout * input_layout_ = nullptr;
  uint32_t start_slot_ = 0;
  uint32_t slot_count_ = 0;
  DXGI_FORMAT index_format_= DXGI_FORMAT_R16_UINT;
  uint32_t index_offset_ = 0;
  D3D11_PRIMITIVE_TOPOLOGY topology_= D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
  std::vector<uint32_t>* strides_ = nullptr;
  std::vector<uint32_t>* offsets_ = nullptr;
};
