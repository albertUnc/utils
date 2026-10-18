#pragma once

#include <iostream>
#include <string>
// clang-format off
#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#endif
// clang-format on
namespace albert::clear
{
    inline void clearScreen() {
        // clang-format off
        static const bool ansiReady = []() {
        #ifdef _WIN32
            HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD mode = 0;
            if (out != INVALID_HANDLE_VALUE && GetConsoleMode(out, &mode)) SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        #endif
            return true;
        }();
        // clang-format on
        (void)ansiReady;
        std::cout << "\033[2J\033[3J\033[H" << std::flush;
    }

    inline const std::string DEFAULT_PROMPT = "Press ENTER to continue\n";
    inline std::string HOLD_PROMPT          = DEFAULT_PROMPT;
    inline void resetHoldPrompt() { HOLD_PROMPT = DEFAULT_PROMPT; }
    inline void changeHoldPrompt(const std::string &temp) { HOLD_PROMPT = temp; }
    inline void hold(const std::string prompt = HOLD_PROMPT) {
        prompt == "default" ? std::cout << prompt : std::cout << DEFAULT_PROMPT;
        std::string bruh;
        std::getline(std::cin, bruh);
        clearScreen();
    }
} // namespace albert::clear    