#include <gtest/gtest.h>
#include <waffle/waffle.h>

TEST(WaffleTest, dummy) { ASSERT_EQ("waffle", waffle::waffle()); }

struct Objective {
  float operator()(const int length, const float width,
                   const std::string algo) {
    if (algo == "add") {
      return length + width;
    } else if (algo == "multiply") {
      return length * width;
    }

    return 0.0;
  }
};

TEST(WaffleTest, basic_usage) {
  std::vector<int> length_vals{1, 2, 3, 4, 5};
  waffle::ParameterRange p_length{"length", length_vals.begin(),
                                  length_vals.end()};

  std::vector<float> width_vals{1.0, 2.0, 3.0, 4.0, 5.0};
  waffle::ParameterRange p_width{"width", width_vals.begin(), width_vals.end()};

  std::vector<std::string> algo_vals{"add", "multiply"};
  waffle::ParameterRange p_algo{"algorithm", algo_vals.begin(),
                                algo_vals.end()};

  int num_combinations = 1;
  while (waffle::detail::nextP(p_length, p_width, p_algo)) {
    num_combinations += 1;
  }

  EXPECT_EQ(num_combinations,
            p_length.steps() * p_width.steps() * p_algo.steps());

  p_length.reset();
  p_width.reset();
  p_algo.reset();

  Objective o;
  waffle::GridSearch gs{o, p_length, p_width, p_algo};
  auto params = gs.run();

  EXPECT_EQ(std::get<0>(params), 5);
  EXPECT_EQ(std::get<1>(params), 5.0);
  EXPECT_EQ(std::get<2>(params), "multiply");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
