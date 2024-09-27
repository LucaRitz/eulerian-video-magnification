#pragma once

#include "../macro_definition.hpp"
#include "temporal_filter.hpp"
#include "pyramid.hpp"

#include <unordered_map>

namespace evm {

    class EXPORT_EVM IdealBandpassTemporalFilter : public TemporalFilter {

    public:
        IdealBandpassTemporalFilter(double, double, int level = -1);

        TemporalFiltered operator()(const SpatialFiltered&, uint16_t) override;
        TemporalFiltered remaining(uint16_t) override;

    private:
        double _lowerFreq;
        double _upperFreq;
        int _level;

        std::unordered_map<uint8_t, cv::Mat> _filterMat;
        uint32_t _bufferSize;
        std::vector<cv::Size> _originalSizes;
        std::vector<evm::SpatialFiltered> _spatialFiltered;

        evm::TemporalFiltered filter(uint32_t, uint16_t);
        cv::Mat concat(uint8_t level, cv::Mat& dest, const std::shared_ptr<evm::Pyramid>& pyramids);
        static uint32_t bufferSize(uint16_t fps);
        static void insert(const cv::Mat& value, uint64_t originalHeight, std::vector<std::shared_ptr<evm::Pyramid>>& vec);
        static cv::Mat buildFilterOfFrequencyBand(cv::Mat& filter, double lowerBound, double upperBound, double sampleRate);
    };
}