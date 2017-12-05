#ifndef CONFIG_HPP_ELI6MD50
#define CONFIG_HPP_ELI6MD50

#include <optional>
#include <string>

struct Config {
    Config();
    Config(const std::string& preambule, const std::string& mockSuffix,
           const std::string& ifdefSuffix);
    std::string preambule() const;
    std::string mockSuffix() const;
    std::string ifdefSuffix() const;

   private:
    std::optional<std::string> _preambule;
    std::optional<std::string> _mockSuffix;
    std::optional<std::string> _ifdefSuffix;
};

#endif /* end of include guard: CONFIG_HPP_ELI6MD50 */
