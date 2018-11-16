#pragma once

#include <cstdint>
#include <d3d11.h>

class Pipeline;

class InputAssemblerStage
{
public:
  InputAssemblerStage(ID3D11DeviceContext* context);

  virtual ~InputAssemblerStage();

  virtual void BindVertexBuffers(uint32_t start_slot, uint32_t count, ID3D11Buffer*const* buffers,
    uint32_t* strides, const uint32_t* offsets);

  virtual void BindIndexBuffer(ID3D11Buffer *buffer, DXGI_FORMAT format, uint32_t offset);

  virtual void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);

  virtual void SetInputLayout(ID3D11InputLayout *layout);

private:
  ID3D11DeviceContext* context_;
};
