#include "../include/User.h"
#include "../include/Customer.h"
#include <iostream>
#include <fstream>

using namespace std;

User::User(string username, string password, bool isAdmin) : username(username), password(password), isAdmin(isAdmin) {} // we only need initializer list here

bool User::checkPassword(string& password) const {
    return this->password == password;
}

ostream& operator<<(ostream& os, const User& user) {
    os << user.username + "," + user.password + "," + (user.isAdmin ? "1" : "0");
    return os;
}

string User::getUsername() const {
    return username;
}

void UserManager::addUser(User* user) {
    users[user->getUsername()] = user; 
}

void UserManager::removeUser(const string& username) {
    auto it = users.find(username);
    if (it != users.end()) {
        delete it->second;
        users.erase(it);
    }
}

User * UserManager::findUser(const string& username) const {
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
void UserManager::saveUsers(const string& usersFile) const {
    ofstream file(usersFile);
    ofstream discountsFile("files/loyal_discounts.txt");
    if (!file.is_open()) {
        cerr << "Error opening users discount file." << endl;
        return;
    }
    bool first = true;
    bool first_customer = true;
    for (const auto& user : users) { // we need to save the users and their loyalty discounts
        if (!first) {
            file << endl;
        }
        file << *user.second;
        if (dynamic_cast<Customer*>(user.second)) { //dynamic cast to check if the user is a customer
            if (!first_customer) discountsFile << endl;
            discountsFile << user.first << " @ " << dynamic_cast<Customer*>(user.second)->getHasUsedLoyaltyDiscount(); 
            first_customer = false;
        }
        first = false;
        
    }

    file.close();
    discountsFile.close();
    
}
