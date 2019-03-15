add_executable(kumo
  src/main.cpp
  src/demo.cpp
  src/demo.hpp
  src/clouds.cpp
  src/clouds.hpp
  src/globe.cpp
  src/globe.hpp
)

target_link_libraries(kumo hasami glad glm SDL2 tiny_obj_loader FreeImage imgui)