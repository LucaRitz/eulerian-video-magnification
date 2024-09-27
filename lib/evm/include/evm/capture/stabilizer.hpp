#pragma once

#include "../macro_definition.hpp"

namespace cv {
    class Mat;
}

namespace evm {

    class EXPORT_EVM Stabilizer {

    public:
        virtual cv::Mat operator()(const cv::Mat& frame) const = 0;
    };

    class EXPORT_EVM DefaultStabilizer : public Stabilizer {

    public:
        virtual cv::Mat operator()(const cv::Mat& frame) const override;
    };
}