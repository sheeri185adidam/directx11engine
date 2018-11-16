#pragma once
#include "ShaderSource.h"
#include "VertexShader.h"

class VertexShaderSource : public ShaderSource
{
public:
  VertexShaderSource(const std::string& name, const std::wstring& filename,
    const std::string& function, const std::string& model)
    : ShaderSource(name, filename, function, model)
  {
    type_ = ShaderType::VERTEX_SHADER;
  }

  virtual ~VertexShaderSource() = default;

  Shader* CreateShader(ID3D11Device* device) override
  {
    if (!IsCompiled())
    {
      const auto result = Compile();
      if (result == nullptr)
        return nullptr;
    }
    return new VertexShader(device, this);
  }
};
