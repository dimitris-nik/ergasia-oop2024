#include "../include/Cart.h"
#include <iostream>
#include <iomanip>

void Cart::addProduct(Product& product, int quantity) {
    items[product] += quantity;
}
void Cart::removeProduct(std::string& title){
     for (auto it = items.begin(); it != items.end(); ) {
        if (it->first.title == title) {
            it = items.erase(it);
         } else {
             ++it;
        }
    }
}
void Cart::updateProduct(Product& product, int quantity){
        items[product] = quantity;
}
void Cart::displayCart() {
    std::cout << std::endl;
    std::cout << "---CART START---" << std::endl;
     double totalCost = 0;
    for (const auto& item : items) {
        std::cout << item.second << " " << item.first.title << std::endl;
         totalCost += item.first.price * item.second;
    }
    std::cout << "---CART END---" << std::endl;
     std::cout << "Total Cost: " << std::fixed << std::setprecision(2) << totalCost << std::endl << std::endl;
}