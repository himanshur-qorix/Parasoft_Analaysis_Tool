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

REM Prompt for source code path (optional)
echo.
set /p SOURCE_CODE_PATH="Enter source code path for MISRA/CERT analysis (optional, press Enter to skip): "

if not "%SOURCE_CODE_PATH%"=="" (
    if not exist "%SOURCE_CODE_PATH%" (
        echo Warning: Source code path does not exist: %SOURCE_CODE_PATH%
        echo Skipping MISRA/CERT pre-analysis...
        set SOURCE_CODE_PATH=
    ) else (
        echo [INFO] Source code path: %SOURCE_CODE_PATH%
        echo [INFO] MISRA/CERT pre-analysis will be performed
    )
)

REM Prompt for AI mode
echo.
echo ==================================================
echo AI Mode Selection
echo ==================================================
echo.
echo Choose fix generation mode:
echo   [1] AI Only      - Use Ollama AI for all violations (requires Ollama)
echo   [2] Hybrid       - Smart: AI for complex, rules for standard (recommended)
echo   [3] Rules Only   - Use only rule-based fixes (no AI required)
echo.
set /p AI_MODE_CHOICE="Enter choice (1-3) [default: 2]: "

if "%AI_MODE_CHOICE%"=="" set AI_MODE_CHOICE=2

if "%AI_MODE_CHOICE%"=="1" (
    set AI_MODE=ai_only
    echo [INFO] Selected: AI Only mode
) else if "%AI_MODE_CHOICE%"=="2" (
    set AI_MODE=hybrid
    echo [INFO] Selected: Hybrid mode (AI + Rules)
) else if "%AI_MODE_CHOICE%"=="3" (
    set AI_MODE=rules_only
    echo [INFO] Selected: Rules Only mode
) else (
    echo Invalid choice. Using default: Hybrid mode
    set AI_MODE=hybrid
)

REM Run the AI Agent
echo.
echo Running analysis with Qorix integration...
echo.

if not "%SOURCE_CODE_PATH%"=="" (
    python scripts\run_agent.py report_dev1.html %MODULE_NAME% --source-code "%SOURCE_CODE_PATH%" --ai-mode %AI_MODE%
) else (
    python scripts\run_agent.py report_dev1.html %MODULE_NAME% --ai-mode %AI_MODE%
)

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