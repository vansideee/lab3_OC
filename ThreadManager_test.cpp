#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "ArrayManager.h"
#include "ThreadManager.h"

void dummy_marker(int id, ArrayManager& arr) {
    arr[0] = id;
}

TEST(ThreadManagerTest, CreateAndTerminate) {
    ArrayManager arr(5);
    {
        ThreadManager tm(1, dummy_marker, arr);
        tm.start_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        EXPECT_EQ(arr[0], 1);
        tm.terminate(1);
    }
    EXPECT_EQ(arr.count_marked(), 0);
}

TEST(ThreadManagerTest, AllBlockedSignal) {
    ArrayManager arr(5);
    ThreadManager tm(2, [](int, ArrayManager&) {}, arr);
    tm.start_all();
    EXPECT_TRUE(tm.all_blocked());
}