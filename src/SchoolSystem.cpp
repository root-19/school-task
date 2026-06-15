#include "../include/SchoolSystem.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

SchoolSystem::SchoolSystem() : currentUser(nullptr) {
    db = DatabaseConnection::getInstance();
    string databasePath = "school_management.db";
    
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
                   "gender TEXT NOT NULL DEFAULT '', "
                   "age INTEGER NOT NULL DEFAULT 0, "
                   "section TEXT NOT NULL, "
                   "session_id INTEGER NOT NULL, "
                   "learning_modality TEXT NOT NULL, "
                   "enrolled_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
                   "FOREIGN KEY (session_id) REFERENCES sessions(id) ON DELETE CASCADE)");
    db->executeSilent("ALTER TABLE students ADD COLUMN gender TEXT NOT NULL DEFAULT ''");
    db->executeSilent("ALTER TABLE students ADD COLUMN age INTEGER NOT NULL DEFAULT 0");
    
    // Insert default admin user
    db->executeSQL("INSERT OR IGNORE INTO users (username, password, email, role) "
                   "VALUES ('admin', 'admin123', 'admin@school.edu', 'ADMIN')");
    
    // cout << "Database initialized successfully!" << endl;
}

void SchoolSystem::clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool SchoolSystem::isEmpty(const string& str) {
    return str.empty() || str.find_first_not_of(' ') == string::npos;
}

void SchoolSystem::handleLogin() {
    const int MAX_ATTEMPTS = 3;
    int attempts = 0;
    
    while (attempts < MAX_ATTEMPTS) {
        string username, password;
        
        cout << "\nEnter username: ";
        getline(cin, username);
        
        cout << "Enter password: ";
        getline(cin, password);
        
        if (isEmpty(username) || isEmpty(password)) {
            cout << "Error: Username and password cannot be empty. Please try again." << endl;
            attempts++;
            continue;
        }
        
        User* user = User::findByUsername(username);
        
        if (user == nullptr) {
            attempts++;
            cout << "Error: User '" << username << "' not found in the database.";
            if (attempts < MAX_ATTEMPTS) {
                cout << " Please try again. (" << attempts << "/" << MAX_ATTEMPTS << " attempts)" << endl;
            } else {
                cout << endl << "Maximum login attempts reached. Returning to main menu." << endl;
            }
            continue;
        }
        
        if (user->authenticate(password)) {
            currentUser = user;
            cout << "Login successful! Welcome, " << user->getUsername() << "!" << endl;
            cout << "Role: " << user->getRoleString() << endl;
            return;
        } else {
            delete user;
            attempts++;
            cout << "Error: Incorrect password.";
            if (attempts < MAX_ATTEMPTS) {
                cout << " Please try again. (" << attempts << "/" << MAX_ATTEMPTS << " attempts)" << endl;
            } else {
                cout << endl << "Maximum login attempts reached. Returning to main menu." << endl;
            }
        }
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
    {
        User* check = (currentUser != nullptr) ? User::findByUsername(currentUser->getUsername()) : nullptr;
        if (check == nullptr) {
            cout << "Error: Your session is no longer valid. Please log in again." << endl;
            logout();
            return;
        }
        delete check;
    }
    
    string studentNumber, studentName, gender, section, learningModality;
    int age = 0;
    int sessionId;
    
    cout << "Enter Student Number: ";
    getline(cin, studentNumber);
    
    if (isEmpty(studentNumber)) {
        cout << "Error: Student number cannot be empty!" << endl;
        return;
    }
    
    if (Student::studentNumberExists(studentNumber)) {
        cout << "Error: Student number '" << studentNumber << "' already exists!" << endl;
        return;
    }
    
    cout << "Enter Student Name: ";
    getline(cin, studentName);
    
    if (isEmpty(studentName)) {
        cout << "Error: Student name cannot be empty!" << endl;
        return;
    }
    
    cout << "Enter Gender (Male/Female/Other): ";
    getline(cin, gender);
    
    if (isEmpty(gender)) {
        cout << "Error: Gender cannot be empty!" << endl;
        return;
    }
    
    cout << "Enter Age: ";
    string ageStr;
    getline(cin, ageStr);
    try {
        age = stoi(ageStr);
    } catch (...) {
        age = 0;
    }
    if (age <= 0 || age > 120) {
        cout << "Error: Please enter a valid age (1-120)." << endl;
        return;
    }
    
    vector<Session*> availSessions;
    for (Session* s : Session::getAll()) {
        if (s->getAvailableSlots() > 0) {
            availSessions.push_back(s);
        } else {
            delete s;
        }
    }
    
    if (availSessions.empty()) {
        cout << "Error: No sessions with available slots found. Please contact an administrator." << endl;
        return;
    }
    
    cout << "\n=== Available Sessions ==="  << endl;
    cout << left << setw(5) << "No."
         << setw(10) << "Section"
         << setw(16) << "Modality"
         << setw(11) << "Available"
         << "Session Name" << endl;
    cout << string(60, '-') << endl;
    for (size_t i = 0; i < availSessions.size(); i++) {
        cout << left << setw(5) << (i + 1)
             << setw(10) << availSessions[i]->getSection()
             << setw(16) << availSessions[i]->getLearningModality()
             << setw(11) << availSessions[i]->getAvailableSlots()
             << availSessions[i]->getSessionName() << endl;
    }
    cout << string(60, '-') << endl;
    cout << "Select session number: ";
    string secChoiceStr;
    getline(cin, secChoiceStr);
    int secChoice = 0;
    try {
        secChoice = stoi(secChoiceStr);
    } catch (...) {
        secChoice = 0;
    }
    if (secChoice < 1 || secChoice > (int)availSessions.size()) {
        cout << "Error: Invalid session selection." << endl;
        for (Session* s : availSessions) delete s;
        return;
    }
    
    Session* session = availSessions[secChoice - 1];
    sessionId      = session->getId();
    section        = session->getSection();
    learningModality = session->getLearningModality();
    
    for (size_t i = 0; i < availSessions.size(); i++) {
        if ((int)i != secChoice - 1) delete availSessions[i];
    }
    
    Student student(0, studentNumber, studentName, gender, age, section, sessionId, learningModality);
    
    if (student.enroll()) {
        cout << "\n--- Enrollment Successful ---" << endl;
        cout << "Student Number : " << studentNumber << endl;
        cout << "Name           : " << studentName << endl;
        cout << "Gender         : " << gender << endl;
        cout << "Age            : " << age << endl;
        cout << "Section        : " << section << endl;
        cout << "Session        : " << session->getSessionName() << endl;
        cout << "Modality       : " << learningModality << endl;
        cout << "Remaining slots: " << session->getAvailableSlots() - 1 << endl;
        cout << "----------------------------" << endl;
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
