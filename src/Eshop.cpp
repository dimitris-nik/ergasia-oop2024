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
    std::string option;
    User* currentUser = nullptr;
    while (true) {
        std::cout << "Welcome to the e-shop!!!" << std::endl;
        std::cout << "Do you want to login or register? (enter option): ";
        std::cin >> option;
        
        if (option == "login") {

            std::string username, password;
            std::cout << "Please enter your username: ";
            std::cin >> username;
            std::cout << "Please enter your password: ";
            std::cin >> password;

            bool loggedIn = false;
            for (User* user : users) {
                if (user->checkCredentials(username, password)) {
                    std::cout << "Welcome " << username << "!" << std::endl;
                    currentUser = user;
                    loggedIn = true;
                    break;
                }
            }
            if (!loggedIn) {
                std::cout << "Invalid username or password." << std::endl;
                                continue;
            }
        } else if (option == "register") {

            std::string username, password;
            bool exists = false;
            while (!exists) {
                exists = true;
                std::cout << "Please enter your username: ";
                std::cin >> username;
                for(User* user : users){
                    if(user->checkCredentials(username)){
                        std::cout << "Username already exists. Please choose another." << std::endl;
                        exists = false;
                        break;
                    }
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
                users.push_back(admin);
                currentUser = admin;
                 
            } else {
                auto customer = new Customer(username, password);
                users.push_back(customer);
                currentUser = customer;                
            }
            std::cout << "Thanks for signing up! You are automatically logged-in as " << username << std::endl;
           
        }else {
             std::cout << "Invalid option. Please enter 'login' or 'register'." << std::endl;
            continue;
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
        break;
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

void Eshop::saveChanges() {
    int endl_flag = 0;
    std::ofstream usersFile(Eshop::usersFile);
    for (const User* user : users) {
        if (endl_flag == 0) {
            endl_flag = 1;
        }
        else { 
            usersFile << std::endl;
        }
        usersFile << *user; //overloaded operator<< in User class
    }
    usersFile.close();
    std::ofstream productsFile(Eshop::productsFile);
    endl_flag = 0;
    for (Product& product : products) {
        if (endl_flag == 0) { // no newline before first product
            endl_flag = 1;
        }
        else { 
            productsFile << std::endl;
        }
        productsFile << product; // overloaded operator<< in Product class
    }
    productsFile.close();
}