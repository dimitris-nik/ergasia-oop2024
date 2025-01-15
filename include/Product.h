#pragma once
#include <string>
#include <iostream>
#include <unordered_map>

class Product {
private:
    std::string title;
    std::string description;
    std::string category;
    std::string subcategory;
    double price;
    std::string measurementType;
    int amount;
    int appearedInCart = 0;
public:
    Product(std::string title, std::string description, std::string category, std::string subcategory, double price, std::string measurementType, int amount);
    void displayProduct() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getCategory() const;
    std::string getSubcategory() const;
    double getPrice() const;
    std::string getMeasurementType() const;
    int getAmount() const;
    int getAppearedInCart() const;
    void increaseAppearedInCart();
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setCategory(const std::string& category);
    void setSubcategory(const std::string& subcategory);
    void setPrice(double price);
    void setMeasurementType(const std::string& measurementType);
    void setAmount(int amount);
    friend std::ostream& operator<<(std::ostream& os, const Product& product);
};

class ProductManager {
std::unordered_map<std::string, Product*> products;
public:
    ~ProductManager();
    bool addProduct(Product* product);
    bool removeProduct(const std::string& title);
    void displayProducts() const;
    Product* findProduct(const std::string& title);
    void showUnavailableProducts() const;
    void saveProducts(const std::string& productsFile) const;
    void showTopProducts() const;
};