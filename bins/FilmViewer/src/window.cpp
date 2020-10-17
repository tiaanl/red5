#include "window.h"

#include <cassert>

LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);

namespace {

const char* g_windowClassName = "mainWindowClass";

void ensureWindowClass(HINSTANCE instance) {
  WNDCLASSEXA wc = {};

  if (!GetClassInfoExA(instance, g_windowClassName, &wc)) {
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
    wc.lpfnWndProc = windowProc;
    wc.hInstance = instance;
    wc.hIcon = LoadIconA(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    wc.lpszClassName = g_windowClassName;
    wc.hIconSm = LoadIconA(nullptr, IDI_APPLICATION);

    RegisterClassExA(&wc);
  }
}
}  // namespace

// static
std::unique_ptr<Window> Window::create(I32 width, I32 height) {
  HINSTANCE instance = GetModuleHandleA(nullptr);

  ensureWindowClass(instance);

  DWORD exStyle = WS_EX_APPWINDOW;
  DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

  RECT windowRect{0, 0, width, height};
  if (!AdjustWindowRectEx(&windowRect, style, FALSE, exStyle)) {
    return {};
  }

  windowRect.right -= windowRect.left;
  windowRect.bottom -= windowRect.top;

  windowRect.left = (GetSystemMetrics(SM_CXFULLSCREEN) - windowRect.right) / 2;
  windowRect.top = (GetSystemMetrics(SM_CYFULLSCREEN) - windowRect.bottom) / 2;

  // Long syntax to allow access to private constructor.
  std::unique_ptr<Window> result(new Window{width, height});

  HWND handle = CreateWindowExA(exStyle, g_windowClassName, "Window", style, windowRect.left,
                                windowRect.top, windowRect.right, windowRect.bottom, HWND_DESKTOP,
                                nullptr, instance, result.get());
  if (!handle) {
    return {};
  }

  return result;
}

LRESULT CALLBACK windowProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  Window* window = nullptr;

  if (msg == WM_CREATE) {
    window = static_cast<Window*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
    SetWindowLongPtrA(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
  } else {
    window = reinterpret_cast<Window*>(GetWindowLongPtrA(wnd, GWLP_USERDATA));
  }

  assert(window);

  switch (msg) {
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
  }

  return DefWindowProcA(wnd, msg, wParam, lParam);
}

void Window::show() {
  ShowWindow(m_handle, SW_SHOWNORMAL);
}

Window::Window(I32 width, I32 height) : m_width{width}, m_height{height} {}
