# School Management System Documentation

## Overview

The **School Management System** is a console-based application written in C++ that uses SQLite for data storage. It follows Object-Oriented Programming (OOP) principles and provides functionality for managing school sessions and student enrollments.

## Features

- **User Authentication**: Login and registration system with two roles (USER and ADMIN)
- **Session Management**: Create, update, and delete school sessions with slot tracking
- **Student Enrollment**: Enroll students with automatic slot reduction
- **Slot Management**: Automatically manages available slots when students enroll or are removed
- **Error Handling**: Validates for duplicate student numbers, invalid sessions, empty fields, and full sessions

## Default Admin Account

| Username | Password  | Email             | Role  |
|----------|-----------|-------------------|-------|
| admin    | admin123  | admin@school.edu  | ADMIN |

## How to Compile

### Using Batch Script
```batch
scripts\compile_school.bat
```

### Manual Compilation
```batch
g++ -std=c++11 -Iinclude src/main.cpp src/SchoolSystem.cpp src/DatabaseConnection.cpp src/User.cpp src/Student.cpp src/Session.cpp -lsqlite3 -o build/school_management.exe
```

## How to Run

```batch
cd build
school_management.exe
```

## Menu Structure

### Main Menu (Login/Register)
1. Login
2. Register
3. Exit

### Admin Menu
1. View All Sessions
2. Add New Session
3. Update Session
4. Delete Session
5. View All Students
6. Remove Student
7. View All Users
8. Logout

### User Menu
1. View Available Sessions
2. View Available Slots
3. Enroll Student
4. View All Students
5. Logout

## Data Flow

1. Application starts and connects to SQLite database
2. Database tables are created if they don't exist
3. Sample sessions are inserted if the database is new
4. User logs in with valid credentials
5. Based on role, admin or user menu is displayed
6. User performs operations (CRUD) on sessions and students
7. All changes are persisted in the SQLite database

## Database Structure

The system uses three main tables:
- **users** - Stores login credentials and roles
- **sessions** - Stores school session information with slot counts
- **students** - Stores student enrollment information linked to sessions

## File Structure

```
ticket-booking/
├── include/          # Header files (.h)
├── src/              # Source files (.cpp)
├── build/            # Compiled executable and database
├── scripts/          # Build scripts
└── docs/             # Documentation
```
