#include <iostream>
#include <string>
#include <limits>

namespace albert::input {

inline int getUserInput(int min = 0, int max = std::numeric_limits<int>::max()) {
    while (true) {
        std::string input;
        getline(std::cin, input);
        int i;
        try {
            i = stoi(input);
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid input! Enter a number between " << min << " and " << max << "\n";
            continue;
        } catch (const std::out_of_range&) {
            std::cout << "Number too large or too small!\n";
            continue;
        }
        if (i > max || i < min) {
            std::cout << "Invalid input! Enter a number between " << min << " and " << max << "\n";
            continue;
        } else return i;
    }
}

}