# Make all but the main() a library so it can be unit tested
add_library(kumo_lib
  src/demo.cpp
  src/demo.hpp
  src/clouds.cpp
  src/clouds.hpp
  src/globe.cpp
  src/globe.hpp
  src/util/cached_vector.hpp
)

enableCXX17(kumo_lib)

add_executable(kumo
  src/main.cpp
)

enableCXX17(kumo)

target_link_libraries(kumo_lib hasami glad glm SDL2 tiny_obj_loader FreeImage imgui stb)
target_link_libraries(kumo kumo_lib)