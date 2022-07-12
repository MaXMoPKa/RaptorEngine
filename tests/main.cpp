#include <gmock/gmock.h>  // for InitGoogleMock
#include <gtest/gtest.h>  // for RUN_ALL_TESTS

int main(int argc, char** argv) {
  testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
