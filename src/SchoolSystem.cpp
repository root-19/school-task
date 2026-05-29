#include "../include/SchoolSystem.h"
#include <iostream>
#include <limits>
#include <iomanip>

using namespace std;

SchoolSystem::SchoolSystem() : currentUser(nullptr) {
    db = DatabaseConnection::getInstance();
    string databasePath = "../build/school_management.db";
    
    if (!db->connect(databasePath)) {
        cerr << "Failed to connect to database. Exiting..." << endl;
        exit(1);
    }
    
    db->executeSQL("PRAGMA foreign_keys = ON");
    initializeDatabase();
}

SchoolSystem::~SchoolSystem() {
    if (currentUser) {
        delete currentUser;
    }
    db->disconnect();
}

void SchoolSystem::initializeDatabase() {
    cout << "Initializing database..." << endl;
    
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
    bool insertResult = db->executeSQL("INSERT OR IGNORE INTO sessions (session_name, section, total_slots, available_slots, learning_modality) VALUES "
                   "('Morning Session 2024', 'A', 30, 30, 'Face-to-Face'), "
                   "('Afternoon Session 2024', 'B', 25, 25, 'Online'), "
                   "('Evening Session 2024', 'C', 20, 20, 'Hybrid'), "
                   "('Weekend Session 2024', 'D', 15, 15, 'Face-to-Face')");
    
    if (insertResult) {
        cout << "Sample sessions inserted successfully!" << endl;
    }
    
    cout << "Database initialized successfully!" << endl;
}

void SchoolSystem::clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool SchoolSystem::isEmpty(const string& str) {
    return str.empty() || str.find_first_not_of(' ') == string::npos;
}

void SchoolSystem::handleLogin() {
    string username, password;
    
    cout << "\nEnter username: ";
    getline(cin, username);
    
    cout << "Enter password: ";
    getline(cin, password);
    
    User* user = User::findByUsername(username);
    
    if (user == nullptr) {
        cout << "User not found!" << endl;
        return;
    }
    
    if (user->authenticate(password)) {
        currentUser = user;
        cout << "Login successful! Welcome, " << user->getUsername() << "!" << endl;
        cout << "Role: " << user->getRoleString() << endl;
    } else {
        cout << "Incorrect password!" << endl;
        delete user;
    }
}

void SchoolSystem::handleRegister() {
    string username, password, email;
    
    cout << "\nEnter username: ";
    getline(cin, username);
    
    cout << "Enter password (min 6 characters): ";
    getline(cin, password);
    
    cout << "Enter email: ";
    getline(cin, email);
    
    if (isEmpty(username) || isEmpty(password) || isEmpty(email)) {
        cout << "Error: All fields are required!" << endl;
        return;
    }
    
    if (password.length() < 6) {
        cout << "Error: Password must be at least 6 characters!" << endl;
        return;
    }
    
    if (User::usernameExists(username)) {
        cout << "Error: Username already exists!" << endl;
        return;
    }
    
    if (User::emailExists(email)) {
        cout << "Error: Email already registered!" << endl;
        return;
    }
    
    User newUser(0, username, password, email, UserRole::USER);
    
    if (newUser.save()) {
        cout << "Registration successful! You can now login." << endl;
    } else {
        cout << "Registration failed!" << endl;
    }
}

void SchoolSystem::logout() {
    if (currentUser != nullptr) {
        cout << "Goodbye, " << currentUser->getUsername() << "!" << endl;
        delete currentUser;
        currentUser = nullptr;
    }
}

void SchoolSystem::displayAvailableSessions() {
    Session::displayAll();
}

void SchoolSystem::displayAvailableSlots() {
    Session::displayAvailableSlots();
}

void SchoolSystem::addSession() {
    string sessionName, section, learningModality;
    int totalSlots;
    
    cout << "Enter Session Name: ";
    getline(cin, sessionName);
    
    if (isEmpty(sessionName)) {
        cout << "Error: Session name cannot be empty!" << endl;
        return;
    }
    
    if (Session::sessionNameExists(sessionName)) {
        cout << "Error: Session name already exists!" << endl;
        return;
    }
    
    cout << "Enter Section: ";
    getline(cin, section);
    
    if (isEmpty(section)) {
        cout << "Error: Section cannot be empty!" << endl;
        return;
    }
    
    cout << "Enter Total Slots: ";
    cin >> totalSlots;
    clearInputBuffer();
    
    if (totalSlots <= 0) {
        cout << "Error: Total slots must be greater than 0!" << endl;
        return;
    }
    
    cout << "Enter Learning Modality (Face-to-Face/Online/Hybrid): ";
    getline(cin, learningModality);
    
    if (isEmpty(learningModality)) {
        cout << "Error: Learning modality cannot be empty!" << endl;
        return;
    }
    
    Session session(0, sessionName, section, totalSlots, totalSlots, learningModality);
    
    if (session.save()) {
        cout << "Session added successfully!" << endl;
    } else {
        cout << "Failed to add session!" << endl;
    }
}

void SchoolSystem::updateSession() {
    Session::displayAll();
    
    int sessionId;
    cout << "\nEnter Session ID to update: ";
    cin >> sessionId;
    clearInputBuffer();
    
    Session* session = Session::findById(sessionId);
    if (!session) {
        cout << "Error: Invalid session ID!" << endl;
        return;
    }
    
    string sessionName, section, learningModality;
    int totalSlots;
    
    cout << "Current Session Name: " << session->getSessionName() << endl;
    cout << "Enter new Session Name (press Enter to keep current): ";
    getline(cin, sessionName);
    if (!isEmpty(sessionName)) {
        if (Session::sessionNameExists(sessionName) && sessionName != session->getSessionName()) {
            cout << "Error: Session name already exists!" << endl;
            delete session;
            return;
        }
        session->setSessionName(sessionName);
    }
    
    cout << "Current Section: " << session->getSection() << endl;
    cout << "Enter new Section (press Enter to keep current): ";
    getline(cin, section);
    if (!isEmpty(section)) session->setSection(section);
    
    cout << "Current Total Slots: " << session->getTotalSlots() << endl;
    cout << "Enter new Total Slots (enter 0 to keep current): ";
    cin >> totalSlots;
    clearInputBuffer();
    if (totalSlots > 0) {
        int enrolled = session->getTotalSlots() - session->getAvailableSlots();
        if (totalSlots < enrolled) {
            cout << "Error: New total slots cannot be less than currently enrolled!" << endl;
            delete session;
            return;
        }
        session->setTotalSlots(totalSlots);
        session->setAvailableSlots(totalSlots - enrolled);
    }
    
    cout << "Current Learning Modality: " << session->getLearningModality() << endl;
    cout << "Enter new Learning Modality (press Enter to keep current): ";
    getline(cin, learningModality);
    if (!isEmpty(learningModality)) session->setLearningModality(learningModality);
    
    if (session->update()) {
        cout << "Session updated successfully!" << endl;
    } else {
        cout << "Failed to update session!" << endl;
    }
    
    delete session;
}

void SchoolSystem::deleteSession() {
    Session::displayAll();
    
    int sessionId;
    cout << "\nEnter Session ID to delete: ";
    cin >> sessionId;
    clearInputBuffer();
    
    Session* session = Session::findById(sessionId);
    if (!session) {
        cout << "Error: Invalid session ID!" << endl;
        return;
    }
    
    vector<Student*> students = Student::getBySessionId(sessionId);
    if (!students.empty()) {
        cout << "Warning: There are " << students.size() << " students enrolled in this session!" << endl;
        cout << "Deleting this session will also remove all enrolled students." << endl;
        
        for (Student* s : students) {
            delete s;
        }
    }
    
    cout << "Are you sure you want to delete this session? (y/n): ";
    char confirm;
    cin >> confirm;
    clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        if (session->remove()) {
            cout << "Session deleted successfully!" << endl;
        } else {
            cout << "Failed to delete session!" << endl;
        }
    } else {
        cout << "Deletion cancelled." << endl;
    }
    
    delete session;
}

void SchoolSystem::enrollStudent() {
    string studentNumber, studentName, section, learningModality;
    int sessionId;
    
    cout << "Enter Student Number: ";
    getline(cin, studentNumber);
    
    if (isEmpty(studentNumber)) {
        cout << "Error: Student number cannot be empty!" << endl;
        return;
    }
    
    if (Student::studentNumberExists(studentNumber)) {
        cout << "Error: Student number already exists!" << endl;
        return;
    }
    
    cout << "Enter Student Name: ";
    getline(cin, studentName);
    
    if (isEmpty(studentName)) {
        cout << "Error: Student name cannot be empty!" << endl;
        return;
    }
    
    cout << "Enter Section: ";
    getline(cin, section);
    
    if (isEmpty(section)) {
        cout << "Error: Section cannot be empty!" << endl;
        return;
    }
    
    displayAvailableSlots();
    
    cout << "Enter Session ID: ";
    cin >> sessionId;
    clearInputBuffer();
    
    Session* session = Session::findById(sessionId);
    if (!session) {
        cout << "Error: Invalid session ID!" << endl;
        return;
    }
    
    if (session->getAvailableSlots() <= 0) {
        cout << "Error: Session is already full!" << endl;
        delete session;
        return;
    }
    
    cout << "Enter Learning Modality (Face-to-Face/Online/Hybrid): ";
    getline(cin, learningModality);
    
    if (isEmpty(learningModality)) {
        cout << "Error: Learning modality cannot be empty!" << endl;
        delete session;
        return;
    }
    
    Student student(0, studentNumber, studentName, section, sessionId, learningModality);
    
    if (student.enroll()) {
        cout << "Student enrolled successfully in " << session->getSessionName() << "!" << endl;
        cout << "Remaining slots: " << session->getAvailableSlots() - 1 << endl;
    }
    
    delete session;
}

void SchoolSystem::displayAllStudents() {
    Student::displayAll();
}

void SchoolSystem::displayAllUsers() {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement("SELECT * FROM users");
        
        if (!stmt) return;
        
        cout << "\n=== All Users ===" << endl;
        cout << left << setw(5) << "ID"
                  << setw(20) << "Username"
                  << setw(30) << "Email"
                  << setw(10) << "Role" << endl;
        cout << string(65, '-') << endl;
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            cout << left << setw(5) << sqlite3_column_int(stmt, 0)
                      << setw(20) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))
                      << setw(30) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))
                      << setw(10) << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) << endl;
        }
        
        cout << string(65, '-') << endl;
        
        sqlite3_finalize(stmt);
        
    } catch (...) {
        cerr << "Error viewing users" << endl;
    }
}

void SchoolSystem::removeStudent() {
    Student::displayAll();
    
    int studentId;
    cout << "\nEnter Student ID to remove: ";
    cin >> studentId;
    clearInputBuffer();
    
    Student* student = Student::findById(studentId);
    if (!student) {
        cout << "Error: Invalid student ID!" << endl;
        return;
    }
    
    cout << "Student: " << student->getStudentName() << " (" << student->getStudentNumber() << ")" << endl;
    cout << "Are you sure you want to remove this student? (y/n): ";
    char confirm;
    cin >> confirm;
    clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        if (student->remove()) {
            cout << "Student removed successfully!" << endl;
            cout << "Session slot has been restored." << endl;
        } else {
            cout << "Failed to remove student!" << endl;
        }
    } else {
        cout << "Removal cancelled." << endl;
    }
    
    delete student;
}

void SchoolSystem::displayUserMenu() {
    cout << "\n========================================" << endl;
    cout << "USER MENU" << endl;
    cout << "========================================" << endl;
    cout << "1. View Available Sessions" << endl;
    cout << "2. View Available Slots" << endl;
    cout << "3. Enroll Student" << endl;
    cout << "4. View All Students" << endl;
    cout << "5. Logout" << endl;
    cout << "========================================" << endl;
    cout << "Enter your choice: ";
}

void SchoolSystem::handleUserMenu() {
    int choice;
    string choiceStr;
    
    while (currentUser != nullptr && !currentUser->isAdmin()) {
        displayUserMenu();
        getline(cin, choiceStr);
        
        try {
            choice = choiceStr.empty() ? 0 : stoi(choiceStr);
        } catch (...) {
            choice = -1;
        }
        
        switch (choice) {
            case 1:
                displayAvailableSessions();
                break;
            case 2:
                displayAvailableSlots();
                break;
            case 3:
                enrollStudent();
                break;
            case 4:
                displayAllStudents();
                break;
            case 5:
                logout();
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}

void SchoolSystem::displayAdminMenu() {
    cout << "\n========================================" << endl;
    cout << "ADMIN MENU" << endl;
    cout << "========================================" << endl;
    cout << "1. View All Sessions" << endl;
    cout << "2. Add New Session" << endl;
    cout << "3. Update Session" << endl;
    cout << "4. Delete Session" << endl;
    cout << "5. View All Students" << endl;
    cout << "6. Remove Student" << endl;
    cout << "7. View All Users" << endl;
    cout << "8. Logout" << endl;
    cout << "========================================" << endl;
    cout << "Enter your choice: ";
}

void SchoolSystem::handleAdminMenu() {
    int choice;
    string choiceStr;
    
    while (currentUser != nullptr && currentUser->isAdmin()) {
        displayAdminMenu();
        getline(cin, choiceStr);
        
        try {
            choice = choiceStr.empty() ? 0 : stoi(choiceStr);
        } catch (...) {
            choice = -1;
        }
        
        switch (choice) {
            case 1:
                displayAvailableSessions();
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
                displayAllStudents();
                break;
            case 6:
                removeStudent();
                break;
            case 7:
                displayAllUsers();
                break;
            case 8:
                logout();
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}

void SchoolSystem::run() {
    int choice;
    string choiceStr;
    
    while (true) {
        cout << "\n========================================" << endl;
        cout << "SCHOOL MANAGEMENT SYSTEM" << endl;
        cout << "========================================" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice: ";
        getline(cin, choiceStr);
        
        try {
            choice = choiceStr.empty() ? 0 : stoi(choiceStr);
        } catch (...) {
            choice = -1;
        }
        
        switch (choice) {
            case 1:
                handleLogin();
                if (currentUser != nullptr) {
                    if (currentUser->isAdmin()) {
                        handleAdminMenu();
                    } else {
                        handleUserMenu();
                    }
                }
                break;
            case 2:
                handleRegister();
                break;
            case 3:
                cout << "Thank you for using School Management System. Goodbye!" << endl;
                return;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }
}
