#ifndef AUTH_H
#define AUTH_H

#include "User.h"
#include <string>

class Auth {
private:
    static User* currentUser;
    
public:
    static bool registerUser(const std::string& username, const std::string& password, 
                            const std::string& email, UserRole role = UserRole::USER);
    static User* login(const std::string& username, const std::string& password);
    static void logout();
    static User* getCurrentUser();
    static bool isLoggedIn();
    static bool isAdmin();
};

#endif // AUTH_H
