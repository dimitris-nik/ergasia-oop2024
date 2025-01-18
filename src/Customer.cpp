#include "../include/Customer.h"
#include "../include/Category.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>


Customer::Customer(const std::string& username, const std::string& password, int ordersCompleted, DiscountStats products_Stats) : User(username, password, false), ordersCompleted(ordersCompleted), products_Stats(products_Stats) {}

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

void Customer::searchProduct(ProductManager products, CategoryManager& categories) {
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
            products.displayProducts();
            std::cout << "Select a product title: ";
            std::string selectedTitle;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, selectedTitle);
            while (!products.findProduct(selectedTitle)) {
                std::cout << "Invalid Product. Please choose from the above." << std::endl;
                std::getline(std::cin, selectedTitle);
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
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    std::cout << "Enter quantity: ";
    std::cin >> quantity;

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
            std::cout << "Not enough available, add " << product->getAmount() << " to cart instead? (y/n)" << std::endl;
            std::string answer;
            std::cin >> answer;
            if (answer == "y") {
                cart.addProduct(product, product->getAmount());
                productFound = true;
            }
            else {
                return;
            }
        }
        
    }
}

void Customer::removeProductFromCart(ProductManager& products) {
    std::string title;
    std::cout << "Which product do you want to remove from your cart? ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
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
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    auto product = products.findProduct(title);
    if (product == nullptr) {
        std::cout << "Product not found!" << std::endl;
        return;
    }
    std::cout << "Enter new quantity: ";
    std::cin >> quantity;
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
    auto discount = products_Stats.getDiscount(categories, ordersCompleted, hasUsedLoyaltyDiscount);
    if (discount.first != nullptr) {
        cart.applyDiscount(discount.first, discount.second);
        if (discount.second == 0.8) {
            std::cout << "You bought " << discount.first->getTitle() << " 3 times in a row in your past orders, 20% discount applied! Can be used only in your next order, so don't miss out!" << std::endl;
        } else if (discount.second == 0.7) {
            std::cout << "You seem to like " << discount.first->getCategory() << " products, you have a 30% discount applied to " << discount.first->getTitle() << "!" << std::endl; 
        } else if (discount.second == 0.6) {
            std::cout << "You have completed 5 orders, 40% discount applied to your favorite product " << discount.first->getTitle() << "!" << std::endl;
            hasUsedLoyaltyDiscount = true;
        }
    }
    for (const auto& item : cart.items) {
        auto &product = item.first;
        auto &quantity = item.second;
        products_Stats.updateProductStats(product, quantity);
    }


    if (ordersCompleted!=0) historyFile << std::endl << std::endl;
    cart.saveToFile(historyFile, ++ordersCompleted);
    historyFile.close();
    cart.checkout();  
    products_Stats.nextCart();
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

bool Customer::executeCommand(ProductManager& products, CategoryManager& categories) {
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