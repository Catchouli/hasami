add_executable(demo
  src/main.cpp
  src/demo.cpp
  src/demo.hpp
)

target_link_libraries(demo hasami glad glm SDL2 tiny_obj_loader FreeImage)