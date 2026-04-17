# Static Code Analysis Guide

**Version:** 3.0.0  
**Developer:** Himanshu R  
**Organization:** Qorix India Pvt Ltd

## 🎯 Overview

Built-in **Static Code Analyzer** that directly analyzes C/C++ source code without requiring external tools. Detects:

- **Runtime Errors**: Null pointers, buffer overflows, division by zero
- **MISRA C Violations**: Compliance with MISRA C 2012 rules
- **CERT C Violations**: Secure coding standards
- **Code Quality Issues**: Dead code, magic numbers, complex expressions

## 🚀 Quick Start

### Using Batch File (Recommended)
```batch
cd scripts
Run_Polyspace.bat
```

Follow the prompts:
1. Enter source code directory (e.g., `D:\Code\Mka\src`)
2. Enter module name (e.g., `Mka`)
3. Wait for analysis to complete

**Output**: Color-coded HTML and Excel reports in `reports\` folder

**Note**: Static analysis workflow **does not include fix generation**. It only analyzes code and generates reports. For AI-powered fix suggestions, use the Parasoft workflow (`Run.bat`).

### Direct Python Command
```batch
cd src
python run_static_analyzer.py D:\Code\Mka Mka
```

## 📊 Checks Performed

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

## 📞 Support

**Need help?**
- Check log file: `static_analysis.log`
- Review sample violations in Excel
- Contact: Himanshu R, Qorix India Pvt Ltd

---

**Qorix India Pvt Ltd** - Quality Code Analysis Tools
