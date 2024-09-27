#include <evm/evm/level_reconstructor.hpp>
#include <evm/evm/evm.hpp>
#include <evm/evm/pyramid.hpp>

#include <algorithm>
#include <opencv2/opencv.hpp>

evm::LevelReconstructor::LevelReconstructor(uint8_t level) : _level(level) {

}

evm::Reconstructed evm::LevelReconstructor::operator()(const cv::Mat& original, const evm::Position& position, const std::shared_ptr<evm::Pyramid>& amplified) const {

    auto levelMat = amplified->at(_level).clone();

    if (!levelMat.empty()) {
        cv::resize(levelMat, levelMat, original.size(), 0, 0, cv::INTER_LINEAR);

        /////////////////////////////////////////////////////////////////////////
        /*cv::Mat levelChannels[3];
        cv::split(levelMat, levelChannels);

        cv::Mat originalChannels[3];
        cv::split(original, originalChannels);

        cv::Mat y = originalChannels[0] + levelChannels[0];
        cv::Mat cb = originalChannels[1] + levelChannels[1];
        cv::Mat cr = originalChannels[2] + levelChannels[2];

        cv::Mat ycbcr[] = {y, cb, cr};

        cv::Mat mixed;
        cv::merge(ycbcr, 3, mixed);*/

        auto mixed = original + levelMat;
        /////////////////////////////////////////////////////////////////////////

        return evm::Reconstructed{original, levelMat, mixed};
    }

    return evm::Reconstructed{};
}