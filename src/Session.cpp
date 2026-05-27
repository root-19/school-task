#include "../include/Session.h"
#include <iostream>
#include <iomanip>

Session::Session() : id(0), totalSlots(0), availableSlots(0) {}

Session::Session(int id, const std::string& sessionName, const std::string& section,
                int totalSlots, int availableSlots, const std::string& learningModality)
    : id(id), sessionName(sessionName), section(section),
      totalSlots(totalSlots), availableSlots(availableSlots),
      learningModality(learningModality) {}

int Session::getId() const { return id; }
std::string Session::getSessionName() const { return sessionName; }
std::string Session::getSection() const { return section; }
int Session::getTotalSlots() const { return totalSlots; }
int Session::getAvailableSlots() const { return availableSlots; }
std::string Session::getLearningModality() const { return learningModality; }

void Session::setId(int id) { this->id = id; }
void Session::setSessionName(const std::string& sessionName) { this->sessionName = sessionName; }
void Session::setSection(const std::string& section) { this->section = section; }
void Session::setTotalSlots(int totalSlots) { this->totalSlots = totalSlots; }
void Session::setAvailableSlots(int availableSlots) { this->availableSlots = availableSlots; }
void Session::setLearningModality(const std::string& learningModality) { this->learningModality = learningModality; }

bool Session::save() {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        sqlite3* con = db->getConnection();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "INSERT INTO sessions (session_name, section, total_slots, available_slots, learning_modality) VALUES (?, ?, ?, ?, ?)"
        );
        
        if (!stmt) return false;
        
        sqlite3_bind_text(stmt, 1, sessionName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, section.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, totalSlots);
        sqlite3_bind_int(stmt, 4, availableSlots);
        sqlite3_bind_text(stmt, 5, learningModality.c_str(), -1, SQLITE_STATIC);
        
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        
        if (rc == SQLITE_DONE) {
            id = sqlite3_last_insert_rowid(con);
            return true;
        }
        
        return false;
    } catch (...) {
        std::cerr << "Error saving session" << std::endl;
        return false;
    }
}

bool Session::update() {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "UPDATE sessions SET session_name = ?, section = ?, total_slots = ?, available_slots = ?, learning_modality = ? WHERE id = ?"
        );
        
        if (!stmt) return false;
        
        sqlite3_bind_text(stmt, 1, sessionName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, section.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, totalSlots);
        sqlite3_bind_int(stmt, 4, availableSlots);
        sqlite3_bind_text(stmt, 5, learningModality.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 6, id);
        
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        
        return rc == SQLITE_DONE;
    } catch (...) {
        std::cerr << "Error updating session" << std::endl;
        return false;
    }
}

bool Session::remove() {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "DELETE FROM sessions WHERE id = ?"
        );
        
        if (!stmt) return false;
        
        sqlite3_bind_int(stmt, 1, id);
        
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        
        return rc == SQLITE_DONE;
    } catch (...) {
        std::cerr << "Error deleting session" << std::endl;
        return false;
    }
}

Session* Session::findById(int id) {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "SELECT * FROM sessions WHERE id = ?"
        );
        
        if (!stmt) return nullptr;
        
        sqlite3_bind_int(stmt, 1, id);
        
        Session* session = nullptr;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            session = new Session(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                sqlite3_column_int(stmt, 3),
                sqlite3_column_int(stmt, 4),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))
            );
        }
        
        sqlite3_finalize(stmt);
        return session;
    } catch (...) {
        std::cerr << "Error finding session by ID" << std::endl;
        return nullptr;
    }
}

Session* Session::findBySessionName(const std::string& sessionName) {
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "SELECT * FROM sessions WHERE session_name = ?"
        );
        
        if (!stmt) return nullptr;
        
        sqlite3_bind_text(stmt, 1, sessionName.c_str(), -1, SQLITE_STATIC);
        
        Session* session = nullptr;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            session = new Session(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                sqlite3_column_int(stmt, 3),
                sqlite3_column_int(stmt, 4),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))
            );
        }
        
        sqlite3_finalize(stmt);
        return session;
    } catch (...) {
        std::cerr << "Error finding session by name" << std::endl;
        return nullptr;
    }
}

std::vector<Session*> Session::getAll() {
    std::vector<Session*> sessions;
    try {
        DatabaseConnection* db = DatabaseConnection::getInstance();
        
        sqlite3_stmt* stmt = db->prepareStatement(
            "SELECT * FROM sessions ORDER BY session_name"
        );
        
        if (!stmt) return sessions;
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Session* session = new Session(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                sqlite3_column_int(stmt, 3),
                sqlite3_column_int(stmt, 4),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5))
            );
            sessions.push_back(session);
        }
        
        sqlite3_finalize(stmt);
    } catch (...) {
        std::cerr << "Error getting all sessions" << std::endl;
    }
    return sessions;
}

bool Session::sessionNameExists(const std::string& sessionName) {
    return findBySessionName(sessionName) != nullptr;
}

void Session::display() const {
    std::cout << std::left << std::setw(5) << id
              << std::setw(30) << sessionName
              << std::setw(15) << section
              << std::setw(15) << availableSlots
              << std::setw(15) << totalSlots
              << std::setw(20) << learningModality << std::endl;
}

void Session::displayAll() {
    std::cout << "\n=== All Sessions ===" << std::endl;
    std::cout << std::left << std::setw(5) << "ID"
              << std::setw(30) << "Session Name"
              << std::setw(15) << "Section"
              << std::setw(15) << "Available"
              << std::setw(15) << "Total"
              << std::setw(20) << "Modality" << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    
    std::vector<Session*> sessions = getAll();
    for (Session* session : sessions) {
        session->display();
        delete session;
    }
    std::cout << std::string(100, '-') << std::endl;
}

void Session::displayAvailableSlots() {
    std::cout << "\n=== Available Slots per Session ===" << std::endl;
    std::cout << std::left << std::setw(5) << "ID"
              << std::setw(30) << "Session Name"
              << std::setw(15) << "Section"
              << std::setw(15) << "Available"
              << std::setw(15) << "Total"
              << std::setw(20) << "Status" << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    
    std::vector<Session*> sessions = getAll();
    for (Session* session : sessions) {
        std::string status = (session->getAvailableSlots() > 0) ? "Open" : "Full";
        std::cout << std::left << std::setw(5) << session->getId()
                  << std::setw(30) << session->getSessionName()
                  << std::setw(15) << session->getSection()
                  << std::setw(15) << session->getAvailableSlots()
                  << std::setw(15) << session->getTotalSlots()
                  << std::setw(20) << status << std::endl;
        delete session;
    }
    std::cout << std::string(100, '-') << std::endl;
}
