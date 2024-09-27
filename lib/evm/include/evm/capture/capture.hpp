#pragma once

#include "../macro_definition.hpp"
#include "../worker.hpp"
#include "../data.hpp"

#include <memory>
#include <queue>
#include <mutex>
#include <opencv2/opencv.hpp>

namespace evm {

    class Source;
    class Selector;
    class Stabilizer;
    class ColorConverter;
    class SourceFrame;

    class EXPORT_EVM Capture : public Worker<Captured> {

    public:
        explicit Capture(std::shared_ptr<WorkingSet<Captured>>, std::shared_ptr<Source>,
                std::shared_ptr<Selector>, std::shared_ptr<Stabilizer>,
                std::shared_ptr<ColorConverter> colorConverter);

    protected:
        virtual void execute(std::shared_ptr<WorkingSet<Captured>> data, std::atomic<bool>& running) override;

    private:
        const std::shared_ptr<Source> _source;
        const std::shared_ptr<Selector> _selector;
        const std::shared_ptr<Stabilizer> _stabilizer;
        const std::shared_ptr<ColorConverter> _colorConverter;

        void captureFrame(std::shared_ptr<WorkingSet<Captured>>);
        bool isValidFrame(std::shared_ptr<WorkingSet<Captured>>, const evm::SourceFrame&);
        void processFrame(std::shared_ptr<WorkingSet<Captured>>, evm::SourceFrame&);
        void signalAndStopIfDone(std::shared_ptr<WorkingSet<Captured>>);
    };
}