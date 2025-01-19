#pragma once
#define CART_H
#include <vector>
#include "Product.h"
#include <unordered_map>
#include <string>

class Cart {
public:
    double totalCost = 0;
    std::unordered_map<Product*, int> items; // item, amount
    void addProduct(Product* product, int quantity);
    bool removeProduct(Product* product);
    bool updateProduct(Product* product, int quantity);
    bool isInCart(Product* product);
    void applyDiscount(Product* product, double discount);
    void checkout();
    void saveToFile(std::ostream& os, int number);
    friend std::ostream& operator<<(std::ostream& os, const Cart& cart);
};
