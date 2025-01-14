#ifndef USER_H
#define USER_H
#include <string>

class User {
protected:
    std::string username;
    std::string password;
    bool isAdmin;
public:
    User(std::string username, std::string password, bool isAdmin);
    virtual ~User() = default;
    virtual void displayMenu() = 0; // Pure virtual function for menu display
    bool checkCredentials(std::string& username, std::string& password);
    bool checkCredentials(std::string& username); // overload the function to check only the username for registration
    friend std::ostream& operator<<(std::ostream& os, const User& user);
};

#endif