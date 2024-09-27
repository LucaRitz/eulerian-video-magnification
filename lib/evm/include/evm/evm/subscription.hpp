#pragma once

#include "../macro_definition.hpp"

#include "subscriber.hpp"
#include "../worker.hpp"
#include "data.hpp"

namespace evm {

    class EXPORT_EVM Subscription : public evm::Worker<Void> {

    public:
        explicit Subscription(std::shared_ptr<evm::WorkingSet<evm::Reconstructed>> evmWorkingSet,
                              std::shared_ptr<evm::WorkingSet<Void>> subscriptionWorkingSet,
                              std::shared_ptr<evm::Subscriber> subscriber);

    protected:
        void execute(std::shared_ptr<WorkingSet<Void>> data, std::atomic<bool>& running) override;

    private:
        std::shared_ptr<evm::WorkingSet<evm::Reconstructed>> _evmWorkingSet;
        std::shared_ptr<evm::Subscriber> _subscriber;
    };
}