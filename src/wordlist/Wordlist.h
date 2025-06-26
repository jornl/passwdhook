#ifndef WORDLIST_H
#define WORDLIST_H

#include <string>
#include <unordered_map>
#include "src/config/Config.h"


namespace PasswordHook {
    class Wordlist {
    public:
        explicit Wordlist(const std::string &filename, const std::string &userName,
                          const Config& config);

        int GetScore(const std::string &word);

    private:
        std::unordered_map<std::string, int> m_wordList;
        const Config& m_config;

        void Load(const std::string &filename, const std::string &userName,
                  bool allowUserNameAsPassword = false);

        static bool iEquals(const std::string &a, const std::string &b);

        std::pair<int, bool> MatchDictionaryWord(const std::string &word, size_t &i);
        static int HandleRepeatingCharacters(const std::string &word, size_t &i, int maxRepeatChars);
        static int HandleConsecutiveDigits(const std::string &word, size_t &i, int maxRepeatDigits);
    };
} // namespace PasswordHook


#endif // WORDLIST_H
