# Knowledge Base Integration Guide

## Overview

The MISRA/CERT checker and other analysis tools now integrate with the knowledge base to provide intelligent insights:

- **Identify known vs new violations** - See which violations you've seen before
- **Proven fix suggestions** - Get fixes that worked in other modules
- **Occurrence tracking** - Know how many times each violation appeared
- **Cross-module insights** - Leverage fixes from other modules

## Enhanced MISRA/CERT Checker

### Basic Usage with Knowledge Base

```powershell
# Analyze with knowledge base integration
python src\misra_cert_checker.py D:\MyProject\src --module Mka --show-known --report html --output report.html
```

### Command Line Options

| Option | Description |
|--------|-------------|
| `--module <name>` | Module name to check against knowledge base |
| `--kb-dir <path>` | Knowledge base directory (default: ../knowledge_base) |
| `--show-known` | Show detailed breakdown of known vs new violations |
| `--report <format>` | Output format: html, json, or text |
| `--output <file>` | Save report to file |

### Enhanced Output

#### Console Output

```
[INFO] Analyzing: D:\MyProject\src
[INFO] Loaded knowledge base for module: Mka
[INFO] Loaded master knowledge base
[INFO] Analyzed 15 file(s), found 65 violation(s).

==================================================
KNOWLEDGE BASE SUMMARY
==================================================
  Known Violations (seen before): 42
  New Violations (first time): 23
  Total Violations: 65
  Known with Proven Fixes: 15

  ⚠️  23 NEW violations detected!
      Review these carefully as they haven't been seen before

  ✅ 15 violations have proven fixes available
      Apply these fixes to resolve issues faster
==================================================

==================================================
KNOWN VIOLATIONS DETAILS
==================================================

  MISRA-C:2012 Rule 14.4
    Previously seen: 8 times
    ✅ Proven fix: Use explicit comparison: if (x != 0) instead of if (x)

  CERT-EXP34-C
    Previously seen: 5 times
    ✅ Proven fix: [From Mkb] Add NULL pointer check before dereference

  MISRA-C:2012 Rule 21.3
    Previously seen: 3 times

==================================================
NEW VIOLATIONS DETAILS
==================================================

  CERT-MEM31-C
    Severity: MEDIUM | Category: Memory
    ⚠️  First time detected - review carefully

  MISRA-C:2012 Rule 16.7
    Severity: REQUIRED | Category: Functions
    ⚠️  First time detected - review carefully
==================================================
```

#### HTML Report

The HTML report includes:

**Summary Cards:**
- Total Violations
- Known Violations (green)
- New Violations (red)
- With Proven Fixes (blue)

**Visual Indicators:**
- ✅ KNOWN (Fix Available) - Green badge
- 📊 KNOWN (Nx) - Blue badge showing occurrence count
- ⚠️ NEW - Red badge for first-time violations

**Fix Suggestions:**
- Proven fixes shown inline under violation messages
- Tooltips with full fix descriptions
- Source module indicated for cross-module fixes

**Alert Banners:**
- Warning for new violations
- Success message for available fixes

## Integration with Workflow

### Automatic Integration in Run.bat

When you run `scripts\Run.bat`, the tool automatically:

1. Checks if knowledge base exists for the module
2. Loads module-specific knowledge base
3. Loads master knowledge base for cross-module insights
4. Classifies violations as known vs new
5. Adds proven fix suggestions to the report

### Example Workflow

#### Initial Analysis (No Knowledge Base)

```powershell
scripts\Run.bat
# Enter module: Mka
# Enter source path: D:\MyProject\src

Output:
  Total Violations: 65
  All 65 are NEW (no knowledge base exists yet)
```

#### Second Analysis (With Knowledge Base)

```powershell
scripts\Run.bat
# Enter module: Mka
# Enter source path: D:\MyProject\src

Output:
  Known Violations: 58 (same as before)
  New Violations: 7 (recently introduced)
  With Proven Fixes: 15 (from previous work)
```

#### Analysis on Different Module (Cross-Module Insights)

```powershell
scripts\Run.bat
# Enter module: Mkb
# Enter source path: D:\AnotherProject\src

Output:
  Known Violations: 32 (seen in Mka or other modules)
  New Violations: 18 (first time in any module)
  With Proven Fixes: 20 (from Mka and other modules)
```

## Benefits

### 1. Focus on What's Important

```
Known: 42 violations (already documented)
New: 23 violations ← REVIEW THESE FIRST!
```

### 2. Apply Proven Fixes

```
CERT-EXP34-C: Found in 3 files
Fix (from Mkb): Add NULL pointer check
Success Rate: 100%
→ Apply the same pattern to all 3 instances
```

### 3. Track Progress

```
First Run: 65 violations (all new)
Second Run: 58 known, 7 new
Third Run: 65 known, 0 new ← All resolved or documented!
```

### 4. Cross-Team Learning

```
Module Mka fixed MISRA-C:2012 Rule 21.3
Module Mkb sees same violation
→ Automatically suggests Mka's fix
```

## Advanced Usage

### Check Against Specific Knowledge Base

```powershell
# Use custom knowledge base location
python src\misra_cert_checker.py D:\MyCode\src ^
  --module MyModule ^
  --kb-dir D:\CustomPath\knowledge_base ^
  --show-known ^
  --report html ^
  --output my_report.html
```

### Standalone Analysis (No Knowledge Base)

```powershell
# Omit --module to skip knowledge base integration
python src\misra_cert_checker.py D:\MyCode\src --report html --output report.html
```

### JSON Output with Knowledge Base

```powershell
# Get machine-readable output with KB insights
python src\misra_cert_checker.py D:\MyCode\src ^
  --module Mka ^
  --report json ^
  --output violations.json
```

## Knowledge Base Integration Flow

```
┌─────────────────────────────────────────────────────────────┐
│  Run MISRA/CERT Checker with --module flag                  │
└─────────────────────────┬───────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────────┐
│  Load Knowledge Bases:                                       │
│  1. Module-specific KB (if exists)                          │
│  2. Master KB (if exists)                                   │
└─────────────────────────┬───────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────────┐
│  For Each Violation Found:                                   │
│  • Check if rule_id exists in module KB                     │
│  • If not, check master KB for cross-module insights       │
│  • Mark as KNOWN or NEW                                     │
│  • Add occurrence count                                     │
│  • Add proven fix if available                              │
└─────────────────────────┬───────────────────────────────────┘
                          ↓
┌─────────────────────────────────────────────────────────────┐
│  Generate Enhanced Report:                                   │
│  • Classify violations (Known vs New)                       │
│  • Show proven fix suggestions inline                       │
│  • Highlight new violations for review                      │
│  • Display occurrence counts                                │
└─────────────────────────────────────────────────────────────┘
```

## Troubleshooting

### Q: No knowledge base summary shown?

**A:** Use `--module <name>` flag to enable knowledge base integration

```powershell
python src\misra_cert_checker.py mycode/ --module Mka
```

### Q: Shows 0 known violations despite having knowledge base?

**A:** Check that:
1. Module name matches exactly (`Mka` vs `mka`)
2. Knowledge base file exists: `knowledge_base/Mka_KnowledgeDatabase.json`
3. Knowledge base has violations populated

### Q: Want cross-module insights only?

**A:** Run consolidation first, then use master KB:

```powershell
scripts\Consolidate_Knowledge.bat
python src\misra_cert_checker.py mycode/ --module NewModule
# Will use master KB for insights
```

### Q: How to see proven fix details?

**A:** Use `--show-known` flag:

```powershell
python src\misra_cert_checker.py mycode/ --module Mka --show-known
```

## Best Practices

### 1. Always Use Module Names

```powershell
# Good - enables knowledge base
python src\misra_cert_checker.py src/ --module Mka

# Missing insights - no KB integration
python src\misra_cert_checker.py src/
```

### 2. Review New Violations First

Focus on NEW violations as they represent:
- Recently introduced issues
- Patterns not seen before
- Potential new types of problems

### 3. Apply Proven Fixes

When you see "✅ KNOWN (Fix Available)":
1. Review the proven fix suggestion
2. Check if it applies to your context
3. Apply the fix
4. Reduces resolution time by 50-70%

### 4. Run After Each Analysis

After running main analysis tool:
```powershell
scripts\Run.bat  # Creates/updates knowledge base
# Next run will show known vs new
```

### 5. Use with Master Knowledge Base

```powershell
# 1. Analyze multiple modules
scripts\Run.bat  # Module 1
scripts\Run.bat  # Module 2

# 2. Consolidate
scripts\Consolidate_Knowledge.bat

# 3. Analyze new module with cross-module insights
python src\misra_cert_checker.py newcode/ --module Module3
# Gets fixes from Module1 and Module2!
```

## See Also

- [CROSS_MODULE_KNOWLEDGE.md](CROSS_MODULE_KNOWLEDGE.md) - Master KB guide
- [AUTO_GENERATE_WORKFLOW.md](AUTO_GENERATE_WORKFLOW.md) - Using without Parasoft report
- [MISRA_CERT_INTEGRATION.md](MISRA_CERT_INTEGRATION.md) - Complete integration guide
- [QUICKSTART.md](QUICKSTART.md) - Getting started
