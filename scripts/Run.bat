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

REM Check if Qorix deviations file exists
if not exist "data\Qorix_CP_Common_Deviations.xlsx" (
    echo Warning: Qorix_CP_Common_Deviations.xlsx not found in data\ folder
    echo The analysis will continue but violations will not be checked against Qorix deviations
    echo Status column will show "Analysis Required" for all violations
    echo.
    echo Press any key to continue or Ctrl+C to cancel...
    pause >nul
)

REM Prompt for module name
set /p MODULE_NAME="Enter module name (e.g., Mka): "

if "%MODULE_NAME%"=="" (
    echo Error: Module name is required
    pause
    exit /b 1
)

REM Run the AI Agent
echo.
echo Running analysis with Qorix integration...
echo.
python scripts\run_agent.py report_dev1.html %MODULE_NAME%

if errorlevel 1 (
    echo.
    echo ==================================================
    echo Error: Analysis failed!
    echo ==================================================
    echo Please check the error messages above
    pause
    exit /b 1
)

echo.
echo ==================================================
echo Analysis completed successfully!
echo ==================================================
echo.
echo Check the following directories for results:
echo   - reports\%MODULE_NAME%_violations_report.xlsx (Excel with Status column)
echo   - reports\%MODULE_NAME%_analysis_summary.json
echo   - knowledge_base\%MODULE_NAME%_KnowledgeDatabase.json
echo   - justifications\%MODULE_NAME%_suppress_comments_*.txt (Parasoft suppress comments)
echo   - fixes\%MODULE_NAME%\
echo.
echo Excel Report includes:
echo   - Status: Justified / Needs Code Update / Analysis Required
echo   - Filtered by Qorix_CP_Common_Deviations.xlsx
echo.
echo.
echo ==================================================
echo Next Step: Apply Suppress Comments to Code
echo ==================================================
echo.
echo To automatically insert suppress comments into your source code:
echo   1. Run: scripts\Apply_Suppressions.bat
echo   2. Select the suppress comments file
echo   3. Provide path to your source code repository
echo   4. Review and approve each change interactively
echo.

pause
exit /b 0