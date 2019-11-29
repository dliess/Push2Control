#include <gtest/gtest.h>
#include <string>
#include "KeyValueVector.h"

using namespace util;

TEST(KeyValueVectorTest, Construct) {
    KeyValueVector<std::string, int> keyValueVector;
    ASSERT_EQ(0, keyValueVector["non-existing-key"]);
}