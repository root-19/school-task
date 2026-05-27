/**
 * User Class Implementation
 * 
 * This file implements the User class for authentication and authorization.
 * It provides CRUD operations for user management with SQLite database.
 * 
 * Key Features:
 * - User registration with validation
 * - User login with password verification
 * - Role-based access control (USER/ADMIN)
 * - Database persistence with prepared statements
 * 
 * Database Table: users
 * - id: Primary key
 * - username: Unique login identifier
 * - password: User password (plain text for demo)
 * - email: Unique email address
 * - role: USER or ADMIN
 * 
 * Security Note: In production, passwords should be hashed using bcrypt or Argon2
 */

#include "../include/User.h"
#include <iostream>
#include <sstream>

// ============================================
// CONSTRUCTORS
// ============================================

// Default constructor - creates empty user with default role
User::User() : id(0), role(UserRole::USER) {}

// Parameterized constructor - used when loading from database
User::User(int id, const std::string& username, const std::string& password, 
           const std::string& email, UserRole role)
    : id(id), username(username), password(password), email(email), role(role) {}

// ============================================
// GETTER METHODS
// ============================================

int User::getId() const { return id; }
std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }
std::string User::getEmail() const { return email; }
UserRole User::getRole() const { return role; }

// Convert role enum to string for display
std::string User::getRoleString() const {
    return (role == UserRole::ADMIN) ? "ADMIN" : "USER";
}

// ============================================
// SETTER METHODS
// ============================================

void User::setId(int id) { this->id = id; }
void User::setUsername(const std::string& username) { this->username = username; }
void User::setPassword(const std::string& password) { this->password = password; }
void User::setEmail(const std::string& email) { this->email = email; }
void User::setRole(UserRole role) { this->role = role; }

// ============================================
// DATABASE OPERATIONS - CRUD
// ============================================

/**
 * Save a new user to the database
 * Uses prepared statements to prevent SQL injection
 * @return true if saved successfully, false otherwise
 */
bool User::save() {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        sqlite3* con = db->getConnection();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "INSERT INTO users (username, password, email, role) VALUES (?, ?, ?, ?)"
        );
        
        if (!stmt) return false;
        
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, getRoleString().c_str(), -1, SQLITE_STATIC);
        
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
        sqlite3* con = db->getConnection();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "UPDATE users SET username = ?, password = ?, email = ?, role = ? WHERE id = ?"
        );
        
        if (!stmt) return false;
        
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, getRoleString().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 5, id);
        
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        
        return rc == SQLITE_DONE;
    } catch (...) {
        std::cerr << "Error updating user" << std::endl;
        return false;
    }
}

bool User::remove() {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "DELETE FROM users WHERE id = ?"
        );
        
        if (!stmt) return false;
        
        sqlite3_bind_int(stmt, 1, id);
        
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        
        return rc == SQLITE_DONE;
    } catch (...) {
        std::cerr << "Error deleting user" << std::endl;
        return false;
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

bool User::usernameExists(const std::string& username) {
    return findByUsername(username) != nullptr;
}

bool User::emailExists(const std::string& email) {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "SELECT COUNT(*) as count FROM users WHERE email = ?"
        );
        
        if (!stmt) return false;
        
        sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);
        
        bool exists = false;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = sqlite3_column_int(stmt, 0) > 0;
        }
        
        sqlite3_finalize(stmt);
        return exists;
    } catch (...) {
        std::cerr << "Error checking email existence" << std::endl;
        return false;
    }
}

bool User::authenticate(const std::string& password) {
    return this->password == password;
}

bool User::isAdmin() const {
    return role == UserRole::ADMIN;
}
