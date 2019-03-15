#include "demo.hpp"

#include "backends/sdl/sdlwindow.hpp"
#include "backends/glraw/glrenderer.hpp"
#include "backends/glraw/glshader.hpp"


int main(int argc, char** argv)
{
  try {
    // Create app after the window so we have a gl context
    hs::sdl::Window<hs::gl::GLRenderer> window(true);
    Demo app(&window);
    window.setApp(&app);
    window.run();
  }
  catch (std::exception& e) {
    fprintf(stderr, "Uncaught exception: %s\n", e.what());
    system("pause");
    return 1;
  }

  return 0;
}