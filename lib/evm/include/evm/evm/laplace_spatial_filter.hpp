#pragma once

#include "../macro_definition.hpp"
#include "spatial_filter.hpp"

namespace evm {

    class EXPORT_EVM LaplaceSpatialFilter : public SpatialFilter {

    public:
        explicit LaplaceSpatialFilter(uint8_t);

        virtual evm::SpatialFiltered operator()(const cv::Mat&) override;

    private:
        uint8_t _levels;
    };
}