# Knowledge Base & Learning Guide

**Complete guide to knowledge databases, cross-module learning, and rule-based pattern recognition**

---

## Table of Contents

1. [Knowledge Base Overview](#knowledge-base-overview)
2. [Cross-Module Knowledge](#cross-module-knowledge)
3. [Parasoft Rules Database](#parasoft-rules-database)
4. [Rule-Based Learning](#rule-based-learning)
5. [Knowledge Base Queries](#knowledge-base-queries)

---

## Knowledge Base Overview

### What is the Knowledge Base?

The Knowledge Base is an intelligent database that tracks:
- **All violations** found in your code
- **Fix history** and success rates
- **Justifications** and suppression patterns
- **Resolution patterns** learned over time
- **Cross-module insights** from analyzing multiple modules

### How It Works

```
1. Analyze Code → 2. Build Knowledge DB → 3. Query & Learn → 4. Apply Insights
```

Each module gets its own knowledge database:
```
knowledge_base/
├── Mka_KnowledgeDatabase.json      # Module-specific knowledge
├── Mkb_KnowledgeDatabase.json
├── Mkc_KnowledgeDatabase.json
├── learned_patterns_db.json         # Learned resolution patterns
└── Master_KnowledgeDatabase.json    # Consolidated cross-module knowledge
```

### Database Structure

Each violation entry contains:
```json
{
  "violation_id": "CERT_C-INT31-i-2",
  "rule": "INT31-C",
  "severity": "HIGH",
  "description": "essentially signed to essentially unsigned",
  "occurrences": 42,
  "files_affected": ["Mka_Cfg.c", "Mka_Cbk.c"],
  "fixes_applied": 38,
  "justifications": 4,
  "resolution_pattern": "explicit_cast",
  "confidence": 0.92,
  "first_seen": "2026-04-10",
  "last_seen": "2026-04-29"
}
```

### Benefits

- ✅ **Track Progress** - See violations over time
- ✅ **Learn Patterns** - AI learns from your team's decisions
- ✅ **Share Knowledge** - Cross-module insights
- ✅ **Prioritize Work** - Focus on common issues
- ✅ **Audit Trail** - Complete violation history

---

## Cross-Module Knowledge

### Overview

The **Cross-Module Knowledge Base** aggregates learnings from all analyzed modules to create a master knowledge database.

### How It Works

#### 1. Analyze Multiple Modules

```powershell
# Analyze Module 1
scripts\Run.bat
# Enter: Mka

# Analyze Module 2  
scripts\Run.bat
# Enter: Mkb

# Analyze Module 3
scripts\Run.bat
# Enter: Mkc
```

Each creates: `knowledge_base/{Module}_KnowledgeDatabase.json`

#### 2. Consolidate Knowledge

**Option 1: GUI**
```
1. Launch scripts\Launch_GUI.bat
2. Go to "Advanced" tab
3. Click "Consolidate Knowledge" button
```

**Option 2: Batch File**
```powershell
.\batch_support\Consolidate_Knowledge.bat
```

**Option 3: Command Line**
```bash
python src\consolidate_knowledge.py
```

#### 3. Output Files

Consolidation creates:
- **Master_KnowledgeDatabase.json** - All violations from all modules
- **Master_Knowledge_Report.xlsx** - Excel report with insights
- **Master_Knowledge_Report.html** - Interactive HTML report

### Cross-Module Insights

The master database provides:

**1. Widespread Issues**
- Violations appearing in 2+ modules
- Company-wide coding problems
- Systematic issues needing attention

**2. Proven Fix Patterns**
- Fixes that worked across multiple modules
- Best practices from other teams
- Reusable code patterns

**3. Module Comparison**
```
Module   | Total Violations | Fixed | Suppressed | Open
---------|------------------|-------|------------|------
Mka      | 248             | 186   | 42         | 20
Mkb      | 312             | 201   | 89         | 22
Mkc      | 156             | 134   | 18         | 4
```

**4. Prioritization**
- Issues affecting multiple modules ranked higher
- Resolution success rates guide fix strategies
- Resource allocation based on impact

### Query Master Knowledge

**Option 1: GUI**
```
1. Launch scripts\Launch_GUI.bat
2. Go to "Advanced" tab
3. Click "Query Master KB" button
```

**Option 2: Batch File**
```powershell
.\batch_support\Query_Master_Knowledge.bat
```

**Option 3: Command Line**
```bash
# Interactive query
python src\run_query.py master_knowledge --interactive

# Export to Excel
python src\run_query.py master_knowledge --excel master_report.xlsx

# Get summary
python src\run_query.py master_knowledge --summary
```

### Use Cases

**1. Team Standardization**
- See how different modules handle same violations
- Establish company-wide coding standards
- Share best practices across teams

**2. Priority Planning**
- Focus on violations affecting multiple modules
- Allocate resources to widespread issues
- Track company-wide progress

**3. Knowledge Sharing**
- New team members learn from existing patterns
- Proven fixes available immediately
- Reduce duplicate effort

**4. Audit & Compliance**
- Complete violation history across all modules
- Demonstrate continuous improvement
- Track compliance metrics

---

## Parasoft Rules Database

### Overview

The **Parasoft Rules Database** contains official fix suggestions from 1200+ Parasoft rules, providing instant, trusted recommendations.

### Database Coverage

- **MISRA C:2012** - 175+ rules
- **CERT C** - 120+ rules
- **CWE** - Common Weakness Enumeration
- **Custom Rules** - Company-specific standards
- **AUTOSAR** - Automotive standards

### How It Works

#### 1. Build Rules Database

**Option 1: GUI**
```
1. Launch scripts\Launch_GUI.bat
2. Go to "Advanced" tab
3. Click "Build Parasoft Rules DB" button
```

**Option 2: Batch File**
```powershell
.\batch_support\Build_Parasoft_Rules_Database.bat
```

**Option 3: Command Line**
```bash
python src\build_parasoft_rules_db.py
```

This creates: `data/Parasoft_Rules_Database.json`

#### 2. Use in Analysis

The rules database is automatically consulted during analysis:

```
1. Violation detected → 2. Check rules DB → 3. Apply official fix
```

**Benefits:**
- ✅ Instant fixes from official Parasoft guidance
- ✅ No AI required for standard violations
- ✅ Consistent with Parasoft best practices
- ✅ Faster than AI inference

### Database Structure

```json
{
  "CERT_C-INT31-i-2": {
    "rule_id": "INT31-C",
    "title": "Ensure that integer conversions do not result in lost or misinterpreted data",
    "severity": "HIGH",
    "category": "Type Safety",
    "fix_template": "Add explicit cast: ({target_type}){expression}",
    "examples": [
      {
        "before": "uint16 value = enum_value;",
        "after": "uint16 value = (uint16)enum_value;",
        "explanation": "Explicit cast ensures type safety"
      }
    ],
    "references": [
      "https://wiki.sei.cmu.edu/confluence/display/c/INT31-C"
    ]
  }
}
```

### Query Rules Database

```bash
# Search by rule ID
python src\query_rules_db.py --rule CERT_C-INT31-i-2

# Search by keyword
python src\query_rules_db.py --keyword "type conversion"

# List all MISRA rules
python src\query_rules_db.py --standard MISRA

# Get fix examples
python src\query_rules_db.py --rule CERT_C-INT31-i-2 --examples
```

### Customization

Add your own rules to `data/Parasoft_Rules_Database.json`:

```json
{
  "CUSTOM-001": {
    "rule_id": "CUSTOM-001",
    "title": "Use company naming convention for global variables",
    "severity": "MEDIUM",
    "fix_template": "Rename to: g_{variable_name}",
    "category": "Naming Convention"
  }
}
```

---

## Rule-Based Learning

### Overview

**Rule-Based Learning** extracts patterns from code without requiring AI, providing fast, deterministic fix suggestions.

### How It Works

1. **Analyze Historical Fixes** - Examines past violation resolutions
2. **Extract Patterns** - Identifies common fix patterns
3. **Build Rule Set** - Creates deterministic rules
4. **Apply Automatically** - Fixes applied instantly

### Pattern Types

**1. Direct Pattern Matching**
```
Pattern: "enum assigned to uint16"
Rule: Add (uint16) cast
Confidence: 100%
```

**2. Context-Based Patterns**
```
Pattern: "null pointer check missing"
Context: "Function with pointer parameter"
Rule: Add null check at function start
Confidence: 95%
```

**3. Code Structure Patterns**
```
Pattern: "switch case without break"
Context: "Non-empty case block"
Rule: Add break statement
Confidence: 98%
```

### Usage

Rule-based learning runs automatically in `hybrid` mode:

```bash
# Automatic rule-based + AI
python src\run_agent.py parasoftReport\report_dev1.html Mka --ai-mode hybrid

# Rules only (no AI)
python src\run_agent.py parasoftReport\report_dev1.html Mka --ai-mode rules_only
```

### Performance Comparison

| Method | Speed | Accuracy | Setup | Resources |
|--------|-------|----------|-------|-----------|
| **Rule-Based** | ⚡ Very Fast | 90% | None | Minimal |
| **AI-Based** | 🐢 Slower | 95% | Ollama | High |
| **Hybrid** | ⚡ Fast | 95% | Optional | Low |

### Benefits

- ✅ **No AI Required** - Works without Ollama
- ✅ **Deterministic** - Same input = same output
- ✅ **Fast** - Instant pattern matching
- ✅ **Offline** - No internet needed
- ✅ **Low Resources** - Minimal CPU/RAM

### Rule Management

View learned rules:
```bash
python src\show_learned_rules.py
```

Export rules:
```bash
python src\export_rules.py --output rules_export.json
```

Import rules (share across teams):
```bash
python src\import_rules.py --input team_rules.json
```

---

## Knowledge Base Queries

### Interactive Query Tool

**Launch Interactive Mode:**

```bash
python src\run_query.py knowledge_base --interactive
```

**Available Queries:**

1. **Summary** - Overall violation statistics
2. **By Rule** - Query specific violation rule
3. **By Severity** - Filter by HIGH/MEDIUM/LOW
4. **By File** - Violations in specific file
5. **History** - Violation trends over time
6. **Recommendations** - AI-suggested actions

### Command Line Queries

**Get Summary:**
```bash
python src\run_query.py knowledge_base --summary
```

**Query by Rule:**
```bash
python src\run_query.py knowledge_base --rule CERT_C-INT31-i-2
```

**Query by Severity:**
```bash
python src\run_query.py knowledge_base --severity HIGH
```

**Export to Excel:**
```bash
python src\run_query.py knowledge_base --excel violation_report.xlsx
```

**Export to HTML:**
```bash
python src\run_query.py knowledge_base --html violation_report.html
```

### Query Master Knowledge Base

After consolidating multiple modules:

```bash
# Interactive queries across all modules
python src\run_query.py master_knowledge --interactive

# Get cross-module comparison
python src\run_query.py master_knowledge --comparison

# Export master report
python src\run_query.py master_knowledge --excel master_violations.xlsx
```

### Example Queries

**Find High-Priority Issues:**
```bash
python src\run_query.py knowledge_base --severity HIGH --status OPEN
```

**Track Progress:**
```bash
python src\run_query.py knowledge_base --history --from 2026-04-01 --to 2026-04-29
```

**Get Fix Success Rate:**
```bash
python src\run_query.py knowledge_base --stats --fix-rate
```

**Find Similar Violations:**
```bash
python src\run_query.py knowledge_base --similar "type conversion"
```

---

## Workflow Integration

### Recommended Workflow

**Step 1: Initial Analysis**
```bash
scripts\Launch_GUI.bat
# Analyze module → Creates knowledge_base/{Module}_KnowledgeDatabase.json
```

**Step 2: Query Violations**
```bash
python src\run_query.py knowledge_base --interactive
# Explore violations, prioritize work
```

**Step 3: Apply Fixes**
- Review generated fixes in `fixes/` folder
- Apply to source code
- Re-run analysis to update knowledge base

**Step 4: Learn Patterns**
```bash
.\batch_support\Learn_Justification_Patterns.bat
# System learns from your team's decisions
```

**Step 5: Cross-Module Consolidation**
```bash
.\batch_support\Consolidate_Knowledge.bat
# Create master knowledge base
```

**Step 6: Share Insights**
```bash
python src\run_query.py master_knowledge --excel team_report.xlsx
# Share with team for standardization
```

### Continuous Improvement

1. **Daily**: Run analysis on changed code
2. **Weekly**: Review knowledge base queries
3. **Monthly**: Consolidate cross-module knowledge
4. **Quarterly**: Update rules database with new patterns

---

## Summary

The Knowledge Base system provides:

- ✅ **Violation Tracking** - Complete history across all modules
- ✅ **Cross-Module Learning** - Share insights across teams
- ✅ **Parasoft Rules DB** - 1200+ official fix suggestions
- ✅ **Rule-Based Patterns** - Fast, deterministic fixes
- ✅ **Interactive Queries** - Flexible data exploration
- ✅ **Progress Tracking** - Monitor improvement over time
- ✅ **Audit Trail** - Complete compliance documentation

For more information, see [AI_GUIDE.md](AI_GUIDE.md) for AI-powered features or [QUICKSTART.md](QUICKSTART.md) for basic usage.
