# Excel Report Generation Guide

## Overview

The **Direct Violation Capture Tool** now supports **Excel format** for professional, multi-sheet violation reports perfect for code reviews, compliance audits, and team meetings.

---

## 🎯 Quick Start

### Generate Excel Report

```bash
# Basic Excel report
python capture_violations.py src/ --format excel

# Custom output filename
python capture_violations.py src/ --format excel --output my_violations.xlsx

# With module name and knowledge base export
python capture_violations.py src/ --module MyModule --format excel --kb
```

**Output:** `violations_report.xlsx` (or custom filename)

---

## 📊 Excel Workbook Structure

### Sheet 1: All Violations
**Complete list of all detected violations**

| Violation ID | Severity | Category | Violation | File | Line | Code | Suggestion | Full Path |
|--------------|----------|----------|-----------|------|------|------|------------|-----------|
| BUFFER-002 | HIGH | CERT | Unsafe use of strcpy() | handler.c | 145 | strcpy(buffer, input); | Use strncpy() with size limit | D:\src\handler.c |
| MEM-001 | HIGH | CERT | malloc() not checked | memory.c | 89 | ptr = malloc(100); | Check if malloc() returned NULL | D:\src\memory.c |

### Sheet 2: High Severity
**Only HIGH severity violations** - critical issues requiring immediate attention

- Buffer overflows (BUFFER-001 to 004)
- Memory management without NULL checks (MEM-001 to 003)
- Null pointer dereferences (NULL-001)
- Format string vulnerabilities (FORMAT-001, 002)

### Sheet 3: Medium Severity
**MEDIUM severity violations** - important issues for code quality

- Pointer not set to NULL after free() (MEM-004)
- MISRA violations (goto, atoi, atof)
- Array access without bounds check
- Assignment in if condition
- Uninitialized variables

### Sheet 4-N: Category Sheets
**Separate sheets for each violation category**

- **CERT Violations**: All CERT-related issues
- **MISRA Violations**: All MISRA-related issues

### Summary Sheet
**Scan statistics and metrics**

| Metric | Value |
|--------|-------|
| Files Scanned | 45 |
| Lines Scanned | 12,543 |
| Total Violations | 87 |
| High Severity | 23 |
| Medium Severity | 64 |
| Low Severity | 0 |

### Category Breakdown Sheet
**Violations grouped by category**

| Category | Count |
|----------|-------|
| CERT | 65 |
| MISRA | 22 |

---

## 💡 Use Cases

### 1. Code Review Meetings
```bash
python capture_violations.py src/ --format excel --output code_review_2026-04-09.xlsx
```
- Open Excel file during meeting
- Filter by severity or category
- Assign violations to team members
- Add comments directly in Excel

### 2. Compliance Audits
```bash
python capture_violations.py src/ --module SafetyCritical --format excel
```
- Generate audit-ready reports
- Show MISRA/CERT compliance
- Professional formatting
- Easy to share with auditors

### 3. Trend Analysis
```bash
# Weekly scans
python capture_violations.py src/ --format excel --output violations_week1.xlsx
python capture_violations.py src/ --format excel --output violations_week2.xlsx
```
- Compare violation counts over time
- Track improvement progress
- Identify problematic modules

### 4. Executive Summary
- Use Summary sheet for management reporting
- Category breakdown shows focus areas
- Severity breakdown shows risk level

---

## 🔧 Advanced Usage

### Combine Excel with Knowledge Base Export

```bash
python capture_violations.py src/ --module Mka --format excel --kb

# Outputs:
# 1. violations_report.xlsx  (Excel report for humans)
# 2. Mka_ViolationCapture.json  (Knowledge base for AI agent)
```

**Workflow:**
1. Review violations in Excel
2. Import knowledge base to Parasoft AI Agent
3. Generate fixes and justifications
4. Track resolution in Excel

### Filter Specific File Types

```bash
# Only C++ files
python capture_violations.py src/ --extensions .cpp,.hpp --format excel

# Only header files
python capture_violations.py src/ --extensions .h --format excel
```

---

## 📈 Excel Features

### Built-in Filtering
- Click column headers to filter
- Multi-select severities (HIGH + MEDIUM)
- Filter by file or category

### Sorting
- Sort by severity (HIGH → MEDIUM → LOW)
- Sort by file name
- Sort by line number
- Sort by violation ID

### Data Analysis
- Use pivot tables on "All Violations" sheet
- Create charts from category breakdown
- Calculate fix completion percentage

### Export and Share
- Email as attachment
- Share via network drive
- Import to Jira/Azure DevOps
- Convert to PDF for archival

---

## 🎨 Customization Tips

### Add Your Own Columns
1. Open Excel file
2. Add columns (e.g., "Assigned To", "Fix Date", "Status")
3. Fill in your data
4. Use for project tracking

### Create Charts
1. Go to "Category Breakdown" or "Summary" sheet
2. Select data
3. Insert → Chart
4. Choose bar chart or pie chart

### Conditional Formatting
1. Select severity column
2. Home → Conditional Formatting
3. Highlight HIGH in red, MEDIUM in yellow

---

## 📝 Comparison: Excel vs Other Formats

| Feature | JSON | Text | CSV | **Excel** |
|---------|------|------|-----|-----------|
| Human Readable | ⚠️ | ✅ | ⚠️ | ✅✅ |
| Machine Readable | ✅ | ❌ | ✅ | ✅ |
| Filtering/Sorting | ❌ | ❌ | ⚠️ | ✅✅ |
| Multiple Sheets | ❌ | ❌ | ❌ | ✅✅ |
| Professional Look | ❌ | ⚠️ | ❌ | ✅✅ |
| Easy Sharing | ⚠️ | ✅ | ✅ | ✅✅ |
| Charts/Graphs | ❌ | ❌ | ❌ | ✅✅ |
| Comments/Notes | ❌ | ❌ | ❌ | ✅✅ |

**Recommendation:** Use **Excel** for reviews and presentations, **JSON** for automation, **Knowledge Base** for AI agent integration.

---

## 🚀 Quick Reference

```bash
# Default (auto-detects extension)
python capture_violations.py src/ --format excel

# Custom filename
python capture_violations.py src/ --format excel --output violations.xlsx

# With module tracking
python capture_violations.py src/ --module MyModule --format excel

# Full workflow (Excel + Knowledge Base)
python capture_violations.py src/ --module MyModule --format excel --kb

# Weekly report with date
python capture_violations.py src/ --format excel --output violations_$(Get-Date -Format "yyyy-MM-dd").xlsx
```

---

## 🔗 Integration with Parasoft AI Agent

**Step 1:** Generate Excel report for review
```bash
python capture_violations.py src/ --module Mka --format excel --kb
```

**Step 2:** Review violations in Excel
- Open `violations_report.xlsx`
- Filter HIGH severity violations
- Identify priorities

**Step 3:** Process with AI Agent
```bash
python src\run_agent.py
# Uses Mka_ViolationCapture.json knowledge base
```

**Step 4:** Track progress
- Update Excel with fix status
- Add columns: "Fix Applied", "Justification Added"
- Compare with next scan

---

## 📌 Tips and Best Practices

### Do's ✅
- Generate Excel reports for code reviews
- Use Summary sheet for management updates
- Filter by severity to prioritize work
- Add custom columns for tracking
- Save weekly reports for trend analysis

### Don'ts ❌
- Don't edit violation data (keep original for reference)
- Don't manually add violations (re-scan instead)
- Don't delete sheets (hide if not needed)

### Performance
- Excel generation is fast (< 1 second for 100 violations)
- File size: ~10-20 KB per 100 violations
- Opens in Excel, LibreOffice, Google Sheets

---

## 🆘 Troubleshooting

### Error: "No module named 'openpyxl'"
```bash
pip install openpyxl pandas
```

### Excel file won't open
- Check file extension is .xlsx
- Ensure pandas version >= 2.0.0
- Try opening in Google Sheets

### Missing sheets
- Some sheets only appear if violations exist
- Empty categories won't generate sheets
- Check "All Violations" sheet first

---

## 📚 Related Documentation

- [VIOLATION_CAPTURE_GUIDE.md](VIOLATION_CAPTURE_GUIDE.md) - Complete tool documentation
- [QUICKSTART.md](QUICKSTART.md) - General getting started guide
- [EXAMPLE_WORKFLOW.md](EXAMPLE_WORKFLOW.md) - Full workflow examples

---

**Last Updated:** April 9, 2026  
**Version:** 1.0.0

