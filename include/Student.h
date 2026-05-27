#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include "DatabaseConnection.h"

class Student {
private:
    int id;
    std::string studentNumber;
    std::string studentName;
    std::string section;
    int sessionId;
    std::string learningModality;

public:
    Student();
    Student(int id, const std::string& studentNumber, const std::string& studentName,
            const std::string& section, int sessionId, const std::string& learningModality);
    
    // Getters
    int getId() const;
    std::string getStudentNumber() const;
    std::string getStudentName() const;
    std::string getSection() const;
    int getSessionId() const;
    std::string getLearningModality() const;
    
    // Setters
    void setId(int id);
    void setStudentNumber(const std::string& studentNumber);
    void setStudentName(const std::string& studentName);
    void setSection(const std::string& section);
    void setSessionId(int sessionId);
    void setLearningModality(const std::string& learningModality);
    
    // CRUD Operations
    bool enroll();
    bool remove();
    static Student* findByStudentNumber(const std::string& studentNumber);
    static Student* findById(int id);
    static std::vector<Student*> getAll();
    static std::vector<Student*> getBySessionId(int sessionId);
    static bool studentNumberExists(const std::string& studentNumber);
    
    // Display
    void display() const;
    static void displayAll();
    static void displayBySession(int sessionId);
};

#endif // STUDENT_H
