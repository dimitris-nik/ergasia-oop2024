#include "../include/Customer.h"
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
void Customer::searchProduct(const std::vector<Product>& products) {
    int choice;
    std::string titleSearch, categorySearch, subcategorySearch;

    std::cout << "Product Search Options:" << std::endl;
    std::cout << "1. Search for a specific product (by title)." << std::endl;
    std::cout << "2. View the products of a specific category." << std::endl;
    std::cout << "3. Show all the available products." << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
        case 1: {
            std::cout << "Enter title to search: ";
            std::getline(std::cin, titleSearch);

            for (const auto& product : products) {
                 if (product.title.find(titleSearch) != std::string::npos) {
                    product.displayProduct();
                    std::cout << std::endl;
                }
            }
            break;
        }
        case 2: {

            std::cout << "Enter category to search: ";
            std::getline(std::cin, categorySearch);
            std::cout << "Enter subcategory to search (leave empty for all subcategories): ";
            std::getline(std::cin, subcategorySearch);
            
            for (const auto& product : products) {
               if (product.category == categorySearch && (subcategorySearch.empty() || product.subcategory == subcategorySearch)) {
                  product.displayProduct();
                    std::cout << std::endl;
               }
            }
            break;
        }
        case 3: {
            std::cout << "Results: ";
            for (const auto& product : products) {
                std::cout << "\"" << product.title << "\" ";
            }
            std::cout << std::endl;
            
            std::string selectedTitle;
            std::cout << "Select a product title: ";
            std::getline(std::cin, selectedTitle);

            for(const auto& product: products){
                if(product.title == selectedTitle) {
                     product.displayProduct();
                    break;
                }
            }
            break;
        }
        default:
            std::cout << "Invalid option." << std::endl;
    }
}
void Customer::addProductToCart(std::vector<Product>& products) {
    std::string title;
    int quantity;

    std::cout << "Which product do you want to add? ";
     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    std::cout << "Enter quantity: ";
    std::cin >> quantity;

    bool productFound = false;
        for (auto& product : products) {
        if (product.title == title) {
             if (product.amount >= quantity) {
                cart.addProduct(product, quantity);
                productFound = true;
                
            } else {
                 if(product.amount == 0){
                    std::cout << "Product out of stock!" << std::endl;
                 } else{
                     std::cout << "Not enough available, added " << product.amount << " to cart." << std::endl;
                    cart.addProduct(product, product.amount);
                 }
                 productFound = true;
            }
             break;
           
        }
    }

    if (!productFound) {
         std::cout << "Product not found!" << std::endl;
    }
}
void Customer::updateProductInCart(std::vector<Product>& products){
     std::string title;
    int quantity;

    std::cout << "Which product do you want to update? ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    std::cout << "Enter new quantity: ";
    std::cin >> quantity;

    bool productFound = false;
        for (auto& product : products) {
        if (product.title == title) {
               if (product.amount >= quantity) {
                cart.updateProduct(product, quantity);
                productFound = true;
            } else {
                std::cout << "Not enough available" << std::endl;
             }
            break;
        }
    }
     if (!productFound) {
         std::cout << "Product not found in cart!" << std::endl;
    }
}
 void Customer::removeProductFromCart(){
        std::string title;
        std::cout << "Which product do you want to remove from your cart? ";
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         std::getline(std::cin,title);

    cart.removeProduct(title);

}
void Customer::completeOrder(std::vector<Product>& products) {
    if (cart.items.empty()) {
        std::cout << "Cart is empty, cannot complete order." << std::endl;
        return;
    }
      
    std::ofstream historyFile(historyFileName, std::ios::app);
    if (historyFile.is_open()) {
        static int orderCount = 1;
        
        historyFile << "---CART " << orderCount++ << " START---" << std::endl;
           double totalCost = 0;
          for (const auto& item : cart.items) {
             historyFile << item.second << " " << item.first.title << std::endl;
             totalCost += item.first.price * item.second;
              for (auto& product : products) {
                if(product.title == item.first.title){
                    product.amount -= item.second;
                }
            }
         }
        historyFile << "---CART " << orderCount - 1 << " END---" << std::endl;
        historyFile << "Total Cost: " << std::fixed << std::setprecision(2) << totalCost << std::endl << std::endl;
        historyFile.close();
         std::cout << "Order Completed!" << std::endl;
        cart.items.clear();

    } else {
        std::cout << "Unable to open order history file." << std::endl;
    }
}

void Customer::viewOrderHistory() {
    std::ifstream historyFile(historyFileName);
    if (historyFile.is_open()) {
        std::string line;
        while (std::getline(historyFile, line)) {
            std::cout << line << std::endl;
        }
        historyFile.close();
    } else {
        std::cout << "No order history found." << std::endl;
    }
}
void Customer::viewCart(){
    cart.displayCart();
}