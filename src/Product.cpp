#include "../include/Product.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include "../include/utils.h"
#include <vector>

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


std::ostream& operator<<(std::ostream& os, const Product& product) { 
    //overload operator << in Product class to save products to file
    os << product.title << " @ " << product.description << " @ "
       << product.category << " @ " << product.subcategory << " @ "
       << std::fixed << std::setprecision(2) << product.price << " @ "
       << product.measurementType << " @ " << product.amount; 
    return os;
}
std::string Product::getTitle() const {
    return title;
}
std::string Product::getCategory() const {
    return category;
}
std::string Product::getSubcategory() const {
    return subcategory;
}
void Product::setTitle(const std::string& title) {
    this->title = title;
}
void Product::setCategory(const std::string& category) {
    this->category = category;
}
void Product::setSubcategory(const std::string& subcategory) {
    this->subcategory = subcategory;
}

double Product::getPrice() const {
    return price;
}
void Product::setPrice(double price) {
    this->price = price;
}

std::string Product::getDescription() const {
    return description;
}
void Product::setDescription(const std::string& description) {
    this->description = description;
}

std::string Product::getMeasurementType() const {
    return measurementType;
}
void Product::setMeasurementType(const std::string& measurementType) {
    this->measurementType = measurementType;
}

int Product::getAmount() const {
    return amount;
}
void Product::setAmount(int amount) {
    this->amount = amount;
}

int Product::getAppearedInCart() const {
    return appearedInCart;
}
void Product::increaseAppearedInCart() {
    appearedInCart++;
}

ProductManager::~ProductManager() {
    for (auto& product : products) {
        delete product.second;
    }
}

bool ProductManager::addProduct(Product* product) {
    if (products.find(product->getTitle()) != products.end()) {
        return false;
    }
    products[product->getTitle()] = product;
    return true;
}
bool ProductManager::removeProduct(const std::string& title) {
    if (products.find(title) == products.end()) {
        return false;
    }
    products.erase(title);
    return true;
}

void ProductManager::displayProducts() const {
    for (const auto& product : products) {
        std::cout << "\"" << product.first << "\" ";
    }
}

Product* ProductManager::findProduct(const std::string& title) {
    if (products.find(title) == products.end()) {
        return nullptr;
    }
    return products[title];
}

void ProductManager::showUnavailableProducts() const {
    bool hasUnavailableProducts = false;
    for (const auto& product : products) {
        if (product.second->getAmount() == 0) {
            product.second->displayProduct();
            hasUnavailableProducts = true;
        }
    }
    if (!hasUnavailableProducts) {
        std::cout << "No unavailable products." << std::endl << std::endl;
    }
}

void ProductManager::saveProducts(const std::string& productsFile) const {
    std::ofstream file(productsFile);
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }
    for (const auto& product : products) {
        file << *(product.second) << std::endl;
    }
    file.close();
}

void ProductManager::showTopProducts() const {
    std::cout << "Top 3 Products:" << std::endl;
    std::vector<Product*> sortedProducts;
    for (const auto& product : products) {
        sortedProducts.push_back(product.second);
    }
    std::sort(sortedProducts.begin(), sortedProducts.end(), [](Product* a, Product* b) {
        return a->getAppearedInCart() > b->getAppearedInCart();
    });
    for (int i = 0; i < 3 && i < sortedProducts.size(); i++) {
        sortedProducts[i]->displayProduct();
        std::cout << "Times appeared in orders: " << sortedProducts[i]->getAppearedInCart() << std::endl << std::endl;
    }
}