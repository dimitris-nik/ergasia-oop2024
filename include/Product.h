#ifndef PRODUCT_H
#define PRODUCT_H
#include <string>
#include <iostream>

class Product {
public:
    std::string title;
    std::string description;
    std::string category;
    std::string subcategory;
    double price;
    std::string measurementType;
    int amount;

    Product(std::string title, std::string description, std::string category, std::string subcategory, double price, std::string measurementType, int amount);

    void displayProduct() const;

    bool operator<(const Product& other) const; // we need need to overload < operator for the map in Cart
};

#endif