#include <evm/evm/gaussian_pyramid.hpp>

#include <evm/color_converter.hpp>

#include <algorithm>

evm::GaussianPyramid::GaussianPyramid(const std::shared_ptr<ColorConverter> colorConverter, const cv::Mat& frame, uint8_t levels) :
    Pyramid(std::move(colorConverter)) {

    cv::Mat previousImage = frame.clone();
    _levels.push_back(previousImage);
    for (int i = 1; i < levels; i++) {

        cv::Mat currentImage;
        cv::pyrDown(previousImage, currentImage,
                    cv::Size(previousImage.cols / 2, previousImage.rows / 2));
        _levels.push_back(currentImage);
        previousImage = currentImage;
    }
    std::reverse(_levels.begin(), _levels.end());
}

evm::GaussianPyramid::GaussianPyramid(const GaussianPyramid& other) :
    Pyramid(other._colorConverter) {

    for (auto& mat : other._levels) {
        _levels.push_back(mat.clone());
    }
}

cv::Mat evm::GaussianPyramid::collapse() const {

    if (_levels.empty()) {
        return cv::Mat{};
    }

    auto collapsed = _levels.at(0).clone();

    for (int i = 1; i < _levels.size(); i++) {
        auto current = _levels.at(i).clone();
        cv::pyrUp(collapsed, collapsed, current.size());
        collapsed += current;
    }

    return collapsed;
}

std::shared_ptr<evm::Pyramid> evm::GaussianPyramid::clone() const {

    return std::make_shared<evm::GaussianPyramid>(*this); // TODO: Make deep copy
}