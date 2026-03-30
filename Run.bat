@echo off
title Parasoft Analysis Automation
color 0A

echo ==================================================
echo        Parasoft Analysis Tool Execution
echo        Version: V1.0.0
echo ==================================================
echo.

color 00

echo.
echo ==================================================
echo Analysis completed successfully.

echo Starting analysis...
echo.

REM Usage: Run with or without --apply to auto-fix where possible
python ParasoftAnalysisTool.py report_dev1.html --apply

echo.
echo ==================================================
echo Analysis completed successfully.
echo Command window will close in 5 seconds...
echo ==================================================

timeout /t 5 /nobreak >nul
exit