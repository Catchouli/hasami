#include "hikari.hpp"

#include "backends/sdl/sdlwindow.hpp"
#include "backends/gl/glrenderer.hpp"

int main(int argc, char** argv)
{
  try {
    // Create app after the window so we have a gl context
    hs::sdl::Window<hs::gl::GLRenderer> window(true);
    Hikari app;
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
