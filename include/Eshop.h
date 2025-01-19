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

class Eshop {
protected:
    UserManager users;
    ProductManager products;
    CategoryManager categories;
    const std::string& categoriesFile;
    const std::string& productsFile;
    const std::string& usersFile;
    const std::string& discountsFile;
    User* login();
    User* registers();
    void loadUsers();
    void loadProducts();
    void loadCategories();
    void loadHistories();
    void saveChanges();
public:
    Eshop(const std::string& categoriesFile = "files/categories.txt", const std::string& productsFile = "files/products.txt", const std::string& usersFile = "files/users.txt", const std::string& discountsFile = "files/discounts.txt");
    ~Eshop();
    void run();
};
