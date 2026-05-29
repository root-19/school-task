@echo off
echo ========================================
echo Compiling School Management System
echo ========================================
echo.

echo Compiling with g++...
g++ -std=c++11 -Iinclude src/main.cpp src/SchoolSystem.cpp src/DatabaseConnection.cpp src/User.cpp src/Student.cpp src/Session.cpp -lsqlite3 -o build/school_management.exe

if %errorLevel% equ 0 (
    echo.
    echo ========================================
    echo Compilation Successful!
    echo ========================================
    echo.
    echo Run the application: cd build && school_management.exe
) else (
    echo.
    echo ========================================
    echo Compilation Failed!
    echo ========================================
    echo.
    echo Make sure SQLite3 is installed and linked correctly.
)

pause
