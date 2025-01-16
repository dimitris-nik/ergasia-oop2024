#pragma once
#include "User.h"
#include "Product.h"
#include "Cart.h"
#include <vector>
#include <string>
class Customer : public User {
    Cart cart;
    int ordersCompleted;
public:
    Customer(const std::string& username, const std::string& password, int ordersCompleted);
    void displayMenu();
    bool executeCommand(ProductManager& products, CategoryManager& categories);
    void searchProduct(ProductManager products, CategoryManager& categories);
    void addProductToCart(ProductManager& products);
    void updateProductInCart(ProductManager& products);
    void removeProductFromCart(ProductManager& products);
    void completeOrder();
    void viewOrderHistory();
    void viewCart();
    
};
