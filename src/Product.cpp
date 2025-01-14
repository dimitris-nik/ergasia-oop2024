#include "../include/Product.h"
#include <iostream>
#include <iomanip>

Product::Product(std::string title, std::string description, std::string category, std::string subcategory, double price, std::string measurementType, int amount)
    : title(title), description(description), category(category), subcategory(subcategory),
    price(price), measurementType(measurementType), amount(amount) {} 
    // all we need is the initializer list

void Product::displayProduct() const {
    std::cout << "-----" << title << "-----" << std::endl;
    std::cout << "Description: " << description << std::endl;
    std::cout << "Category: " << category << std::endl;
    std::cout << "Subcategory: " << subcategory << std::endl;
    std::cout << "Price per " << (measurementType == "Kg" ? "Kg" : "unit") << ": " << std::fixed << std::setprecision(2) << price << "€" << std::endl;
    std::cout << "Total " << (measurementType == "Kg" ? "Kg" : "units") << " available: " << amount << std::endl;
}

bool Product::operator<(const Product& other) const { 
    // need to overload < operator for the map in Cart
    return title < other.title;
}

std::ostream& operator<<(std::ostream& os, const Product& product) { 
    //overload operator << in Product class to save products to file
    os << product.title << " @ " << product.description << " @ "
       << product.category << " @ " << product.subcategory << " @ "
       << std::fixed << std::setprecision(2) << product.price << " @ "
       << product.measurementType << " @ " << product.amount; 
    return os;
}