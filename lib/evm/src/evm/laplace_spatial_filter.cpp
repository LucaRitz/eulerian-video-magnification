#include <evm/evm/laplace_spatial_filter.hpp>
#include <evm/evm/laplace_pyramid.hpp>

evm::LaplaceSpatialFilter::LaplaceSpatialFilter(uint8_t levels) : _levels(levels) {
}

evm::SpatialFiltered evm::LaplaceSpatialFilter::operator()(const cv::Mat& frame) {

    auto pyramid = std::make_shared<evm::LaplacePyramid>(_colorConverter, frame, _levels);
    return evm::SpatialFiltered {
        pyramid,
        _levels
    };
}