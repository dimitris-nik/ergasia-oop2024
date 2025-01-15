#include "../include/Admin.h"
#include "../include/utils.h"
#include <iostream>
#include <map>
#include <limits>
#include <algorithm>

Admin::Admin(std::string username, std::string password) : User(username, password, true) {}

void Admin::displayMenu() {
    std::cout << "---Admin Menu---" << std::endl;
    std::cout << "1. Add Product" << std::endl;
    std::cout << "2. Edit Product" << std::endl;
    std::cout << "3. Remove Product" << std::endl;
    std::cout << "4. Search Product" << std::endl;
    std::cout << "5. Show Unavailable Products" << std::endl;
    std::cout << "6. Show Top 5 Products" << std::endl;
    std::cout << "7. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

void Admin::addProduct(std::map<std::string, Product*>& products, std::vector<Category*>& categories){
    std::string title, description, category, subcategory, measurementType;
    Category* productCategory;
    Category* productSubategory;
    double price;
    int amount;

    std::cout << "Give product title: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);

    std::cout << "Give product description: ";
    std::getline(std::cin, description);

    std::cout << "Give one of the following categories: ";
    for (const auto& cat : categories) {
        std::cout << cat->name << " ";
    }
    std::cout << std::endl;
    
    while(true){
        std::getline(std::cin, category);
        bool categoryExists = false;
        for(const auto& cat : categories){
            if(cat->name == category){
                categoryExists = true;
                productCategory = cat;
                break;
            }
        }
        if(categoryExists) break;
        std::cout << "Invalid Category. Please choose from the above." << std::endl;
    }

    std::cout << "Give one of the following subcategories: ";

    productCategory->displaySubcategories();
    
    while(true){
        std::getline(std::cin, subcategory);
        bool subCategoryExists = false;
        for (const auto& cat : categories) {
            if (cat->name == category) {
                if(cat->findSubcategory(subcategory)){
                    subCategoryExists = true;
                    productSubategory = cat;
                    break;
                }
            }
            if(subCategoryExists) break;
        }
        if(subCategoryExists) break;
        std::cout << "Invalid Subcategory. Please choose from the above." << std::endl;
    }

    std::cout << "Give product price: ";
    std::cin >> price;

    std::cout << "Give product measurement type: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, measurementType);

    std::cout << "Give product amount: ";
    std::cin >> amount;

    Product * product = new Product(title, description, category, subcategory, price, measurementType, amount);
    products[title] = product;
    productCategory->addProduct(product);
    productSubategory->addProduct(product);
    std::cout << product;
    std::cout << "Product addedd successfully!" << std::endl;
    
}

void Admin::editProduct(std::map<std::string, Product*>& products, std::vector<Category*>& categories){
    std::string title;
    std::cout << "Enter the title of the product you want to edit: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    if(products.find(title) == products.end()){
        std::cout << "Product not found." << std::endl;
        return;
    }
    Product* product = products[title];
    std::string newTitle, newDescription, newCategory, newSubcategory, newMeasurementType;
    double newPrice;
    int newAmount;
    std::cout << "Enter new title: ";
    std::getline(std::cin, newTitle);
    std::cout << "Enter new description: ";
    std::getline(std::cin, newDescription);
    std::cout << "Enter new category: ";
    std::getline(std::cin, newCategory);
    std::cout << "Enter new subcategory: ";
    std::getline(std::cin, newSubcategory);
    std::cout << "Enter new price: ";
    std::cin >> newPrice;
    std::cout << "Enter new amount: ";
    std::cin >> newAmount;
    std::cout << "Enter new measurement type: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, newMeasurementType);
    product->title = newTitle;
    product->description = newDescription;
    product->category = newCategory;
    product->subcategory = newSubcategory;
    product->price = newPrice;
    product->amount = newAmount;
    product->measurementType = newMeasurementType;
}

void Admin::removeProduct(std::map<std::string, Product*>& products, std::vector<Category*>& categories){
    std::string title;
    std::cout << "Enter the title of the product you want to remove: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    if(products.find(title) == products.end()){
        std::cout << "Product not found." << std::endl;
        return;
    }
    Product* product = products[title];
    products.erase(title);
    for (const auto& cat : categories) {
        if (cat->name == product->category) {
            cat->removeProduct(product);
        }
    }
    delete product;
}

bool Admin::executeCommand(std::map<std::string, Product*>& products, std::vector<Category*>& categories){
    int choice;
    std::cin >> choice;
    while (choice < 1 || choice > 7) {
        std::cout << "Invalid Option. Please enter a number between 1 and 7: ";
        std::cin >> choice;
    }
    switch(choice){
        case 1: {
            addProduct(products, categories);
            break;
        }
        case 2: {
            editProduct(products, categories);
            break;
        }
        case 3: {
            removeProduct(products, categories);
            break;
        }
        case 7: {
            return false;
        }
        default: {
            std::cout << "Invalid Option." << std::endl;
            break;
        }
    }
    return true;
}