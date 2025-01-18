#pragma once
#include <unordered_map>
#include "Product.h"
#include "Category.h"

typedef struct stats {
    int consecutiveOrders = 0;
    int appearedInCart = 0;
    int totalAmount = 0;
    int foundInLastCart = false;
    double discount = 1.0;
} productStats;
typedef std::pair<Product*, double> discount;

class DiscountStats {
    std::unordered_map<Product*, productStats> products_Stats;
    std::unordered_map<std::string, int> categoriesCounter;
    public: 
        int ordersCompleted;
        void updateProductStats(Product* product, int quantity);
        void nextCart();
        discount getDiscount(CategoryManager& categories, int hasUsedLoyaltyDiscount);
        void setOrdersCompleted(int ordersCompleted);
        int getOrdersCompleted() const;
};