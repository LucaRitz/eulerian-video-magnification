#pragma once

#include "macro_definition.hpp"

#include <opencv2/opencv.hpp>
#include <vector>
#include <future>
#include <memory>

namespace evm {

    struct EXPORT_EVM Position {
        uint64_t _x;
        uint64_t _y;
    };

    struct EXPORT_EVM Roi {
        cv::Mat _frame;
        evm::Position _position;
    };

    struct EXPORT_EVM Captured {
        cv::Mat _frame;
        evm::Roi _selected;
        cv::Mat _stabilized;
        uint16_t _fps;
    };
}