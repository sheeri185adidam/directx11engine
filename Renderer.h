#pragma once
#include <memory>
#include "Pipeline.h"
#include "DirectXDevice.h"
#include "ShaderManager.h"
#include <DirectXMath.h>

class Renderer
{
public:
  Renderer(): desc_(), viewport_()
  {
  }

  virtual ~Renderer() = default;

  virtual bool Initialize(HWND hWnd)
  {
    if (IsInitialized())
      return true;

    if (!device_.Initialize())
      return false;

    swap_chain_ = device_.SwapChain(hWnd);
    if (swap_chain_ == nullptr)
      return false;

    if (!ConfigureBackBuffer())
      return false;

    CreateShaders();

    // create pipeline
    pipeline_ = std::make_shared<Pipeline>(device_.Context());
    initialized_ = true;
    return initialized_;
  }

  virtual bool ConfigureBackBuffer()
  {
    HRESULT hr = S_OK;
    hr = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer_));
    if (FAILED(hr))
      return false;

    if (!device_.RenderTargetView(back_buffer_, nullptr, &render_target_))
      return false;

    back_buffer_->GetDesc(&desc_);

    // Create a depth-stencil view for use with 3D rendering if needed.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(
      DXGI_FORMAT_D24_UNORM_S8_UINT,
      static_cast<UINT> (desc_.Width),
      static_cast<UINT> (desc_.Height),
      1, // This depth stencil view has only one texture.
      1, // Use a single mipmap level.
      D3D11_BIND_DEPTH_STENCIL
    );

    device_.Device()->CreateTexture2D(
      &depthStencilDesc,
      nullptr,
      &depth_stencil_
    );

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

    device_.DepthStencilView(depth_stencil_, &depthStencilViewDesc, &depth_stencil_view_);

    device_.Context()->OMSetRenderTargets(1, &render_target_, depth_stencil_view_);

    ZeroMemory(&viewport_, sizeof(D3D11_VIEWPORT));
    viewport_.Height = static_cast<float>(desc_.Height);
    viewport_.Width = static_cast<float>(desc_.Width);
    viewport_.MinDepth = 0;
    viewport_.MaxDepth = 1;

    device_.Context()->RSSetViewports(1, &viewport_);
    return true;
  }

  virtual void ReleaseBackBuffer()
  {
    // Release the render target view based on the back buffer:
    SAFE_RELEASE(render_target_);

    // Release the back buffer itself:
    SAFE_RELEASE(back_buffer_);

    // The depth stencil will need to be resized, so release it (and view):
    SAFE_RELEASE(depth_stencil_view_);
    SAFE_RELEASE(depth_stencil_)

    // After releasing references to these resources, we need to call Flush() to 
    // ensure that Direct3D also releases any references it might still have to
    // the same resources - such as pipeline bindings.
    device_.Context()->Flush();
  }

  virtual bool OnFullScreenMode()
  {
    HRESULT hr = S_OK;

    hr = swap_chain_->SetFullscreenState(TRUE, nullptr);

    // Swap chains created with the DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL flag need to
    // call ResizeBuffers in order to realize a full-screen mode switch. Otherwise, 
    // your next call to Present will fail.

    // Before calling ResizeBuffers, you have to release all references to the back 
    // buffer device resource.
    ReleaseBackBuffer();

    // Now we can call ResizeBuffers.
    hr = swap_chain_->ResizeBuffers(
      0,                   // Number of buffers. Set this to 0 to preserve the existing setting.
      0, 0,                // Width and height of the swap chain. Set to 0 to match the screen resolution.
      DXGI_FORMAT_UNKNOWN, // This tells DXGI to retain the current back buffer format.
      0
    );

    // Then we can recreate the back buffer, depth buffer, and so on.
    hr = ConfigureBackBuffer();
    return !FAILED(hr);
  }

  virtual bool OnWindowedMode()
  {
    HRESULT hr = S_OK;

    hr = swap_chain_->SetFullscreenState(FALSE, nullptr);

    // Swap chains created with the DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL flag need to
    // call ResizeBuffers in order to realize a change to windowed mode. Otherwise, 
    // your next call to Present will fail.

    // Before calling ResizeBuffers, you have to release all references to the back 
    // buffer device resource.
    ReleaseBackBuffer();

    // Now we can call ResizeBuffers.
    hr = swap_chain_->ResizeBuffers(
      0,                   // Number of buffers. Set this to 0 to preserve the existing setting.
      0, 0,                // Width and height of the swap chain. MUST be set to a non-zero value. For example, match the window size.
      DXGI_FORMAT_UNKNOWN, // This tells DXGI to retain the current back buffer format.
      0
    );

    // Then we can recreate the back buffer, depth buffer, and so on.
    hr = ConfigureBackBuffer();
    return !FAILED(hr);
  }

  virtual float AspectRatio() const
  {
    return static_cast<float>(desc_.Width) / static_cast<float>(desc_.Height);
  }

  virtual void DrawIndexed(UINT count, UINT begin, int offset)
  {
    device_.Context()->DrawIndexed(count, begin, offset);
  }

  virtual void ClearRenderTargetAndDepthStencilBuffers()
  {
    auto context = device_.Context();
    // Use the Direct3D device context to draw.
    context->OMSetRenderTargets(1, &render_target_, depth_stencil_view_);

    // Clear the render target and the z-buffer.
    const float teal[] = { 0.098f, 0.439f, 0.439f, 1.000f };
    context->ClearRenderTargetView(render_target_, teal);
    context->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
  }

  virtual void Present()
  {
    swap_chain_->Present(0, 0);
  }

  virtual ID3D11Device* Device() { return device_.Device(); }
  virtual ID3D11DeviceContext* Context() { return device_.Context(); }

  virtual Pipeline* RenderingPipeline() { return pipeline_.get(); }

  virtual DirectX::XMMATRIX GetWorldMatrix()
  {
    return DirectX::XMMatrixIdentity();
  }

  virtual DirectX::XMMATRIX GetViewMatrix()
  {
    const auto Eye = DirectX::XMVectorSet(0.0f, 4.0f, -10.0f, 0.0f);
    const auto At = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    const auto Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    return DirectX::XMMatrixLookAtLH(Eye, At, Up);
  }

  virtual DirectX::XMMATRIX GetProjectionMatrix()
  {
    return DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, AspectRatio(), 0.01f, 100.0f);
  }

  virtual void UpdateSubresource(ID3D11Resource* resource,uint32_t subresource,
    const D3D11_BOX *dst_box, const void *data,uint32_t row_pitch,uint32_t depth_pitch)
  {
    device_.Context()->UpdateSubresource(resource, subresource, dst_box, data, row_pitch, depth_pitch);
  }

  virtual bool IsInitialized() { return initialized_; }

protected:
  virtual void CreateShaders()
  {
    ShaderManager::Instance().AddShaderSource(new VertexShaderSource("Cube_VS", L"Cube.fx", "VS", "vs_5_0"));
    ShaderManager::Instance().AddShaderSource(new PixelShaderSource("Cube_PS", L"Cube.fx", "PS", "ps_5_0"));
    ShaderManager::Instance().AddShaderSource(new PixelShaderSource("Cube_PSSolid", L"Cube.fx", "PSSolid", "ps_5_0"));
    ShaderManager::Instance().LoadShaders(device_.Device());
  }

private:
  // device
  DirectXDevice device_;

  // resources
  IDXGISwapChain* swap_chain_ = nullptr;
  ID3D11Texture2D* back_buffer_ = nullptr;
  ID3D11RenderTargetView* render_target_ = nullptr;
  ID3D11Texture2D*         depth_stencil_ = nullptr;
  ID3D11DepthStencilView*  depth_stencil_view_ = nullptr;
  D3D11_TEXTURE2D_DESC    desc_;
  D3D11_VIEWPORT          viewport_;

  // pipeline
  std::shared_ptr<Pipeline> pipeline_ = nullptr;
  bool initialized_ = false;
};
