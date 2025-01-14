#include "../include/Eshop.h"
#include "../include/utils.h"
#include <iostream>
#include <sstream>
#include <iomanip> // For fixed and setprecision
using namespace std;

Eshop::Eshop(const string& categoriesFile, const string& productsFile, const string& usersFile) : categoriesFile(categoriesFile), productsFile(productsFile), usersFile(usersFile) {
    loadUsers(usersFile);
    loadProducts(productsFile);
}

Eshop::~Eshop(){
    for(auto user : users){
        delete user.second;
    }   
    for(auto product : products){
        delete product.second;
    }   
}

User* Eshop::login(){
    string username, password;
    cout << "Please enter your username: ";
    cin >> username;
    cout << "Please enter your password: ";
    cin >> password;
    if (users.find(username) != users.end() and users[username]->checkPassword(password)) {
        cout << "Welcome " << username << "!" << endl;
        return users[username];
    }
    else {
        cout << "Invalid username or password. Try again" << endl;
        return login();
    }
}

User* Eshop::registers(){
    string username, password;
    bool exists = false;
    while (!exists) {
        exists = true;
        cout << "Please enter your username: ";
        cin >> username;
        if (users.find(username) != users.end()) {
            cout << "Username already exists. Please choose another." << endl;
            exists = false;
        }
    } 
    cout << "Please enter your password: ";
    cin >> password;
    string isAdminInput;
    cout << "Are you an admin user? (Y/N): ";
    cin >> isAdminInput;
    bool isAdmin = (isAdminInput == "Y" || isAdminInput == "y");

    if (isAdmin) {
        auto admin = new Admin(username, password);
        users[username] = admin;
        return admin;
            
    } else {
        auto customer = new Customer(username, password);
        users[username] = customer;
        return customer;
    }
}

void Eshop::run(){
    string option;
    User* currentUser = nullptr;
    cout << "Welcome to the e-shop!!!" << endl;
    cout << "Do you want to login or register? (enter option): ";
    bool loggedIn = false;
    cin >> option;
    while (option != "login" && option != "register") {
        cout << "Invalid option. Please enter 'login' or 'register'." << endl;
        cin >> option;
    }
    
    if (option == "login") {
        currentUser = login();
    } else if (option == "register") {
        currentUser = registers();
    }
    currentUser->displayMenu();
    while (currentUser->executeCommand()) {
        currentUser->displayMenu();
    }
    cout << "Goodbye!" << endl;
    saveChanges();
    
}

void Eshop::loadUsers(const string& filename) {
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "Error opening users file." << endl;
        return;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string username, password, isAdminStr;
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, isAdminStr, ',');
        bool isAdmin = (isAdminStr == "1");
        if(isAdmin){
            users[username] = new Admin(username, password);
        } else {
            users[username] = new Customer(username, password);
        }
    }
    file.close();
}

void Eshop::loadProducts(const string& filename) {
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "Error opening products file." << endl;
        return;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string title, description, category, subcategory, priceStr, measurementType, amountStr;
        getline(ss, title, '@');
        getline(ss, description, '@');
        getline(ss, category, '@');
        getline(ss, subcategory, '@');
        getline(ss, priceStr, '@');
        getline(ss, measurementType, '@');
        getline(ss, amountStr, '@');
        title = trim(title);
        description = trim(description);
        category = trim(category);
        subcategory = trim(subcategory);
        measurementType = trim(measurementType);
        Product product(title, description, category, subcategory, stod(priceStr), measurementType, stoi(amountStr));
        products[title] = &product;
    }
    file.close();
}

void Eshop::saveChanges() {
    int endl_flag = 0;
    ofstream usersFile(Eshop::usersFile);
    for (auto user: users) {
        if (endl_flag == 0) {
            endl_flag = 1;
        }
        else { 
            usersFile << endl;
        }
        usersFile << *(user.second); //overloaded operator<< in User class
    }
    usersFile.close();
    ofstream productsFile(Eshop::productsFile);
    endl_flag = 0;
    for (auto product : products) {
        if (endl_flag == 0) { // no newline before first product
            endl_flag = 1;
        }
        else { 
            productsFile << endl;
        }
        productsFile << *(product.second); // overloaded operator<< in Product class
    }
    productsFile.close();
}