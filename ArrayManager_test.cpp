#include <gtest/gtest.h>
#include "ArrayManager.h"

class ArrayManagerTest : public ::testing::Test {
protected:
    ArrayManager* arr;
    void SetUp() override {
        arr = new ArrayManager(10);
    }
    void TearDown() override {
        delete arr;
    }
};

TEST_F(ArrayManagerTest, InitialAllZeros) {
    for (int i = 0; i < arr->size(); ++i) {
        EXPECT_EQ((*arr)[i], 0);
    }
    EXPECT_EQ(arr->count_marked(), 0);
}

TEST_F(ArrayManagerTest, MarkAndCount) {
    (*arr)[2] = 1;
    (*arr)[5] = 3;
    (*arr)[7] = 1;
    EXPECT_EQ(arr->count_marked(), 3);
}

TEST_F(ArrayManagerTest, ClearMarker) {
    (*arr)[1] = 2;
    (*arr)[3] = 2;
    (*arr)[8] = 2;
    arr->clear_marker(2);
    EXPECT_EQ(arr->count_marked(), 0);
    for (int i = 0; i < arr->size(); ++i) {
        EXPECT_EQ((*arr)[i], 0);
    }
}

TEST_F(ArrayManagerTest, ToStringFormat) {
    (*arr)[0] = 1;
    (*arr)[1] = 2;
    std::string expected =
        "\n=== Массив ===\n"
        "   1    2    0    0    0    0    0    0    0    0 \n";
    EXPECT_EQ(arr->to_string(), expected);
}