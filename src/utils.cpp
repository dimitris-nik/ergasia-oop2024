#include "../include/utils.h"

using namespace std;
// Remove leading and trailing whitespaces
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Read functions to better handle input
int readInt() {
    string input;
    int num;
    while (true) {
        getline(cin, input);
        try {
            num = stoi(input);
            break;
        } catch (invalid_argument& e) {
            cout << "Invalid input. Please enter a number: ";
        }
    }
    return num;
}

string readString() {
    string input;
    getline(cin, input);
    return input;
}

double readDouble() {
    string input;
    double num;
    while (true) {
        getline(cin, input);
        try {
            num = stod(input);
            break;
        } catch (invalid_argument& e) {
            cout << "Invalid input. Please enter a number: ";
        }
    }
    return num;
}