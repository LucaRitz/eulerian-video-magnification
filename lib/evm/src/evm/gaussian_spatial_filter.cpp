#include <evm/evm/gaussian_spatial_filter.hpp>

#include <evm/evm/gaussian_pyramid.hpp>

evm::GaussianSpatialFilter::GaussianSpatialFilter(uint8_t levels) : _levels(levels) {
}

evm::SpatialFiltered evm::GaussianSpatialFilter::operator()(const cv::Mat& frame) {

    auto pyramid = std::make_shared<evm::GaussianPyramid>(_colorConverter, frame, _levels);
    return evm::SpatialFiltered {
            pyramid,
            _levels
    };
}