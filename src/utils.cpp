#include "../include/utils.h"

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

int readInt() {
    std::string input;
    int num;
    while (true) {
        std::getline(std::cin, input);
        try {
            num = std::stoi(input);
            break;
        } catch (std::invalid_argument& e) {
            std::cout << "Invalid input. Please enter a number: ";
        }
    }
    return num;
}

std::string readString() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

double readDouble() {
    std::string input;
    double num;
    while (true) {
        std::getline(std::cin, input);
        try {
            num = std::stod(input);
            break;
        } catch (std::invalid_argument& e) {
            std::cout << "Invalid input. Please enter a number: ";
        }
    }
    return num;
}