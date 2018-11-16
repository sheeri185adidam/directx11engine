#pragma once

#include <string>
#include <atomic>
#include <utility>
#include <d3dcompiler.h>
#include "Shader.h"

enum class ShaderType
{
  UNKNOWN,
  VERTEX_SHADER,
  HULL_SHADER,
  DOMAIN_SHADER,
  GEOMETRY_SHADER,
  PIXEL_SHADER,
  COMPUTE_SHADER,
};

/// <summary>A shader source class.</summary>
class ShaderSource
{
public:
  virtual ~ShaderSource()
  {
    if(compiled_blob_!=nullptr)
    {
      compiled_blob_->Release();
      compiled_blob_ = nullptr;
    }
  }

  virtual ShaderType Type() const { return type_; }
  virtual const std::string& Name() const { return name_; }
  virtual const std::wstring& Filename() const { return filename_; }
  virtual const std::string& Function() const { return function_; }
  virtual const std::string& Model() { return model_; }

  virtual ID3DBlob* Blob() const { return compiled_blob_; }
  virtual bool IsCompiled() const { return compiled_blob_ != nullptr; }
  virtual ID3DBlob* Compile()
  {
    if (IsCompiled())
      return compiled_blob_;

    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    dwShaderFlags |= D3DCOMPILE_DEBUG;
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* pErrorBlob = nullptr;
    ID3DBlob* pCompiledBlob = nullptr;
    hr = D3DCompileFromFile(filename_.c_str(), nullptr, nullptr, function_.c_str(), model_.c_str(),
      dwShaderFlags, 0, &pCompiledBlob, &pErrorBlob);
    if (FAILED(hr))
    {
      if (pErrorBlob)
      {
        OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
        pErrorBlob->Release();
      }
      return nullptr;
    }

    if (pErrorBlob) pErrorBlob->Release();
    compiled_blob_ = pCompiledBlob;
    return compiled_blob_;
  }

  virtual Shader* CreateShader(ID3D11Device* device) = 0;

protected:
  ShaderSource(std::string name, std::wstring filename,
               std::string function, std::string model)
    : name_(std::move(name))
      , type_(ShaderType::UNKNOWN)
      , filename_(std::move(filename))
      , function_(std::move(function))
      , model_(std::move(model))
      , compiled_blob_(nullptr)
  {
  }

  std::string name_;
  ShaderType type_;
  std::wstring filename_;
  std::string function_;
  std::string model_;
  ID3DBlob* compiled_blob_;
};

