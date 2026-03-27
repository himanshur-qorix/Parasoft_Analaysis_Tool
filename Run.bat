@echo off
title Parasoft Analysis Automation
color 0A

echo ==================================================
echo        Parasoft Analysis Tool Execution
echo        Version: V1.0.0
echo ==================================================
echo.

color 00

echo Starting analysis...
echo.

python ParasoftAnalysisTool.py report_dev1.html Qorix_CP_Common_Deviations.xlsx

echo.
echo ==================================================
echo Analysis completed successfully.
echo Command window will close in 5 seconds...
echo ==================================================

timeout /t 5 /nobreak >nul
exit