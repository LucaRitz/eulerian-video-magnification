#pragma once

#include "../macro_definition.hpp"

#include "data.hpp"

namespace evm {

    class EXPORT_EVM Subscriber {

    public:
        virtual void operator()(const evm::Reconstructed& reconstructed) = 0;
    };
}