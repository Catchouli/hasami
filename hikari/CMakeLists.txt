enable_language(CUDA)

# Example of how to add a cuda library
#add_library(hikari_cuda
#  src/trace.cu
#)
#set_target_properties(hikari_cuda PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
#set_target_properties(hikari_cuda PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_executable(hikari
  src/main.cpp
  src/hikari.hpp
  src/hikari.cpp
  src/trace.cu
  src/trace.hpp
)

enableCXX17(hikari)

target_link_libraries(hikari hasami glad glm SDL2 tiny_obj_loader FreeImage imgui stb)