#pragma once

#include "../macro_definition.hpp"
#include "subscriber.hpp"

#include <opencv2/videoio.hpp>
#include <string>

namespace evm {

    class EXPORT_EVM VideoSubscriber : public Subscriber {

    public:
        VideoSubscriber(std::string file, uint16_t fps);
        ~VideoSubscriber();

        void operator()(const evm::Reconstructed& reconstructed) override;

    private:
        std::string _file;
        uint16_t _fps;

        cv::VideoWriter _writer;
    };
}