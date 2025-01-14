#pragma once
#include "../include/User.h"
#include "../include/Product.h"
#include <vector>

class Admin : public User {
public:
    Admin(std::string username, std::string password);
    void displayMenu();
    bool executeCommand(int choice);

};
