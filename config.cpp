#include "config.hpp"

#include <algorithm>

namespace defaults {
const std::string kDefaultPreambule = R"(
/* This file is generated, do not edit */
)";
const std::string kDefaultMockSuffix = "Mock";
}  // namespace defaults

Config::Config() {}

std::string Config::preambule() const { return defaults::kDefaultPreambule; }
std::string Config::mockSuffix() const { return defaults::kDefaultMockSuffix; }

std::string Config::ifdefSuffix() const {
    std::string suffix = "__";
    suffix += mockSuffix();
    std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::toupper);
    suffix += "_HPP";
    return suffix;
}
