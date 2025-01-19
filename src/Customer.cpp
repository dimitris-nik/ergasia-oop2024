#include "../include/Customer.h"
#include "../include/Category.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

Customer::Customer(const string& username, const string& password, DiscountStats discountStats) : User(username, password, false), discountStats(discountStats) {}

void Customer::displayMenu() {
    cout << endl;
    cout << "---Customer Menu---" << endl;
    cout << "1. Search for a product" << endl;
    cout << "2. Add product to cart" << endl;
    cout << "3. Update product from cart" << endl;
    cout << "4. Remove product from cart" << endl;
    cout << "5. Complete order" << endl;
    cout << "6. View order history" << endl;
    cout << "7. View cart" << endl;
    cout << "8. Exit" << endl;
    discountStats.printDiscount(currDiscount);
    cout << "Enter your choice: ";
} 

void Customer::searchProduct(ProductManager products, CategoryManager& categories) {
    cout << "1. Search for a specific product (by title)." << endl;
    cout << "2. View the products of a specific category." << endl;
    cout << "3. Show all the available products." << endl;
    cout << "Enter your choice: ";
    int choice = readInt();
    while (choice < 1 || choice > 3) {
        cout << "Invalid Option. Please enter a number between 1 and 3: ";
        choice = readInt();
    }
    switch(choice){
        case 1: {
            string titleSearch;
            cout << "Enter title to search: ";
            titleSearch = readString();
            if (products.findProduct(titleSearch)) {
                products.findProduct(titleSearch)->displayProduct();
            } else {
                cout << "Product not found." << endl;
            }
            break;
        }
        case 2: {
            string categorySearch;
            string subcategorySearch;
            cout << "Enter category to search: ";
            categorySearch = readString();
            auto searchCategory = categories.findCategory(categorySearch);
            while (!searchCategory) {
                cout << "Invalid Category. Please choose from the above." << endl;
                categorySearch = readString();
                searchCategory = categories.findCategory(categorySearch);
            }
            cout << "Enter subcategory to search (leave empty for all subcategories): ";
            subcategorySearch = readString();
            auto searchSubcategory = searchCategory->findSubcategory(subcategorySearch);
            while (!subcategorySearch.empty() && !searchSubcategory) {
                cout << "Invalid Subcategory. Please choose from the above, or leave empty." << endl;
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
            cout << "Results: ";
            products.displayProducts();
            cout << "Select a product title: ";
            string selectedTitle;
            selectedTitle = readString();
            while (!products.findProduct(selectedTitle)) {
                cout << "Invalid Product. Please choose from the above." << endl;
                selectedTitle = readString();
            }
            products.findProduct(selectedTitle)->displayProduct();
            break;
        }
        default: {
            cout << "Invalid Option." << endl;
            break;
        }
    }
}

void Customer::addProductToCart(ProductManager& products) {
    string title;
    int quantity;

    cout << "Which product do you want to add? ";
    title = readString();
    cout << "Enter quantity: ";
    quantity = readInt();
    while (quantity <= 0) {
        cout << "Invalid quantity. Please enter a valid quantity: ";   
        quantity = readInt();
    }
    auto product = products.findProduct(title);
    if (product == nullptr) {
        cout << "Product not found!" << endl;
        return;
    }
    if (product->getAmount() >= quantity) {
        cart.addProduct(product, quantity);
    } else {
        if (product->getAmount() == 0) {
            cout << "Product out of stock!" << endl;
        } else {
            cart.addProduct(product, product->getAmount());
            cout << "Not enough available, added " << product->getAmount() << " to cart instead." << endl;
        }
        
    }
}

void Customer::removeProductFromCart(ProductManager& products) {
    string title;
    cout << "Which product do you want to remove from your cart? ";
    title = readString();
    auto product = products.findProduct(title);
    if (product == nullptr) {
        cout << "Product not found!" << endl;
        return;
    }
    if (!cart.removeProduct(product)) {
        cout << "Product not found in cart!" << endl;
        return;
    }
    cout << "Product removed from cart!" << endl;
}

void Customer::viewCart() {
    cout << cart;
}

void Customer::updateProductInCart(ProductManager& products) {
    string title;
    int quantity;

    cout << "Which product do you want to update? ";
    title = readString();
    auto product = products.findProduct(title);
    if (product == nullptr) {
        cout << "Product not found!" << endl;
        return;
    }
    if (!cart.isInCart(product)) {
        cout << "Product not found in cart!" << endl;
        return;
    }
    cout << "Enter new quantity: ";
    quantity = readInt();
    if (quantity == 0) {
        cart.removeProduct(product);
        return;
    }
    if (product->getAmount() >= quantity) {
        cart.updateProduct(product, quantity);
    } else {
        if (product->getAmount() == 0) {
            cout << "Product out of stock!" << endl;
        } else {
            cout << "Not enough available, added " << product->getAmount() << " to cart instead." << endl;
            cart.updateProduct(product, product->getAmount());

        }   
    }
}

void Customer::completeOrder(CategoryManager& categories){
    ofstream historyFile("files/order_history/" + username + "_history.txt", ios::app);   
    if (!historyFile.is_open()) {
        cerr << "Error opening history file." << endl;
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


    if (discountStats.ordersCompleted!=0) historyFile << endl << endl;
    cart.saveToFile(historyFile, ++discountStats.ordersCompleted);
    historyFile.close();
    cart.checkout();  
    discountStats.nextCart();
    updateCurrentDiscount(categories);
    cout << "Order completed!" << endl;
}

void Customer::viewOrderHistory(){
    ifstream historyFile("files/order_history/" + username + "_history.txt");
    if (!historyFile.is_open()) {
        cerr << "Error opening history file." << endl;
        return;
    }
    string line;
    while (getline(historyFile, line)) {
        cout << line << endl;
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
            cout << "Invalid Option." << endl;
            break;
        }
    }
    return true;
}