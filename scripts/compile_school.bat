@echo off
echo ========================================
echo Compiling School Management System
echo ========================================
echo.

echo Compiling with g++...
C:\msys64\ucrt64\bin\g++.exe -std=c++11 -Iinclude -IC:\msys64\ucrt64\include src/main.cpp src/SchoolSystem.cpp src/DatabaseConnection.cpp src/User.cpp src/Student.cpp src/Session.cpp -LC:\msys64\ucrt64\lib -lsqlite3 -o build/school_management.exe

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
