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
  virtual ShaderStage* GetGeometryShaderStage();
  virtual ShaderStage* GetPixelShaderStage();
  virtual InputAssemblerStage* GetInputAssemblerStage();

protected:
  ID3D11DeviceContext* context_;
  std::shared_ptr<ShaderStage> vertex_shader_stage_;
  std::shared_ptr<ShaderStage> pixel_shader_stage_;
  std::shared_ptr<ShaderStage> geometry_shader_stage_;
  std::shared_ptr<InputAssemblerStage> input_assembler_stage_;
};
