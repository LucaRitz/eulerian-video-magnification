#pragma once

#include "../macro_definition.hpp"
#include "pyramid.hpp"

namespace evm {

    class PipelineBuilder;

    struct EXPORT_EVM SpatialFiltered {
        std::shared_ptr<evm::Pyramid> _spatialFiltered;
        uint8_t _levels;

        SpatialFiltered clone() const {

            return SpatialFiltered {
                _spatialFiltered->clone(),
                _levels
            };
        }
    };

    class EXPORT_EVM SpatialFilter {

        friend class PipelineBuilder;

    public:
        virtual evm::SpatialFiltered operator()(const cv::Mat&) = 0;

    protected:
        std::shared_ptr<ColorConverter> _colorConverter;
    };
}