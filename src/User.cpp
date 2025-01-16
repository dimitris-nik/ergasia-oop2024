#include "../include/User.h"
#include <iostream>
#include <fstream>
User::User(std::string username, std::string password, bool isAdmin) : username(username), password(password), isAdmin(isAdmin) {} // we only need initializer list here

bool User::checkPassword(std::string& password) const {
    return this->password == password;
}

std::ostream& operator<<(std::ostream& os, const User& user) {
    os << user.username + "," + user.password + "," + (user.isAdmin ? "1" : "0");
    return os;
}

std::string User::getUsername() const {
    return username;
}

void UserManager::addUser(User* user) {
    users[user->getUsername()] = user; 
}

void UserManager::removeUser(const std::string& username) {
    auto it = users.find(username);
    if (it != users.end()) {
        delete it->second;
        users.erase(it);
    }
}

User * UserManager::findUser(const std::string& username) const {
    auto it = users.find(username);
    if (it != users.end()) {
        return it->second;
    }
    return nullptr;
}

UserManager::~UserManager() {
    for (auto& user : users) {
        delete user.second;
    }
}
void UserManager::saveUsers(const std::string& usersFile) const {
    std::ofstream file(usersFile);
    if (!file.is_open()) {
        std::cerr << "Error opening users file." << std::endl;
        return;
    }
    for (const auto& user : users) {
        file << *user.second << std::endl;
    }
    file.close();
}
