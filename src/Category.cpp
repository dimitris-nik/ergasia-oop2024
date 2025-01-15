#include "../include/Category.h"
#include <iostream>
#include <vector>
#include <string>

Category::Category(std::string name) : name(name) {}

Category::~Category(){
    for (auto& subcategory : subcategories) {
        delete subcategory;
    }
}

void Category::addSubcategory(std::string subcategory){
    Category* newSubcategory = new Category(subcategory);
    subcategories.push_back(newSubcategory);
}

void Category::addProduct(Product* product){
    products.push_back(product);
}

void Category::removeProduct(Product* product){
    for (auto it = products.begin(); it != products.end(); ) {
        if (*it == product) {
            it = products.erase(it);
        } else {
            it++;
        }
    }
    //delete it from subcategories
    for (auto& subcategory : subcategories) {
        subcategory->removeProduct(product);
    }
}

void Category::displaySubcategories() const{
    for (const auto& subcategory : subcategories) {
        std::cout << subcategory->name << std::endl;
    }
}

void Category::displayProducts() const{
    for (const auto& product : products) {
        std::cout << product->title;
    }
}

Category* Category::findSubcategory(const std::string& subcategory) const{
    for (const auto& subcat : subcategories) {
        if (subcat->name == subcategory) {
            return subcat;
        }
    }
    return nullptr;
}