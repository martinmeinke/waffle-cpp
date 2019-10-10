#pragma once

#include <string>
#include <iostream>
#include <tuple>


template <class T>
constexpr
std::string_view
type_name()
{
    using namespace std;
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
    return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
    string_view p = __FUNCSIG__;
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}

namespace waffle
{
    std::string waffle();

    // random access iterator -> relax to fwd iterator at some point
    template <typename IteratorT>
    struct Parameter{
        std::string name_;
        IteratorT start_; // use lazy range?
        IteratorT end_; // use lazy range?
        int index_ = 0;

        Parameter(std::string name, IteratorT start, IteratorT end) : name_(name), start_(start), end_(end)
        {

        }

        bool at_end(){
            return start_ + index_ == end_;
        }

        void reset(){
            index_ = 0;
        }

        IteratorT get(){
            return start_ + index_;
        }       
    };

    /*template <typename ParameterListT, typename ObjectiveFctr>
    struct GridSearch {
        public:
            GridSearch(ParameterListT plt, ObjectiveFctr of) : parameters_(plt), objective_(of) {}
            void run(){
                std::cout << objective_(1, "hallo", 2.0) << std::endl;
            }

        private:
            ParameterListT parameters_;
            ObjectiveFctr objective_;
    };*/

    template <typename First, typename... Args>
    void func(First& f, std::tuple<Args...>& tup)
    {
        std::cout << "func called" << std::endl;
    }

    template <typename... Args>
    void func(std::tuple<Args...>& tup)
    {
        std::cout << "func called" << std::endl;
    }


    template <typename... Args>
    auto param_values(std::tuple<Args...>& tup)
    { 
        // order might be undefined!
        // https://stackoverflow.com/questions/16387354/template-tuple-calling-a-function-on-each-element
        //auto tt = std::apply([](auto& ...x){return (..., x.get());}, parameters_);
        return std::apply([](auto ...x){return std::make_tuple(*x.get()...);} , tup);
    }

    template <typename ObjectiveFctr, typename... ParameterTs>
    struct GridSearchVariadic {
        using ParameterTuple = std::tuple<ParameterTs...>;
        public:
            GridSearchVariadic(ObjectiveFctr of, ParameterTs... params) : 
                objective_(of), 
                parameters_(params...) {
                
            }

            void run(){
                // first score is initially the best one
                auto best_score = std::apply(objective_, param_values(parameters_));
                std::cout << "best score: " << best_score << std::endl;
                std::cout << std::tuple_size<ParameterTuple>::value << std::endl;
            }

        private:
            ObjectiveFctr objective_;
            ParameterTuple parameters_;
    };
}

