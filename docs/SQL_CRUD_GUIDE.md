# SQL CRUD Operations Guide

This guide explains how to perform CRUD (Create, Read, Update, Delete) operations using SQL commands in SQLite.

## Table of Contents
1. [Creating a Database](#creating-a-database)
2. [Creating Tables](#creating-tables)
3. [INSERT - Create Data](#insert---create-data)
4. [SELECT - Read Data](#select---read-data)
5. [UPDATE - Update Data](#update---update-data)
6. [DELETE - Delete Data](#delete---delete-data)

---

## Creating a Database

In SQLite, you don't explicitly "create" a database. The database file is automatically created when you connect to it.

### Command Line (sqlite3)
```bash
sqlite3 school_management.db
```

### C++ Code
```cpp
#include <sqlite3.h>

sqlite3* db;
int rc = sqlite3_open("school_management.db", &db);

if (rc != SQLITE_OK) {
    std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
    return 1;
}
```

**Note:** The file `school_management.db` will be automatically created if it doesn't exist.

---

## Creating Tables

### Basic Syntax
```sql
CREATE TABLE table_name (
    column1 datatype constraints,
    column2 datatype constraints,
    ...
);
```

### Example: Users Table
```sql
CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL,
    email TEXT UNIQUE NOT NULL,
    role TEXT DEFAULT 'USER' CHECK(role IN ('USER', 'ADMIN')),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

### Example: Sessions Table
```sql
CREATE TABLE sessions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    session_name TEXT UNIQUE NOT NULL,
    section TEXT NOT NULL,
    total_slots INTEGER NOT NULL,
    available_slots INTEGER NOT NULL,
    learning_modality TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

### Example: Students Table
```sql
CREATE TABLE students (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    student_number TEXT UNIQUE NOT NULL,
    student_name TEXT NOT NULL,
    section TEXT NOT NULL,
    session_id INTEGER NOT NULL,
    learning_modality TEXT NOT NULL,
    enrolled_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (session_id) REFERENCES sessions(id) ON DELETE CASCADE
);
```

### Common Constraints
- `PRIMARY KEY` - Unique identifier for each row
- `AUTOINCREMENT` - Automatically increments the ID
- `UNIQUE` - Values must be unique (no duplicates)
- `NOT NULL` - Column cannot be empty
- `DEFAULT value` - Default value if not specified
- `CHECK(condition)` - Custom validation rule
- `FOREIGN KEY` - Links to another table's primary key

---

## INSERT - Create Data

### Basic Syntax
```sql
INSERT INTO table_name (column1, column2, ...)
VALUES (value1, value2, ...);
```

### Example: Insert a User
```sql
INSERT INTO users (username, password, email, role)
VALUES ('admin', 'admin123', 'admin@school.edu', 'ADMIN');
```

### Example: Insert Multiple Users
```sql
INSERT INTO users (username, password, email, role) VALUES
    ('john_doe', 'password123', 'john@example.com', 'USER'),
    ('jane_smith', 'password456', 'jane@example.com', 'USER');
```

### Example: Insert a Session
```sql
INSERT INTO sessions (session_name, section, total_slots, available_slots, learning_modality)
VALUES ('Morning Session 2024', 'A', 30, 30, 'Face-to-Face');
```

### Example: Insert a Student
```sql
INSERT INTO students (student_number, student_name, section, session_id, learning_modality)
VALUES ('2024-001', 'Juan Dela Cruz', 'A', 1, 'Face-to-Face');
```

### C++ Code with Prepared Statements (Prevents SQL Injection)
```cpp
sqlite3_stmt* stmt;
const char* sql = "INSERT INTO users (username, password, email, role) VALUES (?, ?, ?, ?)";

int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
if (rc != SQLITE_OK) {
    std::cerr << "Prepare error: " << sqlite3_errmsg(db) << std::endl;
    return false;
}

// Bind parameters
sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 4, role.c_str(), -1, SQLITE_STATIC);

// Execute
rc = sqlite3_step(stmt);
if (rc != SQLITE_DONE) {
    std::cerr << "Insert error: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_finalize(stmt);
    return false;
}

sqlite3_finalize(stmt);
return true;
```

---

## SELECT - Read Data

### Basic Syntax
```sql
SELECT column1, column2, ...
FROM table_name
WHERE condition;
```

### Example: Select All Users
```sql
SELECT * FROM users;
```

### Example: Select Specific Columns
```sql
SELECT username, email, role FROM users;
```

### Example: Select with WHERE Condition
```sql
SELECT * FROM users WHERE username = 'admin';
```

### Example: Select with Multiple Conditions
```sql
SELECT * FROM users WHERE role = 'ADMIN' AND email LIKE '%@school.edu';
```

### Example: Select with ORDER BY
```sql
SELECT * FROM users ORDER BY username ASC;
```

### Example: Select with LIMIT
```sql
SELECT * FROM users LIMIT 10;
```

### Example: Join Tables
```sql
SELECT students.student_name, sessions.session_name
FROM students
JOIN sessions ON students.session_id = sessions.id;
```

### C++ Code with Prepared Statements
```cpp
sqlite3_stmt* stmt;
const char* sql = "SELECT * FROM users WHERE username = ?";

int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
if (rc != SQLITE_OK) {
    std::cerr << "Prepare error: " << sqlite3_errmsg(db) << std::endl;
    return nullptr;
}

// Bind parameter
sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

// Execute and iterate through results
while (sqlite3_step(stmt) == SQLITE_ROW) {
    int id = sqlite3_column_int(stmt, 0);
    const char* username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    const char* password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    const char* email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    const char* role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    
    std::cout << "ID: " << id << ", Username: " << username << std::endl;
}

sqlite3_finalize(stmt);
```

---

## UPDATE - Update Data

### Basic Syntax
```sql
UPDATE table_name
SET column1 = value1, column2 = value2, ...
WHERE condition;
```

### Example: Update User Password
```sql
UPDATE users
SET password = 'newpassword123'
WHERE username = 'admin';
```

### Example: Update Multiple Columns
```sql
UPDATE users
SET email = 'newemail@example.com', role = 'ADMIN'
WHERE id = 1;
```

### Example: Update Session Slots
```sql
UPDATE sessions
SET available_slots = available_slots - 1
WHERE id = 1;
```

### Example: Update with Condition
```sql
UPDATE students
SET section = 'B'
WHERE section = 'A' AND session_id = 1;
```

### C++ Code with Prepared Statements
```cpp
sqlite3_stmt* stmt;
const char* sql = "UPDATE users SET password = ? WHERE username = ?";

int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
if (rc != SQLITE_OK) {
    std::cerr << "Prepare error: " << sqlite3_errmsg(db) << std::endl;
    return false;
}

// Bind parameters
sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);

// Execute
rc = sqlite3_step(stmt);
if (rc != SQLITE_DONE) {
    std::cerr << "Update error: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_finalize(stmt);
    return false;
}

sqlite3_finalize(stmt);
return true;
```

---

## DELETE - Delete Data

### Basic Syntax
```sql
DELETE FROM table_name
WHERE condition;
```

### Example: Delete a User
```sql
DELETE FROM users WHERE id = 1;
```

### Example: Delete by Username
```sql
DELETE FROM users WHERE username = 'john_doe';
```

### Example: Delete with Multiple Conditions
```sql
DELETE FROM students
WHERE session_id = 1 AND section = 'A';
```

### Example: Delete All Rows (Use with caution!)
```sql
DELETE FROM users;
```

### Example: Delete Table (Drop entire table)
```sql
DROP TABLE users;
```

### C++ Code with Prepared Statements
```cpp
sqlite3_stmt* stmt;
const char* sql = "DELETE FROM users WHERE id = ?";

int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
if (rc != SQLITE_OK) {
    std::cerr << "Prepare error: " << sqlite3_errmsg(db) << std::endl;
    return false;
}

// Bind parameter
sqlite3_bind_int(stmt, 1, userId);

// Execute
rc = sqlite3_step(stmt);
if (rc != SQLITE_DONE) {
    std::cerr << "Delete error: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_finalize(stmt);
    return false;
}

sqlite3_finalize(stmt);
return true;
```

---

## SQLite-Specific Features

### INSERT OR IGNORE (Skip if exists)
```sql
INSERT OR IGNORE INTO users (username, password, email, role)
VALUES ('admin', 'admin123', 'admin@school.edu', 'ADMIN');
```

### INSERT OR REPLACE (Update if exists)
```sql
INSERT OR REPLACE INTO users (id, username, password, email, role)
VALUES (1, 'admin', 'newpassword', 'admin@school.edu', 'ADMIN');
```

### LIMIT and OFFSET (Pagination)
```sql
SELECT * FROM users LIMIT 10 OFFSET 20;
-- Returns rows 21-30
```

### LIKE Pattern Matching
```sql
SELECT * FROM users WHERE email LIKE '%@gmail.com';
-- Matches any email ending with @gmail.com

SELECT * FROM users WHERE username LIKE 'john%';
-- Matches any username starting with 'john'
```

### IN Clause
```sql
SELECT * FROM users WHERE role IN ('USER', 'ADMIN');
```

### BETWEEN Clause
```sql
SELECT * FROM sessions WHERE total_slots BETWEEN 20 AND 30;
```

---

## Best Practices

### 1. Always Use Prepared Statements
Prevents SQL injection attacks:
```cpp
// BAD (Vulnerable to SQL injection)
std::string sql = "SELECT * FROM users WHERE username = '" + username + "'";

// GOOD (Safe with prepared statements)
sqlite3_prepare_v2(db, "SELECT * FROM users WHERE username = ?", -1, &stmt, nullptr);
sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
```

### 2. Use Transactions for Multiple Operations
```cpp
// Begin transaction
sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);

// Perform multiple operations
// ...

// Commit if all successful
sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr);

// Or rollback if error occurs
sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, nullptr);
```

### 3. Always Finalize Statements
```cpp
sqlite3_finalize(stmt);  // Free memory
```

### 4. Close Database Connection
```cpp
sqlite3_close(db);
```

### 5. Check Return Values
```cpp
if (rc != SQLITE_OK) {
    std::cerr << "Error: " << sqlite3_errmsg(db) << std::endl;
}
```

---

## Common SQLite Return Codes

- `SQLITE_OK` (0) - Successful
- `SQLITE_ERROR` (1) - SQL error or missing database
- `SQLITE_ROW` (100) - Another row ready (for SELECT)
- `SQLITE_DONE` (101) - Execution complete (for INSERT/UPDATE/DELETE)
- `SQLITE_CONSTRAINT` (19) - Constraint violation
- `SQLITE_MISMATCH` (20) - Data type mismatch

---

## Quick Reference

| Operation | SQL Command | C++ Function |
|-----------|-------------|--------------|
| Create Table | `CREATE TABLE` | `sqlite3_exec()` |
| Insert | `INSERT INTO` | `sqlite3_prepare_v2()` + `sqlite3_step()` |
| Select | `SELECT` | `sqlite3_prepare_v2()` + `sqlite3_step()` |
| Update | `UPDATE` | `sqlite3_prepare_v2()` + `sqlite3_step()` |
| Delete | `DELETE` | `sqlite3_prepare_v2()` + `sqlite3_step()` |

---

## Additional Resources

- [SQLite Official Documentation](https://www.sqlite.org/docs.html)
- [SQLite C/C++ Interface](https://www.sqlite.org/cintro.html)
- [W3Schools SQL Tutorial](https://www.w3schools.com/sql/)
