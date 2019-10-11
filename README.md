# Waffle-cpp

[![Build Status](https://travis-ci.com/martinmeinke/waffle-cpp.svg?token=yJWCsZhJ491sqJawCaHS&branch=master)](https://travis-ci.com/martinmeinke/waffle-cpp)

Waffle is a very simple c++17 library for performing grid search over a set of parameters.

## Example usage
```cpp
// set up some parameters and corresponding value ranges
std::vector<int> length_vals{1, 2, 3, 4, 5};
waffle::ParameterRange p_length{"length", length_vals.begin(),
                                length_vals.end()};

std::vector<float> width_vals{1.0, 2.0, 3.0, 4.0, 5.0};
waffle::ParameterRange p_width{"width", width_vals.begin(), width_vals.end()};

// define scoring function
auto target = [](int a, float b){return a * b;};
waffle::GridSearch gs{p_length, p_width, p_algo;

// run grid search
auto params = gs.argmax(target);
EXPECT_EQ(std::get<0>(params), 5);
EXPECT_EQ(std::get<1>(params), 5.0);
```