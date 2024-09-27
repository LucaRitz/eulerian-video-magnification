
#include <evm/capture/source.hpp>

evm::SourceFrame::SourceFrame(cv::Mat frame, uint16_t fps) :
    _frame(frame),
    _fps(fps) {

}

evm::VideoSource::VideoSource(const std::string& file) :
    _videoCapture(file),
    _totalFrames(static_cast<uint64_t>(_videoCapture.get(cv::CAP_PROP_FRAME_COUNT))) {
}

evm::VideoSource::~VideoSource() {

    _videoCapture.release();
}

evm::SourceFrame evm::VideoSource::operator()() {

    auto frame = grab();
    increaseFramesRead(frame);
    auto fps = readFps();

    return evm::SourceFrame{
        frame,
        fps
    };
}

bool evm::VideoSource::isDone() {

    return _totalFrames == _framesRead;
}

void evm::VideoSource::increaseFramesRead(const cv::Mat& frame) {

    if (!frame.empty()) {
        _framesRead++;
    }
}

cv::Mat evm::VideoSource::grab() {

    cv::Mat frame;
    _videoCapture >> frame;
    return frame;
}

uint16_t evm::VideoSource::readFps() const {

    return static_cast<uint16_t>(_videoCapture.get(cv::CAP_PROP_FPS));
}