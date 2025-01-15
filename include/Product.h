#pragma once
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
    int appearedInCart = 0;

    Product(std::string title, std::string description, std::string category, std::string subcategory, double price, std::string measurementType, int amount);

    void displayProduct() const;

    bool operator<(const Product& other) const; // we need need to overload < operator for the map in Cart
    friend std::ostream& operator<<(std::ostream& os, const Product& product);
};
