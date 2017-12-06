#include "config.hpp"

#include <algorithm>

namespace defaults {
const std::string kDefaultPreambule = R"(
/* This file is generated, do not edit */
)";
const std::string kDefaultMockSuffix = "Mock";
}  // namespace defaults

Config::Config(const std::string& preambule, const std::string& mockSuffix,
               const std::string& ifdefSuffix)
    : _preambule(preambule),
      _mockSuffix(mockSuffix),
      _ifdefSuffix(ifdefSuffix) {}

std::string Config::preambule() const {
    if (_preambule) {
        return *_preambule;
    } else {
        return defaults::kDefaultPreambule;
    }
}
std::string Config::mockSuffix() const {
    if (_mockSuffix) {
        return *_mockSuffix;
    } else {
        return defaults::kDefaultMockSuffix;
    }
};

std::string Config::ifdefSuffix() const {
    std::string suffix = "__";
    suffix += mockSuffix();
    std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::toupper);
    suffix += "_HPP";
    return suffix;
}
