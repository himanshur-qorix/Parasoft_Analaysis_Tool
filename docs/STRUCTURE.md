# Project Structure - Parasoft AI Agent v3.0.0

## 📁 Organized Folder Structure

The project has been reorganized into a clean, professional folder structure:

```
Parasoft_Analaysis_Tool/
│
├── 📁 scripts/                       # Scripts & Launchers
│   ├── run_agent.py                  # Main launcher script
│   ├── run_query.py                  # Query tool launcher
│   ├── apply_suppress_comments.py    # Suppress comment applicator
│   ├── capture_violations.py         # Violation capture utility
│   ├── misra_cert_checker.py         # MISRA/CERT static analyzer
│   ├── run_static_analyzer.py        # Static code analyzer (NEW v3.0)
│   ├── Run.bat                       # Windows batch launcher
│   ├── Run_Static_Analyzer.bat       # Static analyzer launcher (NEW v3.0)
│   ├── Generate_CERT_MISRA_Report.bat # CERT/MISRA report launcher (NEW v3.0)
│   └── Apply_Suppressions.bat        # Windows suppress applicator
│
├── 📁 src/                           # Source Code
│   ├── ParasoftAIAgent.py            # Main AI agent orchestrator
│   ├── KnowledgeDatabaseManager.py   # Database management
│   ├── ViolationAnalyzer.py          # Intelligent analysis
│   ├── CodeFixGenerator.py           # Fix generation with AI
│   ├── OllamaIntegration.py          # AI integration (Ollama)
│   ├── KnowledgeDatabaseQueryTool.py # Query tool
│   ├── ParasoftAnalysisTool.py       # HTML parser (legacy)
│   ├── StaticCodeAnalyzer.py         # Built-in C/C++ static analyzer (NEW v3.0)
│   ├── StaticAnalysisReportGenerator.py  # HTML/Excel report generator (NEW v3.0)
│   ├── ViolationHistoryManager.py    # History tracking & RAG learning (NEW v3.0)
│   ├── ComparisonReportGenerator.py  # Comparison HTML reports (NEW v3.0)
│   └── generate_cert_misra_report.py # CERT/MISRA report generator (NEW v3.0)
│
├── 📁 docs/                          # Documentation
│   ├── QUICKSTART.md                 # Quick start guide
│   ├── OLLAMA_SETUP.md               # AI setup guide
│   ├── EXAMPLE_WORKFLOW.md           # Real-world examples
│   ├── ARCHITECTURE.md               # System architecture
│   ├── CHANGELOG.md                  # Version history
│   ├── PROJECT_SUMMARY.md            # Project summary
│   └── INDEX.md                      # Documentation index
│
├── 📁 config/                        # Configuration
│   └── config.json                   # Agent configuration (AI enabled)
│
├── 📁 assets/                        # Images & Diagrams
│   └── FlowDiagram_V1.0.0.png        # Flow diagram
│
├── 📁 data/                          # Data Files
│   ├── {Module}_violations.json      # Raw violation data
│   └── Qorix_CP_Common_Deviations.xlsx
│
├── 📁 knowledge_base/                # Generated Databases
│   ├── Mka_KnowledgeDatabase.json
│   └── ...
│
├── 📁 reports/                       # Analysis Reports
│   ├── {Module}_static_analysis_report.html    # Color-coded HTML (NEW v3.0)
│   ├── {Module}_violations_report.xlsx         # Excel with context (NEW v3.0)
│   ├── {Module}_history_report.html            # Module timeline (NEW v3.0)
│   ├── cross_module_comparison.html            # Cross-module matrix (NEW v3.0)
│   ├── Mka_analysis_summary.json
│   └── ...
│
├── 📁 history/                       # Violation History (NEW v3.0)
│   ├── master_violation_history.json # All runs, all modules
│   └── resolution_patterns.json      # RAG learning data
│
├── 📁 justifications/                # Suppress Comments
│   ├── Mka_suppress_comments_*.txt
│   └── ...
│
├── 📁 backups/                       # Code Backups (Suppress Applicator)
│   └── parasoft_backups_*/           # Timestamped backup folders
│       └── [Preserved directory structure from source repo]
│
├── 📁 fixes/                         # Generated Fixes
│   ├── Mka/
│   │   ├── Mka_fixes_*.txt
│   │   └── Mka_justifications_*.txt
│   └── ...
│
├── 📄 run_agent.py                   # Main launcher
├── 📄 run_query.py                   # Query tool launcher
├── 📄 README.md                      # Main documentation
├── 📄 requirements.txt               # Python dependencies
└── 📄 .gitignore                     # Git ignore rules
```

---

## 🚀 Running the Tools

### Static Code Analyzer (v3.0 - NEW!)
```bash
# Using batch script (Windows)
scripts\Run_Static_Analyzer.bat

# Direct Python
python scripts\run_static_analyzer.py "path\to\source" ModuleName --no-fixes
```

### Parasoft AI Agent
```bash
# Using batch script (Windows)
scripts\Run.bat

# Using Python launcher
python run_agent.py report_dev1.html Mka

# Query tool
python run_query.py knowledge_base --interactive
```

### Direct Source Execution (Advanced)
```bash
# Add src to Python path and run directly
cd src
python ParasoftAIAgent.py ../report_dev1.html Mka
```

---

## 📂 Folder Descriptions

### Source Code (src/)
Contains all Python source files organized in one location:
- **ParasoftAIAgent.py** - Main entry point, orchestrates all operations
- **KnowledgeDatabaseManager.py** - Manages JSON database operations
- **ViolationAnalyzer.py** - Analyzes violations and categorizes them
- **CodeFixGenerator.py** - Generates fixes and justifications
- **KnowledgeDatabaseQueryTool.py** - Interactive query interface
- **ParasoftAnalysisTool.py** - HTML report parser (from v1.0.0)
- **StaticCodeAnalyzer.py** ⭐ NEW - Built-in C/C++ static analyzer with 20+ rules
- **StaticAnalysisReportGenerator.py** ⭐ NEW - Generates color-coded HTML/Excel reports
- **ViolationHistoryManager.py** ⭐ NEW - Tracks history, RAG learning, trends
- **ComparisonReportGenerator.py** ⭐ NEW - Module timeline & cross-module comparison HTML
- **generate_cert_misra_report.py** ⭐ NEW - Generates HTML reports for CERT/MISRA violations only

### Documentation (docs/)
All markdown documentation consolidated:
- **QUICKSTART.md** - Get started in 30 seconds
- **EXAMPLE_WORKFLOW.md** - Real-world usage examples
- **ARCHITECTURE.md** - System design and architecture
- **CHANGELOG.md** - Version history and changes (including v3.0.0)
- **PROJECT_SUMMARY.md** - Complete project overview
- **INDEX.md** - Documentation navigation

### Configuration (config/)
Configuration files isolated for easy management:
- **config.json** - Agent behavior settings, analysis options, reporting preferences

### Scripts (scripts/)
Automation and helper scripts:
- **Run.bat** - Windows batch automation with user prompts
- **Run_Static_Analyzer.bat** ⭐ NEW - Static code analyzer launcher
- **Generate_CERT_MISRA_Report.bat** ⭐ NEW - CERT/MISRA focused report generator (calls src/generate_cert_misra_report.py)
- **run_static_analyzer.py** ⭐ NEW - Static analyzer orchestrator
- **misra_cert_checker.py** - MISRA/CERT compliance checker

### Assets (assets/)
Images, diagrams, and visual resources:
- **FlowDiagram_V1.0.0.png** - System flow diagram

### Data (data/)
Reference data files and raw violation data:
- **Qorix_CP_Common_Deviations.xlsx** - MISRA/CERT deviation mappings
- **{Module}_violations.json** - Raw violation data per module

### Reports (reports/)
Generated analysis reports:
- **{Module}_static_analysis_report.html** ⭐ NEW - Color-coded violations (RED/ORANGE/GREY)
- **{Module}_violations_report.xlsx** ⭐ NEW - Excel with 11-line code context
- **{Module}_history_report.html** ⭐ NEW - Timeline view with trends (↓↑→)
- **cross_module_comparison.html** ⭐ NEW - Cross-module violation matrix
- **{Module}_analysis_summary.json** - Analysis metadata

### History (history/) ⭐ NEW - v3.0.0
Violation history tracking and RAG learning data:
- **master_violation_history.json** - Complete history of all analysis runs across all modules
  - Timestamped runs with violation counts
  - Severity breakdown per run
  - Top violations tracking
  - Trend calculations (improving/worsening/stable)
  - Cross-module violation patterns
- **resolution_patterns.json** - RAG learning database
  - How violations are resolved (suppressed/fixed/justified/open)
  - Confidence scores for recommendations
  - Common resolution notes
  - Status counts per violation type

### Generated Folders
Created automatically during analysis:
- **knowledge_base/** - Module-specific violation databases
- **fixes/** - Code fixes and justifications per module
- **justifications/** - Suppress comment suggestions
- **backups/** - Source code backups before applying suppressions

---

## 🔧 Launcher Scripts

### run_agent.py
Main launcher that:
- Adds src/ to Python path automatically
- Resolves workspace paths correctly
- Calls ParasoftAIAgent.main()

### run_query.py
Query tool launcher that:
- Adds src/ to Python path automatically
- Calls KnowledgeDatabaseQueryTool.main()

**Benefits:**
- Clean command-line interface
- No need to specify paths
- Works from project root
- Backward compatible

---

## 📝 Import Resolution

The launcher scripts automatically handle Python imports:

```python
# run_agent.py structure
import sys
from pathlib import Path

# Add src directory to path
src_dir = Path(__file__).parent / "src"
sys.path.insert(0, str(src_dir))

# Now imports work correctly
from ParasoftAIAgent import main
```

This ensures:
✅ Clean imports in source files
✅ No relative import issues
✅ Works from any directory
✅ Professional project structure

---

## 🎯 Benefits of New Structure

### Organization
- Clear separation of concerns
- Easy to find any file
- Professional project layout
- Scalable for growth

### Maintainability
- Source code isolated
- Documentation centralized
- Configuration separate
- Generated files distinct

### Usability
- Simple launcher scripts
- Consistent commands
- Clear documentation path
- Easy onboarding

### Development
- Clean imports
- No path confusion
- Easy testing
- Simple debugging

---

## 🔄 Migration Notes

### Old Command → New Command

| Old | New |
|-----|-----|
| `python ParasoftAIAgent.py` | `python run_agent.py` |
| `python KnowledgeDatabaseQueryTool.py` | `python run_query.py` |
| `Run.bat` | `scripts\Run.bat` |
| Root folder clutter | Organized folders |

### File Locations

| File Type | Old Location | New Location |
|-----------|--------------|--------------|
| Python files | Root | `src/` |
| Documentation | Root | `docs/` |
| Config | Root | `config/` |
| Scripts | Root | `scripts/` |
| Assets | Root | `assets/` |
| Data files | Root | `data/` |

---

## ✅ Verification

To verify the structure is correct:

```bash
# List source files
dir src\*.py

# List documentation
dir docs\*.md

# Test main agent
python run_agent.py --help

# Test query tool
python run_query.py --help
```

---

## 🆕 Updated Commands

### Analysis Commands
```bash
# Basic analysis
python run_agent.py report_dev1.html Mka

# With options
python run_agent.py report_dev1.html Mka --no-fixes
python run_agent.py report_dev1.html Mka --workspace D:/MyProject
```

### Query Commands
```bash
# Interactive mode
python run_query.py knowledge_base --interactive

# Export to Excel
python run_query.py knowledge_base --excel violations.xlsx

# Summary report
python run_query.py knowledge_base --summary report.json
```

---

## 📚 Documentation Access

All documentation now in `docs/` folder:
- Read: `type docs\QUICKSTART.md` (Windows) or `cat docs/QUICKSTART.md` (Unix)
- Open: Navigate to `docs/` and open any `.md` file

Main README still at root for GitHub/GitLab display.

---

## 🎉 Summary

✅ Clean folder structure
✅ Source code organized in `src/`
✅ Documentation in `docs/`
✅ Configuration in `config/`
✅ Scripts in `scripts/`
✅ Assets in `assets/`
✅ Data in `data/`
✅ Simple launcher scripts at root
✅ All imports working correctly
✅ Documentation updated
✅ Backward compatible commands

**The project is now professionally organized and ready for team collaboration!**
