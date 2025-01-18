#include "../include/Customer.h"
#include "../include/Category.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>


Customer::Customer(const std::string& username, const std::string& password, DiscountStats discountStats) : User(username, password, false), discountStats(discountStats) {}

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
    discountStats.printDiscount(currDiscount);
    std::cout << "Enter your choice: ";
} 

void Customer::searchProduct(ProductManager products, CategoryManager& categories) {
    std::cout << "1. Search for a specific product (by title)." << std::endl;
    std::cout << "2. View the products of a specific category." << std::endl;
    std::cout << "3. Show all the available products." << std::endl;
    std::cout << "Enter your choice: ";
    int choice = readInt();
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
        }
        case 3: {
            std::cout << "Results: ";
            products.displayProducts();
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

void Customer::addProductToCart(ProductManager& products) {
    std::string title;
    int quantity;

    std::cout << "Which product do you want to add? ";
    title = readString();
    std::cout << "Enter quantity: ";
    quantity = readInt();
    while (quantity <= 0) {
        std::cout << "Invalid quantity. Please enter a valid quantity: ";   
        quantity = readInt();
    }

    bool productFound = false;
    auto product = products.findProduct(title);
    if (product == nullptr) {
        std::cout << "Product not found!" << std::endl;
        return;
    }
    if (product->getAmount() >= quantity) {
        cart.addProduct(product, quantity);
    } else {
        if (product->getAmount() == 0) {
            std::cout << "Product out of stock!" << std::endl;
        } else {
            cart.addProduct(product, product->getAmount());
            std::cout << "Not enough available, added " << product->getAmount() << " to cart instead." << std::endl;
        }
        
    }
}

void Customer::removeProductFromCart(ProductManager& products) {
    std::string title;
    std::cout << "Which product do you want to remove from your cart? ";
    title = readString();
    auto product = products.findProduct(title);
    if (product == nullptr) {
        std::cout << "Product not found!" << std::endl;
        return;
    }
    if (!cart.removeProduct(product)) {
        std::cout << "Product not found in cart!" << std::endl;
    }
    std::cout << "Product removed from cart!" << std::endl;
}

void Customer::viewCart() {
    std::cout << cart;
}

void Customer::updateProductInCart(ProductManager& products) {
    std::string title;
    int quantity;

    std::cout << "Which product do you want to update? ";
    title = readString();
    auto product = products.findProduct(title);
    if (product == nullptr) {
        std::cout << "Product not found!" << std::endl;
        return;
    }
    if (!cart.isInCart(product)) {
        std::cout << "Product not found in cart!" << std::endl;
        return;
    }
    std::cout << "Enter new quantity: ";
    quantity = readInt();
    if (quantity == 0) {
        cart.removeProduct(product);
        return;
    }
    if (product->getAmount() >= quantity) {
        cart.updateProduct(product, quantity);
    } else {
        if (product->getAmount() == 0) {
            std::cout << "Product out of stock!" << std::endl;
        } else {
            std::cout << "Not enough available, added " << product->getAmount() << " to cart instead." << std::endl;
            cart.updateProduct(product, product->getAmount());

        }   
    }
}

void Customer::completeOrder(CategoryManager& categories){
    std::ofstream historyFile("files/order_history/" + username + "_history.txt", std::ios::app);   
    if (!historyFile.is_open()) {
        std::cerr << "Error opening history file." << std::endl;
        return;
    }

    if (currDiscount.product != nullptr and cart.isInCart(currDiscount.product)) {
        cart.applyDiscount(currDiscount.product, currDiscount.multiplier);
        if (currDiscount.multiplier == 0.6) {
            hasUsedLoyaltyDiscount = true;
        }
    }
    for (const auto& item : cart.items) {
        auto &product = item.first;
        auto &quantity = item.second;
        discountStats.updateProductStats(product, quantity);
    }


    if (discountStats.ordersCompleted!=0) historyFile << std::endl << std::endl;
    cart.saveToFile(historyFile, ++discountStats.ordersCompleted);
    historyFile.close();
    cart.checkout();  
    discountStats.nextCart();
    updateCurrentDiscount(categories);
    std::cout << "Order completed!" << std::endl;
}

void Customer::viewOrderHistory(){
    std::ifstream historyFile("files/order_history/" + username + "_history.txt");
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

bool Customer::getHasUsedLoyaltyDiscount() const {
    return hasUsedLoyaltyDiscount;
}

void Customer::setHasUsedLoyaltyDiscount(bool hasUsedLoyaltyDiscount) {
    this->hasUsedLoyaltyDiscount = hasUsedLoyaltyDiscount;
}

void Customer::updateCurrentDiscount(CategoryManager& categories) {
    currDiscount = discountStats.getDiscount(categories, hasUsedLoyaltyDiscount);
}
bool Customer::executeCommand(ProductManager& products, CategoryManager& categories) {
    int choice = readInt();
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
            completeOrder(categories);
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