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

void Admin::addProduct(ProductManager& products, CategoryManager& categories){
    std::string title, description, categoryStr, subcategoryStr, measurementType;
    Category* productCategory;
    Category* productSubcategory;
    double price;
    int amount;

    std::cout << "Give product title: ";
    title = readString();
    while(products.findProduct(title)){
        std::cout << "Product with this title already exists. Please enter a different title: ";
        title = readString();
    }
    std::cout << "Give product description: ";
    description = readString();

    std::cout << "Give one of the following categories: ";
    categories.displayCategories();
    categoryStr = readString();
    productCategory = categories.findCategory(categoryStr);
    while (!productCategory) {
        std::cout << "Invalid Category. Please choose from the above." << std::endl;
        categoryStr = readString();
        productCategory = categories.findCategory(categoryStr);
    }
    std::cout << "Give one of the following subcategories: ";

    productCategory->displaySubcategories();
    subcategoryStr = readString();
    productSubcategory = productCategory->findSubcategory(subcategoryStr);
    while (!productSubcategory) {
        std::cout << "Invalid Subcategory. Please choose from the above." << std::endl;
        subcategoryStr = readString();
        productSubcategory = productCategory->findSubcategory(subcategoryStr);
    }
    std::cout << "Give product price: ";
    price = readDouble();
    std::cout << "Give product measurement type: ";
    measurementType = readString();    

    std::cout << "Give product amount: ";
    amount = readInt();

    Product * product = new Product(title, description, categoryStr, subcategoryStr, price, measurementType, amount);
    products.addProduct(product);
    categories.addProduct(product, categoryStr, subcategoryStr);
    std::cout << "Product added successfully!" << std::endl;
}

void Admin::editProduct(ProductManager& products, CategoryManager& categories){
    std::string title;
    std::cout << "Enter the title of the product you want to edit: ";
    title = readString();
    if(!products.findProduct(title)){
        std::cout << "Product not found." << std::endl;
        return;
    }
    Product* product = products.findProduct(title);
    std::cout << "Enter number of field you want to edit: 1.Title 2.Description 3.Category and Subcategory 4.Price 5.Available Kg 6.Nothing" << std::endl;
    int choice = readInt();
    while (choice < 1 || choice > 6) {
        std::cout << "Invalid Option. Please enter a number between 1 and 6: ";
        choice = readInt();
    }
    switch(choice){
        case 1: {
            std::string newTitle;
            std::cout << "Enter new title: ";
            newTitle = readString();
            while (products.findProduct(newTitle)) {
                std::cout << "Product with this title already exists. Please enter a different title: ";
                newTitle = readString();
            }
            products.removeProduct(product->getTitle());
            product->setTitle(newTitle);
            products.addProduct(product);
            break;
        }
        case 2: {
            std::string newDescription;
            std::cout << "Enter new description: ";
            newDescription = readString();
            product->setDescription(newDescription);
            break;
        }
        case 3: {
            std::string newCategory;
            std::string newSubcategory;
            std::cout << "Enter new category: ";
            categories.displayCategories();
            newCategory = readString();
            while (!categories.findCategory(newCategory)) {
                std::cout << "Invalid Category. Please choose from the above." << std::endl;
                newCategory = readString();
            }
            std::cout << "Enter new subcategory: ";
            categories.findCategory(newCategory)->displaySubcategories();
            newSubcategory = readString();
            while (!categories.findCategory(newCategory)->findSubcategory(newSubcategory)) {
                std::cout << "Invalid Subcategory. Please choose from the above." << std::endl;
                newSubcategory = readString();
            }
            categories.removeProduct(product);
            product->setCategory(newCategory);
            product->setSubcategory(newSubcategory);
            categories.addProduct(product, newCategory, newSubcategory);
            break;
        }
        case 4: {
            double newPrice;
            std::cout << "Enter new price: ";
            newPrice = readDouble();
            while (newPrice < 0) {
                std::cout << "Invalid price. Please enter a valid price: ";
                newPrice = readDouble();
            }
            product->setPrice(newPrice);
            break;
        }
        case 5: {
            int newAmount;
            std::cout << "Enter new amount: ";
            newAmount = readInt();
            while (newAmount < 0) {
                std::cout << "Invalid amount. Please enter a valid amount: ";
                newAmount = readInt();
            }
            product->setAmount(newAmount);
            break;
        }
        case 6: {
            break;
        }
        default: {
            std::cout << "Invalid Option." << std::endl;
            return;
        }
    }
    std::cout << "Product updated!" << std::endl;

}

void Admin::removeProduct(ProductManager& products, CategoryManager& categories){
    std::string title;
    std::cout << "Enter the title of the product you want to remove: ";
    title = readString();
    if(!products.findProduct(title)){
        std::cout << "Product not found." << std::endl;
        return;
    }
    Product* product = products.findProduct(title);
    categories.removeProduct(product); // Ligo periergo alla dax
    products.removeProduct(title);
    delete product;
}


void Admin::searchProduct(ProductManager& products, CategoryManager& categories){
    std::cout << "1. Search for a specific product (by title)." << std::endl;
    std::cout << "2. View the products of a specific category." << std::endl;
    std::cout << "3. Show all the available products." << std::endl;
    std::cout << "Enter your choice: ";
    int choice;
    choice = readInt();
    while (choice < 1 || choice > 3) {
        std::cout << "Invalid Option. Please enter a number between 1 and 3: ";
        choice = readInt();
    }
    switch(choice){
        case 1: {
            std::string titleSearch;
            std::cout << "Enter title to search: ";
            titleSearch = readString();
            if (products.findProduct(titleSearch)) {
                products.findProduct(titleSearch)->displayProduct();
            } else {
                std::cout << "Product not found." << std::endl;
            }
            break;
        }
        case 2: {
            std::string categorySearch;
            std::string subcategorySearch;
            std::cout << "Enter category to search: ";
            categorySearch = readString();
            auto searchCategory = categories.findCategory(categorySearch);
            while (!searchCategory) {
                std::cout << "Invalid Category. Please choose from the above." << std::endl;
                categorySearch = readString();
                searchCategory = categories.findCategory(categorySearch);
            }
            std::cout << "Enter subcategory to search (leave empty for all subcategories): ";
            subcategorySearch = readString();
            auto searchSubcategory = searchCategory->findSubcategory(subcategorySearch);
            while (!subcategorySearch.empty() && !searchSubcategory) {
                std::cout << "Invalid Subcategory. Please choose from the above, or leave empty." << std::endl;
                subcategorySearch = readString();
                searchSubcategory = searchCategory->findSubcategory(subcategorySearch);
            }
            if (subcategorySearch.empty()) {
                searchCategory->displayProducts();
            } else {
                searchSubcategory->displayProducts();
            }
            break;
        }
        case 3: {
            std::cout << "Results: ";
            products.displayProducts();
            std::cout << std::endl;
            std::cout << "Select a product title: ";
            std::string selectedTitle;
            selectedTitle = readString();
            while (!products.findProduct(selectedTitle)) {
                std::cout << "Invalid Product. Please choose from the above." << std::endl;
                selectedTitle = readString();
            }
            products.findProduct(selectedTitle)->displayProduct();
            break;
        }
        default: {
            std::cout << "Invalid Option." << std::endl;
            break;
        }
    }

}


bool Admin::executeCommand(ProductManager& products, CategoryManager& categories){
    int choice = readInt();
    while (choice < 1 || choice > 7) {
        std::cout << "Invalid Option. Please enter a number between 1 and 7: ";
        choice = readInt();
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
            searchProduct(products, categories);
            break;
        }
        case 5: {
            products.showUnavailableProducts();
            break;
        }
        case 6: {
            products.showTopProducts();
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