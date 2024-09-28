#include <evm/evm/display_subscriber.hpp>

void evm::DisplaySubscriber::operator()(const evm::Reconstructed& reconstructed) {

    cv::Mat orig = reconstructed._original;
    orig.convertTo(orig, CV_8UC3);
    cv::imshow("ORIG", orig);

    cv::Mat disp = reconstructed._resulting;
    double min,max;
    minMaxLoc(disp, &min, &max);
    disp.convertTo(disp, CV_8UC3, 255.0/(max-min), -min * 255.0/(max-min));
    cv::imshow("DI", disp);

    cv::Mat ampli = reconstructed._amplified;
    ampli.convertTo(ampli, CV_8UC3);
    cv::imshow("AMP", ampli);


    cv::Mat channels[3];
    cv::split(ampli, channels);
    auto zeros = cv::Mat::zeros(ampli.size(), CV_8UC1);
    channels[0] = zeros;
    channels[2] = zeros;
    cv::merge(channels, 3, ampli);

    minMaxLoc(ampli, &min, &max);
    ampli.convertTo(ampli, CV_8UC3, 255.0/(max-min), -min * 255.0/(max-min));
    cv::imshow("Green", ampli);

    cv::waitKey(34);
}