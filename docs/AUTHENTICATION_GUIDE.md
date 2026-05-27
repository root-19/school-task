# Authentication System Documentation
<!-- ./school_management.exe -->
## Overview
The School Management System implements a secure authentication and authorization system with Role-Based Access Control (RBAC). This document explains how the login and registration functionality works.

## System Architecture

### Components
1. **User Class** (`User.h`, `User.cpp`) - Manages user data and database operations
2. **Auth Class** (`Auth.h`, `Auth.cpp`) - Handles authentication logic
3. **Database** - SQLite database with `users` table

### Header Files (.h) vs Implementation Files (.cpp)

In C++, classes are typically split into two files:

**User.h (Header File):**
- Contains class declaration (interface)
- Defines the class structure, member variables, and method signatures
- Does not contain implementation logic
- Included by other files that need to use the class
- Acts as a "contract" showing what the class can do

**User.cpp (Implementation File):**
- Contains the actual implementation of methods
- Defines how each method works
- Compiled separately and linked together
- Includes the header file to access class declarations

**Example:**

User.h (Declaration):
```cpp
class User {
private:
    int id;
    std::string username;
public:
    int getId() const;           // Method signature only
    void setId(int id);          // No implementation here
};
```

User.cpp (Implementation):
```cpp
#include "User.h"

int User::getId() const {        // Actual implementation
    return id;
}

void User::setId(int id) {       // Actual implementation
    this->id = id;
}
```

**Why Split Files?**
- **Modularity** - Easier to organize and maintain code
- **Compilation Speed** - Changes to .cpp don't require recompiling files that only include .h
- **Encapsulation** - Header shows interface, implementation hides details
- **Reusability** - Other files can include the header without seeing implementation

### User Roles
- **USER** - Regular users who can view sessions and enroll students
- **ADMIN** - Administrators with full access to manage sessions and users

## Database Schema

### Users Table
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

**Fields:**
- `id` - Unique identifier for each user
- `username` - Unique username for login
- `password` - Plain text password (for demonstration purposes)
- `email` - Unique email address
- `role` - User role (USER or ADMIN)
- `created_at` - Account creation timestamp

## Registration Process

### How It Works

1. **User Input Collection**
   - Username
   - Password (minimum 6 characters)
   - Email address

2. **Validation Steps**
   ```cpp
   // Check if username already exists
   if (User::usernameExists(username)) {
       std::cout << "Username already exists!" << std::endl;
       return false;
   }
   
   // Check if email already exists
   if (User::emailExists(email)) {
       std::cout << "Email already registered!" << std::endl;
       return false;
   }
   
   // Validate password length
   if (password.length() < 6) {
       std::cout << "Password must be at least 6 characters!" << std::endl;
       return false;
   }
   ```

3. **User Creation**
   - Creates a new `User` object with provided data
   - Default role is `USER`
   - Saves to database using SQLite prepared statements

4. **Database Insertion**
   ```cpp
   sqlite3_stmt* stmt = db->prepareStatement(
       "INSERT INTO users (username, password, email, role) VALUES (?, ?, ?, ?)"
   );
   
   sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
   sqlite3_bind_text(stmt, 4, getRoleString().c_str(), -1, SQLITE_STATIC);
   ```

### Registration Flow Diagram
```
User enters credentials
         ↓
Validate username uniqueness
         ↓
Validate email uniqueness
         ↓
Validate password length
         ↓
Create User object
         ↓
Save to database
         ↓
Success message
```

## Login Process

### How It Works

1. **User Input Collection**
   - Username
   - Password

2. **User Lookup**
   ```cpp
   User* user = User::findByUsername(username);
   
   if (user == nullptr) {
       std::cout << "User not found!" << std::endl;
       return nullptr;
   }
   ```

3. **Password Verification**
   ```cpp
   if (user->authenticate(password)) {
       currentUser = user;
       std::cout << "Login successful! Welcome, " << user->getUsername() << "!" << std::endl;
       std::cout << "Role: " << user->getRoleString() << std::endl;
       return user;
   } else {
       std::cout << "Incorrect password!" << std::endl;
       delete user;
       return nullptr;
   }
   ```

4. **Session Management**
   - On successful login, user is stored in `Auth::currentUser`
   - User remains logged in until logout or program exit
   - Role is checked for menu access control

### Login Flow Diagram
```
User enters credentials
         ↓
Find user by username
         ↓
User exists?
    ↓ No ↓ Yes
Error message  ↓
               ↓
         Verify password
               ↓
         Password correct?
         ↓ No ↓ Yes
      Error message  ↓
                     ↓
              Set current user
                     ↓
              Show role-based menu
```

## Role-Based Access Control (RBAC)

### Implementation

The system uses RBAC to control access to different features:

```cpp
bool Auth::isAdmin() {
    return currentUser != nullptr && currentUser->isAdmin();
}
```

### Access Levels

**USER Role:**
- View available sessions
- View available slots
- Enroll students
- View all enrolled students
- Logout

**ADMIN Role:**
- All USER permissions
- View all sessions
- Add new sessions
- Update sessions
- Delete sessions
- Remove students
- View all users

### Menu Access Control

```cpp
if (Auth::isLoggedIn()) {
    if (Auth::isAdmin()) {
        displayAdminMenu();
    } else {
        displayUserMenu();
    }
}
```

## Security Features

### Current Implementation
- Unique username constraint
- Unique email constraint
- Minimum password length (6 characters)
- Role-based access control
- Session management

### Note on Security
**Important:** This is a demonstration system. In a production environment, you should implement:
- Password hashing (e.g., bcrypt, Argon2)
- Session tokens
- HTTPS encryption
- Rate limiting
- Input sanitization
- SQL injection prevention (already implemented via prepared statements)

## Code Structure

### User Class Methods

**Constructor:**
```cpp
User(int id, const std::string& username, const std::string& password, 
     const std::string& email, UserRole role)
```

**Database Operations:**
- `save()` - Insert new user into database
- `update()` - Update existing user
- `remove()` - Delete user from database
- `findById(int id)` - Find user by ID
- `findByUsername(string username)` - Find user by username

**Validation:**
- `usernameExists(string username)` - Check username uniqueness
- `emailExists(string email)` - Check email uniqueness
- `authenticate(string password)` - Verify password

**Getters/Setters:**
- `getId()`, `getUsername()`, `getPassword()`, `getEmail()`, `getRole()`
- `getRoleString()` - Returns "USER" or "ADMIN"
- `isAdmin()` - Returns true if role is ADMIN

### Auth Class Methods

**Static Methods:**
- `registerUser(username, password, email, role)` - Register new user
- `login(username, password)` - Authenticate user
- `logout()` - End user session
- `getCurrentUser()` - Get currently logged-in user
- `isLoggedIn()` - Check if user is logged in
- `isAdmin()` - Check if current user is admin

## Default Admin Account

The system creates a default administrator account on first run:

- **Username:** `admin`
- **Password:** `admin123`
- **Email:** `admin@school.edu`
- **Role:** `ADMIN`

This account is automatically inserted into the database during initialization.

## Usage Example

### Registration
```cpp
// User registration
bool success = Auth::registerUser("john_doe", "password123", "john@example.com", UserRole::USER);
if (success) {
    std::cout << "Registration successful!" << std::endl;
}
```

### Login
```cpp
// User login
User* user = Auth::login("john_doe", "password123");
if (user != nullptr) {
    std::cout << "Logged in as: " << user->getUsername() << std::endl;
    std::cout << "Role: " << user->getRoleString() << std::endl;
}
```

### Logout
```cpp
// User logout
Auth::logout();
```

## Database Operations

### Prepared Statements
All database operations use SQLite prepared statements to prevent SQL injection:

```cpp
sqlite3_stmt* stmt = db->prepareStatement("SELECT * FROM users WHERE username = ?");
sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
```

### Transaction Safety
The enrollment process uses transactions to ensure data consistency:
```cpp
db->executeSQL("BEGIN TRANSACTION");
// ... perform operations ...
db->executeSQL("COMMIT");
```

## Error Handling

The system handles various error scenarios:

1. **Duplicate Username** - "Username already exists!"
2. **Duplicate Email** - "Email already registered!"
3. **Short Password** - "Password must be at least 6 characters!"
4. **User Not Found** - "User not found!"
5. **Incorrect Password** - "Incorrect password!"
6. **Database Errors** - Generic error messages with stderr output

## Future Enhancements

Potential improvements for the authentication system:

1. **Password Hashing** - Implement bcrypt or Argon2
2. **Password Reset** - Add password recovery functionality
3. **Email Verification** - Verify email addresses during registration
4. **Two-Factor Authentication** - Add 2FA support
5. **Session Expiration** - Implement automatic logout after inactivity
6. **Account Locking** - Lock accounts after failed login attempts
7. **Audit Logging** - Log all authentication events

## Conclusion

The authentication system provides a solid foundation for user management in the School Management System. It implements basic security features, role-based access control, and proper database operations using SQLite. The system is designed to be easily extensible for future security enhancements.
