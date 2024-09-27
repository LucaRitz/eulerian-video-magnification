#pragma once

#include "macro_definition.hpp"

#include <opencv2/opencv.hpp>

namespace evm {

    class EXPORT_EVM ColorConverter {

    public:
        virtual void toComputing(cv::Mat& frame) const = 0;
        virtual void grayToComputing(cv::Mat& frame) const = 0;
        virtual void toResulting(cv::Mat& frame) const = 0;
        virtual void resultingToGray(cv::Mat& frame) const = 0;
    };

    class EXPORT_EVM RGBColorConverter : public ColorConverter {

    public:
        RGBColorConverter();

        void toComputing(cv::Mat& frame) const override;
        void grayToComputing(cv::Mat& frame) const override;
        void toResulting(cv::Mat& frame) const override;
        void resultingToGray(cv::Mat& frame) const override;

    private:
        cv::ColorConversionCodes _computingGray;
        cv::ColorConversionCodes _resultingGray;
    };

    class EXPORT_EVM ColorSpaceConverter : public ColorConverter {

    public:
        explicit ColorSpaceConverter(cv::ColorConversionCodes, cv::ColorConversionCodes, cv::ColorConversionCodes, cv::ColorConversionCodes);

        void toComputing(cv::Mat& frame) const override;
        void grayToComputing(cv::Mat& frame) const override;
        void toResulting(cv::Mat& frame) const override;
        void resultingToGray(cv::Mat& frame) const override;

    private:
        cv::ColorConversionCodes _computing;
        cv::ColorConversionCodes _computingGray;
        cv::ColorConversionCodes _resulting;
        cv::ColorConversionCodes _resultingGray;
    };
}