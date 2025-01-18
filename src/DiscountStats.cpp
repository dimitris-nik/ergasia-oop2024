#include "../include/DiscountStats.h"
#include <vector>
#include <map>

void DiscountStats::updateProductStats(Product* product, int quantity) {
    categoriesCounter[product->getCategory()] += quantity;
    if (products_Stats.find(product) == products_Stats.end()) {
        products_Stats[product].consecutiveOrders = 1;
        products_Stats[product].totalAmount = quantity;
        products_Stats[product].foundInLastCart = true;
        product->increaseAppearedInCart();
    } else {
        if (!products_Stats[product].foundInLastCart) {
            product->increaseAppearedInCart();
            products_Stats[product].foundInLastCart = true;
        }
        products_Stats[product].consecutiveOrders++;
        products_Stats[product].totalAmount += quantity;
    }
}

void DiscountStats::nextCart() {
    for (auto& p : products_Stats) {
        auto& stats = p.second;
        if (!stats.foundInLastCart or stats.consecutiveOrders == 4) {
            stats.consecutiveOrders = 0;
        }
    }
    for (auto& p : products_Stats) {
        p.second.foundInLastCart = false;
    }
    categoriesCounter.clear();
}

std::pair<Product*, double> DiscountStats::getDiscount(CategoryManager& categories, int ordersCompleted, int hasUsedLoyaltyDiscount) {
    std::vector<std::pair<Product*, double>> discounts;
    for (const auto& p : products_Stats) {
        if (p.second.consecutiveOrders == 3) {
            discounts.push_back({p.first, 0.8});
        }
    }
    for (const auto& c: categoriesCounter) {
        auto* productCategory = categories.findCategory(c.first);
        if (c.second >= productCategory->getAmountForDiscount()) {
            discounts.push_back({productCategory->generateRandomProduct(), 0.7});
        }
    }
    if (ordersCompleted >= 5) {
        Product* favoriteProduct = nullptr;
        int maxAmount = 0;
        for (const auto& p : products_Stats) {
            if (p.second.totalAmount > maxAmount) {
                maxAmount = p.second.totalAmount;
                favoriteProduct = p.first;
            }
        }
        discounts.push_back({favoriteProduct, 0.6});
    }
    if (discounts.empty()) {
        return {nullptr, 1.0};
    }
    return discounts[rand() % discounts.size()];

}


