#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>

#include "Wordlist.h"
namespace PasswordHook {
    Wordlist::Wordlist(const std::string &filename, const std::string &userName, const Config &config) :
        m_config(config) {
        Load(filename, userName, m_config.Get<bool>("AllowUsernameAsPassword", false));
    }

    void Wordlist::Load(const std::string &filename, const std::string &userName, bool allowUserNameAsPassword) {
        if (!filename.ends_with(".csv") && !filename.ends_with(".txt")) {
            std::cerr << "Invalid wordlist file format. Expected .csv or .txt file." << "\n";
            throw std::runtime_error("Invalid wordlist file format");
        }

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
            m_wordList[word] = value;
        }

        if (!allowUserNameAsPassword && !userName.empty()) {
            m_wordList[userName] = 0;
        }
    }

    int Wordlist::GetScore(const std::string &word) {
        int score = 0;
        size_t i = 0;
        const int maxRepeatChars = m_config.Get<int>("MaxRepeatingCharacters", 3);
        const int maxRepeatDigits = m_config.Get<int>("MaxRepeatingDigits", 2);

        while (i < word.size()) {
            auto [dictScore, matchFound] = MatchDictionaryWord(word, i);
            if (dictScore > 0) {
                score += dictScore;
                continue;
            }

            if (std::isdigit(word[i])) {
                score += HandleRepeatingDigits(word, i, maxRepeatDigits);
                continue;
            }

            if (!matchFound) {
                score += HandleRepeatingCharacters(word, i, maxRepeatChars);
            }
        }

        return score;
    }

    std::pair<int, bool> Wordlist::MatchDictionaryWord(const std::string &word, size_t &i) {
        for (const auto &[dictWord, dictValue]: m_wordList) {
            if (i + dictWord.size() <= word.size()) {
                if (std::string candidate = word.substr(i, dictWord.size()); iEquals(candidate, dictWord)) {
                    i += dictWord.size();
                    return {dictValue, true};
                }
            }
        }

        return {0, false};
    }

    int Wordlist::HandleRepeatingCharacters(const std::string &word, size_t &i, const int maxRepeatChars) {
        const char currentChar = word[i];
        int runLength = 1;
        size_t j = i + 1;

        while (j < word.size() && word[j] == currentChar && !std::isdigit(word[j])) {
            runLength++;
            j++;
        }

        i = j;

        if (maxRepeatChars == 0) {
            return runLength;
        }

        return (runLength <= maxRepeatChars) ? runLength : runLength / 2;
    }

    int Wordlist::HandleRepeatingDigits(const std::string &word, size_t &i, const int maxRepeatDigits) {
        size_t j = i + 1;
        while (j < word.size() && std::isdigit(word[j])) {
            ++j;
        }
        const int digitRun = static_cast<int>(j - i);
        i = j;

        if (maxRepeatDigits == 0) {
            return digitRun;
        }

        return (digitRun <= maxRepeatDigits) ? digitRun : digitRun / 2;
    }

    bool Wordlist::iEquals(const std::string &a, const std::string &b) {
        return std::ranges::equal(a, b,
                                  [](const char ca, const char cb) { return std::tolower(ca) == std::tolower(cb); });
    }
} // namespace PasswordHook
