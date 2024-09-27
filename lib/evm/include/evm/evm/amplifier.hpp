#pragma once

#include "../macro_definition.hpp"
#include "pyramid.hpp"

#include <vector>
#include <memory>

namespace evm {

    class EXPORT_EVM Amplifier {

    public:
        explicit Amplifier(std::vector<float> factors);

        void operator*(const std::vector<std::shared_ptr<evm::Pyramid>>& pyramids);

    private:
        std::vector<float> _factors;
    };
}