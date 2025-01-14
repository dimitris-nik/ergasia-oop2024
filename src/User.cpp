#include "../include/User.h"

User::User(std::string username, std::string password, bool isAdmin) : username(username), password(password), isAdmin(isAdmin) {} // we only need initializer list here

bool User::checkCredentials(std::string& username, std::string& password) {
    return this->username == username && this->password == password;
}

bool User::checkCredentials(std::string& username) {
    return this->username == username;
}

std::ostream& operator<<(std::ostream& os, const User& user) {
    os << user.username + "," + user.password + "," + (user.isAdmin ? "1" : "0");
    return os;
}

