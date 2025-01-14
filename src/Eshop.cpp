#include "../include/Eshop.h"
#include "../include/utils.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <iomanip> // For std::fixed and std::setprecision


Eshop::Eshop(const std::string& categoriesFile, const std::string& productsFile, const std::string& usersFile) : categoriesFile(categoriesFile), productsFile(productsFile), usersFile(usersFile) {}

Eshop::~Eshop(){
    for(User* user : users){
            delete user;
    }
}

void Eshop::run(){
    loadUsers(usersFile);
    loadProducts(productsFile);
    for (User* user : users) {
        std::cout << *user << std::endl;
    }
    for (Product& product : products) {
        product.displayProduct();
    }
    
}

void Eshop::loadUsers(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening users file." << std::endl;
        return;
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string username, password, isAdminStr;

        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        std::getline(ss, isAdminStr, ',');
        bool isAdmin = (isAdminStr == "1");

        if(isAdmin){
            users.push_back(new Admin(username, password));
        } else {
            users.push_back(new Customer(username,password));
        }
    }
    file.close();
}

void Eshop::loadProducts(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening products file." << std::endl;
        return;
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string title, description, category, subcategory, priceStr, measurementType, amountStr;

        std::getline(ss, title, '@');
        std::getline(ss, description, '@');
        std::getline(ss, category, '@');
        std::getline(ss, subcategory, '@');
        std::getline(ss, priceStr, '@');
        std::getline(ss, measurementType, '@');
        std::getline(ss, amountStr, '@');
        title = trim(title);
        description = trim(description);
        category = trim(category);
        subcategory = trim(subcategory);
        measurementType = trim(measurementType);

        Product product(title, description, category, subcategory, std::stod(priceStr), measurementType, std::stoi(amountStr));
        products.push_back(product);
    }
    file.close();
}