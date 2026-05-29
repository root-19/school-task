# Include Folder Guide

## What is the `include/` folder?

The `include/` folder contains **header files** (`.h`) for the School Management System. Header files define the structure, interface, and public API of each class in the system.

## How It Works

Header files serve as **contracts** that tell the compiler what classes, methods, and variables exist in the program before compilation. They allow different source files to communicate with each other without needing to see the full implementation.

### Purpose of Each Header File

### 1. DatabaseConnection.h
- **Purpose**: Defines the singleton database connection class
- **Key Components**:
  - `DatabaseConnection` class with `getInstance()` method
  - SQLite connection management (connect, disconnect, executeSQL)
  - Prepared statement support for secure queries

### 2. User.h
- **Purpose**: Defines the user authentication class
- **Key Components**:
  - `User` class with properties: id, username, password, email, role
  - `UserRole` enum (USER, ADMIN)
  - Methods: save(), update(), findByUsername(), authenticate()

### 3. Session.h
- **Purpose**: Defines the school session class
- **Key Components**:
  - `Session` class with properties: id, name, section, totalSlots, availableSlots, modality
  - Methods: save(), update(), remove(), findById(), getAll()
  - Display methods for showing session information

### 4. Student.h
- **Purpose**: Defines the student enrollment class
- **Key Components**:
  - `Student` class with properties: id, studentNumber, name, section, sessionId, modality
  - Methods: enroll(), remove(), findByStudentNumber(), getAll()
  - Links to Session class via sessionId

### 5. SchoolSystem.h
- **Purpose**: Defines the main application controller
- **Key Components**:
  - `SchoolSystem` class that orchestrates all functionality
  - Menu display methods for admin and user roles
  - Database initialization
  - Input validation utilities

## Why Use Header Files?

1. **Separation of Interface and Implementation**: Header files show WHAT the class does, while `.cpp` files show HOW it does it
2. **Code Reusability**: Multiple source files can include the same header
3. **Compilation Efficiency**: Only changed source files need recompilation
4. **Type Safety**: The compiler can check type consistency across files

## How to Include Headers

In source files, headers are included using:
```cpp
#include "../include/ClassName.h"
```

The `../include/` path navigates up one directory level from `src/` to find the header files.
