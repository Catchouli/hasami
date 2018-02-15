#pragma once

void test();

namespace hs
{
  class Renderer;

  class Window
  {
    virtual ~Window() {}
    virtual Renderer* getRenderer() = 0;
  };

  class Renderer
  {
    virtual ~Renderer() {}
  };
}