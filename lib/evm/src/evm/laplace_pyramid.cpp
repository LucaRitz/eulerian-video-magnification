#include <evm/evm/laplace_pyramid.hpp>

#include <opencv2/opencv.hpp>
#include <evm/color_converter.hpp>

evm::LaplacePyramid::LaplacePyramid(const std::shared_ptr<ColorConverter> colorConverter,
                                    const cv::Mat& image, uint8_t levels) :
        Pyramid(colorConverter) {
    std::vector<cv::Mat> gaussian_pyramid;
    gaussian_pyramid.push_back(image);

    cv::Mat clone = image.clone();
    cv::Mat& previousWorkingImage = clone;
    for (int i = 1; i < levels; i++) {
        cv::Mat currentImage;
        cv::pyrDown(previousWorkingImage, currentImage,
                    cv::Size(previousWorkingImage.cols / 2, previousWorkingImage.rows / 2));
        gaussian_pyramid.push_back(currentImage);
        previousWorkingImage = currentImage;
    }

    _levels.push_back(gaussian_pyramid.at(gaussian_pyramid.size() - 1));

    previousWorkingImage = gaussian_pyramid.at(gaussian_pyramid.size() - 1);
    for (int i = gaussian_pyramid.size() - 2; i >= 0; i--) {
        cv::Mat currentImage;
        cv::pyrUp(previousWorkingImage, currentImage, gaussian_pyramid.at(i).size());

        currentImage = gaussian_pyramid.at(i) - currentImage;
        colorConverter->resultingToGray(currentImage);

        _levels.push_back(currentImage);
        previousWorkingImage = gaussian_pyramid.at(i);
    }
}

evm::LaplacePyramid::LaplacePyramid(const LaplacePyramid& other) :
        Pyramid(other._colorConverter) {

    for (auto& mat : other._levels) {
        _levels.push_back(mat.clone());
    }
}

cv::Mat evm::LaplacePyramid::collapse() const {

    if (_levels.empty()) {
        return cv::Mat{};
    }

    auto collapsed = _levels.at(0).clone();

    for (int i = 1; i < _levels.size(); i++) {
        auto current = _levels.at(i).clone();
        cv::pyrUp(collapsed, collapsed, current.size());

        _colorConverter->grayToComputing(current);

        collapsed += current;
    }

    return collapsed;
}

std::shared_ptr<evm::Pyramid> evm::LaplacePyramid::clone() const {

    return std::make_shared<evm::LaplacePyramid>(*this); // TODO: Make deep copy
}