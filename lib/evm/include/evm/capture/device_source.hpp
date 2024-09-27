#pragma once

#include "../macro_definition.hpp"
#include "source.hpp"

namespace evm {

    class EXPORT_EVM DeviceSource : public Source {

    public:
        explicit DeviceSource(int device);
        ~DeviceSource();

        evm::SourceFrame operator()() override;
        bool isDone() override;

    private:
        cv::VideoCapture _videoCapture;

        [[nodiscard]] cv::Mat grab();
        [[nodiscard]] uint16_t readFps() const;
    };
}