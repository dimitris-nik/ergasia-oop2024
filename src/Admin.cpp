#include "../include/Admin.h"
#include "../include/utils.h"
#include <iostream>
#include <map>
#include <limits>
#include <algorithm>
using namespace std;

Admin::Admin(string username, string password) : User(username, password, true) {}

void Admin::displayMenu() {
    cout << endl;
    cout << "---Admin Menu---" << endl;
    cout << "1. Add Product" << endl;
    cout << "2. Edit Product" << endl;
    cout << "3. Remove Product" << endl;
    cout << "4. Search Product" << endl;
    cout << "5. Show Unavailable Products" << endl;
    cout << "6. Show Top 5 Products" << endl;
    cout << "7. Exit" << endl;
    cout << "Enter your choice: ";
}

void Admin::addProduct(ProductManager& products, CategoryManager& categories){
    string title, description, categoryStr, subcategoryStr, measurementType;
    Category* productCategory;
    Category* productSubcategory;
    double price;
    int amount;

    cout << "Give product title: ";
    title = readString();
    while(products.findProduct(title)){
        cout << "Product with this title already exists. Please enter a different title: ";
        title = readString();
    }
    cout << "Give product description: ";
    description = readString();

    cout << "Give one of the following categories: ";
    categories.displayCategories();
    categoryStr = readString();
    productCategory = categories.findCategory(categoryStr);
    while (!productCategory) {
        cout << "Invalid Category. Please choose from the above." << endl;
        categoryStr = readString();
        productCategory = categories.findCategory(categoryStr);
    }
    cout << "Give one of the following subcategories: ";

    productCategory->displaySubcategories();
    subcategoryStr = readString();
    productSubcategory = productCategory->findSubcategory(subcategoryStr);
    while (!productSubcategory) {
        cout << "Invalid Subcategory. Please choose from the above." << endl;
        subcategoryStr = readString();
        productSubcategory = productCategory->findSubcategory(subcategoryStr);
    }
    cout << "Give product price: ";
    price = readDouble();
    cout << "Give product measurement type: ";
    measurementType = readString();    

    cout << "Give product amount: ";
    amount = readInt();

    Product * product = new Product(title, description, categoryStr, subcategoryStr, price, measurementType, amount);
    products.addProduct(product);
    categories.addProduct(product, categoryStr, subcategoryStr);
    cout << "Product added successfully!" << endl;
}

void Admin::editProduct(ProductManager& products, CategoryManager& categories){
    string title;
    cout << "Enter the title of the product you want to edit: ";
    title = readString();
    if(!products.findProduct(title)){
        cout << "Product not found." << endl;
        return;
    }
    Product* product = products.findProduct(title);
    cout << "Enter number of field you want to edit: 1.Title 2.Description 3.Category and Subcategory 4.Price 5.Available Kg 6.Nothing" << endl;
    int choice = readInt();
    while (choice < 1 || choice > 6) {
        cout << "Invalid Option. Please enter a number between 1 and 6: ";
        choice = readInt();
    }
    switch(choice){
        case 1: {
            string newTitle;
            cout << "Enter new title: ";
            newTitle = readString();
            while (products.findProduct(newTitle)) {
                cout << "Product with this title already exists or you typed the same title. Please enter a different title: ";
                newTitle = readString();
            }
            // We can't change the key of the product in the map, so we need to remove it and add it again
            products.removeProduct(product->getTitle());
            product->setTitle(newTitle);
            products.addProduct(product);
            break;
        }
        case 2: {
            string newDescription;
            cout << "Enter new description: ";
            newDescription = readString();
            product->setDescription(newDescription);
            break;
        }
        case 3: {
            string newCategory;
            string newSubcategory;
            cout << "Enter new category: ";
            categories.displayCategories();
            newCategory = readString();
            while (!categories.findCategory(newCategory)) {
                cout << "Invalid Category. Please choose from the above." << endl;
                newCategory = readString();
            }
            cout << "Enter new subcategory: ";
            categories.findCategory(newCategory)->displaySubcategories();
            newSubcategory = readString();
            while (!categories.findCategory(newCategory)->findSubcategory(newSubcategory)) {
                cout << "Invalid Subcategory. Please choose from the above." << endl;
                newSubcategory = readString();
            }
            // Completely remove the product from the category and subcategory and then add it again
            categories.removeProduct(product);
            product->setCategory(newCategory);
            product->setSubcategory(newSubcategory);
            categories.addProduct(product, newCategory, newSubcategory);
            break;
        }
        case 4: {
            double newPrice;
            cout << "Enter new price: ";
            newPrice = readDouble();
            while (newPrice < 0) {
                cout << "Invalid price. Please enter a valid price: ";
                newPrice = readDouble();
            }
            product->setPrice(newPrice);
            break;
        }
        case 5: {
            int newAmount;
            cout << "Enter new amount: ";
            newAmount = readInt();
            while (newAmount < 0) {
                cout << "Invalid amount. Please enter a valid amount: ";
                newAmount = readInt();
            }
            product->setAmount(newAmount);
            break;
        }
        case 6: {
            break;
        }
        default: {
            cout << "Invalid Option." << endl;
            return;
        }
    }
    cout << "Product updated!" << endl;

}

void Admin::removeProduct(ProductManager& products, CategoryManager& categories){
    string title;
    cout << "Enter the title of the product you want to remove: ";
    title = readString();
    if(!products.findProduct(title)){
        cout << "Product not found." << endl;
        return;
    }
    Product* product = products.findProduct(title);
    categories.removeProduct(product); 
    products.removeProduct(title);
    delete product;
}




bool Admin::executeCommand(ProductManager& products, CategoryManager& categories){
    int choice = readInt();
    while (choice < 1 || choice > 7) {
        cout << "Invalid Option. Please enter a number between 1 and 7: ";
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
            cout << "Invalid Option." << endl;
            break;
        }
    }
    return true;
}