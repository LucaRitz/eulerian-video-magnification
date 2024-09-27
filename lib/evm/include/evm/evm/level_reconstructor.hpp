#pragma once

#include "../macro_definition.hpp"
#include "reconstructor.hpp"

namespace evm {

    class Reconstructed;
    class Position;

    class EXPORT_EVM LevelReconstructor : public Reconstructor {
    public:
        explicit LevelReconstructor(uint8_t level);

        virtual evm::Reconstructed operator()(const cv::Mat& original, const evm::Position& position, const std::shared_ptr<evm::Pyramid>& amplified) const override;

    private:
        uint8_t _level;
    };
}