# sdi2300208 sdi2300139
## ΣΗΜΑΝΤΙΚΟ: το repository ονομάστηκε εκ παραδρομής `silly-little-guys` καθώς είναι το όνομα της ομάδας που χρησιμοποιούσαμε στις ομαδικές εργασίες του παρελθόντος και δεν διαβάσαμε την εκφώνηση εγκαίρως. Τα sdi μας είναι: `sdi2300208` και `sdi2300139`. Μας συγχωρείτε για την αναστάτωση.

Στο παρακάτω README θα εξηγηθούν οι σχεδιαστικές επιλογές και η δομή των κλάσεων. 

Γενικά, τα ονόματα των μεθόδων είναι αρκετά περιγραφικά της λειτουργίας τους, και πιστεύουμε πως δεν υπάρχουν ασάφειες στην υλοποίησή τους, οπότε η ανάλυσή τους στο README είναι περιττή.

Για παράδειγμα, είναι προφανές ότι μια συνάρτηση `removeProductFromCart` με την παρακάτω υλοποίηση:
```c++
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
```
Δεν χρειάζεται περαιτέρω ανάλυση στο readme.

Παρ'όλ'αυτά, ο κώδικας είναι εμπλουτισμένος με σχόλια σε σημεία που κρίθηκε ότι χρειάζεται. 

# Eshop

Σε αυτήν την εργασία υλοποιήθηκε το Eshop όπως περιγράφεται στην εκφώνηση και σε συμφωνία με τα samples/unit tests. 

```c++
class Eshop {
protected:
    UserManager users;
    ProductManager products;
    CategoryManager categories;
    const std::string& categoriesFile;
    const std::string& productsFile;
    const std::string& usersFile;
    const std::string& discountsFile;
    const std::string& loyalDiscounts;
    User* login();
    User* registers();
    void loadUsers();
    void loadProducts();
    void loadCategories();
    void loadHistories();
    void saveChanges();
public:
    Eshop(const std::string& categoriesFile, const std::string& productsFile, const std::string& usersFile, const std::string& discountsFile, const std::string& loyalDiscounts);
    ~Eshop();
    void run();
};
```
O constructor της Eshop δέχεται ως παράμετρους τα απαραίτητα αρχεία για την λειτουργία του Eshop.
Ωστόσο επειδή στα samples δέχεται μόνο 3 αρχεία και αγνοεί τα discounts, στην main γίνεται default ανάθεση του discountsFile σε "discounts.txt" και του loyalDiscounts σε "loyalDiscounts.txt", αν δεν δοθούν ως ορίσματα στην κλήση του προγράμματος.
```c++
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
```
Στην κλάση αποθηκεύονται ως protected τα απαραίτητα στοιχεία του Eshop (χρήστες, κατηγορίες, προϊόντα), καθώς ίσως θα θέλαμε σε μια μελοντική επέκταση να κληρονομείται σε άλλες κλάσεις (πχ Supermarket) και να έχουν πρόσβαση στα στοιχεία του Eshop.

Παρακάτω θα εξηγήσουμε την υλοποίηση κάθε κλάσης με λεπτομέρεια.

## User
```c++
class User {
protected:
    std::string username;
    std::string password;
    bool isAdmin;
public:
    User(std::string username, std::string password, bool isAdmin);
    virtual ~User() = default;
    std::string getUsername() const;
    virtual void displayMenu() = 0; // Pure virtual function for menu display
    virtual bool executeCommand(ProductManager& products, CategoryManager& categories) = 0; // Pure virtual function to execute user commands
    bool checkPassword(std::string& password) const; //for login
    void searchProduct(ProductManager& products, CategoryManager& categories);
    friend std::ostream& operator<<(std::ostream& os, const User& user);
};

class UserManager {
    std::unordered_map<std::string, User*> users;
public:
    ~UserManager();
    void addUser(User* user);
    void removeUser(const std::string& username);
    User* findUser(const std::string& username) const;
    void saveUsers(const std::string& usersFile, const std::string& loyalDiscounts) const;
};
```
Στην κλάση `User`, πέρα από το να αποθηκεύει τα βασικά αναγνωριστικά στοιχεία του χρήστη, χρησιμοποιούνται pure virtual functions (displaymenu και executeCommand) προκειμένου να διαφοροποιήσουμε την λειτουργία της κάθε μίας για τους Admin και τους Customer.

Ωστόσο η μέθοδος searchProduct ορίζεται και υλοποιείται στο User καθώς είναι πανομοιότυπη και για τους 2 τύπους χρηστών. Οι κλάσεις User και Customer την κληρονομούν και την χρησιμοποιούν.

Επιπλέον έχει φτιαχτεί μία κλάση `UserManager`, αυτή που χρησιμοποιείται στο Eshop, η οποία αποθηκεύει σε ένα hashtable (unordered_map) όλους τους χρήστες, χρησιμοποιώντας ως κλειδί το username τους καθώς είναι μοναδικό.

Χρησιμοποιούνται μεθόδοι add, remove και find User για την διαχείριση της βάσης χρηστών από το Eshop. 

Η μέθοδος saveUsers αποθηκεύει όλους τους χρήστες στο αρχείο που παρέχεται από το eshop (και μία πληροφορία απαραίτητη για τα discounts σε ένα άλλο αρχείο, θα επιστρέψουμε σε αυτό αργότερα).

### Admin και Customer
```c++
class Admin : public User {
public:
    Admin(std::string username, std::string password);
    void displayMenu();
    bool executeCommand(ProductManager& products, CategoryManager& categories);
    void addProduct(ProductManager& products, CategoryManager& categories);
    void editProduct(ProductManager& products, CategoryManager& categories);
    void removeProduct(ProductManager& products, CategoryManager& categories);
};

class Customer : public User {
    Cart cart;
    DiscountStats discountStats;
    bool hasUsedLoyaltyDiscount = false;
    discount currDiscount = {nullptr, 1.0};
    
public:
    Customer(const std::string& username, const std::string& password, DiscountStats productStats);
    void displayMenu();
    bool executeCommand(ProductManager& products, CategoryManager& categories);
    void addProductToCart(ProductManager& products);
    void updateProductInCart(ProductManager& products);
    void removeProductFromCart(ProductManager& products);
    void completeOrder(CategoryManager& categories);
    void viewOrderHistory();
    void viewCart();
    void updateCurrentDiscount(CategoryManager& categories);
    bool getHasUsedLoyaltyDiscount() const;
    void setHasUsedLoyaltyDiscount(bool hasUsedLoyaltyDiscount);
};


```
Οι κλάσεις Admin και Customer αποτελούν επέκταση του User, και υλοποιούνται οι πολυμορφικές μεθόδοι displayMenu και executeCommand.

Η executeCommand χρησιμοποιεί τις ανάλογες μεθόδους της κάθε κλάσης για να τρέξει τα κατάλληλα commands. Επίσης όπως φαίνεται και στον κώδικα, τόσο η executeCommand όσο και οι μέθοδοι του κάθε command μεθόδοι διαχειρίζονται το User input, δηλαδη το τι command θα επιλέξει ο χρήστης, τα πεδία που ζητούνται όταν πχ ένας admin προσθέτει ένα προϊόν, κ.ο.κ.

Και στις 2 υλοποιήσεις του executeCommand, περνάμε με arguements references στα products και τα categories, διότι είναι απαραίτητα για τις λειτουργίες.

Ο customer επίσης περιέχει μερικά πεδία που χρησιμοποιούνται για τα discounts, που θα εξηγήσουμε ξεχωριστά στο BONUS κομμάτι του README.

## Product
```c++
class Product {
private:
    std::string title;
    std::string description;
    std::string category;
    std::string subcategory;
    double price;
    std::string measurementType;
    int amount;
    int appearedInCart = 0;
public:
    Product(std::string title, std::string description, std::string category, std::string subcategory, double price, std::string measurementType, int amount);
    void displayProduct() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getCategory() const;
    std::string getSubcategory() const;
    double getPrice() const;
    std::string getMeasurementType() const;
    int getAmount() const;
    int getAppearedInCart() const;
    void increaseAppearedInCart();
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setCategory(const std::string& category);
    void setSubcategory(const std::string& subcategory);
    void setPrice(double price);
    void setMeasurementType(const std::string& measurementType);
    void setAmount(int amount);
    friend std::ostream& operator<<(std::ostream& os, const Product& product);
};

class ProductManager {
std::unordered_map<std::string, Product*> products;
public:
    ~ProductManager();
    void addProduct(Product* product);
    void removeProduct(const std::string& title);
    void displayProducts() const;
    Product* findProduct(const std::string& title);
    void showUnavailableProducts() const;
    void saveProducts(const std::string& productsFile) const;
    void showTopProducts() const;
};
```

Στην κλάση `Product`, αποθηκεύονται τα βασικά αναγνωριστικά στοιχεία του κάθε προϊόντος.

Χρησιμοποιούνται getters και setters για όλα τα πεδία των προϊόντων. Επιπλέον έχει γίνει operator overloading στην κλάση Product προκειμένου να τυπώνουμε εύκολα ένα προϊόν.

Επιπλέον υπάρχει το πεδίο `int appearedInCart = 0;`, που χρησιμοποιείται για τα top products (προϊόντα που έχουν εμφανιστεί στις περισσότερες παραγγελίες)

Επιπλέον έχει φτιαχτεί μία κλάση `ProductManager`, αυτή που χρησιμοποιείται στο Eshop, η οποία αποθηκεύει σε ένα hashtable (unordered_map) όλα τα προϊίοντα, χρησιμοποιώντας ως κλειδί το όνομα των προϊόντων καθώς είναι μοναδικό. Το όνομα κάθε μεθόδου αυτής της κλάσης περιγράφει ικανοποιητικά την λειτουργία της. Η `saveProducts` είναι ανάλογη της `saveUsers`.



## Cart
```c++
class Cart {  
public:
    double totalCost = 0;
    std::unordered_map<Product*, int> items; // item, amount
    void addProduct(Product* product, int quantity);
    bool removeProduct(Product* product);
    bool updateProduct(Product* product, int quantity);
    bool isInCart(Product* product);
    void applyDiscount(Product* product, double discount);
    void checkout();
    void saveToFile(std::ostream& os, int number);
    friend std::ostream& operator<<(std::ostream& os, const Cart& cart);
};
```
Το cart είναι μία κλάση που αποτελεί πεδίο του customer. Περιέχει ένα map από προιόντα σε μία ποσότητα στο καλάθι. Η χρήση Product pointers κάνει εύκολη την καθολική ενημέρωση του stock των προϊόντων σε περίπτωση checkout καθώς και την διατήρηση των προϊόντων στο καλάθι του χρήστη σε περίπτωση επεξεργασίας τους (πχ σε μια μελοντική επέκταση του προγράμματος που υποστήριζε ταυτόχρονη σύνδεση αν ένας admin άλλαζε το όνομα ενός προϊόντος την ώρα που ήταν στο καλάθι, θα ενημερωθεί σωστά και στο καλάθι).


## Category
```c++
class Category {
    private:
        std::vector<Category*> subcategories;
        std::vector<Product*> products;
        int amountForDiscount;
    public:
        std::string name;
        Category(const std::string & name);
        ~Category();
        Category* addSubcategory(const std::string& subcategory);
        void displaySubcategories() const;
        void addProduct(Product* product);
        void removeProduct(Product* product);
        Product * generateRandomProduct();
        void displayProducts() const;
        int getAmountForDiscount() const;
        void setAmountForDiscount(int amount);
        Category* findSubcategory(const std::string& subcategory) const;
};

class CategoryManager {
    private:
        std::vector<Category*> categories;
    public:
        ~CategoryManager();
        void addCategory(Category* category);
        void displayCategories() const;
        Category* findCategory(const std::string& category) const;
        void addProduct(Product* product, const std::string& category, const std::string& subcategory);
        void removeProduct(Product* product);
};
```
Στην κλάση `Category`, αποθηκεύονται τα βασικά στοιχεία μιας κατηγορίας, δηλαδή το όνομα της, μια λίστα απο Product pointers που περιέχει τα προϊόντα της κατηγορίας, καθώς και μια λίστα απο Category pointers που περιέχει της υποκατηγορίες της κατηγορίας. Αυτος ο recursion-like ορισμός επιτρέπει την δημιουργία απεριόριστων επιπέδων από categories σε μελλοντική επέκταση.

Η ξεχωριστή λίστα απο προϊόντα σε κάθε επίπεδο διευκολύνει την λειτουργία της αναζήτησης προϊόντος ανά κατηγορία στους πελάτες και στους διαχειριστές, καθώς τυπώνει κατευθείαν όλα τα προϊόντα σε οποιαδήποτε κατηγορία απλά με την κλήση της `displayProducts`.

Μπορεί να κοστίζει λίγο σε μνήμη, καθώς αποθηκεύονται πολλά instances ενός `Product*` σε κάθε υπο-επίπεδο, αλλά αποτελεί **time-space tradeoff**.

Δεν χρησιμοποιούνται getters και setters για τα categories διότι προς το παρόν οι διαχειριστές δεν μπορούν να επεξεργαστούν κατηγορίες.

Επιπλέον υπάρχει το πεδίο `int amountForDiscount = 0;`, που είναι κομμάτι του bonus.

Επιπλέον έχει φτιαχτεί μία κλάση `CategoryManager`, αυτή που χρησιμοποιείται στο Eshop, η οποία αποτελεί κάτι σαν το "μηδενικό επίπεδο" των κατηγοριών. 

Η `addProduct` του CategoryManager αποτελεί μια πιο implementation-specific προσέγγιση καθώς δέχεται ως arguements μόνο την κατηγορία και την υποκατηγορία καθώς μέχρι εκεί φτάνουν τα προϊόντα στα παραδείγματα. Ωστόσο η δομή της κλάσης Category επιτρέπει ορισμό αναδρομικών συναρτήσεων που αναζητούν προϊόντα μεχρι και το τελευταίο επίπεδο υποκατηγορίας με αλγορίμους όπως ο DFS (για πολύ μεγάλα shops με πάρα πολλές κατηγορίες και υποκατηγορίες). 

# BONUS: DISCOUNTS
Για το bonus κομμάτι της εργασίας υλοποιήθηκε η κλάση DiscountStats:
```c++
typedef struct stats {
    int consecutiveOrders = 0; // max amount of times the product was found in the cart in a row
    int appearedInCart = 0; // total times the product was found in the cart (need this for favorite product)
    int foundInLastCart = false; // if the product was found in the last cart
} productStats;

typedef struct discount {
    Product* product;
    double multiplier;
} discount;

class DiscountStats {
    std::unordered_map<Product*, productStats> products_Stats;
    std::unordered_map<std::string, int> categoriesCounter;
    int ordersCompleted = 0;
    public: 
        void updateProductStats(Product* product, int quantity);
        void nextCart();
        discount getDiscount(CategoryManager& categories, int hasUsedLoyaltyDiscount);
        void incrementOrdersCompleted();
        int getOrdersCompleted() const;
        void printDiscount(discount discount);
};
```
Κάθε αντικείμενο `DiscountStats` περιέχι ένα map (`products_Stats`) από `Product*` σε ένα struct, το οποίο περιέχει κατάλληλα πεδία ώστε να υλοποιηθούν οι λειτουργίες που περιγράφονται, όπως φανερώνουν τα comments.

Η μέθοδος `updateProductStats` περιέχει λογική που ενημερώνει κατάλληλα την κάθε καταχώρηση του `products_Stats`, δεδομένου ότι το προϊόν μόλις εμφανίστηκε στο καλάθι (βλ κώδικα). 
Η `updateProductStats` λειτουργεί με την υπόθεση ότι κάθε διαδοχική της κλήση πρόκειται για προϊόν του ίδιου καλαθιού. Με την μέθοδος `nextCart` δηλώνουμε ότι το τρέχων καλάθι τελείωσε και προχωράμε στο επόμενο, με την ανάλογη ενημέρωση των πεδίων στο `products_Stats`

Επιπλέον ορίστηκε ο τύπος `discount`, ο οποίος πρόκειται απλά για ένα struct που περιέχει ένα `Product*` και το αντίστοιχο discount multiplier (πχ για έκπτωση 20% έχουμε multiplier = 0.8)

Η `DiscountStats` επίσης περιέχει έναν μετρητή `ordersCompleted` που αυξάνεται με κάθε νέα παραγγελία του χρήστη, απαραίτητη για την έκπτωση 40% στις 5 παραγγελίες, με τους αντίστοιχες getters και setters.

Η `getDiscount` είναι η μέθοδος που, πρώτα κάνει generate όλες τις διαθέσιμες εκπτώσεις, και έπειτα επιστρέφει μία από αυτές στην τύχη, αν υπάρχει. Αν δεν υπάρχει επιστρέφει μία default εκπτωση 1.0 (δηλαδή καμία έκπτωση), και το `Product*` ως `nullptr`. 
Δέχεται ως arguement τον `hasUsedLoyaltyDiscount` που είναι ένα bool που δηλώνει αν ο χρήστης έχει ήδη χρησιμοποιήσει το loyalty discount (αποθηκεύεται στον Customer), καθώς και ένα reference στο `CategoryManager` γιατί εκεί αποθηκεύεται η πληροφορία `amountForDiscount` η οποία δηλώνει το πόσα προϊόντα ίδιας κατηγορίας πρέπει να αγοράσει για την έκπτωση 30% σε τυχαίο προϊόν της ίδιας κατηγορίας.
Επιπλέον, υλοποιήθηκε η μέθοδος `generateRandomProduct` στην κλάση `Category`, που επιστρέφει ένα τυχαίο προϊόν από την κατηγορία.

```c++
Product* Category::generateRandomProduct(){
    if (products.empty()) {
        return nullptr;
    }
    return products[rand() % products.size()];
}
```

Η κλάση Customer περιέχει το πεδίο `discount currDiscount = {nullptr, 1.0};` που αποθηκεύει την έκπτωση που δικαιούται αυτή τη στιγμή, και που θα εφαρμόσει στο καλάθι του στην επόμενη παραγγελία. Η μέθοδος `updateCurrentDiscount` ενημερώνει το πεδίο αυτό, και καλείται μία φορά στην αρχή όταν αρχικοποιείται ο Customer στο Eshop, και έπειτα μετά από κάθε παραγγελία.

Το σύστημα ενημερώνει τον χρήστη για την έκπτωση που δικαιούται, με την χρήση της `printDiscount`. Η printDiscount καλείται κάθε φορά στην displayMenu του Customer, ενημερώνοντάς τον ότι έχει διαθέσιμη έκπτωση, και ποια είναι αυτή (βλ. κώδικα).

Τέλος, ως αγαπημένο προϊόν ορίζεται αυτό που έχει εμφανιστεί τις περισσότερες φορές στο καλάθι του, ανεξαρτήτως ποσότητας. Αυτό γίνεται για να μην παίρνουν προτεραιότητα τα προϊόντα από τα οποία αγοράζουμε μεγάλη ποσότητα ούτως ή άλλως (πχ αν αγοράσει μήλα ή ντομάτες, τότε προφανώς η ποσότητα θα είναι πάντα μεγαλύτερη από αυτή που θα ήταν αν αγόραζε μπλούζες). Το προϊόν που εμφανίστηκε τις περισσότερες φορές στο καλάθι το υπολογίζει η μέθοδος `getDiscount`.

# utils και input
```c++
#pragma once
#include <string>
#include <algorithm>
#include <iostream>

std::string trim(const std::string& str);
int readInt();
std::string readString();
double readDouble();
```

Στα αρχεία `utils.h` και `utils.cpp` υλοποιούνται οι συναρτήσεις υπεύθυνες για την εισαγωγή δεδομένων από τον χρήστη. Χρησιμοποιείται πάντα getline για να δεχόμαστε οτιδήποτε (πχ για ακέραιο getline + stod) τόσο για να ελέγχουμε το validity του input όσο για μην ασχολούμαστε με ζητήματα του `std::cin` (πχ περισσευούμενο '\n' στον cin buffer).

Επιπλέον η συνάρτηση `trim()` αφαιρεί τα trailing και leading whitespaces από τα strings που χρειάζεται γιατί χρησιμοποιούνται ως κλειδιά στα maps και πρέπει να εξασφαλίζουμε την ομοιότητά τους στις εισαγωγές από τον χρήστη κλπ.