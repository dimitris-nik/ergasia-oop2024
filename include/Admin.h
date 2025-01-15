#pragma once
#include "User.h"
#include "Product.h"
#include <vector>
#include "Category.h"
#include <map>

class Admin : public User {
public:
    Admin(std::string username, std::string password);
    void displayMenu();
    bool executeCommand(std::map<std::string, Product*>& products, std::vector<Category*>& categories);
    void addProduct(std::map<std::string, Product*> products, std::vector<Category*>& categories);
    void editProduct(std::map<std::string, Product*> products, std::vector<Category*>& categories);
    void removeProduct(std::map<std::string, Product*> products, std::vector<Category*>& categories);
    void searchProduct(std::map<std::string, Product*> products);
    void showUnavailableProducts(std::map<std::string, Product*> products);
    void showTopProducts(std::map<std::string, Product*> products);
    
    
};
