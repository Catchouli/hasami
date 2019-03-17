#include "catch.hpp"
#include "app_test_utils.hpp"
#include "backends/sdl/sdlwindow.hpp"
#include "backends/glraw/glrenderer.hpp"

TEST_CASE("Window can be created", "[window]") {
  hs::sdl::Window<hs::gl::GLRenderer> window(true);

  SECTION("Window::run stops when running() returns false, and render gets correctly called") {
    int updated = 0;
    TestApp testApp([&updated](auto*) { ++updated; }, [&updated]() { return updated < 5; });
    window.setApp(&testApp);

    CHECK(updated == 0);
    window.run();
    CHECK(updated == 5);
  }

  SECTION("Window renderer is available") {
    withTestApp(&window, [](hs::Window* window)
    {
      REQUIRE(window != nullptr);
      auto* renderer = window->renderer();
      REQUIRE(renderer != nullptr);
    });
  }
}