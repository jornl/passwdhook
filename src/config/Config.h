
#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

namespace PasswordHook {
    class Config {
    public:
        explicit Config(const std::string &filename);

        template<typename Type>
        Type Get(const std::string &key, Type defaultValue) const;

    private:
        std::unordered_map<std::string, std::string> m_configMap;
        void Load(const std::string &filename);
    };


} // namespace PasswordHook
#include "Config.tpp"

#endif // CONFIG_H
