#include <string>
#include <memory>

#include "Renderer.h"
#include "Application.h"
#include "CubeScene.h"

//-----------------------------------------------------------------------------
// Main function: Creates window, calls initialization functions, and hosts
// the render loop.
//-----------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  HRESULT hr = S_OK;

  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  // Begin initialization.

  // Create Cube Scene
  auto scene = new CubeScene();
  auto scene_manager = std::make_shared<SceneManager>();
  scene_manager->AddScene(scene);

  auto renderer = std::make_shared<Renderer>();

  auto app = std::make_shared<Application>(renderer, scene_manager);

  // Create a window.
  hr = app->Initialize();

  if (SUCCEEDED(hr))
  {
    // Run the program.
    hr = app->Run();
  }

  // Cleanup is handled in destructors.
  return hr;
}