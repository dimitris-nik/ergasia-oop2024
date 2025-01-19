#pragma once
#include <vector>
#include <string>
#include "Product.h"

class Category {
    private:
        std::vector<Category*> subcategories;
        std::vector<Product*> products;
        int amountForDiscount;
    public:
        std::string name;
        Category(const std::string & name);
        ~Category();
        Category* addSubcategory(const std::string& subcategory);
        void displaySubcategories() const;
        void addProduct(Product* product);
        void removeProduct(Product* product);
        Product * generateRandomProduct();
        void displayProducts() const;
        int getAmountForDiscount() const;
        void setAmountForDiscount(int amount);
        Category* findSubcategory(const std::string& subcategory) const;
};

class CategoryManager {
    private:
        std::vector<Category*> categories;
    public:
        ~CategoryManager();
        Category* addCategory(const std::string & category);
        void displayCategories() const;
        Category* findCategory(const std::string& category) const;
        void addProduct(Product* product, const std::string& category, const std::string& subcategory);
        void removeProduct(Product* product);
};