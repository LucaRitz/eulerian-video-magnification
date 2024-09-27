#pragma once

#include "../macro_definition.hpp"

#include <string>
#include <opencv2/opencv.hpp>

namespace evm {

    struct EXPORT_EVM SourceFrame {

        explicit SourceFrame(cv::Mat, uint16_t);

        cv::Mat _frame;
        uint16_t _fps;
    };

    class EXPORT_EVM Source {

    public:
        virtual evm::SourceFrame operator()() = 0;
        virtual bool isDone() = 0;
    };

    class EXPORT_EVM VideoSource : public Source {

    public:
        explicit VideoSource(const std::string& file);
        ~VideoSource();

        evm::SourceFrame operator()() override;
        bool isDone() override;

    private:
        cv::VideoCapture _videoCapture;
        uint64_t _totalFrames;
        uint64_t _framesRead;

        void increaseFramesRead(const cv::Mat&);
        [[nodiscard]] cv::Mat grab();
        [[nodiscard]] uint16_t readFps() const;
    };
}