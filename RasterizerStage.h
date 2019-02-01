#pragma once
#include <d3d11.h>
#include <cassert>
#include <vector>

class RasterizerStage
{
public:
  RasterizerStage(ID3D11DeviceContext* context)
    : context_(context)
    , state_desc_()
    , viewports_count_(0)
    , viewports_(D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE, D3D11_VIEWPORT())
    , scissors_(D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE, D3D11_RECT())
  {
    assert(context_ != nullptr);
  }

  virtual ~RasterizerStage();

  virtual bool SetState(D3D11_RASTERIZER_DESC desc)
  {
    ID3D11Device* device = nullptr;
    context_->GetDevice(&device);
    if (device == nullptr)
      return false;

    state_desc_ = desc;
    device->CreateRasterizerState(&state_desc_, &state_);
    return true;
  }

  virtual const D3D11_RASTERIZER_DESC& GetState() { return state_desc_; }

  virtual void SetViewports(const D3D11_VIEWPORT *viewports, uint32_t count)
  {
    if (viewports == nullptr)
      return;

    if (count > viewports_.size())
      return;

    for(uint32_t i=0; i<count;++i)
    {
      viewports_[i] = viewports[i];
    }

    viewports_count_ = count;
    context_->RSSetViewports(count, viewports);
  }

  virtual void SetViewport(D3D11_VIEWPORT viewport)
  {
    SetViewports(&viewport, 1);
  }

  virtual void SetViewport(float left_x, float left_y, float width, 
    float height, float min_depth, float max_depth)
  {
    D3D11_VIEWPORT viewport = { left_x, left_y, width, 
      height, min_depth, max_depth };

    SetViewports(&viewport, 1);
  }

  virtual std::vector<D3D11_VIEWPORT> GetViewports()
  {
    std::vector<D3D11_VIEWPORT> result(viewports_count_);
    for(uint32_t i=0;i<viewports_count_;++i)
    {
      result[i] = viewports_[i];
    }
    return result;
  }

  virtual void SetScissors(const D3D11_RECT* scissors, uint32_t count)
  {
    if (scissors == nullptr)
      return;

    if (count > scissors_.size())
      return;

    for (uint32_t i = 0; i < count; ++i)
    {
      scissors_[i] = scissors[i];
    }

    scissor_count_ = count;
    context_->RSSetScissorRects(count, scissors);
  }

private:
  ID3D11DeviceContext* context_;
  D3D11_RASTERIZER_DESC state_desc_;
  ID3D11RasterizerState* state_ = nullptr;
  uint32_t viewports_count_;
  std::vector<D3D11_VIEWPORT> viewports_;
  uint32_t scissor_count_;
  std::vector<D3D11_RECT> scissors_;
};

