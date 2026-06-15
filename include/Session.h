#ifndef SESSION_H
#define SESSION_H

#include <string>
#include <vector>
#include "DatabaseConnection.h"

class Session {
private:
    int id;
    std::string sessionName;
    std::string section;
    int totalSlots;
    int availableSlots;
    std::string learningModality;
    
public:
    Session();
    Session(int id, const std::string& sessionName, const std::string& section,
            int totalSlots, int availableSlots, const std::string& learningModality);
    
    // Getters
    int getId() const;
    std::string getSessionName() const;
    std::string getSection() const;
    int getTotalSlots() const;
    int getAvailableSlots() const;
    std::string getLearningModality() const;
    
    // Setters
    void setId(int id);
    void setSessionName(const std::string& sessionName);
    void setSection(const std::string& section);
    void setTotalSlots(int totalSlots);
    void setAvailableSlots(int availableSlots);
    void setLearningModality(const std::string& learningModality);
    
    // CRUD Operations
    bool save();
    bool update();
    bool remove();
    static Session* findById(int id);
    static Session* findBySessionName(const std::string& sessionName);
    static std::vector<Session*> getAll();
    static bool sessionNameExists(const std::string& sessionName);
    static std::vector<std::string> getAvailableSections();
    
    // Display
    void display() const;
    static void displayAll();
    static void displayAvailableSlots();
};

#endif
