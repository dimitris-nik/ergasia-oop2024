#include "../include/Category.h"
#include "../include/utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

Category::Category(const string& name) : name(name) {
    subcategories = vector<Category*>();
    products = vector<Product*>();
}

Category::~Category(){
    for (auto& subcategory : subcategories) {
        if (subcategory) {
            delete subcategory;
            subcategory = nullptr;
        }
    }
}

Category* Category::addSubcategory(const string& subcategory){
    Category* newSubcategory = new Category(subcategory);
    subcategories.push_back(newSubcategory);
    return newSubcategory;
}

void Category::addProduct(Product* product){
    products.push_back(product);
}

void Category::removeProduct(Product* product){
    for (auto it = products.begin(); it != products.end(); ) {
        if (*it == product) {
            it = products.erase(it);
        } else {
            ++it;
        }
    }
    //delete it from subcategories
    for (auto& subcategory : subcategories) {
        if (subcategory) {
            subcategory->removeProduct(product);
        }
    }
}

Product* Category::generateRandomProduct(){
    if (products.empty()) {
        return nullptr;
    }
    return products[rand() % products.size()];
}

void Category::displaySubcategories() const{
    for (const auto& subcat : subcategories) {
        cout << subcat->name << " ";
    }
    cout << endl;
}

void Category::displayProducts() const{
    for (const auto& product : products) {
        cout << "\"" << product->getTitle() << "\" ";
    }
    cout << endl;
}

Category* Category::findSubcategory(const string& subcategory) const{
    for (const auto& subcat : subcategories) {
        if (subcat->name == subcategory) {
            return subcat;
        }
    }
    return nullptr;
}

Category* CategoryManager::addCategory(const string & category){
    Category* newCategory = new Category(category);
    categories.push_back(newCategory);
    return newCategory;
}

int Category::getAmountForDiscount() const{
    return amountForDiscount;
}

void Category::setAmountForDiscount(int amount){
    amountForDiscount = amount;
}

CategoryManager::~CategoryManager(){
    for (auto& category : categories) {
        if (category) {
            delete category;
            category = nullptr;
        }
    }
}

void CategoryManager::displayCategories() const{
    for (const auto& category : categories) {
        cout << category->name << " ";
    }
    cout << endl;
}

Category* CategoryManager::findCategory(const string& category) const{
    for (const auto& cat : categories) {
        if (cat->name == category) {
            return cat;
        }
    }
    return nullptr;
}

void CategoryManager::addProduct(Product* product, const string& category, const string& subcategory){
    Category* cat = findCategory(category);
    if (cat == nullptr) {
        cerr << "Product " << product->getTitle() << " has invalid category " << category << endl;
        return;
    }
    Category* subcat = cat->findSubcategory(subcategory);
    if (subcat == nullptr) {
        cerr << "Product " << product->getTitle() << " has invalid subcategory " << subcategory << endl;
        return;
    }
    cat->addProduct(product);
    subcat->addProduct(product);
}

void CategoryManager::removeProduct(Product* product){
    string category = product->getCategory();
    findCategory(category)->removeProduct(product);
}