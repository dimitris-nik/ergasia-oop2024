#include "../include/Eshop.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

    string categoriesFile;
    string productsFile;
    string usersFile;
    string discountsFile;
    string loyalDiscounts;

    if (argc > 1) categoriesFile = argv[1];
    else categoriesFile = "files/categories.txt";
    if (argc > 2) productsFile = argv[2];
    else productsFile = "files/products.txt";
    if (argc > 3) usersFile = argv[3];
    else usersFile = "files/users.txt";
    if (argc > 4) discountsFile = argv[4];
    else discountsFile = "files/discounts.txt";
    if (argc > 5) loyalDiscounts = argv[5];
    else loyalDiscounts = "files/loyal_discounts.txt";


    Eshop eshop(categoriesFile, productsFile, usersFile, discountsFile, loyalDiscounts);
    eshop.run();
    return 0;
}