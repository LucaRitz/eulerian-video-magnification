#pragma once

#include "../macro_definition.hpp"
#include "../worker.hpp"
#include "../data.hpp"
#include "spatial_filter.hpp"
#include "temporal_filter.hpp"
#include "amplifier.hpp"
#include "reconstructor.hpp"
#include "data.hpp"
#include "../color_converter.hpp"

#include <memory>
#include <vector>
#include <opencv2/opencv.hpp>

namespace evm {

    class EXPORT_EVM EVMPipeline : public Worker<Reconstructed> {

    public:
        EVMPipeline(std::shared_ptr<WorkingSet<evm::Captured>>, std::shared_ptr<WorkingSet<evm::Reconstructed>>, std::shared_ptr<SpatialFilter>, std::shared_ptr<TemporalFilter>, std::shared_ptr<Amplifier>, std::shared_ptr<Reconstructor>,
                std::shared_ptr<ColorConverter>);

    protected:
        virtual void execute(std::shared_ptr<WorkingSet<evm::Reconstructed>> data, std::atomic<bool>& running) override;

    private:
        std::shared_ptr<evm::WorkingSet<evm::Captured>> _captureWorkingSet;
        uint16_t _lastFps;

        std::shared_ptr<SpatialFilter> _spatialFilter;
        std::shared_ptr<TemporalFilter> _temporalFilter;
        std::shared_ptr<Amplifier> _amplifier;
        std::shared_ptr<Reconstructor> _reconstructor;
        std::shared_ptr<ColorConverter> _colorConverter;

        std::vector<evm::Captured> _originals;

        void amplifyAndResolveFrames(std::shared_ptr<WorkingSet<evm::Reconstructed>>, const evm::TemporalFiltered&);
    };
}