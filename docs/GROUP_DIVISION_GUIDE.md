# Group Division Guide - 16 Members (8 Pairs)


## Part 1: Database Layer (Pair 1)

**Files:**
- `include/DatabaseConnection.h`
- `src/DatabaseConnection.cpp`
- `SchoolSystem::initializeDatabase()` sa `src/SchoolSystem.cpp`

**Responsibilities:**
- Gumawa ng singleton database connection
- Implement `connect()`, `disconnect()`, `executeSQL()`, `prepareStatement()`
- Gumawa ng database initialization code (CREATE TABLE statements)
- I-insert ang default admin account at sample sessions

**Key Methods:**
```cpp
DatabaseConnection::getInstance()
DatabaseConnection::connect(path)
DatabaseConnection::executeSQL(sql)
DatabaseConnection::prepareStatement(sql)
SchoolSystem::initializeDatabase()
```

**Deliverable:** Working database connection at initialized tables

---

## Part 2: User Authentication Model (Pair 2)

**Files:**
- `include/User.h`
- `src/User.cpp`

**Responsibilities:**
- Implement User class with properties (id, username, password, email, role)
- Implement UserRole enum (USER, ADMIN)
- Implement `save()`, `update()`, `authenticate()`
- Implement `findByUsername()`, `findById()`
- Implement `usernameExists()`, `emailExists()`

**Key Methods:**
```cpp
User::save()
User::update()
User::authenticate(password)
User::findByUsername(name)
User::findById(id)
User::usernameExists(name)
User::emailExists(email)
User::isAdmin()
```

**Deliverable:** Working user authentication model

---

## Part 3: Login & Registration Interface (Pair 3)

**Files:**
- `src/SchoolSystem.cpp` (login/register parts)
- `include/SchoolSystem.h` (login/register declarations)

**Responsibilities:**
- Implement `handleLogin()` - magtanong ng username/password, i-validate
- Implement `handleRegister()` - magtanong ng details, i-validate, i-save
- Implement `logout()` - mag-logout ng current user
- Implement input validation (`isEmpty()`, `clearInputBuffer()`)

**Key Methods:**
```cpp
SchoolSystem::handleLogin()
SchoolSystem::handleRegister()
SchoolSystem::logout()
SchoolSystem::clearInputBuffer()
SchoolSystem::isEmpty(str)
```

**Deliverable:** Working login/register menu

---

## Part 4: Session Data Model (Pair 4)

**Files:**
- `include/Session.h`
- `src/Session.cpp` (CRUD operations)

**Responsibilities:**
- Implement Session class with properties
- Implement `save()`, `update()`, `remove()`
- Implement `findById()`, `findBySessionName()`
- Implement `getAll()`, `sessionNameExists()`

**Key Methods:**
```cpp
Session::save()
Session::update()
Session::remove()
Session::findById(id)
Session::findBySessionName(name)
Session::getAll()
Session::sessionNameExists(name)
```

**Deliverable:** Working session data model

---

## Part 5: Session Display & Queries (Pair 5)

**Files:**
- `src/Session.cpp` (display methods)
- `src/SchoolSystem.cpp` (display functions)

**Responsibilities:**
- Implement `display()` - iprint ang isang session
- Implement `displayAll()` - iprint lahat ng sessions
- Implement `displayAvailableSlots()` - iprint sessions with Open/Full status
- Implement `SchoolSystem::displayAvailableSessions()`
- Implement `SchoolSystem::displayAvailableSlots()`

**Key Methods:**
```cpp
Session::display()
Session::displayAll()
Session::displayAvailableSlots()
SchoolSystem::displayAvailableSessions()
SchoolSystem::displayAvailableSlots()
```

**Deliverable:** Working session display features

---

## Part 6: Student Data Model (Pair 6)

**Files:**
- `include/Student.h`
- `src/Student.cpp` (basic CRUD)

**Responsibilities:**
- Implement Student class with properties
- Implement `findByStudentNumber()`, `findById()`
- Implement `getAll()`, `getBySessionId()`
- Implement `studentNumberExists()`
- Implement `display()` at `displayAll()`

**Key Methods:**
```cpp
Student::findByStudentNumber(num)
Student::findById(id)
Student::getAll()
Student::getBySessionId(id)
Student::studentNumberExists(num)
Student::display()
Student::displayAll()
```

**Deliverable:** Working student data model

---

## Part 7: Enrollment & Slot Management (Pair 7)

**Files:**
- `src/Student.cpp` (enroll/remove with transactions)
- `src/SchoolSystem.cpp` (enroll/remove student)

**Responsibilities:**
- Implement `Student::enroll()` with transaction handling
  - Check unique student number
  - Check session availability
  - INSERT student at UPDATE session slots
- Implement `Student::remove()` with transaction handling
  - UPDATE session slots (restore slot)
  - DELETE student
- Implement `SchoolSystem::enrollStudent()`
- Implement `SchoolSystem::removeStudent()`

**Key Methods:**
```cpp
Student::enroll()
Student::remove()
SchoolSystem::enrollStudent()
SchoolSystem::removeStudent()
```

**Deliverable:** Working enrollment with automatic slot management

---

## Part 8: Admin & User Menus + Main (Pair 8)

**Files:**
- `src/SchoolSystem.cpp` (menu handlers)
- `src/main.cpp`
- `include/SchoolSystem.h`

**Responsibilities:**
- Implement `displayAdminMenu()` at `handleAdminMenu()`
- Implement `displayUserMenu()` at `handleUserMenu()`
- Implement `addSession()`, `updateSession()`, `deleteSession()`
- Implement `displayAllStudents()`, `displayAllUsers()`
- Implement `main.cpp` - entry point
- Implement `SchoolSystem::run()` - main loop

**Key Methods:**
```cpp
SchoolSystem::displayAdminMenu()
SchoolSystem::handleAdminMenu()
SchoolSystem::displayUserMenu()
SchoolSystem::handleUserMenu()
SchoolSystem::addSession()
SchoolSystem::updateSession()
SchoolSystem::deleteSession()
SchoolSystem::displayAllStudents()
SchoolSystem::displayAllUsers()
SchoolSystem::run()
main()
```

**Deliverable:** Complete working application

---

## Flow ng Development (Step-by-Step)

```
Step 1: Part 1 (Database Layer)
   |
Step 2: Part 2 (User Model) + Part 4 (Session Model) + Part 6 (Student Model)
   | (pwede parallel development)
   |
Step 3: Part 3 (Login Interface)
   |
Step 4: Part 5 (Session Display)
   |
Step 5: Part 7 (Enrollment Logic)
   |
Step 6: Part 8 (Menus + Main) → Integration ng lahat
```

## Paano Mag-integrate

Bawat part ay may interface (header file) na ipapasa sa next part:

1. **Part 1** → Part 2, 3, 4, 6 (DatabaseConnection)
2. **Part 2** → Part 3 (User class)
3. **Part 4** → Part 5, 7 (Session class)
4. **Part 6** → Part 7 (Student class)
5. **Part 7** → Part 8 (Enrollment features)
6. **Part 8** → Final integration at testing

## Git Workflow Recommendation

```
main branch (stable)
  |
  ├── pair1-database
  ├── pair2-user-model
  ├── pair3-login-interface
  ├── pair4-session-model
  ├── pair5-session-display
  ├── pair6-student-model
  ├── pair7-enrollment
  └── pair8-menus
```

Bawat pair ay gumawa ng sarili nilang branch. Kapag tapos na, merge sa main branch.

## Testing Checklist per Part

| Part | Test |
|------|------|
| Part 1 | Makaconnect sa database, ma-create tables |
| Part 2 | Maka-save ng user, maka-find, maka-authenticate |
| Part 3 | Makapag-login at register |
| Part 4 | Maka-save, update, delete ng session |
| Part 5 | Makita ang lahat ng sessions sa console |
| Part 6 | Maka-find ng student, makita ang list |
| Part 7 | Makapag-enroll, bumaba ang slots; makapag-remove, tumaas ang slots |
| Part 8 | Gumana ang buong application end-to-end |
