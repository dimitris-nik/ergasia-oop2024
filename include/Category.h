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
        void addSubcategory(std::string subcategory);
        void displaySubcategories() const;
        void addProduct(Product* product);
        void removeProduct(Product* product);
        void displayProducts() const;
        Category* findSubcategory(const std::string& subcategory) const;

};