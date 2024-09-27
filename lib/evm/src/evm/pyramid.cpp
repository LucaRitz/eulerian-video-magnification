#include <evm/evm/pyramid.hpp>
#include <evm/color_converter.hpp>

evm::Pyramid::Pyramid(std::shared_ptr<ColorConverter> colorConverter) :
    _colorConverter(std::move(colorConverter)) {

}

cv::Mat evm::Pyramid::at(int level) const {
    if (level > levels() - 1) {
        return cv::Mat{};
    }
    return _levels.at(level).clone();
}

void evm::Pyramid::operator*(const std::vector<float>& factors) const {

    for(int i = 0; i < _levels.size(); i++) {
        auto& mat = _levels.at(i);
        if (factors.size() > i) {
            mat *= factors.at(i);
        }
    }
}

void evm::Pyramid::clear() {

    _levels.clear();
}

int evm::Pyramid::levels() const {

    return _levels.size();
}

void evm::Pyramid::push_back(cv::Mat value) {

    _levels.emplace_back(value);
}