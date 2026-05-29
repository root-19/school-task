/**
 * School Management System - Main Application
 * 
 * OOP-based school management system with SQLite database.
 * Features:
 * - User authentication (Login/Register)
 * - Student enrollment with unique student numbers
 * - Automatic slot management for sessions
 * - Session management (add, update, delete)
 * - Student management (enroll, remove, view)
 * - Error handling for duplicates, empty fields, and full sessions
 * 
 * Compile Command:
 * g++ -std=c++11 -Iinclude src/main.cpp src/SchoolSystem.cpp src/DatabaseConnection.cpp src/User.cpp src/Student.cpp src/Session.cpp -lsqlite3 -o build/school_management.exe
 * 
 * Run Command:
 * cd build && ./school_management.exe
 */

#include <iostream>
#include "../include/SchoolSystem.h"

int main() {
    SchoolSystem system;
    system.run();
    return 0;
}
