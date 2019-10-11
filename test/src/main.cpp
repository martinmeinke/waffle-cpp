#include <variant>

#include <gtest/gtest.h>
#include <waffle/waffle.h>

TEST(WaffleTest, dummy) { ASSERT_EQ("waffle", waffle::waffle()); }

struct Objective {
  float operator()(int steps, std::string algo, float decay) {
    return static_cast<float>(steps) / decay;
  }
};

TEST(WaffleTest, basic_usage) {
  std::vector<int> p1_vals{1, 2, 3};
  waffle::ParameterRange p1{"steps", p1_vals.begin(), p1_vals.end()};

  std::vector<std::string> p2_vals{"algo1", "balgo"};
  waffle::ParameterRange p2{"algorithm", p2_vals.begin(), p2_vals.end()};

  std::vector<float> p3_vals{3.3, 4.4};
  waffle::ParameterRange p3{"decay", p3_vals.begin(), p3_vals.end()};

  Objective o;

  int num_combinations = 1;
  while (nextP(p1, p2, p3)) {
    num_combinations += 1;
  }

  EXPECT_EQ(num_combinations, p1.steps() * p2.steps() * p3.steps());

  p1.reset();
  p2.reset();
  p3.reset();
  waffle::GridSearch gs{o, p1, p2, p3};
  gs.run();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
