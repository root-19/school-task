# File Functions Guide

## Detailed Explanation of Every File and Function

---

## 1. include/DatabaseConnection.h

### Class: DatabaseConnection
**Pattern**: Singleton (only one instance exists)

| Function | Return Type | Description |
|----------|-------------|-------------|
| `getInstance()` | `DatabaseConnection*` | Returns the single database connection instance |
| `getConnection()` | `sqlite3*` | Returns the raw SQLite connection handle |
| `connect(path)` | `bool` | Opens connection to database file |
| `disconnect()` | `void` | Closes database connection |
| `isConnected()` | `bool` | Checks if database is connected |
| `executeSQL(sql)` | `bool` | Executes a SQL statement directly |
| `prepareStatement(sql)` | `sqlite3_stmt*` | Prepares a parameterized query for safe execution |

**Why Singleton?**: Prevents multiple database connections that could cause data corruption.

---

## 2. include/User.h

### Class: User
**Represents**: A system user (student or admin)

| Property | Type | Description |
|----------|------|-------------|
| `id` | `int` | Unique identifier (auto-generated) |
| `username` | `string` | Login username (unique) |
| `password` | `string` | Login password |
| `email` | `string` | User email (unique) |
| `role` | `UserRole` | USER or ADMIN enum |

| Function | Return Type | Description |
|----------|-------------|-------------|
| `getId()` | `int` | Returns user ID |
| `getUsername()` | `string` | Returns username |
| `getPassword()` | `string` | Returns password |
| `getEmail()` | `string` | Returns email |
| `getRole()` | `UserRole` | Returns role enum |
| `getRoleString()` | `string` | Returns "USER" or "ADMIN" text |
| `setId(id)` | `void` | Sets user ID |
| `setUsername(name)` | `void` | Sets username |
| `setPassword(pass)` | `void` | Sets password |
| `setEmail(email)` | `void` | Sets email |
| `setRole(role)` | `void` | Sets role |
| `authenticate(password)` | `bool` | Checks if password matches |
| `save()` | `bool` | Inserts new user into database |
| `update()` | `bool` | Updates existing user in database |
| `findByUsername(name)` | `User*` | Finds user by username (returns pointer) |
| `findById(id)` | `User*` | Finds user by ID (returns pointer) |
| `usernameExists(name)` | `bool` | Checks if username is taken |
| `emailExists(email)` | `bool` | Checks if email is registered |
| `isAdmin()` | `bool` | Returns true if user is admin |

---

## 3. include/Session.h

### Class: Session
**Represents**: A school session (class schedule)

| Property | Type | Description |
|----------|------|-------------|
| `id` | `int` | Unique session ID |
| `sessionName` | `string` | Name of the session |
| `section` | `string` | Section identifier |
| `totalSlots` | `int` | Maximum number of students |
| `availableSlots` | `int` | Current available spots |
| `learningModality` | `string` | Face-to-Face, Online, or Hybrid |

| Function | Return Type | Description |
|----------|-------------|-------------|
| `getId()` | `int` | Returns session ID |
| `getSessionName()` | `string` | Returns session name |
| `getSection()` | `string` | Returns section |
| `getTotalSlots()` | `int` | Returns total slots |
| `getAvailableSlots()` | `int` | Returns available slots |
| `getLearningModality()` | `string` | Returns modality |
| `setId(id)` | `void` | Sets session ID |
| `setSessionName(name)` | `void` | Sets session name |
| `setSection(section)` | `void` | Sets section |
| `setTotalSlots(slots)` | `void` | Sets total slots |
| `setAvailableSlots(slots)` | `void` | Sets available slots |
| `setLearningModality(modality)` | `void` | Sets modality |
| `save()` | `bool` | Inserts new session into database |
| `update()` | `bool` | Updates session in database |
| `remove()` | `bool` | Deletes session from database |
| `findById(id)` | `Session*` | Finds session by ID |
| `findBySessionName(name)` | `Session*` | Finds session by name |
| `getAll()` | `vector<Session*>` | Returns all sessions |
| `sessionNameExists(name)` | `bool` | Checks if session name exists |
| `display()` | `void` | Prints single session to console |
| `displayAll()` | `void` | Prints all sessions to console |
| `displayAvailableSlots()` | `void` | Prints sessions with slot status |

---

## 4. include/Student.h

### Class: Student
**Represents**: An enrolled student

| Property | Type | Description |
|----------|------|-------------|
| `id` | `int` | Unique student ID |
| `studentNumber` | `string` | Unique student identifier |
| `studentName` | `string` | Full name of student |
| `section` | `string` | Assigned section |
| `sessionId` | `int` | Linked session ID |
| `learningModality` | `string` | Chosen learning modality |

| Function | Return Type | Description |
|----------|-------------|-------------|
| `getId()` | `int` | Returns student ID |
| `getStudentNumber()` | `string` | Returns student number |
| `getStudentName()` | `string` | Returns student name |
| `getSection()` | `string` | Returns section |
| `getSessionId()` | `int` | Returns linked session ID |
| `getLearningModality()` | `string` | Returns modality |
| `setId(id)` | `void` | Sets student ID |
| `setStudentNumber(num)` | `void` | Sets student number |
| `setStudentName(name)` | `void` | Sets student name |
| `setSection(section)` | `void` | Sets section |
| `setSessionId(id)` | `void` | Sets session ID |
| `setLearningModality(modality)` | `void` | Sets modality |
| `enroll()` | `bool` | Enrolls student, reduces session slots |
| `remove()` | `bool` | Removes student, restores session slots |
| `findByStudentNumber(num)` | `Student*` | Finds student by number |
| `findById(id)` | `Student*` | Finds student by ID |
| `getAll()` | `vector<Student*>` | Returns all students |
| `getBySessionId(id)` | `vector<Student*>` | Returns students in a session |
| `studentNumberExists(num)` | `bool` | Checks if student number is taken |
| `display()` | `void` | Prints single student to console |
| `displayAll()` | `void` | Prints all students to console |

---

## 5. include/SchoolSystem.h

### Class: SchoolSystem
**Represents**: Main application controller

| Property | Type | Description |
|----------|------|-------------|
| `db` | `DatabaseConnection*` | Pointer to database connection |
| `currentUser` | `User*` | Currently logged-in user |

| Function | Return Type | Description |
|----------|-------------|-------------|
| `SchoolSystem()` | Constructor | Initializes database connection |
| `~SchoolSystem()` | Destructor | Cleans up resources |
| `initializeDatabase()` | `void` | Creates tables and sample data |
| `clearInputBuffer()` | `void` | Clears cin buffer for safe input |
| `isEmpty(str)` | `bool` | Checks if string is empty or whitespace |
| `handleLogin()` | `void` | Processes user login |
| `handleRegister()` | `void` | Processes user registration |
| `logout()` | `void` | Logs out current user |
| `displayUserMenu()` | `void` | Shows user menu options |
| `handleUserMenu()` | `void` | Handles user menu choices |
| `displayAdminMenu()` | `void` | Shows admin menu options |
| `handleAdminMenu()` | `void` | Handles admin menu choices |
| `displayAvailableSessions()` | `void` | Shows all sessions |
| `displayAvailableSlots()` | `void` | Shows sessions with available slots |
| `addSession()` | `void` | Handles adding a new session |
| `updateSession()` | `void` | Handles updating a session |
| `deleteSession()` | `void` | Handles deleting a session |
| `enrollStudent()` | `void` | Handles student enrollment |
| `displayAllStudents()` | `void` | Shows all enrolled students |
| `displayAllUsers()` | `void` | Shows all registered users |
| `removeStudent()` | `void` | Handles removing a student |
| `run()` | `void` | Starts the main application loop |

---

## 6. src/main.cpp

| Function | Description |
|----------|-------------|
| `main()` | Creates SchoolSystem and calls run() |

**Flow**:
1. Create `SchoolSystem system`
2. Call `system.run()`
3. Return 0 on success

---

## 7. src/DatabaseConnection.cpp

| Function | Description |
|----------|-------------|
| `DatabaseConnection()` | Private constructor (singleton) |
| `getInstance()` | Creates or returns single instance |
| `connect(path)` | Opens SQLite database file |
| `getConnection()` | Returns raw sqlite3 pointer |
| `disconnect()` | Closes database |
| `isConnected()` | Checks connection status |
| `executeSQL(sql)` | Runs SQL directly (for CREATE, INSERT, etc.) |
| `prepareStatement(sql)` | Creates safe parameterized query |

**Key Logic**: Uses `sqlite3_open()`, `sqlite3_exec()`, `sqlite3_prepare_v2()`, `sqlite3_step()`, and `sqlite3_finalize()` for database operations.

---

## 8. src/User.cpp

| Function | Description |
|----------|-------------|
| `authenticate(password)` | Compares stored password with input |
| `save()` | Inserts user using prepared statement |
| `update()` | Updates user record in database |
| `findByUsername(name)` | Queries database for username |
| `findById(id)` | Queries database for ID |
| `usernameExists(name)` | Checks if SELECT returns a row |
| `emailExists(email)` | Checks if SELECT returns a row |
| `isAdmin()` | Compares role with ADMIN enum |

**Key Logic**: All database queries use prepared statements with parameter binding to prevent SQL injection.

---

## 9. src/Session.cpp

| Function | Description |
|----------|-------------|
| `save()` | INSERT INTO sessions with all fields |
| `update()` | UPDATE sessions WHERE id = ? |
| `remove()` | DELETE FROM sessions WHERE id = ? |
| `findById(id)` | SELECT * FROM sessions WHERE id = ? |
| `findBySessionName(name)` | SELECT * FROM sessions WHERE session_name = ? |
| `getAll()` | SELECT * FROM sessions ORDER BY session_name |
| `sessionNameExists(name)` | Calls findBySessionName() |
| `display()` | Prints formatted session data |
| `displayAll()` | Prints header + calls display() for each session |
| `displayAvailableSlots()` | Prints sessions with Open/Full status |

**Key Logic**: `getAll()` creates Session objects from database rows using `sqlite3_column_int()` and `sqlite3_column_text()`.

---

## 10. src/Student.cpp

| Function | Description |
|----------|-------------|
| `enroll()` | Validates, starts transaction, INSERT student, UPDATE session slots, commits |
| `remove()` | Starts transaction, UPDATE session slots, DELTE student, commits |
| `findByStudentNumber(num)` | SELECT * FROM students WHEREE student_number = ? |
| `findById(id)` | SELECT * FROM students WHERE id = ? |
| `getAll()` | SELECT * FROM students ORDER BY enrolled_at DESC |
| `getBySessionId(id)` | SELECT * FROM students WHERE session_id = ? |
| `studentNumberExists(num)` | Calls findByStudentNumber() |
| `display()` | Prints formatted student data (looks up session name) |
| `displayAll()` | Prints header + calls display() for each student |

**Key Logic**: 
- `enroll()` uses transactions to ensure both student INSERT and slot UPDATE succeed together
- `remove()` uses transactions to ensure both slot UPDATE and student DELETE succeed together
- If any step fails, ROLLBACK restores the database to its previous state

---

## 11. src/SchoolSystem.cpp

| Function | Description |
|----------|-------------|
| `initializeDatabase()` | Creates tables if not exists, inserts default admin, inserts sample sessions |
| `handleLogin()` | Gets username/password, calls User::findByUsername(), validates password |
| `handleRegister()` | Gets input, validates, creates User object, calls save() |
| `displayAvailableSessions()` | Calls Session::displayAll() |
| `displayAvailableSlots()` | Calls Session::displayAvailableSlots() |
| `addSession()` | Gets input, validates, creates Session object, calls save() |
| `updateSession()` | Shows sessions, gets ID, finds session, updates fields, calls update() |
| `deleteSession()` | Shows sessions, gets ID, confirms, calls remove() |
| `enrollStudent()` | Gets input, validates, shows slots, gets session ID, creates Student, calls enroll() |
| `displayAllStudents()` | Calls Student::displayAll() |
| `displayAllUsers()` | Queries users table directly, prints formatted table |
| `removeStudent()` | Shows students, gets ID, confirms, calls remove() |
| `displayUserMenu()` | Prints user menu options |
| `handleUserMenu()` | Loop that processes user menu choices |
| `displayAdminMenu()` | Prints admin menu options |
| `handleAdminMenu()` | Loop that processes admin menu choices |
| `run()` | Main loop that shows login/register menu |

**Key Logic**: The `run()` function is the main event loop that keeps the application running until the user chooses to exit.

---

## Error Handling Summary

| Error Type | How It's Handled |
|------------|------------------|
| Duplicate student number | `studentNumberExists()` check before enrollment |
| Duplicate session name | `sessionNameExists()` check before adding |
| Full session | `getAvailableSlots() <= 0` check before enrollment |
| Empty fields | `isEmpty()` validation on all inputs |
| Invalid session ID | `findById()` returns nullptr, checked before use |
| Database errors | Try-catch blocks with error messages |
| SQL injection | All queries use prepared statements with parameter binding |

## Data Flow Examples

### Enrollment Flow
```
User Menu → enrollStudent()
  → Get student details
  → Student::enroll()
    → Check unique student number
    → Check session availability
    → BEGIN TRANSACTION
    → INSERT INTO students
    → UPDATE sessions SET available_slots = available_slots - 1
    → COMMIT
  → Display success message
```

### Remove Student Flow
```
Admin Menu → removeStudent()
  → Get student ID
  → Student::remove()
    → Get student session ID
    → BEGIN TRANSACTION
    → UPDATE sessions SET available_slots = available_slots + 1
    → DELETE FROM students WHERE id = ?
    → COMMIT
  → Display success message
```
