#include "../include/DiscountStats.h"
#include <vector>
#include <map>

void DiscountStats::updateProductStats(Product* product, int quantity) {
    categoriesCounter[product->getCategory()] += quantity;
    products_Stats[product].appearedInCart += 1;
    product->increaseAppearedInCart();
    if (products_Stats.find(product) == products_Stats.end()) {
        products_Stats[product].consecutiveOrders = 1;
        products_Stats[product].totalAmount = quantity;
        products_Stats[product].foundInLastCart = true;
    } else {
        if (!products_Stats[product].foundInLastCart) {
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

discount DiscountStats::getDiscount(CategoryManager& categories, int hasUsedLoyaltyDiscount) {
    std::vector<discount> discounts;
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
    if (ordersCompleted >= 5 and !hasUsedLoyaltyDiscount) {
        Product* favoriteProduct = nullptr;
        int maxAmount = 0;
        for (const auto& p : products_Stats) {
            if (p.second.appearedInCart > maxAmount) {
                maxAmount = p.second.appearedInCart;
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

void DiscountStats::printDiscount(discount discount) {
    if (discount.product == nullptr) {
        return;
    }
    if (discount.multiplier == 0.8) {
        std::cout << "You bought " << discount.product->getTitle() << " 3 times in a row in your past orders, 20% discount on your next order!" << std::endl;
    } else if (discount.multiplier == 0.7) {
        std::cout << "You seem to like " << discount.product->getCategory() << " products, you have a 30% discount to " << discount.product->getTitle() << " on your next order!" << std::endl; 
    } else if (discount.multiplier == 0.6) {
        std::cout << "You have completed 5 orders, 40% discount to your favorite product, " << discount.product->getTitle() << "! Can only be used once!" << std::endl;
    }
}

