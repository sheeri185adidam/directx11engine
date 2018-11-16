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
    m_World = world;
    m_View = view;
    m_Projection = projection;
  }

  virtual ~Scene(){};
protected:
  Scene(){};

  DirectX::XMMATRIX                m_World{};
  DirectX::XMMATRIX                m_View{};
  DirectX::XMMATRIX                m_Projection{};
};
