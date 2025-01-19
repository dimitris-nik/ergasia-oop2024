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
    // Overloaded operator << to make saving users to file easier
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
void UserManager::saveUsers(const string& usersFile, const string& loyalDiscounts) const {
    ofstream file(usersFile);
    ofstream discountsFile(loyalDiscounts);
    if (!file.is_open()) {
        cerr << "Error opening users file." << endl;
        return;
    }
    if (!discountsFile.is_open()) {
        cerr << "Error opening loyal discounts file." << endl;
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
// searchProduct is common for both Admin and Customer
void User::searchProduct(ProductManager& products, CategoryManager& categories){
    cout << "1. Search for a specific product (by title)." << endl;
    cout << "2. View the products of a specific category." << endl;
    cout << "3. Show all the available products." << endl;
    cout << "Enter your choice: ";
    int choice;
    choice = readInt();
    while (choice < 1 || choice > 3) {
        cout << "Invalid Option. Please enter a number between 1 and 3: ";
        choice = readInt();
    }
    switch(choice){
        case 1: {
            string titleSearch;
            cout << "Enter title to search: ";
            titleSearch = readString();
            if (products.findProduct(titleSearch)) {
                products.findProduct(titleSearch)->displayProduct();
            } else {
                cout << "Product not found." << endl;
            }
            break;
        }
        case 2: {
            string categorySearch;
            string subcategorySearch;
            cout << "Enter category to search: ";
            categorySearch = readString();
            auto searchCategory = categories.findCategory(categorySearch);
            while (!searchCategory) {
                cout << "Invalid Category. Please choose from the above." << endl;
                categorySearch = readString();
                searchCategory = categories.findCategory(categorySearch);
            }
            cout << "Enter subcategory to search (leave empty for all subcategories): ";
            subcategorySearch = readString();
            auto searchSubcategory = searchCategory->findSubcategory(subcategorySearch);
            while (!subcategorySearch.empty() && !searchSubcategory) {
                cout << "Invalid Subcategory. Please choose from the above, or leave empty." << endl;
                subcategorySearch = readString();
                searchSubcategory = searchCategory->findSubcategory(subcategorySearch);
            }
            if (subcategorySearch.empty()) {
                searchCategory->displayProducts();
            } else {
                searchSubcategory->displayProducts();
            }
            break;
        }
        case 3: {
            cout << "Results: ";
            products.displayProducts();
            cout << endl;
            cout << "Select a product title: ";
            string selectedTitle;
            selectedTitle = readString();
            while (!products.findProduct(selectedTitle)) {
                cout << "Invalid Product. Please choose from the above." << endl;
                selectedTitle = readString();
            }
            products.findProduct(selectedTitle)->displayProduct();
            break;
        }
        default: {
            cout << "Invalid Option." << endl;
            break;
        }
    }

}
