#include <gtest/gtest.h>
#include "FixedSizeVector.h"

using namespace util;

TEST(FixedSizeVectorTest, Construct) {
   FixedSizeVector<int, 10> vector1;
 //  FixedSizeVector<int, 10> vector2{1,2,3,4};
   FixedSizeVector<int, 10> vector3(vector1);
   FixedSizeVector<int, 10> vector4(FixedSizeVector<int, 10>());
}

TEST(FixedSizeVectorTest, Mutators) {
   FixedSizeVector<int, 10> vector1;
   ASSERT_EQ(0, vector1.size());
   ASSERT_NO_THROW(vector1.push_back(1));
   ASSERT_NO_THROW(vector1.push_back(2));
   ASSERT_NO_THROW(vector1.push_back(3));
   ASSERT_NO_THROW(vector1.push_back(4));
   ASSERT_EQ(4, vector1.size());
   ASSERT_EQ(1, vector1[0]);
   ASSERT_EQ(2, vector1[1]);
   ASSERT_EQ(3, vector1[2]);
   ASSERT_EQ(4, vector1[3]);
   vector1.erase(vector1.begin());
   ASSERT_EQ(3, vector1.size());
   ASSERT_EQ(2, vector1[0]);
   ASSERT_EQ(3, vector1[1]);
   ASSERT_EQ(4, vector1[2]);
   ASSERT_NO_THROW(vector1.erase(vector1.begin() + 100));
}

TEST(FixedSizeVectorTest, Oversize) {
   FixedSizeVector<int, 3> vector1;
   ASSERT_NO_THROW(vector1.push_back(1));
   ASSERT_NO_THROW(vector1.push_back(2));
   ASSERT_NO_THROW(vector1.push_back(3));
   ASSERT_THROW(vector1.push_back(4), std::length_error);
}

TEST(FixedSizeVectorTest, Iterate) {
   FixedSizeVector<int, 3> vector1;
   ASSERT_NO_THROW(vector1.push_back(10));
   ASSERT_NO_THROW(vector1.push_back(10));
   ASSERT_NO_THROW(vector1.push_back(10));
   for(auto& e : vector1)
   {
      ASSERT_EQ(10, e);
   }
}

TEST(FixedSizeVectorTest, Erase) {
   FixedSizeVector<int, 3> vector1;
   ASSERT_NO_THROW(vector1.push_back(10));
   ASSERT_NO_THROW(vector1.push_back(10));
   ASSERT_NO_THROW(vector1.push_back(10));
   ASSERT_EQ(3, vector1.size());
   vector1.erase(vector1.begin());
   ASSERT_EQ(2, vector1.size());
}