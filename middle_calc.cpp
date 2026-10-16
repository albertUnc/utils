#include <iostream>
#include <format>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

bool validateInput(string s) {
    string part1, part2;
    istringstream ss(s);
    getline(ss, part1, ' ');
    getline(ss, part2);
    try {
        stoi(part1);
        stoi(part2);
    } catch (...) { 
        return false;
    }
    return true;
}
float calculatePlace(float screen, float target) {
    float screenHalf = screen / 2;
    float targetHalf = target / 2;
    float point = screenHalf - targetHalf;
    //round to 1 decimal 
    point = round(point * 100) / 100;
    return point;
}

int main() {
    while (true) {
        cout << "Middle calculator. Enter syntax: screen W, target object W\n";
        string input;
        getline(cin, input);
        if (!validateInput(input)) {
            cout << "Invalid input.\n";
            continue;
        }
        string part1, part2;
        istringstream ss(input);
        getline(ss, part1, ' ');
        getline(ss, part2);
        int screenW = stoi(part1);
        int targetW = stoi(part2);
        float point = calculatePlace(screenW, targetW);
        cout << format("Most left point of the target object for it to be in the middle:\n{}\n\n", point);
    }
}