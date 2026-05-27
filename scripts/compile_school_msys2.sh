#!/bin/bash
echo "========================================"
echo "Compiling School Management with SQLite"
echo "========================================"
echo ""

echo "Compiling with SQLite (built-in with MSYS2)..."
g++ -std=c++11 -I../include \
    src/main_school.cpp src/DatabaseConnection.cpp src/User.cpp src/Student.cpp src/Session.cpp src/Auth.cpp \
    -lsqlite3 -o build/school_management.exe

if [ $? -eq 0 ]; then
    echo ""
    echo "========================================"
    echo "Compilation Successful!"
    echo "========================================"
    echo ""
    echo "Run: cd build && ./school_management.exe"
else
    echo ""
    echo "========================================"
    echo "Compilation Failed!"
    echo "========================================"
    echo ""
    echo "Make sure SQLite3 is installed: pacman -S mingw-w64-ucrt-x86_64-sqlite3"
fi
