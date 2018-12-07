#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Renderer.h"

class Scene
{
public:
  virtual bool Initialize(Renderer* renderer) = 0;
  virtual bool Update(Renderer* renderer, float elapsed) = 0;
  virtual bool Render(Renderer* renderer) = 0;

  virtual void TransformMatrices(const DirectX::XMMATRIX& world, 
    const DirectX::XMMATRIX& view, 
    const DirectX::XMMATRIX& projection)
  {
    world_ = world;
    view_ = view;
    projection_ = projection;
  }

  virtual ~Scene(){};
protected:
  Scene(){};

  DirectX::XMMATRIX                world_{};
  DirectX::XMMATRIX                view_{};
  DirectX::XMMATRIX                projection_{};
};
