#include <gtest/gtest.h>
#include "ThreadedLoop.h"

using namespace util;

TEST(ThreadedLoopTest, Construct) {
    ThreadedLoop threadedLoop(std::chrono::milliseconds(1), [](){
        
    });
}