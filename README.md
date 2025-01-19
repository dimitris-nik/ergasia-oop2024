# sdi2300208 sdi2300139

# Eshop
Σε αυτήν την εργασία υλοποιήθηκε το Eshop όπως περιγράφεται στην εκφώνηση και σε συμφωνία με τα samples/unit tests. 

```c
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
Στην κλάση αποθηκεύονται ως protected τα απαραίτητα στοιχεία του Eshop (χρήστες, κατηγορίες, προϊόντα), καθώς ίσως θα θέλαμε σε μια μελοντική επέκταση να κληρονομείται σε άλλες κλάσεις (πχ Supermarket) και να έχουν πρόσβαση στα στοιχεία του Eshop.

Παρακάτω θα εξηγήσουμε την υλοποίηση κάθε κλάσης με λεπτομέρεια.

## User
```c
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

Επιπλέον έχει φτιαχτεί μία κλάση `UserManager`, αυτή που χρησιμοποιείται στο Eshop, η οποία αποθηκεύει σε ένα hashtable (unordered_map) όλους τους χρήστες, χρησιμοποιώντας ως κλειδί το username τους καθώς είναι μοναδικό.

Χρησιμοποιούνται μεθόδοι add, remove και find User για την διαχείριση της βάσης χρηστών από το Eshop. 

Η μέθοδος saveUsers αποθηκεύει όλους τους χρήστες στο αρχείο που παρέχεται από το eshop (και μία πληροφορία απαραίτητη για τα discounts σε ένα άλλο αρχείο, θα επιστρέψουμε σε αυτό αργότερα).

### Admin και Customer
```c
class Admin : public User {
    void addProduct(ProductManager& products, CategoryManager& categories);
    void editProduct(ProductManager& products, CategoryManager& categories);
    void removeProduct(ProductManager& products, CategoryManager& categories);
    void searchProduct(ProductManager& products, CategoryManager& categories);    
public:
    Admin(std::string username, std::string password);
    void displayMenu();
    bool executeCommand(ProductManager& products, CategoryManager& categories);
};

class Customer : public User {
    Cart cart;
    DiscountStats discountStats;
    bool hasUsedLoyaltyDiscount = false;
    discount currDiscount = {nullptr, 1.0};
    void searchProduct(ProductManager products, CategoryManager& categories);
    void addProductToCart(ProductManager& products);
    void updateProductInCart(ProductManager& products);
    void removeProductFromCart(ProductManager& products);
    void completeOrder(CategoryManager& categories);
    void viewOrderHistory();
    void viewCart();
public:
    Customer(const std::string& username, const std::string& password, DiscountStats productStats);
    void displayMenu();
    bool executeCommand(ProductManager& products, CategoryManager& categories);
    void updateCurrentDiscount(CategoryManager& categories);
    bool getHasUsedLoyaltyDiscount() const;
    void setHasUsedLoyaltyDiscount(bool hasUsedLoyaltyDiscount);
};

```
Οι κλάσεις Admin και Customer αποτελούν επέκταση του User, και υλοποιούνται οι πολυμορφικές μεθόδοι displayMenu και executeCommand.

Η executeCommand χρησιμοποιεί τις εσωτερικές μεθόδους της κάθε κλάσης για να τρέξει τα κατάλληλα commands. Επίσης όπως φαίνεται και στον κώδικα, τόσο η executeCommand όσο και οι εσωτερικές μεθόδοι διαχειρίζονται το User input, δηλαδη το τι command θα επιλέξει ο χρήστης, τα πεδία που ζητούνται όταν πχ ένας admin προσθέτει ένα προϊόν, κ.ο.κ.

Ο customer επίσης περιέχει μερικά πεδία που χρησιμοποιούνται για τα discounts, που θα εξηγήσουμε ξεχωριστά στο BONUS κομμάτι του README.

## Product
```c
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

Το cart είναι μία κλάση που αποτελεί πεδίο του customer. Περιέχει ένα map από προιόντα σε μία ποσότητα στο καλάθι. Η χρήση Product pointers κάνει εύκολη την καθολική ενημέρωση του stock των προϊόντων σε περίπτωση checkout καθώς και την διατήρηση των προϊόντων στο καλάθι του χρήστη σε περίπτωση επεξεργασίας τους (πχ σε μια μελοντική επέκταση του προγράμματος που υποστήριζε ταυτόχρονη σύνδεση αν ένας admin άλλαζε το όνομα ενός προϊόντος την ώρα που ήταν στο καλάθι, θα ενημερωθεί σωστά και στο καλάθι).


## Category
```c
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
        Category* addCategory(const std::string & category);
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

Επιπλέον έχει φτιαχτεί μία κλάση `CategoryManager`, αυτή που χρησιμοποιείται στο Eshop, η οποία αποτελεί κάτι σαν το "μηδενικό επίπεδο" των κατηγοριών. Η `addProduct` του CategoryManager αποτελεί μια πιο implementation-specific προσέγγιση καθώς δέχεται ως arguements μόνο την κατηγορία και την υποκατηγορία καθώς μέχρι εκεί φτάνουν τα προϊόντα στα παραδείγματα. Ωστόσο η δομή της κλάσης Category επιτρέπει ορισμό αναδρομικών συναρτήσεων που αναζητούν προϊόντα μεχρι και το τελευταίο επίπεδο με αλγορίμους όπως ο DFS.
 