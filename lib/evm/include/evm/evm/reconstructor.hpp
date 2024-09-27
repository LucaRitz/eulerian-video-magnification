#pragma once

#include "../macro_definition.hpp"
#include "pyramid.hpp"
#include "data.hpp"
#include "../data.hpp"

#include <vector>

namespace cv {
    class Mat;
}

namespace evm {

    class EXPORT_EVM Reconstructor {
    public:
        virtual evm::Reconstructed operator()(const cv::Mat& original, const evm::Position& position, const std::shared_ptr<evm::Pyramid>& amplified) const;
    };
}