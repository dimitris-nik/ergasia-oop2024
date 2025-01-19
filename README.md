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


