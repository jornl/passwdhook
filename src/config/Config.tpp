#ifndef CONFIG_TPP
#define CONFIG_TPP

#include <algorithm>
#include <string>
#include <ranges>

namespace PasswordHook {

    template<typename Type>
    Type Config::Get(const std::string &key, Type defaultValue) const {
        if (!m_configMap.contains(key)) {
            return defaultValue;
        }

        const auto &value = m_configMap.at(key);

        if constexpr (std::is_same_v<Type, int>) {
            try {
                return std::stoi(value);
            } catch (...) {
                return defaultValue;
            }
        }
        if constexpr (std::is_same_v<Type, bool>) {
            std::string lowerValue = value;
            std::ranges::transform(lowerValue, lowerValue.begin(), ::tolower);
            return (lowerValue == "true" || lowerValue == "yes");
        }
        if constexpr (std::is_same_v<Type, std::string>) {
            return value;
        }

        static_assert(std::is_same_v<Type, int> || std::is_same_v<Type, bool> || std::is_same_v<Type, std::string>,
                      "Unsupported type for Config::Get");

        return defaultValue;
    }


} // namespace PasswordHook

#endif // CONFIG_TPP