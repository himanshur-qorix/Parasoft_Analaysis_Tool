# MISRA/CERT Static Analyzer Integration

## Overview

The Parasoft AI Agent now includes integrated MISRA-C:2012 and CERT-C static analysis as a pre-analysis step before the main Parasoft violation analysis workflow.

## What's New

### 1. Source Code Path Configuration

**Config File**: `config/config.json`

```json
{
  "workspace": {
    "source_code_path": ""
  },
  "analysis": {
    "run_misra_cert_checker": true,
    "misra_cert_report_format": "html"
  }
}
```

- `source_code_path`: Default path to your C/C++ source code
- `run_misra_cert_checker`: Enable/disable MISRA/CERT pre-analysis (default: true)
- `misra_cert_report_format`: Report format - `html`, `json`, or `text` (default: html)

### 2. Workflow Updates

The analysis now follows this sequence:

1. **MISRA/CERT Pre-Analysis** (Optional)
   - Runs static analysis on source code
   - Generates detailed MISRA-C:2012 and CERT-C violation report
   - Saves report to `reports/` directory

2. **Parasoft Analysis** (Existing)
   - Analyzes Parasoft report_dev1.html
   - Applies Qorix deviations
   - Generates fixes and justifications
   - Updates knowledge database

## Usage

### Command Line

```bash
# Basic usage with MISRA/CERT pre-analysis
python src/ParasoftAIAgent.py report_dev1.html Mka --source-code D:/MyProject/src

# Full example with all options
python src/ParasoftAIAgent.py report_dev1.html Mka \
  --source-code D:/MyProject/src \
  --workspace D:/MyProject \
  --qorix data/Qorix_CP_Common_Deviations.xlsx \
  --ai-mode hybrid

# Skip MISRA/CERT pre-analysis
python src/ParasoftAIAgent.py report_dev1.html Mka
```

### Command Line Options

| Option | Description |
|--------|-------------|
| `--source-code <path>` | Path to C/C++ source code directory or file |
| `--workspace <path>` | Workspace/repository root path |
| `--qorix <path>` | Path to Qorix deviations Excel file |
| `--ai-mode <mode>` | AI mode: `ai_only`, `hybrid`, or `rules_only` |
| `--no-fixes` | Skip code fix generation |
| `--no-justifications` | Skip justification generation |

## Output

### MISRA/CERT Report

Generated in `reports/` directory:
- Format: `{module_name}_misra_cert_report_{timestamp}.html` (or .json/.txt)
- Contains detailed violation information:
  - Rule ID (MISRA-C:2012 or CERT-C)
  - Severity level
  - Category
  - Description and rationale
  - File, line, and column location
  - Code snippet

### Analysis Results

The complete analysis results JSON includes:
```json
{
  "git_info": {...},
  "misra_cert_analysis": {
    "status": "success",
    "report_path": "reports/Mka_misra_cert_report_20260415_120000.html",
    "violations_found": 42,
    "timestamp": "2026-04-15T12:00:00"
  },
  "analysis": {...},
  "fixes": {...},
  "justifications": {...}
}
```

## Supported Checks

### MISRA-C:2012 Rules
- Rule 1.1: Standard C syntax compliance
- Rule 2.1: Unreachable code
- Rule 2.2: Dead code
- Rule 10.1: Inappropriate essential types
- Rule 11.1: Pointer to function conversions
- Rule 12.1: Operator precedence
- Rule 14.4: Boolean controlling expressions
- Rule 15.1: goto statement usage
- Rule 16.1, 16.4: Switch statement structure
- Rule 17.7: Function return value usage
- Rule 18.1: Pointer arithmetic bounds
- Rule 21.3, 21.6: Standard library usage
- Rule 22.1: Resource management

### CERT-C Rules
- DCL30-C: Storage duration declarations
- EXP33-C, EXP34-C, EXP35-C: Expression safety
- INT30-C, INT32-C, INT33-C: Integer operations
- ARR30-C: Array bounds
- STR31-C: String buffer safety
- MEM30-C, MEM31-C, MEM34-C: Memory management
- FIO30-C: Format string security
- ERR33-C: Error handling

## Configuration in Code

To run MISRA/CERT analysis programmatically:

```python
from src.ParasoftAIAgent import ParasoftAIAgent
from pathlib import Path

agent = ParasoftAIAgent(workspace_path="D:/MyProject")

# Run full analysis with MISRA/CERT pre-check
results = agent.run_full_analysis(
    report_path=Path("report_dev1.html"),
    module_name="Mka",
    source_code_path="D:/MyProject/src",
    generate_fixes=True,
    add_justifications=True,
    qorix_file="data/Qorix.xlsx"
)

# Access MISRA/CERT results
if results['misra_cert_analysis']:
    print(f"Violations: {results['misra_cert_analysis']['violations_found']}")
    print(f"Report: {results['misra_cert_analysis']['report_path']}")
```

## Standalone MISRA/CERT Checker

You can also run the MISRA/CERT checker independently:

```bash
# Analyze a single file
python scripts/misra_cert_checker.py src/main.c --report html --output report.html

# Analyze a directory
python scripts/misra_cert_checker.py src/ --report json --output violations.json

# Text output to console
python scripts/misra_cert_checker.py src/
```

## Benefits

1. **Early Detection**: Catch MISRA/CERT violations before Parasoft analysis
2. **Comprehensive Coverage**: Static analysis patterns for common coding standard violations
3. **Integrated Reports**: Both MISRA/CERT and Parasoft results in one workflow
4. **Flexible Configuration**: Enable/disable via config or command line
5. **Multiple Formats**: HTML, JSON, or text reports

## Notes

- MISRA/CERT pre-analysis is optional and controlled by configuration
- If source code path is not provided, only Parasoft analysis runs
- MISRA/CERT checker errors are logged but don't stop the main workflow
- Reports are timestamped and saved in the `reports/` directory
- Pattern-based detection simulates Parasoft-style violation checking

## See Also

- [QUICKSTART.md](QUICKSTART.md) - Getting started guide
- [ARCHITECTURE.md](ARCHITECTURE.md) - System architecture
- [AI_MODES.md](AI_MODES.md) - AI integration modes
- [EXCEL_REPORT_GUIDE.md](EXCEL_REPORT_GUIDE.md) - Report generation
