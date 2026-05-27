/**
 * DatabaseConnection Class Implementation
 * 
 * This file implements a Singleton pattern for database connection management.
 * Using SQLite as the database engine for simplicity and portability.
 * 
 * Singleton Pattern Benefits:
 * - Only one database connection exists throughout the application
 * - Prevents multiple connections to the same database
 * - Centralized connection management
 * 
 * SQLite Features Used:
 * - sqlite3_open(): Opens database connection
 * - sqlite3_prepare_v2(): Prepares SQL statements (prevents SQL injection)
 * - sqlite3_step(): Executes prepared statements
 * - sqlite3_bind_*(): Binds parameters to prepared statements
 * - sqlite3_column_*(): Retrieves column values from results
 */

#include "../include/DatabaseConnection.h"
#include <iostream>

// Static instance pointer - initially null (Singleton pattern)
DatabaseConnection* DatabaseConnection::instance = nullptr;

// ============================================
// CONSTRUCTOR & DESTRUCTOR
// ============================================

// Private constructor - prevents direct instantiation
// Initializes db pointer to nullptr
DatabaseConnection::DatabaseConnection() : db(nullptr) {}

// ============================================
// SINGLETON PATTERN METHODS
// ============================================

/**
 * Get the singleton instance of DatabaseConnection
 * Creates the instance if it doesn't exist
 * @return Pointer to the single DatabaseConnection instance
 */
DatabaseConnection* DatabaseConnection::getInstance() {
    if (instance == nullptr) {
        instance = new DatabaseConnection();
    }
    return instance;
}

// Destructor - automatically closes database connection when object is destroyed
DatabaseConnection::~DatabaseConnection() {
    disconnect();
}

// ============================================
// DATABASE CONNECTION METHODS
// ============================================

/**
 * Connect to SQLite database file
 * @param databasePath Path to the .db file (e.g., "school_management.db")
 * @return true if connection successful, false otherwise
 */
bool DatabaseConnection::connect(const std::string& databasePath) {
    int rc = sqlite3_open(databasePath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    std::cout << "Connected to SQLite database successfully!" << std::endl;
    return true;
}

/**
 * Get the raw SQLite database connection handle
 * Used by other classes to execute queries
 * @return Pointer to sqlite3 database handle
 */
sqlite3* DatabaseConnection::getConnection() {
    return db;
}

/**
 * Close database connection and cleanup resources
 */
void DatabaseConnection::disconnect() {
    if (db != nullptr) {
        sqlite3_close(db);
        db = nullptr;
    }
}

/**
 * Check if database connection is active
 * @return true if connected, false otherwise
 */
bool DatabaseConnection::isConnected() {
    return db != nullptr;
}

// ============================================
// SQL EXECUTION METHODS
// ============================================

/**
 * Execute a simple SQL statement (for DDL - CREATE, ALTER, DROP)
 * Note: This is for statements that don't return results
 * @param sql SQL statement string
 * @return true if execution successful, false otherwise
 */
bool DatabaseConnection::executeSQL(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

/**
 * Prepare a SQL statement for execution with parameters
 * This prevents SQL injection attacks by using parameter binding
 * @param sql SQL statement with ? placeholders (e.g., "SELECT * FROM users WHERE id = ?")
 * @return Prepared statement pointer or nullptr if error
 * 
 * Usage Example:
 *   sqlite3_stmt* stmt = db->prepareStatement("SELECT * FROM users WHERE id = ?");
 *   sqlite3_bind_int(stmt, 1, userId);
 *   while (sqlite3_step(stmt) == SQLITE_ROW) { ... }
 *   sqlite3_finalize(stmt);
 */
sqlite3_stmt* DatabaseConnection::prepareStatement(const std::string& sql) {
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare statement error: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }
    return stmt;
}
