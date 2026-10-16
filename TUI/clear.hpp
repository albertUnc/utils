#pragma once

#include <cstdlib>
#include <string>
#include <iostream>
namespace albert::clear
{
    inline void clearScreen() {
        #ifdef _WIN32
            std::system("cls");
        #else
            std::system("clear");
        #endif
    }

    inline const std::string DEFAULT_PROMPT = "Press ENTER to continue\n";
    inline std::string HOLD_PROMPT = DEFAULT_PROMPT;
    inline void changeHoldPrompt(const std::string& temp) { HOLD_PROMPT = temp; }
    inline void hold(const std::string prompt = HOLD_PROMPT) {
        prompt == "default" ? std::cout << prompt : std::cout << DEFAULT_PROMPT;
        std::string bruh;
        std::getline(std::cin, bruh);
        clearScreen();
    }
}