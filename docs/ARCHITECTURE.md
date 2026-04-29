# Parasoft AI Agent - Architecture & Design

## System Architecture

```
â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”
â”‚                     PARASOFT AI AGENT v2.0.0                      â”‚
â”‚                  (Intelligent Violation Analysis System)          â”‚
â””â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜

â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”
â”‚   INPUT LAYER       â”‚
â”œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”¤
â”‚ â€¢ report_dev1.html  â”‚ â—„â”€â”€â”€ Parasoft HTML Report
â”‚ â€¢ Module Name       â”‚ â—„â”€â”€â”€ User Input (e.g., "Mka")
â”‚ â€¢ Config.json       â”‚ â—„â”€â”€â”€ Configuration Settings
â””â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”¬â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜
           â”‚
           â–¼
â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”
â”‚                    PROCESSING LAYER                             â”‚
â”œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”¤
â”‚                                                                 â”‚
â”‚  â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”    â”‚
â”‚  â”‚         ParasoftAIAgent.py (Orchestrator)              â”‚    â”‚
â”‚  â”‚  â€¢ Initializes workspace                               â”‚    â”‚
â”‚  â”‚  â€¢ Detects Git repository                              â”‚    â”‚
â”‚  â”‚  â€¢ Coordinates all components                          â”‚    â”‚
â”‚  â”‚  â€¢ Manages workflow                                    â”‚    â”‚
â”‚  â””â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”¬â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜    â”‚
â”‚               â”‚                                                 â”‚
â”‚    â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”¼â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”¬â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”¬â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”     â”‚
â”‚    â”‚          â”‚          â”‚                 â”‚             â”‚     â”‚
â”‚    â–¼          â–¼          â–¼                 â–¼             â–¼     â”‚
â”‚  â”Œâ”€â”€â”€â”€â”€â”€â” â”Œâ”€â”€â”€â”€â”€â”€â” â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”  â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”  â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”  â”‚
â”‚  â”‚ Git  â”‚ â”‚Parse â”‚ â”‚Knowledge â”‚  â”‚ Violation   â”‚  â”‚  Code  â”‚  â”‚
â”‚  â”‚Check â”‚ â”‚Reportâ”‚ â”‚Database  â”‚  â”‚  Analyzer   â”‚  â”‚  Fix   â”‚  â”‚
â”‚  â”‚      â”‚ â”‚      â”‚ â”‚Manager   â”‚  â”‚             â”‚  â”‚Generateâ”‚  â”‚
â”‚  â””â”€â”€â”€â”€â”€â”€â”˜ â””â”€â”€â”€â”¬â”€â”€â”˜ â””â”€â”€â”€â”€â”¬â”€â”€â”€â”€â”€â”˜  â””â”€â”€â”€â”€â”€â”€â”¬â”€â”€â”€â”€â”€â”€â”˜  â””â”€â”€â”€â”¬â”€â”€â”€â”€â”˜  â”‚
â”‚               â”‚         â”‚               â”‚              â”‚        â”‚
â”‚         â”Œâ”€â”€â”€â”€â”€â”´â”€â”€â”€â”€â”€â”€â”€â”€â”€â”´â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”´â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜        â”‚
â”‚         â”‚                                                        â”‚
â””â”€â”€â”€â”€â”€â”€â”€â”€â”€â”¼â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜
          â”‚
          â–¼
â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”
â”‚                    STORAGE LAYER                                â”‚
â”œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”¤
â”‚                                                                 â”‚
â”‚  â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”  â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”  â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”  â”‚
â”‚  â”‚  knowledge_base/   â”‚  â”‚    reports/     â”‚  â”‚   fixes/    â”‚  â”‚
â”‚  â”‚  â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”‚  â”‚  â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”‚  â”‚  â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”‚  â”‚
â”‚  â”‚  â€¢ Mka_KB.json     â”‚  â”‚  â€¢ summary.json â”‚  â”‚  â€¢ Mka/     â”‚  â”‚
â”‚  â”‚  â€¢ ModB_KB.json    â”‚  â”‚  â€¢ statistics   â”‚  â”‚    - fixes  â”‚  â”‚
â”‚  â”‚  â€¢ Violations DB   â”‚  â”‚  â€¢ Git info     â”‚  â”‚    - justs  â”‚  â”‚
â”‚  â””â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜  â””â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜  â””â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜  â”‚
â”‚                                                                 â”‚
â””â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜
          â”‚
          â–¼
â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”
â”‚                    QUERY & OUTPUT LAYER                         â”‚
â”œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”¤
â”‚                                                                 â”‚
â”‚        â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”              â”‚
â”‚        â”‚ KnowledgeDatabaseQueryTool.py           â”‚              â”‚
â”‚        â”‚  â€¢ Interactive queries                  â”‚              â”‚
â”‚        â”‚  â€¢ Filtering & searching                â”‚              â”‚
â”‚        â”‚  â€¢ Statistics generation                â”‚              â”‚
â”‚        â”‚  â€¢ Export capabilities                  â”‚              â”‚
â”‚        â””â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”¬â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜              â”‚
â”‚                     â”‚                                           â”‚
â”‚         â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”¼â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”                              â”‚
â”‚         â”‚           â”‚           â”‚                              â”‚
â”‚         â–¼           â–¼           â–¼                              â”‚
â”‚    â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”  â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”  â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”                         â”‚
â”‚    â”‚ Excel  â”‚  â”‚  JSON  â”‚  â”‚Console â”‚                         â”‚
â”‚    â”‚ Export â”‚  â”‚ Report â”‚  â”‚ Output â”‚                         â”‚
â”‚    â””â”€â”€â”€â”€â”€â”€â”€â”€â”˜  â””â”€â”€â”€â”€â”€â”€â”€â”€â”˜  â””â”€â”€â”€â”€â”€â”€â”€â”€â”˜                         â”‚
â”‚                                                                 â”‚
â””â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜
```

---

## Component Details

### 1. ParasoftAIAgent.py
**Role:** Main orchestrator and entry point

**Responsibilities:**
- Initialize workspace and directories
- Check Git integration
- Coordinate analysis workflow
- Manage component lifecycle
- Generate summary reports

**Key Methods:**
- `check_git_integration()` - Verify Git repository
- `analyze_report()` - Run violation analysis
- `generate_fixes()` - Create fix suggestions
- `add_justifications()` - Generate justifications
- `run_full_analysis()` - Complete workflow

---

### 2. KnowledgeDatabaseManager.py
**Role:** Knowledge database management

**Responsibilities:**
- Load/save knowledge databases
- Add and update violations
- Track occurrence counts
- Manage fix/justification status
- Generate statistics

**Database Structure:**
```json
{
  "module_name": "Mka",
  "total_unique_violations": 42,
  "violations": {
    "CERT-C.STR31-C": {
      "violation_id": "CERT-C.STR31-C",
      "occurrence_count": 5,
      "severity": "HIGH",
      "files_affected": [...],
      "fix_applied": false,
      ...
    }
  },
  "statistics": {...},
  "metadata": {...}
}
```

---

### 3. ViolationAnalyzer.py
**Role:** Intelligent violation analysis

**Responsibilities:**
- Analyze new violations
- Classify severity and category
- Detect security issues
- Estimate fix complexity
- Generate AI analysis prompts

**Analysis Features:**
- Security violation detection
- Memory safety identification
- Threading issue recognition
- Complexity estimation

---

### 4. CodeFixGenerator.py
**Role:** Fix and justification generation

**Responsibilities:**
- Generate fix suggestions
- Create justification comments
- Format Parasoft justifications
- Save fixes to files

**Fix Types:**
- Explicit casting
- Buffer safety
- Null checks
- Memory management
- MISRA/CERT compliance

---

### 5. KnowledgeDatabaseQueryTool.py
**Role:** Query and reporting interface

**Responsibilities:**
- Query violations by various criteria
- Generate summary reports
- Export to Excel/JSON
- Interactive CLI mode

**Query Capabilities:**
- By severity, category, module
- Unfixed/unjustified violations
- Top violations by occurrence
- File-specific violations

---

## Data Flow

```
â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”
â”‚   User      â”‚
â””â”€â”€â”€â”€â”€â”€â”¬â”€â”€â”€â”€â”€â”€â”˜
       â”‚ Runs: python src\run_agent.py report.html Mka
       â–¼
â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”
â”‚  ParasoftAIAgent        â”‚
â”‚  1. Check Git           â”‚
â”‚  2. Parse report        â”‚â—„â”€â”€ ParasoftAnalysisTool.py
â”‚  3. Analyze violations  â”‚â”€â”€â–º ViolationAnalyzer.py
â””â”€â”€â”€â”€â”€â”€â”€â”€â”¬â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜
         â”‚
         â–¼
â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”
â”‚  ViolationAnalyzer                 â”‚
â”‚  â€¢ Categorize violations           â”‚
â”‚  â€¢ Detect patterns                 â”‚
â”‚  â€¢ Estimate complexity             â”‚
â””â”€â”€â”€â”€â”€â”€â”€â”€â”¬â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜
         â”‚
         â–¼
â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”
â”‚  KnowledgeDatabaseManager          â”‚
â”‚  â€¢ Load existing DB                â”‚
â”‚  â€¢ Add new violations              â”‚
â”‚  â€¢ Update occurrence counts        â”‚
â”‚  â€¢ Save updated DB                 â”‚
â””â”€â”€â”€â”€â”€â”€â”€â”€â”¬â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜
         â”‚
         â–¼
â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”
â”‚  CodeFixGenerator                  â”‚
â”‚  â€¢ Generate fix suggestions        â”‚
â”‚  â€¢ Create justifications           â”‚
â”‚  â€¢ Save to files                   â”‚
â””â”€â”€â”€â”€â”€â”€â”€â”€â”¬â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜
         â”‚
         â–¼
â”Œâ”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”
â”‚  OUTPUT                 â”‚
â”‚  â€¢ Knowledge DB         â”‚
â”‚  â€¢ Analysis summary     â”‚
â”‚  â€¢ Fixes file           â”‚
â”‚  â€¢ Justifications file  â”‚
â””â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”˜
```

---

## Knowledge Database Schema

```json
{
  "module_name": "string",
  "created_date": "ISO-8601 datetime",
  "last_updated": "ISO-8601 datetime",
  "version": "1.0",
  "total_unique_violations": "integer",
  
  "violations": {
    "VIOLATION_ID": {
      "violation_id": "string",
      "violation_text": "string",
      "first_seen": "ISO-8601 datetime",
      "last_seen": "ISO-8601 datetime",
      "occurrence_count": "integer",
      "files_affected": [
        {
          "file": "string",
          "line": "integer",
          "timestamp": "ISO-8601 datetime"
        }
      ],
      "severity": "HIGH|MEDIUM|LOW",
      "category": "CERT|MISRA|CWE|OTHER",
      "justifiable": "Yes|No|Analyse",
      "fix_applied": "boolean",
      "fix_details": "object|null",
      "justification_added": "boolean",
      "justification_text": "string|null",
      "analysis_notes": [
        {
          "timestamp": "ISO-8601 datetime",
          "note": "string"
        }
      ]
    }
  },
  
  "statistics": {
    "total_analyses": "integer",
    "total_violations_processed": "integer",
    "most_common_violations": [
      {
        "violation_id": "string",
        "count": "integer"
      }
    ]
  },
  
  "metadata": {
    "git_commits": ["string"],
    "analysis_history": [
      {
        "timestamp": "ISO-8601 datetime",
        "violations_found": "integer",
        "new_violations": "integer",
        "git_commit": "string"
      }
    ]
  }
}
```

---

## Extension Points

### Future AI Integration

```python
# Example: OpenAI Integration (Future Enhancement)
class AIAnalyzer:
    def analyze_violation(self, violation):
        prompt = self.violation_analyzer.get_ai_analysis_prompt(violation)
        response = openai.chat.completions.create(
            model="gpt-4",
            messages=[{"role": "user", "content": prompt}]
        )
        return response.choices[0].message.content
```

### Custom Rules

```python
# Example: Custom Rule Definition
class CustomRuleEngine:
    def define_rule(self, rule_id, pattern, severity):
        # Add custom organization-specific rules
        pass
```

### Webhooks/Notifications

```python
# Example: Slack Integration
class NotificationManager:
    def notify_high_severity(self, violations):
        # Send alerts for critical issues
        pass
```

---

## Performance Considerations

### Scalability
- **Knowledge Database**: JSON format for easy parsing
- **Incremental Updates**: Only new violations processed
- **Lazy Loading**: Load databases on-demand

### Optimization Opportunities
- Parallel processing for multiple modules
- Database indexing for faster queries
- Caching of frequently accessed data
- Batch processing for large reports

---

## Security Considerations

### Data Privacy
- Knowledge databases stored locally
- No external data transmission (unless AI enabled)
- Git integration respects .gitignore

### Code Safety
- Read-only report parsing
- No automatic code modification
- User reviews all fixes before application

---

## Testing Strategy

### Unit Tests (Recommended)
```python
# Example test structure
def test_violation_categorization():
    analyzer = ViolationAnalyzer("TestModule", kb_manager)
    violation = {"Violation ID": "CERT-C.STR31-C", ...}
    result = analyzer._is_security_violation("CERT-C.STR31-C", "...")
    assert result == True
```

### Integration Tests
- End-to-end workflow testing
- Knowledge database persistence
- Excel export validation

---

## Deployment Options

### Standalone
- Run on individual developer machines
- Local knowledge base storage

### Team Server
- Centralized knowledge base
- Shared violation tracking
- Team analytics

### CI/CD Integration
- Automated analysis on commits
- Build pipeline integration
- Trend tracking over sprints

---

**For implementation details, refer to source code docstrings and inline comments.**


- Database caching for repeated queries
- Batch processing for large codebases

---

## Project Structure

### Folder Organization

```
Parasoft_Analaysis_Tool/
│
├── scripts/                         # Scripts & Launchers
│   └── Launch_GUI.bat                # GUI Launcher
│
├── batch_support/                   # Batch Files  
│   ├── Run.bat                       # Main batch launcher
│   ├── Generate_Code_Fixes.bat       # Fix generation
│   ├── Apply_Suppressions.bat        # Suppress comment applicator
│   ├── Run_Static_Analyzer.bat       # Static analyzer
│   ├── Build_Parasoft_Rules_Database.bat
│   ├── Consolidate_Knowledge.bat
│   ├── Learn_Justification_Patterns.bat
│   └── Query_Master_Knowledge.bat
│
├── src/                              # Source Code
│   ├── parasoft_gui.py               # GUI Application
│   ├── ParasoftAIAgent.py            # Main orchestrator
│   ├── KnowledgeDatabaseManager.py   # Database management
│   ├── ViolationAnalyzer.py          # Analysis engine
│   ├── CodeFixGenerator.py           # Fix generation
│   ├── StaticCodeAnalyzer.py         # Static analyzer
│   ├── ViolationHistoryManager.py    # History & RAG
│   └── run_agent.py                  # Main launcher
│
├── docs/                             # Documentation (6 core + 4 ref)
│   ├── QUICKSTART.md                 # Getting started
│   ├── GUI_GUIDE.md                  # GUI walkthrough
│   ├── AI_GUIDE.md                   # AI features
│   ├── KNOWLEDGE_BASE_GUIDE.md       # KB & learning
│   ├── STATIC_ANALYSIS.md            # Analysis features
│   ├── ARCHITECTURE.md               # This file
│   ├── INDEX.md                      # Navigation
│   ├── GUI_QUICK_REFERENCE.md        # Quick ref
│   ├── BATCH_FILES_GUIDE.md          # Batch commands
│   └── EXAMPLE_WORKFLOW.md           # Examples
│
├── parasoftReport/                   # Parasoft Reports
│   ├── report_dev1.html              # Main input report
│   └── README.md
│
├── knowledge_base/                   # Generated Databases
│   ├── {Module}_KnowledgeDatabase.json
│   ├── learned_patterns_db.json
│   └── Master_KnowledgeDatabase.json
│
├── reports/                          # Analysis Reports
│   ├── {Module}_static_analysis_report.html
│   ├── {Module}_violations_report.xlsx
│   └── {Module}_history_report.html
│
├── history/                          # Violation History
│   ├── master_violation_history.json
│   └── resolution_patterns.json
│
├── fixes/                            # Generated Fixes
│   └── {Module}/
│       ├── *_code_fixes_*.txt
│       └── *_justifications_*.txt
│
├── justifications/                   # Suppress Comments
│   └── *_suppress_comments_*.txt
│
├── learning/                         # AI Training Data
│   ├── fixes_dataset/
│   ├── justifications_dataset/
│   └── violations_dataset/
│
├── data/                             # Data Files
│   ├── Parasoft_Rules_Database.json
│   └── Qorix_CP_Common_Deviations.xlsx
│
└── config/                           # Configuration
    └── config.json
```

### Key Directories

**Input:**
- parasoftReport/ - Parasoft HTML reports
- data/ - Rules database, deviation files

**Processing:**
- src/ - All Python source code
- atch_support/ - Batch file automation

**Output:**
- eports/ - HTML, Excel, JSON reports
- ixes/ - Generated code fixes
- justifications/ - Suppress comments
- knowledge_base/ - Violation databases

**Documentation:**
- docs/ - All markdown documentation (10 files, down from 41)

---

## Version History

### [4.0.0] - 2026-04-29

**Major Release - GUI & Documentation Consolidation**

#### Graphical User Interface
- Complete GUI application with 5 tabs
- Real-time logging and process control
- AI Chat integration with dynamic model loading
- Help tab with documentation viewer
- Startup system checks

#### Documentation Consolidation
- **Reduced from 41 to 10 MD files** (75% reduction)
- **6 Core Guides:** QUICKSTART, GUI_GUIDE, AI_GUIDE, KNOWLEDGE_BASE_GUIDE, STATIC_ANALYSIS, ARCHITECTURE
- **4 Reference Docs:** INDEX, GUI_QUICK_REFERENCE, BATCH_FILES_GUIDE, EXAMPLE_WORKFLOW
- Comprehensive consolidation of related topics
- Improved navigation with INDEX.md

#### Project Reorganization
- Moved batch files to atch_support/
- Moved report to parasoftReport/ folder
- Better folder structure and organization

### [3.0.0] - 2026-04-16

**Major Release - History Tracking & RAG Learning**

#### Violation History Manager
- Complete historical tracking across modules
- Master history database consolidation
- Trend analysis (improving/worsening/stable)
- Cross-module intelligence

#### RAG Learning System
- Resolution pattern learning
- Confidence-based recommendations
- Common resolution tracking
- Cross-module learning

#### Comparison Reports
- Module history reports (HTML)
- Cross-module comparison matrix
- Trend indicators and statistics

### [2.3.0] - 2026-04-12

**Parasoft Rules Database**

- 1200+ official Parasoft rule fixes
- MISRA C, CERT C, CWE coverage
- Instant trusted recommendations
- Batch file for database building

### [2.2.0] - 2026-04-10

**Cross-Module Knowledge & Reports**

- Master knowledge database
- Cross-module consolidation
- HTML/Excel comparison reports
- Knowledge base query tool

### [2.1.0] - 2026-04-08

**MISRA/CERT Static Analyzer**

- Built-in C/C++ static analyzer
- MISRA C:2012 compliance checking
- CERT C secure coding standards
- Color-coded reports (Polyspace-style)
- Auto-generate workflow (no Parasoft needed)

### [2.0.0] - 2026-04-05

**AI Agent with Knowledge Database**

- Ollama AI integration
- Knowledge database system
- AI/Hybrid/Rules modes
- Violation history tracking

### [1.0.0] - 2026-03-28

**Initial Release**

- Parasoft report parsing
- Basic violation analysis
- Fix generation
- Justification creation

---

## Summary

The Parasoft Analysis Tool provides:

- ✅ **Complete AI System** - Ollama-powered analysis
- ✅ **Knowledge Management** - Cross-module learning
- ✅ **Static Analysis** - Built-in MISRA/CERT checker
- ✅ **GUI Interface** - User-friendly point-and-click
- ✅ **Documentation** - 10 comprehensive guides
- ✅ **History Tracking** - RAG learning from resolutions
- ✅ **Multiple Workflows** - Parasoft, static, auto-generate
- ✅ **Batch Automation** - 10+ batch files for all operations

For more information, see [INDEX.md](INDEX.md) for complete documentation navigation.

---

**Developer:** Himanshu R  
**Organization:** Qorix India Pvt Ltd  
**Version:** 4.0.0  
**Platform:** Windows  
**Language:** Python 3.8+
