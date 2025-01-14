#include "../include/Eshop.h"
#include <iostream>

int main(int argc, char* argv[]) {

    std::string categoriesFile;
    std::string productsFile;
    std::string usersFile;

    if (argc != 4) { //default files
        categoriesFile = "files/categories.txt";
        productsFile = "files/products.txt";
        usersFile = "files/users.txt";
    }
    else{
        categoriesFile = argv[1];
        productsFile = argv[2];
        usersFile = argv[3];
    }

    Eshop eshop(categoriesFile, productsFile, usersFile);
    eshop.run();
    return 0;
}