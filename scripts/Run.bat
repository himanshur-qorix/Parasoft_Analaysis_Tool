@echo off
title Parasoft AI Agent
color 0A

REM Change to project root directory
cd /d "%~dp0.."

echo ==================================================
echo        Parasoft AI Agent - Version 2.1.1
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

REM Ensure required directories exist
if not exist "reports" mkdir reports
if not exist "knowledge_base" mkdir knowledge_base
if not exist "fixes" mkdir fixes
if not exist "justifications" mkdir justifications

echo.
echo Starting AI Agent analysis...
echo.

REM Prompt for module name first
set /p MODULE_NAME="Enter module name (e.g., Mka): "

if "%MODULE_NAME%"=="" (
    echo Error: Module name is required
    pause
    exit /b 1
)

REM Prompt for source code path
echo.
set /p SOURCE_CODE_PATH="Enter source code path for MISRA/CERT analysis (optional, press Enter to skip): "

REM Initialize variables
set REPORT_FILE=report_dev1.html
set GENERATED_REPORT=

REM Validate source code path if provided
if not "%SOURCE_CODE_PATH%"=="" (
    if not exist "%SOURCE_CODE_PATH%" (
        echo Warning: Source code path does not exist: %SOURCE_CODE_PATH%
        echo Skipping MISRA/CERT analysis...
        set SOURCE_CODE_PATH=
    ) else (
        echo [INFO] Source code path: %SOURCE_CODE_PATH%
    )
)

REM Check if report_dev1.html exists
if not exist "%REPORT_FILE%" (
    echo.
    echo ==================================================
    echo report_dev1.html not found
    echo ==================================================
    
    if "%SOURCE_CODE_PATH%"=="" (
        echo.
        echo Error: No Parasoft report found and no source code path provided
        echo.
        echo Please do one of the following:
        echo   1. Place report_dev1.html in the project folder, OR
        echo   2. Provide source code path to generate MISRA/CERT report
        echo.
        pause
        exit /b 1
    )
    
    echo Generating MISRA/CERT report from source code...
    echo.
    
    REM Create reports directory if it doesn't exist
    if not exist "reports" mkdir reports
    
    REM Generate MISRA/CERT report as HTML with knowledge base integration
    python src\misra_cert_checker.py "%SOURCE_CODE_PATH%" --report html --output "reports\%MODULE_NAME%_misra_cert_report.html" --module %MODULE_NAME% --show-known
    
    if errorlevel 1 (
        echo.
        echo ==================================================
        echo Error: Failed to generate MISRA/CERT report
        echo ==================================================
        pause
        exit /b 1
    )
    
    REM Use the generated MISRA/CERT report as input
    set REPORT_FILE=reports\%MODULE_NAME%_misra_cert_report.html
    set GENERATED_REPORT=yes
    
    echo [OK] MISRA/CERT report generated: %REPORT_FILE%
    echo [INFO] This report will be used for analysis
    echo.
) else (
    echo [OK] Using existing Parasoft report: %REPORT_FILE%
    if not "%SOURCE_CODE_PATH%"=="" (
        echo [INFO] MISRA/CERT pre-analysis will also be performed
    )
)

REM Check if Qorix deviations file exists
if not exist "data\Qorix_CP_Common_Deviations.xlsx" (
    echo.
    echo Warning: Qorix_CP_Common_Deviations.xlsx not found in data\ folder
    echo The analysis will continue but violations will not be checked against Qorix deviations
    echo Status column will show "Analysis Required" for all violations
    echo.
    echo Press any key to continue or Ctrl+C to cancel...
    pause >nul
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

REM Run the AI Agent (without code fix generation)
echo.
echo Running analysis with Qorix integration...
echo Note: Code fix generation is now separate - use Generate_Code_Fixes.bat
echo.

if not "%SOURCE_CODE_PATH%"=="" (
    if "%GENERATED_REPORT%"=="yes" (
        REM Report was generated from MISRA/CERT, source code already analyzed
        python src\run_agent.py "%REPORT_FILE%" %MODULE_NAME% --ai-mode %AI_MODE% --no-fixes
    ) else (
        REM Using Parasoft report with MISRA/CERT pre-analysis
        python src\run_agent.py "%REPORT_FILE%" %MODULE_NAME% --source-code "%SOURCE_CODE_PATH%" --ai-mode %AI_MODE% --no-fixes
    )
) else (
    REM No source code path, just analyze the report
    python src\run_agent.py "%REPORT_FILE%" %MODULE_NAME% --ai-mode %AI_MODE% --no-fixes
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
echo   - reports\%MODULE_NAME%_violations_report_UPDATED.xlsx (Excel with Justifications)
echo   - reports\%MODULE_NAME%_analysis_summary.json
echo   - knowledge_base\%MODULE_NAME%_KnowledgeDatabase.json
echo   - justifications\%MODULE_NAME%_suppress_comments_*.txt (Parasoft suppress comments)
echo.
echo Excel Report includes:
echo   - Status: Justified / Needs Code Update / Analysis Required
echo   - Justification: Shows which violations have justifications added
echo   - Filtered by Qorix_CP_Common_Deviations.xlsx
echo.
echo.
echo ==================================================
echo Next Steps
echo ==================================================
echo.
echo Analysis Complete! Next, generate code fix suggestions:
echo   1. Run: scripts\Generate_Code_Fixes.bat
echo   2. Select AI mode (hybrid recommended)
echo   3. View fixes interactively or in HTML
echo.
echo To apply suppress comments:
echo   1. Run: scripts\Apply_Suppressions.bat
echo   2. Select the suppress comments file
echo   3. Provide path to your source code repository
echo   4. Review and approve each change interactively
echo.

pause
exit /b 0