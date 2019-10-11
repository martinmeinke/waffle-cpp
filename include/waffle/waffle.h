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

template <typename ObjectiveFctr, typename... ParameterTs> struct GridSearch {
  using ParameterTuple = std::tuple<ParameterTs...>;

public:
  GridSearch(ObjectiveFctr of, ParameterTs... params)
      : objective_(of), parameters_(params...) {
    std::cout << "GridSearch constructed using "
              << std::tuple_size<ParameterTuple>::value << " parameters"
              << std::endl;
  }

  void print_parameter_values() {
    std::cout << "p1: " << *std::get<0>(parameters_).get() << std::endl;
    std::cout << "p2: " << *std::get<1>(parameters_).get() << std::endl;
    std::cout << "p3: " << *std::get<2>(parameters_).get() << std::endl;
  }

  void run() {
    int iteration = 1;
    // first score is initially the best one
    auto best_score = std::apply(objective_, param_values(parameters_));
    while (std::apply([](auto &... x) { return nextP(x...); }, parameters_)) {
      iteration += 1;
      auto iteration_score = std::apply(objective_, param_values(parameters_));
      std::cout << "iteration: " << iteration << std::endl;
      std::cout << "score: " << iteration_score << std::endl;
      best_score = std::max(best_score, iteration_score);
    }

    std::cout << "total iterations: " << iteration << std::endl;
    std::cout << "best score: " << best_score << std::endl;
  }

private:
  ObjectiveFctr objective_;
  ParameterTuple parameters_;
};
} // namespace waffle
