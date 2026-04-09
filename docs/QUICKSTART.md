# Quick Start Guide - Parasoft AI Agent

## 30-Second Quick Start

1. Place `report_dev1.html` in the tool folder
2. Run `Run.bat`
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
python run_agent.py report_dev1.html YourModule

# View results
python run_query.py knowledge_base --interactive
```

---

## Common Use Cases

### Use Case 1: First-Time Analysis
```bash
# Run full analysis
python run_agent.py report_dev1.html Mka

# Check what violations were found
python run_query.py knowledge_base --module Mka --summary
```

### Use Case 2: Export for Review
```bash
# Export to Excel for team review
python run_query.py knowledge_base --excel violations.xlsx --module Mka
```

### Use Case 3: Find High Priority Issues
```bash
# Interactive mode to query high severity issues
python run_query.py knowledge_base --interactive
# Select option 3 (Query by severity)
# Enter "HIGH"
```

### Use Case 4: Track Progress Over Time
```bash
# Run analysis periodically
python run_agent.py report_dev1_latest.html Mka

# The knowledge database will automatically:
# - Track new violations
# - Update occurrence counts
# - Record analysis history
```

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
python run_agent.py report_dev1.html Mka

# Analysis without fixes
python run_agent.py report_dev1.html Mka --no-fixes

# Analysis without justifications
python run_agent.py report_dev1.html Mka --no-justifications

# Interactive query tool
python run_query.py knowledge_base --interactive

# Generate summary report
python run_query.py knowledge_base --summary report.json

# Export to Excel
python run_query.py knowledge_base --excel output.xlsx
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
| No violations found | Verify report_dev1.html is valid |
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
