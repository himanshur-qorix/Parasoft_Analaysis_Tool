@echo off
REM ===================================================================
REM CERT & MISRA Violation Report Generator
REM Generates HTML comparison reports from Knowledge Base
REM Version: 3.0.0
REM ===================================================================

setlocal enabledelayedexpansion

REM Set console colors
color 0B

echo.
echo =========================================================================
echo              CERT ^& MISRA Violation Report Generator
echo =========================================================================
echo                    Qorix India Pvt Ltd
echo                   Parasoft Analysis Tool v3.0.0
echo =========================================================================
echo.

REM Navigate to project root
cd /d "%~dp0.."

REM Check if Python is installed
python --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Python is not installed or not in PATH
    echo Please install Python 3.8 or higher
    pause
    exit /b 1
)

REM Step 1: Get Module Name
echo Step 1: Enter Module Name
echo -------------------------------------------------------------------------
echo.

set /p MODULE_NAME="Enter module name (e.g., Tls, Mka, Ssc): "

if "!MODULE_NAME!"=="" (
    echo [ERROR] Module name cannot be empty
    pause
    exit /b 1
)

echo.
echo =========================================================================
echo Selected Module: !MODULE_NAME!
echo =========================================================================
echo.

REM Step 2: Verify Knowledge Base Exists
set KB_FILE=knowledge_base\!MODULE_NAME!_KnowledgeDatabase.json

if not exist "!KB_FILE!" (
    echo [ERROR] Knowledge base not found for module '!MODULE_NAME!'
    echo.
    echo Expected file: !KB_FILE!
    echo.
    echo Available knowledge bases:
    dir /b knowledge_base\*_KnowledgeDatabase.json 2>nul
    echo.
    echo Please run the static analyzer first to create the knowledge base:
    echo    scripts\Run_Static_Analyzer.bat
    echo.
    pause
    exit /b 1
)

echo ✓ Knowledge base found: !KB_FILE!
echo.

REM Step 3: Generate Report
echo Step 2: Generating CERT ^& MISRA Report
echo -------------------------------------------------------------------------
echo.

python src\generate_cert_misra_report.py "!MODULE_NAME!"

if errorlevel 1 (
    echo.
    echo [ERROR] Report generation failed
    pause
    exit /b 1
)

echo.
echo =========================================================================
echo                    Report Generation Complete!
echo =========================================================================
echo.
echo Output file: reports\!MODULE_NAME!_CERT_MISRA_Report.html
echo.
echo The report includes:
echo   - Total CERT and MISRA violations
echo   - Severity breakdown
echo   - Top 10 violations by occurrence
echo   - Status summary (Open/Fixed/Justified)
echo.
echo =========================================================================
echo.

REM Step 4: Ask if user wants to generate another report
echo.
set /p ANOTHER="Generate report for another module? (Y/N): "

if /i "!ANOTHER!"=="Y" (
    cls
    goto :START
)

echo.
echo Done! Press any key to exit...
pause >nul
exit /b 0

:START
cls
goto :EOF
