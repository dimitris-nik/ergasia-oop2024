#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "User.h"
#include "Customer.h"
#include "Admin.h"
#include "Product.h"
#include "Category.h"
#include "utils.h"

class Eshop {
protected:
    UserManager users;
    ProductManager products;
    CategoryManager categories;
    const std::string& categoriesFile;
    const std::string& productsFile;
    const std::string& usersFile;
    const std::string& discountsFile;
    const std::string& loyalDiscountsFile;
    User* login();
    User* registers();
    void loadUsers();
    void loadProducts();
    void loadCategories();
    void loadHistories();
    void loadLoyalDiscounts();
    void saveChanges();
public:
    Eshop(const std::string& categoriesFile, const std::string& productsFile, const std::string& usersFile, const std::string& discountsFile, const std::string& loyalDiscountsFile);
    ~Eshop();
    void run();
};
