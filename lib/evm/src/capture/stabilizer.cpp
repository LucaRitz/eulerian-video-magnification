#include <evm/capture/stabilizer.hpp>
#include <opencv2/opencv.hpp>

cv::Mat evm::DefaultStabilizer::operator()(const cv::Mat& frame) const {

    return frame;
}