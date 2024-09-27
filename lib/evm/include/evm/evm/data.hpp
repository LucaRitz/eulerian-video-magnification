#pragma once

#include "../macro_definition.hpp"

#include <opencv2/opencv.hpp>

namespace evm {

    struct EXPORT_EVM Reconstructed {

        cv::Mat _original;
        cv::Mat _amplified;
        cv::Mat _resulting;
    };
}