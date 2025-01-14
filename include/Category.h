#pragma once
#include <vector>
#include <string>

class Category {
    private:
        vector<Category*> subcategories;
        vector<Product*> products;
    public:
        std::string name;
        Category(std::string name);
        void addSubcategory(Category* subcategory);
        void displaySubcategories() const;
        void addProduct(Product* product);
        void displayProducts() const;

}