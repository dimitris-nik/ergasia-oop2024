#include "../include/DiscountStats.h"
#include <vector>
#include <map>

using namespace std;

void DiscountStats::updateProductStats(Product* product, int quantity) {
    categoriesCounter[product->getCategory()] += quantity; // logic is pretty straightforward
    products_Stats[product].appearedInCart += 1; // we need this for favorite product
    product->increaseAppearedInCart(); //we also need this for top products
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
            stats.consecutiveOrders = 0; // reset any consecutive orders if the product was not found in the last cart, 
        }                                // or if it was found 4 times in a row (it means the discount was applied in the 4th order)
    }
    for (auto& p : products_Stats) {
        p.second.foundInLastCart = false;
    }
    categoriesCounter.clear();
}

discount DiscountStats::getDiscount(CategoryManager& categories, int hasUsedLoyaltyDiscount) {
    vector<discount> discounts;
    for (const auto& p : products_Stats) {
        if (p.second.consecutiveOrders == 3) {
            discounts.push_back({p.first, 0.8}); // 20% discount for buying 3 times in a row
        }
    }
    for (const auto& c: categoriesCounter) {
        auto* productCategory = categories.findCategory(c.first);
        if (c.second >= productCategory->getAmountForDiscount()) {
            discounts.push_back({productCategory->generateRandomProduct(), 0.7}); // 30% discount for buying a lot of products from a category
        }
    }
    if (ordersCompleted >= 5 and !hasUsedLoyaltyDiscount) { // 40% discount for completing 5 orders
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
    return discounts[rand() % discounts.size()]; // return a random discount from the available ones

}

void DiscountStats::printDiscount(discount discount) {
    if (discount.product == nullptr) {
        return;
    }
    if (discount.multiplier == 0.8) {
        cout << "You bought " << discount.product->getTitle() << " 3 times in a row in your past orders, 20% discount on your next order!" << endl;
    } else if (discount.multiplier == 0.7) {
        cout << "You seem to like " << discount.product->getCategory() << " products, you have a 30% discount to " << discount.product->getTitle() << " on your next order!" << endl; 
    } else if (discount.multiplier == 0.6) {
        cout << "You have completed 5 orders, 40% discount to your favorite product, " << discount.product->getTitle() << "! Can only be used once!" << endl;
    }
}

