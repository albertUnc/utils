#pragma once

#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace albert::tui
{
    namespace Color
    {
        inline const std::string Black   = "\033[30m";
        inline const std::string Red     = "\033[31m";
        inline const std::string Green   = "\033[32m";
        inline const std::string Yellow  = "\033[33m";
        inline const std::string Blue    = "\033[34m";
        inline const std::string Magenta = "\033[35m";
        inline const std::string Cyan    = "\033[36m";
        inline const std::string White   = "\033[37m";

        inline const std::string BrightBlack   = "\033[90m";
        inline const std::string BrightRed     = "\033[91m";
        inline const std::string BrightGreen   = "\033[92m";
        inline const std::string BrightYellow  = "\033[93m";
        inline const std::string BrightBlue    = "\033[94m";
        inline const std::string BrightMagenta = "\033[95m";
        inline const std::string BrightCyan    = "\033[96m";
        inline const std::string BrightWhite   = "\033[97m";

        inline const std::string BackgroundBlack   = "\033[40m";
        inline const std::string BackgroundRed     = "\033[41m";
        inline const std::string BackgroundGreen   = "\033[42m";
        inline const std::string BackgroundYellow  = "\033[43m";
        inline const std::string BackgroundBlue    = "\033[44m";
        inline const std::string BackgroundMagenta = "\033[45m";
        inline const std::string BackgroundCyan    = "\033[46m";
        inline const std::string BackgroundWhite   = "\033[47m";

        inline const std::string Reset      = "\033[0m";
        inline const std::string Bold       = "\033[1m";
        inline const std::string Italic     = "\033[3m";
        inline const std::string Underlined = "\033[4m";
    } // namespace Color
    class Button {
    private:
        std::function<void()> m_handler = nullptr;

    public:
        std::string highlightColor = Color::BrightYellow;
        std::string primaryColor   = Color::White;

        bool visible     = true;
        bool enabled     = true;
        bool highlighted = false;
        bool bold        = false;
        bool italic      = false;
        bool underlined  = false;
        std::string text = "button";

        void setOnClick(std::function<void()> func) { m_handler = func; }
        void onClicked() {
            if (m_handler && enabled) {
                m_handler();
            }
        }

        void draw() {
            if (!visible) {
                for (size_t i = 0; i < text.size(); ++i) std::cout << " ";
                return;
            }
            std::string attributes = Color::Reset;
            attributes += highlighted ? highlightColor : primaryColor;
            if (bold) attributes += Color::Bold;
            if (italic) attributes += Color::Italic;
            if (underlined) attributes += Color::Underlined;
            std::cout << std::format("{}{}{}", attributes, text, Color::Reset);
        }
    };
    class Label {
        public:
        std::string text = "label";
        std::string primaryColor = Color::White;
        std::string secondaryColor = Color::Blue;

        bool visible = true;
        bool underlined = false;
        bool italic = false;
        bool bold = false;
        bool usingSecondary = false;

        void draw() {
            if (!visible) {
                for (size_t i = 0; i < text.size(); ++i) std::cout << " ";
                return;
            }
            std::string attributes = Color::Reset;
            attributes += usingSecondary ? secondaryColor : primaryColor;
            if (bold) attributes += Color::Bold;
            if (italic) attributes += Color::Italic;
            if (underlined) attributes += Color::Underlined;
            std::cout << std::format("{}{}{}", attributes, text, Color::Reset);
        }
    };
    class Window {
    };
} // namespace albert::tui