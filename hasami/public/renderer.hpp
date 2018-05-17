#pragma once


namespace hs {

class App;

class Window
{
public:
  virtual ~Window() {}

  virtual void setApp(App* app) = 0;
  virtual void run() = 0;
};

class Renderer
{
public:
  virtual ~Renderer() {}
};

class RendererComponent
{
public:
  Renderer* renderer() const { return m_renderer; }

private:
  friend class Renderer;
  Renderer* m_renderer;
};

}