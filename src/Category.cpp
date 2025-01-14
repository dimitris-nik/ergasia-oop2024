#include <../include/Category.h>
#include <iostream>
#include <vector>
#include <string>
#pragma once

Category::Category(std::string name) : name(name) {}
void Category::addSubcategory(Category* subcategory){
    subcategories.push_back(subcategory);
}
void Category::displaySubcategories() const;
void Category::addProduct(Product* product){
    products.push_back(product);
}
void Category::displayProducts() const{
    std::cout << "Products: " << std::endl;
    for (const auto& product : products) {
        product->displayProduct();
    }
}