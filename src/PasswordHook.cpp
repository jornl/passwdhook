#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "config/Config.h"
#include "wordlist/Wordlist.h"

struct AppConfig {
    int minimumScore = 10;
    int maxRepetition = 3;
    int maxRepeatingNumbers = 2;
    bool allowUsernameAsPassword = false;
};

AppConfig config;

std::unordered_map<std::string, int> LoadWordList(const std::string &filename, const std::string &userName = "") {
    std::unordered_map<std::string, int> wordlist;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening wordlist file: " << filename << "\n";
        throw std::runtime_error("Could not open wordlist");
    }
    std::string line;
    while (std::getline(file, line)) {
        auto comma = line.find(',');
        if (comma == std::string::npos)
            continue;

        std::string word = line.substr(0, comma);
        int value = std::stoi(line.substr(comma + 1));
        wordlist[word] = value;
    }

    if (!config.allowUsernameAsPassword && !userName.empty()) {
        wordlist[userName] = 0;
    }

    return wordlist;
}

bool iequals(const std::string &a, const std::string &b) {
    return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                      [](char a, char b) { return std::tolower(a) == std::tolower(b); });
}

int CalculateScore(const std::string &input, const std::unordered_map<std::string, int> &wordList) {
    int totalScore = 0;
    size_t i = 0;
    while (i < input.size()) {
        bool matchedDictionaryWord = false;
        for (const auto &[dictWord, dictValue]: wordList) {
            if (i + dictWord.size() <= input.size()) {
                std::string candidate = input.substr(i, dictWord.size());
                if (iequals(candidate, dictWord)) {
                    totalScore += dictValue;
                    i += dictWord.size();
                    matchedDictionaryWord = true;
                    break;
                }
            }
        }
        if (!matchedDictionaryWord) {
            char currentChar = input[i];
            int runLength = 1;
            size_t j = i + 1;
            while (j < input.size() && input[j] == currentChar) {
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

int main(int argc, char *argv[]) {
    int score{0};

    if (argc != 4) {
        std::cout << "Usage: PasswordHook <wordlist.csv> <username> <password>\n";
        return 1;
    }

    try {
        PasswordHook::Config cfg("./config.ini");
        //
        config.minimumScore = cfg.Get<int>("MinimumScore", 10);
        config.maxRepetition = cfg.Get<int>("MaxRepeatingCharacters", 3);
        config.allowUsernameAsPassword = cfg.Get<bool>("AllowUsernameAsPassword", false);
        config.maxRepeatingNumbers = cfg.Get<int>("MaxRepeatingNumbers", 2);

        const std::string username = argv[2];

        PasswordHook::Wordlist wordlist (argv[1], username, &cfg);

        score = wordlist.GetScore(argv[3]);



    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }


    // Load wordlist
    const auto wordList = LoadWordList(argv[1], argv[2]);
    const std::string password = argv[3];

    // const int score = CalculateScore(password, wordList);

    std::cout << "Password score: " << score << "\n";
    if (score >= config.minimumScore) {
        std::cout << "Password is strong enough.\n";
        return 0;
    } else {
        std::cout << "Password is too weak.\n";
        return 1;
    }
}
