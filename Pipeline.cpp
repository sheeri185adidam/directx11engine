#include "Pipeline.h"
#include "VertexShaderStage.h"
#include "PixelShaderStage.h"
#include "GeometryShaderStage.h"

Pipeline::Pipeline(ID3D11DeviceContext* context)
  : context_(context)
{
  input_assembler_stage_ = std::make_shared<InputAssemblerStage>(context_);
  vertex_shader_stage_ = std::make_shared<VertexShaderStage>(context_);
  geometry_shader_stage_ = std::make_shared<GeometryShaderStage>(context_);
  pixel_shader_stage_ = std::make_shared<PixelShaderStage>(context_);
}

Pipeline::~Pipeline()
{
  if (context_ != nullptr)
  {
    context_->Release();
    context_ = nullptr;
  }
}

ID3D11DeviceContext* Pipeline::Context() { return context_; }

ShaderStage* Pipeline::GetVertexShaderStage() { return vertex_shader_stage_.get(); }
ShaderStage* Pipeline::GetGeometryShaderStage() { return geometry_shader_stage_.get(); }
ShaderStage* Pipeline::GetPixelShaderStage() { return pixel_shader_stage_.get(); }
InputAssemblerStage* Pipeline::GetInputAssemblerStage() { return input_assembler_stage_.get(); }
