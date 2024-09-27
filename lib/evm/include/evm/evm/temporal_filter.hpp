#pragma once

#include "../macro_definition.hpp"
#include "spatial_filter.hpp" // # TODO: Move TemporalFiltered to data?
#include "pyramid.hpp"

#include <vector>

namespace evm {

    class PipelineBuilder;

    struct EXPORT_EVM TemporalFiltered {
        std::vector<std::shared_ptr<evm::Pyramid>> _temporalFiltered;
    };

    class EXPORT_EVM TemporalFilter {

        friend class PipelineBuilder;

    public:
        virtual TemporalFiltered operator()(const SpatialFiltered&, uint16_t) = 0;
        virtual TemporalFiltered remaining(uint16_t) = 0;

    protected:
        std::shared_ptr<ColorConverter> _colorConverter;
    };
}