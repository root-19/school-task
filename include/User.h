#ifndef USER_H
#define USER_H

#include <string>
#include "DatabaseConnection.h"

enum class UserRole {
    USER,
    ADMIN
};

class User {
private:
    int id;
    std::string username;
    std::string password;
    std::string email;
    UserRole role;
    
public:
    User();
    User(int id, const std::string& username, const std::string& password, 
         const std::string& email, UserRole role = UserRole::USER);
    
    // Getters
    int getId() const;
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getEmail() const;
    UserRole getRole() const;
    std::string getRoleString() const;
    
    // Setters
    void setId(int id);
    void setUsername(const std::string& username);
    void setPassword(const std::string& password);
    void setEmail(const std::string& email);
    void setRole(UserRole role);
    
    // Authentication
    bool authenticate(const std::string& password) const;
    
    // CRUD Operations
    bool save();
    bool update();
    static User* findByUsername(const std::string& username);
    static User* findById(int id);
    static bool usernameExists(const std::string& username);
    static bool emailExists(const std::string& email);
    bool isAdmin() const;
};

#endif
