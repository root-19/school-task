@echo off
echo ========================================
echo Compiling School Management System
echo ========================================
echo.

REM Set MySQL Connector/C++ paths - adjust if needed
set MYSQL_INCLUDE=C:\Program Files\MySQL\Connector C++ 8.0\include
set MYSQL_LIB=C:\Program Files\MySQL\Connector C++ 8.0\lib64

REM Check if MySQL Connector is installed
if not exist "%MYSQL_INCLUDE%" (
    echo MySQL Connector/C++ not found at default location.
    echo Please install it or update the paths in this script.
    echo.
    echo Current include path: %MYSQL_INCLUDE%
    echo Current lib path: %MYSQL_LIB%
    pause
    exit /b 1
)

echo Compiling with g++...
g++ -std=c++11 -I"%MYSQL_INCLUDE%" -L"%MYSQL_LIB%" ^
    main_school.cpp DatabaseConnection.cpp User.cpp Student.cpp Session.cpp Auth.cpp ^
    -lmysqlcppconn -o school_management.exe

if %errorLevel% equ 0 (
    echo.
    echo ========================================
    echo Compilation Successful!
    echo ========================================
    echo.
    echo Run the application: school_management.exe
) else (
    echo.
    echo ========================================
    echo Compilation Failed!
    echo ========================================
    echo.
    echo Make sure MySQL Connector/C++ is installed and paths are correct.
)

pause
