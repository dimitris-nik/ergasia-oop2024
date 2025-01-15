#include "../include/Category.h"
#include "../include/utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

Category::Category(const std::string& name) : name(name) {
    subcategories = std::vector<Category*>();
    products = std::vector<Product*>();
}

Category::~Category(){
    for (auto& subcategory : subcategories) {
        delete subcategory;
    }
}

Category* Category::addSubcategory(const std::string& subcategory){
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
            it++;
        }
    }
    //delete it from subcategories
    for (auto& subcategory : subcategories) {
        subcategory->removeProduct(product);
    }
}

void Category::displaySubcategories() const{
    for (const auto& subcat : subcategories) {
        std::cout << subcat->name << " ";
    }
    std::cout << std::endl;
}

void Category::displayProducts() const{
    for (const auto& product : products) {
        std::cout << "\"" << product->title << "\" ";
    }
    std::cout << std::endl;
}

Category* Category::findSubcategory(const std::string& subcategory) const{
    for (const auto& subcat : subcategories) {
        if (subcat->name == subcategory) {
            return subcat;
        }
    }
    return nullptr;
}

Category* CategoryManager::addCategory(const std::string & category){
    Category* newCategory = new Category(category);
    categories.push_back(newCategory);
    return newCategory;
}

CategoryManager::CategoryManager(const std::string & categoriesFile){
    std::ifstream file(categoriesFile);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening category file." << std::endl;
        return;
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string categoryName;
        std::string subcategoriesLine;

        std::getline(ss, categoryName, '(');
        categoryName = trim(categoryName);

        Category* category = addCategory(categoryName);

        if (std::getline(ss, subcategoriesLine, ')')) {
            std::stringstream subcats(subcategoriesLine);
            std::string subcategory;
            while (std::getline(subcats, subcategory, '@')) {
                category->addSubcategory(trim(subcategory));
            }
        }
    }
    file.close();
}

CategoryManager::~CategoryManager(){
    for (auto& category : categories) {
        delete category;
    }
}

void CategoryManager::displayCategories() const{
    for (const auto& category : categories) {
        std::cout << category->name << " ";
    }
    std::cout << std::endl;
}

Category* CategoryManager::findCategory(const std::string& category) const{
    for (const auto& cat : categories) {
        if (cat->name == category) {
            return cat;
        }
    }
    return nullptr;
}