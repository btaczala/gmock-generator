#ifndef CONFIG_HPP_ELI6MD50
#define CONFIG_HPP_ELI6MD50

#include <string>

struct Config {
    Config();
    std::string preambule() const;
    std::string mockSuffix() const;
    std::string ifdefSuffix() const;
};

#endif /* end of include guard: CONFIG_HPP_ELI6MD50 */
