#pragma once
#include <d3d11.h>
#include <vector>
#include <cassert>

class StreamOutputStage
{
public:
  StreamOutputStage(ID3D11DeviceContext* context, uint32_t count)
    : context_(context)
    , target_count_(count)
    , targets_(new std::vector<ID3D11Buffer*>(target_count_))
    , offsets_(new std::vector<uint32_t>(target_count_))
  {
    assert(context_ != nullptr);
  }

  StreamOutputStage(ID3D11DeviceContext* context)
    : StreamOutputStage(context, 4)
  {
  }

  virtual ~StreamOutputStage()
  {
    delete targets_;
    targets_ = nullptr;

    delete offsets_;
    offsets_ = nullptr;
  }

  virtual void SetTargets(uint32_t count, ID3D11Buffer * const * buffers, const uint32_t* offsets)
  {
    if (buffers == nullptr || offsets == nullptr)
      return;

    if (count > target_count_)
      return;

    RemoveTargets();
    for(uint32_t i = 0; i < count; ++i)
    {
      (*targets_)[i] = buffers[i];
      (*offsets_)[i] = offsets[i];
    }
  }

  virtual void SetTarget(ID3D11Buffer* buffer, uint32_t offset)
  {
    SetTargets(1, &buffer, &offset);
  }

  virtual uint32_t TargetCount() { return target_count_; }

protected:
  virtual void RemoveTargets()
  {
    for(uint32_t i=0; i<target_count_;++i)
    {
      (*targets_)[i] = nullptr;
      (*offsets_)[i] = 0;
    }
  }

private:
  ID3D11DeviceContext* context_ = nullptr;
  uint32_t target_count_;
  std::vector<ID3D11Buffer*>* targets_ = nullptr;
  std::vector<uint32_t>* offsets_ = nullptr;
};

