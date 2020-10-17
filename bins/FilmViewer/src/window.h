#pragma once

#include <Windows.h>
#include <nucleus/Types.h>

#include <memory>

class Window {
public:
  static std::unique_ptr<Window> create(I32 width, I32 height);

  void show();

private:
  friend LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);

  Window(I32 width, I32 height);

  I32 m_width;
  I32 m_height;

  HWND m_handle = nullptr;
  HBITMAP m_bitmap = nullptr;
};
