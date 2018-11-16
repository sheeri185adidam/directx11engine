#pragma once
#include "ShaderSource.h"
#include "PixelShader.h"

class PixelShaderSource : public ShaderSource
{
public:
  PixelShaderSource(const std::string& name, const std::wstring& filename,
    const std::string& function, const std::string& model)
    : ShaderSource(name, filename, function, model)
  {
    type_ = ShaderType::PIXEL_SHADER;
  }

  virtual ~PixelShaderSource() = default;

  Shader* CreateShader(ID3D11Device* device) override
  {
    if (!IsCompiled())
    {
      const auto result = Compile();
      if (result == nullptr)
        return nullptr;
    }
    return new PixelShader(device, this);
  }
};
