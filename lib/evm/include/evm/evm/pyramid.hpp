#pragma once

#include "../macro_definition.hpp"

#include <vector>
#include <opencv2/opencv.hpp>

namespace evm {

    class ColorConverter;

    class EXPORT_EVM Pyramid {

    public:
        explicit Pyramid(std::shared_ptr<ColorConverter> colorConverter);
        virtual ~Pyramid() = default;

        [[nodiscard]] cv::Mat at(int level) const;
        [[nodiscard]] virtual cv::Mat collapse() const = 0;
        [[nodiscard]] virtual std::shared_ptr<Pyramid> clone() const = 0; // TODO: Replace with factory
        void clear();
        void operator*(const std::vector<float>& factors) const;

        void push_back(cv::Mat value);
        [[nodiscard]] int levels() const;

    protected:
        std::vector<cv::Mat> _levels;
        std::shared_ptr<ColorConverter> _colorConverter;
    };
}