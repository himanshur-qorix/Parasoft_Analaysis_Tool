# 📚 Documentation Index - Parasoft AI Agent

Welcome to the Parasoft AI Agent documentation! This index helps you find the right document for your needs.

## 📁 New Project Structure (v2.0.1)

The project has been reorganized with a clean folder structure:
- **src/** - All Python source code
- **docs/** - All documentation (you are here!)
- **config/** - Configuration files
- **scripts/** - Automation scripts
- **assets/** - Images and diagrams
- **data/** - Reference data files

See [STRUCTURE.md](../STRUCTURE.md) for complete details.

---

## 🚀 Getting Started

| Document | Purpose | When to Read |
|----------|---------|--------------|
| [README.md](../README.md) | Complete user documentation | First-time setup, comprehensive guide |
| [QUICKSTART.md](QUICKSTART.md) | Quick start guide | Want to start in 30 seconds |
| [EXAMPLE_WORKFLOW.md](EXAMPLE_WORKFLOW.md) | Real-world usage example | Learning by example |
| [STRUCTURE.md](../STRUCTURE.md) | New folder organization | Understanding project layout |

**Recommendation:** Start with [QUICKSTART.md](QUICKSTART.md), then move to [README.md](../README.md) for details.

---

## � Direct Violation Capture (New!)

| Document | Purpose | When to Read |
|----------|---------|--------------|
| [VIOLATION_CAPTURE_GUIDE.md](VIOLATION_CAPTURE_GUIDE.md) | Direct source code scanning | Scan code without Parasoft |
| [EXCEL_REPORT_GUIDE.md](EXCEL_REPORT_GUIDE.md) | Excel report generation | Professional violation reports |
| [capture_violations.py](../capture_violations.py) | Standalone scanning tool | Detect violations directly |

**What's New:** The violation capture tool scans C/C++ source files directly to detect buffer overflows, memory leaks, MISRA/CERT violations - **without requiring Parasoft**. Generate reports in JSON, Text, CSV, or **Excel** format!

**Quick Usage:**
```bash
# Scan and generate Excel report
python capture_violations.py src/ --format excel

# Scan with knowledge base export
python capture_violations.py src/ --module MyModule --kb
```

---

## �🏗️ Technical Documentation

| Document | Purpose | Audience |
|----------|---------|----------|
| [ARCHITECTURE.md](ARCHITECTURE.md) | System architecture & design | Developers, advanced users |
| [CHANGELOG.md](CHANGELOG.md) | Version history & changes | Anyone tracking updates |
| [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md) | Project completion summary | Project managers, stakeholders |

**Recommendation:** Developers should read [ARCHITECTURE.md](ARCHITECTURE.md) to understand system design.

---

## 📋 Configuration & Setup

| File | Purpose | Notes |
|------|---------|-------|
| [requirements.txt](requirements.txt) | Python dependencies | Run: `pip install -r requirements.txt` |
| [config.json](config.json) | Agent configuration | Customize analysis behavior |
| [.gitignore](.gitignore) | Git ignore rules | For repository integration |

**Recommendation:** Review [config.json](config.json) to customize the agent for your needs.

---

## 🛠️ Source Code Files

### Core Components

| File | Lines | Description |
|------|-------|-------------|
| [run_agent.py](run_agent.py) | 265 | Main orchestrator - entry point |
| [KnowledgeDatabaseManager.py](KnowledgeDatabaseManager.py) | 374 | Knowledge database management |
| [ViolationAnalyzer.py](ViolationAnalyzer.py) | 303 | Intelligent violation analysis |
| [CodeFixGenerator.py](CodeFixGenerator.py) | 475 | Fix and justification generation |
| [run_query.py](run_query.py) | 613 | Query and reporting tool |

### Legacy/Support

| File | Description |
|------|-------------|
| [ParasoftAnalysisTool.py](ParasoftAnalysisTool.py) | Original HTML parser (v1.0.0) |
| [Run.bat](Run.bat) | Windows batch automation script |

---

## 📖 Usage by Scenario

### Scenario 1: First-Time User
1. Read [QUICKSTART.md](QUICKSTART.md)
2. Run [Run.bat](Run.bat)
3. Check [EXAMPLE_WORKFLOW.md](EXAMPLE_WORKFLOW.md)

### Scenario 2: Command-Line User
1. Read [README.md](README.md) - "Command Line Usage" section
2. Review command options
3. Use [run_query.py](run_query.py) for queries

### Scenario 3: Developer/Integrator
1. Read [ARCHITECTURE.md](ARCHITECTURE.md)
2. Study source code files
3. Review [config.json](config.json) for customization
4. Check [CHANGELOG.md](CHANGELOG.md) for version info

### Scenario 4: Team Lead/Manager
1. Read [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md)
2. Review [EXAMPLE_WORKFLOW.md](EXAMPLE_WORKFLOW.md)
3. Use Excel export feature for team reports
4. Check [README.md](README.md) - "Workflow Integration" section

---

## 🎯 Common Tasks & Relevant Docs

### Task: Install and Run First Analysis
- [QUICKSTART.md](QUICKSTART.md) - Full setup guide
- [requirements.txt](requirements.txt) - Dependencies
- [Run.bat](Run.bat) - Quick execution

### Task: Query Knowledge Database
- [README.md](README.md) - "Querying Knowledge Database" section
- [run_query.py](run_query.py) - Tool usage

### Task: Generate Reports
- [README.md](README.md) - "Understanding the Analysis" section
- [EXAMPLE_WORKFLOW.md](EXAMPLE_WORKFLOW.md) - Step 4: Export for Team Review
- [EXCEL_REPORT_GUIDE.md](EXCEL_REPORT_GUIDE.md) - **NEW:** Excel report generation

### Task: Scan Source Code Directly
- [VIOLATION_CAPTURE_GUIDE.md](VIOLATION_CAPTURE_GUIDE.md) - Complete scanning guide
- [EXCEL_REPORT_GUIDE.md](EXCEL_REPORT_GUIDE.md) - Excel output format
- [capture_violations.py](../capture_violations.py) - Scanning tool

### Task: Apply Fixes to Code
- [EXAMPLE_WORKFLOW.md](EXAMPLE_WORKFLOW.md) - Step 6: Apply Fixes
- [README.md](README.md) - "Automatic Fix Generation" section

### Task: Add Justifications
- [README.md](README.md) - "Parasoft Justification Format" section
- [EXAMPLE_WORKFLOW.md](EXAMPLE_WORKFLOW.md) - Step 7: Add Justifications

### Task: Understand System Architecture
- [ARCHITECTURE.md](ARCHITECTURE.md) - Complete architecture
- Source code docstrings

### Task: Customize Agent Behavior
- [config.json](config.json) - Configuration options
- [README.md](README.md) - "Configuration Options" section

---

## 📊 File Size & Complexity Reference

```
Simple         Medium         Complex
(< 100 lines)  (100-300)      (300+)
─────────────────────────────────────────
config.json    ParasoftAIAgent ViolationAnalyzer
Run.bat        README.md      CodeFixGenerator
               QUICKSTART.md  KnowledgeDatabaseManager
                              KnowledgeDatabaseQueryTool
                              ARCHITECTURE.md
```

---

## 🔍 Quick Reference

### All Python Files
```
run_agent.py              - Main entry point
KnowledgeDatabaseManager.py     - Database operations
ViolationAnalyzer.py            - Analysis logic
CodeFixGenerator.py             - Fix generation
run_query.py   - Query interface
ParasoftAnalysisTool.py         - HTML parser (legacy)
```

### All Documentation Files
```
README.md               - Complete documentation
QUICKSTART.md          - Quick start guide
EXAMPLE_WORKFLOW.md    - Real-world example
ARCHITECTURE.md        - System architecture
CHANGELOG.md           - Version history
PROJECT_SUMMARY.md     - Project summary
INDEX.md               - This file
```

### All Configuration Files
```
config.json        - Agent configuration
requirements.txt   - Python dependencies
.gitignore        - Git ignore rules
```

### All Scripts
```
Run.bat           - Windows automation script
```

---

## 📞 Document Maintenance

### Last Updated
- **Date:** April 9, 2026
- **Version:** 2.0.0
- **Author:** Himanshu R

### Document Status
All documents are current and synchronized with version 2.0.0.

---

## 🗂️ Directory Structure

```
Parasoft_Analaysis_Tool/
│
├── 📄 Core Python Files
│   ├── run_agent.py
│   ├── KnowledgeDatabaseManager.py
│   ├── ViolationAnalyzer.py
│   ├── CodeFixGenerator.py
│   ├── run_query.py
│   └── ParasoftAnalysisTool.py
│
├── 📋 Configuration Files
│   ├── config.json
│   ├── requirements.txt
│   └── .gitignore
│
├── 📚 Documentation
│   ├── README.md
│   ├── QUICKSTART.md
│   ├── EXAMPLE_WORKFLOW.md
│   ├── ARCHITECTURE.md
│   ├── CHANGELOG.md
│   ├── PROJECT_SUMMARY.md
│   └── INDEX.md (this file)
│
├── 🔧 Scripts
│   └── Run.bat
│
├── 📊 Data Files (auto-generated)
│   ├── knowledge_base/
│   ├── reports/
│   ├── fixes/
│   └── parasoft_agent.log
│
└── 🎨 Assets
    └── FlowDiagram_V1.0.0.png
```

---

## 💡 Tips for Using This Index

1. **New Users:** Start at the top, work down
2. **Quick Lookup:** Use Ctrl+F to search
3. **Developers:** Jump to "Source Code Files" section
4. **Managers:** Check "Scenario 4" section

---

## 🔗 External Resources

- **Parasoft Documentation:** https://docs.parasoft.com/
- **MISRA Guidelines:** https://www.misra.org.uk/
- **CERT Secure Coding:** https://wiki.sei.cmu.edu/confluence/display/seccode
- **Python Documentation:** https://docs.python.org/

---

**Need help? Start with [QUICKSTART.md](QUICKSTART.md)!** 🚀
