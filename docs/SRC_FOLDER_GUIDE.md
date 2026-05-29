# Source Folder Guide

## What is the `src/` folder?

The `src/` folder contains **implementation files** (`.cpp`) for the School Management System. These files contain the actual code that makes the system work - the logic, calculations, database queries, and user interface.

## How It Works

Each `.cpp` file in `src/` implements the interface defined in the corresponding `.h` header file in `include/`. When the program is compiled, all `.cpp` files are combined to create the final executable.

### Compilation Process
```
src/main.cpp + src/SchoolSystem.cpp + src/DatabaseConnection.cpp + src/User.cpp + src/Student.cpp + src/Session.cpp
    |
    v
  g++ compiler + SQLite library
    |
    v
build/school_management.exe
```

### Purpose of Each Source File

### 1. main.cpp
- **Role**: Entry point of the application
- **Function**: Creates a `SchoolSystem` object and calls `run()` to start the application
- **Why it's needed**: Every C++ program needs a `main()` function as the starting point

### 2. SchoolSystem.cpp
- **Role**: Main controller of the application
- **Function**: 
  - Manages the main menu loop
  - Handles user authentication (login/register)
  - Displays admin and user menus
  - Delegates operations to Student and Session classes
  - Initializes the database on first run

### 3. DatabaseConnection.cpp
- **Role**: Database manager (Singleton pattern)
- **Function**: 
  - Opens and closes SQLite database connections
  - Executes SQL statements
  - Prepares parameterized queries for security
  - Ensures only one database connection exists at a time

### 4. User.cpp
- **Role**: User authentication and management
- **Function**: 
  - Creates new user accounts (registration)
  - Validates login credentials
  - Retrieves user information from database
  - Manages user roles (USER vs ADMIN)

### 5. Student.cpp
- **Role**: Student enrollment management
- **Function**: 
  - Enrolls new students (reduces session slots)
  - Removes students (restores session slots)
  - Validates unique student numbers
  - Retrieves student information
  - Links students to sessions

### 6. Session.cpp
- **Role**: School session management
- **Function**: 
  - Creates new sessions with slot limits
  - Updates session information
  - Deletes sessions
  - Tracks available slots
  - Displays session information

## Relationship Between Files

```
main.cpp
  |
  v
SchoolSystem.cpp
  |
  +---> DatabaseConnection.cpp (database access)
  +---> User.cpp (authentication)
  +---> Session.cpp (session CRUD)
  +---> Student.cpp (enrollment CRUD)
```

## How Changes Flow

1. **User Input**: SchoolSystem.cpp receives menu choice
2. **Validation**: Input is validated in SchoolSystem.cpp
3. **Business Logic**: SchoolSystem.cpp calls methods on Student or Session
4. **Database Access**: Student/Session use DatabaseConnection to execute SQL
5. **Response**: Results flow back to SchoolSystem.cpp for display

## Adding New Features

To add a new feature:
1. Update the relevant `.h` file in `include/` to declare new methods
2. Implement the methods in the corresponding `.cpp` file in `src/`
3. Update `SchoolSystem.cpp` to call the new functionality from menus
4. Recompile the program
