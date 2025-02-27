#include "../include/Cart.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Items is an unordered map binding products to their quantity in the cart
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
    cout << "Discount applied: " << 100 - discount * 100 << "% off on " << product->getTitle() << "!" << endl;
}

// Upon checkout, update the product amount which represents the stock
void Cart::checkout() {
    for (const auto& item : items) {
        item.first->setAmount(item.first->getAmount() - item.second);
    }
    items.clear();
    totalCost = 0;
}

void Cart::saveToFile(ostream& os, int number) { // Unfortunately, we cannot use the overloaded operator << here because we need to print the number of the cart :(
    os << "---CART " << number <<" START---" << endl;
    for (const auto& item : items) {
        os << item.second << " " << item.first->getTitle() << endl;
    }
    os << "---CART " << number <<" END---" << endl;
    os << "Total Cost: " << fixed << setprecision(2) << totalCost;
}

ostream& operator<<(ostream& os, const Cart& cart) {
    cout << endl;
    os << "---CART START---" << endl;
    for (const auto& item : cart.items) {
        os << item.second << " " << item.first->getTitle() << endl;
    }
    os << "---CART END---" << endl;
    os << "Total Cost: " << fixed << setprecision(2) << cart.totalCost << endl << endl;
    return os;
}

