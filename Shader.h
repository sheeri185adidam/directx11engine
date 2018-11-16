#pragma once
#include <string>
#include <d3d11.h>

class ShaderSource;

class Shader
{
public:
  virtual ~Shader() = default;
  virtual bool Create(ID3D11Device* device, const ShaderSource* source) = 0;
  virtual bool IsCreated() = 0;
  virtual const std::string& Name() { return name_; }

protected:
  Shader() = default;
  std::string name_;
};
