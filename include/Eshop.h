#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "User.h"
#include "Customer.h"
#include "Admin.h"
#include "Product.h"
#include "Category.h"
#include "utils.h"
typedef std::map<std::string, User*> usersDatabase;
typedef std::map<std::string, Product*> productDatabase;

class Eshop {
protected:
    std::map<std::string, User*> users;
    ProductManager products;
    CategoryManager categories;
    const std::string& productsFile;
    const std::string& usersFile;
    User* login();
    User* registers();
    void loadUsers();
    void loadProducts();
    void loadCategories();
    void loadHistories();
    void saveChanges();
public:
    Eshop(const std::string& categoriesFile, const std::string& productsFile, const std::string& usersFile);
    ~Eshop();
    void run();
};
