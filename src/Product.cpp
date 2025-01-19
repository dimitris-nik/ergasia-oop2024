#include "../include/Product.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include "../include/utils.h"
#include <vector>

using namespace std;

Product::Product(string title, string description, string category, string subcategory, double price, string measurementType, int amount)
    : title(title), description(description), category(category), subcategory(subcategory),
    price(price), measurementType(measurementType), amount(amount) {} 

void Product::displayProduct() const {
    cout << "-----" << title << "-----" << endl;
    cout << "Description: " << description << endl;
    cout << "Category: " << category << endl;
    cout << "Subcategory: " << subcategory << endl;
    cout << "Price per " << (measurementType == "Kg" ? "Kg" : "unit") << ": " << fixed << setprecision(2) << price << "€" << endl;
    cout << "Total " << (measurementType == "Kg" ? "Kg" : "units") << " available: " << amount << endl;
}


ostream& operator<<(ostream& os, const Product& product) { 
    //overload operator << in Product class to make saving products to file easier
    os << product.title << " @ " << product.description << " @ "
       << product.category << " @ " << product.subcategory << " @ "
       << fixed << setprecision(2) << product.price << " @ "
       << product.measurementType << " @ " << product.amount; 
    return os;
}
string Product::getTitle() const {
    return title;
}
string Product::getCategory() const {
    return category;
}
string Product::getSubcategory() const {
    return subcategory;
}
void Product::setTitle(const string& title) {
    this->title = title;
}
void Product::setCategory(const string& category) {
    this->category = category;
}
void Product::setSubcategory(const string& subcategory) {
    this->subcategory = subcategory;
}

double Product::getPrice() const {
    return price;
}
void Product::setPrice(double price) {
    this->price = price;
}

string Product::getDescription() const {
    return description;
}
void Product::setDescription(const string& description) {
    this->description = description;
}

string Product::getMeasurementType() const {
    return measurementType;
}
void Product::setMeasurementType(const string& measurementType) {
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

void ProductManager::addProduct(Product* product) {
    products[product->getTitle()] = product;
}
void ProductManager::removeProduct(const string& title) {
    products.erase(title);
}

void ProductManager::displayProducts() const {
    for (const auto& product : products) {
        cout << "\"" << product.first << "\" ";
    }
    cout << endl;
}

Product* ProductManager::findProduct(const string& title) {
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
        cout << "No unavailable products." << endl << endl;
    }
}

void ProductManager::showTopProducts() const {
    cout << "Top 3 Products:" << endl;
    vector<Product*> sortedProducts;
    // Add all products to a vector 
    for (const auto& product : products) {
        sortedProducts.push_back(product.second);
    }
    // Sort the products by the number of times they appeared in carts
    sort(sortedProducts.begin(), sortedProducts.end(), [](Product* a, Product* b) {
        return a->getAppearedInCart() > b->getAppearedInCart();
    });
    // Display the top 3 products
    for (size_t i = 0; i < 3 && i < sortedProducts.size(); i++) {
        sortedProducts[i]->displayProduct();
        cout << "Times appeared in orders: " << sortedProducts[i]->getAppearedInCart() << endl << endl;
    }
}

void ProductManager::saveProducts(const string& productsFile) const {
    ofstream file(productsFile);
    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return;
    }
    bool first = true;
    for (const auto& product : products) {
        // Add a newline before each product except the first one
        if (!first) {
            file << endl;
        }
        // Get the product and save it to the file
        file << *(product.second);
        first = false;
    }
    file.close();
}
