#pragma once

#include <map>
#include <memory>
#include <d3d11.h>
#include "InputAssemblerStage.h"

class ShaderStage;

class Pipeline
{
public:
  Pipeline(ID3D11DeviceContext* context);
  virtual ~Pipeline();

  virtual ID3D11DeviceContext* Context();
  virtual ShaderStage* GetVertexShaderStage();
  virtual ShaderStage* GetPixelShaderStage();
  virtual InputAssemblerStage* GetInputAssemblerStage();

protected:
  virtual ShaderStage* GetShaderStage(const std::string& stage);

  ID3D11DeviceContext* context_;
  std::map<std::string, std::shared_ptr<ShaderStage> > stages_;
  std::shared_ptr<InputAssemblerStage> input_assembler_stage_;
};
