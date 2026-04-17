# 📚 Documentation Index - Parasoft AI Agent

Welcome to the Parasoft AI Agent documentation! This index helps you find the right document for your needs.

## 📁 New Project Structure (v3.0.0)

The project has been enhanced with violation history tracking and RAG learning:
- **src/** - All Python source code (including new ViolationHistoryManager & ComparisonReportGenerator)
- **docs/** - All documentation (you are here!)
- **config/** - Configuration files
- **scripts/** - Automation scripts (including Run_Static_Analyzer.bat)
- **assets/** - Images and diagrams
- **data/** - Reference data files and raw violation data
- **history/** ⭐ NEW - Violation history tracking and RAG learning data

See [STRUCTURE.md](STRUCTURE.md) for complete details.

---

## 🚀 Getting Started

| Document | Purpose | When to Read |
|----------|---------|--------------|
| [README.md](../README.md) | Complete user documentation | First-time setup, comprehensive guide |
| [QUICKSTART.md](QUICKSTART.md) | Quick start guide (v3.0 updated) | Want to start in 30 seconds |
| [EXAMPLE_WORKFLOW.md](EXAMPLE_WORKFLOW.md) | Real-world usage example | Learning by example |
| [STRUCTURE.md](STRUCTURE.md) | Project organization (v3.0) | Understanding project layout |
| [CHANGELOG.md](CHANGELOG.md) | Version history (v3.0.0 added) | See what's new in v3.0.0 |
| [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md) | Complete project summary (v3.0) | Full feature overview |

**Recommendation:** Start with [QUICKSTART.md](QUICKSTART.md), then move to [README.md](../README.md) for details.

---

## 🆕 Version 3.0.0 Features (NEW!)

| Feature | Description | Documentation |
|---------|-------------|---------------|
| **Violation History Tracking** | Complete timeline of all violations across modules | [CHANGELOG.md](CHANGELOG.md#300---2026-04-16) |
| **RAG Learning System** | AI learns from resolution patterns with confidence scores | [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md#new-feature-2-rag-learning-system) |
| **Comparison Reports** | Module timeline & cross-module matrix HTML reports | [PROJECT_SUMMARY.md](PROJECT_SUMMARY.md#new-feature-3-comparison-report-generator) |
| **Multi-line Comment Fix** | 99% false positive reduction (3000+ → 28) | [CHANGELOG.md](CHANGELOG.md#enhanced-static-analyzer) |
| **6 Report Types** | HTML, Excel, KB, JSON, History, Cross-Module | [STRUCTURE.md](STRUCTURE.md#reports-reports) |

**Key Benefits:**
- 📊 Track violations over time (improving ↓, worsening ↑, stable →)
- 🧠 Learn from how teams resolve violations
- 🔍 Cross-module intelligence and pattern recognition
- 📈 Confidence-based recommendations (e.g., "85% of teams suppress this")
- 🎯 Timeline and matrix visualizations

---

## 🤖 AI Configuration

| Document | Purpose | When to Read |
|----------|---------|--------------|
| [AI_MODES.md](AI_MODES.md) | AI mode selection guide | Choosing AI vs rules |
| [OLLAMA_SETUP.md](OLLAMA_SETUP.md) | Ollama installation guide | Setting up local AI |
| [AI_QUICKSTART.md](AI_QUICKSTART.md) | Quick AI setup | Fast AI configuration |

**AI Modes Available:**
- **AI Only** - Use Ollama for all violations (requires Ollama)
- **Hybrid** - Smart: AI for complex, rules for standard (recommended)
- **Rules Only** - Only rule-based fixes (no AI needed)

---

## 📊 Static Code Analyzer (v3.0 - Built-in Analysis)

| Document | Purpose | When to Read |
|----------|---------|--------------|
| [QUICKSTART.md](QUICKSTART.md#-static-code-analyzer-v30---built-in-cc-analysis) | Static analyzer quick start | Running built-in analyzer |
| [STRUCTURE.md](STRUCTURE.md#source-code-src) | New source files | Understanding new components |
| [CHANGELOG.md](CHANGELOG.md#300---2026-04-16) | v3.0.0 changes | Complete feature list |

**What It Detects:**
- Runtime errors (null pointer, buffer overflow, div by zero)
- MISRA C violations (20+ rules)
- CERT C issues (15+ rules)
- Code quality problems
- Context-aware with multi-line comment detection

**Output Reports:**
1. Color-coded HTML (RED/ORANGE/GREY)
2. Excel with 11-line code context
3. Knowledge base JSON
4. Raw violations JSON
5. **Module history HTML** (timeline with trends) ⭐ NEW
6. **Cross-module comparison HTML** (matrix view) ⭐ NEW

---

## 📊 CERT & MISRA Focused Reports (v3.0 - NEW!)

| Tool | Purpose | Usage |
|------|---------|-------|
| **Generate_CERT_MISRA_Report.bat** | Compliance-focused HTML reports | `scripts\Generate_CERT_MISRA_Report.bat` |
| **generate_cert_misra_report.py** | Python script for CERT/MISRA reports | `python src\generate_cert_misra_report.py ModuleName` |

**What's New:** Generate beautiful HTML reports focused exclusively on CERT and MISRA violations from your knowledge base. Perfect for compliance reviews and tracking standards adherence!

**Features:**
- Auto-filters CERT and MISRA violations from knowledge base
- Top 10 violations by occurrence for each category
- Severity breakdown statistics
- Status summary (Open/Fixed/Justified)
- Professional styled HTML with responsive design
- Takes only module name as input (uses existing KB)

**Quick Usage:**
```bash
# Using batch file (easiest)
scripts\Generate_CERT_MISRA_Report.bat
# Enter: Tls
# Output: reports\Tls_CERT_MISRA_Report.html

# Or direct Python
python src\generate_cert_misra_report.py Tls
```

**When to use:**
- Preparing compliance reports for code review
- Tracking CERT/MISRA progress over time
- Quick compliance status check
- Presenting to stakeholders/management

---

## 📊 Direct Violation Capture (New!)

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
