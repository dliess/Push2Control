#include <gtest/gtest.h>
#include "Ringbuffer.hpp"

TEST(RingbufferTest, SizeTest) {
    util::Ringbuffer<int, 4> ringBuffer;
    ASSERT_EQ(ringBuffer.Size(), 0);
    ringBuffer.push(1);
    ASSERT_EQ(ringBuffer.Size(), 1);
    ringBuffer.push(2);
    ASSERT_EQ(ringBuffer.Size(), 2);
    ringBuffer.push(3);
    ASSERT_EQ(ringBuffer.Size(), 3);
    ringBuffer.push(4);
    ASSERT_EQ(ringBuffer.Size(), 4);
    ringBuffer.push(5);
    ASSERT_EQ(ringBuffer.Size(), 4);
}

TEST(RingbufferTest, CapacityTest) {
    util::Ringbuffer<int, 4> ringBuffer;
    ASSERT_EQ(ringBuffer.Capacity, 4);
}

TEST(RingbufferTest, IterationTest) {
    util::Ringbuffer<int, 4> ringBuffer;
    for(int i = 0; i < 10; ++i)
    {
        ringBuffer.push(i);
    }
    for(int i = 0; i < ringBuffer.Size(); ++i)
    {
        ASSERT_EQ(ringBuffer[i], i + 6);
    }
}
