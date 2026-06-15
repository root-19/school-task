#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <sqlite3.h>
#include <string>

class DatabaseConnection {
private:
    sqlite3* db;
    static DatabaseConnection* instance;    
    DatabaseConnection();
    
public:
    static DatabaseConnection* getInstance();
    ~DatabaseConnection();
    
    sqlite3* getConnection();
    bool connect(const std::string& databasePath);
    void disconnect();
    bool isConnected();
    bool executeSQL(const std::string& sql);
    bool executeSilent(const std::string& sql);
    sqlite3_stmt* prepareStatement(const std::string& sql);
};

#endif
