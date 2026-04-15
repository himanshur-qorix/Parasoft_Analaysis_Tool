@echo off
setlocal enabledelayedexpansion
title Apply Parasoft Suppress Comments
color 0A

REM Change to project root directory
cd /d "%~dp0.."

echo ==================================================
echo    Apply Parasoft Suppress Comments to Code
echo    Version: 1.0.0
echo    Developer: Himanshu R
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
if exist "venv\Scripts\activate.bat" (
    call venv\Scripts\activate.bat
)

echo This tool will help you apply Parasoft suppress comments to your source code.
echo.

REM Step 2: Get target repository path (ask once at the beginning)
echo Step 1: Specify the target repository
echo.
set /p TARGET_REPO="Enter the full path to your source code repository: "

if "!TARGET_REPO!"=="" (
    echo Error: Target repository path is required
    pause
    exit /b 1
)

if not exist "!TARGET_REPO!" (
    echo Error: Target repository path does not exist: !TARGET_REPO!
    pause
    exit /b 1
)

echo.
echo Target repository set to: !TARGET_REPO!
echo.

REM Track processed files for summary
set TOTAL_FILES_PROCESSED=0

:FILE_SELECTION_LOOP

echo ==================================================
echo Step 2: Select the suppress comments file
echo ==================================================
echo.

REM List available suppress comment files
if not exist "justifications\" (
    echo Error: No justifications folder found
    echo Please run the analysis first to generate suppress comments
    pause
    exit /b 1
)

echo Available suppress comment files:
echo.
set count=0
for %%F in (justifications\*_suppress_comments_*.txt) do (
    set /a count+=1
    echo   [!count!] %%~nxF
    set "file!count!=%%F"
)

if !count!==0 (
    echo No suppress comment files found in justifications\ folder
    echo Please run the analysis first to generate suppress comments
    pause
    exit /b 1
)

echo.
set /p FILE_CHOICE="Enter file number (1-!count!) or 'q' to quit: "

REM Check for quit
if /i "!FILE_CHOICE!"=="q" (
    goto :END_PROCESSING
)

REM Validate choice
if "!FILE_CHOICE!"=="" (
    echo Error: No selection made
    pause
    exit /b 1
)

REM Check if choice is within valid range
if !FILE_CHOICE! LSS 1 (
    echo Error: Invalid choice - must be at least 1
    pause
    exit /b 1
)

if !FILE_CHOICE! GTR !count! (
    echo Error: Invalid choice - must be at most !count!
    pause
    exit /b 1
)

call set SUPPRESS_FILE=%%file!FILE_CHOICE!%%

if "!SUPPRESS_FILE!"=="" (
    echo Error: Invalid file selection
    pause
    exit /b 1
)

echo.
echo Selected file: !SUPPRESS_FILE!
echo.
echo ==================================================
echo Starting interactive application...
echo ==================================================
echo.
echo Instructions:
echo   - For each suppression, you will see a preview
echo   - Choose: y=apply, n=skip, a=apply all, q=quit
echo   - Backups will be created automatically
echo.

pause

REM Run the applicator
python src\apply_suppress_comments.py "!SUPPRESS_FILE!" "!TARGET_REPO!"

if errorlevel 1 (
    echo.
    echo ==================================================
    echo Error: Application failed for this file!
    echo ==================================================
    echo Please check the error messages above
    echo.
) else (
    set /a TOTAL_FILES_PROCESSED+=1
    echo.
    echo ==================================================
    echo File processing completed!
    echo ==================================================
    echo.
)

REM Ask if user wants to process another file
echo.
set /p CONTINUE="Do you want to process another suppress file? (y/n): "

if /i "!CONTINUE!"=="y" (
    echo.
    goto :FILE_SELECTION_LOOP
)

:END_PROCESSING

echo.
echo ==================================================
echo All processing completed!
echo ==================================================
echo.
echo Total files processed: !TOTAL_FILES_PROCESSED!
echo Backups saved in: backups\parasoft_backups_YYYYMMDD_HHMMSS\
echo (Located in project root, not in your source code)
echo Review the changes in your source code repository.
echo.

pause
exit /b 0
