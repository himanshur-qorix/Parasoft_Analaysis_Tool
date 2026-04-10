# Project Structure - Parasoft AI Agent v2.2.0

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
│   ├── Run.bat                       # Windows batch launcher
│   └── Apply_Suppressions.bat        # Windows suppress applicator
│
├── 📁 src/                           # Source Code
│   ├── ParasoftAIAgent.py            # Main AI agent orchestrator
│   ├── KnowledgeDatabaseManager.py   # Database management
│   ├── ViolationAnalyzer.py          # Intelligent analysis
│   ├── CodeFixGenerator.py           # Fix generation with AI
│   ├── OllamaIntegration.py          # AI integration (Ollama)
│   ├── KnowledgeDatabaseQueryTool.py # Query tool
│   └── ParasoftAnalysisTool.py       # HTML parser (legacy)
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
│   └── Qorix_CP_Common_Deviations.xlsx
│
├── 📁 knowledge_base/                # Generated Databases
│   ├── Mka_KnowledgeDatabase.json
│   └── ...
│
├── 📁 reports/                       # Analysis Reports
│   ├── Mka_analysis_summary.json
│   ├── Mka_violations_report.xlsx
│   └── ...
│
├── 📁 justifications/                # Suppress Comments
│   ├── Mka_suppress_comments_*.txt
│   └── ...
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

## 🚀 Running the Agent

### From Project Root (Recommended)

#### Using Batch Script (Windows)
```bash
scripts\Run.bat
```

#### Using Python Launcher
```bash
# Main agent
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

### Documentation (docs/)
All markdown documentation consolidated:
- **QUICKSTART.md** - Get started in 30 seconds
- **EXAMPLE_WORKFLOW.md** - Real-world usage examples
- **ARCHITECTURE.md** - System design and architecture
- **CHANGELOG.md** - Version history and changes
- **PROJECT_SUMMARY.md** - Complete project overview
- **INDEX.md** - Documentation navigation

### Configuration (config/)
Configuration files isolated for easy management:
- **config.json** - Agent behavior settings, analysis options, reporting preferences

### Scripts (scripts/)
Automation and helper scripts:
- **Run.bat** - Windows batch automation with user prompts

### Assets (assets/)
Images, diagrams, and visual resources:
- **FlowDiagram_V1.0.0.png** - System flow diagram

### Data (data/)
Reference data files:
- **Qorix_CP_Common_Deviations.xlsx** - MISRA/CERT deviation mappings

### Generated Folders
Created automatically during analysis:
- **knowledge_base/** - Module-specific violation databases
- **reports/** - Analysis summary reports
- **fixes/** - Code fixes and justifications per module

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
