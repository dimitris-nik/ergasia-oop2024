#ifndef CUSTOMER_H
#define CUSTOMER_H

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
    std::string userType();
    void executeCommand(int choice);
    void searchProduct();
    void addProductToCart();
    void updateProductInCart();
    void removeProductFromCart();
    void completeOrder();
    void viewOrderHistory();
    void viewCart();
};

#endif