#include <evm/evm/video_subscriber.hpp>

evm::VideoSubscriber::VideoSubscriber(std::string file, uint16_t fps) :
    _file(std::move(file)),
    _fps(fps),
    _writer() {
}

evm::VideoSubscriber::~VideoSubscriber() {

    _writer.release();
}

void evm::VideoSubscriber::operator()(const evm::Reconstructed& reconstructed) {

    if (!_writer.isOpened()) {
        auto codec = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
        _writer.open(_file, codec, _fps, reconstructed._resulting.size(), true);
    }

    cv::Mat disp = reconstructed._resulting;
    double min,max;
    minMaxLoc(disp, &min, &max);
    disp.convertTo(disp, CV_8UC3, 255.0/(max-min), -min * 255.0/(max-min));
    _writer.write(disp);
}