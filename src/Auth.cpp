#include "../include/Auth.h"
#include <iostream>
#include <limits>

User* Auth::currentUser = nullptr;

bool Auth::registerUser(const std::string& username, const std::string& password, 
                        const std::string& email, UserRole role) {
    // Check if username already exists
    if (User::usernameExists(username)) {
        std::cout << "Username already exists!" << std::endl;
        return false;
    }
    
    // Check if email already exists
    if (User::emailExists(email)) {
        std::cout << "Email already registered!" << std::endl;
        return false;
    }
    
    // Validate password length
    if (password.length() < 6) {
        std::cout << "Password must be at least 6 characters!" << std::endl;
        return false;
    }
    
    // Create new user
    User newUser(0, username, password, email, role);
    
    if (newUser.save()) {
        std::cout << "Registration successful!" << std::endl;
        return true;
    } else {
        std::cout << "Registration failed!" << std::endl;
        return false;
    }
}

User* Auth::login(const std::string& username, const std::string& password) {
    User* user = User::findByUsername(username);
    
    if (user == nullptr) {
        std::cout << "User not found!" << std::endl;
        return nullptr;
    }
    
    if (user->authenticate(password)) {
        currentUser = user;
        std::cout << "Login successful! Welcome, " << user->getUsername() << "!" << std::endl;
        std::cout << "Role: " << user->getRoleString() << std::endl;
        return user;
    } else {
        std::cout << "Incorrect password!" << std::endl;
        delete user;
        return nullptr;
    }
}

void Auth::logout() {
    if (currentUser != nullptr) {
        std::cout << "Goodbye, " << currentUser->getUsername() << "!" << std::endl;
        delete currentUser;
        currentUser = nullptr;
    }
}

User* Auth::getCurrentUser() {
    return currentUser;
}

bool Auth::isLoggedIn() {
    return currentUser != nullptr;
}

bool Auth::isAdmin() {
    return currentUser != nullptr && currentUser->isAdmin();
}
