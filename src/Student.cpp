#include "../include/Student.h"
#include <iostream>
#include <iomanip>
#include "../include/Session.h"

Student::Student() : id(0), sessionId(0) {}

Student::Student(int id, const std::string& studentNumber, const std::string& studentName,
                const std::string& section, int sessionId, const std::string& learningModality)
    : id(id), studentNumber(studentNumber), studentName(studentName),
      section(section), sessionId(sessionId), learningModality(learningModality) {}

int Student::getId() const { return id; }
std::string Student::getStudentNumber() const { return studentNumber; }
std::string Student::getStudentName() const { return studentName; }
std::string Student::getSection() const { return section; }
int Student::getSessionId() const { return sessionId; }
std::string Student::getLearningModality() const { return learningModality; }

void Student::setId(int id) { this->id = id; }
void Student::setStudentNumber(const std::string& studentNumber) { this->studentNumber = studentNumber; }
void Student::setStudentName(const std::string& studentName) { this->studentName = studentName; }
void Student::setSection(const std::string& section) { this->section = section; }
void Student::setSessionId(int sessionId) { this->sessionId = sessionId; }
void Student::setLearningModality(const std::string& learningModality) { this->learningModality = learningModality; }

bool Student::enroll() {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        sqlite3* con = db->getConnection();
        
        if (studentNumberExists(studentNumber)) {
            std::cerr << "Error: Student number already exists!" << std::endl;
            return false;
        }
        
        Session* session = Session::findById(sessionId);
        if (!session) {
            std::cerr << "Error: Invalid session!" << std::endl;
            return false;
        }
        
        if (session->getAvailableSlots() <= 0) {
            std::cerr << "Error: Session is already full!" << std::endl;
            delete session;
            return false;
        }
        
        db->executeSQL("BEGIN TRANSACTION");
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "INSERT INTO students (student_number, student_name, section, session_id, learning_modality) VALUES (?, ?, ?, ?, ?)"
        );
        
        if (!stmt) {
            db->executeSQL("ROLLBACK");
            delete session;
            return false;
        }
        
        sqlite3_bind_text(stmt, 1, studentNumber.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, studentName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, section.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, sessionId);
        sqlite3_bind_text(stmt, 5, learningModality.c_str(), -1, SQLITE_STATIC);
        
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        
        bool result = (rc == SQLITE_DONE);
        
        if (result) {
            session->setAvailableSlots(session->getAvailableSlots() - 1);
            session->update();
            db->executeSQL("COMMIT");
        } else {
            db->executeSQL("ROLLBACK");
        }
        
        delete session;
        return result;
        
    } catch (...) {
        std::cerr << "Error enrolling student" << std::endl;
        try {
            DatabaseConnection* db = DatabaseConnection::getInstance();
            db->executeSQL("ROLLBACK");
        } catch (...) {}
        return false;
    }
}

bool Student::remove() {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        Student* student = Student::findById(id);
        if (!student) {
            return false;
        }
        
        db->executeSQL("BEGIN TRANSACTION");
        
        Session* session = Session::findById(student->getSessionId());
        if (session) {
            session->setAvailableSlots(session->getAvailableSlots() + 1);
            session->update();
            delete session;
        }
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "DELETE FROM students WHERE id = ?"
        );
        
        if (!stmt) {
            db->executeSQL("ROLLBACK");
            delete student;
            return false;
        }
        
        sqlite3_bind_int(stmt, 1, id);
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        
        bool result = (rc == SQLITE_DONE);
        
        if (result) {
            db->executeSQL("COMMIT");
        } else {
            db->executeSQL("ROLLBACK");
        }
        
        delete student;
        return result;
        
    } catch (...) {
        std::cerr << "Error removing student" << std::endl;
        try {
            DatabaseConnection* db = DatabaseConnection::getInstance();
            db->executeSQL("ROLLBACK");
        } catch (...) {}
        return false;
    }
}

Student* Student::findByStudentNumber(const std::string& studentNumber) {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "SELECT * FROM students WHERE student_number = ?"
        );
        
        if (!stmt) return nullptr;
        
        sqlite3_bind_text(stmt, 1, studentNumber.c_str(), -1, SQLITE_STATIC);
        
        Student* student = nullptr;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            student = new Student(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
                sqlite3_column_int(stmt, 4),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))
            );
        }
        
        sqlite3_finalize(stmt);
        return student;
    } catch (...) {
        std::cerr << "Error finding student" << std::endl;
        return nullptr;
    }
}

Student* Student::findById(int id) {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "SELECT * FROM students WHERE id = ?"
        );
        
        if (!stmt) return nullptr;
        
        sqlite3_bind_int(stmt, 1, id);
        
        Student* student = nullptr;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            student = new Student(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
                sqlite3_column_int(stmt, 4),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))
            );
        }
        
        sqlite3_finalize(stmt);
        return student;
    } catch (...) {
        std::cerr << "Error finding student by ID" << std::endl;
        return nullptr;
    }
}

std::vector<Student*> Student::getAll() {
    std::vector<Student*> students;
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "SELECT * FROM students ORDER BY enrolled_at DESC"
        );
        
        if (!stmt) return students;
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Student* student = new Student(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
                sqlite3_column_int(stmt, 4),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))
            );
            students.push_back(student);
        }
        
        sqlite3_finalize(stmt);
    } catch (...) {
        std::cerr << "Error getting all students" << std::endl;
    }
    return students;
}

std::vector<Student*> Student::getBySessionId(int sessionId) {
    std::vector<Student*> students;
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "SELECT * FROM students WHERE session_id = ? ORDER BY enrolled_at DESC"
        );
        
        if (!stmt) return students;
        
        sqlite3_bind_int(stmt, 1, sessionId);
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Student* student = new Student(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)),
                sqlite3_column_int(stmt, 4),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))
            );
            students.push_back(student);
        }
        
        sqlite3_finalize(stmt);
    } catch (...) {
        std::cerr << "Error getting students by session" << std::endl;
    }
    return students;
}

bool Student::studentNumberExists(const std::string& studentNumber) {
    Student* s = findByStudentNumber(studentNumber);
    bool exists = (s != nullptr);
    delete s;
    return exists;
}

void Student::display() const {
    Session* session = Session::findById(sessionId);
    std::cout << std::left << std::setw(5)  << id
              << std::setw(14) << studentNumber
              << std::setw(22) << studentName
              << std::setw(8)  << section
              << std::setw(22) << (session ? session->getSessionName() : "N/A")
              << std::setw(13) << learningModality << std::endl;
    delete session;
}

void Student::displayAll() {
    std::cout << "\n=== All Enrolled Students ===" << std::endl;
    std::cout << std::left << std::setw(5)  << "ID"
              << std::setw(14) << "Student No."
              << std::setw(22) << "Name"
              << std::setw(8)  << "Section"
              << std::setw(22) << "Session"
              << std::setw(13) << "Modality" << std::endl;
    std::cout << std::string(84, '-') << std::endl;
    
    std::vector<Student*> students = getAll();
    if (students.empty()) {
        std::cout << "No students enrolled." << std::endl;
    }
    
    for (Student* student : students) {
        student->display();
        delete student;
    }
    std::cout << std::string(84, '-') << std::endl;
}
