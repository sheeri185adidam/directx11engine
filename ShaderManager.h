#pragma once
#include "ShaderSource.h"
#include <string>
#include <map>
#include "VertexShaderSource.h"
#include "PixelShaderSource.h"
#include "Shader.h"

class ShaderManager
{
public:
  static ShaderManager& Instance()
  {
    static ShaderManager manager;
    return manager;
  }

  virtual ~ShaderManager()
  {
    shader_sources_.clear();
    shaders_.clear();
  }

  virtual bool AddShaderSource(ShaderSource* shader)
  {
    if (shader == nullptr)
      return false;

    auto name = shader->Name();
    const auto result = shader_sources_.find(name);
    if (result != shader_sources_.end())
      return false;
    shader_sources_.insert(std::make_pair(name, std::shared_ptr<ShaderSource>(shader)));
    return true;
  }

  virtual bool AddShaderSource(const std::string& name, ShaderType type, const std::wstring& filename, 
    const std::string& function, const std::string& model)
  {
    const auto result = shader_sources_.find(name);
    if (result != shader_sources_.end())
      return false;

    std::shared_ptr<ShaderSource> source = nullptr;
    switch(type)
    {
      case ShaderType::VERTEX_SHADER:
        shader_sources_.insert(std::make_pair(name, std::make_shared<VertexShaderSource>(name, filename, function, model)));
        break;
      case ShaderType::PIXEL_SHADER:
        shader_sources_.insert(std::make_pair(name, std::make_shared<PixelShaderSource>(name, filename, function, model)));
        break;
      default:
        return false;
    }
    return true;
  }

  virtual ShaderSource* GetShaderSource(const std::string& name)
  {
    const auto result = shader_sources_.find(name);
    if (result == shader_sources_.end())
      return nullptr;
    return result->second.get();
  }

  virtual void RemoveShaderSource(const std::string& name)
  {
    const auto result = shader_sources_.find(name);
    if (result == shader_sources_.end())
      return;

    shader_sources_.erase(result);
  }

  virtual bool LoadShaders(ID3D11Device* device)
  {
    for (auto& pair : shader_sources_)
    {
      const auto source = pair.second;
      if (!source->IsCompiled())
      {
        const auto result = source->Compile();
        if (result == nullptr)
          return false;
      }

      shaders_.insert(std::make_pair(source->Name(), std::shared_ptr<Shader>(source->CreateShader(device))));
    }
    return true;
  }

  virtual Shader* GetShader(const std::string& name)
  {
    const auto result = shaders_.find(name);
    if (result == shaders_.end())
      return nullptr;
    return result->second.get();
  }

  virtual void UnloadShaders()
  {
    shaders_.clear();
  }

  virtual void UnloadShader(const std::string& name)
  {
    const auto result = shaders_.find(name);
    if (result == shaders_.end())
      return;
    shaders_.erase(result);
  }

private:
  ShaderManager() = default;

  std::map<std::string, std::shared_ptr<ShaderSource>> shader_sources_;
  std::map<std::string, std::shared_ptr<Shader>> shaders_;
};
