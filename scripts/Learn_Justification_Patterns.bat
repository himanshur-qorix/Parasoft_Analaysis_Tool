@echo off
REM ============================================================================
REM AI-Powered Justification Pattern Learning & Quality Analysis
REM ============================================================================
REM This script uses Ollama AI to intelligently analyze justification patterns:
REM   1. Identify common justification patterns across modules
REM   2. Distinguish tool-generated vs human-written justifications
REM   3. Use AI to evaluate justification quality
REM   4. Identify specific mistakes in tool-generated justifications
REM   5. Provide AI-powered categorization and recommendations
REM   6. Generate training insights for few-shot learning
REM ============================================================================

REM Change to project root directory
cd /d "%~dp0.."

setlocal enabledelayedexpansion

echo.
echo ================================================================================
echo    AI-POWERED JUSTIFICATION PATTERN LEARNING TOOL
echo ================================================================================
echo.
echo This tool uses Ollama AI to analyze justification quality and identify mistakes:
echo   - Intelligently categorize justification reasons
echo   - Detect tool-generated vs human-written justifications  
echo   - Evaluate quality and identify specific issues
echo   - Provide AI-powered improvement recommendations
echo   - Learn what makes a good justification
echo.
echo Powered by: Ollama LLM (Qwen 2.5 Coder)
echo ================================================================================
echo.

REM Check if Python is available
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Python is not installed or not in PATH
    echo Please install Python 3.7 or higher
    pause
    exit /b 1
)

REM Check if justifications directory exists
if not exist "justifications" (
    echo [ERROR] justifications\ directory not found!
    echo Please ensure you have generated suppression comments first.
    echo.
    echo Run one of these to generate justifications:
    echo   - ParasoftAnalysisTool.py
    echo   - generate_code_fixes.py
    echo.
    pause
    exit /b 1
)

REM Count suppress comment files
set count=0
for %%f in (justifications\*_suppress_comments_*.txt) do set /a count+=1

if %count%==0 (
    echo [WARNING] No suppression comment files found in justifications\ folder
    echo.
    echo Please run analysis on modules first to generate suppression comments.
    pause
    exit /b 1
)

echo [OK] Found %count% suppression comment files to analyze
echo.

REM Check if Ollama is available
echo Checking Ollama AI availability...
python -c "import ollama" >nul 2>&1
if %errorlevel% neq 0 (
    echo.
    echo [WARNING] Ollama not installed - will use rule-based analysis only
    echo.
    echo For AI-powered analysis, install Ollama:
    echo   1. Install Ollama library: pip install ollama
    echo   2. Start Ollama server: ollama serve
    echo   3. Ensure model is available: ollama pull qwen2.5-coder:1.5b-base
    echo.
    echo Continuing with rule-based analysis...
    echo.
) else (
    echo [OK] Ollama library installed
    echo.
    eAI-Powered Pattern Learning Complete!
echo ================================================================================
echo.
echo 📊 Reports Generated:
echo    - Console output above shows AI-powered analysis
echo    - knowledge_base\justification_patterns_report.json (detailed JSON)
echo.
echo 🤖 AI Insights Provided:
echo    - Intelligent categorization of justification reasons
echo    - Quality assessment of tool-generated justifications
echo    - Identification of specific mistakes and issues
echo    - Comparison of tool vs human justification quality
echo    - AI-powered improvement recommendations
echo.
echo 💡 Next Steps:
echo    1. Review quality issues identified by AI
echo    2. Fix generic/poor justifications in your code
echo    3. Use insights to improve tool's justification generation
echo    4. Re-run to verify improvements
echo    5. AI few-shot learning automatically uses better examples
    )
)

echo.
echo ================================================================================
echo Starting Pattern Analysis...
echo ================================================================================
echo.

python src\learn_justification_patterns.py

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Pattern analysis failed!
    pause
    exit /b 1
)

echo.
echo ================================================================================
echo Pattern Learning Complete!
echo ================================================================================
echo.
echo 📊 Reports Generated:
echo    - Console output above shows summary
echo    - knowledge_base\justification_patterns_report.json (detailed JSON)
echo.
echo 💡 Next Steps:
echo    1. Review the cross-module patterns identified
echo    2. Consider documenting common deviations in project standards
echo    3. Use insights to improve justification quality in new code
echo    4. The AI few-shot learning will use these patterns automatically
echo.
echo ================================================================================
echo.

pause
