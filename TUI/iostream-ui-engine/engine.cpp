#include "engine.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif


namespace
{
    namespace term
    {
        struct Vector2i {
            int y = 0;
            int x = 0;
        };

        std::string moveTo(int row, int col) {
            return "\033[" + std::to_string(row) + ";" + std::to_string(col) + "H";
        }
        std::string moveTo(Vector2i target) {
            return "\033[" + std::to_string(target.y) + ";" + std::to_string(target.x) + "H";
        }
        std::string up(int n = 1) { return "\033[" + std::to_string(n) + "A"; }
        std::string down(int n = 1) { return "\033[" + std::to_string(n) + "B"; }
        std::string right(int n = 1) { return "\033[" + std::to_string(n) + "C"; }
        std::string left(int n = 1) { return "\033[" + std::to_string(n) + "D"; }
        const std::string Home          = "\033[H";
        const std::string EraseLine     = "\033[2K"; // whole line
        const std::string EraseToEnd    = "\033[K";  // cursor to end of line
        const std::string ClearScreen   = "\033[2J\033[H";
        const std::string HideCursor    = "\033[?25l";
        const std::string ShowCursor    = "\033[?25h";
        const std::string AltScreenOn   = "\033[?1049h";
        const std::string AltScreenOff  = "\033[?1049l";
        const std::string SaveCursor    = "\033[s";
        const std::string RestoreCursor = "\033[u";

        Vector2i getSize() {
            // clang-format off
            #ifdef _WIN32
                CONSOLE_SCREEN_BUFFER_INFO csbi;
                if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
                    return {csbi.srWindow.Bottom - csbi.srWindow.Top + 1,
                            csbi.srWindow.Right - csbi.srWindow.Left + 1};
                }
            #else
                winsize w;
                if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) {
                    return {w.ws_row, w.ws_col};
                }
            #endif
            // clang-format on
            return {24, 80}; // fallback if it fails (e.g. output is piped to a file)
        }
    } // namespace term
} // namespace


namespace albert::tui {
    
}