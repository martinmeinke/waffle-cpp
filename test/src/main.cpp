#include <variant>

#include <gtest/gtest.h>
#include <waffle/waffle.h>

TEST(WaffleTest, dummy)
{
    ASSERT_EQ("waffle", waffle::waffle());
}

struct Objective{
  float operator()(int steps, std::string algo, float decay)
  {
    return static_cast<float>(steps) / decay;
  }
};

TEST(WaffleTest, waffle)
{
    using namespace waffle;
    std::vector<int> p1_vals{1,2,3};
    Parameter p1{"steps", p1_vals.begin(), p1_vals.end()};

    std::vector<std::string> p2_vals{"algo1", "balgo"};
    Parameter p2{"algorithm", p2_vals.begin(), p2_vals.end()};

    std::vector<float> p3_vals{0.01, 0.001, 0.0001};
    Parameter p3{"decay", p3_vals.begin(), p3_vals.end()};

    // the variant to visit
    /*using ParameterVariant = std::variant<
      Parameter<std::vector<int>>, 
      Parameter<std::vector<std::string>>, 
      Parameter<std::vector<float>>>;
    using ParameterList = std::vector<ParameterVariant>;

    ParameterList parameter_list = {p1, p2, p3};
    Objective o;
    GridSearch<ParameterList, Objective> gs{parameter_list, o};*/
    
    Objective o;
    GridSearchVariadic gs{o, p1, p2, p3};
    gs.run();

}

/*TEST(WaffleTest, waffle_wunschapi)
{
    GridSearch(scoring_functor, {{{"p1", range(1.0,20.0,0.5)}}});
}*/

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
