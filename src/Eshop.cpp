#include "../include/Eshop.h"
#include "../include/utils.h"
#include <iostream>
#include <sstream>
#include <iomanip> // For std::fixed and std::setprecision


Eshop::Eshop(const std::string& categoriesFile, const std::string& productsFile, const std::string& usersFile) : categoriesFile(categoriesFile), productsFile(productsFile), usersFile(usersFile) {}

Eshop::~Eshop(){
    for(auto user : users){
        delete user.second;
    }   
}

void Eshop::run(){
    loadUsers(usersFile);
    loadProducts(productsFile);
    std::string option;
    User* currentUser = nullptr;
    std::cout << "Welcome to the e-shop!!!" << std::endl;
    std::cout << "Do you want to login or register? (enter option): ";
    std::cin >> option;
    while (option != "login" && option != "register") {
        std::cout << "Invalid option. Please enter 'login' or 'register'." << std::endl;
        std::cin >> option;
    }
    
    if (option == "login") {
        bool loggedIn = false;
        

        
        while (!loggedIn){
            std::string username, password;
            std::cout << "Please enter your username: ";
            std::cin >> username;
            std::cout << "Please enter your password: ";
            std::cin >> password;
            if (users.find(username) != users.end() and users[username]->checkPassword(password)) {
                std::cout << "Welcome " << username << "!" << std::endl;
                currentUser = users[username];
                loggedIn = true;
            }
            else {
                std::cout << "Invalid username or password. Try again" << std::endl;
            }
        }
        
    } else if (option == "register") {

        std::string username, password;
        bool exists = false;
        while (!exists) {
            exists = true;
            std::cout << "Please enter your username: ";
            std::cin >> username;
            if (users.find(username) != users.end()) {
                std::cout << "Username already exists. Please choose another." << std::endl;
                exists = false;
            }
        } 
        std::cout << "Please enter your password: ";
        std::cin >> password;
        std::string isAdminInput;
        std::cout << "Are you an admin user? (Y/N): ";
        std::cin >> isAdminInput;
        bool isAdmin = (isAdminInput == "Y" || isAdminInput == "y");

        if (isAdmin) {
            auto admin = new Admin(username, password);
            users[username] = admin;
            currentUser = admin;
                
        } else {
            auto customer = new Customer(username, password);
            users[username] = customer;
            currentUser = customer;
        }
        std::cout << "Thanks for signing up! You are automatically logged-in as " << username << std::endl;
        
    }

    while (true) {
        currentUser->displayMenu();
        int choice;
        std::cin >> choice;
        while (choice < 1 || choice > 8) {
            std::cout << "Invalid choice. Please enter a number between 1 and 8." << std::endl;
            std::cin >> choice;
        }
        if (choice == 7) {
            break;
        }
        else{
            currentUser->executeCommand(choice);
        }

    }
    std::cout << "Goodbye!" << std::endl;
    saveChanges();
    
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
            users[username] = new Admin(username, password);
        } else {
            users[username] = new Customer(username, password);
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
        products[title] = &product;
    }
    file.close();
}

void Eshop::saveChanges() {
    int endl_flag = 0;
    std::ofstream usersFile(Eshop::usersFile);
    for (auto user: users) {
        if (endl_flag == 0) {
            endl_flag = 1;
        }
        else { 
            usersFile << std::endl;
        }
        usersFile << *(user.second); //overloaded operator<< in User class
    }
    usersFile.close();
    std::ofstream productsFile(Eshop::productsFile);
    endl_flag = 0;
    for (auto product : products) {
        if (endl_flag == 0) { // no newline before first product
            endl_flag = 1;
        }
        else { 
            productsFile << std::endl;
        }
        productsFile << product.second; // overloaded operator<< in Product class
    }
    productsFile.close();
}