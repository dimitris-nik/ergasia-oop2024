#ifndef ESHOP_H
#define ESHOP_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "User.h"
#include "Customer.h"
#include "Admin.h"
#include "Product.h"

class Eshop {
protected:
    std::map<std::string, User*> users;
    std::map<std::string, Product*> products;

    const std::string& productsFile;
    const std::string& usersFile;
    const std::string& categoriesFile;
    void loadUsers(const std::string& filename);
    void loadProducts(const std::string& filename);
    void saveChanges();
public:
    Eshop(const std::string& categoriesFile, const std::string& productsFile, const std::string& usersFile);
    ~Eshop();
    void run();
};

#endif