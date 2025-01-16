#pragma once
#include <string>
#include <map>
#include <vector>
#include "Product.h"
#include "Category.h"

class User {
protected:
    std::string username;
    std::string password;
    bool isAdmin;
public:
    User(std::string username, std::string password, bool isAdmin);
    virtual ~User() = default;
    std::string getUsername() const;
    virtual void displayMenu() = 0; // Pure virtual function for menu display
    virtual bool executeCommand(ProductManager& products, CategoryManager& categories) = 0; // Pure virtual function to execute user choice
    bool checkPassword(std::string& password) const;
    friend std::ostream& operator<<(std::ostream& os, const User& user);
};

class UserManager {
    std::unordered_map<std::string, User*> users;
public:
    ~UserManager();
    void addUser(User* user);
    void removeUser(const std::string& username);
    User* findUser(const std::string& username) const;
    void saveUsers(const std::string& usersFile) const;
};
