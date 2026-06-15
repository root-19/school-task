#!/bin/bash
cd /c/Users/admin/Desktop/ticket-booking
/ucrt64/bin/g++ -std=c++11 \
  -Iinclude \
  -I/ucrt64/include \
  src/main.cpp \
  src/SchoolSystem.cpp \
  src/DatabaseConnection.cpp \
  src/User.cpp \
  src/Student.cpp \
  src/Session.cpp \
  -L/ucrt64/lib \
  -lsqlite3 \
  -o build/school_management.exe \
  2>/tmp/cperr.txt
echo "Exit code: $?"
cat /tmp/cperr.txt
cp /tmp/cperr.txt /c/Users/admin/Desktop/ticket-booking/build/err2.txt
