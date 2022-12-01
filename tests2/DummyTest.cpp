#include <gtest/gtest.h>
#include <gmock/gmock.h>


using namespace ::testing;


TEST(DummyTest, ThisShouldFail)
{
    ASSERT_TRUE(false);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}

