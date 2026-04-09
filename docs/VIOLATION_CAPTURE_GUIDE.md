# Direct Violation Capture Tool - User Guide

## Overview

The **Direct Violation Capture Tool** (`capture_violations.py`) is a standalone Python script that scans C/C++ source code directly to detect common coding violations **without requiring Parasoft**.

### 🎯 What It Does

- ✅ Scans C/C++ source files (.c, .cpp, .h, .hpp)
- ✅ Detects 15+ violation patterns
- ✅ Identifies buffer overflows, null pointer issues, memory leaks
- ✅ Checks MISRA and CERT coding standards
- ✅ Generates detailed reports (JSON, Text, CSV, **Excel**)
- ✅ Exports to knowledge base format
- ✅ Works independently - no Parasoft needed!

---

## 🚀 Quick Start

### Basic Usage

```bash
# Scan a directory
python capture_violations.py src/

# Scan with module name and export to knowledge base
python capture_violations.py src/ --module Mka --kb

# Generate text report
python capture_violations.py src/ --format text --output report.txt

# Generate Excel report
python capture_violations.py src/ --format excel --output violations.xlsx
```

---

## 📋 Command Line Options

```
python capture_violations.py <source_directory> [options]

Options:
  --module <name>      Module name (default: 'Unknown')
  --format <fmt>       Output format: json, text, csv, excel (default: json)
  --output <file>      Output file (default: violations_report.json)
  --kb                 Export in knowledge base format
  --extensions <exts>  File extensions (default: .c,.cpp,.h,.hpp)
```

---

## 🔍 Detected Violations

### Buffer Safety (HIGH Severity)
- **BUFFER-001**: Unsafe `gets()` usage
- **BUFFER-002**: Unsafe `strcpy()` usage
- **BUFFER-003**: Unsafe `strcat()` usage
- **BUFFER-004**: Unsafe `sprintf()` usage

### Memory Management (HIGH/MEDIUM)
- **MEM-001**: `malloc()` without NULL check
- **MEM-002**: `calloc()` without NULL check
- **MEM-003**: `realloc()` without NULL check
- **MEM-004**: Pointer not set to NULL after `free()`

### Null Pointer (HIGH)
- **NULL-001**: Pointer dereference without NULL check

### MISRA Violations (MEDIUM)
- **MISRA-001**: Use of `goto` statement
- **MISRA-002**: Use of `atoi()` - undefined behavior
- **MISRA-003**: Use of `atof()` - undefined behavior

### Format String (HIGH)
- **FORMAT-001**: Non-constant format string in `printf()`
- **FORMAT-002**: Non-constant format string in `fprintf()`

### Other Issues
- **CAST-001**: Integer cast to void pointer
- **ARRAY-001**: Array access without bounds check
- **COMP-001**: Assignment in if condition (= vs ==)
- **INIT-001**: Variable declared without initialization

---

## 📊 Output Formats

### 1. JSON Format (Default)

```json
{
  "scan_timestamp": "2026-04-09T15:30:00",
  "files_scanned": 25,
  "total_lines": 5423,
  "total_violations": 42,
  "violations": [
    {
      "Violation ID": "BUFFER-002",
      "Violation": "Unsafe use of strcpy() - buffer overflow risk",
      "File": "handler.c",
      "Line number": 145,
      "Severity": "HIGH",
      "Category": "CERT",
      "Suggestion": "Use strncpy() or strcpy_s() with size limit"
    }
  ],
  "summary": { ... }
}
```

### 2. Text Format

```
================================================================================
VIOLATION CAPTURE REPORT
================================================================================
Scan Date: 2026-04-09 15:30:00
Files Scanned: 25
Total Lines: 5423
Total Violations: 42
================================================================================

SEVERITY BREAKDOWN:
  HIGH:   15
  MEDIUM: 20
  LOW:    7

CATEGORY BREAKDOWN:
  CERT: 25
  MISRA: 17

DETAILED VIOLATIONS:
--------------------------------------------------------------------------------

VIOLATION #1
  ID:       BUFFER-002
  Severity: HIGH
  Category: CERT
  Message:  Unsafe use of strcpy() - buffer overflow risk
  File:     handler.c:145
  Code:     strcpy(buffer, user_input);
  Fix:      Use strncpy() or strcpy_s() with size limit
--------------------------------------------------------------------------------
...
```

### 3. CSV Format

```csv
Violation ID,Severity,Category,File,Line,Message,Code,Suggestion
BUFFER-002,HIGH,CERT,handler.c,145,"Unsafe use of strcpy()","strcpy(buffer, user_input);","Use strncpy()"
...
```

### 4. Excel Format

**Multi-sheet Excel workbook** with organized data:

```bash
python capture_violations.py src/ --format excel --output violations.xlsx
```

**Sheets included:**
- **All Violations**: Complete violation list
- **High Severity**: Only HIGH severity violations
- **Medium Severity**: Only MEDIUM severity violations  
- **CERT Violations**: CERT-specific violations
- **MISRA Violations**: MISRA-specific violations
- **Summary**: Scan statistics and metrics
- **Category Breakdown**: Violations by category

**Benefits:**
- Easy filtering and sorting
- Professional formatting
- Ready for review meetings
- Can be imported to other tools

### 5. Knowledge Base Format

With `--kb` flag, generates compatible format for the Parasoft AI Agent:

```json
{
  "module_name": "Mka",
  "total_unique_violations": 15,
  "violations": {
    "BUFFER-002": {
      "violation_id": "BUFFER-002",
      "occurrence_count": 5,
      "severity": "HIGH",
      "files_affected": [...]
    }
  }
}
```

---

## 🎯 Usage Examples

### Example 1: Scan Single Module

```bash
# Scan Mka module source code
python capture_violations.py D:/Project/Mka/src --module Mka --kb

# Output:
# - violations_report.json (detailed report)
# - Mka_ViolationCapture.json (knowledge base format)
```

### Example 2: Generate Text Report

```bash
python capture_violations.py src/ --format text --output violations.txt

# Review violations.txt for human-readable report
```

### Example 3: Generate Excel Report

```bash
# Generate comprehensive Excel report with multiple sheets
python capture_violations.py src/ --format excel --output violations.xlsx

# Output: violations.xlsx with:
#   - All Violations sheet
#   - High/Medium severity sheets
#   - Category-specific sheets
#   - Summary and breakdown sheets
```

### Example 4: Custom Extensions

```bash
# Scan only .cpp and .h files
python capture_violations.py src/ --extensions .cpp,.h
```

### Example 5: Multiple Modules

```bash
# Scan different modules
python capture_violations.py src/Mka --module Mka --kb
python capture_violations.py src/Engine --module Engine --kb
python capture_violations.py src/Utils --module Utils --kb

# Now you have:
# - Mka_ViolationCapture.json
# - Engine_ViolationCapture.json
# - Utils_ViolationCapture.json
```

---

## 🔗 Integration with Parasoft AI Agent

### Option 1: Direct Knowledge Base Import

The `--kb` export creates files compatible with the knowledge base format:

```bash
# Capture violations
python capture_violations.py src/Mka --module Mka --kb

# Move to knowledge base directory
move Mka_ViolationCapture.json knowledge_base/Mka_DirectScan.json

# Query using the query tool
python run_query.py knowledge_base --interactive
```

### Option 2: Merge with Parasoft Reports

1. **Run direct scan:**
   ```bash
   python capture_violations.py src/Mka --module Mka --kb
   ```

2. **Run Parasoft analysis:**
   ```bash
   python run_agent.py report_dev1.html Mka
   ```

3. **Query both:**
   ```bash
   python run_query.py knowledge_base --module Mka --summary
   ```

---

## 📈 Sample Workflow

```
┌─────────────────────────────────────────────────────────┐
│  Complete Code Quality Workflow                         │
└─────────────────────────────────────────────────────────┘

Step 1: Direct Scan (Fast, Basic Checks)
  │
  ├─► python capture_violations.py src/ --module Mka --kb
  └─► Output: Quick violation report

Step 2: Parasoft Analysis (Comprehensive)
  │
  ├─► Run Parasoft C/C++test → report_dev1.html
  └─► python run_agent.py report_dev1.html Mka

Step 3: Review All Violations
  │
  └─► python run_query.py knowledge_base --interactive

Step 4: Apply Fixes
  │
  ├─► Review fixes/ directory
  └─► Apply code changes
```

---

## ⚙️ Customization

### Add Custom Violation Patterns

Edit `capture_violations.py` and add to `self.patterns` dictionary:

```python
'CUSTOM-001': {
    'pattern': r'your_regex_pattern',
    'message': 'Your violation message',
    'severity': 'HIGH',  # HIGH, MEDIUM, LOW
    'category': 'CERT',  # CERT, MISRA, CWE, OTHER
    'suggestion': 'How to fix it'
}
```

### Example: Detect Magic Numbers

```python
'CUSTOM-MAGIC': {
    'pattern': r'\bif\s*\(\s*\w+\s*[=!<>]+\s*\d{2,}\s*\)',
    'message': 'Magic number in comparison',
    'severity': 'LOW',
    'category': 'MISRA',
    'suggestion': 'Use named constant instead of magic number'
}
```

---

## 🆚 Comparison: Direct Scan vs Parasoft

| Feature | Direct Scan (This Tool) | Parasoft C/C++test |
|---------|------------------------|-------------------|
| **Speed** | ⚡ Very Fast | 🐢 Slower (comprehensive) |
| **Coverage** | ✓ 15+ patterns | ✓✓✓ 1000+ rules |
| **Cost** | 🆓 Free | 💰 Commercial license |
| **Setup** | ✓ Just Python | Complex setup |
| **Accuracy** | Basic regex-based | Deep analysis |
| **False Positives** | Higher | Lower |
| **Use Case** | Quick checks, CI/CD | Full certification |

**Recommendation:** Use both!
- Direct scan for quick feedback during development
- Parasoft for final, comprehensive analysis

---

## 🚨 Limitations

### What This Tool Does NOT Do:

❌ Deep flow analysis (e.g., tracking pointer through functions)
❌ Inter-procedural analysis
❌ Complex data flow analysis
❌ Path-sensitive analysis
❌ Symbolic execution
❌ Replace professional static analysis tools

### Best Used For:

✅ Quick violation checks during development
✅ CI/CD pipeline integration
✅ Pre-commit hooks
✅ Learning about common violations
✅ Complementing Parasoft analysis

---

## 🔧 Troubleshooting

**Issue:** No violations found
```bash
# Solution: Check file extensions
python capture_violations.py src/ --extensions .c,.cpp,.h,.hpp
```

**Issue:** Too many false positives
```bash
# Solution: Review and customize patterns in the script
# Edit self.patterns dictionary to refine regex
```

**Issue:** Script runs slowly
```bash
# Solution: Scan specific subdirectories
python capture_violations.py src/specific_module/
```

---

## 📚 Additional Resources

- **Main Tool**: `run_agent.py` - For Parasoft HTML report analysis
- **Query Tool**: `run_query.py` - For querying knowledge base
- **Documentation**: `docs/` folder - Complete documentation

---

## 💡 Tips & Best Practices

1. **Run Early, Run Often**
   ```bash
   # Add to your development workflow
   python capture_violations.py src/ --module MyModule
   ```

2. **Use in Git Hooks**
   ```bash
   #!/bin/bash
   # .git/hooks/pre-commit
   python capture_violations.py $(git diff --name-only HEAD | grep -E '\.(c|cpp|h)$')
   ```

3. **CI/CD Integration**
   ```yaml
   # .github/workflows/quality.yml
   - name: Scan Violations
     run: python capture_violations.py src/ --format csv --output violations.csv
   ```

4. **Track Progress**
   ```bash
   # Weekly scans
   python capture_violations.py src/ --output "scan_$(date +%Y%m%d).json"
   ```

---

**Ready to scan your code!** 🚀

```bash
python capture_violations.py src/ --module YourModule --kb
```
