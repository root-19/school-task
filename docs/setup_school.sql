-- School System Management Database Setup Script for SQLite
-- This file is for reference - the database is created automatically by the application

-- Create users table (RBAC)
CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL,
    email TEXT UNIQUE NOT NULL,
    role TEXT DEFAULT 'USER' CHECK(role IN ('USER', 'ADMIN')),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Create sessions table
CREATE TABLE IF NOT EXISTS sessions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    session_name TEXT UNIQUE NOT NULL,
    section TEXT NOT NULL,
    total_slots INTEGER NOT NULL,
    available_slots INTEGER NOT NULL,
    learning_modality TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Create students table
CREATE TABLE IF NOT EXISTS students (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    student_number TEXT UNIQUE NOT NULL,
    student_name TEXT NOT NULL,
    section TEXT NOT NULL,
    session_id INTEGER NOT NULL,
    learning_modality TEXT NOT NULL,
    enrolled_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (session_id) REFERENCES sessions(id) ON DELETE CASCADE
);

-- Insert default admin user (username: admin, password: admin123)
INSERT OR IGNORE INTO users (username, password, email, role) 
VALUES ('admin', 'admin123', 'admin@school.edu', 'ADMIN');

-- Insert sample sessions
INSERT OR IGNORE INTO sessions (session_name, section, total_slots, available_slots, learning_modality) VALUES
('Morning Session 2024', 'A', 30, 30, 'Face-to-Face'),
('Afternoon Session 2024', 'B', 25, 25, 'Online'),
('Evening Session 2024', 'C', 20, 20, 'Hybrid'),
('Weekend Session 2024', 'D', 15, 15, 'Face-to-Face');




-- Create table
CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL,
    email TEXT UNIQUE NOT NULL,
    role TEXT DEFAULT 'USER'
);

-- Insert
INSERT INTO users (username, password, email, role)
VALUES ('admin', 'admin123', 'admin@school.edu', 'ADMIN');

-- Select
SELECT * FROM users WHERE username = 'admin';

-- Update
UPDATE users SET password = 'newpass' WHERE id = 1;

-- Delete
DELETE FROM users WHERE id = 1;
