#pragma once

#include "../macro_definition.hpp"

namespace cv {
    class Mat;
}

namespace evm {

    class Roi;

    class EXPORT_EVM Selector {

    public:
        virtual evm::Roi operator()(const cv::Mat& frame) const = 0;
    };

    class EXPORT_EVM DefaultSelector : public Selector {

    public:
        virtual evm::Roi operator()(const cv::Mat& frame) const override;
    };
}