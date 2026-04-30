@echo off
REM ========================================================================
REM Feedback Learning GUI Launcher
REM Interactive interface for providing feedback on MISRA/CERT violations
REM ========================================================================

setlocal

REM Get script directory
set "SCRIPT_DIR=%~dp0"
cd /d "%SCRIPT_DIR%"

echo.
echo ========================================================================
echo   FEEDBACK LEARNING SYSTEM
echo   Machine Learning for MISRA/CERT Detection Improvement
echo ========================================================================
echo.

REM Check if virtual environment exists
if exist "venv\Scripts\activate.bat" (
    echo [INFO] Activating virtual environment...
    call venv\Scripts\activate.bat
) else (
    echo [WARNING] Virtual environment not found. Using system Python...
)

REM Check if FeedbackLearner exists
if not exist "src\FeedbackLearner.py" (
    echo [ERROR] FeedbackLearner.py not found!
    echo Please ensure the regression learning system is installed.
    pause
    exit /b 1
)

echo [INFO] Launching Feedback GUI...
echo.

REM Launch the feedback GUI
python src\feedback_gui.py

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] Failed to launch Feedback GUI (Error Code: %ERRORLEVEL%)
    echo.
    echo Possible causes:
    echo   1. Python not installed or not in PATH
    echo   2. Required dependencies missing (tkinter)
    echo   3. FeedbackLearner module error
    echo.
    pause
)

endlocal
