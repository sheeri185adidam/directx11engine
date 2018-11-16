#pragma once
#include "Scene.h"
#include <memory>
#include <list>
#include "Renderer.h"

class SceneManager
{
public:
  SceneManager(){}
  virtual ~SceneManager(){}

  virtual void AddScene(Scene* scene)
  {
    scene_list_.push_back(std::shared_ptr<Scene>(scene));
  }

  virtual void InitializeScenes(Renderer* renderer)
  {
    for (auto& scene : scene_list_)
    {
      scene->Initialize(renderer);
    }
  }

  virtual void UpdateScenes(Renderer* renderer, float elapsed)
  {
    for (auto& scene : scene_list_)
    {
      scene->Update(renderer, elapsed);
    }
  }

  virtual void RenderScenes(Renderer* renderer)
  {
    for (auto& scene : scene_list_)
    {
      scene->Render(renderer);
    }
  }

private:
  std::list<std::shared_ptr<Scene>> scene_list_;
};
