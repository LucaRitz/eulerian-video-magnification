#include <gtest/gtest.h>
#include <evm/evm.hpp>
#include <iostream>
#include <chrono>
#include <functional>
#include <string>

namespace evm::test {

    template<class R>
    R measured(const std::function<R ()>& function, const std::string& step = "");
}

TEST(EVMPipeline, should_magnify_expected_frequency) {

    // Act
    auto result = evm::test::measured<int>([]() -> int {return evm::type(5);}, "Function call");

    // Assert
    ASSERT_EQ(5, result);
}

template<class R>
R evm::test::measured(const std::function<R ()>& function, const std::string& step) {

    auto start = std::chrono::high_resolution_clock::now();
    auto result = function();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << (step.empty() ? "Measured step" : step) << " took " << std::to_string(duration.count()) << " ms" << std::endl;

    return result;
}