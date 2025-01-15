#include "../include/Customer.h"
#include "../include/Category.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>


Customer::Customer(std::string username, std::string password) : User(username, password, false), cart() {
        historyFileName = "files/order_history/" + username + "_history.txt";
}

void Customer::displayMenu() {
    std::cout << std::endl;
    std::cout << "---Customer Menu---" << std::endl;
    std::cout << "1. Search for a product" << std::endl;
    std::cout << "2. Add product to cart" << std::endl;
    std::cout << "3. Update product from cart" << std::endl;
    std::cout << "4. Remove product from cart" << std::endl;
    std::cout << "5. Complete order" << std::endl;
    std::cout << "6. View order history" << std::endl;
     std::cout << "7. View cart" << std::endl;
    std::cout << "8. Exit" << std::endl;
    std::cout << "Enter your choice: ";
} 

void Customer::searchProduct(std::map<std::string, Product*> products, CategoryManager& categories) {
    std::cout << "1. Search for a specific product (by title)." << std::endl;
    std::cout << "2. View the products of a specific category." << std::endl;
    std::cout << "3. Show all the available products." << std::endl;
    std::cout << "Enter your choice: ";
    int choice;
    std::cin >> choice;
    while (choice < 1 || choice > 3) {
        std::cout << "Invalid Option. Please enter a number between 1 and 3: ";
        std::cin >> choice;
    }
    switch(choice){
        case 1: {
            std::string titleSearch;
            std::cout << "Enter title to search: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, titleSearch);
            if (products.find(titleSearch) != products.end()) {
                products[titleSearch]->displayProduct();
            } else {
                std::cout << "Product not found." << std::endl;
            }
            break;
        }
        case 2: {
            std::string categorySearch;
            std::string subcategorySearch;
            std::cout << "Enter category to search: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, categorySearch);
            auto searchCategory = categories.findCategory(categorySearch);
            while (!searchCategory) {
                std::cout << "Invalid Category. Please choose from the above." << std::endl;
                std::getline(std::cin, categorySearch);
                searchCategory = categories.findCategory(categorySearch);
            }
            std::cout << "Enter subcategory to search (leave empty for all subcategories): ";
            std::getline(std::cin, subcategorySearch);
            auto searchSubcategory = searchCategory->findSubcategory(subcategorySearch);
            while (!subcategorySearch.empty() && !searchSubcategory) {
                std::cout << "Invalid Subcategory. Please choose from the above, or leave empty." << std::endl;
                std::getline(std::cin, subcategorySearch);
                searchSubcategory = searchCategory->findSubcategory(subcategorySearch);
            }
            if (subcategorySearch.empty()) {
                searchCategory->displayProducts();
            } else {
                searchSubcategory->displayProducts();
            }
        }
        case 3: {
            std::cout << "Results: ";
            for (const auto& product : products) {
                std::cout << "\"" << product.first << "\" ";
            }
            std::cout << std::endl;
            std::cout << "Select a product title: ";
            std::string selectedTitle;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, selectedTitle);
            while (products.find(selectedTitle) == products.end()) {
                std::cout << "Invalid Product. Please choose from the above." << std::endl;
                std::getline(std::cin, selectedTitle);
            }
            products[selectedTitle]->displayProduct();
            break;
        }
        default: {
            std::cout << "Invalid Option." << std::endl;
            break;
        }
    }
}

void Customer::addProductToCart(std::map<std::string, Product*>& products) {
    std::string title;
    int quantity;

    std::cout << "Which product do you want to add? ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    std::cout << "Enter quantity: ";
    std::cin >> quantity;

    bool productFound = false;
    auto product = products.find(title);
    if (product != products.end()) {
        productFound = true;
        if (product->second->amount >= quantity) {
            cart.addProduct(*product->second, quantity);
        } else {
            if (product->second->amount == 0) {
                std::cout << "Product out of stock!" << std::endl;
            } else {
                std::cout << "Not enough available, add " << product->second->amount << " to cart instead? (y/n)" << std::endl;
                std::string answer;
                std::cin >> answer;
                if (answer == "y") {
                    cart.addProduct(*product->second, product->second->amount);
                    productFound = true;
                }
                else {
                    return;
                }
            }
            
        }
    }

    if (!productFound) {
        std::cout << "Product not found!" << std::endl;
    }
}

void Customer::removeProductFromCart(std::map<std::string, Product*>& products) {
    std::string title;
    std::cout << "Which product do you want to remove from your cart? ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    if (products.find(title) != products.end()) {
        cart.removeProduct(*products[title]);
    } else {
        std::cout << "Product does not exist!" << std::endl;
    }
}

void Customer::viewCart() {
    std::cout << cart;
}

void Customer::updateProductInCart(std::map<std::string, Product*>& products) {
    std::string title;
    int quantity;

    std::cout << "Which product do you want to update? ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    std::cout << "Enter new quantity: ";
    std::cin >> quantity;

    bool productFound = false;
    auto product = products.find(title);
    if (product != products.end()) {
        productFound = true;
        if (quantity == 0) {
            cart.removeProduct(*product->second);
        } else {
            if (product->second->amount >= quantity) {
                cart.updateProduct(*product->second, quantity);
            } else {
                std::cout << "Not enough available" << std::endl;
            }
        }
    }

    if (!productFound) {
        std::cout << "Product not found in cart!" << std::endl;
    }
}

void Customer::completeOrder(){
    std::ofstream historyFile(historyFileName, std::ios::app);
    if (!historyFile.is_open()) {
        std::cerr << "Error opening history file." << std::endl;
        return;
    }
    historyFile << cart;
    historyFile.close();
    
    cart.clearCart();    
    std::cout << "Order completed!" << std::endl;
}

void Customer::viewOrderHistory(){
    std::ifstream historyFile(historyFileName);
    if (!historyFile.is_open()) {
        std::cerr << "Error opening history file." << std::endl;
        return;
    }
    std::string line;
    while (std::getline(historyFile, line)) {
        std::cout << line << std::endl;
    }
    historyFile.close();
}

bool Customer::executeCommand(std::map<std::string, Product*>& products, CategoryManager& categories) {
    int choice;
    std::cin >> choice;
    while (choice < 1 || choice > 8) {
        std::cout << "Invalid Option. Please enter a number between 1 and 8: ";
        std::cin >> choice;
    }
    switch(choice){
        case 1: {
            searchProduct(products, categories);
            break;
        }
        case 2: {
            addProductToCart(products);
            break;
        }
        case 3: {
            updateProductInCart(products);
            break;
        }
        case 4: {
            removeProductFromCart(products);
            break;
        }
        case 5: {
            completeOrder();
            break;
        }
        case 6: {
            viewOrderHistory();
            break;
        }
        case 7: {
            viewCart();
            break;
        }
        case 8: {
            return false;
        }
        default: {
            std::cout << "Invalid Option." << std::endl;
            break;
        }
    }
    return true;
}