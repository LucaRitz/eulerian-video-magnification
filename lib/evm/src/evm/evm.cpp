#include <evm/evm/evm.hpp>
#include <evm/evm/spatial_filter.hpp>
#include <evm/evm/temporal_filter.hpp>
#include <evm/evm/amplifier.hpp>
#include <evm/evm/reconstructor.hpp>

evm::EVMPipeline::EVMPipeline(std::shared_ptr<WorkingSet<evm::Captured>> captureWorkingSet, std::shared_ptr<WorkingSet<evm::Reconstructed>> evmWorkingSet, std::shared_ptr<SpatialFilter> spatialFilter, std::shared_ptr<TemporalFilter> temporalFilter, std::shared_ptr<Amplifier> amplifier, std::shared_ptr<Reconstructor> reconstructor,
                              std::shared_ptr<ColorConverter> colorConverter) :
        Worker(std::move(evmWorkingSet)),
        _captureWorkingSet(std::move(captureWorkingSet)),
        _lastFps(0),
        _spatialFilter(std::move(spatialFilter)),
        _temporalFilter(std::move(temporalFilter)),
        _amplifier(std::move(amplifier)),
        _reconstructor(std::move(reconstructor)),
        _colorConverter(std::move(colorConverter)) {

}

void evm::EVMPipeline::execute(std::shared_ptr<WorkingSet<evm::Reconstructed>> data, std::atomic<bool>& running) {

    while(running) {
        std::unique_lock lk(_captureWorkingSet->_mutex);
        _captureWorkingSet->_conditionVariable.wait(lk, [this, &running]{ return this->_captureWorkingSet->hasData() || !running || this->_captureWorkingSet->isDone(); });
        auto frame = _captureWorkingSet->front();
        lk.unlock();

        if (frame._frame.empty()) {

            if (_captureWorkingSet->isDone()) {
                auto temporalFiltered = (*_temporalFilter).remaining(_lastFps);

                amplifyAndResolveFrames(data, temporalFiltered);
                data->setDone();
                stop();
            }
        } else {
            _originals.push_back(frame);
            _lastFps = frame._fps;
            auto spatialFiltered = (*_spatialFilter)(frame._stabilized);
            auto temporalFiltered = (*_temporalFilter)(spatialFiltered, _lastFps);

            amplifyAndResolveFrames(data, temporalFiltered);
        }
    }
}

void evm::EVMPipeline::amplifyAndResolveFrames(std::shared_ptr<WorkingSet<evm::Reconstructed>> data,
                                               const evm::TemporalFiltered& temporalFiltered) {

    auto frames = temporalFiltered._temporalFiltered.size();

    if (frames > 0) {

        (*_amplifier) * temporalFiltered._temporalFiltered;
        auto& amplified = temporalFiltered._temporalFiltered;

        for (int i = 0; i < frames; i++) {
            auto& original = _originals.at(i);
            auto reconstructed = (*_reconstructor)(original._stabilized, original._selected._position, amplified.at(i));

            reconstructed._fft = temporalFiltered._fft;
            reconstructed._frequencies = temporalFiltered._frequencies;

            _colorConverter->toResulting(reconstructed._amplified);
            _colorConverter->toResulting(reconstructed._original);
            _colorConverter->toResulting(reconstructed._resulting);

            data->insert(reconstructed);
        }
        _originals.erase(_originals.begin(), _originals.begin() + frames);
    }
}
