#include <evm/capture/device_source.hpp>

evm::DeviceSource::DeviceSource(int device) :
    _videoCapture(device) {
}

evm::DeviceSource::~DeviceSource() {

    _videoCapture.release();
}

evm::SourceFrame evm::DeviceSource::operator()() {

    auto frame = grab();
    auto fps = readFps();

    return evm::SourceFrame{
            frame,
            fps
    };
}

bool evm::DeviceSource::isDone() {

    return false;
}

cv::Mat evm::DeviceSource::grab() {

    cv::Mat frame;
    _videoCapture >> frame;
    return frame;
}

uint16_t evm::DeviceSource::readFps() const {

    return static_cast<uint16_t>(_videoCapture.get(cv::CAP_PROP_FPS));
}