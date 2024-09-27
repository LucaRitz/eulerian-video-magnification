#include <evm/evm/ideal_bandpass_temporal_filter.hpp>

#include <evm/color_converter.hpp>

#include <opencv2/opencv.hpp>
#include <algorithm>

evm::IdealBandpassTemporalFilter::IdealBandpassTemporalFilter(double lowerFreq, double upperFreq, int level) :
        _lowerFreq(lowerFreq),
        _upperFreq(upperFreq),
        _level(level),
        _bufferSize(0),
        _originalSizes() {

}

evm::TemporalFiltered evm::IdealBandpassTemporalFilter::operator()(const SpatialFiltered& spatialFiltered, uint16_t fps) {

    auto spatialFilteredClone = spatialFiltered.clone();
    spatialFilteredClone._spatialFiltered->clear();

    _spatialFiltered.push_back(spatialFilteredClone);

    for (uint8_t level = 0; level < spatialFiltered._levels; level++) {

        if (_level != -1 && level != _level) {
            continue;
        }

        if (_filterMat.find(level) == _filterMat.end()) {
            _filterMat[level] = cv::Mat{};
        }

        auto& filterMat = _filterMat.find(level)->second;
        concat(level, filterMat, spatialFiltered._spatialFiltered);
    }
    _bufferSize++;

    auto optimalBufferSize = bufferSize(fps);

    if (_bufferSize >= optimalBufferSize) {
        int buffDiff = _bufferSize - optimalBufferSize;
        int toProcess = std::max(1, buffDiff);

        return filter(toProcess, fps);
    }

    return evm::TemporalFiltered{};
}

evm::TemporalFiltered evm::IdealBandpassTemporalFilter::remaining(uint16_t fps) {

    return filter(_spatialFiltered.size(), fps);
}

evm::TemporalFiltered evm::IdealBandpassTemporalFilter::filter(uint32_t toProcess, uint16_t fps) {

    std::vector<std::shared_ptr<evm::Pyramid>> results;
    for(uint32_t i = 0; i < toProcess; i++) {
        results.push_back(_spatialFiltered.at(i)._spatialFiltered);
    }

    auto& spatialFiltered = _spatialFiltered.at(0);

    for (uint8_t level = 0; level < spatialFiltered._levels; level++) {

        if (_level != -1 && _level != level) {
            continue;
        }

        if (_filterMat.find(level) == _filterMat.end()) {
            _filterMat[level] = cv::Mat{};
        }

        auto& filterMat = _filterMat.find(level)->second;

        auto originalHeight = _originalSizes.at(level).height;

        if (!results.empty()) {
            auto* channelsToFilter = new cv::Mat[filterMat.channels()];
            cv::split(filterMat, channelsToFilter);

            for (uint8_t i = 0; i < filterMat.channels(); i++) {
                cv::Mat channelToFilter = channelsToFilter[i];

                auto width = channelToFilter.cols;
                auto height = cv::getOptimalDFTSize(channelToFilter.rows);

                cv::Mat tempImg;
                cv::copyMakeBorder(channelToFilter, tempImg,
                                   0, height - channelToFilter.rows,
                                   0, width - channelToFilter.cols,
                                   cv::BORDER_CONSTANT, cv::Scalar::all(0));

                cv::dft(tempImg, tempImg, cv::DFT_ROWS | cv::DFT_SCALE);

                cv::Mat filter = tempImg.clone();
                buildFilterOfFrequencyBand(filter, _lowerFreq, _upperFreq, fps);

                cv::mulSpectrums(tempImg, filter, tempImg, cv::DFT_ROWS);

                cv::idft(tempImg, tempImg, cv::DFT_ROWS | cv::DFT_SCALE);

                tempImg(cv::Rect(0, 0, channelToFilter.cols, channelToFilter.rows)).copyTo(channelsToFilter[i]);
            }

            cv::Mat res;
            cv::merge(channelsToFilter, filterMat.channels(), res);
            delete[] channelsToFilter;

            cv::normalize(res, res, 0, 1, cv::NORM_MINMAX);
            insert(res, originalHeight, results);

            _filterMat[level] = filterMat.colRange(results.size(), filterMat.cols);
        }
    }

    if (!results.empty()) {
        _spatialFiltered.erase(_spatialFiltered.begin(), _spatialFiltered.begin() + results.size());
        _bufferSize -= results.size();
    }

    return TemporalFiltered{results};
}

uint32_t evm::IdealBandpassTemporalFilter::bufferSize(uint16_t fps) {

    // Calculate number of images needed to represent 2 seconds of film material
    uint32_t round = std::max(2*fps, 16);
    round--;
    round |= round >> 1;
    round |= round >> 2;
    round |= round >> 4;
    round |= round >> 8;
    round |= round >> 16;
    round++;

    return round;
}

cv::Mat evm::IdealBandpassTemporalFilter::concat(uint8_t level, cv::Mat& dest, const std::shared_ptr<evm::Pyramid>& pyramide) {

    cv::Mat singleFrame = pyramide->at(level).clone();
    if (_originalSizes.size() <= level) {
        _originalSizes.push_back(singleFrame.size());
    }

    cv::resize(singleFrame, singleFrame, _originalSizes.at(level), 0, 0, cv::INTER_LINEAR);
    singleFrame = singleFrame.reshape(singleFrame.channels(), singleFrame.cols * singleFrame.rows).clone();

    if (dest.cols == 0) {
        singleFrame.copyTo(dest);
    } else {
        hconcat(dest, singleFrame, dest);
    }

    return dest;
}

cv::Mat evm::IdealBandpassTemporalFilter::buildFilterOfFrequencyBand(cv::Mat& filter, double lowerBound, double upperBound,
                                                                 double sampleRate) {
    double width = filter.cols;
    double height = filter.rows;

    double fl = 2 * lowerBound * width / sampleRate;
    double fh = 2 * upperBound * width / sampleRate;

    double response;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x >= fl && x <= fh) {
                response = 1.0f;
            } else
                response = 0.0f;

            filter.at<float>(y, x) = response;
        }
    }

    return filter;
}

void
evm::IdealBandpassTemporalFilter::insert(const cv::Mat& allFrames, uint64_t originalHeight, std::vector<std::shared_ptr<evm::Pyramid>>& vec) {

    for (int i = 0; i < vec.size(); i++) {
        int column = i;
        cv::Mat line = allFrames.col(column).clone();
        line = line.reshape(line.channels(), originalHeight).clone();
        auto& pyramid = vec.at(i);
        pyramid->push_back(line);
    }
}