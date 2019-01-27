#include "Pipeline.h"
#include "InputAssemblerStage.h"
#include <cassert>

InputAssemblerStage::InputAssemblerStage() = default;

InputAssemblerStage::InputAssemblerStage(ID3D11DeviceContext* context)
  :context_(context)
{
  assert(context_ != nullptr);
  vertex_buffers_ = new VertexBufferContainerType(InputAssemblerStage::VertexBuffersMax());
  strides_ = new std::vector<uint32_t>(InputAssemblerStage::VertexBuffersMax());
  offsets_ = new std::vector<uint32_t>(InputAssemblerStage::VertexBuffersMax());
}

InputAssemblerStage::~InputAssemblerStage() = default;

void InputAssemblerStage::SetVertexBuffer(uint32_t slot, ID3D11Buffer* buffer, uint32_t stride, uint32_t offset)
{
  if (!IsInitialized())
    return;

  if (buffer == nullptr)
    return;

  if (slot >= VertexBuffersMax())
    return;

  RemoveBuffers();
  (*vertex_buffers_)[slot] = buffer;
  (*strides_)[slot] = stride;
  (*offsets_)[slot] = offset;
  start_slot_ = slot;
  slot_count_ = 1;
  Updated(true);
}

void InputAssemblerStage::SetVertexBuffers(uint32_t start_slot, uint32_t count, ID3D11Buffer** buffers,
  uint32_t* strides, uint32_t* offsets)
{
  if (!IsInitialized())
    return;

  if (buffers == nullptr || strides == nullptr || offsets == nullptr)
    return;

  if (start_slot + count >= VertexBuffersMax())
    return;

  RemoveBuffers();

  const auto buffer_begin = vertex_buffers_->begin() + start_slot;
  for (uint32_t i = 0; i < count; ++i)
  {
    *(buffer_begin + i) = buffers[i];
  }

  const auto stride_begin = strides_->begin() + start_slot;
  for (uint32_t i = 0; i < count; ++i)
  {
    *(stride_begin + i) = strides[i];
  }

  const auto offset_begin = offsets_->begin() + start_slot;
  for (uint32_t i = 0; i < count; ++i)
  {
    *(offset_begin + i) = offsets[i];
  }

  start_slot_ = start_slot;
  slot_count_ = count;
  Updated(true);
}

void InputAssemblerStage::SetIndexBuffer(ID3D11Buffer *buffer, DXGI_FORMAT format, uint32_t offset)
{
  if (!IsInitialized())
    return;

  if (buffer == nullptr)
    return;

  index_buffer_ = buffer;
  index_format_ = format;
  index_offset_ = offset;
  Updated(true);
}

void InputAssemblerStage::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
  if (!IsInitialized())
    return;

  topology_ = topology;
  Updated(true);
}

void InputAssemblerStage::SetInputLayout(ID3D11InputLayout *layout)
{
  if (!IsInitialized())
    return;
  input_layout_ = layout;
  Updated(true);
}

void InputAssemblerStage::RemoveBuffers()
{
  for (auto& buffer : *vertex_buffers_)
  {
    buffer = nullptr;
  }

  for (auto& stride : *strides_)
  {
    stride = 0;
  }

  for (auto& offset : *offsets_)
  {
    offset = 0;
  }

  index_buffer_ = nullptr;
  Updated(true);
}

void InputAssemblerStage::ApplyState()
{
  if (!IsUpdated())
    return;

  context_->IASetVertexBuffers(start_slot_, slot_count_, &(*vertex_buffers_)[0], &(*strides_)[0], &(*offsets_)[0]);
  context_->IASetIndexBuffer(index_buffer_, index_format_, index_offset_);
  context_->IASetPrimitiveTopology(topology_);
  context_->IASetInputLayout(input_layout_);
  Updated(false);
}