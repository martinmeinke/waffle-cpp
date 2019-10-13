#pragma once

#include <iostream>
#include <string>
#include <tuple>

namespace waffle {
std::string waffle();

template <typename IteratorT> struct ParameterRange {
  std::string name_;
  IteratorT start_;
  IteratorT end_;
  IteratorT curr_;

  ParameterRange(std::string name, IteratorT start, IteratorT end)
      : name_(name), start_(start), end_(end), curr_(start) {}

  int steps() { return std::distance(start_, end_); }

  void inc() { curr_++; }

  bool at_end() {
    // this is the last step
    return (curr_ + 1) == end_;
  }

  void reset() { curr_ = start_; }

  IteratorT get() { return curr_; }
};

namespace detail {

template <typename First> int numIterations(First &f) { return f.steps(); }

template <typename First, typename... Rest>
int numIterations(First &f, Rest &... rest) {
  return numIterations(f) * numIterations(rest...);
}

template <typename First> void printParameter(First &f) {
  std::cout << "p: " << f.name_ << ":=" << *f.get() << std::endl;
}

template <typename First, typename... Rest>
void printParameter(First &f, Rest &... rest) {
  printParameter(f);
  printParameter(rest...);
}

template <typename First> void resetP(First &f) { f.reset(); }

template <typename First, typename... Rest>
void resetP(First &f, Rest &... rest) {
  resetP(f);
  resetP(rest...);
}

template <typename First> bool nextP(First &f) {
  if (!f.at_end()) {
    f.inc();
    return true;
  }

  return false;
}

template <typename First, typename... Rest>
bool nextP(First &f, Rest &... rest) {
  bool incremented_in_rest = nextP(rest...);
  if (!incremented_in_rest) {
    resetP(rest...);
    return nextP(f);
  }

  return incremented_in_rest;
}

template <typename... Args> auto param_values(std::tuple<Args...> &tup) {
  // order might be undefined!
  // https://stackoverflow.com/questions/16387354/template-tuple-calling-a-function-on-each-element
  // auto tt = std::apply([](auto& ...x){return (..., x.get());}, parameters_);
  return std::apply([](auto &... x) { return std::make_tuple(*x.get()...); },
                    tup);
}

} // namespace detail

template <typename... ParameterTs> struct GridSearch {
  using ParameterTuple = std::tuple<ParameterTs...>;

public:
  GridSearch(ParameterTs... params) : parameters_(params...) {
    std::cout << "GridSearch constructed using "
              << std::tuple_size<ParameterTuple>::value << " parameters"
              << std::endl;
  }

  void print_parameter_values() {
    std::apply([](auto &... x) { detail::printParameter(x...); }, parameters_);
  }

  void reset_parameters() {
    std::apply([](auto &... x) { detail::resetP(x...); }, parameters_);
  }

  int num_total_iterations() {
    return std::apply([](auto &... x) { return detail::numIterations(x...); },
                      parameters_);
  }

  template <typename ObjectiveFctr> auto argmax(ObjectiveFctr &of) {
    reset_parameters();
    int iteration = 1;
    // first score is initially the best one
    auto best_score = std::apply(of, detail::param_values(parameters_));
    auto best_param_values = detail::param_values(parameters_);

    for (int i = 1; i < num_total_iterations(); i++) {
      std::apply([](auto &... x) { return detail::nextP(x...); }, parameters_);
      iteration = i + 1;
      auto iteration_score = std::apply(of, detail::param_values(parameters_));
      if (iteration_score > best_score) {
        best_score = iteration_score;
        best_param_values = detail::param_values(parameters_);
      }
    }

    std::cout << "Completed total iterations: " << iteration << std::endl;
    std::cout << "best score: " << best_score << std::endl;

    return best_param_values;
  }

private:
  ParameterTuple parameters_;
};
} // namespace waffle
