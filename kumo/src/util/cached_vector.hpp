#pragma once

#include <vector>
#include <cstdio>
#include <functional>
#include <cassert>

namespace internal {

  /// Write a vector to a file as binary with a version tag
  template <typename T>
  void writeVector(const char* filename, const int version, const std::vector<T>& data)
  {
    FILE* fd = fopen(filename, "wb");
    if (!fd) {
      fprintf(stderr, "Failed to write vector\n");
      return;
    }

    int32_t v = int32_t(version);
    fwrite(&v, sizeof(int32_t), 1, fd);
    fwrite(data.data(), sizeof(T), data.size(), fd);
    fclose(fd);
  }

  /// Read a vector from a file as binary with a version tag
  /// Returns false if the file could not be loaded or the version differed
  template <typename T>
  bool tryReadVector(const char* filename, const int version, std::vector<T>& data)
  {
    data.clear();

    FILE* fd = fopen(filename, "rb");
    if (!fd)
      return false;

    // Check version correct
    int32_t curVersion = 0;
    if (!fread(&curVersion, sizeof(int32_t), 1, fd) || version != curVersion) {
      fclose(fd);
      return false;
    }

    // Get length of data
    const long startPos = ftell(fd);
    fseek(fd, 0, SEEK_END);
    const long endPos = ftell(fd);
    fseek(fd, startPos, SEEK_SET);
    const long size = endPos - startPos;

    // Check length is correct
    assert(size % sizeof(T) == 0);
    if (size % sizeof(T) != 0) {
      fprintf(stderr, "Failed to read vector from file, length of file was wrong\n");
      return false;
    }

    // Size vector
    const size_t elements = size_t(size) / sizeof(T);
    data.resize(elements);

    // Read data
    fread(data.data(), size, 1, fd);

    fclose(fd);
    return true;
  }

}

// Generate a vector and cache it to disk, raeding it from the disk for future runs
// If the generate function changes, increment the version to invalidate old cache files
template <typename T>
void cacheVector(const char* filename, int version, std::vector<T>& data, std::function<void(std::vector<T>&)> generate)
{
  using namespace internal;
  if (!tryReadVector(filename, version, data)) {
    generate(data);
    writeVector(filename, version, data);
  }
}