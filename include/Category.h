#pragma once
#include <vector>
#include <string>
#include "Product.h"

class Category {
    private:
        std::vector<Category*> subcategories;
        std::vector<Product*> products;
    public:
        std::string name;
        Category(std::string name);
        ~Category();
        Category* addSubcategory(std::string subcategory);
        void displaySubcategories() const;
        void addProduct(Product* product);
        void removeProduct(Product* product);
        void displayProducts() const;
        Category* findSubcategory(const std::string& subcategory) const;
};

class CategoryManager {
    private:
        std::vector<Category*> categories;
    public:
        CategoryManager(const std::string & categoriesFile);
        ~CategoryManager();
        Category* addCategory(std::string category);
        void displayCategories() const;
        Category* findCategory(const std::string& category) const;
};