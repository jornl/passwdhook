#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "Config.h"


namespace PasswordRater {
    Config::Config(const std::string &filename) {
        Load(filename);
    }

    void Config::Load(const std::string &filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error opening config file: " << filename << ". Using default values.\n";
        }

        std::string line;

        while (std::getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t"));

            if (line.empty() || line[0] == '#' || line[0] == ';' || line[0] == '[')
                continue;

            if (const auto eq = line.find('='); eq != std::string::npos) {
                m_configMap[line.substr(0, eq)] = line.substr(eq + 1);
            }
        }
    }
} // namespace PasswordHook
