@echo off
REM ============================================================================
REM Launch_GUI.bat
REM Launches the Parasoft Analysis Tool GUI
REM Organization: Qorix India Pvt Ltd
REM Version: 4.0.0
REM ============================================================================

title Parasoft Analysis Tool v4.0.0 - GUI Launcher
color 0B

echo.
echo ========================================================================
echo   Parasoft Analysis Tool v4.0.0 - GUI Launcher
echo   Qorix India Pvt Ltd
echo ========================================================================
echo.

REM Check if Python is installed
python --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Python is not installed or not in PATH
    echo Please install Python 3.8+ and add it to PATH
    echo.
    pause
    exit /b 1
)

echo [INFO] Python found. Launching GUI...
echo.

REM Activate virtual environment if exists
if exist "..\venv\Scripts\activate.bat" (
    call ..\venv\Scripts\activate.bat
    echo [INFO] Virtual environment activated
)

REM Launch the GUI from src folder
python ..\src\parasoft_gui.py

REM Check if GUI exited with error
if errorlevel 1 (
    echo.
    echo [ERROR] GUI exited with an error
    pause
)

exit /b 0
