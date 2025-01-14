#include "../include/Eshop.h"
#include <iostream>

int main(int argc, char* argv[]) {

    std::string categoriesFile;
    std::string productsFile;
    std::string usersFile;

    if (argc != 4) { //default files
        std::cerr << "Usage: ./oop24 <categories_file> <products_file> <users_file>" << std::endl;
        return 1;
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