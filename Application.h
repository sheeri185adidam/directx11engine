#pragma once

#include "Renderer.h"
#include <memory>
#include <string>
#include "Renderer.h"
#include "SceneManager.h"

class Application
{
public:
  Application(std::shared_ptr<Renderer> renderer, std::shared_ptr<SceneManager> scene_manager);
  ~Application();

  HRESULT Initialize();

  static LRESULT CALLBACK StaticWindowProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
  );

  HRESULT Run();

private:
  static POINTS HandleMouseInput(HWND hWnd, LPARAM lParam);

  HMENU     m_hMenu;
  RECT      m_rc;
  HWND      m_hWnd;

  std::shared_ptr<Renderer> renderer_;
  std::shared_ptr<SceneManager> scene_manager_;

};

static HINSTANCE m_hInstance;
static std::wstring m_windowClassName;