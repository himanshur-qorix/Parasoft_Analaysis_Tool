@echo off
title Consolidate Knowledge Bases
color 0A

REM Change to project root directory
cd /d "%~dp0.."

echo ==================================================
echo    Knowledge Base Consolidation Tool
echo    Parasoft AI Agent - Version 2.1.1
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
if exist "venv\" (
    call venv\Scripts\activate.bat
)

echo [INFO] Starting knowledge base consolidation...
echo.

REM Run the consolidation tool
python src\consolidate_knowledge.py

if errorlevel 1 (
    echo.
    echo ==================================================
    echo Error: Consolidation failed!
    echo ==================================================
    pause
    exit /b 1
)

echo.
echo ==================================================
echo SUCCESS: Knowledge bases consolidated!
echo ==================================================
echo.
echo The master knowledge database has been created at:
echo   knowledge_base\Master_KnowledgeDatabase.json
echo.
echo An Excel report has been exported to:
echo   reports\Master_Knowledge_Report.xlsx
echo.
echo Next steps:
echo   1. Review the master database and recommendations
echo   2. Query the master knowledge base using:
echo      scripts\Query_Master_Knowledge.bat
echo   3. Use insights to improve future analyses
echo.
pause
