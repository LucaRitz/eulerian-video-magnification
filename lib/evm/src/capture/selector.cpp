#include <evm/capture/selector.hpp>
#include <evm/data.hpp>

#include <opencv2/opencv.hpp>

evm::Roi evm::DefaultSelector::operator()(const cv::Mat& frame) const {

    return {frame, evm::Position{0, 0}};
}