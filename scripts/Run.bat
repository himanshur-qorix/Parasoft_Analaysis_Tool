@echo off
title Parasoft AI Agent
color 0A

REM Change to project root directory
cd /d "%~dp0.."

echo ==================================================
echo        Parasoft AI Agent - Version 2.0.0
echo        Developer: Himanshu R
echo ==================================================
echo.

REM Check if Python is installed
python --version >nul 2>&1
if errorlevel 1 (
    echo Error: Python is not installed or not in PATH
    pause
    exit /b 1
)

REM Install dependencies if needed
if not exist "venv\" (
    echo Creating virtual environment...
    python -m venv venv
    call venv\Scripts\activate.bat
    echo Installing dependencies...
    pip install -r requirements.txt
) else (
    call venv\Scripts\activate.bat
)

echo.
echo Starting AI Agent analysis...
echo.

REM Check if report_dev1.html exists
if not exist "report_dev1.html" (
    echo Error: report_dev1.html not found in current directory
    echo Please place the Parasoft report in the tool folder
    pause
    exit /b 1
)

REM Prompt for module name
set /p MODULE_NAME="Enter module name (e.g., Mka): "

if "%MODULE_NAME%"=="" (
    echo Error: Module name is required
    pause
    exit /b 1
)

REM Run the AI Agent
python run_agent.py report_dev1.html %MODULE_NAME%

echo.
echo ==================================================
echo Analysis completed successfully!
echo ==================================================
echo.
echo Check the following directories for results:
echo   - knowledge_base\%MODULE_NAME%_KnowledgeDatabase.json
echo   - reports\%MODULE_NAME%_analysis_summary.json
echo   - fixes\%MODULE_NAME%\
echo.

pause


set /p TARGET_REPO="Enter the full path to your testing repository: "

REM Usage: Run with or without --apply to auto-fix where possible
python ParasoftAnalysisTool.py report_dev1.html --apply --target-repo "%TARGET_REPO%"

echo.
echo ==================================================
echo Analysis completed successfully.
echo Command window will close in 5 seconds...
echo ==================================================

pause >nul
REM timeout /t 5 /nobreak >nul
exit