#include <evm/evm/amplifier.hpp>
#include <evm/evm/pyramid.hpp>

evm::Amplifier::Amplifier(std::vector<float> factors) : _factors(std::move(factors)) {

}

void evm::Amplifier::operator*(const std::vector<std::shared_ptr<evm::Pyramid>>& pyramids) {

    for(auto& pyramid : pyramids) {
        (*pyramid) * _factors;
    }
}
