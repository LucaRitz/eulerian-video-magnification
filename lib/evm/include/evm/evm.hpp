#pragma once

#include "macro_definition.hpp"
#include "data.hpp"
#include "evm/data.hpp"
#include "worker.hpp"
#include "evm/evm.hpp"

#include <memory>

namespace evm {

    class Capture;
    class Source;
    class Selector;
    class Stabilizer;
    class Captured;

    class EVMPipeline;
    class SpatialFilter;
    class TemporalFilter;
    class Amplifier;
    class Reconstructor;

    class Subscription;
    class Subscriber;

    class ColorConverter;

    class EXPORT_EVM Pipeline {

        friend class PipelineBuilder;

    public:
        void stop();
        void join();

    private:
        explicit Pipeline(std::shared_ptr<WorkingSet<Captured>>, std::shared_ptr<WorkingSet<Reconstructed>>,
                std::shared_ptr<WorkingSet<Void>>, std::shared_ptr<Capture>, std::shared_ptr<EVMPipeline>,
                        std::shared_ptr<Subscription>);

        std::shared_ptr<WorkingSet<Captured>> _captureWorkingSet;
        std::shared_ptr<WorkingSet<Reconstructed>> _evmWorkingSet;
        std::shared_ptr<WorkingSet<Void>> _subscriptionWorkingSet;
        std::shared_ptr<Capture> _capture;
        std::shared_ptr<EVMPipeline> _evm;
        std::shared_ptr<Subscription> _subscription;
    };

    class EXPORT_EVM PipelineBuilder {

    public:

        explicit PipelineBuilder(const std::shared_ptr<ColorConverter> = nullptr);

        PipelineBuilder& operator<<(std::shared_ptr<Source>);
        PipelineBuilder& operator<<(std::shared_ptr<Selector>);
        PipelineBuilder& operator<<(std::shared_ptr<Stabilizer>);
        PipelineBuilder& operator<<(std::shared_ptr<SpatialFilter>);
        PipelineBuilder& operator<<(std::shared_ptr<TemporalFilter>);
        PipelineBuilder& operator<<(std::shared_ptr<Amplifier>);
        PipelineBuilder& operator<<(std::shared_ptr<Reconstructor>);
        PipelineBuilder& operator<<(std::shared_ptr<Subscriber>);

        Pipeline build();

    private:
        std::shared_ptr<Source> _source;
        std::shared_ptr<Selector> _selector;
        std::shared_ptr<Stabilizer> _stabilizer;
        std::shared_ptr<SpatialFilter> _spatialFilter;
        std::shared_ptr<TemporalFilter> _temporalFilter;
        std::shared_ptr<Amplifier> _amplifier;
        std::shared_ptr<Reconstructor> _reconstructor;
        std::shared_ptr<Subscriber> _subscriber;
        std::shared_ptr<ColorConverter> _colorConverter;
    };

    /**
     * Generates a type.
     *
     * @param value The value to take.
     *
     */
    EXPORT_EVM int type(int value);
}