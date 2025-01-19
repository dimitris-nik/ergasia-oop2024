#include "../include/Eshop.h"
#include <iostream>
#include <sstream>
#include <iomanip> // For fixed and setprecision
#include <limits> 
#include <array>
using namespace std;

Eshop::Eshop(const string& categoriesFile, const string& productsFile, const string& usersFile, const string& discountsFile, const string& loyalDiscountsFile) : categoriesFile(categoriesFile), productsFile(productsFile), usersFile(usersFile), discountsFile(discountsFile), loyalDiscountsFile(loyalDiscountsFile) {
    loadCategories();
    loadProducts();
    loadUsers();
    loadLoyalDiscounts();
}

Eshop::~Eshop() {
    saveChanges();
}

User* Eshop::login(){
    string username, password;
    cout << "Please enter your username: ";
    username = readString();
    cout << "Please enter your password: ";
    password = readString();
    User * user = users.findUser(username);
    while (user == nullptr || !user->checkPassword(password)) {
        cout << "Invalid username or password. Please try again." << endl;
        cout << "Please enter your username: ";
        username = readString();
        cout << "Please enter your password: ";
        password = readString();
        user = users.findUser(username);
    }
    return user;
}

User* Eshop::registers(){
    string username;
    bool exists = false;
    while (!exists) {
        exists = true;
        cout << "Please enter your username: ";
        username = readString();
        if (users.findUser(username) != nullptr) {
            cout << "Username already exists. Please enter a different username: ";
            username = readString();
            exists = false;
        }
    } 
    cout << "Please enter your password: ";
    string password = readString();
    cout << "Are you an admin user? (Y/N): ";
    string isAdminInput = readString();
    bool isAdmin = (isAdminInput == "Y" || isAdminInput == "y");

    if (isAdmin) {
        auto admin = new Admin(username, password);
        users.addUser(admin);
        return admin;
            
    } else {
        auto discountStats = DiscountStats();
        auto customer = new Customer(username, password, discountStats);
        // create history file on signup
        ofstream historyFile("files/order_history/" + username + "_history.txt");
        if (!historyFile.is_open()) {
            cerr << "Error creating history file." << endl;   
            return nullptr;
        }
        users.addUser(customer);
        return customer;
    }
}

void Eshop::run(){
    string option;
    User* currentUser = nullptr;
    cout << "Welcome to the e-shop!!!" << endl;
    cout << "Do you want to login or register? (enter option): ";
    option = readString();
    while (option != "login" && option != "register") {
        cout << "Invalid option. Please enter 'login' or 'register':";
        option = readString();
    }
    
    //call register or login function that will return the user
    if (option == "login") {
        currentUser = login();
    } else if (option == "register") {
        currentUser = registers();
    }
    if (currentUser == nullptr) {
        cout << "Error logging in or registering. Exiting..." << endl;
        return;
    }
    else {
        cout << endl;
        cout << "Welcome, " << currentUser->getUsername() << "!" << endl;
    }
    currentUser->displayMenu();
    // Execute command returns false when the user wants to exit
    // displayMenu() function is overloaded in Admin and Customer classes
    while (currentUser->executeCommand(products, categories)) {
        currentUser->displayMenu();
    }
    cout << "Goodbye!" << endl;
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
        // Read the line and split it by commas
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, isAdminStr, ',');
        bool isAdmin = (isAdminStr == "1");
        if(isAdmin){
            users.addUser(new Admin(username, password));
        } else {
            // If the user is a customer, we need to load their order history in order to calculate potential discounts
            string historyFileName = "files/order_history/" + username + "_history.txt";
            ifstream historyFile(historyFileName);
            auto discountStats = DiscountStats();
            if (historyFile.is_open()) {
                string line;
                while (getline(historyFile, line)) {
                    // Check if the line is the start of a new cart
                    if (line.find("START---") != string::npos) {
                        discountStats.nextCart();
                        discountStats.incrementOrdersCompleted();
                    }
                    // Check if the line is the end of a cart
                    else if (!((line.find("END---") != string::npos) or (line.find("Total Cost")!= string::npos)) && line != ""){
                        int quantity;
                        string title;
                        stringstream ss(line);
                        ss >> quantity;
                        getline(ss, title);
                        title = trim(title);
                        auto product = products.findProduct(title);
                        // Check if the product exists
                        if (!product) continue; 
                        discountStats.updateProductStats(product, quantity);
                    }
                }
                historyFile.close();
            }
            else{
                cerr << "Error opening history file." << endl;
            }
            // Create new customer with the loaded discountStats object
            users.addUser(new Customer(username, password, discountStats));
        }
    }
    file.close();
}

void Eshop::loadLoyalDiscounts(){
    ifstream loyalDiscountsfile(loyalDiscountsFile);   
    string line;
    while (getline(loyalDiscountsfile, line)) {
        stringstream ss(line);
        string username, discountStr;
        // Read the line and split it by @
        getline(ss, username, '@');
        getline(ss, discountStr);
        User* user = users.findUser(trim(username));
        if (user) {
            reinterpret_cast<Customer*>(user)->setHasUsedLoyaltyDiscount(stoi(discountStr));
            reinterpret_cast<Customer*>(user)->updateCurrentDiscount(categories);
        }
    }
    loyalDiscountsfile.close();
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
        // Read the line and split it by @
        getline(ss, title, '@');
        getline(ss, description, '@');
        getline(ss, category, '@');
        getline(ss, subcategory, '@');
        getline(ss, priceStr, '@');
        getline(ss, measurementType, '@');
        getline(ss, amountStr, '@');
        // trim() function to remove leading and trailing whitespaces
        title = trim(title);
        description = trim(description);
        category = trim(category);
        subcategory = trim(subcategory);
        measurementType = trim(measurementType);
        // Product object is created here and deleted in the destructor of ProductManager or when the product is removed
        Product * product= new Product(title, description, category, subcategory, stod(priceStr), measurementType, stoi(amountStr));
        categories.addProduct(product, category, subcategory);
        products.addProduct(product);
    }
    file.close();
}

void Eshop::loadCategories() {
    ifstream file(categoriesFile);
    string line;

    if (!file.is_open()) {
        cerr << "Error opening category file." << endl;
        return;
    }

    ifstream dFile(discountsFile);
    if (!dFile.is_open()) {
        cerr << "Error opening discounts file." << endl;
        return;
    }
    while (getline(file, line)) {
        // Create category and then add its subcategories
        stringstream ss(line);
        string categoryName;
        string subcategoriesLine;

        getline(ss, categoryName, '(');
        categoryName = trim(categoryName);

        Category* category = new Category(categoryName); //this is created here and deleted in the destructor of CategoryManager

        if (getline(ss, subcategoriesLine, ')')) {
            stringstream subcats(subcategoriesLine);
            string subcategory;
            while (getline(subcats, subcategory, '@')) {
                category->addSubcategory(trim(subcategory));
            }
        }
        categories.addCategory(category);
        
    }
    while (getline(dFile, line)) {
        // Read the amount of product a customer needs to buy to get a discount
        stringstream ss(line);
        string categoryName;
        string amountStr;
        getline(ss, categoryName, '@');
        getline(ss, amountStr, '@');
        Category* category = categories.findCategory(trim(categoryName));
        if (category) {
            category->setAmountForDiscount(stoi(amountStr));
        }
        else {
            cerr << "Category in discounts.txt not found, please check files." << endl;
        }
    }
    file.close();
}

void Eshop::saveChanges() {
    products.saveProducts(productsFile);
    users.saveUsers(usersFile, loyalDiscountsFile);
}