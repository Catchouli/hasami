#include <stdio.h>
#include <cstring>
#include <cassert>

#include "cuda.h"
#include "cuda_runtime_api.h"
#include "glad/glad.h"
#include "cuda_gl_interop.h"

#include "trace.hpp"

void cudaCheckErr() {
  // check for error
  cudaError_t error = cudaGetLastError();
  if (error != cudaSuccess) {
    // print the CUDA error message and exit
    printf("CUDA error: %s\n", cudaGetErrorString(error));
    exit(-1);
  }
}

void cudaAssert(cudaError_t code, const char *file, int line, bool abort)
{
  if (code != cudaSuccess) {
    fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);

    system("pause");

    if (abort) {
      exit(code);
    }
  }
}

CudaSurface::CudaSurface(unsigned int texid)
  : m_cudaGraphicsResource(nullptr)
  , m_cudaArray(nullptr)
  , m_mapped(false)
{
  // todo: make this short circuit if one fails
  cudaCheck(cudaGraphicsGLRegisterImage(&m_cudaGraphicsResource, texid, GL_TEXTURE_2D, CU_GRAPHICS_REGISTER_FLAGS_SURFACE_LDST));
}

CudaSurface::~CudaSurface()
{
  if (m_mapped) {
    unmap();
  }
}


cudaSurfaceObject_t CudaSurface::map() {
  assert(!m_mapped);
  if (!m_mapped) {
    m_mapped = true;

    cudaCheck(cudaGraphicsMapResources(1, &m_cudaGraphicsResource));
    cudaCheck(cudaGraphicsSubResourceGetMappedArray(&m_cudaArray, m_cudaGraphicsResource, 0, 0));

    cudaResourceDesc pixelDescription;
    memset(&pixelDescription, 0, sizeof(pixelDescription));
    pixelDescription.resType = cudaResourceTypeArray;
    pixelDescription.res.array.array = m_cudaArray;
    cudaCheck(cudaCreateSurfaceObject(&m_pixelSurface, &pixelDescription));
  }

  return m_pixelSurface;
}

void CudaSurface::unmap() {
  assert(m_mapped);
  if (m_mapped) {
    m_mapped = false;

    cudaCheck(cudaGraphicsUnmapResources(1, &m_cudaGraphicsResource));
    cudaCheck(cudaDestroySurfaceObject(m_pixelSurface));
  }
}

__global__
void trace_kernel(cudaSurfaceObject_t pixelSurface, float time) {
  int idx = blockIdx.x;
  int idy = blockIdx.y;
  surf2Dwrite<float4>(float4{fmod(time / 6.0f, 1.0f), 0.0f, 0.0f, 1.0f}, pixelSurface, idx * 16, idy);
}

int trace(CudaSurface* cudaSurface, float time) {
  auto surf = cudaSurface->map();
  trace_kernel<<<dim3(800, 800, 1), 1>>>(surf, time);
  cudaCheckErr();
  cudaSurface->unmap();
  return 0;
}