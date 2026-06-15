@echo off
cd /d C:\Users\admin\Desktop\ticket-booking
C:\msys64\ucrt64\bin\g++.exe -std=c++11 -Iinclude -IC:\msys64\ucrt64\include src/main.cpp src/SchoolSystem.cpp src/DatabaseConnection.cpp src/User.cpp src/Student.cpp src/Session.cpp -LC:\msys64\ucrt64\lib -lsqlite3 -o build/school_management.exe 2>C:\Users\admin\AppData\Local\Temp\gpp_err.txt
echo Build exit: %errorlevel%
type C:\Users\admin\AppData\Local\Temp\gpp_err.txt
