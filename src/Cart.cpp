#include "../include/Cart.h"
#include <iostream>
#include <iomanip>

void Cart::addProduct(Product* product, int quantity) {
    items[product] += quantity;
    totalCost += product->getPrice() * quantity;
}
bool Cart::removeProduct(Product* product){
    if (items.find(product) != items.end()) {
        totalCost -= product->getPrice() * items[product];
        items.erase(product);
        return true;
    }
    return false;
}
bool Cart::updateProduct(Product* product, int quantity){   
    if (items.find(product) != items.end()) {
        totalCost -= product->getPrice() * items[product];
        items[product] = quantity;
        totalCost += product->getPrice() * quantity;
        return true;
    }
    return false;
}
 
bool Cart::isInCart(Product* product){
    return items.find(product) != items.end();
}  

void Cart::applyDiscount(Product* product, double discount) {
    totalCost -= product->getPrice() * items[product];
    totalCost += product->getPrice() * discount * items[product];
    std::cout << "Discount applied: " << 100 - discount * 100 << "% off on " << product->getTitle() << "!" << std::endl;
}

void Cart::checkout() {
    for (const auto& item : items) {
        item.first->setAmount(item.first->getAmount() - item.second);
    }
    items.clear();
    totalCost = 0;
}

void Cart::saveToFile(std::ostream& os, int number) { // this is necessary unfortunately to save numbered cart to file, would be cleaner if i could only use << operator :(
    os << "---CART " << number <<" START---" << std::endl;
    for (const auto& item : items) {
        os << item.second << " " << item.first->getTitle() << std::endl;
    }
    os << "---CART " << number <<" END---" << std::endl;
    os << "Total Cost: " << std::fixed << std::setprecision(2) << totalCost;
}

std::ostream& operator<<(std::ostream& os, const Cart& cart) {
    os << "---CART START---" << std::endl;
    for (const auto& item : cart.items) {
        os << item.second << " " << item.first->getTitle() << std::endl;
    }
    os << "---CART END---" << std::endl;
    os << "Total Cost: " << std::fixed << std::setprecision(2) << cart.totalCost << std::endl << std::endl;
    return os;
}

