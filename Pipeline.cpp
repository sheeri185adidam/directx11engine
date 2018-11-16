#include "Pipeline.h"
#include "VertexShaderStage.h"
#include "PixelShaderStage.h"

Pipeline::Pipeline(ID3D11DeviceContext* context)
  : context_(context)
{
  input_assembler_stage_ = std::make_shared<InputAssemblerStage>(context_);
  stages_.insert(std::make_pair("vertex_shader", std::make_shared<VertexShaderStage>(context_)));
  stages_.insert(std::make_pair("pixel_shader", std::make_shared<PixelShaderStage>(context_)));
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

ShaderStage* Pipeline::GetShaderStage(const std::string& stage)
{
  const auto result = stages_.find(stage);
  if (result == stages_.end())
    return nullptr;
  return result->second.get();
}

ShaderStage* Pipeline::GetVertexShaderStage()
{
  return GetShaderStage("vertex_shader");
}

ShaderStage* Pipeline::GetPixelShaderStage()
{
  return GetShaderStage("pixel_shader");
}

InputAssemblerStage* Pipeline::GetInputAssemblerStage() { return input_assembler_stage_.get(); }
