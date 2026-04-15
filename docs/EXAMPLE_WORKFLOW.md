# Example Workflow - Parasoft AI Agent

This document shows a typical workflow for using the Parasoft AI Agent on a real project.

---

## Scenario: Analyzing Module "Mka" from a Parasoft Report

### Step 1: Initial Setup

```bash
# Navigate to your project
cd D:\Development\MyProject

# Copy the AI Agent tool to your project
# Assuming tool is in D:\Development\GitHub\Parasoft_Analaysis_Tool

# Place the Parasoft report in the tool directory
copy \\server\reports\Mka_report_dev1.html D:\Development\GitHub\Parasoft_Analaysis_Tool\report_dev1.html
```

### Step 2: Run Initial Analysis

```bash
cd D:\Development\GitHub\Parasoft_Analaysis_Tool

# Run the AI Agent
python src\run_agent.py report_dev1.html Mka
```

**Expected Output:**
```
============================================================
     PARASOFT AI AGENT - FULL ANALYSIS
Module: Mka
============================================================
2026-04-09 14:30:15 - INFO - Parasoft AI Agent initialized at: D:\Development\GitHub\Parasoft_Analaysis_Tool
2026-04-09 14:30:15 - INFO - ✓ Git repository detected
2026-04-09 14:30:15 - INFO - Git Info - Branch: main, Commit: a1b2c3d
2026-04-09 14:30:16 - INFO - Starting analysis for module: Mka
2026-04-09 14:30:16 - INFO - Found 157 total violations
2026-04-09 14:30:17 - INFO - Analysis complete: 42 new, 115 existing violations
2026-04-09 14:30:18 - INFO - Knowledge base saved: knowledge_base\Mka_KnowledgeDatabase.json
2026-04-09 14:30:19 - INFO - Generating fixes for 42 violations
2026-04-09 14:30:20 - INFO - Fixes saved to: fixes\Mka\Mka_fixes_20260409_143020.txt
2026-04-09 14:30:21 - INFO - Justifications saved to: fixes\Mka\Mka_justifications_20260409_143021.txt
============================================================
ANALYSIS COMPLETED SUCCESSFULLY
============================================================

✓ Analysis completed successfully!
  Total violations: 157
  New unique violations: 42
  Knowledge base: knowledge_base\Mka_KnowledgeDatabase.json
  Fixes generated: 38
  Justifications: 12
```

### Step 3: Review High Priority Issues

```bash
# Launch interactive query tool
python src\run_query.py knowledge_base --interactive
```

**Interactive Session:**
```
Available Commands:
  1. List modules
  2. Module summary
  3. Query by severity
  4. Query by category
  5. Query unfixed violations
  ...

Enter command number: 3

Enter severity (HIGH/MEDIUM/LOW): HIGH
Enter module name (or press Enter for all): Mka

Found 15 violations
  - CERT-C.STR31-C (Mka)
  - CERT-C.MEM35-C (Mka)
  - MISRA-C:2012-21.3 (Mka)
  ...
```

### Step 4: Export for Team Review

```bash
# Export to Excel for easy sharing
python src\run_query.py knowledge_base --excel Mka_violations_review.xlsx --module Mka
```

**Excel File Created:**
- **Sheet 1: All Violations** - Complete list with details
- **Sheet 2: High Severity** - Critical issues only
- **Sheet 3: Unfixed** - Issues needing attention
- **Sheet 4: Summary** - Statistics and overview

### Step 5: Review Generated Fixes

Open the fixes file: `fixes\Mka\Mka_fixes_20260409_143020.txt`

**Example Fix Entry:**
```
FIX #5
================================================================================
Violation ID: CERT-C.STR31-C
Severity: HIGH
Category: CERT

Description:
Do not pass a non-null-terminated character sequence to a library function 
that expects a string

Fix Type: buffer_safety
Priority: CRITICAL

Fix Description:
Ensure buffer boundaries are checked before access

Example:
// Before:
char buffer[10];
strcpy(buffer, user_input);  // Unsafe

// After:
char buffer[10];
strncpy(buffer, user_input, sizeof(buffer) - 1);
buffer[sizeof(buffer) - 1] = '\0';  // Ensure null termination

Files Affected:
  - mka_handler.c:145
  - mka_parser.c:89
  - mka_utils.c:234
```

### Step 6: Apply Fixes to Code

Navigate to the affected file and apply the fix:

**Before (mka_handler.c:145):**
```c
void process_data(const char* input) {
    char buffer[MAX_SIZE];
    strcpy(buffer, input);  // CERT-C.STR31-C violation
    // ... rest of code
}
```

**After:**
```c
void process_data(const char* input) {
    char buffer[MAX_SIZE];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';  // Fixed: CERT-C.STR31-C
    // ... rest of code
}
```

### Step 7: Add Justifications Where Needed

For violations that cannot be fixed, apply justifications from 
`fixes\Mka\Mka_justifications_20260409_143021.txt`

**Example (mka_legacy.c:567):**
```c
/* parasoft-begin-suppress MISRA-C:2012-21.3 "Legacy code compatibility - cannot be changed without breaking existing functionality" */
/* 
 * Justification: Legacy system interface requires use of malloc/free
 * Reviewed by: Himanshu R
 * Date: 2026-04-09
 * Ticket/Issue: PROJ-1234
 */
void* allocate_legacy_buffer(size_t size) {
    return malloc(size);  // Required for legacy API compatibility
}
/* parasoft-end-suppress MISRA-C:2012-21.3 */
```

### Step 8: Re-run Analysis After Fixes

```bash
# Get new Parasoft report after applying fixes
# Copy new report
copy \\server\reports\Mka_report_dev1_v2.html report_dev1.html

# Run analysis again
python src\run_agent.py report_dev1.html Mka
```

**Expected Output:**
```
Found 120 total violations  # Down from 157!
Analysis complete: 2 new, 118 existing violations
```

### Step 9: Track Progress Over Time

```bash
# Generate summary report to track progress
python src\run_query.py knowledge_base --summary Mka_progress_report.json
```

**Progress Report (Mka_progress_report.json):**
```json
{
  "generated_at": "2026-04-09T15:30:00",
  "total_modules": 1,
  "total_unique_violations": 44,
  "fix_status": {
    "fixed": 35,
    "unfixed": 9,
    "percentage_fixed": 79.55
  },
  "severity_breakdown": {
    "HIGH": 5,    // Down from 15!
    "MEDIUM": 28,
    "LOW": 11
  }
}
```

### Step 10: Final Excel Report for Management

```bash
# Create final report
python src\run_query.py knowledge_base --excel Mka_Final_Report_Week16.xlsx --module Mka
```

Share `Mka_Final_Report_Week16.xlsx` with the team showing:
- 79.55% of violations addressed
- All HIGH severity issues resolved
- Clear tracking of remaining issues

---

## Summary of Results

| Metric | Initial | After Fixes | Improvement |
|--------|---------|-------------|-------------|
| Total Violations | 157 | 120 | -23.6% |
| High Severity | 15 | 5 | -66.7% |
| Fixes Applied | 0 | 35 | +35 |
| Justifications | 0 | 12 | +12 |

---

## Best Practices Demonstrated

1. ✅ **Incremental Analysis** - Run after each major fix
2. ✅ **Priority Focus** - Address HIGH severity first
3. ✅ **Team Collaboration** - Use Excel exports for reviews
4. ✅ **Documentation** - Apply proper justifications
5. ✅ **Progress Tracking** - Monitor trends over time

---

## Next Iteration

For continuous improvement:

1. **Weekly Analysis**
   - Run analysis every week with latest Parasoft report
   - Track new violations introduced
   - Ensure old violations stay fixed

2. **Knowledge Base Maintenance**
   - Review knowledge database monthly
   - Update justifications as needed
   - Archive old analysis history

3. **Team Training**
   - Share common violation patterns
   - Develop team coding guidelines
   - Prevent violations before they occur

---

## Automation Opportunities

Consider automating the workflow:

```bash
# Create a weekly automation script
@echo off
REM Get latest report from server
copy \\server\reports\Mka_report_dev1.html report_dev1.html

REM Run analysis
python run_agent.py report_dev1.html Mka

REM Export results
python src\run_query.py knowledge_base --excel Weekly_Report_%DATE%.xlsx --module Mka

REM Email results to team
echo Analysis complete! Check Weekly_Report_%DATE%.xlsx
```

---

**This workflow demonstrates the power of the Parasoft AI Agent for systematic code quality improvement!**

