# Parasoft AI Analysis Tool

**AI-powered agent for Parasoft analysis with intelligent knowledge database management**

## Version
- **2.0.0** - AI Agent with Knowledge Database
- Version 1.0.0 - Basic Report Analysis
- Developer: Himanshu R
- Platform: Windows
- CodeSet: Python

---

## 🚀 Features

### Core Capabilities
1. **Git Integration** - Seamlessly integrated with Visual Studio Code and Git repositories
2. **Intelligent Analysis** - AI-powered analysis of report_dev1 reports for each module
3. **Knowledge Database** - Automatic creation and update of module-specific knowledge databases
4. **Unique Violation Tracking** - Identifies and tracks unique violations across analyses
5. **Code Fix Generation** - Provides automated fix suggestions for violations
6. **Parasoft Justifications** - Generates Parasoft-formatted justification comments
7. **Query Tool** - Comprehensive tool to analyze and export knowledge database

### Knowledge Database Features
- **Module-Specific Storage**: Each module gets its own `{Module}_KnowledgeDatabase.json`
- **Automatic Updates**: Incremental updates for existing databases
- **Historical Tracking**: Tracks violation history, occurrences, and trends
- **Smart Categorization**: Automatic severity and category classification
- **Fix Tracking**: Tracks which violations have been fixed or justified

---

## 📋 Prerequisites

- Python 3.8 or higher
- Git (for repository integration)
- Visual Studio Code (recommended)

---

## 🛠️ Installation

### Step 1: Clone or Download the Repository
```bash
git clone <repository-url>
cd Parasoft_Analaysis_Tool
```

### Step 2: Install Dependencies
```bash
# Option 1: Using pip directly
pip install -r requirements.txt

# Option 2: Using virtual environment (recommended)
python -m venv venv
venv\Scripts\activate  # On Windows
pip install -r requirements.txt
```

### Step 3: Configure (Optional)
Edit `config.json` to customize agent behavior:
- Analysis settings
- Git integration preferences
- Reporting options
- AI integration (if available)

---

## 📖 How to Use

### Method 1: Quick Start with Run.bat (Windows)

1. **Place your report**: Copy `report_dev1.html` from Parasoft to the project root folder
2. **Run the batch file**: Double-click `scripts\Run.bat` (or run from project root)
3. **Enter module name**: When prompted, enter the module name (e.g., "Mka")
4. **Review results**: Check the generated directories for outputs

### Method 2: Command Line Usage

#### Basic Analysis
```bash
python run_agent.py <report_path> <module_name>
```

**Example:**
```bash
python run_agent.py report_dev1.html Mka
```

#### Advanced Options
```bash
# Skip code fix generation
python run_agent.py report_dev1.html Mka --no-fixes

# Skip justification generation
python run_agent.py report_dev1.html Mka --no-justifications

# Specify custom workspace
python run_agent.py report_dev1.html Mka --workspace D:/MyProject

# Combine options
python run_agent.py report_dev1.html Mka --no-justifications --workspace D:/MyProject
```

---

## 📂 Project Structure

The project is organized into the following folders:

```
Parasoft_Analaysis_Tool/
├── src/                          # Python source code
│   ├── ParasoftAIAgent.py
│   ├── KnowledgeDatabaseManager.py
│   ├── ViolationAnalyzer.py
│   ├── CodeFixGenerator.py
│   ├── KnowledgeDatabaseQueryTool.py
│   └── ParasoftAnalysisTool.py
│
├── docs/                         # Documentation
│   ├── QUICKSTART.md
│   ├── EXAMPLE_WORKFLOW.md
│   ├── ARCHITECTURE.md
│   ├── CHANGELOG.md
│   ├── PROJECT_SUMMARY.md
│   └── INDEX.md
│
├── config/                       # Configuration files
│   └── config.json
│
├── scripts/                      # Automation scripts
│   └── Run.bat
│
├── assets/                       # Images and diagrams
│   └── FlowDiagram_V1.0.0.png
│
├── data/                         # Data files
│   └── Qorix_CP_Common_Deviations.xlsx
│
├── knowledge_base/              # Generated knowledge databases
│   ├── Mka_KnowledgeDatabase.json
│   ├── ModuleB_KnowledgeDatabase.json
│   └── ...
│
├── reports/                     # Analysis reports
│   ├── Mka_analysis_summary.json
│   └── ...
│
├── fixes/                       # Generated fixes
│   ├── Mka/
│   │   ├── Mka_fixes_*.txt
│   │   └── Mka_justifications_*.txt
│   └── ...
│
├── run_agent.py                 # Main launcher script
├── run_query.py                 # Query tool launcher
├── requirements.txt             # Python dependencies
├── .gitignore                   # Git ignore rules
└── README.md                    # This file
```

### Output Structure

After running the analysis, output files are created in:

```
knowledge_base/                  # Module-specific databases
├── Mka_KnowledgeDatabase.json
├── ModuleB_KnowledgeDatabase.json
└── ...

reports/                         # Analysis summaries
├── Mka_analysis_summary.json
└── ...

fixes/                           # Fix suggestions
├── Mka/
│   ├── Mka_fixes_20260409_143022.txt
│   └── Mka_justifications_20260409_143025.txt
└── ...
```

### Knowledge Database Format
Each `{Module}_KnowledgeDatabase.json` contains:
- Unique violations with detailed metadata
- Occurrence counts and file locations
- Severity and category classifications
- Fix and justification status
- Analysis history and statistics

---

## 🔍 Querying Knowledge Database

Use the Knowledge Database Query Tool to analyze violations:

### Interactive Mode
```bash
python run_query.py knowledge_base --interactive
```

**Available Commands:**
1. List all modules
2. Get module summary
3. Query by severity (HIGH/MEDIUM/LOW)
4. Query by category (CERT/MISRA/CWE/OTHER)
5. Query unfixed violations
6. Query unjustified violations
7. Query by file name
8. Get top violations
9. Generate summary report
10. Export to Excel

### Generate Summary Report
```bash
python run_query.py knowledge_base --summary report.json
```

### Export to Excel
```bash
# Export all modules
python run_query.py knowledge_base --excel violations.xlsx

# Export specific module
python run_query.py knowledge_base --excel violations.xlsx --module Mka
```

---

## 📊 Understanding the Analysis

### Violation Severity Levels
- **HIGH**: Critical issues requiring immediate attention
  - Security vulnerabilities
  - Memory safety issues
  - CERT critical rules
  - Required MISRA rules

- **MEDIUM**: Important issues that should be addressed
  - Standard compliance violations
  - Code quality issues
  - Advisory MISRA rules

- **LOW**: Minor issues and recommendations
  - Style guidelines
  - Optimization suggestions
  - Advisory rules

### Violation Categories
- **CERT**: CERT Secure Coding Standards
- **MISRA**: MISRA C/C++ Guidelines
- **CWE**: Common Weakness Enumeration
- **OTHER**: Other coding standards and custom rules

---

## 🔧 Advanced Features

### Git Integration

The agent automatically integrates with your Git repository:
- Detects Git repository status
- Tracks analysis per Git commit
- Records Git branch and commit info in knowledge database

**Ensure your workspace is a Git repository:**
```bash
git init  # If not already initialized
git remote add origin <repository-url>
```

### AI-Powered Analysis

The agent includes intelligent analysis features:
- **Security Detection**: Identifies security-related violations
- **Memory Safety**: Detects memory management issues
- **Threading Issues**: Recognizes concurrency problems
- **Fix Complexity Estimation**: Estimates effort required for fixes

### Automatic Fix Generation

Fixes are generated based on violation type:
- **Explicit Casting**: For type conversion violations
- **Buffer Safety**: For buffer overflow issues
- **Null Checks**: For pointer dereference violations
- **Memory Management**: For allocation/deallocation issues

### Parasoft Justification Format

Generated justifications follow Parasoft standards:
```c
/* parasoft-begin-suppress RULE_ID "Reason for suppression" */
/* 
 * Justification: [Detailed reason]
 * Reviewed by: [Developer Name]
 * Date: YYYY-MM-DD
 * Ticket/Issue: [Reference]
 */
... your code here ...
/* parasoft-end-suppress RULE_ID */
```

---

## 📈 Workflow Integration

### Recommended Workflow

1. **Initial Analysis**
   ```bash
   python run_agent.py report_dev1.html Mka
   ```

2. **Query High Priority Issues**
   ```bash
   python run_query.py knowledge_base --interactive
   # Then select option 3 and choose "HIGH" severity
   ```

3. **Review Generated Fixes**
   - Check `fixes/Mka/` directory for fix suggestions
   - Review and apply fixes to your code

4. **Add Justifications**
   - Review `fixes/Mka/*_justifications_*.txt`
   - Apply justifications to code where fixes are not applicable

5. **Continuous Analysis**
   - Run analysis after code changes
   - Knowledge database automatically updates with new violations

6. **Export Reports**
   ```bash
   python run_query.py knowledge_base --excel status_report.xlsx
   ```

---

## 🐛 Troubleshooting

### Common Issues

**Issue: "Git is not installed or not in PATH"**
- **Solution**: Install Git from https://git-scm.com/ and add to PATH

**Issue: "Module not found" errors**
- **Solution**: Install dependencies with `pip install -r requirements.txt`

**Issue: "No violations found in the report"**
- **Solution**: Verify the report_dev1.html file is a valid Parasoft report

**Issue: "Permission denied" when creating directories**
- **Solution**: Run with appropriate permissions or change workspace location

---

## 📝 Configuration Options

Edit `config.json` to customize:

```json
{
  "analysis": {
    "auto_generate_fixes": true,          // Generate fixes automatically
    "auto_generate_justifications": false, // Generate justifications
    "severity_threshold": "MEDIUM"         // Minimum severity to process
  },
  "git_integration": {
    "enabled": true,                       // Enable Git integration
    "track_commits": true                  // Track Git commits in KB
  },
  "knowledge_database": {
    "auto_save": true,                     // Auto-save after analysis
    "backup_enabled": true,                // Create backups
    "max_backup_count": 5                  // Maximum backup files
  }
}
```

---

## 🔮 Future Enhancements

- AI integration with OpenAI/Anthropic for advanced analysis
- Automated code fix application
- Team collaboration features
- Web-based dashboard
- Integration with CI/CD pipelines
- Custom rule definitions

---

## 📄 License

Copyright © 2026 Himanshu R
All rights reserved.

---

## 🤝 Support

For issues, questions, or contributions:
1. Check existing documentation
2. Review log files (`parasoft_agent.log`)
3. Contact: [Your Contact Information]

---

## 📚 Additional Resources

### Understanding Parasoft Reports
- Parasoft Official Documentation: https://docs.parasoft.com/
- MISRA Guidelines: https://www.misra.org.uk/
- CERT Secure Coding: https://wiki.sei.cmu.edu/confluence/display/seccode

### Python Development
- Python Documentation: https://docs.python.org/
- pandas Documentation: https://pandas.pydata.org/docs/
- BeautifulSoup Documentation: https://www.crummy.com/software/BeautifulSoup/bs4/doc/

---

**Happy Coding! 🚀** 