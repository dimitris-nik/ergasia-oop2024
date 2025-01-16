#pragma once
#define CART_H
#include <vector>
#include "Product.h"
#include <map>
#include <string>

class Cart {
    double totalCost = 0;
public:
    std::map<Product*, int> items; // item, amount
    void addProduct(Product* product, int quantity);
    bool removeProduct(Product* product);
    bool updateProduct(Product* product, int quantity);
    void checkout();
    void saveToFile(std::ostream& os, int number);
    friend std::ostream& operator<<(std::ostream& os, const Cart& cart);
};
