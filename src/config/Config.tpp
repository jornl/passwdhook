#pragma once

#include <algorithm>
#include <string>

namespace PasswordHook {

    template<typename Type>
    Type Config::Get(const std::string &key, Type defaultValue) const {
        if (!m_configMap.contains(key)) {
            return defaultValue;
        }

        const auto &value = m_configMap.at(key);

        if constexpr (std::is_same_v<Type, int>) {
            return std::stoi(value);
        }
        if constexpr (std::is_same_v<Type, bool>) {
            std::string lowerValue = value;
            std::transform(lowerValue.begin(), lowerValue.end(), lowerValue.begin(), ::tolower);
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
