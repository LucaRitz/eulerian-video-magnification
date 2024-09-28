#pragma once

#include "../macro_definition.hpp"

#include <opencv2/opencv.hpp>
#include <vector>

namespace evm {

    struct EXPORT_EVM Reconstructed {

        cv::Mat _original;
        cv::Mat _amplified;
        cv::Mat _resulting;
        std::vector<cv::Mat> _fft;
        std::vector<std::vector<float>> _frequencies;
    };
}