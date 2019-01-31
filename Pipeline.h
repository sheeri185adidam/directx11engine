#pragma once

#include <map>
#include <memory>
#include <d3d11.h>
#include "InputAssemblerStage.h"
#include "StreamOutputStage.h"

class ShaderStage;

class Pipeline
{
public:
  Pipeline(ID3D11DeviceContext* context);
  virtual ~Pipeline();

  virtual ID3D11DeviceContext* Context();
  virtual InputAssemblerStage* GetInputAssemblerStage();
  virtual ShaderStage* GetVertexShaderStage();
  virtual ShaderStage* GetGeometryShaderStage();
  virtual StreamOutputStage* GetStreamOutputStage();
  virtual ShaderStage* GetPixelShaderStage();

protected:
  ID3D11DeviceContext* context_;
  std::shared_ptr<InputAssemblerStage> input_assembler_stage_;
  std::shared_ptr<ShaderStage> vertex_shader_stage_;
  std::shared_ptr<ShaderStage> geometry_shader_stage_;
  std::shared_ptr<StreamOutputStage> stream_output_stage_;
  std::shared_ptr<ShaderStage> pixel_shader_stage_;
};
