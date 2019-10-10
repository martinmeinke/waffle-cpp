#pragma once

#include <string>
#include <iostream>
#include <tuple>

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

        // operator ++?

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

    template <typename ObjectiveFctr, typename... ParameterTs>
    struct GridSearchVariadic {
        using ParameterTuple = std::tuple<ParameterTs...>;
        public:
            GridSearchVariadic(ObjectiveFctr of, ParameterTs... params) : 
                objective_(of), 
                parameters_(params...) {
                
            }
            void run(){
                std::cout << objective_(1, "hallo", 2.0) << std::endl;
                std::cout << *std::get<0>(parameters_).get() << std::endl;
                std::cout << std::tuple_size<ParameterTuple>::value << std::endl;
            }

        private:
            ObjectiveFctr objective_;
            ParameterTuple parameters_;
    };
}

