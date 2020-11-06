//
// Created by ch on 2020/11/4.
//

#include "construct.h"
#include <gtest/gtest.h>

//test for Built-in type
TEST(TestForBuiltInType, Test_ConstructIntFromInt) {
    int *p = new int (10);
    simple_stl::construct<int, int>(p,8);
    ASSERT_EQ(8, *p) << "construct function doesn't work correctly : construct int from int\n";
}

//test for class
class ClassForTest{
public:
    ClassForTest() {}
    ClassForTest(int n): data(n) {}
    int get_data () {return data;}
private:
    int data;
};

TEST(TestForClass, Test_) {
    ClassForTest c;
    simple_stl::construct<ClassForTest, int>(&c,8);
    ASSERT_EQ(8, c.get_data()) << "construct function doesn't work correctly : class constructor \n";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}