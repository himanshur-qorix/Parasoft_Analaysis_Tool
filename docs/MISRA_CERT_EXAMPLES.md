# MISRA/CERT Integration - Quick Examples

## Basic Usage Examples

### Example 1: With MISRA/CERT Pre-Analysis

```powershell
# Run complete analysis with MISRA/CERT pre-check
python src/ParasoftAIAgent.py report_dev1.html Mka --source-code D:/MyProject/src

# Output:
# ==================================================
# RUNNING MISRA/CERT STATIC ANALYZER
# Source Code Path: D:/MyProject/src
# Module: Mka
# ==================================================
# [INFO] Running MISRA/CERT checker on: D:/MyProject/src
# [OK] MISRA/CERT analysis completed successfully
# [OK] Report saved: reports/Mka_misra_cert_report_20260415_120000.html
# 
# [SUCCESS] Analysis completed successfully!
# 
#   MISRA/CERT Pre-Analysis:
#     Violations found: 42
#     Report: reports/Mka_misra_cert_report_20260415_120000.html
#
#   Parasoft Analysis:
#   Total violations: 156
#   ...
```

### Example 2: Using Configuration File

**config/config.json:**
```json
{
  "workspace": {
    "source_code_path": "D:/MyProject/src"
  },
  "analysis": {
    "run_misra_cert_checker": true,
    "misra_cert_report_format": "html"
  }
}
```

**Command:**
```powershell
# Source code path from config will be used automatically
python src/ParasoftAIAgent.py report_dev1.html Mka --source-code D:/MyProject/src
```

### Example 3: Different Report Formats

```powershell
# HTML report (default)
python src/ParasoftAIAgent.py report_dev1.html Mka --source-code D:/MyProject/src

# JSON report (configure in config.json)
# Set "misra_cert_report_format": "json"

# Text report (configure in config.json)
# Set "misra_cert_report_format": "text"
```

### Example 4: Without MISRA/CERT Pre-Analysis

```powershell
# Skip MISRA/CERT - just run Parasoft analysis
python src/ParasoftAIAgent.py report_dev1.html Mka
```

### Example 5: Standalone MISRA/CERT Checker

```powershell
# Run MISRA/CERT checker independently
python src\misra_cert_checker.py D:/MyProject/src --report html --output my_report.html

# Analyze single file
python src\misra_cert_checker.py D:/MyProject/src/main.c --report text

# JSON output
python src\misra_cert_checker.py D:/MyProject/src --report json --output violations.json
```

## Batch File Usage

The `Run.bat` script now supports source code path:

```
==================================================
       Parasoft AI Agent - Version 2.0.0
==================================================

Enter module name (e.g., Mka): Mka
Enter source code path for MISRA/CERT analysis (optional, press Enter to skip): D:\MyProject\src
[INFO] Source code path: D:\MyProject\src
[INFO] MISRA/CERT pre-analysis will be performed

AI Mode Selection
==================================================
Choose fix generation mode:
  [1] AI Only      - Use Ollama AI for all violations (requires Ollama)
  [2] Hybrid       - Smart: AI for complex, rules for standard (recommended)
  [3] Rules Only   - Use only rule-based fixes (no AI required)

Enter choice (1-3) [default: 2]: 2
```

## Sample Report Output

### MISRA/CERT HTML Report

The generated HTML report includes:

- **Summary Statistics**
  - Total violations
  - Files analyzed
  - Lines of code analyzed
  - Violations by standard (MISRA/CERT)
  - Severity distribution

- **Detailed Violations Table**
  - Rule ID (e.g., MISRA-C:2012 Rule 14.4)
  - Standard (MISRA/CERT)
  - Severity (Required/Advisory/High/Medium/Low)
  - Category (Control Flow, Memory, etc.)
  - Description
  - File and line number
  - Code snippet
  - Rationale

### Sample Violation Entry

```
═══════════════════════════════════════════════════════════════════
File: src/main.c
═══════════════════════════════════════════════════════════════════

[MISRA-C:2012 Rule 14.4] Required | Control Flow
────────────────────────────────────────────────
Description:
  The controlling expression of an if statement shall be essentially Boolean.

Line 45, Column 5:
  if (x = getValue()) {
  ^

Rationale:
  Non-Boolean conditions in if/while can mask unintentional assignments.
```

## Programmatic Usage

### Python API Example

```python
from pathlib import Path
from src.ParasoftAIAgent import ParasoftAIAgent

# Initialize agent
agent = ParasoftAIAgent(workspace_path="D:/MyProject")

# Run MISRA/CERT checker only
misra_results = agent.run_misra_cert_checker(
    source_code_path="D:/MyProject/src",
    module_name="Mka",
    report_format="html"
)

if misra_results['status'] == 'success':
    print(f"Violations found: {misra_results['violations_found']}")
    print(f"Report: {misra_results['report_path']}")

# Run full analysis with MISRA/CERT pre-check
full_results = agent.run_full_analysis(
    report_path=Path("report_dev1.html"),
    module_name="Mka",
    source_code_path="D:/MyProject/src",
    generate_fixes=True,
    add_justifications=True
)

# Access results
print(f"MISRA/CERT: {full_results['misra_cert_analysis']['violations_found']} violations")
print(f"Parasoft: {full_results['analysis']['total_violations']} violations")
```

## Configuration Options

### Disable MISRA/CERT Checker

**config.json:**
```json
{
  "analysis": {
    "run_misra_cert_checker": false
  }
}
```

### Change Report Format

**config.json:**
```json
{
  "analysis": {
    "misra_cert_report_format": "json"
  }
}
```

Options: `"html"`, `"json"`, `"text"`

## Detected Violations

### Common MISRA-C Violations Detected

1. **Control Flow Issues**
   - goto statement usage
   - Missing default in switch
   - Non-Boolean conditions
   - Multiple loop exits

2. **Memory Safety**
   - malloc/free usage
   - Pointer arithmetic
   - Array bounds issues

3. **Code Quality**
   - Unreachable code
   - Dead code
   - Unused return values

### Common CERT-C Violations Detected

1. **Security Issues**
   - Null pointer dereferences
   - Buffer overflows
   - Format string vulnerabilities

2. **Memory Management**
   - Use-after-free
   - Memory leaks
   - Double-free

3. **Integer Safety**
   - Integer overflow
   - Division by zero
   - Unsigned wrapping

## Troubleshooting

### Source Path Not Found

```
Warning: Source code path does not exist: D:/Invalid/Path
Skipping MISRA/CERT pre-analysis...
```

**Solution:** Verify the path exists and contains C/C++ source files.

### No Violations Found

```
[INFO] Analyzed 5 file(s), found 0 violation(s).
```

**Reasons:**
- Source code follows MISRA/CERT guidelines
- Only pattern-based checks (may miss complex issues)
- No .c/.cpp/.h/.hpp files in directory

### Checker Script Not Found

```
[ERROR] MISRA/CERT checker script not found: src/misra_cert_checker.py
```

**Solution:** Ensure `misra_cert_checker.py` exists in the `scripts/` directory.

## Integration Tips

1. **First Run**: Start with MISRA/CERT analysis to identify coding standard violations early
2. **Regular Checks**: Run before Parasoft analysis to catch issues at multiple levels
3. **Report Comparison**: Compare MISRA/CERT and Parasoft reports for comprehensive coverage
4. **Focus Areas**: Use MISRA/CERT for standard compliance, Parasoft for project-specific rules
5. **Automated Workflow**: Integrate both checks in CI/CD pipeline

## See Also

- [MISRA_CERT_INTEGRATION.md](MISRA_CERT_INTEGRATION.md) - Complete integration guide
- [AI_QUICKSTART.md](AI_QUICKSTART.md) - Getting started with AI features
- [EXAMPLE_WORKFLOW.md](EXAMPLE_WORKFLOW.md) - Complete workflow examples

