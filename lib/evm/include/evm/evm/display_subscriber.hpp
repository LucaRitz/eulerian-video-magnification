#pragma once

#include "subscriber.hpp"

namespace evm {

    class EXPORT_EVM DisplaySubscriber : public Subscriber {

    public:
        void operator()(const evm::Reconstructed& reconstructed) override;
    };
}