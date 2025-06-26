#include <fstream>
#include <iostream>
#include <string>

#include "config/Config.h"
#include "wordlist/Wordlist.h"


int main(const int argc, char *argv[]) {
    int score{0};

    if (argc != 4) {
        std::cout << "Usage: PasswordHook <wordlist.csv> <username> <password>\n";
        return 1;
    }

    const PasswordHook::Config cfg("./passwdhook.ini");

    const std::string username = argv[2];
    const std::string password = argv[3];

    PasswordHook::Wordlist wordlist(argv[1], username, cfg);

    score = wordlist.GetScore(password);

    std::cout << "Password score: " << score << "\n";
    if (score >= cfg.Get<int>("MinimumScore", 10)) {
        std::cout << "Password is strong enough.\n";
        return 0;
    }
    std::cout << "Password is too weak.\n";
    return 1;
}
