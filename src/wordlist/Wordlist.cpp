#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "Wordlist.h"
namespace PasswordHook {
    Wordlist::Wordlist(const std::string &filename, const std::string &userName, const Config& config): m_config(config) {
        Load(filename, userName, m_config.Get<int>("AllowUsernameAsPassword", false));
    }

    void Wordlist::Load(const std::string &filename, const std::string &userName, bool allowUserNameAsPassword) {
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

    int Wordlist::GetScore(const std::string &input) {
        ScoreDictionaryWord(input);

        return score; // Placeholder for actual score calculation
    }

    void Wordlist::ScoreDictionaryWord(const std::string &word) {
        size_t i = 0;

        const int maxRepeatChars = m_config.Get<int>("MaxRepeatingCharacters", 3);
        const int maxRepeatDigits = m_config.Get<int>("MaxRepeatingDigits", 2);

        while (i < word.size()) {
            bool matchedDictionaryWord = false;

            // Check wordlist matches
            for (const auto &[dictWord, dictValue]: m_wordList) {
                if (i + dictWord.size() <= word.size()) {
                    if (std::string candidate = word.substr(i, dictWord.size()); iEquals(candidate, dictWord)) {
                        score += dictValue; // Only counts as 1, could also subtract full word length here if doing
                                            // effective length
                        i += dictWord.size();
                        matchedDictionaryWord = true;
                        break;
                    }
                }
            }

            if (matchedDictionaryWord)
                continue;

            // Check for repeating digits
            if (std::isdigit(word[i])) {
                size_t j = i + 1;
                while (j < word.size() && std::isdigit(word[j])) {
                    ++j;
                }

                if (const int digitRun = static_cast<int>(j - i); digitRun <= maxRepeatDigits) {
                    score += digitRun;
                } else {
                    score += digitRun / 2;
                }

                i = j;
                continue;
            }

            // Check for repeating characters
            const char currentChar = word[i];
            int runLength = 1;
            size_t j = i + 1;
            while (j < word.size() && word[j] == currentChar && !std::isdigit(word[j])) {
                runLength++;
                j++;
            }

            if (runLength <= maxRepeatChars)
                score += runLength;
            else
                score += runLength / 2;

            i += runLength;
        }
    }

    bool Wordlist::iEquals(const std::string &a, const std::string &b) {
        return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                          [](char a, char b) { return std::tolower(a) == std::tolower(b); });
    }
} // namespace PasswordHook
