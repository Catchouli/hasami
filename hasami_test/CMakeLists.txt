project(hasami_test VERSION 0.1 LANGUAGES CXX)

add_executable(hasami_test
  app_test_utils.hpp
  gl/window_test.cpp
  gl/renderer_test.cpp
)

target_link_libraries(hasami_test hasami catch)

target_include_directories(hasami_test PRIVATE .)