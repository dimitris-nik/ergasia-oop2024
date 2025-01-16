#include "../include/Eshop.h"
#include <iostream>
#include <sstream>
#include <iomanip> // For fixed and setprecision
#include <limits> 
#include <array>
using namespace std;

Eshop::Eshop(const string& categoriesFile, const string& productsFile, const string& usersFile, const string& discountsFile) : categoriesFile(categoriesFile), productsFile(productsFile), usersFile(usersFile), discountsFile(discountsFile) {
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
            std::unordered_map<std::string , std::array<int, 3>> productStats; // Product, <consecutive orders, total amount, found in last cart>
            if (historyFile.is_open()) {
                std::string line;
                while (std::getline(historyFile, line)) {
                    if (line.find("START---") != std::string::npos) {
                        orders++;
                    }
                    else if (!((line.find("END---") != std::string::npos) or (line.find("Total Cost")!= std::string::npos)) && line!="" ){
                        int quantity;
                        std::string title;
                        std::stringstream ss(line);
                        ss >> quantity;
                        getline(ss, title);
                        title = trim(title);
                        if (productStats.find(title) == productStats.end()) {
                            productStats[title] = {1, quantity, true};
                            Product*  prod = products.findProduct(title);
                            if (prod != nullptr) {
                                prod->increaseAppearedInCart();
                            }
                        } else {
                            // Check if product was found in current cart
                            if (!productStats[title][2]) {
                                Product*  prod = products.findProduct(title);
                                if (prod != nullptr) {
                                    prod->increaseAppearedInCart();
                                }
                                productStats[title][2] = true;
                            }
                            productStats[title][0]++;
                            productStats[title][1] += quantity;
                        }
                    }
                    else if (line.find("END---")!= std::string::npos){
                        // Reset consecutive orders for false products or products with 4 consecutive orders (The 4th order is the one that the discount was applied)
                        for (auto& p : productStats) {
                            auto& stats = p.second;
                            if (!stats[2] or stats[0] == 4) {
                                stats[0] = 0;
                            }
                        }
                        // Set all products to false
                        for (auto& p : productStats) {
                            p.second[2] = false;
                        }
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

    std::ifstream dFile(discountsFile);
    if (!dFile.is_open()) {
        std::cerr << "Error opening discounts file." << std::endl;
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
        // Read discount
        std::string discountLine;
        std::getline(dFile, discountLine);
        std::stringstream discountStream(discountLine);
        std::string discountStr;
        std::getline(discountStream, discountStr, '@');
        // Ignore category name
        std::getline(discountStream, discountStr);
        int discount = std::stoi(discountStr);
        category->setAmountForDiscount(discount);
    }
    file.close();
}

void Eshop::saveChanges() {
    products.saveProducts(productsFile);
    users.saveUsers(usersFile);
}