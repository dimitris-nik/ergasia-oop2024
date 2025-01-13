#include "headers/user.h"
User::User(string username, string password, string userType) :
    username(username), password(password), userType(userType) {}
bool User::isAdmin() {
    if (userType == "admin") {
        return true;
    }
    return false;
}
bool User::authenticate(string password_attempt) {
    if (password == password_attempt) {
        return true;
    }
    return false;
}


