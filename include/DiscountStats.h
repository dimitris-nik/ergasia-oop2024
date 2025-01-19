#pragma once
#include <unordered_map>
#include "Product.h"
#include "Category.h"

typedef struct stats {
    int consecutiveOrders = 0; // max amount of times the product was found in the cart in a row
    int appearedInCart = 0; // total times the product was found in the cart (regardless of the amount)
    int totalAmount = 0; // total amount of the product found in the cart
    int foundInLastCart = false; // if the product was found in the last cart
} productStats;

typedef struct discount {
    Product* product;
    double multiplier;
} discount;

class DiscountStats {
    std::unordered_map<Product*, productStats> products_Stats;
    std::unordered_map<std::string, int> categoriesCounter;
    int ordersCompleted = 0;
    public: 
        void updateProductStats(Product* product, int quantity);
        void nextCart();
        discount getDiscount(CategoryManager& categories, int hasUsedLoyaltyDiscount);
        void incrementOrdersCompleted();
        int getOrdersCompleted() const;
        void printDiscount(discount discount);
};