@echo off
cd /d C:\Users\admin\Desktop\ticket-booking
C:\msys64\ucrt64\bin\g++.exe -std=c++11 -Iinclude -IC:\msys64\ucrt64\include -c src/SchoolSystem.cpp -o build/SchoolSystem.o 1>>build\compile_log.txt 2>>build\compile_log.txt
echo SchoolSystem: %errorlevel% >> build\compile_log.txt
C:\msys64\ucrt64\bin\g++.exe -std=c++11 -Iinclude -IC:\msys64\ucrt64\include -c src/Student.cpp -o build/Student.o 1>>build\compile_log.txt 2>>build\compile_log.txt
echo Student: %errorlevel% >> build\compile_log.txt
C:\msys64\ucrt64\bin\g++.exe -std=c++11 -Iinclude -IC:\msys64\ucrt64\include -c src/Session.cpp -o build/Session.o 1>>build\compile_log.txt 2>>build\compile_log.txt
echo Session: %errorlevel% >> build\compile_log.txt
C:\msys64\ucrt64\bin\g++.exe -std=c++11 -Iinclude -IC:\msys64\ucrt64\include -c src/User.cpp -o build/User.o 1>>build\compile_log.txt 2>>build\compile_log.txt
echo User: %errorlevel% >> build\compile_log.txt
C:\msys64\ucrt64\bin\g++.exe -std=c++11 -Iinclude -IC:\msys64\ucrt64\include -c src/DatabaseConnection.cpp -o build/DatabaseConnection.o 1>>build\compile_log.txt 2>>build\compile_log.txt
echo DatabaseConnection: %errorlevel% >> build\compile_log.txt
C:\msys64\ucrt64\bin\g++.exe -std=c++11 -Iinclude -IC:\msys64\ucrt64\include -c src/main.cpp -o build/main.o 1>>build\compile_log.txt 2>>build\compile_log.txt
echo main: %errorlevel% >> build\compile_log.txt
