#include <iostream>
#include <functional>
#include <experimental/optional>
#include <tuple>
#include <cassert>
#include <algorithm>
#include <cfloat>
#include <cstdlib>
#include <list>



void prev(double price, const std::function<void(double)>& result, std::pair<bool, double>& state) {

    auto& [ticked, value] = state;
    
    if (ticked)
        result(value);

    ticked = true;
    value = price;
}



void take(double price, const std::function<void(double)>& result, size_t& state, const size_t& size) {
    if (state++ < size) {
        result(price);
    }
}


void mwa(double price, const std::function<void(double)>& result, std::pair<std::list<double>, double> & state, const size_t& size) {

    auto& [queue, current_mwa] = state;
    
    queue.push_back(price);
    current_mwa += price/size;
    
    if (queue.size() >= size)
    {

        if (queue.size() > size)
        {
            current_mwa -= queue.front()/size;
            queue.pop_front();
        }
        
        result(current_mwa);
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// simple test framework for functions

namespace test_framework {
    template<typename T, typename... Args>
    bool run(
        std::string_view name, 
        const std::vector<double>& input, 
        const std::vector<std::optional<double>>& expected,
        void(*function_to_test)(double, const std::function<void(double)>&, T&, const Args&...), Args... args) 
    {
        assert(input.size() == expected.size());

        size_t step {0};
        std::optional<double> expected_output;
        size_t step_checked {std::numeric_limits<size_t>::max()};
        size_t failures_detected {0};

        auto receive_result = [&](double r) {
            
            if (expected_output) {
                if (std::abs(*expected_output - r) > 2 * DBL_EPSILON * std::max(*expected_output, r)) {
                    std::cout << "FAILURE: " << name << ": expected value in step " << step << " was " << *expected_output << " but received " << r << std::endl;
                    ++failures_detected;
                }
            } else {
                std::cout << "FAILURE: " << name << ": no value was expected in step " << step << " but received " << r << std::endl;
                ++failures_detected;
            }
            step_checked = step;
        };

        auto function_to_test_invoker = [=, cb = std::function<void(double)>{receive_result}, state = T{}](double p) mutable {
            function_to_test(p, cb, state, args...);
        };

        for (step = 0; step < input.size(); ++step) {
            expected_output = expected[step];

            function_to_test_invoker(input[step]);

            if (expected_output && step_checked != step) {
                std::cout << "FAILURE: " << name << ": value " << *expected_output << " was expected in step " << step << " but none received" << std::endl;
                ++failures_detected;
            }
        }

        if (failures_detected == 0)
            std::cout << "SUCCESS: " << name << std::endl;

        return failures_detected == 0;
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sample tests

void test1()
{
    test_framework::run("test1", 
        {100., 100.1, 100.2, 100.3}, 
        {std::nullopt, 100., 100.1, 100.2},
        prev);
}

void test2()
{
    test_framework::run("test2", 
        {100., 100.1, 100.2, 100.3, 100.4},
        {100., 100.1, 100.2, std::nullopt, std::nullopt},
        take, size_t{3});
}

void test3() {
    test_framework::run("test3", 
        {100., 100.1, 100.2, 100.3, 100.4, 100.5, 100.4, 100.3, 100.2, 100.1},
        {std::nullopt, std::nullopt, std::nullopt, std::nullopt, 100.2, 100.3, 100.36, 100.38, 100.36, 100.3},
        mwa, size_t{5});
}

int main() {
    test1();
    test2();
    test3();
}
