#pragma once

#include "../macro_definition.hpp"
#include "pyramid.hpp"

namespace cv {
    class Mat;
}

namespace evm {

    class ColorConverter;

    class EXPORT_EVM LaplacePyramid : public Pyramid {

    public:
        LaplacePyramid(const std::shared_ptr<ColorConverter>, const cv::Mat&, uint8_t);
        LaplacePyramid(const LaplacePyramid&);

        [[nodiscard]] cv::Mat collapse() const override;
        [[nodiscard]] std::shared_ptr<Pyramid> clone() const override;
    };
}