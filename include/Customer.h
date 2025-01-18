#pragma once
#include "User.h"
#include "Product.h"
#include "Cart.h"
#include "DiscountStats.h"
#include <vector>
#include <string>
class Customer : public User {
    Cart cart;
    DiscountStats discountStats;
    bool hasUsedLoyaltyDiscount = false;
    discount currDiscount;
    void updateProductStats();
public:
    Customer(const std::string& username, const std::string& password, int ordersCompleted, DiscountStats productStats);
    void displayMenu();
    bool executeCommand(ProductManager& products, CategoryManager& categories);
    void searchProduct(ProductManager products, CategoryManager& categories);
    void addProductToCart(ProductManager& products);
    void updateProductInCart(ProductManager& products);
    void removeProductFromCart(ProductManager& products);
    void completeOrder(CategoryManager& categories);
    void viewOrderHistory();
    void viewCart();
    bool getHasUsedLoyaltyDiscount() const;
    void setHasUsedLoyaltyDiscount(bool hasUsedLoyaltyDiscount);
};
