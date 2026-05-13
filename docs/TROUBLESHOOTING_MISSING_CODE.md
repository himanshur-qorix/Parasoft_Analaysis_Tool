# Troubleshooting Guide: Missing Code Context

## Quick Diagnosis

Run this to check your knowledge base format:

```batch
Check_KB.bat Mka
```

This will show you:
- ✅ How violations are stored (dict vs string format)
- ✅ How many violations have code snippets
- ✅ How many need enrichment
- ✅ Sample violations with their data format

## The Issue You're Seeing

```
VIOLATION ID: MISRAC2012-RULE_10_3-b-2
FILE: UNKNOWN
LINE: UNKNOWN
CODE SNIPPET: No code snippet available
```

**Root Causes:**

### 1. Knowledge Base Format Mismatch
The KB stores `files_affected` as:
```json
{
  "files_affected": [
    {
      "file": "Mka.c",
      "line": 145,
      "timestamp": "2026-05-11T..."
    }
  ]
}
```

But the old code expected strings like:
```json
{
  "files_affected": ["Mka.c:145"]
}
```

**✅ FIXED:** Updated ViolationEnricher, InteractiveCodeFixer, and CodeFixGenerator to handle both formats.

### 2. Source Code Path Not Provided
When violations are first captured from the Parasoft HTML report, they only get filenames like "Mka.c", not full paths. The enricher needs to know where your source code is to find these files.

**Solution:** Always provide source code path when running fixes:

```batch
Apply_Fixes.bat Mka "D:\Your\Source\Path"
```

### 3. Parasoft Report Only Has Filenames
The Parasoft HTML report format looks like:
```html
<td>Mka.c</td>
<td>145</td>
<td>Violation description...</td>
```

It doesn't include full paths or code snippets. That's why enrichment is needed!

## How to Fix It

### Step 1: Check Your Knowledge Base

```batch
Check_KB.bat Mka
```

**Expected output:**
```
================================================================================
  KNOWLEDGE BASE DIAGNOSTICS
================================================================================

📊 Total Violations: 246
✅ Enriched: 0 (0.0%)
⚠️  Needs Enrichment: 246

📋 Sample Violations (first 5):

[1] MISRAC2012-RULE_10_3-b-2
    Files Affected: 1
    Format: dict
    File: Mka.c
    Line: 145
    Has Code Snippet: ❌
    Has File Path: ❌
    Enriched: ❌

💡 RECOMMENDATION:
   246 violations need enrichment!

   Run:
   Enrich_Violations.bat Mka "SOURCE_CODE_PATH"
```

### Step 2: Enrich Violations

**Option A: Manual Enrichment**
```batch
Enrich_Violations.bat Mka "D:\Task&Data\CY26Q2_SP2\ParsoftFixes\Parasoft_Analaysis_Tool\Input\Mka_Config_1_original"
```

**Option B: Automatic (Recommended)**
```batch
# Just run Apply_Fixes with source path - enrichment happens automatically!
Apply_Fixes.bat Mka "D:\Task&Data\CY26Q2_SP2\ParsoftFixes\Parasoft_Analaysis_Tool\Input\Mka_Config_1_original"
```

### Step 3: Verify Enrichment

```batch
Check_KB.bat Mka
```

**After enrichment, you should see:**
```
📊 Total Violations: 246
✅ Enriched: 234 (95.1%)
⚠️  Needs Enrichment: 12

[1] MISRAC2012-RULE_10_3-b-2
    Files Affected: 1
    Format: dict
    File: Mka.c
    Line: 145
    Has Code Snippet: ✅
    Has File Path: ✅
    Enriched: ✅
```

### Step 4: Apply AI Fixes

Now the AI can see your actual code:

```batch
Apply_Fixes.bat Mka "D:\Task&Data\...\Input\Mka_Config_1_original"
```

**What you'll see now:**
```
================================================================================
  Violation: MISRAC2012-RULE_10_3-b-2
  Category: MISRA
  Severity: MEDIUM
================================================================================

Description: The expression of the 'essentially Enum' type is assigned to an 
object with the 'essentially unsigned' type

📄 Current Code (Mka.c:145):
--------------------------------------------------------------------------------
142: void ProcessMessage(uint8 msgType) {
143:     MkaStatusType status;
144:     
145:     status = MKA_GetEnumValue(msgType);
146:     
147:     if (status == MKA_OK) {
148:         // Process message
149:     }
150: }
--------------------------------------------------------------------------------

🤖 Generating AI-powered fix...

✨ Proposed Fix:
--------------------------------------------------------------------------------
Type: cast
Description: Add explicit type cast from enum to unsigned type to prevent 
implicit conversion and ensure type safety according to MISRA C standards.
--------------------------------------------------------------------------------

📊 Changes Preview:
--------------------------------------------------------------------------------
- 145:     status = MKA_GetEnumValue(msgType);
+ 145:     status = MKA_GetEnumValue((MkaStatusType)msgType);
--------------------------------------------------------------------------------

  Apply this fix? [y=yes, n=no, e=edit, s=skip all]: 
```

## Common Issues

### Issue: "Source file not found"

```
[INFO] Enrichment complete: 0 enriched, 246 skipped
```

**Cause:** Source code path is wrong or doesn't contain the files mentioned in the report.

**Solution:**
1. Check what files are in your KB: `Check_KB.bat Mka`
2. Find where those files actually are on disk
3. Use the correct path: `Enrich_Violations.bat Mka "CORRECT_PATH"`

### Issue: "Files_affected is empty"

```
[DEBUG] No files_affected for CERT_C-INT31-i-2
```

**Cause:** Violation was captured without file information.

**Solution:** This shouldn't happen with normal Parasoft reports. Check how the KB was created. You may need to re-run `Run.bat` with a proper Parasoft report.

### Issue: "Multiple files found"

```
[DEBUG] Multiple files found for Mka.c, using: D:\Source\Mka\src\Mka.c
```

**This is normal!** If you have multiple files with the same name (e.g., in different folders), the enricher will use the first match. Usually this is fine.

## Full Workflow

### For the first time:

```batch
# 1. Check current state
Check_KB.bat Mka

# 2. Enrich if needed (or let Apply_Fixes do it automatically)
Enrich_Violations.bat Mka "D:\Your\Source\Path"

# 3. Verify
Check_KB.bat Mka

# 4. Apply fixes
Apply_Fixes.bat Mka "D:\Your\Source\Path"
```

### For subsequent runs:

```batch
# Just run Apply_Fixes - everything happens automatically!
Apply_Fixes.bat Mka "D:\Your\Source\Path"
```

## Expected File Paths

Based on your project structure, use:

```batch
# If analyzing code from Input folder
Apply_Fixes.bat Mka "D:\Task&Data\CY26Q2_SP2\ParsoftFixes\Parasoft_Analaysis_Tool\Input\Mka_Config_1_original"

# Or shorter if running from project root
Apply_Fixes.bat Mka "Input\Mka_Config_1_original"
```

## Debug Mode

For more detailed logging, edit the enricher call in `Apply_Fixes.bat` to add `--verbose`:

```batch
python src\ViolationEnricher.py --kb "..." --source "..." --verbose
```

Or run Python directly:

```batch
python
>>> from src.ViolationEnricher import ViolationEnricher
>>> from pathlib import Path
>>> enricher = ViolationEnricher(Path("Input/Mka_Config_1_original"))
>>> # Check file cache
>>> print(f"Found {len(enricher.file_cache)} unique filenames")
>>> print("Sample files:", list(enricher.file_cache.keys())[:10])
```

## What Gets Updated

When enrichment succeeds, each violation in the KB gets:

```json
{
  "violation_id": "MISRAC2012-RULE_10_3-b-2",
  "file_path": "D:\\...\\Mka.c",           // ← Added
  "file": "Mka.c",                          // ← Added
  "line": 145,                              // ← Added
  "code_snippet": "status = MKA_...",      // ← Added
  "code_context": "void Process...",        // ← Added
  "code_context_lines": {                   // ← Added
    "start": 140,
    "end": 150,
    "violation": 145
  },
  "enriched": true,                         // ← Added
  "files_affected": [...]                   // Already exists
}
```

## Summary

✅ **What was fixed:**
- ViolationEnricher now handles dict format properly
- InteractiveCodeFixer handles both dict and string formats
- CodeFixGenerator handles both dict and string formats
- Apply_Fixes.bat auto-enriches if needed
- Check_KB.bat helps diagnose issues

✅ **What you need to do:**
1. Run `Check_KB.bat Mka` to see current state
2. Run `Apply_Fixes.bat Mka "SOURCE_PATH"` with correct source path
3. Enrichment happens automatically if needed
4. AI now sees your actual code!

---

**Version:** 4.1.0  
**Developer:** Himanshu R  
**Organization:** Qorix India Pvt Ltd
