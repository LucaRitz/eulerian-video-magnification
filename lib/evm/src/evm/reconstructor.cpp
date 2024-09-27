#include <evm/evm/reconstructor.hpp>

#include <opencv2/opencv.hpp>

evm::Reconstructed evm::Reconstructor::operator()(const cv::Mat& original, const evm::Position& position, const std::shared_ptr<evm::Pyramid>& amplified) const {

    auto amplifiedFrame = amplified->collapse();
    auto resultFrame = original + amplifiedFrame;
    return evm::Reconstructed{original, amplifiedFrame, resultFrame};
}