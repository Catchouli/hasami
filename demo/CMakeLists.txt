add_executable(demo
  src/demo.cpp
)

target_link_libraries(demo hasami glad glm SDL2 tiny_obj_loader)