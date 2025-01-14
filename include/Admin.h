#ifndef ADMIN_H
#define ADMIN_H

#include "../include/User.h"
#include "../include/Product.h"
#include <vector>

class Admin : public User {
public:
    Admin(std::string username, std::string password);
    void displayMenu();
    std::string userType();
    void executeCommand(int choice);
    
};

#endif