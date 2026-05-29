#include "../include/User.h"
#include <iostream>

User::User() : id(0), role(UserRole::USER) {}

User::User(int id, const std::string& username, const std::string& password,
           const std::string& email, UserRole role)
    : id(id), username(username), password(password), email(email), role(role) {}

int User::getId() const { return id; }
std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }
std::string User::getEmail() const { return email; }
UserRole User::getRole() const { return role; }

std::string User::getRoleString() const {
    return (role == UserRole::ADMIN) ? "ADMIN" : "USER";
}

void User::setId(int id) { this->id = id; }
void User::setUsername(const std::string& username) { this->username = username; }
void User::setPassword(const std::string& password) { this->password = password; }
void User::setEmail(const std::string& email) { this->email = email; }
void User::setRole(UserRole role) { this->role = role; }

bool User::authenticate(const std::string& password) const {
    return this->password == password;
}

bool User::save() {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        sqlite3* con = db->getConnection();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "INSERT INTO users (username, password, email, role) VALUES (?, ?, ?, ?)"
        );
        
        if (!stmt) return false;
        
        std::string roleStr = (role == UserRole::ADMIN) ? "ADMIN" : "USER";
        
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, roleStr.c_str(), -1, SQLITE_STATIC);
        
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        
        if (rc == SQLITE_DONE) {
            id = sqlite3_last_insert_rowid(con);
            return true;
        }
        
        return false;
    } catch (...) {
        std::cerr << "Error saving user" << std::endl;
        return false;
    }
}

bool User::update() {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        std::string roleStr = (role == UserRole::ADMIN) ? "ADMIN" : "USER";
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "UPDATE users SET password = ?, email = ?, role = ? WHERE id = ?"
        );
        
        if (!stmt) return false;
        
        sqlite3_bind_text(stmt, 1, password.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, roleStr.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, id);
        
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        
        return rc == SQLITE_DONE;
    } catch (...) {
        std::cerr << "Error updating user" << std::endl;
        return false;
    }
}

User* User::findByUsername(const std::string& username) {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "SELECT * FROM users WHERE username = ?"
        );
        
        if (!stmt) return nullptr;
        
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        
        User* user = nullptr;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string roleStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            UserRole role = (roleStr == "ADMIN") ? UserRole::ADMIN : UserRole::USER;
            
            user = new User(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
                role
            );
        }
        
        sqlite3_finalize(stmt);
        return user;
    } catch (...) {
        std::cerr << "Error finding user by username" << std::endl;
        return nullptr;
    }
}

User* User::findById(int id) {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "SELECT * FROM users WHERE id = ?"
        );
        
        if (!stmt) return nullptr;
        
        sqlite3_bind_int(stmt, 1, id);
        
        User* user = nullptr;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string roleStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            UserRole role = (roleStr == "ADMIN") ? UserRole::ADMIN : UserRole::USER;
            
            user = new User(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
                role
            );
        }
        
        sqlite3_finalize(stmt);
        return user;
    } catch (...) {
        std::cerr << "Error finding user by ID" << std::endl;
        return nullptr;
    }
}

bool User::usernameExists(const std::string& username) {
    User* user = findByUsername(username);
    bool exists = (user != nullptr);
    delete user;
    return exists;
}

bool User::emailExists(const std::string& email) {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "SELECT * FROM users WHERE email = ?"
        );
        
        if (!stmt) return false;
        
        sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);
        
        bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        
        return exists;
    } catch (...) {
        return false;
    }
}

bool User::isAdmin() const {
    return role == UserRole::ADMIN;
}
