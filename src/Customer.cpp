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

bool Customer::executeCommand(std::map<std::string, Product*>& products, CategoryManager& categories) {
    int choice;
    std::cin >> choice;
    while (choice < 1 || choice > 8) {
        std::cout << "Invalid Option. Please enter a number between 1 and 8: ";
        std::cin >> choice;
    }
    switch(choice){
        default: {
            std::cout << "Invalid Option." << std::endl;
            break;
        }
    }
    return true;
}