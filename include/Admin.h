#pragma once
#include "User.h"
#include "Product.h"
#include <vector>
#include "Category.h"
#include <unordered_map>

class Admin : public User {
    void addProduct(ProductManager& products, CategoryManager& categories);
    void editProduct(ProductManager& products, CategoryManager& categories);
    void removeProduct(ProductManager& products, CategoryManager& categories);
public:
    Admin(std::string username, std::string password);
    void displayMenu();
    bool executeCommand(ProductManager& products, CategoryManager& categories);
};
