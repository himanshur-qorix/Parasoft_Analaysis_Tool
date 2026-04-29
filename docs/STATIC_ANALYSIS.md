# Static Code Analysis Guide

**Complete guide to built-in static analysis, MISRA/CERT checking, violation capture, and auto-generation**

**Version:** 4.0.0  
**Developer:** Himanshu R  
**Organization:** Qorix India Pvt Ltd

---

## 🎉 **NEW in v4.0** - Enhanced MISRA/CERT Detection

The Static Analyzer has been **significantly enhanced** with:
- ✅ **1,204 Parasoft Rules** integrated
- ✅ **15+ MISRA checks** (4x increase)
- ✅ **13+ CERT checks** (4x increase)
- ✅ **12+ CWE mappings** for security compliance
- ✅ **Knowledge base patterns** with proven fixes

**👉 See [STATIC_ANALYZER_V4_ENHANCEMENTS.md](STATIC_ANALYZER_V4_ENHANCEMENTS.md) for full details!**

---

## Table of Contents

1. [Overview](#overview)
2. [Quick Start](#quick-start)
3. [Checks Performed](#checks-performed)
4. [Auto-Generate Workflow](#auto-generate-workflow)
5. [MISRA/CERT Integration](#misracert-integration)
6. [Violation Capture](#violation-capture)
7. [Polyspace Integration](#polyspace-integration)
8. [Report Formats](#report-formats)

---

## Overview

Built-in **Static Code Analyzer** that directly analyzes C/C++ source code without requiring external tools.

### Capabilities

- ✅ **Runtime Error Detection** - Null pointers, buffer overflows, division by zero
- ✅ **MISRA C Compliance** - MISRA C:2012 rules checking
- ✅ **CERT C Standards** - Secure coding standards validation
- ✅ **Code Quality** - Dead code, magic numbers, complexity metrics
- ✅ **Auto-Generate Reports** - Create reports from source code without Parasoft
- ✅ **Polyspace-Style** - Color-coded reporting (RED/ORANGE/GREY)

### Key Features

- 🚀 **No External Tools Required** - Standalone C/C++ analyzer
- 🎨 **Color-Coded Reports** - Polyspace-compatible severity coding
- 📊 **Multiple Formats** - HTML, Excel, JSON outputs
- 🔍 **Context-Aware** - Multi-line code context in reports
- ⚡ **Fast Analysis** - Processes thousands of lines quickly
- 🎯 **False Positive Reduction** - Smart filtering reduces noise

---

## Quick Start

### Method 1: GUI (Recommended)

```bash
scripts\Launch_GUI.bat
```

1. Enter module name
2. Enter source code path in "Input Path" field
3. Click "🔍 Run Static Analyzer" button
4. View results in `reports\` folder

### Method 2: Batch File

```batch
.\batch_support\Run_Static_Analyzer.bat
```

Follow prompts:
1. Enter source code directory (e.g., `D:\Code\Mka\src`)
2. Enter module name (e.g., `Mka`)
3. Wait for analysis

### Method 3: Command Line

```bash
# Basic usage
python src\run_static_analyzer.py D:\Code\Mka Mka

# With options
python src\run_static_analyzer.py D:\Code\Mka Mka --format html --output reports\
```

### Output Files

All generated in `reports\` folder:
- `{Module}_static_analysis_report.html` - Interactive web report
- `{Module}_violations_report.xlsx` - Excel workbook
- `{Module}_violations.json` - Raw violation data
- `knowledge_base\{Module}_KnowledgeDatabase.json` - Knowledge DB

---

## Checks Performed

### Runtime Error Detection

| Check | Description | Severity |
|-------|-------------|----------|
| NULL_DEREF | Potential null pointer dereference | CRITICAL |
| DIV_ZERO | Division by zero | CRITICAL |
| ARRAY_INDEX | Array access without bounds check | HIGH |
| UNINITIALIZED | Uninitialized variable usage | HIGH |
| OVERFLOW | Potential integer overflow | MEDIUM |

### MISRA C Compliance

| Rule | Description | Severity |
|------|-------------|----------|
| MISRA_10_3 | Implicit enum conversion | MEDIUM |
| MISRA_20_1 | #include after code | MEDIUM |
| MISRA_5_4 | Macro name too long | MEDIUM |
| MISRA_8_9 | Static var placement | LOW |

### CERT C Secure Coding

| Rule | Description | Severity |
|------|-------------|----------|
| CERT_INT31 | Integer conversion | MEDIUM |
| CERT_STR31 | Unsafe string functions | HIGH |
| CERT_MEM35 | Memory allocation check | HIGH |

### Code Quality

| Check | Description | Severity |
|-------|-------------|----------|
| DEAD_CODE | Unreachable code | LOW |
| MAGIC_NUMBER | Hard-coded numbers | LOW |
| LONG_FUNCTION | Function too long (>50 lines) | LOW |
| COMPLEX_EXPR | Complex boolean logic | LOW |

## 📝 Example Output

```
================================================================================
Static Analysis Summary:
  Files analyzed: 45
  Lines analyzed: 8,234
  Total findings: 127
  [CRITICAL]: 12
  [HIGH]: 34
  [MEDIUM]: 56
  [LOW]: 25
================================================================================

Output files:
  [HTML Report]: reports\Tls_static_analysis_report.html
  [Excel Report]: reports\Tls_violations_report.xlsx
  [Knowledge Base]: knowledge_base\Tls_KnowledgeDatabase.json
  [Violations JSON]: data\Tls_violations.json

Color Code Summary:
  RED (Critical):    12 - Proven errors (null deref, div by zero)
  ORANGE (High):     34 - Likely errors (uninitialized, buffer overflow)
  GREY (Quality):    81 - Code quality issues (MISRA, CERT, style)
================================================================================
```

## 🎨 Color-Coded Reports

### Polyspace-Style Color Coding

The static analyzer uses **Polyspace-compatible color codes** to categorize violations:

#### 🔴 RED CODE (Critical)
- **Meaning**: Proven runtime errors that will cause crashes or undefined behavior
- **Examples**:
  - Null pointer dereference
  - Division by zero
  - Integer overflow
- **Action**: **Fix immediately** - These are guaranteed bugs

#### 🟠 ORANGE CODE (High Priority)
- **Meaning**: Likely errors with high probability of causing runtime issues
- **Examples**:
  - Uninitialized variables
  - Array access without bounds checking
  - Unsafe string operations
- **Action**: **Fix soon** - High risk of bugs

#### ⚪ GREY CODE (Code Quality)
- **Meaning**: Code quality issues, MISRA/CERT violations, style problems
- **Examples**:
  - MISRA C compliance issues
  - CERT C secure coding violations
  - Dead code, magic numbers
- **Action**: **Review and fix** - Improves code maintainability and compliance

## 📊 Report Structure

### HTML Report (NEW!)

**Color-coded interactive web report** with:
- **Dashboard**: Summary statistics with RED/ORANGE/GREY breakdown
- **Tabbed Navigation**: Filter by color code (All / RED / ORANGE / GREY)
- **Violation Cards**: Each violation shows:
  - Violation ID and severity badge
  - File location and line number
  - Full description
  - **Multi-line code context** (5 lines before + violation + 5 lines after)
  - Syntax-highlighted code snippets
- **Professional Design**: Qorix branding with modern UI
- **No external dependencies**: Self-contained HTML file

Open with any web browser for interactive analysis!

### Excel Report (Enhanced)

Multi-sheet workbook with **color-coded rows**:

#### Sheets:
1. **All Violations** - Complete list with color-coded rows (RED/ORANGE/GREY backgrounds)
2. **RED - Critical** - Only RED code violations with multi-line context
3. **ORANGE - High Priority** - Only ORANGE code violations with context
4. **GREY - Code Quality** - Only GREY code violations
5. **Category Sheets** - Violations grouped by category (Runtime Error, MISRA, CERT, etc.)
6. **Summary** - Statistics, category breakdown, top violation types

#### Columns:
- Violation ID
- Color Code (RED/ORANGE/GREY)
- Check ID
- Severity (CRITICAL/HIGH/MEDIUM/LOW)
- Category
- Description
- File
- Line
- Code Snippet (Single Line)
- **Code Context (Multi-line)** - NEW! 11 lines of context around violation

### Viewing Reports

**HTML Report (Recommended for Review):**
```batch
cd reports
start Tls_static_analysis_report.html
```
- Filter by color code using tabs
- Hover over violation cards for better visibility
- Code snippets with syntax highlighting
- Responsive design works on any screen size

**Excel Report (For Data Analysis):**
```batch
cd reports
start Tls_violations_report.xlsx
```
- Color-coded rows for quick identification
- Multi-line code context in dedicated column
- Filter and sort by any column
- Export to other tools

## 🎨 Smart Context-Aware Analysis

The analyzer reduces false positives by checking context:

**Example 1: Null Check Detection**
```c
// This WON'T trigger NULL_DEREF:
if (ptr != NULL) {
    *ptr = value;  // ✅ Has null check above
}

// This WILL trigger NULL_DEREF:
*ptr = value;  // ❌ No null check
```

**Example 2: Bounds Check**
```c
// This WON'T trigger ARRAY_INDEX:
if (index < ARRAY_SIZE) {
    arr[index] = value;  // ✅ Has bounds check
}

// This WILL trigger ARRAY_INDEX:
arr[index] = value;  // ❌ No bounds check
```

**Example 3: Division by Zero**
```c
// This WON'T trigger DIV_ZERO:
result = value / 10;  // ✅ Constant non-zero

// This WILL trigger DIV_ZERO:
result = value / divisor;  // ❌ Variable divisor
```

## 🔧 Advanced Usage

### Analyze Specific File Types
```batch
python run_static_analyzer.py D:\Code\ Module --file-patterns *.c *.h
```

### Skip Automatic Fix Generation
```batch
python run_static_analyzer.py D:\Code\ Module --no-fixes
```

### Custom Output Directory
```batch
python run_static_analyzer.py D:\Code\ Module --output-dir D:\Reports
```

### Programmatic Usage
```python
from StaticCodeAnalyzer import StaticCodeAnalyzer
from pathlib import Path

analyzer = StaticCodeAnalyzer()
violations = analyzer.analyze_directory(Path('D:/Code/Mka'))

# Filter critical issues
critical = analyzer.filter_by_severity('CRITICAL')

# Filter by category
runtime_errors = analyzer.filter_by_category('Runtime Error')

# Get summary
summary = analyzer.get_summary()
print(f"Found {summary['critical']} critical issues")
```

## 🎯 Integration with Existing Workflow

### Complete Analysis Pipeline

```batch
REM 1. Run static analysis
cd scripts
Run_Polyspace.bat
REM Input: D:\Code\Mka
REM Module: Mka

REM 2. Review Excel report
start ..\reports\Mka_violations_report.xlsx

REM 3. Generate AI-powered fixes
Generate_Code_Fixes.bat
REM Select: Mka, Hybrid mode

REM 4. Review fixes
cd ..\src
python view_fixes_interactive.py Mka

REM 5. Apply suppressions
cd ..\scripts
Apply_Suppressions.bat
```

### Combine with Parasoft

```batch
REM Step 1: Run Par asoft analysis
scripts\Run.bat

REM Step 2: Run static analysis (same module)
scripts\Run_Polyspace.bat

REM Both findings merged into same knowledge base!
REM Generate combined report
```

## 🛠️ Customization

### Adding Custom Checks

Edit `StaticCodeAnalyzer.py`:

```python
# Add to _init_checkers()
self.runtime_checks['MY_CHECK'] = {
    'pattern': r'pattern_here',
    'description': 'My custom check',
    'severity': 'HIGH',
    'category': 'Runtime Error'
}
```

### Adjusting Thresholds

```python
# Function length threshold (default: 50 lines)
if function_lines > 100:  # Change to 100
    self._add_violation(...)
```

### File Patterns

Default patterns: `*.c`, `*.cpp`, `*.h`, `*.hpp`

To analyze other extensions:
```batch
python run_static_analyzer.py D:\Code Module --file-patterns *.c *.cc *.cxx
```

## 🎭 Severity Levels

| Severity | Meaning | Action |
|----------|---------|--------|
| **CRITICAL** | Definite error, will cause crashes/bugs | Fix immediately |
| **HIGH** | Likely error, high risk of issues | Fix soon |
| **MEDIUM** | Code quality/compliance issue | Review and fix |
| **LOW** | Style/convention violation | Fix when convenient |

## 📈 Performance

**Typical Analysis Speeds:**
- Small project (< 10K lines): < 5 seconds
- Medium project (10-50K lines): 10-30 seconds
- Large project (50-200K lines): 1-3 minutes

**Memory Usage:**
- Minimal (< 100MB for most projects)
- No external tools required
- Pure Python implementation

## 🔍 Limitations

**Current Version Limitations:**
1. Pattern-based detection (not full semantic analysis)
2. Some false positives possible (reduced by context checks)
3. Limited to C/C++ (no other languages yet)
4. Basic data flow analysis
5. Does NOT detect:
   - Complex race conditions
   - All memory leaks
   - Performance issues
   - Architectural problems

**Future Enhancements:**
- Integration with clang static analyzer
- More advanced data flow tracking
- Custom rule definitions via config file
- Support for more languages

## 💡 Best Practices

1. **Run Regularly**: Analyze code after each major change
2. **Fix Critical First**: Address CRITICAL and HIGH severity issues immediately
3. **Review Context**: Check code snippet in report to understand violation
4. **Use AI Fixes**: Let AI generate suggested fixes in hybrid mode
5. **Track Progress**: Monitor violation trends over time
6. **Combine Tools**: Run both Parasoft and static analysis for comprehensive coverage

## 🆚 Comparison with Other Tools

| Feature | StaticAnalyzer | Polyspace | Parasoft |
|---------|---------------|-----------|----------|
| **Cost** | Free (built-in) | Commercial | Commercial |
| **Setup** | None | Installation | Installation |
| **Speed** | Fast | Slow | Medium |
| **Accuracy** | Good | Excellent | Excellent |
| **False Positives** | Low-Medium | Very Low | Low |
| **Integration** | Native | Reports | Reports |

---

## Auto-Generate Workflow

### Overview

The tool supports **automatic report generation** when Parasoft's `report_dev1.html` is not available. Analyze source code directly without needing a pre-existing Parasoft report.

### Intelligent Report Detection

When you run the tool, it follows this decision tree:

```
Start Analysis
     ↓
Check: Does parasoftReport\report_dev1.html exist?
     ├─ YES → Use Parasoft report
     │         ├─ Source path provided? → Also run MISRA/CERT pre-analysis
     │         └─ No source path → Analyze Parasoft report only
     │
     └─ NO → Check: Source code path provided?
               ├─ YES → Generate MISRA/CERT report automatically
               │         └─ Use generated report for analysis
               │
               └─ NO → ERROR: Need either report or source code
```

### Automatic Report Generation

When `parasoftReport\report_dev1.html` is missing but source code path is provided:

1. **Tool runs**: `python src\misra_cert_checker.py <source_path> --report html --output reports\<module>_misra_cert_report.html`
2. **Report generated**: MISRA-C:2012 and CERT-C violations detected
3. **Report used**: Generated HTML report becomes input for analysis
4. **Full workflow continues**: Qorix mapping, fix generation, knowledge base updates

### Usage Examples

**Example 1: No Report - Generate from Source**

```powershell
# Run the batch script
batch_support\Run.bat

# Prompts:
Enter module name: Mka
Enter source code path: D:\MyProject\src

# Output:
==================================================
parasoftReport\report_dev1.html not found
==================================================
Generating MISRA/CERT report from source code...

[INFO] Analyzing: D:\MyProject\src
[INFO] Analyzed 15 file(s), found 42 violation(s).
[OK] MISRA/CERT report generated: reports\Mka_misra_cert_report.html
[INFO] This report will be used for analysis

Running analysis with Qorix integration...
[INFO] Detected MISRA/CERT report format
[INFO] Parsed 42 violations from MISRA/CERT report
```

**Example 2: With Parasoft Report + Source Code**

```powershell
# Have parasoftReport\report_dev1.html
batch_support\Run.bat

# Prompts:
Enter module name: Mka
Enter source code path: D:\MyProject\src

# Output:
[INFO] Found parasoftReport\report_dev1.html
[INFO] Running MISRA/CERT pre-analysis...
[OK] Pre-analysis complete
[INFO] Analyzing Parasoft report...
```

### Benefits

- ✅ **No Parasoft Required** - Analyze code without commercial tools
- ✅ **Consistent Format** - Same workflow regardless of report source
- ✅ **Automatic Fallback** - Seamlessly switches between sources
- ✅ **Comprehensive Coverage** - MISRA, CERT, and custom checks

---

## MISRA/CERT Integration

### Overview

Integrated MISRA-C:2012 and CERT-C static analysis runs as a pre-analysis step before main Parasoft violation analysis.

### Configuration

Edit `config/config.json`:

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

**Options:**
- `source_code_path`: Default path to C/C++ source
- `run_misra_cert_checker`: Enable/disable pre-analysis (default: `true`)
- `misra_cert_report_format`: `html`, `json`, or `text` (default: `html`)

### Analysis Workflow

1. **MISRA/CERT Pre-Analysis** (Optional)
   - Runs static analysis on source code
   - Generates detailed MISRA-C:2012 and CERT-C violation report
   - Saves report to `reports/` directory

2. **Parasoft Analysis** (Main)
   - Analyzes Parasoft report_dev1.html
   - Applies Qorix deviations
   - Generates fixes and justifications
   - Updates knowledge database

### Command Line Usage

```bash
# Basic usage with MISRA/CERT pre-analysis
python src\run_agent.py parasoftReport\report_dev1.html Mka --source-code D:/MyProject/src

# Full example with all options
python src\run_agent.py parasoftReport\report_dev1.html Mka \
  --source-code D:/MyProject/src \
  --workspace D:/MyProject \
  --qorix data/Qorix_CP_Common_Deviations.xlsx \
  --ai-mode hybrid

# Skip MISRA/CERT pre-analysis
python src\run_agent.py parasoftReport\report_dev1.html Mka
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

### MISRA C:2012 Rules Checked

**Supported Categories:**
- **Type System** (Rules 10.x) - Type conversions, casts
- **Expressions** (Rules 12.x) - Side effects, precedence
- **Control Flow** (Rules 14.x, 15.x) - Loops, switches
- **Functions** (Rules 17.x) - Function declarations, parameters
- **Preprocessing** (Rules 19.x, 20.x) - Macros, includes
- **Standard Library** (Rules 21.x) - Library function usage

### CERT C Rules Checked

**Supported Sections:**
- **INT** - Integer handling
- **STR** - String handling  
- **MEM** - Memory management
- **FIO** - File I/O
- **ERR** - Error handling
- **ARR** - Array usage

### Example MISRA/CERT Report

```
================================================================================
MISRA/CERT Analysis Report
Module: Mka
Generated: 2026-04-29 14:32:15
================================================================================

MISRA C:2012 Violations: 28
├─ Rule 10.3 (Type conversion): 12 occurrences
├─ Rule 10.4 (Essential type): 8 occurrences
├─ Rule 14.4 (If statement): 5 occurrences
└─ Rule 20.1 (Include directive): 3 occurrences

CERT C Violations: 14
├─ INT31-C (Integer conversion): 8 occurrences
├─ STR31-C (String size): 4 occurrences
└─ MEM35-C (Allocation check): 2 occurrences

Total Violations: 42
Output: reports\Mka_misra_cert_report.html
================================================================================
```

---

## Violation Capture

### Overview

**Direct Violation Capture** extracts violations directly from C/C++ source code, bypassing HTML report parsing.

### Use Cases

- Capture violations from source without Parasoft
- Re-analyze code after fixes applied
- Build custom violation databases
- Integrate with CI/CD pipelines

### Usage

**Option 1: GUI**
```
1. Launch scripts\Launch_GUI.bat
2. Go to "Advanced" tab
3. Click "Capture Violations" button
4. Select source directory
```

**Option 2: Batch File**
```powershell
.\batch_support\Capture_Violations.bat
```

**Option 3: Command Line**
```bash
# Capture from directory
python src\capture_violations.py D:\Code\Mka --module Mka

# Capture single file
python src\capture_violations.py D:\Code\Mka\Mka_Cfg.c --module Mka

# With output file
python src\capture_violations.py D:\Code\Mka --module Mka --output violations.json
```

### What Gets Captured

- **Suppression Comments** - Existing `parasoft-suppress` directives
- **Violation Context** - Surrounding code (5 lines before/after)
- **File Locations** - Exact line numbers
- **Rule IDs** - MISRA, CERT, Parasoft rule identifiers
- **Justifications** - Reasoning from comments

### Output Format

```json
{
  "violations": [
    {
      "file": "D:\\Code\\Mka\\Mka_Cfg.c",
      "line": 142,
      "rule_id": "CERT_C-INT31-i-2",
      "type": "suppression",
      "justification": "Enum to uint16 cast is safe in this context",
      "code_context": {
        "before": ["line 137", "line 138", ..., "line 141"],
        "violation_line": "uint16 timeout = (uint16)TIMEOUT_ENUM;",
        "after": ["line 143", "line 144", ..., "line 147"]
      }
    }
  ],
  "summary": {
    "total_violations": 42,
    "suppressed": 38,
    "open": 4,
    "files_analyzed": 15
  }
}
```

### Improving Capture Accuracy

**1. Add Clear Comments**
```c
/* parasoft-suppress CERT_C-INT31-i-2 "Enum to uint cast is safe" */
uint16 value = (uint16)MY_ENUM;
```

**2. Use Multi-Line Comments**
```c
/* parasoft-begin-suppress CERT_C-INT31-i-2
 * All enum to uint16 casts in this section are safe
 * because enum values are validated at compile time.
 */
uint16 a = (uint16)ENUM_A;
uint16 b = (uint16)ENUM_B;
/* parasoft-end-suppress CERT_C-INT31-i-2 */
```

**3. Include Rule Details**
```c
/* parasoft-suppress MISRA-C:2012 Rule 10.3
 * Deviation approved by architecture team - ref: DOC-12345
 */
```

### Benefits

- ✅ **Direct Source Analysis** - No HTML parsing needed
- ✅ **Fast Processing** - Analyze thousands of lines quickly
- ✅ **Context Preservation** - Full code context captured
- ✅ **CI/CD Integration** - Easily integrate with build pipelines
- ✅ **Incremental Updates** - Re-analyze changed files only

---

## Polyspace Integration

### Overview

The static analyzer generates **Polyspace-compatible reports** with the same color-coding system.

### Polyspace Color Codes

#### 🔴 RED CODE (Critical Runtime Errors)

**Meaning**: Proven runtime errors that **will** cause crashes or undefined behavior

**Examples:**
- Null pointer dereference
- Division by zero
- Integer overflow
- Buffer overflow

**Action**: **Fix immediately** - These are guaranteed bugs

**Code Example:**
```c
int* ptr = NULL;
*ptr = 10;  // RED: Null pointer dereference
```

#### 🟠 ORANGE CODE (High Priority Issues)

**Meaning**: Likely errors with high probability of causing runtime issues

**Examples:**
- Uninitialized variables
- Array access without bounds checking
- Unsafe string operations
- Missing null checks

**Action**: **Fix soon** - High risk of bugs

**Code Example:**
```c
int value;  // Uninitialized
if (value > 10) {  // ORANGE: Uninitialized variable
    doSomething();
}
```

#### ⚪ GREY CODE (Code Quality Issues)

**Meaning**: Code quality issues, MISRA/CERT violations, style problems

**Examples:**
- MISRA C compliance issues
- CERT C secure coding violations
- Dead code
- Magic numbers
- Naming conventions

**Action**: **Review and fix** - Improves code maintainability

**Code Example:**
```c
if (timeout == 1000) {  // GREY: Magic number
    return;
}
```

### Report Compatibility

**HTML Reports:**
- Same color-coding as Polyspace
- Interactive filtering by color
- Dashboard with color breakdown
- Click to view code context

**Excel Reports:**
- Color column (RED/ORANGE/GREY)
- Sort and filter by color
- Compatible with Polyspace Excel exports

### Comparison with Polyspace

| Feature | Static Analyzer | Polyspace |
|---------|----------------|-----------|
| **Color Codes** | RED, ORANGE, GREY | RED, ORANGE, GREEN, GREY |
| **Proven Errors** | Pattern-based | Full semantic analysis |
| **Setup Time** | < 1 minute | Hours/Days |
| **Cost** | Free (built-in) | Commercial license |
| **Speed** | Fast (minutes) | Slow (hours) |
| **Accuracy** | Good | Excellent |
| **False Positives** | Low-Medium | Very Low |

### When to Use Each

**Use Static Analyzer:**
- Quick daily/CI checks
- Pre-submission validation
- Code reviews
- Initial codebase assessment

**Use Polyspace:**
- Critical safety systems
- Certification requirements (ISO 26262, DO-178C)
- Deep semantic analysis
- Zero tolerance for false negatives

### Integration Workflow

```
1. Daily: Static Analyzer (fast feedback)
2. Weekly: Parasoft Analysis (compliance)
3. Monthly: Polyspace (deep analysis)
4. Release: All three (complete validation)
```

---

## Report Formats

### HTML Reports

**Interactive Web Reports** with:
- **Dashboard**: Summary statistics with color breakdown
- **Tabbed Navigation**: Filter by All / RED / ORANGE / GREY
- **Violation Cards**: Each shows:
  - Violation ID and severity badge
  - File location and line number
  - Full description
  - Multi-line code context (5 lines before + violation + 5 lines after)
  - Syntax-highlighted code snippets
- **Professional Design**: Qorix branding with modern UI
- **Self-Contained**: No external dependencies

**Open with any web browser for interactive analysis!**

### Excel Reports

**Excel Workbooks** with:
- **Summary Sheet**: Statistics and charts
- **Violations Sheet**: Complete violation list with:
  - Color coding (RED/ORANGE/GREY)
  - Rule ID and description
  - File path and line number
  - Code context (3-5 lines)
  - Severity and category
- **Filtering**: Auto-filters on all columns
- **Sorting**: Pre-sorted by severity
- **Charts**: Violation distribution graphs

### JSON Reports

**Machine-Readable Format** for:
- CI/CD integration
- Custom reporting tools
- Data analysis
- Trend tracking

**Structure:**
```json
{
  "metadata": {
    "module": "Mka",
    "analyzed": "2026-04-29T14:32:15",
    "files_analyzed": 15,
    "lines_analyzed": 8234
  },
  "summary": {
    "total_violations": 127,
    "by_severity": {
      "CRITICAL": 12,
      "HIGH": 34,
      "MEDIUM": 56,
      "LOW": 25
    },
    "by_color": {
      "RED": 12,
      "ORANGE": 34,
      "GREY": 81
    }
  },
  "violations": [...]
}
```

---

## Best Practices

### 1. Run Analysis Regularly
- **Daily**: Quick static analysis on changed files
- **Pre-Commit**: Validate before committing
- **CI/CD**: Automated analysis in build pipeline

### 2. Prioritize Fixes
1. **RED** violations first (proven bugs)
2. **ORANGE** violations next (likely bugs)
3. **GREY** violations last (quality/compliance)

### 3. Use Appropriate Tools
- **Quick Checks**: Built-in static analyzer
- **Compliance**: MISRA/CERT analyzer
- **Complete Analysis**: Parasoft + AI fixes
- **Critical Systems**: Add Polyspace

### 4. Track Progress
- Monitor violation trends over time
- Set reduction goals (e.g., "reduce RED by 50% this sprint")
- Celebrate improvements

### 5. Learn from Reports
- Review code context in reports
- Understand why violations occur
- Share knowledge with team

---

## Troubleshooting

**Issue: "No violations found"**
- Check source path is correct
- Verify files are C/C++ (*.c, *.cpp, *.h, *.hpp)
- Try with `--verbose` flag for detailed output

**Issue: "Too many false positives"**
- Use `--filter` option to exclude patterns
- Adjust severity thresholds in code
- Review context to confirm false positives

**Issue: "Analysis too slow"**
- Analyze only changed files
- Use smaller directory scope
- Exclude generated/vendor code

**Issue: "Reports not generated"**
- Check `reports\` folder exists
- Verify write permissions
- Check disk space

---

## Summary

The Static Analysis system provides:

- ✅ **Comprehensive Checking** - Runtime, MISRA, CERT, quality
- ✅ **Multiple Entry Points** - GUI, batch, command line
- ✅ **Automatic Report Generation** - No Parasoft required
- ✅ **Polyspace-Compatible** - Same color coding system
- ✅ **Multiple Formats** - HTML, Excel, JSON
- ✅ **Fast & Lightweight** - No external dependencies
- ✅ **CI/CD Ready** - Easy integration

For AI-powered fix generation, see [AI_GUIDE.md](AI_GUIDE.md). For knowledge base features, see [KNOWLEDGE_BASE_GUIDE.md](KNOWLEDGE_BASE_GUIDE.md).

---

**Qorix India Pvt Ltd** - Quality Code Analysis Tools
