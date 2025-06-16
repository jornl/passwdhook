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

        int score{0};
        int GetScore(const std::string &input);



    private:
        std::unordered_map<std::string, int> m_wordList;
        const Config& m_config;

        void Load(const std::string &filename, const std::string &userName,
                  bool allowUserNameAsPassword = false);

        static bool iEquals(const std::string &a, const std::string &b);

        void ScoreDictionaryWord(const std::string &word);
    };
} // namespace PasswordHook


#endif // WORDLIST_H
