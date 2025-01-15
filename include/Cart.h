#pragma once
#define CART_H
#include <vector>
#include "Product.h"
#include <map>
#include <string>

class Cart {
public:
    std::map<Product*, int> items; // item, amount
    void addProduct(Product* product, int quantity);
    void removeProduct(Product* product);
    void updateProduct(Product* product, int quantity);
    void clearCart();
    void checkout();
    friend std::ostream& operator<<(std::ostream& os, const Cart& cart);
};
