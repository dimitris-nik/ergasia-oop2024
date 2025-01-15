#include "../include/Cart.h"
#include <iostream>
#include <iomanip>

void Cart::addProduct(Product& product, int quantity) {
    items[product] += quantity;
}
void Cart::removeProduct(Product& product){
    if (items.find(product) != items.end()) {
        items.erase(product);
        std::cout << "Product removed!" << std::endl;
    }
    else {
        std::cout << "Product not found in cart!" << std::endl;
    }
}
void Cart::updateProduct(Product& product, int quantity){
    items[product] = quantity;
}

void Cart::clearCart() {
    items.clear();
}
/*
std::cout << std::endl;
    std::cout << "---CART START---" << std::endl;
     double totalCost = 0;
    for (const auto& item : items) {
        std::cout << item.second << " " << item.first.title << std::endl;
         totalCost += item.first.price * item.second;
    }
    std::cout << "---CART END---" << std::endl;
    std::cout << "Total Cost: " << std::fixed << std::setprecision(2) << totalCost << std::endl << std::endl;
*/
std::ostream& operator<<(std::ostream& os, const Cart& cart) {
    os << "---CART START---" << std::endl;
    double totalCost = 0;
    for (const auto& item : cart.items) {
        os << item.second << " " << item.first.title << std::endl;
        totalCost += item.first.price * item.second;
    }
    os << "---CART END---" << std::endl;
    os << "Total Cost: " << std::fixed << std::setprecision(2) << totalCost << std::endl << std::endl;
    return os;

}

