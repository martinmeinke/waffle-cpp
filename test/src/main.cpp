#include <gtest/gtest.h>
#include <waffle/waffle.h>

TEST(WaffleTest, dummy)
{
    ASSERT_TRUE(true);
}

TEST(WaffleTest, waffle)
{
    ASSERT_EQ("waffle", waffle::waffle());
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
