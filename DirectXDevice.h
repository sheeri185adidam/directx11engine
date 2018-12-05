#pragma once
#include "practice.h"
#include <cstdint>
#include <d3d11.h>
#include <wrl.h>
#include <dxgi1_3.h>

class DirectXDevice
{
public:
  virtual bool Initialize();

  virtual IDXGISwapChain* SwapChain(HWND wnd);

  virtual ID3D11Buffer* VertexBuffer(uint32_t size, bool cpu_writable, bool streamout, D3D11_SUBRESOURCE_DATA* data);

  virtual ID3D11Buffer* IndexBuffer(uint32_t size, bool cpu_writable, D3D11_SUBRESOURCE_DATA* data);

  virtual ID3D11Buffer* ConstantBuffer(uint32_t size, bool gpu_writable, bool cpu_writable, D3D11_SUBRESOURCE_DATA* data);
  
  virtual ID3D11Buffer* StructuredBuffer(uint32_t count, uint32_t structsize, bool cpu_writable, bool gpu_writable, D3D11_SUBRESOURCE_DATA* data);

  virtual void* Texture1D(uint32_t width, uint32_t miplevels, uint32_t array_size, DXGI_FORMAT format,
    uint32_t bindflags, bool gpu_writable, bool cpu_writable, uint32_t misc_flags, D3D11_SUBRESOURCE_DATA* data);

  // ShaderSource Resource View (SRV)
  virtual ID3D11ShaderResourceView* BufferShaderResourceView(ID3D11Resource* resource, DXGI_FORMAT format, uint32_t offset, uint32_t width);
   
  virtual ID3D11ShaderResourceView* Texture1DShaderResourceView(ID3D11Resource* resource, DXGI_FORMAT format, uint32_t most_detailed_mip, uint32_t mip_levels);

  virtual ID3D11ShaderResourceView* Texture1DArrayShaderResourceView(ID3D11Resource* resource, DXGI_FORMAT format,
    uint32_t most_detailed_mip, uint32_t mip_levels, uint32_t first_array_slice, uint32_t array_size);

  // Unordered Access View (UAV)
  virtual ID3D11UnorderedAccessView* Texture1DUnorderedAccessView(ID3D11Resource* resource, DXGI_FORMAT format, uint32_t mip_slice);

  virtual ID3D11UnorderedAccessView* Texture1DArrayUnorderedAccessView(ID3D11Resource* resource, DXGI_FORMAT format,
    uint32_t mip_slice, uint32_t first_array_slice, uint32_t array_size);

  // Render Target view (RTV)
  virtual ID3D11RenderTargetView* Texture1DRenderTargetView(ID3D11Resource* resource, DXGI_FORMAT format, uint32_t mip_slice);

  virtual ID3D11RenderTargetView* Texture1DArrayRenderTargetView(ID3D11Resource* resource, DXGI_FORMAT format,
    uint32_t mip_slice, uint32_t first_array_slice, uint32_t array_size);

  // Depth Stencil View (DSV)
  virtual ID3D11DepthStencilView* Texture1DDepthStencilView(ID3D11Resource* resource, DXGI_FORMAT format, uint32_t mip_slice);

  virtual ID3D11DepthStencilView* Texture1DArrayDepthStencilView(ID3D11Resource* resource, DXGI_FORMAT format,
    uint32_t mip_slice, uint32_t first_array_slice, uint32_t array_size);

  virtual ID3D11DeviceContext* Context() { return context_; }

  virtual bool RenderTargetView(ID3D11Resource *resource, const D3D11_RENDER_TARGET_VIEW_DESC *desc, ID3D11RenderTargetView **view);

  virtual bool DepthStencilView(ID3D11Resource *resource, const D3D11_DEPTH_STENCIL_VIEW_DESC *desc, ID3D11DepthStencilView **view);

  virtual ID3D11Device* Device() { return device_; }

  virtual ~DirectXDevice() 
  {
    SAFE_RELEASE(context_);
    SAFE_RELEASE(device_);
  }

protected:
  virtual ID3D11Buffer* CreateBuffer(const D3D11_BUFFER_DESC *desc, const D3D11_SUBRESOURCE_DATA *data);
  virtual bool IsInitialized() { return device_ != nullptr; }

  D3D_FEATURE_LEVEL feature_level_ = {};
  ID3D11Device* device_ = nullptr;
  ID3D11DeviceContext* context_ = nullptr;
};
