#include <evm/evm/subscription.hpp>

evm::Subscription::Subscription(std::shared_ptr<evm::WorkingSet<evm::Reconstructed>> evmWorkingSet,
                                std::shared_ptr<evm::WorkingSet<Void>> subscriptionWorkingSet,
                                std::shared_ptr<evm::Subscriber> subscriber) :
                                Worker(subscriptionWorkingSet),
                                _evmWorkingSet(evmWorkingSet),
                                _subscriber(subscriber) {

}

void evm::Subscription::execute(std::shared_ptr<WorkingSet<Void>> data, std::atomic<bool>& running) {

    while (running) {

        std::unique_lock lk(_evmWorkingSet->_mutex);
        _evmWorkingSet->_conditionVariable.wait(lk, [this, &running]{ return this->_evmWorkingSet->hasData() || !running || this->_evmWorkingSet->isDone(); });
        auto frame = _evmWorkingSet->front();
        lk.unlock();

        if (frame._resulting.empty()) {
            if (_evmWorkingSet->isDone()) {
                stop();
            }
        } else {
            (*_subscriber)(frame);
        }
    }
}