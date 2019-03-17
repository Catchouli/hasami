#include "../kumo/src/util/cached_vector.hpp"
#include <catch.hpp>
#include <filesystem>

template <typename T>
bool vectorEquals(const std::vector<T>& a, const std::vector<T>& b) {
  if (a.size() != b.size())
    return false;
  for (size_t i = 0; i < a.size() && i < b.size(); ++i) {
    if (a[i] != b[i]) {
      return false;
    }
  }
  return true;
}

TEST_CASE("cached vectors") {

  auto genRandomVector = [](const int size, std::vector<float>& vector) {
    for (int i = 0; i < size; ++i) {
      vector.push_back(float(rand()));
    }
  };

  SECTION("vector equality") {
    std::vector<float> testData1;
    std::vector<float> testData2;

    CHECK(vectorEquals(testData1, testData2));
    CHECK(vectorEquals(testData2, testData1));

    testData1.push_back(1.0f);

    CHECK_FALSE(vectorEquals(testData1, testData2));
    CHECK_FALSE(vectorEquals(testData2, testData1));

    testData2.push_back(1.0f);

    CHECK(vectorEquals(testData1, testData2));
    CHECK(vectorEquals(testData2, testData1));

    testData2.push_back(1.0f);

    CHECK_FALSE(vectorEquals(testData1, testData2));
    CHECK_FALSE(vectorEquals(testData2, testData1));

    testData1.clear();
    testData2.clear();

    genRandomVector(1000, testData1);
    genRandomVector(1000, testData2);

    CHECK_FALSE(vectorEquals(testData1, testData2));
    CHECK_FALSE(vectorEquals(testData2, testData1));
    CHECK(vectorEquals(testData1, testData1));
    CHECK(vectorEquals(testData2, testData2));
  }

  SECTION("vector can be cached in a file") {
    const char* filename = tmpnam(nullptr);

    const std::vector<float> testData = [&genRandomVector]() {
      std::vector<float> data;
      genRandomVector(1000, data);
      return data;
    }();

    std::vector<float> readInData;

    int timesRun = 0;
    auto genTestData = [&timesRun, &testData, &genRandomVector](std::vector<float>& vector) {
      ++timesRun;
      vector.insert(vector.end(), testData.begin(), testData.end());
    };

    // Make sure file doesn't exist initially
    std::filesystem::remove(filename);

    CHECK(timesRun == 0);
    CHECK_FALSE(vectorEquals(testData, readInData));

    // Try generating initially
    cacheVector<float>(filename, 1, readInData, genTestData);

    CHECK(timesRun == 1);
    CHECK(vectorEquals(testData, readInData));
    readInData.clear();

    // Try deleting the cache and letting it generate again
    std::filesystem::remove(filename);
    cacheVector<float>(filename, 1, readInData, genTestData);

    CHECK(timesRun == 2);
    CHECK(vectorEquals(testData, readInData));
    readInData.clear();
    CHECK_FALSE(vectorEquals(testData, readInData));

    // Try reading from cache this time
    cacheVector<float>(filename, 1, readInData, genTestData);

    CHECK(timesRun == 2);
    CHECK(vectorEquals(testData, readInData));

    // Clean up
    std::filesystem::remove(filename);
  }
}