#ifndef CONFIG_HPP_ELI6MD50
#define CONFIG_HPP_ELI6MD50

#include <experimental/optional>
#include <string>

struct Config {
    Config();
    Config(const std::string& preambule, const std::string& mockSuffix,
           const std::string& ifdefSuffix);
    std::string preambule() const;
    std::string mockSuffix() const;
    std::string ifdefSuffix() const;

   private:
    std::experimental::optional<std::string> _preambule;
    std::experimental::optional<std::string> _mockSuffix;
    std::experimental::optional<std::string> _ifdefSuffix;
};

#endif /* end of include guard: CONFIG_HPP_ELI6MD50 */
