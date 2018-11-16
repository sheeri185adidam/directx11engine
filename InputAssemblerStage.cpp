#include "Pipeline.h"
#include "InputAssemblerStage.h"
#include <cassert>

InputAssemblerStage::InputAssemblerStage(ID3D11DeviceContext* context)
  :context_(context)
{
  assert(context_ != nullptr);
}

InputAssemblerStage::~InputAssemblerStage() = default;

void InputAssemblerStage::BindVertexBuffers(uint32_t start_slot, uint32_t count, ID3D11Buffer*const* buffers,
  uint32_t* strides, const uint32_t* offsets)
{
  if (buffers == nullptr || strides == nullptr || offsets == nullptr)
    return;

  context_->IASetVertexBuffers(start_slot, count, buffers, strides, offsets);
}

void InputAssemblerStage::BindIndexBuffer(ID3D11Buffer *buffer, DXGI_FORMAT format, uint32_t offset)
{
  if (buffer == nullptr)
    return;
  context_->IASetIndexBuffer(buffer, format, offset);
}

void InputAssemblerStage::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
  context_->IASetPrimitiveTopology(topology);
}

void InputAssemblerStage::SetInputLayout(ID3D11InputLayout *layout)
{
  context_->IASetInputLayout(layout);
}