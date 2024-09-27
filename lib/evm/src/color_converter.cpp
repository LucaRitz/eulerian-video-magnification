#include <evm/color_converter.hpp>

evm::RGBColorConverter::RGBColorConverter() :
    _computingGray(cv::COLOR_GRAY2RGB),
    _resultingGray(cv::COLOR_RGB2GRAY) {
}

void evm::RGBColorConverter::toComputing(cv::Mat& frame) const {
    // NOOP
}

void evm::RGBColorConverter::grayToComputing(cv::Mat& frame) const {

    cv::cvtColor(frame, frame, _computingGray);
}

void evm::RGBColorConverter::toResulting(cv::Mat& frame) const {
    // NOOP
}

void evm::RGBColorConverter::resultingToGray(cv::Mat& frame) const {

    cv::cvtColor(frame, frame, _resultingGray);
}

evm::ColorSpaceConverter::ColorSpaceConverter(cv::ColorConversionCodes computing, cv::ColorConversionCodes computingGray,
                                              cv::ColorConversionCodes resulting, cv::ColorConversionCodes resultingGray) :
    _computing(computing),
    _computingGray(computingGray),
    _resulting(resulting),
    _resultingGray(resultingGray) {

}

void evm::ColorSpaceConverter::toComputing(cv::Mat& frame) const {

    cv::cvtColor(frame, frame, _computing);
}

void evm::ColorSpaceConverter::grayToComputing(cv::Mat& frame) const {


    cv::cvtColor(frame, frame, _computingGray);
    cv::cvtColor(frame, frame, _computing);
}

void evm::ColorSpaceConverter::toResulting(cv::Mat& frame) const {

    cv::cvtColor(frame, frame, _resulting);
}

void evm::ColorSpaceConverter::resultingToGray(cv::Mat& frame) const {

    cv::cvtColor(frame, frame, _resulting);
    cv::cvtColor(frame, frame, _resultingGray);
}