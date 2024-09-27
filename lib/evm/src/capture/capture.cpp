#include <evm/capture/capture.hpp>
#include <evm/capture/source.hpp>
#include <evm/capture/selector.hpp>
#include <evm/capture/stabilizer.hpp>
#include <evm/color_converter.hpp>

evm::Capture::Capture(std::shared_ptr<WorkingSet<Captured>> workerData, std::shared_ptr<Source> source,
                      std::shared_ptr<Selector> selector, std::shared_ptr<Stabilizer> stabilizer,
                      std::shared_ptr<ColorConverter> colorConverter) :
    Worker(std::move(workerData)),
    _source(std::move(source)),
    _selector(std::move(selector)),
    _stabilizer(std::move(stabilizer)),
    _colorConverter(std::move(colorConverter)) {
}

void evm::Capture::execute(std::shared_ptr<WorkingSet<Captured>> data, std::atomic<bool>& running) {

    while (running) {
        captureFrame(data);
        signalAndStopIfDone(data);
    }
}

void evm::Capture::captureFrame(std::shared_ptr<WorkingSet<Captured>> data) {

    auto frame = (*_source)();
    if (isValidFrame(data, frame)) {
        processFrame(data, frame);
    }
}

bool evm::Capture::isValidFrame(std::shared_ptr<WorkingSet<Captured>> data, const evm::SourceFrame& frame) {

    if (frame._frame.empty()) {
        return false;
    }
    return true;
}

void evm::Capture::processFrame(std::shared_ptr<WorkingSet<Captured>> data, evm::SourceFrame& frame) {

    frame._frame.convertTo(frame._frame, CV_32FC3);
    _colorConverter->toComputing(frame._frame);

    auto selected = (*_selector)(frame._frame);
    auto stabilized = (*_stabilizer)(selected._frame);

    evm::Captured captured {
            frame._frame,
            selected,
            stabilized,
            frame._fps
    };
    data->insert(captured);
}

void evm::Capture::signalAndStopIfDone(std::shared_ptr<WorkingSet<Captured>> data) {

    if (_source->isDone()) {
        data->setDone();
        stop();
    }
}