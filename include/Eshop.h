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

class Eshop {
protected:
    std::map<std::string, User*> users;
    std::map<std::string, Product*> products;
    std::vector<Category*> categories;
    const std::string& productsFile;
    const std::string& usersFile;
    const std::string& categoriesFile;
    User* login();
    User* registers();
    void loadUsers();
    void loadProducts();
    void loadCategories();
    void saveChanges();
public:
    Eshop(const std::string& categoriesFile, const std::string& productsFile, const std::string& usersFile);
    ~Eshop();
    void run();
};
