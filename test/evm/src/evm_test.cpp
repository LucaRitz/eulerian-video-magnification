#include <evm/evm.hpp>
#include <evm/capture/source.hpp>
#include <evm/capture/selector.hpp>
#include <evm/capture/stabilizer.hpp>
#include <evm/capture/device_source.hpp>
#include <evm/evm/laplace_spatial_filter.hpp>
#include <evm/evm/gaussian_spatial_filter.hpp>
#include <evm/evm/ideal_bandpass_temporal_filter.hpp>
#include <evm/evm/amplifier.hpp>
#include <evm/evm/reconstructor.hpp>
#include <evm/evm/level_reconstructor.hpp>
#include <evm/evm/display_subscriber.hpp>
#include <evm/evm/video_subscriber.hpp>
#include <evm/color_converter.hpp>

#include <gtest/gtest.h>
#include <iostream>
#include <chrono>
#include <functional>
#include <string>

#include <thread>
#include <chrono>

namespace evm::test {

    template<class R>
    R measured(const std::function<R ()>& function, const std::string& step = "");
}

TEST(Pipeline, should_magnify_color_video) {

    auto colorConverterHSV = std::make_shared<evm::ColorSpaceConverter>(cv::COLOR_RGB2HSV, cv::COLOR_GRAY2RGB, cv::COLOR_HSV2RGB, cv::COLOR_RGB2GRAY);
    auto colorConverterYUV = std::make_shared<evm::ColorSpaceConverter>(cv::COLOR_RGB2YUV, cv::COLOR_GRAY2RGB, cv::COLOR_YUV2RGB, cv::COLOR_RGB2GRAY);
    auto builder = evm::PipelineBuilder {}
        << std::make_shared<evm::VideoSource>("resources/mit_dude_orig.mp4")
        << std::make_shared<evm::DefaultSelector>() // TODO: Rename "AllSelector"
        << std::make_shared<evm::DefaultStabilizer>() // TODO: Rename "NoStabilizer"
        << std::make_shared<evm::GaussianSpatialFilter>(5)
        << std::make_shared<evm::IdealBandpassTemporalFilter>(0.833, 1, 0)
        << std::make_shared<evm::Amplifier>(std::vector<float>{100})
        << std::make_shared<evm::LevelReconstructor>(0)
        // << std::make_shared<evm::DisplaySubscriber>();
        << std::make_shared<evm::VideoSubscriber>("resources/mit_dude_result.mp4", 30);
    auto pipeline = builder.build();

    // Act
    pipeline.join();
}

TEST(Pipeline, should_magnify_color_webcam) {

    auto colorConverterHSV = std::make_shared<evm::ColorSpaceConverter>(cv::COLOR_RGB2HSV, cv::COLOR_GRAY2RGB, cv::COLOR_HSV2RGB, cv::COLOR_RGB2GRAY);
    auto colorConverterYUV = std::make_shared<evm::ColorSpaceConverter>(cv::COLOR_RGB2YUV, cv::COLOR_GRAY2RGB, cv::COLOR_YUV2RGB, cv::COLOR_RGB2GRAY);
    auto builder = evm::PipelineBuilder {}
            << std::make_shared<evm::DeviceSource>(0)
            << std::make_shared<evm::DefaultSelector>() // TODO: Rename "AllSelector"
            << std::make_shared<evm::DefaultStabilizer>() // TODO: Rename "NoStabilizer"
            << std::make_shared<evm::GaussianSpatialFilter>(5)
            << std::make_shared<evm::IdealBandpassTemporalFilter>(0.833, 1, 0)
            << std::make_shared<evm::Amplifier>(std::vector<float>{1000})
            << std::make_shared<evm::LevelReconstructor>(0)
            << std::make_shared<evm::DisplaySubscriber>();
    auto pipeline = builder.build();

    // Act
    pipeline.join();
}

template<class R>
R evm::test::measured(const std::function<R ()>& function, const std::string& step) {

    auto start = std::chrono::high_resolution_clock::now();
    auto result = function();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << (step.empty() ? "Measured step" : step) << " took " << std::to_string(duration.count()) << " ms" << std::endl;

    return result;
}