# School Enrollment System - Technical Documentation

## Overview
**School Enrollment System** - Isang console-based C++ application para sa pag-manage ng student enrollment sa mga sessions. Gamit ang **SQLite** database para sa data persistence.

---
  
## System Architecture

### 1. Database Layer (SQLite)
**File:** `DatabaseConnection.h/cpp`

**Technique: Singleton Pattern**
```cpp
static DatabaseConnection* instance;
static DatabaseConnection* getInstance();
```
- **Purpose:** Isang database connection lang sa buong application
- **Benefit:** Iwas multiple connections, tipid resources

**Key Methods:**
- `connect()` - Open SQLite database
- `executeSQL()` - Run SQL with error logging
- `prepareStatement()` - Para sa parameterized queries (security vs SQL injection)

---

### 2. Entity Classes (Models)

#### **User Class** (`User.h/cpp`)
**Purpose:** Authentication at role-based access

**Key Features:**
- **Enum UserRole:** `USER` at `ADMIN` para sa role-based menu
- **Password Auth:** `authenticate()` method
- **Static Methods:** `findByUsername()`, `usernameExists()` - query without creating instance

```cpp
enum class UserRole { USER, ADMIN };
```

#### **Session Class** (`Session.h/cpp`)
**Purpose:** Manage available sessions/sections

**Key Features:**
- **Slot Management:** `totalSlots` at `availableSlots`
- **Auto-update:** Kapag may nag-enroll, `availableSlots` bumabawas
- **Learning Modality:** Face-to-Face, Online, Hybrid

#### **Student Class** (`Student.h/cpp`)
**Purpose:** Student data at enrollment

**Key Features:**
- **Auto-generated Student Number:** `generateStudentNumber()`
- **Session Assignment:** Naka-link sa `sessionId`
- **Validation:** `studentNumberExists()` - check bago mag-save

---

### 3. Controller Layer

#### **SchoolSystem Class** (`SchoolSystem.h/cpp`)
**Purpose:** Main controller, menu system, at business logic

**Architecture Pattern:** Composition
```cpp
private:
    DatabaseConnection* db;  // Database access
    User* currentUser;       // Current logged-in user
```

---

## Key Techniques Used

### 1. **Singleton Pattern** (DatabaseConnection)
```cpp
static DatabaseConnection* getInstance() {
    if (instance == nullptr) {
        instance = new DatabaseConnection();
    }
    return instance;
}
```

### 2. **Static Factory Methods**
```cpp
// Query without instantiating
User* user = User::findByUsername("john");
Student* student = Student::findById(1);
```

### 3. **Enum for Type Safety**
```cpp
enum class UserRole { USER, ADMIN };
// Hindi pwede mag-assign ng random int
user.setRole(UserRole::ADMIN);
```

### 4. **Prepared Statements (SQL Injection Protection)**
```cpp
sqlite3_stmt* stmt = db->prepareStatement(
    "SELECT * FROM users WHERE username = ?"
);
sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
```

### 5. **RAII Pattern**
```cpp
// Destructor auto-cleanup
~DatabaseConnection() { disconnect(); }
```

---

## Database Schema

```sql
-- Users Table
CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL,
    email TEXT UNIQUE NOT NULL,
    role INTEGER DEFAULT 0  -- 0=USER, 1=ADMIN
);

-- Sessions Table (Available sections/slots)
CREATE TABLE sessions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    session_name TEXT NOT NULL,
    section TEXT NOT NULL,
    total_slots INTEGER DEFAULT 30,
    available_slots INTEGER DEFAULT 30,
    learning_modality TEXT
);

-- Students Table
CREATE TABLE students (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    student_number TEXT UNIQUE NOT NULL,
    student_name TEXT NOT NULL,
    gender TEXT,
    age INTEGER,
    section TEXT,
    session_id INTEGER,
    learning_modality TEXT,
    FOREIGN KEY (session_id) REFERENCES sessions(id)
);
```

---

## Feature Highlights

### **1. Role-Based Access Control**
| Role | Features |
|------|----------|
| **USER** | Enroll student, View sessions, View own students |
| **ADMIN** | Add/Edit/Delete sessions, View all students, Manage users |

### **2. Auto Slot Management**
- Kapag may nag-enroll → `availableSlots` - 1
- Kapag nag-delete ng student → `availableSlots` + 1
- Hindi pwede mag-enroll pag zero na slots

### **3. Input Validation**
- Empty string check
- Duplicate username/email check
- Duplicate student number check
- Numeric input validation

### **4. Data Integrity**
- Foreign key constraints
- Unique constraints (username, email, student_number)
- Cascade-like behavior via code

---

## File Structure

```
ticket-booking/
├── include/           # Header files (.h)
│   ├── DatabaseConnection.h
│   ├── SchoolSystem.h
│   ├── Session.h
│   ├── Student.h
│   └── User.h
├── src/               # Implementation (.cpp)
│   ├── DatabaseConnection.cpp
│   ├── SchoolSystem.cpp
│   ├── Session.cpp
│   ├── Student.cpp
│   ├── User.cpp
│   └── main.cpp
└── build/             # Compiled output
```

---

## OOP Principles Applied

| Principle | Implementation |
|-----------|---------------|
| **Encapsulation** | Private fields, public getters/setters |
| **Abstraction** | Class interfaces hide complex SQL |
| **Single Responsibility** | Each class has one job |
| **DRY (Don't Repeat)** | Static methods, reusable queries |

---

## Demo Flow

```
1. Login/Register
   └── Guest → Register new account
   └── Existing → Login

2. Menu Display (depends on role)
   └── USER Menu:
       ├── View Available Sessions
       ├── Enroll Student
       ├── View My Students
       └── Logout
   
   └── ADMIN Menu:
       ├── Manage Sessions (CRUD)
       ├── View All Students
       ├── View All Users
       └── Logout

3. Enrollment Process:
   └── Input student details
   └── Select session (with available slots check)
   └── Generate student number (auto)
   └── Save to database
   └── Update available slots
```

---

## Technical Stack

| Component | Technology |
|-----------|------------|
| Language | C++11/14 |
| Database | SQLite3 |
| Build | g++ / Makefile |
| Platform | Windows/Linux |

---

## 📚 OOP Principles Explained (Tagalog)

### **1. Encapsulation (Pagbabalot ng Data)**
**Ano:** Pagtatago ng internal data at pag-provide ng controlled access

**Paano sa Code:**
```cpp
private:
    std::string password;    // Nakatago, di direktang ma-access
    
public:
    std::string getPassword() const;  // Controlled access lang
```

**Purpose:** 
- Proteksyon ng sensitive data
- Walang random changes sa loob
- Controlled lang ang data flow

---

### **2. Abstraction (Pagtatago ng Complexity)**
**Ano:** Simple interface lang ang pinapakita, complex logic nakatago

**Paano sa Code:**
```cpp
// User side - simple lang:
user.save();  // Ganyan lang!

// Pero sa loob - complex SQL:
bool User::save() {
    std::string sql = "INSERT INTO users (...) VALUES (...)";
    // ... maraming code dito
}
```

**Purpose:**
- User di na stress sa complex details
- Focus lang sa "what to do" hindi "how to do"

---

### **3. Single Responsibility (Isang Trabaho Lang)**
**Ano:** Bawat class, isang specific job lang

**Sa System natin:**
| Class | Trabaho Niya |
|-------|-------------|
| `DatabaseConnection` | Database access lang |
| `User` | User data at auth |
| `Student` | Student management |
| `Session` | Session/section management |
| `SchoolSystem` | Menu at flow control |

**Purpose:**
- Madali i-maintain
- Kung may bug, alam mo agad saang file
- Organized ang code

---

### **4. DRY - Don't Repeat Yourself (Wag Maulit)**
**Ano:** Iwas duplicate code

**Paano sa Code:**
```cpp
// Hindi na kailangan mag-instance para mag-query:
User* user = User::findByUsername("john");  // Static method

// Ginagamit sa maraming lugar:
if (User::usernameExists("john")) { ... }
```

**Purpose:**
- Tipid sa lines of code
- Isang change lang, everywhere updated

---

## 📁 Ano ang .h at .cpp? Bakit Hiwalay?

### **.h Files (Header Files)**
**Laman:**
- Class declaration ("Ano ang meron sa class?")
- Function signatures ("Ano ang mga function?")
- Variable definitions
- Enum, constants

**Example:**
```cpp
// User.h - "Ano ang pwede gawin ng User?"
class User {
private:
    int id;
    std::string username;
public:
    User();                          // Constructor
    std::string getUsername() const; // Getter
    bool save();                     // Method
};
```

---

### **.cpp Files (Implementation/Source)**
**Laman:**
- Actual code/body ng functions ("Paano ginagawa?")
- SQL queries
- Business logic
- Algorithm

**Example:**
```cpp
// User.cpp - "Paano ginagawa ang save?"
bool User::save() {
    std::string sql = "INSERT INTO users...";
    // ... actual database code
    return db->executeSQL(sql);
}
```

---

### **Bakit Hiwalay?**

| Reason | Explanation |
|--------|-------------|
| **1. Separation of Interface vs Implementation** | .h = "Ano ang pwede?" / .cpp = "Paano gawin?" |
| **2. Faster Compilation** | Pag nag-change sa .cpp lang, di na kailangan i-recompile ang ibang files |
| **3. Code Reuse** | Iba-ibang .cpp pwede mag-include ng isang .h |
| **4. Information Hiding** | Users ng class di kailangan makita ang complex code |
| **5. Organization** | Maliwanag kung saan hahanapin |

**Analogy:**
```
.h File = Menu sa Restaurant (Ano ang available?)
.cpp File = Kitchen (Paano niluluto?)

Customer → Nakikita Menu lang (.h)
Chef → Gumagawa sa Kitchen (.cpp)
```

---

### **Paano Nag-uusap?**

```cpp
// File: main.cpp
#include "User.h"  // Include header para malaman kung ano ang User

int main() {
    User user;           // Gumamit ng declaration sa .h
    user.save();         // Tawagin ang function (nasa .cpp ang actual code)
}
```

**Linker:**
- Compile time: .h → alam kung ano ang structure
- Link time: .cpp → actual code ni-save sa executable

---

*End of Documentation*
