<div align="center">
  <img src="Images/logo.svg" alt="Qorix Logo" width="200"/>
  <h1>Parasoft AI Analysis Tool</h1>
  <p><strong>AI-powered agent for Parasoft analysis with intelligent knowledge database management</strong></p>
  <p>Developed by <strong>Qorix India Pvt Ltd</strong></p>
</div>

---

## 📖 Documentation

For comprehensive documentation, see the [`docs/`](docs/) folder:

- **[Fix Viewing Options](docs/FIX_VIEWING_OPTIONS.md)** - Interactive viewer, HTML reports, text files (NEW! v2.3)
- **[Parasoft Rules Database](docs/PARASOFT_RULES_DATABASE.md)** - Official Parasoft-approved fix suggestions (NEW! v2.3)
- **[Knowledge Base Integration](docs/KNOWLEDGE_BASE_INTEGRATION.md)** - Smart analysis with KB insights (NEW!)
- **[Cross-Module Knowledge Base](docs/CROSS_MODULE_KNOWLEDGE.md)** - Consolidate learnings across modules
- **[Auto-Generate Workflow](docs/AUTO_GENERATE_WORKFLOW.md)** - Use without Parasoft report
- **[Quick Start Guide](docs/AI_QUICKSTART.md)** - Get started in 5 minutes
- **[MISRA/CERT Integration](docs/MISRA_CERT_INTEGRATION.md)** - Static analyzer integration
- **[Architecture Diagram](docs/ARCHITECTURE_DIAGRAM.md)** - System architecture and workflow
- **[Project Structure](docs/STRUCTURE.md)** - Complete folder organization
- **[Reorganization Summary](docs/REORGANIZATION_SUMMARY.md)** - Latest structural changes
- **[Documentation Index](docs/INDEX.md)** - Complete documentation catalog

---

## 📌 Version Information

| Version | Release | Features |
|---------|---------|----------|
| **2.3.0** | Jan 2025 | Parasoft Rules Database - Official fix suggestions from 1200+ rules |
| 2.2.1 | Apr 2026 | Knowledge Base Integration for Analyzers |
| 2.2.0 | Apr 2026 | Cross-Module Knowledge Base & HTML Reports |
| 2.1.1 | Apr 2026 | Auto-Generate Workflow |
| 2.1.0 | Apr 2026 | MISRA/CERT Static Analyzer Integration |
| 2.0.0 | Apr 2026 | AI Agent with Knowledge Database |
| 1.0.0 | Mar 2026 | Basic Report Analysis |

**Developer:** Himanshu R  
**Organization:** Qorix India Pvt Ltd  
**Platform:** Windows  
**Language:** Python 3.8+

---

## 🚀 Features

### Core Capabilities
1. **Interactive Fix Viewer (NEW! v2.3)** - Keyboard-driven terminal interface for browsing fixes with navigation, search, and color coding
2. **HTML Fix Reports (NEW! v2.3)** - Professional web reports with Qorix branding, filtering, and syntax highlighting
3. **Parasoft Rules Database (v2.3)** - Leverages 1200+ official Parasoft rule documentation files for authoritative fix suggestions and justifications
4. **Smart Knowledge Base Integration** - Analyzers cross-check violations against KB for known vs new detection and proven fix suggestions
5. **Cross-Module Knowledge Base** - Consolidate learnings across all modules for intelligent insights and proven fix recommendations
6. **Auto-Generate Workflow** - Automatically generate MISRA/CERT report when Parasoft report unavailable
7. **MISRA/CERT Pre-Analysis** - Integrated MISRA-C:2012 and CERT-C static analyzer for comprehensive code checking
8. **Git Integration** - Seamlessly integrated with Visual Studio Code and Git repositories
9. **AI-Powered Analysis** - Local LLM integration via Ollama for intelligent fix suggestions
10. **Qorix Deviations Integration** - Automatically checks violations against Qorix_CP_Common_Deviations.xlsx
11. **Knowledge Database** - Automatic creation and update of module-specific knowledge databases
12. **Excel Report Generation** - Automatically generates comprehensive Excel reports with violation status
13. **Parasoft Suppress Comments** - Generates properly formatted suppress comments for justified violations
14. **Interactive Code Application** - Apply suppress comments to source code with user approval for each change
15. **Unique Violation Tracking** - Identifies and tracks unique violations across analyses
16. **Hybrid Fix Generation** - AI for complex cases, rule-based for standard violations (optimal performance)
17. **Parasoft Justifications** - Generates Parasoft-formatted justification comments
18. **Query Tool** - Comprehensive tool to analyze and export knowledge database

### MISRA/CERT Integration (New!)
- **Pattern-Based Detection**: 20+ MISRA-C:2012 and 15+ CERT-C rules
- **Pre-Analysis Step**: Runs before Parasoft analysis for early violation detection
- **Multiple Report Formats**: HTML, JSON, or text outputs
- **Optional Integration**: Can be enabled/disabled via config or command line
- **Comprehensive Coverage**: Control flow, memory safety, type safety, and more
- **Detailed Reports**: Rule descriptions, rationale, severity, and code snippets

### AI Integration (Ollama)
- **Lightweight & Local**: Runs small models (2-4GB) with minimal resource usage
- **Zero Cost**: No API fees, completely free
- **Data Privacy**: All analysis happens on your machine
- **Offline Capable**: Works without internet connection
- **Smart Hybrid Mode**: Uses AI only for complex violations, rules for standard cases
- **Auto Fallback**: Seamlessly falls back to rule-based if AI unavailable

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

### Step 4: AI Setup with Ollama (Optional - Recommended)

**Ollama** provides local LLM integration for enhanced fix suggestions with **zero cost** and **data privacy**.

#### Why Ollama?
- ✅ **Lightweight**: Small models (~2-4GB) with low CPU/RAM usage
- ✅ **No API Costs**: Runs locally, no cloud fees
- ✅ **Data Privacy**: All code stays on your machine
- ✅ **Offline**: Works without internet connection
- ✅ **Simple Setup**: 3 commands to get running

#### Installation Steps

**1. Install Ollama**
```bash
# Windows (download installer from ollama.com)
# Or use winget:
winget install Ollama.Ollama

# Linux / WSL
curl -fsSL https://ollama.com/install.sh | sh

# macOS
brew install ollama
```

**2. Pull a Lightweight Model**

For low resource consumption, use one of these models:

```bash
# Option 1: Qwen 2.5 Coder 1.5B (Recommended - Code-specialized, ~900MB)
ollama pull qwen2.5-coder:1.5b-base

# Option 2: Phi3 Mini (3.8B params, ~2.3GB)
ollama pull phi3:mini

# Option 3: Qwen 2.5 3B (~2GB, general purpose)
ollama pull qwen2.5:3b
```

**3. Start Ollama Server**
```bash
# Start the server (runs in background)
ollama serve
```

**4. Verify Installation**
```bash
# Test the model
ollama run phi3:mini "Write a hello world in C"
```

**5. Enable in Config**

The tool is pre-configured for Ollama! Check `config/config.json`:
```json
{
  "ai_integration": {
    "enabled": true,
    "provider": "ollama",
    "ollama": {
      "base_url": "http://localhost:11434",
      "model": "qwen2.5-coder:1.5b-base",
      "fallback_to_rules": true
    }
  }
}
```

**To disable AI:** Set `"enabled": false` in config.json

#### Resource Requirements

| Model | Size | RAM | Speed | Quality |
|-------|------|-----|-------|---------|
| qwen2.5-coder:1.5b-base | 900MB | 2GB | Very Fast | Excellent for code |
| phi3:mini | 2.3GB | 4GB | Fast | Good |
| qwen2.5:3b | 2.0GB | 4GB | Fast | Excellent |
| llama3.2:3b | 2.0GB | 4GB | Fast | Good |

**Recommended for Teams:** `qwen2.5-coder:1.5b-base` (fastest, code-optimized)

#### How It Works

The tool uses AI **only when beneficial**:
- ✅ Complex/unknown violations → AI analysis
- ✅ Ambiguous patterns → AI suggestions
- ❌ Standard MISRA/CERT rules → Fast rule-based fixes
- ❌ Ollama unavailable → Automatic fallback to rules

This **hybrid approach** keeps resource usage low while providing AI benefits where needed.

---

## 📖 How to Use

### Method 1: Quick Start with Run.bat (Windows)

**Option A: With Parasoft Report**
1. **Place your report**: Copy `report_dev1.html` from Parasoft to the project root folder
2. **Run**: Double-click `scripts\Run.bat`
3. **Follow prompts**: Enter module name and options

**Option B: Generate Report from Source Code (New!)**
1. **No Parasoft report needed**: Skip placing report_dev1.html
2. **Run**: Double-click `scripts\Run.bat`
3. **Provide source path**: When prompted, enter path to your C/C++ source code
4. **Automatic generation**: Tool generates MISRA/CERT report and uses it for analysis

The tool automatically:
- Detects if report_dev1.html exists
- If not found but source code path provided → generates MISRA/CERT report
- Uses the generated report for complete analysis workflow
- Applies Qorix deviations and creates knowledge database
2. **Run the batch file**: Double-click `scripts\Run.bat` (or run from project root)
3. **Enter module name**: When prompted, enter the module name (e.g., "Mka")
4. **Review results**: Check the generated directories for outputs

### Method 2: Command Line Usage

#### Basic Analysis
```bash
python src\run_agent.py <report_path> <module_name>
```

**Example:**
```bash
python src\run_agent.py report_dev1.html Mka
```

#### Advanced Options
```bash
# Skip justification generation
python src\run_agent.py report_dev1.html Mka --no-justifications

# Specify custom workspace
python src\run_agent.py report_dev1.html Mka --workspace D:/MyProject

# Specify custom Qorix deviations file
python src\run_agent.py report_dev1.html Mka --qorix custom_deviations.xlsx

# Combine options
python src\run_agent.py report_dev1.html Mka --no-justifications --workspace D:/MyProject --qorix data/Qorix.xlsx
```

**Note:** Code fix generation is now a separate step (see Step 2 below).

---

## � Complete Workflow

**Important:** The workflow is now separated into two distinct steps for better clarity and control:

### Step 1: Run Analysis (NO Code Fixes)
```bash
# Option A: Using batch file (recommended for Windows)
scripts\Run.bat

# Option B: Using command line
python src\run_agent.py report_dev1.html Mka
```

**What it does:**
- Parses Parasoft report or generates MISRA/CERT report from source code
- Creates/updates knowledge database
- Generates Excel report with violation status (Justified/Needs Code Update/Analysis Required)
- Creates suppress comments file for justified violations
- Adds justification comments to code
- **Does NOT generate code fixes** (separate step below)

**Outputs:**
- `knowledge_base/{Module}_KnowledgeDatabase.json` - Analysis data
- `reports/{Module}_violations_report.xlsx` - Excel summary with status
- `justifications/{Module}_suppress_comments_{timestamp}.txt` - Suppression comments
- Analysis summary and statistics

### Step 2: Generate Code Fix Suggestions (Separate Step)
```bash
# Generate fixes using the knowledge database from Step 1
scripts\Generate_Code_Fixes.bat

# Try different AI modes:
# - Hybrid (recommended): Parasoft DB → AI → Rules
# - AI Only: Pure AI suggestions  
# - Rules Only: Parasoft DB + Patterns (no AI)
```

**What it does:**
- Uses existing knowledge database (from Step 1)
- Generates code fix suggestions with priority: Parasoft DB → AI → Pattern-based
- Creates both text and HTML fix reports
- Offers interactive viewing options
- Much faster than full re-analysis (10-30 seconds vs 2-5 minutes)

**When to use:**
- After completing Step 1 analysis
- Want to try different AI modes for comparison
- After building/updating Parasoft Rules Database
- Need to regenerate fixes without re-running full analysis

**Outputs:**
- `fixes/{Module}/{Module}_fixes_{timestamp}.txt` - Text format with all fixes
- `fixes/{Module}/{Module}_fixes_{timestamp}.html` - Professional HTML report with filtering
- Interactive viewer option for browsing fixes
- Before/after code snippets with security relevance and CWE mappings

### Step 3: View Code Fixes (Multiple Options)
```bash
# After Step 2, view the generated fixes:

# Option 1: Interactive Terminal Viewer (recommended)
python src\view_fixes_interactive.py fixes\Mka\Mka_fixes_*.txt
# - Keyboard navigation (Arrow keys, A/D)
# - Search violations (S key)
# - Color-coded display
# - Open HTML report (H key)

# Option 2: HTML Report in Browser
# Open: fixes\Mka\Mka_fixes_*.html
# - Filterable by priority, type, search
# - Syntax highlighting
# - Qorix branding
# - Collapsible fix cards

# Option 3: Text File
# Open: fixes\Mka\Mka_fixes_*.txt in any text editor
```

**See [FIX_VIEWING_OPTIONS.md](docs/FIX_VIEWING_OPTIONS.md) for complete guide on viewing options.**

### Step 4: Apply Suppress Comments (Interactive)
```bash
# Option A: Using batch file (easiest)
scripts\Apply_Suppressions.bat

# Option B: Using command line
python src\apply_suppress_comments.py justifications\Mka_suppress_comments_20260410_143022.txt D:\Path\To\SourceCode
```

**Process:**
1. Tool shows preview of each suppression
2. You review the code context
3. Choose: `y` (apply), `n` (skip), `a` (apply all), `q` (quit)
4. Automatic backup created for each modified file in `backups/` folder (outside source code)

**Example Session:**
```
File: Mka_Internal.c, Line: 3454
Current code:
    3451:     uint8_t result;
    3452:     result = calculate_value(param1, param2);
>>> 3453:     return (int16_t)result;
    3454: }

Proposed changes:
     ADD: /* parasoft-begin-suppress CERT_C-INT02-b-2 "Reason: Mka_Parasoft_REF_3453" */
     3453:     return (int16_t)result;
     ADD: /* parasoft-end-suppress CERT_C-INT02-b-2 */

Apply this suppression? (y=yes, n=no, a=yes to all, q=quit): y
[SUCCESS] Suppression applied successfully
[INFO] Backup created: src/Mka_Internal.c
```

### Step 5: Review and Commit
1. Review modified files in your source code
2. Check backup folder (`parasoft_backups_YYYYMMDD_HHMMSS/`) if you need to revert
3. Test your code
4. Commit changes to Git

---

## �📂 Project Structure

The project is organized into the following folders:

```
Parasoft_Analaysis_Tool/
├── src/                          # Python source code
│   ├── ParasoftAIAgent.py
│   ├── KnowledgeDatabaseManager.py
│   ├── ViolationAnalyzer.py
│   ├── CodeFixGenerator.py
│   ├── ParasoftRulesParser.py
│   ├── generate_code_fixes.py    # Code fix generation utility
│   ├── view_fixes_interactive.py # Interactive fix viewer
│   ├── run_agent.py
│   └── ...
│
├── docs/                         # Documentation
│   ├── QUICKSTART.md
│   ├── FIX_VIEWING_OPTIONS.md
│   ├── PARASOFT_RULES_DATABASE.md
│   ├── EXAMPLE_WORKFLOW.md
│   ├── ARCHITECTURE.md
│   ├── CHANGELOG.md
│   ├── PROJECT_SUMMARY.md
│   └── INDEX.md
│
├── config/                       # Configuration files
│   └── config.json
│
├── scripts/                      # Batch automation scripts (Windows)
│   ├── Run.bat                   # Main analysis workflow (NO code fixes)
│   ├── Generate_Code_Fixes.bat   # Generate code fix suggestions (SEPARATE)
│   ├── Apply_Suppressions.bat    # Interactive suppress comment applicator
│   ├── Build_Parasoft_Rules_Database.bat  # Build official rules DB
│   ├── Consolidate_Knowledge.bat  # Merge module knowledge bases
│   └── Query_Master_Knowledge.bat # Query consolidated insights
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
├── apply_suppress_comments.py   # Interactive suppress comment applicator
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

reports/                         # Analysis summaries and Excel reports
├── Mka_analysis_summary.json
├── Mka_violations_report.xlsx  # Excel report with violations
├── Mka_violations_report_UPDATED.xlsx  # Excel report with justifications added
└── ...

justifications/                  # Generated suppress comments
├── Mka_suppress_comments_20260410_143022.txt
└── ...

fixes/                           # Fix suggestions
├── Mka/
│   ├── Mka_fixes_20260409_143022.txt
│   └── Mka_justifications_20260409_143025.txt
└── ...
```

### Excel Report Contents
The generated Excel report (`{Module}_violations_report.xlsx`) contains:
- **Summary Sheet**: Analysis statistics and metrics including:
  - Total violations count
  - Unique violation types
  - Files affected
  - Status breakdown (Justified, Needs Code Update, Analysis Required)
- **Unique Violations Sheet**: Count of each violation type (sorted by frequency)
- **Detailed Violations Sheet**: All violations with:
  - Violation description
  - Violation ID
  - File name
  - Line number
  - **Status** (Justified / Needs Code Update / Analysis Required)

The **updated Excel report** (`{Module}_violations_report_UPDATED.xlsx`) is generated after adding justifications and includes:
- All sheets from the original report
- **Additional "Justification" column** in Detailed Violations sheet showing:
  - "Yes - [justification text]" for violations with justifications
  - "No" for violations without justifications

### Parasoft Suppress Comments
For violations marked as "Justified" in the Qorix deviations file, the tool automatically generates suppress comments in the format:
```c
/* parasoft-begin-suppress CERT_C-INT02-b-2 CERT_C-EXP14-a-3 "Reason: Module_Parasoft_REF_123" */
... (your code at line 123) ...
/* parasoft-end-suppress CERT_C-INT02-b-2 CERT_C-EXP14-a-3 */
```
These comments are saved in `justifications/{Module}_suppress_comments_{timestamp}.txt`

### Interactive Suppress Comment Application
The tool includes an interactive applicator to insert suppress comments directly into your source code:

**Usage:**
```bash
# Run the interactive batch file
scripts\Apply_Suppressions.bat

# Or use Python directly
python src\apply_suppress_comments.py justifications\Module_suppress_comments_timestamp.txt D:\Path\To\SourceCode
```

**Features:**
- Shows preview of each change before applying
- Interactive: approve/reject each suppression individually
- Automatic backup creation in dedicated folder (`backups/parasoft_backups_timestamp/`)
- **Backups stored outside source repository** to avoid search conflicts
- Options: y=apply, n=skip, a=apply all, q=quit
- Searches for files recursively in your repository
- Handles multiple files with the same name
- **Smart detection**: Automatically skips lines inside comment blocks (prevents incorrect placement)

**Important Notes:**
- The tool will **NOT** insert suppress comments inside comment blocks (`/* ... */` or `/****...****/`)
- If a violation is reported on a line that's inside a comment, it will be auto-skipped
- This prevents incorrect placements like adding suppressions inside function header comments
- Such violations should be addressed in the actual code, not in documentation

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
python src\run_query.py knowledge_base --interactive
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
python src\run_query.py knowledge_base --summary report.json
```

### Export to Excel
```bash
# Export all modules
python src\run_query.py knowledge_base --excel violations.xlsx

# Export specific module
python src\run_query.py knowledge_base --excel violations.xlsx --module Mka
```

---

## 🌐 Cross-Module Knowledge Base (NEW!)

**Consolidate learnings from all analyzed modules** to create a master knowledge database with intelligent insights.

### Why Use It?

- **Learn from past analyses** - Apply proven fixes from other modules
- **Identify patterns** - Find violations appearing across multiple modules
- **Prioritize work** - Focus on widespread issues first
- **Share knowledge** - Leverage team expertise across projects
- **Track progress** - Measure improvement company-wide

### Usage

#### Step 1: Analyze Multiple Modules
```powershell
# Analyze each module separately
scripts\Run.bat  # Enter: Mka
scripts\Run.bat  # Enter: Mkb
scripts\Run.bat  # Enter: Mkc
```

#### Step 2: Consolidate Knowledge
```powershell
# Consolidate all knowledge databases
scripts\Consolidate_Knowledge.bat
```

**Output:**
- `Master_KnowledgeDatabase.json` - Consolidated knowledge
- `Master_Knowledge_Report.xlsx` - Excel report with insights
- `Master_Knowledge_Report.html` - Interactive HTML report
- Recommendations for priority actions
- Cross-module violation patterns
- Proven fix examples

#### Step 3: Query Master Knowledge
```powershell
# Interactive query tool
scripts\Query_Master_Knowledge.bat
```

**Features:**
- View cross-module violations
- Find proven fixes from other modules
- Get violation insights with fix success rates
- Filter by category, severity
- Export consolidated reports
- View recommendations

### Benefits

```powershell
# Example: Find violations with proven fixes
Query Master Knowledge > Select option 7

==================================================
VIOLATIONS WITH PROVEN FIXES (34 total)
==================================================

1. CERT-EXP34-C
   Fix Success Rate: 100.0%
   Modules with Fixes: 3
   Total Occurrences: 15
   → Apply the same fix pattern!

2. MISRA-C:2012 Rule 21.3
   Fix Success Rate: 66.7%
   Modules with Fixes: 2
   → Review and adapt existing fixes
```

**See [CROSS_MODULE_KNOWLEDGE.md](docs/CROSS_MODULE_KNOWLEDGE.md) for complete guide**

---

## 🧠 Smart Analysis with Knowledge Base (NEW!)

**Every analysis now checks against your knowledge base** to provide intelligent insights.

### What You Get

When running analysis on a module (new or existing):

```powershell
scripts\Run.bat
# Enter module: Mka
# Enter source: D:\MyProject\src

Output with Knowledge Base Integration:

==================================================
KNOWLEDGE BASE SUMMARY
==================================================
  Known Violations (seen before): 42
  New Violations (first time): 23
  Total Violations: 65
  Known with Proven Fixes: 15

  ⚠️  23 NEW violations detected!
      Review these carefully

  ✅ 15 violations have proven fixes available
      Apply these to resolve faster
==================================================
```

### Features

**✅ Automatic Classification**
- **KNOWN** - Violations seen before (green badge)
- **NEW** - First-time violations (red badge)
- **2023 Proven Fixes** - Solutions from previous analyses

**📊 Smart Insights**
```
CERT-EXP34-C
  Previously seen: 5 times
  ✅ Proven fix: Add NULL pointer check before dereference
                 [From Module Mkb - 100% success rate]
```

**🎯 Prioritization**
- Focus on NEW violations first (potential regressions)
- Apply proven fixes to KNOWN violations
- Track violation trends over time

### How It Works

1. **Run analysis** on any module
2. **Tool automatically checks** module KB and master KB
3. **Classifies each violation** as KNOWN or NEW
4. **Suggests proven fixes** for known violations
5. **Enhanced HTML report** shows everything visually

### Enhanced Reports

HTML reports now include:
- Color-coded badges (KNOWN/NEW)
- Inline fix suggestions with source module
- Occurrence counts
- Alert banners for new violations
- Success rates for proven fixes

**See [KNOWLEDGE_BASE_INTEGRATION.md](docs/KNOWLEDGE_BASE_INTEGRATION.md) for complete guide**

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
   python src\run_agent.py report_dev1.html Mka
   ```

2. **Query High Priority Issues**
   ```bash
   python src\run_query.py knowledge_base --interactive
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
   python src\run_query.py knowledge_base --excel status_report.xlsx
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

**Copyright © 2025 Qorix India Pvt Ltd**  
All rights reserved.

This software is proprietary and confidential.  
Developed by Qorix India Pvt Ltd for internal use and authorized clients.

---

## 🤝 Support

**Qorix India Pvt Ltd**

For issues, questions, or contributions:
1. Check existing documentation in [`docs/`](docs/) folder
2. Review log files (`parasoft_agent.log`)
3. Contact: Qorix Support Team
4. Visit: [Qorix India Pvt Ltd](https://qorix.com)

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