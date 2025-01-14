#pragma once
#define CART_H
#include <vector>
#include "Product.h"
#include <map>
#include <string>

class Cart {
public:
    std::map<Product, int> items; // item, amount
    void addProduct(Product& product, int quantity);
    void removeProduct(std::string& title);
    void updateProduct(Product& product, int quantity);
    void displayCart();
};
