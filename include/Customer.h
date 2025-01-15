#pragma once
#include "User.h"
#include "Product.h"
#include "Cart.h"
#include <vector>
#include <string>
class Customer : public User {
    Cart cart;
    std::string historyFileName;
public:
    Customer(std::string username, std::string password);
    void displayMenu();
    bool executeCommand(std::map<std::string, Product*>& products, CategoryManager& categories);
    void searchProduct(std::map<std::string, Product*> products, CategoryManager& categories);
    void addProductToCart(std::map<std::string, Product*>& products);
    void updateProductInCart(std::map<std::string, Product*>& products);
    void removeProductFromCart(std::map<std::string, Product*>& products);
    void completeOrder();
    void viewOrderHistory();
    void viewCart();
    
};
