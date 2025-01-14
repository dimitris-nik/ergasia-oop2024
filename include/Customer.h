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
    void displayMenu() override;
    void searchProduct(const std::vector<Product>& products);
    void addProductToCart(std::vector<Product>& products);
    void updateProductInCart(std::vector<Product>& products);
     void removeProductFromCart();
    void completeOrder(std::vector<Product>& products);
    void viewOrderHistory();
    void viewCart();
    
};

#endif