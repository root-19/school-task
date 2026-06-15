#ifndef SCHOOLSYSTEM_H
#define SCHOOLSYSTEM_H

#include <string>
#include <vector>
#include "DatabaseConnection.h"
#include "User.h"
#include "Student.h"
#include "Session.h"

class SchoolSystem {
private:
    DatabaseConnection* db;
    User* currentUser;
    
    void initializeDatabase();
    void clearInputBuffer();
    bool isEmpty(const std::string& str);
    
    // Authentication
    void handleLogin();
    void handleRegister();
    void logout();
    
    // User Menu
    void displayUserMenu();
    void handleUserMenu();
    
    // Admin Menu
    void displayAdminMenu();
    void handleAdminMenu();
    
public:
    SchoolSystem();
    ~SchoolSystem();
    
    // Session management
    void displayAvailableSessions();
    void displayAvailableSlots();
    void addSession();
    void updateSession();
    void deleteSession();
    
    // Student management
    void enrollStudent();
    void displayAllStudents();
    void displayAllUsers();
    void removeStudent();
    
    // Run the main menu
    void run();
};

#endif
