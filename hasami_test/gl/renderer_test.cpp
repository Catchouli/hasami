#include "catch.hpp"
#include "app_test_utils.hpp"
#include "backends/sdl/sdlwindow.hpp"
#include "backends/glraw/glrenderer.hpp"

TEST_CASE("Renderer usage from window", "[renderer, gl]") {
  hs::sdl::Window<hs::gl::GLRenderer> window(true);

  SECTION("A renderer can be required") {
    withTestApp(&window, [](hs::Window* window)
    {
      REQUIRE(window != nullptr);
      auto* renderer = window->renderer();
      REQUIRE(renderer != nullptr);

      SECTION("A texture can be created") {
        REQUIRE(std::shared_ptr<hs::Texture>(window->renderer()->createTexture()) != nullptr);
      }

      SECTION("A buffer can be created") {
        REQUIRE(std::shared_ptr<hs::Buffer>(window->renderer()->createBuffer()) != nullptr);
      }

      SECTION("A shader can be created") {
        REQUIRE(std::shared_ptr<hs::Shader>(window->renderer()->createShader()) != nullptr);
      }

      SECTION("A state manager is available") {
        REQUIRE(renderer->stateManager() != nullptr);
      }
    });
  }
}