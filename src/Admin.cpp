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

void Admin::addProduct(std::map<std::string, Product*>& products, CategoryManager& categories){
    std::string title, description, categoryStr, subcategoryStr, measurementType;
    Category* productCategory;
    Category* productSubcategory;
    double price;
    int amount;

    std::cout << "Give product title: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    while(products.find(title) != products.end()){
        std::cout << "Product with this title already exists. Please enter a different title: ";
        std::getline(std::cin, title);
    }

    std::cout << "Give product description: ";
    std::getline(std::cin, description);

    std::cout << "Give one of the following categories: ";
    categories.displayCategories();
    while(true){
        std::getline(std::cin, categoryStr);
        productCategory = categories.findCategory(categoryStr);
        if(productCategory){
            break;
        }
        std::cout << "Invalid Category. Please choose from the above." << std::endl;
    }

    std::cout << "Give one of the following subcategories: ";

    productCategory->displaySubcategories();

    while(true){
        std::getline(std::cin, subcategoryStr);
        productSubcategory = productCategory->findSubcategory(subcategoryStr);
        if(productSubcategory){
            break;
        }
        std::cout << "Invalid Category. Please choose from the above." << std::endl;
    }
    std::cout << "Give product price: ";
    std::cin >> price;
    std::cout << "Give product measurement type: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, measurementType);

    std::cout << "Give product amount: ";
    std::cin >> amount;

    Product * product = new Product(title, description, categoryStr, subcategoryStr, price, measurementType, amount);
    products[title] = product;
    productCategory->addProduct(product);
    productSubcategory->addProduct(product);
    std::cout << "Product added successfully!" << std::endl;
}

void Admin::editProduct(std::map<std::string, Product*>& products, CategoryManager& categories){
    std::string title;
    std::cout << "Enter the title of the product you want to edit: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    if(products.find(title) == products.end()){
        std::cout << "Product not found." << std::endl;
        return;
    }
    Product* product = products[title];
    std::cout << "Enter number of field you want to edit: 1.Title 2.Description 3.Category and Subcategory 4.Price 5.Available Kg 6.Nothing" << std::endl;
    int choice;
    std::cin >> choice;
    while (choice < 1 || choice > 6) {
        std::cout << "Invalid Option. Please enter a number between 1 and 6: ";
        std::cin >> choice;
    }
    switch(choice){
        case 1: {
            std::string newTitle;
            std::cout << "Enter new title: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, newTitle);
            while (products.find(newTitle) != products.end()) {
                std::cout << "Product with this title already exists. Please enter a different title: ";
                std::getline(std::cin, newTitle);
            }
            products.erase(title);
            product->title = newTitle;
            products[newTitle] = product;
            break;
        }
        case 2: {
            std::string newDescription;
            std::cout << "Enter new description: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, newDescription);
            product->description = newDescription;
            break;
        }
        case 3: {
            std::string newCategory;
            std::string newSubcategory;
            std::cout << "Enter new category: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, newCategory);
            std::cout << "Enter new subcategory: ";
            std::getline(std::cin, newSubcategory);
            product->category = newCategory;
            product->subcategory = newSubcategory;
            categories.findCategory(product->category)->removeProduct(product);
            categories.findCategory(newCategory)->addProduct(product);
            break;
        }
        case 4: {
            double newPrice;
            std::cout << "Enter new price: ";
            std::cin >> newPrice;
            product->price = newPrice;
            break;
        }
        case 5: {
            int newAmount;
            std::cout << "Enter new amount: ";
            std::cin >> newAmount;
            product->amount = newAmount;
            break;
        }
        case 6: {
            break;
        }
        default: {
            std::cout << "Invalid Option." << std::endl;
            break;
        }
    }

}

void Admin::removeProduct(std::map<std::string, Product*>& products, CategoryManager& categories){
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
    categories.findCategory(product->category)->removeProduct(product);
    delete product;
}

bool Admin::executeCommand(std::map<std::string, Product*>& products, CategoryManager& categories){
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
        case 4: {
            break;
        }
        case 5: {
            break;
        }
        case 6: {
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