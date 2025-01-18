#pragma once
#include <unordered_map>
#include "Product.h"
#include "Category.h"

typedef struct stats {
    int consecutiveOrders = 0;
    int totalAmount = 0;
    int foundInLastCart = false;
    double discount = 1.0;
} productStats;

class DiscountStats {
    std::unordered_map<Product*, productStats> products_Stats;
    std::unordered_map<std::string, int> categoriesCounter;
    public:
        void updateProductStats(Product* product, int quantity);
        void nextCart();
        std::pair<Product*, double> getDiscount(CategoryManager& categories, int ordersCompleted, int hasUsedLoyaltyDiscount);
};