#include <string>
#include <memory>

#include "Application.h"
#include <iostream>

Application::Application(std::shared_ptr<Renderer> renderer, std::shared_ptr<SceneManager> scene_manager)
  : m_hMenu(nullptr)
  , m_rc()
  , m_hWnd(nullptr)
  , renderer_(renderer)
  , scene_manager_(scene_manager)
{
  m_windowClassName = L"Direct3DWindowClass";
  m_hInstance = nullptr;
}

HRESULT Application::Initialize()
{
  // Window resources are dealt with here.

  if (m_hInstance == NULL)
    m_hInstance = (HINSTANCE)GetModuleHandle(NULL);

  HICON hIcon = NULL;
  WCHAR szExePath[MAX_PATH];
  GetModuleFileName(NULL, szExePath, MAX_PATH);

  // If the icon is NULL, then use the first one found in the exe
  if (hIcon == NULL)
    hIcon = ExtractIcon(m_hInstance, szExePath, 0);

  // Register the windows class
  WNDCLASS wndClass;
  wndClass.style = CS_DBLCLKS;
  wndClass.lpfnWndProc = Application::StaticWindowProc;
  wndClass.cbClsExtra = 0;
  wndClass.cbWndExtra = 0;
  wndClass.hInstance = m_hInstance;
  wndClass.hIcon = hIcon;
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wndClass.lpszMenuName = NULL;
  wndClass.lpszClassName = m_windowClassName.c_str();

  if (!RegisterClass(&wndClass))
  {
    DWORD dwError = GetLastError();
    if (dwError != ERROR_CLASS_ALREADY_EXISTS)
      return HRESULT_FROM_WIN32(dwError);
  }

  m_rc;
  int x = CW_USEDEFAULT;
  int y = CW_USEDEFAULT;

  // No menu in this example.
  m_hMenu = NULL;

  // This example uses a non-resizable 640 by 480 viewport for simplicity.
  int nDefaultWidth = 800;
  int nDefaultHeight = 600;
  SetRect(&m_rc, 0, 0, nDefaultWidth, nDefaultHeight);
  AdjustWindowRect(
    &m_rc,
    WS_OVERLAPPEDWINDOW,
    (m_hMenu != NULL) ? true : false
  );

  // Create the window for our viewport.
  m_hWnd = CreateWindow(
    m_windowClassName.c_str(),
    L"Cube11",
    WS_OVERLAPPEDWINDOW,
    x, y,
    (m_rc.right - m_rc.left), (m_rc.bottom - m_rc.top),
    0,
    m_hMenu,
    m_hInstance,
    0
  );

  if (m_hWnd == NULL)
  {
    DWORD dwError = GetLastError();
    return HRESULT_FROM_WIN32(dwError);
  }

  // initialize renderer and scenes
  renderer_->Initialize(m_hWnd);
  scene_manager_->InitializeScenes(renderer_.get());
  return S_OK;
}

HRESULT Application::Run()
{
  HRESULT hr = S_OK;

  if (!IsWindowVisible(m_hWnd))
    ShowWindow(m_hWnd, SW_SHOW);

  // The render loop is controlled here.
  bool bGotMsg;
  MSG  msg;
  msg.message = WM_NULL;
  PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

  while (WM_QUIT != msg.message)
  {
    // Process window events.
    // Use PeekMessage() so we can use idle time to render the scene. 
    bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

    if (bGotMsg)
    {
      // Translate and dispatch the message
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      // Update the scenes.
      scene_manager_->UpdateScenes(renderer_.get(), 0.0f);
      renderer_->ClearRenderTargetAndDepthStencilBuffers();
      scene_manager_->RenderScenes(renderer_.get());
      renderer_->Present();
    }
  }

  return hr;
}


//-----------------------------------------------------------------------------
// Destructor.
//-----------------------------------------------------------------------------
Application::~Application()
{

}

//-----------------------------------------------------------------------------
// Process windows messages. This looks for window close events, letting us
// exit out of the sample.
//-----------------------------------------------------------------------------
LRESULT CALLBACK Application::StaticWindowProc(
  HWND hWnd,
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam
)
{
  switch (uMsg)
  {
    case WM_CLOSE:
    {
      HMENU hMenu;
      hMenu = GetMenu(hWnd);
      if (hMenu != NULL)
      {
        DestroyMenu(hMenu);
      }
      DestroyWindow(hWnd);
      UnregisterClass(
        m_windowClassName.c_str(),
        m_hInstance
      );
      return 0;
    }

    case WM_DESTROY:
      PostQuitMessage(0);
      break;

    case WM_LBUTTONUP:
      HandleMouseInput(hWnd, lParam);
      break;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

POINTS Application::HandleMouseInput(HWND hWnd, LPARAM lParam)
{
  RECT rcClient;                 // client area rectangle 
  POINT ptClientUL;              // client upper left corner 
  POINT ptClientLR;              // client lower right corner 
  // Capture mouse input. 

  SetCapture(hWnd);

  // Retrieve the screen coordinates of the client area, 
  // and convert them into client coordinates. 

  GetClientRect(hWnd, &rcClient);
  ptClientUL.x = rcClient.left;
  ptClientUL.y = rcClient.top;

  // Add one to the right and bottom sides, because the 
  // coordinates retrieved by GetClientRect do not 
  // include the far left and lowermost pixels. 

  ptClientLR.x = rcClient.right + 1;
  ptClientLR.y = rcClient.bottom + 1;
  ClientToScreen(hWnd, &ptClientUL);
  ClientToScreen(hWnd, &ptClientLR);

  // Copy the client coordinates of the client area 
  // to the rcClient structure. Confine the mouse cursor 
  // to the client area by passing the rcClient structure 
  // to the ClipCursor function. 

  SetRect(&rcClient, ptClientUL.x, ptClientUL.y,
    ptClientLR.x, ptClientLR.y);
  ClipCursor(&rcClient);

  // Convert the cursor coordinates into a POINTS 
  // structure, which defines the beginning point of the 
  // line drawn during a WM_MOUSEMOVE message. 

  POINTS click = MAKEPOINTS(lParam);
  ClipCursor(nullptr);
  ReleaseCapture();
  return click;
}