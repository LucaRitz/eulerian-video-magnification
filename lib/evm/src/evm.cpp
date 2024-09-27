#include <evm/evm.hpp>

#include <evm/capture/capture.hpp>
#include <evm/capture/source.hpp>
#include <evm/capture/selector.hpp>
#include <evm/capture/stabilizer.hpp>
#include <evm/evm/evm.hpp>
#include <evm/evm/subscription.hpp>
#include <evm/color_converter.hpp>

evm::Pipeline::Pipeline(std::shared_ptr<WorkingSet<Captured>> captureWorkingSet,
                        std::shared_ptr<WorkingSet<Reconstructed>> evmWorkingSet,
                        std::shared_ptr<WorkingSet<Void>> subscriptionWorkingSet,
                        std::shared_ptr<Capture> capture, std::shared_ptr<EVMPipeline> evm,
                        std::shared_ptr<Subscription> subscription):
        _captureWorkingSet(std::move(captureWorkingSet)),
        _evmWorkingSet(std::move(evmWorkingSet)),
        _subscriptionWorkingSet(std::move(subscriptionWorkingSet)),
        _capture(std::move(capture)),
        _evm(std::move(evm)),
        _subscription(std::move(subscription)) {

}

void evm::Pipeline::stop() {

    _capture->stop();
    _evm->stop();
    _subscription->stop();

    _captureWorkingSet->notify();
    _evmWorkingSet->notify();
    _subscriptionWorkingSet->notify();

    join();
}

void evm::Pipeline::join() {

    _capture->join();
    _evm->join();
    _subscription->join();
}

evm::PipelineBuilder::PipelineBuilder(const std::shared_ptr<ColorConverter> colorConverter) :
    _colorConverter(colorConverter ? colorConverter : std::make_shared<evm::RGBColorConverter>()) {
}

evm::PipelineBuilder& evm::PipelineBuilder::operator<<(std::shared_ptr<Source> source) {

    _source = std::move(source);
    return *this;
}

evm::PipelineBuilder& evm::PipelineBuilder::operator<<(std::shared_ptr<Selector> selector) {

    _selector = std::move(selector);
    return *this;
}

evm::PipelineBuilder& evm::PipelineBuilder::operator<<(std::shared_ptr<Stabilizer> stabilizer) {

    _stabilizer = std::move(stabilizer);
    return *this;
}

evm::PipelineBuilder& evm::PipelineBuilder::operator<<(std::shared_ptr<SpatialFilter> spatialFilter) {

    _spatialFilter = std::move(spatialFilter);
    _spatialFilter->_colorConverter = _colorConverter;
    return *this;
}

evm::PipelineBuilder& evm::PipelineBuilder::operator<<(std::shared_ptr<TemporalFilter> temporalFilter) {

    _temporalFilter = std::move(temporalFilter);
    _temporalFilter->_colorConverter = _colorConverter;
    return *this;
}

evm::PipelineBuilder& evm::PipelineBuilder::operator<<(std::shared_ptr<Amplifier> amplifier) {

    _amplifier = std::move(amplifier);
    return *this;
}

evm::PipelineBuilder& evm::PipelineBuilder::operator<<(std::shared_ptr<Reconstructor> reconstructor) {

    _reconstructor = std::move(reconstructor);
    return *this;
}

evm::PipelineBuilder& evm::PipelineBuilder::operator<<(std::shared_ptr<Subscriber> subscriber) {

    _subscriber = std::move(subscriber);
    return *this;
}

evm::Pipeline evm::PipelineBuilder::build() {

    auto captureWorkingSet = std::make_shared<WorkingSet<Captured>>();
    auto evmWorkingSet = std::make_shared<WorkingSet<Reconstructed>>();
    auto subscriptionWorkingSet = std::make_shared<WorkingSet<Void>>();

    if (_source == nullptr) {
        throw 1; // TODO: Throw correct exception
    }

    if (_selector == nullptr) {
        _selector = std::make_shared<evm::DefaultSelector>();
    }

    if (_stabilizer == nullptr) {
        _stabilizer = std::make_shared<evm::DefaultStabilizer>();
    }

    return evm::Pipeline {
        captureWorkingSet,
        evmWorkingSet,
        subscriptionWorkingSet,
        std::make_shared<evm::Capture>(captureWorkingSet, _source, _selector, _stabilizer, _colorConverter),
        std::make_shared<evm::EVMPipeline>(captureWorkingSet, evmWorkingSet, _spatialFilter, _temporalFilter, _amplifier, _reconstructor, _colorConverter),
        std::make_shared<evm::Subscription>(evmWorkingSet, subscriptionWorkingSet, _subscriber)
    };
}

int evm::type(int value) {

    return value;
}