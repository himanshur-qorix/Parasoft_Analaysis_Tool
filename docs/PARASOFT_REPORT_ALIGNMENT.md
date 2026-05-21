# Parasoft Report Alignment Guide

## Overview
This document explains how the tool aligns with Parasoft reports and tracks justifications accurately.

## Issue 1: Justified Violations Not Counted (FIXED ✅)

### Problem
The CERT/MISRA report generator was showing "0 Justified" even when violations had justifications added.

### Root Cause
**Field name mismatch:**
- Knowledge Base stores: `justification_added` (boolean) and `justification_text` (string)
- Report generator was checking: `justification` or `justified`

### Solution
Updated `src/generate_cert_misra_report.py` line 93-98 to check for correct field:
```python
# Status - check for fix or justification
if v.get('fix_applied'):
    stats['fixed'] += 1
elif v.get('justification_added') or v.get('justification') or v.get('justified'):
    stats['justified'] += 1
else:
    stats['open'] += 1
```

Now the report will correctly count:
- **Fixed**: Violations with `fix_applied=True`
- **Justified**: Violations with `justification_added=True` or legacy fields
- **Open**: All other violations

---

## Issue 2: Parasoft Report Format Alignment

### Supported Parasoft Report Formats

#### 1. **Standard Parasoft HTML Report**
**Format:**
```
File Name (bold)
  Line | Violation Message | Rule ID
  Line | Violation Message | Rule ID
```

**Parser:** `parse_parasoft_html()` in `ParasoftAnalysisTool.py`

**Extracted Fields:**
- Violation (message)
- Violation ID (rule)
- File (source file)
- Line number

#### 2. **MISRA/CERT Static Analysis Report**
**Format:**
```
Standard | Rule ID | Severity | Category | File | Line | Message | Snippet
```

**Parser:** `parse_misra_cert_report()` in `ParasoftAnalysisTool.py`

**Extracted Fields:**
- Standard (MISRA/CERT)
- Rule ID
- Severity (HIGH/MEDIUM/LOW)
- Category
- File
- Line
- Message
- Code Snippet (optional)

### Knowledge Base Structure

After parsing, violations are stored in KB with:
```json
{
  "violation_id": "MISRAC2012-RULE_10_3-b-2",
  "violation_text": "The value of an expression shall not be assigned...",
  "category": "MISRA",
  "severity": "MEDIUM",
  "rule": "MISRAC2012-RULE_10_3",
  "occurrence_count": 5,
  "files_affected": [
    {
      "file": "Mka_Cfg.c",
      "line": 780,
      "timestamp": "2026-01-15T10:30:00"
    }
  ],
  "fix_applied": false,
  "justification_added": true,
  "justification_text": "Common deviation: Enum values are intentionally...",
  "history": [...]
}
```

### Accuracy Improvements

#### ✅ **Already Implemented:**
1. **Dual Format Support**: Handles both standard Parasoft and MISRA/CERT report formats
2. **File-level Grouping**: Groups violations by source file
3. **Occurrence Counting**: Tracks multiple occurrences of same violation
4. **History Tracking**: Records fix/justification timestamps
5. **Status Tracking**: Distinguishes Fixed vs Justified vs Open

#### 🎯 **Recommended Enhancements:**

**A. Parse Additional Parasoft Fields**
```python
# Extract if available in report:
- Code snippet (surrounding lines)
- Violation category (Advisory/Required/Mandatory)
- CWE/CVE mappings
- Technical debt score
```

**B. Match Parasoft Counting Logic**
```python
# Parasoft counts:
- Total violations (all occurrences)
- Unique violations (grouped by rule ID)
- Suppressed violations (with justifications)

# Tool should align:
total_violations = sum(v['occurrence_count'] for v in violations)
unique_violations = len(violations)  # Unique rule IDs
justified_violations = sum(v['occurrence_count'] for v in violations if v['justification_added'])
```

**C. Support Suppression Comments**
```python
# Parasoft format:
// parasoft-suppress MISRAC2012-RULE_10_3-b "Enum to integer conversion required"

# Tool should:
1. Detect existing suppressions in source
2. Count as "justified" in reports
3. Generate in Parasoft format
```

---

## How to Verify Alignment

### Step 1: Generate Parasoft Report
1. Run Parasoft C/C++test analysis
2. Export HTML report with all violations
3. Save to `parasoftReport/` folder

### Step 2: Run Tool Analysis
```batch
python src/ParasoftAnalysisTool.py Mka_violations.html Qorix_Deviations.xlsx Mka
```

### Step 3: Generate CERT/MISRA Report
```batch
python src/generate_cert_misra_report.py Mka
```

### Step 4: Compare Numbers
**Parasoft Report:**
- Total: 1792
- CERT: 761
- MISRA: 1031
- Justified: X

**Tool Report (After Fix):**
- Total: Should match
- CERT: Should match
- MISRA: Should match
- Justified: Should match (now correctly counted)

---

## Testing Checklist

- [ ] Create violation with justification
- [ ] Run CERT/MISRA report generator
- [ ] Verify "Justified" count > 0
- [ ] Compare with original Parasoft report numbers
- [ ] Check that Fixed vs Justified are separate categories
- [ ] Verify occurrence counts match

---

## Future Enhancements

### 1. XML Report Support
Parasoft also generates XML reports with richer data:
```xml
<Violation>
  <Rule>MISRAC2012-RULE_10_3-b</Rule>
  <Severity>2</Severity>
  <Message>...</Message>
  <File>path/to/file.c</File>
  <Line>780</Line>
  <Suppression>false</Suppression>
</Violation>
```

### 2. Baseline Comparison
- Track violations added/removed between runs
- Show trend analysis (improving/degrading)

### 3. Dashboard Integration
- Real-time violation tracking
- Module comparison charts
- Compliance score over time

---

## Related Files

- `src/generate_cert_misra_report.py` - Main report generator (FIXED)
- `src/ParasoftAnalysisTool.py` - HTML parser
- `src/KnowledgeDatabaseManager.py` - KB storage
- `src/StaticAnalysisReportGenerator.py` - HTML/Excel reports

---

## Contact

For questions about Parasoft report alignment:
- Developer: Himanshu R
- Organization: Qorix India Pvt Ltd

**Last Updated:** May 13, 2026
