#include "demo.hpp"

#include "gl/sdlwindow.hpp"

int main(int argc, char** argv)
{
  try {
    // Create app after the window so we have a gl context
    hs::sdl::Window window;
    Demo app;
    window.setApp(&app);
    window.run();
  }
  catch (...) {
    return 1;
  }

  return 0;
}