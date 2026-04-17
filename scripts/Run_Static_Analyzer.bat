@echo off
REM ================================================================================
REM Static Code Analysis Tool - Complete Workflow
REM 
REM Purpose: Analyze C/C++ source code and generate knowledge base
REM Version: 3.0.0
REM 
REM Developer: Himanshu R
REM Organization: Qorix India Pvt Ltd
REM ================================================================================

setlocal enabledelayedexpansion

echo.
echo ================================================================================
echo                   Static Code Analysis Tool v3.0.0
echo                       Qorix India Pvt Ltd
echo ================================================================================
echo.

REM Set project paths
set SCRIPT_DIR=%~dp0
REM Remove "scripts\" (8 characters) from the end to get project root
set PROJECT_ROOT=%SCRIPT_DIR:~0,-8%
set SRC_DIR=%PROJECT_ROOT%src\
set DATA_DIR=%PROJECT_ROOT%data\
set REPORTS_DIR=%PROJECT_ROOT%reports\
set KB_DIR=%PROJECT_ROOT%knowledge_base\

echo [DEBUG] Script directory: %SCRIPT_DIR%
echo [DEBUG] Project root: %PROJECT_ROOT%
echo [DEBUG] Source directory: %SRC_DIR%
echo.

REM Check if Python is available
python --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Python is not installed or not in PATH
    echo Please install Python 3.8+ and try again
    pause
    exit /b 1
)

echo [INFO] Python version:
python --version
echo.

REM ============================================================================
REM STEP 1: Select Polyspace Report
REM ============================================================================

echo.
echo ============================================================================
echo STEP 1: Select Source Code Directory
echo ============================================================================
echo.
echo Enter the directory path containing C/C++ source code to analyze:
echo Example: D:\Projects\MyModule\src
echo          D:\Code\Mka
echo.
set /p SOURCE_DIR="Source code directory: "

REM Remove quotes if present
set SOURCE_DIR=%SOURCE_DIR:"=%

if not exist "%SOURCE_DIR%" (
    echo [ERROR] Directory not found: %SOURCE_DIR%
    pause
    exit /b 1
)

echo [OK] Source directory: %SOURCE_DIR%

REM ============================================================================
REM STEP 2: Module Name
REM ============================================================================

echo.
echo ============================================================================
echo STEP 2: Module Identification
echo ============================================================================
echo.
echo Enter module name (e.g., Mka, Eep, Can, Lin):
set /p MODULE_NAME="Module Name: "

if "%MODULE_NAME%"=="" (
    echo [ERROR] Module name is required
    pause
    exit /b 1
)

echo [OK] Module: %MODULE_NAME%

REM ============================================================================
REM STEP 3: Run Static Code Analysis
REM ============================================================================

echo.
echo ============================================================================
echo STEP 3: Running Static Code Analysis
echo ============================================================================
echo.

echo [DEBUG] Changing to source directory: %SRC_DIR%
cd /d "%SRC_DIR%"

if errorlevel 1 (
    echo [ERROR] Failed to change to source directory
    echo [ERROR] Expected path: %SRC_DIR%
    pause
    exit /b 1
)

echo [DEBUG] Current directory: %CD%
echo [DEBUG] Running: python run_static_analyzer.py "%SOURCE_DIR%" "%MODULE_NAME%" --no-fixes
echo.

python run_static_analyzer.py "%SOURCE_DIR%" "%MODULE_NAME%" --no-fixes

if errorlevel 1 (
    echo.
    echo [ERROR] Static code analysis failed
    pause
    exit /b 1
)

echo.
echo [SUCCESS] Static code analysis completed!
echo.
echo Output files generated for module '%MODULE_NAME%':
echo   - HTML Report: %REPORTS_DIR%\%MODULE_NAME%_static_analysis_report.html
echo   - Excel Report: %REPORTS_DIR%\%MODULE_NAME%_violations_report.xlsx
echo   - Knowledge Base: %KB_DIR%\%MODULE_NAME%_KnowledgeDatabase.json
echo   - JSON Data: %DATA_DIR%\%MODULE_NAME%_violations.json
echo   - Module History: %REPORTS_DIR%\%MODULE_NAME%_history_report.html
echo   - Cross-Module Comparison: %REPORTS_DIR%\cross_module_comparison.html
echo.
echo Color-coded reports generated:
echo   RED    = Critical issues (null pointer, div by zero)
echo   ORANGE = High priority (uninitialized, buffer overflow)
echo   GREY   = Code quality (MISRA, CERT, style issues)
echo.
echo [TIP] Opening HTML report for module '%MODULE_NAME%'...
start "" "%REPORTS_DIR%\%MODULE_NAME%_static_analysis_report.html"
timeout /t 2 >nul
echo.

REM ============================================================================
REM STEP 4: Post-Analysis Options
REM ============================================================================

echo.
echo ============================================================================
echo STEP 4: What would you like to do next?
echo ============================================================================
echo.
echo Analysis complete! Choose next action:
echo.
echo   1. AI Mode - Generate AI-powered fix suggestions
echo   2. Hybrid Mode - AI + Rule-based suggestions (Recommended)
echo   3. Rules Only - Pattern-based fix suggestions
echo   4. Exit - Just view reports, no fix generation
echo.
set /p ACTION_CHOICE="Enter choice [1/2/3/4]: "

if "%ACTION_CHOICE%"=="1" (
    echo.
    echo [INFO] Generating AI-powered fix suggestions...
    cd ..
    cd src
    python ..\scripts\generate_code_fixes.py %MODULE_NAME% --ai-mode ai_only --source-code "%SOURCE_DIR%"
    cd ..
    cd scripts
) else if "%ACTION_CHOICE%"=="2" (
    echo.
    echo [INFO] Generating hybrid fix suggestions...
    cd ..
    cd src
    python ..\scripts\generate_code_fixes.py %MODULE_NAME% --ai-mode hybrid --source-code "%SOURCE_DIR%"
    cd ..
    cd scripts
) else if "%ACTION_CHOICE%"=="3" (
    echo.
    echo [INFO] Generating rule-based fix suggestions...
    cd ..
    cd src
    python ..\scripts\generate_code_fixes.py %MODULE_NAME% --ai-mode rules_only --source-code "%SOURCE_DIR%"
    cd ..
    cd scripts
) else (
    echo.
    echo [INFO] Skipping fix generation
)

REM ============================================================================
REM STEP 5: Summary and Next Steps
REM ============================================================================

echo.
echo ============================================================================
echo                           Analysis Complete!
echo ============================================================================
echo.
echo Module: %MODULE_NAME%
echo Source Code: %SOURCE_DIR%
echo.
echo How to view reports:
echo   1. HTML Report (Recommended): start reports\%MODULE_NAME%_static_analysis_report.html
echo   2. Excel Report: start reports\%MODULE_NAME%_violations_report.xlsx
echo.
echo Focus Areas:
echo   - RED violations (Critical) = Proven errors - fix first
echo   - ORANGE violations (High) = Likely errors - fix next
echo   - GREY violations (Quality) = Code quality improvements
echo.
echo ============================================================================
echo.

pause
