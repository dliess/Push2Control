#include <gtest/gtest.h>
#include "2DMatrixCheck.h"

using Matrix = std::vector<std::vector<int>>;

TEST(TwoDMatrixCheckTest, DimOfTest) {
   ASSERT_EQ( std::make_pair(2,3), util::dimOf(Matrix{ {1,2,3}, {1,2,3} }) );
   ASSERT_EQ( std::make_pair(0,0), util::dimOf(Matrix{}) );
   ASSERT_EQ( std::make_pair(1,1), util::dimOf(Matrix{{1}}) );
   ASSERT_EQ( std::make_pair(2,3), util::dimOf(Matrix{ {1,2,3}, {1,3} }) );
}

TEST(TwoDMatrixCheckTest, IsConsistentTest) {
   ASSERT_EQ( true, util::isConsistent(Matrix{ {1,2,3}, {1,2,3} }) );
   ASSERT_EQ( false, util::isConsistent(Matrix{ {1,2,3}, {1,3} }) );
   ASSERT_EQ( true, util::isConsistent(Matrix{}) );
}