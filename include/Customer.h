#pragma once
#include "User.h"
#include "Product.h"
#include "Cart.h"
#include "utils.h"
#include "DiscountStats.h"
#include <vector>
#include <string>

class Customer : public User {
    Cart cart;
    DiscountStats discountStats;
    bool hasUsedLoyaltyDiscount = false;
    discount currDiscount = {nullptr, 1.0};
    void addProductToCart(ProductManager& products);
    void updateProductInCart(ProductManager& products);
    void removeProductFromCart(ProductManager& products);
    void completeOrder(CategoryManager& categories);
    void viewOrderHistory();
    void viewCart();
public:
    Customer(const std::string& username, const std::string& password, DiscountStats productStats);
    void displayMenu();
    bool executeCommand(ProductManager& products, CategoryManager& categories);
    void updateCurrentDiscount(CategoryManager& categories);
    bool getHasUsedLoyaltyDiscount() const;
    void setHasUsedLoyaltyDiscount(bool hasUsedLoyaltyDiscount);
};
