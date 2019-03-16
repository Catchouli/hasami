project(hasami_test VERSION 0.1 LANGUAGES CXX)

add_executable(hasami_test
  gl/window.cpp
)

target_link_libraries(hasami_test hasami catch)

target_include_directories(hasami_test PRIVATE src)