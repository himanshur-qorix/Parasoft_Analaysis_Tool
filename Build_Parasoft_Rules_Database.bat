@echo off
REM ============================================================================
REM Build_Parasoft_Rules_Database.bat
REM Builds/Rebuilds the Parasoft Rules Database from official HTML documentation
REM Organization: Qorix India Pvt Ltd
REM ============================================================================

echo.
echo ========================================================================
echo   PARASOFT RULES DATABASE BUILDER
echo   Qorix India Pvt Ltd
echo ========================================================================
echo.

echo [INFO] Building Parasoft Rules Database from official documentation...
echo.

python src\ParasoftRulesParser.py

IF %ERRORLEVEL% EQU 0 (
    echo.
    echo [SUCCESS] Parasoft Rules Database built successfully!
    echo.
    echo The database has been saved to:
    echo   data\Parasoft_Rules_Database.json
    echo.
    echo This database contains 1200+ official Parasoft rules with:
    echo   - Rule descriptions
    echo   - Security relevance
    echo   - Violation examples
    echo   - Official repair strategies
    echo   - CWE mappings
    echo   - Standards references
    echo.
    echo This database is used by the Code Fix Generator to provide
    echo official Parasoft-approved fix suggestions and justifications.
    echo.
) ELSE (
    echo.
    echo [ERROR] Failed to build Parasoft Rules Database
    echo Please check the error messages above
    echo.
)

pause
