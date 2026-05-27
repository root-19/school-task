# School Management System - C++ Console Application

A console-based school management system built with C++ using Object-Oriented Programming (OOP) principles, connected to MySQL database with CRUD operations, student enrollment, session slot management, and Role-Based Access Control (RBAC).

## Features

### User Authentication (RBAC)
- User registration and login
- Role-based access (User and Admin)
- Session management

### User Features
- View available sessions
- View available slots per session
- Enroll students with automatic slot reduction
- View all enrolled students

### Admin Features
- All user features plus:
- Add new sessions (Create)
- Update existing sessions (Update)
- Delete sessions (Delete)
- Remove students (with automatic slot restoration)
- View all registered users

### Student Enrollment System
- Collects: student name, student number, section, session, learning modality
- Student number must be unique
- Automatic slot count reduction on enrollment
- Automatic slot count increase on student removal
- Error handling for:
  - Duplicate student numbers
  - Invalid session names
  - Empty fields
  - Full session slots

### Database Integration
- MySQL connectivity using MySQL Connector/C++
- CRUD operations for all entities
- Transaction support for enrollment operations
- Foreign key constraints for data integrity

## Prerequisites

- C++ compiler (g++, MinGW, MSYS2, or Visual Studio)
- MySQL Server installed and running
- MySQL Connector/C++ library

## Installation

### 1. Install MySQL Connector/C++

#### Windows (using vcpkg):
```bash
vcpkg install mysql-connector-cpp
```

#### Linux (Ubuntu/Debian):
```bash
sudo apt-get install libmysqlcppconn-dev
```

#### macOS (using Homebrew):
```bash
brew install mysql-connector-c++
```

#### MSYS2:
```bash
pacman -S mingw-w64-ucrt-x86_64-connector-cpp
```

### 2. Setup Database

1. Start MySQL server
2. Run the setup script:
```bash
mysql -u root -p < setup_school.sql
```

Or manually execute the SQL commands in `setup_school.sql` using MySQL Workbench or command line.

**Default Admin Credentials:**
- Username: `admin`
- Password: `admin123`

### 3. Configure Database Connection

Edit `main_school.cpp` and update the database connection parameters:
```cpp
std::string host = "localhost";
std::string user = "root";        // Your MySQL username
std::string password = "";        // Your MySQL password
std::string database = "school_management";
```

### 4. Compile the Project

#### Using g++ (Linux/macOS):
```bash
g++ -std=c++11 -I/usr/include/cppconn -L/usr/lib main_school.cpp DatabaseConnection.cpp User.cpp Student.cpp Session.cpp Auth.cpp -lmysqlcppconn -o school_management
```

#### Using g++ (Windows with MinGW):
```bash
compile_school.bat
```

#### Using MSYS2:
```bash
bash compile_school_msys2.sh
```

#### Using Visual Studio:
1. Create a new C++ Console Application project
2. Add all `.cpp` and `.h` files to the project
3. Configure project properties:
   - C/C++ > Additional Include Directories: Add MySQL Connector include path
   - Linker > Additional Library Directories: Add MySQL Connector lib path
   - Linker > Additional Dependencies: Add `mysqlcppconn.lib`
4. Build and run

## Usage

### Running the Application

```bash
./school_management        # Linux/macOS/MSYS2
school_management.exe       # Windows
```

### User Workflow

1. **Register** (if you don't have an account)
   - Choose option 2 from main menu
   - Enter username, password (min 6 characters), and email

2. **Login**
   - Choose option 1 from main menu
   - Enter your credentials

3. **Enroll Student**
   - View available sessions (option 1)
   - View available slots (option 2)
   - Enroll a student by entering:
     - Student number (must be unique)
     - Student name
     - Section
     - Session ID
     - Learning modality

4. **View Students**
   - View all enrolled students (option 4)

### Admin Workflow

1. **Login as Admin**
   - Use default admin credentials or create a new admin user

2. **Manage Sessions**
   - View all sessions (option 1)
   - Add new session (option 2)
   - Update existing session (option 3)
   - Delete session (option 4)

3. **Manage Students**
   - View all students (option 5)
   - Remove student (option 6) - automatically restores slot

4. **View System Data**
   - View all users (option 7)

## Error Handling

The system includes comprehensive error handling:

- **Duplicate Student Numbers**: Prevents enrollment if student number already exists
- **Invalid Session Names**: Validates session existence before enrollment
- **Empty Fields**: Checks for empty input in all required fields
- **Full Session Slots**: Prevents enrollment when no slots are available
- **Database Transactions**: Ensures data consistency during enrollment/removal

## Project Structure

```
ticket-booking/
├── main_school.cpp           # Main application entry point and UI
├── DatabaseConnection.h      # Database connection management header
├── DatabaseConnection.cpp    # Database connection implementation
├── User.h                    # User class header with RBAC
├── User.cpp                  # User class implementation
├── Student.h                 # Student class header
├── Student.cpp               # Student class implementation
├── Session.h                 # Session class header
├── Session.cpp               # Session class implementation
├── Auth.h                    # Authentication system header
├── Auth.cpp                  # Authentication implementation
├── setup_school.sql          # Database setup script
├── compile_school.bat        # Windows compilation script
├── compile_school_msys2.sh   # MSYS2 compilation script
└── README_SCHOOL.md          # This file
```

## Database Schema

### Users Table
- `id`: Primary key
- `username`: Unique username
- `password`: User password
- `email`: Unique email address
- `role`: USER or ADMIN

### Sessions Table
- `id`: Primary key
- `session_name`: Unique session name
- `section`: Section identifier
- `total_slots`: Total number of slots
- `available_slots`: Number of available slots
- `learning_modality`: Face-to-Face, Online, or Hybrid

### Students Table
- `id`: Primary key
- `student_number`: Unique student number
- `student_name`: Student's full name
- `section`: Section identifier
- `session_id`: Foreign key to sessions table
- `learning_modality`: Learning modality
- `enrolled_at`: Timestamp of enrollment

## Security Notes

⚠️ **Important:** This is a demonstration project. For production use:

1. Implement proper password hashing (e.g., bcrypt, Argon2)
2. Use prepared statements (already implemented)
3. Implement input validation and sanitization
4. Add SSL/TLS for database connections
5. Implement session timeout
6. Add rate limiting for login attempts
7. Use environment variables for sensitive configuration

## Troubleshooting

### Connection Issues
- Ensure MySQL server is running
- Verify database credentials in `main_school.cpp`
- Check if the database `school_management` exists

### Compilation Errors
- Verify MySQL Connector/C++ is properly installed
- Check include and library paths in your compiler settings
- Ensure all required libraries are linked

### Runtime Errors
- Check database connection before running
- Verify all tables exist in the database
- Ensure the default admin user exists

## License

This project is for educational purposes.

## Author

Created as a demonstration of C++ OOP, CRUD operations, student enrollment system, and MySQL integration with RBAC.
