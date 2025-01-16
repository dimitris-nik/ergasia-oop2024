#include "../include/Eshop.h"
#include <iostream>
#include <sstream>
#include <iomanip> // For fixed and setprecision
using namespace std;

Eshop::Eshop(const string& categoriesFile, const string& productsFile, const string& usersFile) : categoriesFile(categoriesFile), productsFile(productsFile), usersFile(usersFile) {
    loadCategories();
    loadProducts();
    loadUsers();
}

Eshop::~Eshop() {
    users.saveUsers(usersFile);
    products.saveProducts(productsFile);
}

User* Eshop::login(){
    string username, password;
    cout << "Please enter your username: ";
    cin >> username;
    cout << "Please enter your password: ";
    cin >> password;
    User * user = users.findUser(username);
    while (user == nullptr || !user->checkPassword(password)) {
        cout << "Invalid username or password. Please try again." << endl;
        cout << "Please enter your username: ";
        cin >> username;
        cout << "Please enter your password: ";
        cin >> password;
        user = users.findUser(username);
    }
    return user;
}

User* Eshop::registers(){
    string username, password;
    bool exists = false;
    while (!exists) {
        exists = true;
        cout << "Please enter your username: ";
        cin >> username;
        if (users.findUser(username) != nullptr) {
            cout << "Username already exists. Please enter a different username." << endl;
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
        users.addUser(admin);
        return admin;
            
    } else {
        auto customer = new Customer(username, password, 0);
        users.addUser(customer);
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
    while (currentUser->executeCommand(products, categories)) {
        currentUser->displayMenu();
    }
    cout << "Goodbye!" << endl;
    saveChanges();
}



void Eshop::loadUsers() {
    ifstream file(usersFile);
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
            users.addUser(new Admin(username, password));
        } else {
            std::string historyFileName = "files/order_history/" + username + "_history.txt";
            std::ifstream historyFile(historyFileName);
            int orders = 0;
            if (historyFile.is_open()) {
                std::string line;
                while (std::getline(historyFile, line)) {
                    if (line.find("START---") != std::string::npos) {
                        orders++;
                    }
                }
                historyFile.close();
            }
            else{
                cerr << "Error opening history file." << endl;
            }
            users.addUser(new Customer(username, password, orders));
        }
    }
    file.close();
}

void Eshop::loadProducts() {
    ifstream file(productsFile);
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
        Product * product= new Product(title, description, category, subcategory, stod(priceStr), measurementType, stoi(amountStr));
        categories.addProduct(product, category, subcategory);
        products.addProduct(product);
    }
    file.close();
}

void Eshop::loadCategories() {
    std::ifstream file(categoriesFile);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening category file." << std::endl;
        return;
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string categoryName;
        std::string subcategoriesLine;

        std::getline(ss, categoryName, '(');
        categoryName = trim(categoryName);

        Category* category = categories.addCategory(categoryName);

        if (std::getline(ss, subcategoriesLine, ')')) {
            std::stringstream subcats(subcategoriesLine);
            std::string subcategory;
            while (std::getline(subcats, subcategory, '@')) {
                category->addSubcategory(trim(subcategory));
            }
        }
    }
    file.close();
}

void Eshop::saveChanges() {
    products.saveProducts(productsFile);
    users.saveUsers(usersFile);
}