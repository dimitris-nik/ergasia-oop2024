#include <string>
using namespace std;
class User {
    public:
        string username;
        string password;
        string userType;

        User(string username, string password, string userType);
        bool isAdmin();
        bool authenticate(string password_attempt);
};
