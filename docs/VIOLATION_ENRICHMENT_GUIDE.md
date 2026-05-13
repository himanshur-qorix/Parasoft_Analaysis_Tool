# Fixing Missing Code Context in Violations

## The Problem You Encountered

When trying to generate AI-powered fixes, you saw violations with incomplete data:

```
VIOLATION ID: CERT_C-INT31-i-2
RULE: UNKNOWN
SEVERITY: LOW
FILE: UNKNOWN
LINE: UNKNOWN
DESCRIPTION: The expression of the 'essentially Enum' type is assigned to an object with the 'essentially unsigned' type
CODE SNIPPET: No code snippet available
```

This happens because **Parasoft HTML reports only contain filenames and line numbers**, not the actual source code. The AI needs to see the actual code to generate proper fixes!

## The Solution: Violation Enrichment

We've added a **Violation Enricher** that:
1. ✅ Reads your knowledge base violations
2. ✅ Finds the actual source files in your codebase
3. ✅ Reads the code at the violation lines
4. ✅ Adds code snippets and context
5. ✅ Saves enriched data back to knowledge base

## How to Use It

### Option 1: Automatic (Recommended)

`Apply_Fixes.bat` now **automatically enriches violations** if needed:

```batch
Apply_Fixes.bat Mka "D:\Source\Mka"
```

The batch file will:
1. Check if violations have code context
2. If not, automatically run enrichment
3. Then proceed with AI-powered fixes

### Option 2: Manual (Optional)

If you want to enrich violations separately:

```batch
# Step 1: Enrich violations
Enrich_Violations.bat Mka "D:\Source\Mka"

# Step 2: Apply fixes
Apply_Fixes.bat Mka "D:\Source\Mka"
```

### Option 3: Enrich All Modules

```batch
# Enrich all knowledge bases at once
Enrich_Violations.bat --all "D:\Source"
```

## Before vs After Enrichment

### Before Enrichment
```json
{
  "violation_id": "CERT_C-INT31-i-2",
  "violation_text": "The expression of the 'essentially Enum' type...",
  "severity": "LOW",
  "category": "CERT",
  "file_path": "UNKNOWN",
  "line": "UNKNOWN",
  "code_snippet": "No code snippet available",
  "files_affected": ["Mka.c:145"]
}
```

### After Enrichment
```json
{
  "violation_id": "CERT_C-INT31-i-2",
  "violation_text": "The expression of the 'essentially Enum' type...",
  "severity": "LOW",
  "category": "CERT",
  "file_path": "D:\\Source\\Mka\\src\\Mka.c",
  "file": "Mka.c",
  "line": 145,
  "code_snippet": "status = MKA_GetEnumValue(msgType);",
  "code_context": "void ProcessMessage(uint8 msgType) {\n    MkaStatusType status;\n    \n    status = MKA_GetEnumValue(msgType);\n    \n    if (status == MKA_OK) {\n        // Process\n    }\n}",
  "code_context_lines": {
    "start": 140,
    "end": 150,
    "violation": 145
  },
  "enriched": true,
  "files_affected": ["Mka.c:145"]
}
```

## What the Enricher Does

### 1. Builds File Index
```
Scanning source directory...
  ✓ Found: D:\Source\Mka\src\Mka.c
  ✓ Found: D:\Source\Mka\inc\Mka.h
  ✓ Found: D:\Source\Mka\src\Mka_Wrapper.c
  ...
Indexed 47 unique filenames
```

### 2. Matches Violations to Files
```
Processing violation: CERT_C-INT31-i-2
  File affected: Mka.c:145
  Searching for: Mka.c
  ✓ Found: D:\Source\Mka\src\Mka.c
```

### 3. Reads Code Context
```
Reading code from Mka.c at line 145
  Context: lines 140-150 (5 before, 5 after)
  Target line: status = MKA_GetEnumValue(msgType);
  ✓ Code context extracted
```

### 4. Saves Enriched Data
```
Enriching knowledge base: Mka_KnowledgeDatabase.json
  ✓ Enriched 234 violations
  ✗ Skipped 12 (no file info)
  ✓ Saved to knowledge base
```

## Example Workflow

```batch
# 1. Run analysis (creates knowledge base)
Run.bat Mka

# 2. Enrich violations with source code
Enrich_Violations.bat Mka "D:\Source\Mka"

# Output:
# ================================================================================
#   VIOLATION ENRICHER
# ================================================================================
# [1/2] Activating Python environment...
# [OK] Python environment activated
#
# [2/2] Enriching violations with source code context...
# [INFO] Indexed 47 unique filenames from source directory
# [INFO] Enrichment complete: 234 enriched, 12 skipped
# ✅ Enriched 234 violations
#
# ================================================================================
#   Enrichment Complete!
# ================================================================================
# Your violations now have actual source code context for better AI fixes.

# 3. Apply AI-powered fixes
Apply_Fixes.bat Mka "D:\Source\Mka"

# Now the AI sees actual code like:
# 📄 Current Code (Mka.c:145):
# ────────────────────────────────────────
# 142: void ProcessMessage(uint8 msgType) {
# 143:     MkaStatusType status;
# 144:     
# 145:     status = MKA_GetEnumValue(msgType);
# 146:     
# 147:     if (status == MKA_OK) {
# 148:         // Process message
# 149:     }
# 150: }
# ────────────────────────────────────────
#
# 🤖 Generating AI-powered fix...
# ✨ Proposed Fix:
# Type: cast
# Description: Add explicit type cast from MkaStatusType enum...
```

## Why This Matters for AI Fixes

### Without Code Context (❌ Before)
AI prompt:
```
Violation: CERT_C-INT31-i-2
Description: Enum to unsigned type assignment
File: UNKNOWN
Code: No code snippet available

Generate a fix...
```

AI generates:
```c
// Generic suggestion
uint8_t value = (uint8_t)myEnum;  // Uses generic names!
```

### With Code Context (✅ After)
AI prompt:
```
Violation: CERT_C-INT31-i-2
Description: Enum to unsigned type assignment
File: Mka.c, Line 145

CURRENT CODE:
142: void ProcessMessage(uint8 msgType) {
143:     MkaStatusType status;
144:     
145:     status = MKA_GetEnumValue(msgType);
146:     
147:     if (status == MKA_OK) {
...

Generate a fix for the actual code above...
```

AI generates:
```c
// Context-aware fix with ACTUAL variable names!
142: void ProcessMessage(uint8 msgType) {
143:     MkaStatusType status;
144:     
145:     status = MKA_GetEnumValue((MkaStatusType)msgType);  // Uses real names!
146:     
147:     if (status == MKA_OK) {
```

## Troubleshooting

### Enrichment Failed
```
ERROR: Source code path does not exist: D:\Source\Mka
```
**Solution:** Verify the source code path is correct and exists.

### No Files Found
```
[INFO] Indexed 0 unique filenames from source directory
```
**Solution:** 
- Check the path contains C/C++ source files (.c, .cpp, .h)
- Ensure you're pointing to the actual source directory, not a parent folder

### Files Not Matched
```
[INFO] Enrichment complete: 0 enriched, 234 skipped
```
**Solution:**
- Knowledge base has filenames like "Mka.c:145"
- Source directory must contain files named "Mka.c"
- Check that filenames match between report and source

### Multiple Files with Same Name
```
[DEBUG] Multiple files found for Mka.c, using: D:\Source\Mka\src\Mka.c
```
**Solution:** This is normal. The enricher will use the first match. If incorrect, organize your source to have unique paths.

## Command Reference

### Enrich Single Module
```batch
Enrich_Violations.bat MODULE_NAME "SOURCE_PATH"
```

### Enrich All Modules
```batch
Enrich_Violations.bat --all "SOURCE_PATH"
```

### Python Direct Call
```batch
# Single KB
python src\ViolationEnricher.py --kb "knowledge_base\Mka_KnowledgeDatabase.json" --source "D:\Source\Mka"

# All KBs
python src\ViolationEnricher.py --kb-dir knowledge_base --source "D:\Source"
```

## Integration with Apply_Fixes.bat

`Apply_Fixes.bat` now includes automatic enrichment:

```batch
echo [3/4] Checking if violations need source code enrichment...

# Checks first violation in KB
# If it has no code_snippet or file_path is UNKNOWN:
#   → Runs enrichment automatically
#   → Then continues with fixes
# If already enriched:
#   → Skips enrichment
#   → Goes straight to fixes
```

You don't need to run enrichment manually anymore! Just use:

```batch
Apply_Fixes.bat Mka "D:\Source\Mka"
```

It will handle everything automatically.

## Performance

- **File indexing:** ~1-2 seconds for 1000 files
- **Enrichment:** ~10-50ms per violation
- **Total time:** Usually < 5 seconds for typical projects

## Summary

**Problem:** Violations had no actual code, AI generated generic fixes

**Solution:** Violation Enricher reads actual source code and adds context

**Usage:** Automatic in `Apply_Fixes.bat`, or manual with `Enrich_Violations.bat`

**Result:** AI sees real code, generates precise, context-aware fixes!

---

**Version:** 4.1.0  
**Developer:** Himanshu R  
**Organization:** Qorix India Pvt Ltd
