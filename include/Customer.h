#pragma once
#include "User.h"
#include "Product.h"
#include "Cart.h"
#include <vector>
#include <string>
class Customer : public User {
public:
    Cart cart;
    std::string historyFileName;
    Customer(std::string username, std::string password);
    void displayMenu();
    bool executeCommand(std::map<std::string, Product*>& products, CategoryManager& categories);
    void searchProduct(std::map<std::string, Product*> products);
    void addProductToCart(std::map<std::string, Product*>& products);
    void updateProductInCart(std::map<std::string, Product*>& products);
    void removeProductFromCart();
    void completeOrder(std::map<std::string, Product*>& products);
    void viewOrderHistory();
    void viewCart();
    
};
