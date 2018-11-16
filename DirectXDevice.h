#pragma once
#include "practice.h"
#include <cstdint>
#include <d3d11.h>
#include <wrl.h>
#include <dxgi1_3.h>

class DirectXDevice
{
public:
  virtual bool Initialize() 
  {
    HRESULT hr = S_OK;

    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // This flag adds support for surfaces with a color-channel ordering different
    // from the API default. It is required for compatibility with Direct2D.
    UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // Create the Direct3D 11 API device object and a corresponding context.
    ID3D11Device*        device;
    ID3D11DeviceContext* context = nullptr;

    hr = D3D11CreateDevice(
      nullptr,                    // Specify nullptr to use the default adapter.
      D3D_DRIVER_TYPE_HARDWARE,   // Create a device using the hardware graphics driver.
      0,                          // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
      deviceFlags,                // Set debug and Direct2D compatibility flags.
      levels,                     // List of feature levels this app can support.
      ARRAYSIZE(levels),          // Size of the list above.
      D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
      &device,                    // Returns the Direct3D device created.
      &feature_level_,            // Returns feature level of device created.
      &context                    // Returns the device immediate context.
    );

    if (FAILED(hr))
    {
      return false;
    }

    // Store pointers to the Direct3D 11.1 API device and immediate context.
    device_ = device;
    context_ = context;
    return true;
  }

  virtual IDXGISwapChain* SwapChain(HWND wnd)
  {
    HRESULT hr = S_OK;
    IDXGISwapChain* swapchain = nullptr;

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
    desc.Windowed = TRUE; // Sets the initial state of full-screen mode.
    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SampleDesc.Count = 1;      //multisampling setting
    desc.SampleDesc.Quality = 0;    //vendor-specific flag
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    desc.OutputWindow = wnd;

    // Create the DXGI device object to use in other factories, such as Direct2D.
    IDXGIDevice3* dxgiDevice;
    hr = device_->QueryInterface(__uuidof(IDXGIDevice3), reinterpret_cast<void**>(&dxgiDevice));
    if (FAILED(hr))
      return nullptr;

    // Create swap chain.
    IDXGIAdapter* adapter;
    IDXGIFactory* factory;

    hr = dxgiDevice->GetAdapter(&adapter);
    dxgiDevice->Release();
    if (FAILED(hr))
      return nullptr;

    adapter->GetParent(IID_PPV_ARGS(&factory));
    hr = factory->CreateSwapChain(device_, &desc, &swapchain);
    adapter->Release();
    factory->Release();
    return swapchain;
  }

  virtual ID3D11Buffer* VertexBuffer(uint32_t size, bool cpu_writable, bool streamout, D3D11_SUBRESOURCE_DATA* data)
  {
    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = size;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    if (streamout)
    {
      desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
    }
    else 
    {
      desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    }

    if (cpu_writable)
    {
      desc.Usage = D3D11_USAGE_DYNAMIC;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else 
    {
      desc.Usage = D3D11_USAGE_IMMUTABLE;
      desc.CPUAccessFlags = 0;

    }

    return CreateBuffer(&desc, data);
  }

  virtual ID3D11Buffer* IndexBuffer(uint32_t size, bool cpu_writable, D3D11_SUBRESOURCE_DATA* data)
  {
    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = size;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    if (cpu_writable)
    {
      desc.Usage = D3D11_USAGE_DYNAMIC;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else
    {
      desc.Usage = D3D11_USAGE_IMMUTABLE;
      desc.CPUAccessFlags = 0;
    }

    return CreateBuffer(&desc, data);
  }

  virtual ID3D11Buffer* ConstantBuffer(uint32_t size, bool gpu_writable, bool cpu_writable, D3D11_SUBRESOURCE_DATA* data)
  {
    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = size;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    if (cpu_writable)
    {
      desc.Usage = D3D11_USAGE_DYNAMIC;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else
    {
      if (gpu_writable)
      {
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
      }
      else
      {
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.CPUAccessFlags = 0;
      }
    }

    return CreateBuffer(&desc, data);
  }
  
  virtual ID3D11Buffer* StructuredBuffer(uint32_t count, uint32_t structsize, bool cpu_writable, bool gpu_writable, D3D11_SUBRESOURCE_DATA* data)
  {
    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = count*structsize;
    desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    desc.StructureByteStride = structsize;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    if (!cpu_writable && !gpu_writable)
    {
      desc.Usage = D3D11_USAGE_IMMUTABLE;
      desc.CPUAccessFlags = 0;
    }
    else if (cpu_writable && !gpu_writable)
    {
      desc.Usage = D3D11_USAGE_DYNAMIC;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else if (!cpu_writable && gpu_writable)
    {
      desc.Usage = D3D11_USAGE_DEFAULT;
      desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
      desc.CPUAccessFlags = 0;
    }
    else
    {
      // error
      // cpu and gpu can't write together
      return nullptr;
    }
    return CreateBuffer(&desc, data);
  }

  virtual void* Texture1D(uint32_t width, uint32_t miplevels, uint32_t array_size, DXGI_FORMAT format,
    uint32_t bindflags, bool gpu_writable, bool cpu_writable, uint32_t misc_flags, D3D11_SUBRESOURCE_DATA* data)
  {
    D3D11_TEXTURE1D_DESC desc;
    desc.Width = width;
    desc.MipLevels = miplevels;
    desc.ArraySize = array_size;
    desc.Format = format;
    desc.BindFlags = bindflags;

    if (!cpu_writable && !gpu_writable)
    {
      desc.Usage = D3D11_USAGE_IMMUTABLE;
      desc.CPUAccessFlags = 0;
    }
    else if (cpu_writable && !gpu_writable)
    {
      desc.Usage = D3D11_USAGE_DYNAMIC;
      desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else if (!cpu_writable && gpu_writable)
    {
      desc.Usage = D3D11_USAGE_DEFAULT;
      desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
      desc.CPUAccessFlags = 0;
    }
    else
    {
      // error
      // cpu and gpu can't write together
      return nullptr;
    }

    auto device = Device();
    if (device == nullptr)
      return nullptr;

    ID3D11Texture1D* texture = nullptr;
    auto hr = device->CreateTexture1D(&desc, data, &texture);
    if (FAILED(hr))
      return nullptr;

    return texture;
  }

  // ShaderSource Resource View (SRV)
  virtual ID3D11ShaderResourceView* BufferShaderResourceView(ID3D11Resource* resource, DXGI_FORMAT format, uint32_t offset, uint32_t width)
  {
    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    desc.Format = format;
    desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    desc.Buffer.ElementOffset = offset;
    desc.Buffer.ElementWidth = width;

    auto device = Device();
    if (device == nullptr)
      return nullptr;

    ID3D11ShaderResourceView* view = nullptr;
    auto hr = device->CreateShaderResourceView(resource,&desc, &view);
    return view;
  }
   
  virtual ID3D11ShaderResourceView* Texture1DShaderResourceView(ID3D11Resource* resource, DXGI_FORMAT format, uint32_t most_detailed_mip, uint32_t mip_levels)
  {
    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    desc.Format = format;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
    desc.Texture1D.MostDetailedMip = most_detailed_mip;
    desc.Texture1D.MipLevels = mip_levels;

    auto device = Device();
    if (device == nullptr)
      return nullptr;

    ID3D11ShaderResourceView* view = nullptr;
    auto hr = device->CreateShaderResourceView(resource, &desc, &view);
    return view;
  }

  virtual ID3D11ShaderResourceView* Texture1DArrayShaderResourceView(ID3D11Resource* resource, DXGI_FORMAT format, 
    uint32_t most_detailed_mip, uint32_t mip_levels, uint32_t first_array_slice, uint32_t array_size)
  {
    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    desc.Format = format;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
    desc.Texture1DArray.MostDetailedMip = most_detailed_mip;
    desc.Texture1DArray.MipLevels = mip_levels;
    desc.Texture1DArray.FirstArraySlice = first_array_slice;
    desc.Texture1DArray.ArraySize = array_size;

    auto device = Device();
    if (device == nullptr)
      return nullptr;

    ID3D11ShaderResourceView* view = nullptr;
    auto hr = device->CreateShaderResourceView(resource, &desc, &view);
    return view;
  }

  // Unordered Access View (UAV)
  virtual ID3D11UnorderedAccessView* Texture1DUnorderedAccessView(ID3D11Resource* resource, DXGI_FORMAT format, uint32_t mip_slice)
  {
    D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
    desc.Format = format;
    desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
    desc.Texture1D.MipSlice = mip_slice;

    auto device = Device();
    if (device == nullptr)
      return nullptr;

    ID3D11UnorderedAccessView* view = nullptr;
    auto hr = device->CreateUnorderedAccessView(resource, &desc, &view);
    return view;
  }

  virtual ID3D11UnorderedAccessView* Texture1DArrayUnorderedAccessView(ID3D11Resource* resource, DXGI_FORMAT format, 
    uint32_t mip_slice, uint32_t first_array_slice, uint32_t array_size)
  {
    D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
    desc.Format = format;
    desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
    desc.Texture1DArray.MipSlice = mip_slice;
    desc.Texture1DArray.FirstArraySlice = first_array_slice;
    desc.Texture1DArray.ArraySize = array_size;

    auto device = Device();
    if (device == nullptr)
      return nullptr;

    ID3D11UnorderedAccessView* view = nullptr;
    auto hr = device->CreateUnorderedAccessView(resource, &desc, &view);
    return view;
  }

  // Render Target view (RTV)
  virtual ID3D11RenderTargetView* Texture1DRenderTargetView(ID3D11Resource* resource, DXGI_FORMAT format, uint32_t mip_slice)
  {
    D3D11_RENDER_TARGET_VIEW_DESC desc;
    desc.Format = format;
    desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
    desc.Texture1D.MipSlice = mip_slice;

    auto device = Device();
    if (device == nullptr)
      return nullptr;

    ID3D11RenderTargetView* view = nullptr;
    auto hr = device->CreateRenderTargetView(resource, &desc, &view);
    return view;
  }

  virtual ID3D11RenderTargetView* Texture1DArrayRenderTargetView(ID3D11Resource* resource, DXGI_FORMAT format, 
    uint32_t mip_slice, uint32_t first_array_slice, uint32_t array_size)
  {
    D3D11_RENDER_TARGET_VIEW_DESC desc;
    desc.Format = format;
    desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
    desc.Texture1DArray.MipSlice = mip_slice;
    desc.Texture1DArray.FirstArraySlice = first_array_slice;
    desc.Texture1DArray.ArraySize = array_size;

    auto device = Device();
    if (device == nullptr)
      return nullptr;

    ID3D11RenderTargetView* view = nullptr;
    auto hr = device->CreateRenderTargetView(resource, &desc, &view);
    return view;
  }

  // Depth Stencil View (DSV)
  virtual ID3D11DepthStencilView* Texture1DDepthStencilView(ID3D11Resource* resource, DXGI_FORMAT format, uint32_t mip_slice)
  {
    D3D11_DEPTH_STENCIL_VIEW_DESC desc;
    desc.Format = format;
    desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
    desc.Texture1D.MipSlice = mip_slice;

    auto device = Device();
    if (device == nullptr)
      return nullptr;

    ID3D11DepthStencilView* view = nullptr;
    auto hr = device->CreateDepthStencilView(resource, &desc, &view);
    return view;
  }

  virtual ID3D11DepthStencilView* Texture1DArrayDepthStencilView(ID3D11Resource* resource, DXGI_FORMAT format, 
    uint32_t mip_slice, uint32_t first_array_slice, uint32_t array_size)
  {
    D3D11_DEPTH_STENCIL_VIEW_DESC desc;
    desc.Format = format;
    desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
    desc.Texture1DArray.MipSlice = mip_slice;
    desc.Texture1DArray.FirstArraySlice = first_array_slice;
    desc.Texture1DArray.ArraySize = array_size;

    auto device = Device();
    if (device == nullptr)
      return nullptr;

    ID3D11DepthStencilView* view = nullptr;
    auto hr = device->CreateDepthStencilView(resource, &desc, &view);
    return view;
  }

  virtual ID3D11DeviceContext* Context() { return context_; }

  virtual bool RenderTargetView(ID3D11Resource *resource, const D3D11_RENDER_TARGET_VIEW_DESC *desc, ID3D11RenderTargetView **view)
  {
    auto device = Device();
    if (device == nullptr)
      return false;
    const auto result = device->CreateRenderTargetView(resource, desc, view);
    return !FAILED(result);
  }

  virtual bool DepthStencilView(ID3D11Resource *resource, const D3D11_DEPTH_STENCIL_VIEW_DESC *desc, ID3D11DepthStencilView **view)
  {
    auto device = Device();
    if (device == nullptr)
      return false;
    const auto result = device->CreateDepthStencilView(resource, desc, view);
    return !FAILED(result);
  }

  virtual ID3D11Device* Device() { return device_; }

  virtual ~DirectXDevice() 
  {
    SAFE_RELEASE(context_);
    SAFE_RELEASE(device_);
  }

protected:
  virtual ID3D11Buffer* CreateBuffer(const D3D11_BUFFER_DESC *desc, const D3D11_SUBRESOURCE_DATA *data)
  {
    auto device = Device();
    if (device == nullptr)
      return nullptr;

    ID3D11Buffer* buffer = nullptr;
    auto hr = device->CreateBuffer(desc, data, &buffer);
    if (FAILED(hr))
      return nullptr;

    return buffer;
  }
  virtual bool IsInitialized() { return device_ != nullptr; }

  D3D_FEATURE_LEVEL feature_level_ = {};
  ID3D11Device* device_ = nullptr;
  ID3D11DeviceContext* context_ = nullptr;
};
