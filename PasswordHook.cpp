#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <cctype>
#include <algorithm>

struct Config
{
    int minimumScore = 10;
    int maxRepetition = 3;
    bool allowUsernameAsPassword = false;
};

Config config;

std::unordered_map<std::string, std::string> LoadConfig(const std::string &filename)
{
    std::unordered_map<std::string, std::string> config;
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Error opening config file: " << filename << "\n";
        throw std::runtime_error("Could not open config file");
    }
    std::string line;
    while (std::getline(file, line))
    {
        auto comment = line.find_first_of(";#");
        if (comment != std::string::npos)
            line = line.substr(0, comment);
            
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.empty() || line[0] == '[')
            continue;

        auto eq = line.find('=');
        if (eq != std::string::npos)
        {
            config[line.substr(0, eq)] = line.substr(eq + 1);
        }
    }
    return config;
}

std::unordered_map<std::string, int> LoadWordList(const std::string &filename, const std::string &userName = "")
{
    std::unordered_map<std::string, int> wordlist;
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Error opening wordlist file: " << filename << "\n";
        throw std::runtime_error("Could not open wordlist");
    }
    std::string line;
    while (std::getline(file, line))
    {
        auto comma = line.find(',');
        if (comma == std::string::npos)
            continue;

        std::string word = line.substr(0, comma);
        int value = std::stoi(line.substr(comma + 1));
        wordlist[word] = value;
    }

    if (!config.allowUsernameAsPassword && !userName.empty())
    {
        wordlist[userName] = 0;
    }

    return wordlist;
}

bool iequals(const std::string &a, const std::string &b)
{
    return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                      [](char a, char b)
                      { return std::tolower(a) == std::tolower(b); });
}

int CalculateScore(const std::string &input, const std::unordered_map<std::string, int> &wordList)
{
    int totalScore = 0;
    size_t i = 0;
    while (i < input.size())
    {
        bool matchedDictionaryWord = false;
        for (const auto &[dictWord, dictValue] : wordList)
        {
            if (i + dictWord.size() <= input.size())
            {
                std::string candidate = input.substr(i, dictWord.size());
                if (iequals(candidate, dictWord))
                {
                    totalScore += dictValue;
                    i += dictWord.size();
                    matchedDictionaryWord = true;
                    break;
                }
            }
        }
        if (!matchedDictionaryWord)
        {
            char currentChar = input[i];
            int runLength = 1;
            size_t j = i + 1;
            while (j < input.size() && input[j] == currentChar)
            {
                runLength++;
                j++;
            }
            if (runLength <= config.maxRepetition)
                totalScore += runLength;
            else
                totalScore += runLength / 2;
            i += runLength;
        }
    }
    return totalScore;
}

int main(int argc, char *argv[])
{
    // Usage: PasswordHook <wordlist.csv> <username> <password>
    if (argc != 4)
    {
        std::cout << "Usage: PasswordHook <wordlist.csv> <username> <password>\n";
        return 1;
    }

    std::ifstream configFile("config.ini");
    if (configFile)
    {
        auto cfg = LoadConfig("config.ini");
        if (cfg.count("MinimumScore"))
            config.minimumScore = std::stoi(cfg["MinimumScore"]);

        if (cfg.count("MaxRepeatingCharacters"))
            config.maxRepetition = std::stoi(cfg["MaxRepeatingCharacters"]);

        if (cfg.count("AllowUsernameAsPassword"))
        {
            std::string val = cfg["AllowUsernameAsPassword"];
            std::transform(val.begin(), val.end(), val.begin(), ::tolower);
            config.allowUsernameAsPassword = (val == "true" || val == "1" || val == "yes");
        }
    }

    // Load wordlist
    auto wordList = LoadWordList(argv[1], argv[2]);
    std::string password = argv[3];

    int score = CalculateScore(password, wordList);

    std::cout << "Password score: " << score << "\n";
    if (score >= config.minimumScore)
    {
        std::cout << "Password is strong enough.\n";
        return 0;
    }
    else
    {
        std::cout << "Password is too weak.\n";
        return 1;
    }
}