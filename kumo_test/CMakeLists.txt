project(kumo_test VERSION 0.1 LANGUAGES CXX)

add_executable(kumo_test
  util/cached_vector_test.cpp
)

target_link_libraries(kumo_test kumo_lib catch)

target_include_directories(kumo_test PRIVATE .)