# Polyspace Integration Guide

**Version:** 3.0.0  
**Developer:** Himanshu R  
**Organization:** Qorix India Pvt Ltd

## 🎯 Overview

The Parasoft Analysis Tool now supports **Polyspace Code Prover / Bug Finder** integration, allowing you to:

- Parse Polyspace HTML, Excel, and TSV reports
- Generate unified knowledge bases from Polyspace findings
- Create AI-powered fix suggestions for Polyspace issues
- Cross-reference Polyspace and Parasoft violations
- Track justifications and suppressions across both tools

## 📊 Supported Formats

### Input Formats
- **HTML Reports**: Polyspace HTML export (default format)
- **Excel Reports**: `.xlsx` or `.xls` exports from Polyspace Access
- **TSV/CSV**: Tab-separated or comma-separated exports

### Polyspace Color Codes
| Color  | Meaning | Severity Mapping |
|--------|---------|------------------|
| 🔴 Red | Proven runtime error | CRITICAL |
| 🟠 Orange | Potential error (analysis inconclusive) | HIGH |
| 🟢 Green | Proven safe (no error possible) | LOW |
| ⚪ Gray | Unreachable code or deactivated check | MEDIUM |

### Supported Checks
- **Runtime Errors**: Overflow, Division by Zero, Null pointer, etc.
- **MISRA C/C++**: All MISRA compliance checks
- **CERT C/C++**: CERT secure coding standards
- **CWE**: Common Weakness Enumeration
- **AUTOSAR C++**: Automotive industry standards
- **Data Flow**: Uninitialized variables, dead code
- **Concurrency**: Race conditions, deadlocks

## 🚀 Quick Start

### 1. Export Polyspace Report

From Polyspace:
```
Results → Export → HTML (or Excel/TSV)
```

### 2. Run Polyspace Analysis

**Method A: Using Batch File (Recommended)**
```batch
cd scripts
Run_Polyspace.bat
```

Follow the prompts:
1. Enter path to Polyspace report file
2. Enter module name (e.g., Mka, Eep, Can)
3. Optionally provide source code path
4. Choose whether to generate fixes

**Method B: Direct Python Command**
```batch
cd src
python run_polyspace_agent.py path\to\report.html ModuleName
```

With source code context:
```batch
python run_polyspace_agent.py report.html Mka --source-code D:\Code\Mka
```

Skip automatic fixes:
```batch
python run_polyspace_agent.py report.html Mka --no-fixes
```

### 3. Review Results

Generated files:
- `reports\<Module>_violations_report.xlsx` - Excel report with findings
- `knowledge_base\<Module>_KnowledgeDatabase.json` - Knowledge base
- `data\<Module>_violations.json` - Raw violations data

Excel sheets:
- **All Violations** - Complete list
- **Red - Proven Errors** - Critical issues (🔴)
- **Orange - Potential Errors** - High-priority issues (🟠)
- **Summary** - Statistics and category breakdown

### 4. Generate Code Fixes (Optional)

```batch
cd scripts
Generate_Code_Fixes.bat
```

Select module and AI mode:
- **AI Only** - Use AI for all suggestions
- **Hybrid** - AI + Parasoft DB + Rule-based (recommended)
- **Rules Only** - Pattern-based only

## 🔧 Advanced Usage

### Parse Multiple Formats

```python
from PolyspaceParser import PolyspaceParser

parser = PolyspaceParser()

# Parse HTML
violations_html = parser.parse_report(Path('report.html'))

# Parse Excel
violations_xlsx = parser.parse_report(Path('results.xlsx'))

# Parse TSV
violations_tsv = parser.parse_report(Path('export.tsv'))

# Get summary
summary = parser.get_summary()
print(f"Red findings: {summary['red_findings']}")
```

### Filter Findings

```python
# Get only red findings (proven errors)
red_errors = parser.filter_by_color('red')

# Get critical severity
critical = parser.filter_by_severity('CRITICAL')

# Get MISRA violations
misra_violations = parser.filter_by_category('MISRA C/C++')
```

### Combine with Parasoft Analysis

**Workflow:**
1. Run Parasoft analysis → Generate KB
2. Run Polyspace analysis → Merge into same KB
3. Generate unified report with both tools' findings
4. Cross-reference duplicates (same issue found by both)

```batch
REM Step 1: Parasoft
scripts\Run.bat

REM Step 2: Polyspace (same module)
scripts\Run_Polyspace.bat

REM Both findings now in same knowledge base!
```

## 📋 Polyspace Check ID Format

Polyspace uses specific check IDs:

| Format | Example | Description |
|--------|---------|-------------|
| RTE:* | RTE:OVFL | Runtime Error: Integer Overflow |
| MISRA* | MISRA-C3:10.3 | MISRA C 2012 Rule 10.3 |
| CERT* | CERT-C:INT31-C | CERT C Rule INT31-C |
| CWE* | CWE-190 | DEPRECATED CWE-190 (Integer Overflow) |

These are automatically converted to standardized violation IDs:
```
POLYSPACE-RTE-OVFL
POLYSPACE-MISRA-C3-10_3
POLYSPACE-CERT-C-INT31-C
```

## 🎨 Excel Report Structure

### Sheet: "All Violations"
| Column | Description |
|--------|-------------|
| Violation ID | Unique identifier (POLYSPACE-*) |
| Check ID | Original Polyspace check |
| Color | Red/Orange/Green/Gray |
| Severity | CRITICAL/HIGH/MEDIUM/LOW |
| Category | Runtime Error, MISRA, CERT, etc. |
| Description | Full violation description |
| File | Source file path |
| Line | Line number |
| Comment | User annotations from Polyspace |

### Sheet: "Summary"
- Total findings count
- Breakdown by color
- Breakdown by category
- Breakdown by severity

## 🤖 AI-Powered Features

### Justification Suggestions

The AI analyzes cross-module patterns:
```
If multiple modules have justified the same Polyspace finding
→ AI suggests adding to common deviations Excel
→ New sheet added: "AI Justification Suggestions"
```

### Context-Aware Fixes

With source code path provided:
```batch
--source-code D:\Code\MyModule
```

AI extracts ±5 lines of code context:
- Better understanding of violation
- More accurate fix suggestions
- Specific code examples

### Fix Types for Polyspace

| Polyspace Color | Fix Strategy |
|----------------|--------------|
| 🔴 Red | AI generates fix to eliminate proven error |
| 🟠 Orange | AI suggests validation/bounds checks |
| 🟢 Green | No fix needed (proven safe) |
| ⚪ Gray | Suggest removing dead code or adding comments |

## 📊 Integration with Existing Workflow

### Knowledge Base Structure

Polyspace violations are stored alongside Parasoft:

```json
{
  "violation_id": "POLYSPACE-RTE-OVFL-1",
  "tool": "Polyspace",
  "check_id": "RTE:OVFL",
  "color": "red",
  "severity": "CRITICAL",
  "category": "Runtime Error",
  "violation_text": "RTE:OVFL: Proven runtime error. Integer overflow detected",
  "files_affected": ["src/module.c:123"],
  "fix_applied": false,
  "justification_added": false
}
```

### Common Deviations Support

If Polyspace finding should be justified globally:
1. AI detects cross-module pattern
2. Adds suggestion to "AI Justification Suggestions" sheet
3. User reviews and adds to `Qorix_CP_Common_Deviations.xlsx`

## 🛠️ Troubleshooting

### Issue: "No violations found in report"

**Causes:**
- Report format not recognized
- Empty report file
- Incorrect file path

**Solutions:**
1. Verify file exists and is not empty
2. Try different export format (HTML recommended)
3. Check log file: `polyspace_analysis.log`

### Issue: "Failed to parse Excel report"

**Causes:**
- Missing pandas/openpyxl libraries
- Corrupted Excel file
- Non-standard sheet structure

**Solutions:**
```batch
pip install pandas openpyxl
```

Try exporting as HTML instead:
```batch
python run_polyspace_agent.py report.html ModuleName
```

### Issue: "Source code context not found"

**Causes:**
- Incorrect source path
- File renamed/moved since Polyspace analysis
- Different directory structure

**Solutions:**
- Verify source code path exists
- Use absolute paths
- Skip source code context (still generates fixes without it)

## 📝 Example Workflow

### Complete Analysis Pipeline

```batch
REM 1. Run Polyspace on your code (in Polyspace tool)
REM 2. Export report to HTML

REM 3. Parse Polyspace report
cd D:\Parasoft_Analysis_Tool\scripts
Run_Polyspace.bat

REM Inputs:
REM   Report: D:\PolyspaceResults\Mka_results.html
REM   Module: Mka
REM   Source: D:\Code\Mka\src

REM 4. Review Excel report
start ..\reports\Mka_violations_report.xlsx

REM 5. Generate fixes for critical issues
Generate_Code_Fixes.bat
REM Select: Mka
REM Mode: Hybrid

REM 6. Review AI justification suggestions
REM Check "AI Justification Suggestions" sheet in Excel

REM 7. Apply suppressions (if needed)
Apply_Suppressions.bat
```

## 🎯 Best Practices

1. **Export Format**: Use HTML for best compatibility
2. **Module Names**: Keep consistent with Parasoft module names
3. **Source Code**: Always provide source path for better fixes
4. **Red Findings First**: Focus on red (proven errors) before orange
5. **Cross-Tool Analysis**: Run both Parasoft and Polyspace on same code
6. **AI Mode**: Use "Hybrid" mode for balanced results
7. **Regular Updates**: Re-run analysis after code changes

## 🔄 Version History

### v3.0.0 (Current)
- ✨ Added Polyspace Integration
- 📊 Multi-format report parser (HTML/Excel/TSV)
- 🤖 AI-powered fixes for Polyspace findings
- 📈 Cross-module justification analysis
- 🎨 Enhanced Excel reports with color-coded findings

### v2.0.0
- Added Parasoft Rules Database integration
- AI-only, Hybrid, and Rules-only modes
- Interactive fix generation

### v1.0.0
- Initial Parasoft support
- Basic knowledge base management

## 📞 Support

**Issues or questions?**
- Check log file: `polyspace_analysis.log`
- Review documentation: `docs/`
- Contact: Himanshu R, Qorix India Pvt Ltd

---

**Qorix India Pvt Ltd** - Quality Code Analysis Tools
