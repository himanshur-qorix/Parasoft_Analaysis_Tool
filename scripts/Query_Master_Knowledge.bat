@echo off
title Query Master Knowledge Base
color 0A

REM Change to project root directory
cd /d "%~dp0.."

echo ==================================================
echo    Master Knowledge Base Query Tool
echo    Parasoft AI Agent - Version 2.1.1
echo ==================================================
echo.

REM Check if Python is installed
python --version >nul 2>&1
if errorlevel 1 (
    echo Error: Python is not installed or not in PATH
    pause
    exit /b 1
)

REM Activate virtual environment if it exists
if exist "venv\" (
    call venv\Scripts\activate.bat
)

REM Run the query tool
python src\query_master_knowledge.py

if errorlevel 1 (
    echo.
    echo ==================================================
    echo Error: Query tool failed!
    echo ==================================================
    pause
    exit /b 1
)

echo.
pause
