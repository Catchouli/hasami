project(simplefilewatcher VERSION 0.1 LANGUAGES CXX)

add_library(simplefilewatcher
  source/FileWatcher.cpp
  source/FileWatcherWin32.cpp
)

target_include_directories(simplefilewatcher
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    source
  )