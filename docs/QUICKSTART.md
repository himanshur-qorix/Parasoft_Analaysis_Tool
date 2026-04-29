# Quick Start Guide - Parasoft AI Agent

## 🚀 Version 3.0 - Static Code Analyzer Quick Start (NEW!)

### 30-Second Quick Start (Static Analyzer)

1. Navigate to scripts folder
2. Run `Run_Static_Analyzer.bat`
3. Enter source code path (e.g., `d:\Development\GitHub\ParasoftVersion2\Parasoft_Analaysis_Tool\Inputs\ssc\src`)
4. Enter module name (e.g., "Tls")
5. Check results in `reports/` and `history/` directories

**Generates 6 Reports:**
- Color-coded HTML report (RED/ORANGE/GREY)
- Excel violations report with code context
- Knowledge base JSON
- Raw violations JSON
- Module history report (timeline with trends)
- Cross-module comparison (matrix view)

---

## 📊 Static Code Analyzer (v3.0 - Built-in C/C++ Analysis)

### First Time Setup
```bash
# 1. Install dependencies
pip install -r requirements.txt

# 2. Verify Python 3.8+
python --version
```

### Running Static Analysis
```bash
# Using batch script (Windows) - Recommended
cd scripts
Run_Static_Analyzer.bat

# Direct Python
python scripts\run_static_analyzer.py "path\to\source" ModuleName --no-fixes
```

### What It Analyzes
- ✅ Runtime errors (null pointer, buffer overflow, division by zero)
- ✅ MISRA C violations (20+ rules)
- ✅ CERT C issues (15+ rules)
- ✅ Code quality problems
- ✅ Multi-line comment detection (99% false positive reduction)

### Output Files
```
reports/
├── Tls_static_analysis_report.html      # Color-coded violations
├── Tls_violations_report.xlsx            # Excel with context
├── Tls_history_report.html               # Timeline view
└── cross_module_comparison.html          # All modules

history/
├── master_violation_history.json         # All runs tracking
└── resolution_patterns.json              # RAG learning data
```

### Post-Analysis Options
After analysis completes, choose:
1. **AI Mode** - Generate fixes using Ollama AI
2. **Hybrid Mode** - AI for complex, rules for standard (recommended)
3. **Rules Only** - Rule-based fixes without AI
4. **Exit** - Analysis only, no fix generation

---

## 🤖 Parasoft AI Agent (Original Workflow)

## 30-Second Quick Start

1. Place `report_dev1.html` in the `parasoftReport\` folder
2. Run `batch_support\Run.bat`
3. Enter your module name (e.g., "Mka")
4. Check results in `knowledge_base/` and `fixes/` directories

---

## 5-Minute Setup

### First Time Setup
```bash
# 1. Install dependencies
pip install -r requirements.txt

# 2. Verify Git is installed
git --version

# 3. Initialize Git repository (if needed)
cd your-project
git init
```

### Running Analysis
```bash
# Basic usage
python src\run_agent.py report_dev1.html YourModule

# View results
python src\run_query.py knowledge_base --interactive
```

---

## Common Use Cases

### Use Case 1: First-Time Analysis
```bash
# Run full analysis
python src\run_agent.py report_dev1.html Mka

# Check what violations were found
python src\run_query.py knowledge_base --module Mka --summary
```

### Use Case 2: Export for Review
```bash
# Export to Excel for team review
python src\run_query.py knowledge_base --excel violations.xlsx --module Mka
```

### Use Case 3: Find High Priority Issues
```bash
# Interactive mode to query high severity issues
python src\run_query.py knowledge_base --interactive
# Select option 3 (Query by severity)
# Enter "HIGH"
```

### Use Case 4: Track Progress Over Time
```bash
# Run analysis periodically
python src\run_agent.py parasoftReport\report_dev1_latest.html Mka

# The knowledge database will automatically:
# - Track new violations
# - Update occurrence counts
# - Record analysis history
```

### Use Case 5: Generate CERT & MISRA Focused Report (v3.0 - NEW!)
```bash
# Using batch file (easiest)
cd scripts
Generate_CERT_MISRA_Report.bat
# Enter: Tls

# Or direct Python
python src\generate_cert_misra_report.py Tls

# Output: reports\Tls_CERT_MISRA_Report.html
# - Filtered CERT and MISRA violations only
# - Top 10 violations by occurrence
# - Severity breakdown
# - Status summary (Open/Fixed/Justified)
```

**When to use:**
- Need compliance-focused report for code review
- Tracking CERT/MISRA progress
- Quick compliance status check
- Report to management/stakeholders

---

## Understanding Output Files

### Knowledge Database (`Mka_KnowledgeDatabase.json`)
- Contains all unique violations
- Tracks occurrence counts
- Records fix and justification status
- Maintains analysis history

### Analysis Summary (`Mka_analysis_summary.json`)
- Overall statistics
- Git commit information
- Counts of new vs existing violations

### Fixes Directory (`fixes/Mka/`)
- `Mka_fixes_*.txt` - Code fix suggestions
- `Mka_justifications_*.txt` - Parasoft justification comments

---

## Quick Reference Commands

```bash
# Full analysis with all features
python src\run_agent.py parasoftReport\report_dev1.html Mka

# Analysis without fixes
python src\run_agent.py parasoftReport\report_dev1.html Mka --no-fixes

# Analysis without justifications
python src\run_agent.py parasoftReport\report_dev1.html Mka --no-justifications

# Interactive query tool
python src\run_query.py knowledge_base --interactive

# Generate summary report
python src\run_query.py knowledge_base --summary report.json

# Export to Excel
python src\run_query.py knowledge_base --excel output.xlsx
```

---

## Tips & Tricks

1. **Run analysis after each code change** to track violation trends
2. **Use Excel export** for team reviews and presentations
3. **Focus on HIGH severity first** for maximum impact
4. **Review justifications** before applying to ensure accuracy
5. **Keep knowledge database** under version control for history

---

## Troubleshooting Quick Fixes

| Problem | Solution |
|---------|----------|
| Dependencies not found | `pip install -r requirements.txt` |
| Git not found | Install from https://git-scm.com/ |
| No violations found | Verify parasoftReport\report_dev1.html is valid |
| Permission errors | Run as administrator or change workspace |

---

## Next Steps

After first analysis:
1. Review knowledge database summary
2. Export to Excel for team discussion
3. Apply high-priority fixes
4. Add justifications where needed
5. Re-run analysis to track progress

---

For detailed documentation, see [README.md](README.md)

