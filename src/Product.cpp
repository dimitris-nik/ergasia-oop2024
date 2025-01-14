#include "../include/Product.h"
#include <iostream>
#include <iomanip>

Product::Product(std::string title, std::string description, std::string category, std::string subcategory, double price, std::string measurementType, int amount)
    : title(title), description(description), category(category), subcategory(subcategory),
    price(price), measurementType(measurementType), amount(amount) {} // we only need initializer list

void Product::displayProduct() const {
    std::cout << "-----" << title << "-----" << std::endl;
    std::cout << "Description: " << description << std::endl;
    std::cout << "Category: " << category << std::endl;
    std::cout << "Subcategory: " << subcategory << std::endl;
    std::cout << "Price per " << (measurementType == "Kg" ? "Kg" : "unit") << ": " << std::fixed << std::setprecision(2) << price << "€" << std::endl;
    std::cout << "Total " << (measurementType == "Kg" ? "Kg" : "units") << " available: " << amount << std::endl;
}

// Implementation of the less-than operator
bool Product::operator<(const Product& other) const {
    // Use title for comparison
    return title < other.title;
}