/**
 * School Management System - Main Application
 * 
 * This is the entry point for the School Management System console application.
 * It provides a menu-driven interface for:
 * - User authentication (Login/Register)
 * - Student enrollment management
 * - Session management
 * - Role-based access control (USER/ADMIN)
 * 
 * Compile Command:
 * g++ -std=c++11 -Iinclude src/*.cpp -lsqlite3 -o build/school_management.exe
 * 
 * Run Command:
 * cd build && ./school_management.exe
 */

// Standard library headers
#include <iostream>     // For console input/output (cin, cout)
#include <limits>       // For numeric limits (used in input buffer clearing)
#include <iomanip>      // For output formatting (setw, setfill)
#include <cstdlib>      // For system() function (clear screen)
#include <string>       // For std::string

// Project headers
#include "../include/DatabaseConnection.h"  // Database connection singleton
#include "../include/User.h"                // User class for authentication
#include "../include/Student.h"             // Student class for enrollment
#include "../include/Session.h"             // Session class for slot management
#include "../include/Auth.h"                // Authentication system

// ============================================
// UTILITY FUNCTIONS
// ============================================
// void clearInputBuffer() {
//     std::cin.clear();
//     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
// }

/**
 * Clear the console screen
 * Uses "clear" for Linux/MSYS2 and "cls" for Windows
 */
// void clearScreen() {
//     #ifdef _WIN32
//         system("cls");
//     #else
//         system("clear");
//     #endif
// }

/**
 * Pause and wait for user to press Enter
 * Used to let user read messages before screen clears
 */
// void pauseScreen() {
//     std::cout << "\nPress Enter to continue...";
//     clearInputBuffer();
// }

/**
 * Center text in console
 * Assumes console width of 80 characters
 * @param text The text to center
 * @param width Console width (default 80)
 */
// std::string centerText(const std::string& text, int width = 80) {
//     int padding = (width - text.length()) / 2;
//     if (padding < 0) padding = 0;
//     return std::string(padding, ' ') + text;
// }

void displayMainMenu() {
    clearScreen();
    std::cout << centerText("========================================") << std::endl;
    std::cout << centerText("SCHOOL MANAGEMENT SYSTEM") << std::endl;
    std::cout << centerText("========================================") << std::endl;
    std::cout << centerText("1. Login") << std::endl;
    std::cout << centerText("2. Register") << std::endl;
    std::cout << centerText("3. Forgot Password") << std::endl;
    std::cout << centerText("4. Exit") << std::endl;
    std::cout << centerText("========================================") << std::endl;
    std::cout << centerText("Enter your choice: ", 80) << std::flush;
}

void displayUserMenu() {
    clearScreen();
    std::cout << centerText("========================================") << std::endl;
    std::cout << centerText("USER MENU") << std::endl;
    std::cout << centerText("========================================") << std::endl;
    std::cout << centerText("1. View Available Sessions") << std::endl;
    std::cout << centerText("2. View Available Slots") << std::endl;
    std::cout << centerText("3. Enroll Student") << std::endl;
    std::cout << centerText("4. View All Students") << std::endl;
    std::cout << centerText("5. Logout") << std::endl;
    std::cout << centerText("========================================") << std::endl;
    std::cout << centerText("Enter your choice: ", 80) << std::flush;
}

void displayAdminMenu() {
    clearScreen();
    std::cout << centerText("========================================") << std::endl;
    std::cout << centerText("ADMIN MENU") << std::endl;
    std::cout << centerText("========================================") << std::endl;
    std::cout << centerText("1. View All Sessions") << std::endl;
    std::cout << centerText("2. Add New Session") << std::endl;
    std::cout << centerText("3. Update Session") << std::endl;
    std::cout << centerText("4. Delete Session") << std::endl;
    std::cout << centerText("5. View All Students") << std::endl;
    std::cout << centerText("6. Remove Student") << std::endl;
    std::cout << centerText("7. View All Users") << std::endl;
    std::cout << centerText("8. Logout") << std::endl;
    std::cout << centerText("========================================") << std::endl;
    std::cout << centerText("Enter your choice: ", 80) << std::flush;
}

// Validation functions
bool isEmpty(const std::string& str) {
    return str.empty() || str.find_first_not_of(' ') == std::string::npos;
}

// User functions
void viewAvailableSessions() {
    Session::displayAll();
}

void viewAvailableSlots() {
    Session::displayAvailableSlots();
}

void enrollStudent() {
    std::string studentNumber, studentName, section, learningModality;
    int sessionId;
    
    clearInputBuffer();
    
    // Get student number
    std::cout << "Enter Student Number: ";
    std::getline(std::cin, studentNumber);
    
    if (isEmpty(studentNumber)) {
        std::cout << "Error: Student number cannot be empty!" << std::endl;
        return;
    }
    
    if (Student::studentNumberExists(studentNumber)) {
        std::cout << "Error: Student number already exists!" << std::endl;
        return;
    }
    
    // Get student name
    std::cout << "Enter Student Name: ";
    std::getline(std::cin, studentName);
    
    if (isEmpty(studentName)) {
        std::cout << "Error: Student name cannot be empty!" << std::endl;
        return;
    }
    
    // Get section
    std::cout << "Enter Section: ";
    std::getline(std::cin, section);
    
    if (isEmpty(section)) {
        std::cout << "Error: Section cannot be empty!" << std::endl;
        return;
    }
    
    // Display available sessions
    Session::displayAvailableSlots();
    
    // Get session ID
    std::cout << "Enter Session ID: ";
    std::cin >> sessionId;
    clearInputBuffer();
    
    Session* session = Session::findById(sessionId);
    if (!session) {
        std::cout << "Error: Invalid session ID!" << std::endl;
        return;
    }
    
    if (session->getAvailableSlots() <= 0) {
        std::cout << "Error: Session is already full!" << std::endl;
        delete session;
        return;
    }
    
    // Get learning modality
    std::cout << "Enter Learning Modality (Face-to-Face/Online/Hybrid): ";
    std::getline(std::cin, learningModality);
    
    if (isEmpty(learningModality)) {
        std::cout << "Error: Learning modality cannot be empty!" << std::endl;
        delete session;
        return;
    }
    
    // Create and enroll student
    Student student(0, studentNumber, studentName, section, sessionId, learningModality);
    
    if (student.enroll()) {
        std::cout << "Student enrolled successfully in " << session->getSessionName() << "!" << std::endl;
        std::cout << "Remaining slots: " << session->getAvailableSlots() - 1 << std::endl;
    }
    
    delete session;
}

void viewAllStudents() {
    Student::displayAll();
}

// Admin functions
void addSession() {
    std::string sessionName, section, learningModality;
    int totalSlots;
    
    clearInputBuffer();
    std::cout << "Enter Session Name: ";
    std::getline(std::cin, sessionName);
    
    if (isEmpty(sessionName)) {
        std::cout << "Error: Session name cannot be empty!" << std::endl;
        return;
    }
    
    if (Session::sessionNameExists(sessionName)) {
        std::cout << "Error: Session name already exists!" << std::endl;
        return;
    }
    
    std::cout << "Enter Section: ";
    std::getline(std::cin, section);
    
    if (isEmpty(section)) {
        std::cout << "Error: Section cannot be empty!" << std::endl;
        return;
    }
    
    std::cout << "Enter Total Slots: ";
    std::cin >> totalSlots;
    clearInputBuffer();
    
    if (totalSlots <= 0) {
        std::cout << "Error: Total slots must be greater than 0!" << std::endl;
        return;
    }
    
    std::cout << "Enter Learning Modality (Face-to-Face/Online/Hybrid): ";
    std::getline(std::cin, learningModality);
    
    if (isEmpty(learningModality)) {
        std::cout << "Error: Learning modality cannot be empty!" << std::endl;
        return;
    }
    
    Session session(0, sessionName, section, totalSlots, totalSlots, learningModality);
    
    if (session.save()) {
        std::cout << "Session added successfully!" << std::endl;
    } else {
        std::cout << "Failed to add session!" << std::endl;
    }
}

void updateSession() {
    Session::displayAll();
    
    int sessionId;
    std::cout << "\nEnter Session ID to update: ";
    std::cin >> sessionId;
    clearInputBuffer();
    
    Session* session = Session::findById(sessionId);
    if (!session) {
        std::cout << "Error: Invalid session ID!" << std::endl;
        return;
    }
    
    std::string sessionName, section, learningModality;
    int totalSlots;
    
    std::cout << "Current Session Name: " << session->getSessionName() << std::endl;
    std::cout << "Enter new Session Name (press Enter to keep current): ";
    std::getline(std::cin, sessionName);
    if (!isEmpty(sessionName)) {
        if (Session::sessionNameExists(sessionName) && sessionName != session->getSessionName()) {
            std::cout << "Error: Session name already exists!" << std::endl;
            delete session;
            return;
        }
        session->setSessionName(sessionName);
    }
    
    std::cout << "Current Section: " << session->getSection() << std::endl;
    std::cout << "Enter new Section (press Enter to keep current): ";
    std::getline(std::cin, section);
    if (!isEmpty(section)) session->setSection(section);
    
    std::cout << "Current Total Slots: " << session->getTotalSlots() << std::endl;
    std::cout << "Enter new Total Slots (enter 0 to keep current): ";
    std::cin >> totalSlots;
    clearInputBuffer();
    if (totalSlots > 0) {
        int enrolled = session->getTotalSlots() - session->getAvailableSlots();
        session->setTotalSlots(totalSlots);
        session->setAvailableSlots(totalSlots - enrolled);
    }
    
    std::cout << "Current Learning Modality: " << session->getLearningModality() << std::endl;
    std::cout << "Enter new Learning Modality (press Enter to keep current): ";
    std::getline(std::cin, learningModality);
    if (!isEmpty(learningModality)) session->setLearningModality(learningModality);
    
    if (session->update()) {
        std::cout << "Session updated successfully!" << std::endl;
    } else {
        std::cout << "Failed to update session!" << std::endl;
    }
    
    delete session;
}

void deleteSession() {
    Session::displayAll();
    
    int sessionId;
    std::cout << "\nEnter Session ID to delete: ";
    std::cin >> sessionId;
    clearInputBuffer();
    
    Session* session = Session::findById(sessionId);
    if (!session) {
        std::cout << "Error: Invalid session ID!" << std::endl;
        return;
    }
    
    // Check if there are enrolled students
    std::vector<Student*> students = Student::getBySessionId(sessionId);
    if (!students.empty()) {
        std::cout << "Warning: There are " << students.size() << " students enrolled in this session!" << std::endl;
        std::cout << "Deleting this session will also remove all enrolled students." << std::endl;
        
        for (Student* s : students) {
            delete s;
        }
    }
    
    std::cout << "Are you sure you want to delete this session? (y/n): ";
    char confirm;
    std::cin >> confirm;
    clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        if (session->remove()) {
            std::cout << "Session deleted successfully!" << std::endl;
        } else {
            std::cout << "Failed to delete session!" << std::endl;
        }
    } else {
        std::cout << "Deletion cancelled." << std::endl;
    }
    
    delete session;
}

void removeStudent() {
    Student::displayAll();
    
    int studentId;
    std::cout << "\nEnter Student ID to remove: ";
    std::cin >> studentId;
    clearInputBuffer();
    
    Student* student = Student::findById(studentId);
    if (!student) {
        std::cout << "Error: Invalid student ID!" << std::endl;
        return;
    }
    
    std::cout << "Student: " << student->getStudentName() << " (" << student->getStudentNumber() << ")" << std::endl;
    std::cout << "Are you sure you want to remove this student? (y/n): ";
    char confirm;
    std::cin >> confirm;
    clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        if (student->remove()) {
            std::cout << "Student removed successfully!" << std::endl;
            std::cout << "Session slot has been restored." << std::endl;
        } else {
            std::cout << "Failed to remove student!" << std::endl;
        }
    } else {
        std::cout << "Removal cancelled." << std::endl;
    }
    
    delete student;
}

void viewAllUsers() {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement("SELECT * FROM users");
        
        if (!stmt) return;
        
        std::cout << "\n=== All Users ===" << std::endl;
        std::cout << std::left << std::setw(5) << "ID"
                  << std::setw(20) << "Username"
                  << std::setw(30) << "Email"
                  << std::setw(10) << "Role" << std::endl;
        std::cout << std::string(65, '-') << std::endl;
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << std::left << std::setw(5) << sqlite3_column_int(stmt, 0)
                      << std::setw(20) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))
                      << std::setw(30) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))
                      << std::setw(10) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) << std::endl;
        }
        
        std::cout << std::string(65, '-') << std::endl;
        
        sqlite3_finalize(stmt);
    } catch (...) {
        std::cerr << "Error viewing users" << std::endl;
    }
}

// Authentication functions
void handleLogin() {
    std::string username, password;
    
    clearInputBuffer();
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    
    Auth::login(username, password);
    pauseScreen();
}

void handleRegister() {
    std::string username, password, email;
    
    clearInputBuffer();
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    
    std::cout << "Enter password (min 6 characters): ";
    std::getline(std::cin, password);
    
    std::cout << "Enter email: ";
    std::getline(std::cin, email);
    
    Auth::registerUser(username, password, email);
    pauseScreen();
}

/**
 * Handle forgot password functionality
 * Allows users to reset their password by verifying username and email
 */
void handleForgotPassword() {
    std::string username, email, newPassword, confirmPassword;
    
    clearInputBuffer();
    std::cout << "\n=== Forgot Password ===" << std::endl;
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    
    std::cout << "Enter email: ";
    std::getline(std::cin, email);
    
    // Validate inputs
    if (isEmpty(username) || isEmpty(email)) {
        std::cout << "Error: Username and email cannot be empty!" << std::endl;
        pauseScreen();
        return;
    }
    
    // Find user by username
    User* user = User::findByUsername(username);
    if (!user) {
        std::cout << "Error: Username not found!" << std::endl;
        pauseScreen();
        return;
    }
    
    // Verify email matches
    if (user->getEmail() != email) {
        std::cout << "Error: Email does not match the username!" << std::endl;
        delete user;
        pauseScreen();
        return;
    }
    
    // Get new password
    std::cout << "Enter new password (min 6 characters): ";
    std::getline(std::cin, newPassword);
    
    std::cout << "Confirm new password: ";
    std::getline(std::cin, confirmPassword);
    
    // Validate password
    if (newPassword.length() < 6) {
        std::cout << "Error: Password must be at least 6 characters!" << std::endl;
        delete user;
        pauseScreen();
        return;
    }
    
    if (newPassword != confirmPassword) {
        std::cout << "Error: Passwords do not match!" << std::endl;
        delete user;
        pauseScreen();
        return;
    }
    
    // Update password
    user->setPassword(newPassword);
    if (user->update()) {
        std::cout << "Password reset successful! You can now login with your new password." << std::endl;
    } else {
        std::cout << "Error: Failed to reset password!" << std::endl;
    }
    
    delete user;
    pauseScreen();
}

int main() {
    // Database connection parameters
    std::string databasePath = "../build/school_management.db";
    
    // Connect to database
    DatabaseConnection* db = DatabaseConnection::getInstance();
    if (!db->connect(databasePath)) {
        std::cerr << "Failed to connect to database. Exiting..." << std::endl;
        return 1;
    }
    
    // Initialize database tables
    std::cout << "Initializing database..." << std::endl;
    db->executeSQL("CREATE TABLE IF NOT EXISTS users ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "username TEXT UNIQUE NOT NULL, "
                   "password TEXT NOT NULL, "
                   "email TEXT UNIQUE NOT NULL, "
                   "role TEXT DEFAULT 'USER' CHECK(role IN ('USER', 'ADMIN')), "
                   "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");
    
    db->executeSQL("CREATE TABLE IF NOT EXISTS sessions ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "session_name TEXT UNIQUE NOT NULL, "
                   "section TEXT NOT NULL, "
                   "total_slots INTEGER NOT NULL, "
                   "available_slots INTEGER NOT NULL, "
                   "learning_modality TEXT NOT NULL, "
                   "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");
    
    db->executeSQL("CREATE TABLE IF NOT EXISTS students ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "student_number TEXT UNIQUE NOT NULL, "
                   "student_name TEXT NOT NULL, "
                   "section TEXT NOT NULL, "
                   "session_id INTEGER NOT NULL, "
                   "learning_modality TEXT NOT NULL, "
                   "enrolled_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
                   "FOREIGN KEY (session_id) REFERENCES sessions(id) ON DELETE CASCADE)");
    
    // Insert default admin user
    db->executeSQL("INSERT OR IGNORE INTO users (username, password, email, role) "
                   "VALUES ('admin', 'admin123', 'admin@school.edu', 'ADMIN')");
    
    // Insert sample sessions
    db->executeSQL("INSERT OR IGNORE INTO sessions (session_name, section, total_slots, available_slots, learning_modality) VALUES "
                   "('Morning Session 2024', 'A', 30, 30, 'Face-to-Face'), "
                   "('Afternoon Session 2024', 'B', 25, 25, 'Online'), "
                   "('Evening Session 2024', 'C', 20, 20, 'Hybrid'), "
                   "('Weekend Session 2024', 'D', 15, 15, 'Face-to-Face')");
    
    std::cout << "Database initialized successfully!" << std::endl;
    
    int choice;
    
    while (true) {
        if (!Auth::isLoggedIn()) {
            displayMainMenu();
            std::cin >> choice;
            clearInputBuffer();
            
            switch (choice) {
                case 1:
                    handleLogin();
                    break;
                case 2:
                    handleRegister();
                    break;
                case 3:
                    handleForgotPassword();
                    break;
                case 4:
                    std::cout << "Thank you for using School Management System. Goodbye!" << std::endl;
                    db->disconnect();
                    return 0;
                default:
                    std::cout << "Invalid choice! Please try again." << std::endl;
            }
        } else {
            if (Auth::isAdmin()) {
                displayAdminMenu();
                std::cin >> choice;
                clearInputBuffer();
                
                switch (choice) {
                    case 1:
                        viewAvailableSessions();
                        break;
                    case 2:
                        addSession();
                        break;
                    case 3:
                        updateSession();
                        break;
                    case 4:
                        deleteSession();
                        break;
                    case 5:
                        viewAllStudents();
                        break;
                    case 6:
                        removeStudent();
                        break;
                    case 7:
                        viewAllUsers();
                        break;
                    case 8:
                        Auth::logout();
                        break;
                    default:
                        std::cout << "Invalid choice! Please try again." << std::endl;
                }
            } else {
                displayUserMenu();
                std::cin >> choice;
                clearInputBuffer();
                
                switch (choice) {
                    case 1:
                        viewAvailableSessions();
                        break;
                    case 2:
                        viewAvailableSlots();
                        break;
                    case 3:
                        enrollStudent();
                        break;
                    case 4:
                        viewAllStudents();
                        break;
                    case 5:
                        Auth::logout();
                        break;
                    default:
                        std::cout << "Invalid choice! Please try again." << std::endl;
                }
            }
        }
    }
    
    return 0;
}
