@echo off
REM ============================================================================
REM Generate_Code_Fixes.bat
REM Generates code fix suggestions for Parasoft violations
REM Uses Parasoft Rules Database (1200+ official rules) for authoritative fixes
REM Organization: Qorix India Pvt Ltd
REM Version: 2.3.0
REM ============================================================================

title Code Fix Generator
color 0B

REM Change to project root directory
cd /d "%~dp0.."

echo.
echo ========================================================================
echo   CODE FIX GENERATOR - Version 2.3.0
echo   Qorix India Pvt Ltd
echo   Powered by Parasoft Rules Database (1200+ official rules)
echo ========================================================================
echo.

REM Check if Python is installed
python --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Python is not installed or not in PATH
    pause
    exit /b 1
)

REM Activate virtual environment if exists
if exist "venv\Scripts\activate.bat" (
    call venv\Scripts\activate.bat
)

REM Prompt for module name
set /p MODULE_NAME="Enter module name (e.g., Mka): "

if "%MODULE_NAME%"=="" (
    echo [ERROR] Module name is required
    pause
    exit /b 1
)

REM Check if knowledge base exists
if not exist "knowledge_base\%MODULE_NAME%_KnowledgeDatabase.json" (
    echo.
    echo [ERROR] Knowledge base not found for module: %MODULE_NAME%
    echo.
    echo Please run Run.bat first to create the knowledge base.
    echo The knowledge base is required to generate code fixes.
    echo.
    pause
    exit /b 1
)

echo.
echo ==================================================
echo AI Mode Selection
echo ==================================================
echo.
echo Choose fix generation mode:
echo   [1] AI Only      - AI-generated suggestions only (requires Ollama + model)
echo   [2] Hybrid       - Parasoft DB first, then AI for unmatched (RECOMMENDED)
echo   [3] Rules Only   - Parasoft DB + pattern-based fixes (no AI/Ollama needed)
echo.
echo Mode Details:
echo   - AI Only:   Generate unique AI suggestions, skip Parasoft database
echo   - Hybrid:    Official Parasoft examples + AI fallback + patterns
echo   - Rules:     Official Parasoft examples + hardcoded patterns only
echo.
set /p AI_MODE_CHOICE="Enter choice (1-3) [default: 2]: "

if "%AI_MODE_CHOICE%"=="" set AI_MODE_CHOICE=2

if "%AI_MODE_CHOICE%"=="1" (
    set AI_MODE=ai_only
    echo [INFO] Selected: AI Only mode - AI-generated fixes
) else if "%AI_MODE_CHOICE%"=="2" (
    set AI_MODE=hybrid
    echo [INFO] Selected: Hybrid mode - Parasoft DB ^+ AI ^+ Patterns ^(RECOMMENDED^)
) else if "%AI_MODE_CHOICE%"=="3" (
    set AI_MODE=rules_only
    echo [INFO] Selected: Rules Only mode - Parasoft DB ^+ Patterns ^(No AI^)
) else (
    echo Invalid choice. Using default: Hybrid mode
    set AI_MODE=hybrid
)

echo.
echo ==================================================
echo Generating Code Fixes
echo ==================================================
echo.
echo Module: %MODULE_NAME%
echo AI Mode: %AI_MODE%
echo.

REM Run the code fix generator
python src\generate_code_fixes.py %MODULE_NAME% --ai-mode %AI_MODE%

if errorlevel 1 (
    echo.
    echo ==================================================
    echo [ERROR] Code fix generation failed!
    echo ==================================================
    echo Please check the error messages above
    pause
    exit /b 1
)

echo.
echo ==================================================
echo [SUCCESS] Code Fixes Generated!
echo ==================================================
echo.
echo Your code fix suggestions have been saved to:
echo   Text: %MODULE_NAME%_code_suggestion\%MODULE_NAME%_fixes_*.txt
echo   HTML: %MODULE_NAME%_code_suggestion\%MODULE_NAME%_fixes_*.html
echo.

if "%AI_MODE%"=="ai_only" (
    echo Fix Generation Strategy: AI ONLY
    echo   ✅ AI-generated suggestions ^(unique, context-aware^)
    echo   ✅ Pattern-based fallback ^(if AI fails^)
    echo   ℹ️  Parasoft DB was skipped ^(AI-only mode^)
) else if "%AI_MODE%"=="rules_only" (
    echo Fix Generation Strategy: RULES ONLY
    echo   ✅ Official Parasoft repair examples ^(1200+ rules^)
    echo   ✅ Pattern-based rules ^(hardcoded fixes^)
    echo   ℹ️  AI was disabled ^(rules-only mode^)
) else (
    echo Fix Generation Strategy: HYBRID ^(Balanced^)
    echo   ✅ Official Parasoft examples ^(priority 1^)
    echo   ✅ AI-generated suggestions ^(priority 2^)
    echo   ✅ Pattern-based rules ^(priority 3^)
)

echo.
echo All fixes include:
echo   • Before/after code snippets
echo   • Security relevance and CWE mappings
echo   • Detailed explanations
echo.
echo ==================================================
echo How Would You Like to View the Fixes?
echo ==================================================
echo.
echo   [1] Interactive Viewer   - Navigate with keyboard (RECOMMENDED)
echo   [2] HTML Report         - Open in browser with filtering
echo   [3] Text File           - Open in Notepad
echo   [4] Skip                - View later
echo.
set /p VIEW_CHOICE="Enter choice (1-4) [default: 1]: "

if "%VIEW_CHOICE%"=="" set VIEW_CHOICE=1

REM Find the latest fixes file
for /f "delims=" %%i in ('dir /b /o-d "%MODULE_NAME%_code_suggestion\%MODULE_NAME%_fixes_*.txt" 2^>nul') do (
    set LATEST_FIXES_FILE=%MODULE_NAME%_code_suggestion\%%i
    goto :found_file
)

:found_file

if "%VIEW_CHOICE%"=="1" (
    echo.
    echo [INFO] Starting Interactive Viewer...
    echo.
    python src\view_fixes_interactive.py "%LATEST_FIXES_FILE%"
) else if "%VIEW_CHOICE%"=="2" (
    echo.
    echo [INFO] Opening HTML report in browser...
    echo.
    REM Find HTML file
    for /f "delims=" %%i in ('dir /b /o-d "%MODULE_NAME%_code_suggestion\%MODULE_NAME%_fixes_*.html" 2^>nul') do (
        set LATEST_HTML_FILE=%MODULE_NAME%_code_suggestion\%%i
        goto :open_html
    )
    :open_html
    start "" "%LATEST_HTML_FILE%"
    echo [OK] HTML report opened in default browser
) else if "%VIEW_CHOICE%"=="3" (
    echo.
    echo [INFO] Opening text file in Notepad...
    notepad "%LATEST_FIXES_FILE%"
) else (
    echo.
    echo [INFO] Skipped viewing. Files are saved and ready when you need them.
)

echo.
echo ==================================================
echo Next Steps
echo ==================================================
echo.
echo To view the fixes again:
echo   - Interactive: python src\view_fixes_interactive.py "%LATEST_FIXES_FILE%"
echo   - HTML: Open %MODULE_NAME%_code_suggestion\%MODULE_NAME%_fixes_*.html in browser
echo   - Text: Open %MODULE_NAME%_code_suggestion\%MODULE_NAME%_fixes_*.txt
echo.
echo To apply fixes to your source code:
echo   1. Review each fix suggestion
echo   2. Apply appropriate fixes manually
echo   3. Re-run analysis to verify
echo.
echo To apply suppress comments for justified violations:
echo   Run: scripts\Apply_Suppressions.bat
echo.

pause
exit /b 0
