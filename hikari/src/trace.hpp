#pragma once

#include "cuda.h"
#include "cuda_runtime.h"

struct cudaArray;
struct cudaGraphicsResource;

class CudaSurface
{
public:
  CudaSurface(unsigned int texid);
  ~CudaSurface();

  CudaSurface(const CudaSurface&) = delete;
  CudaSurface& operator=(const CudaSurface&) = delete;

  cudaTextureObject_t map();
  void unmap();

private:
  bool m_mapped;
  cudaArray* m_cudaArray;
  cudaSurfaceObject_t m_pixelSurface;
  cudaGraphicsResource* m_cudaGraphicsResource;
};

#define cudaCheck(code) { cudaAssert((code), __FILE__, __LINE__, false); }

void cudaCheckErr();
void cudaAssert(cudaError_t code, const char *file, int line, bool abort);

int trace(CudaSurface* pixelBuf, float time);