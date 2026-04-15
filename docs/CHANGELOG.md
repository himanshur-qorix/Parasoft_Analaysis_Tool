# Changelog - Parasoft Analysis Tool

All notable changes to this project will be documented in this file.

---

## [2.2.1] - 2026-04-15

### ✨ Added - Knowledge Base Integration for Analysis Tools

#### MISRA/CERT Checker Enhancement
- **Cross-checks against knowledge base** during analysis
- Classifies violations as **KNOWN vs NEW**
- Shows **proven fix suggestions** from previous analyses
- Displays **occurrence counts** for known violations
- Enhanced HTML reports with KB insights

#### New Command Line Options
- `--module <name>` - Enable knowledge base integration
- `--kb-dir <path>` - Specify custom knowledge base directory
- `--show-known` - Show detailed breakdown of known vs new violations

#### Enhanced Reporting
- **Visual indicators** in HTML reports:
  - ✅ KNOWN (Fix Available) - Green badge
  - 📊 KNOWN (Nx) - Blue badge with occurrence count
  - ⚠️ NEW - Red badge for first-time violations
- **Inline fix suggestions** with proven solutions
- **Alert banners** for new violations and available fixes
- **Statistics cards** showing known/new breakdown

#### Integration Features
- Checks module-specific knowledge base first
- Falls back to master KB for cross-module insights
- Suggests fixes from other modules automatically
- Tracks violation trends over time
- Helps prioritize new issues

#### Benefits
- **Focus on new issues** - Quickly identify what's different
- **Apply proven fixes** - Leverage solutions that worked before
- **Track progress** - See violation trends across analyses
- **Cross-team learning** - Use fixes from other modules

#### Documentation
- Complete guide: [KNOWLEDGE_BASE_INTEGRATION.md](KNOWLEDGE_BASE_INTEGRATION.md)
- Updated README with usage examples
- Enhanced Run.bat with automatic KB integration

---

## [2.2.0] - 2026-04-15

### ✨ Added - Cross-Module Knowledge Base

#### Master Knowledge Base Manager
- **Consolidates knowledge** from all module-specific databases
- Aggregates violations, fixes, and justifications across modules
- Identifies cross-module patterns and common issues
- Calculates fix success rates per violation
- Generates intelligent recommendations

#### Cross-Module Insights
- **Cross-module violation detection** - Identifies issues appearing in 2+ modules
- **Proven fix recommendations** - Suggests fixes that worked in other modules
- **Fix success rate tracking** - Shows % success for each violation type
- **Justification templates** - Reusable justifications from other modules
- **Pattern recognition** - Identifies company-wide vs module-specific issues

#### New Tools & Scripts
- **MasterKnowledgeBaseManager.py** - Core consolidation logic
- **consolidate_knowledge.py** - Consolidation script
- **query_master_knowledge.py** - Interactive query tool
- **Consolidate_Knowledge.bat** - Windows batch script
- **Query_Master_Knowledge.bat** - Windows query script

#### Features
- Automatic aggregation of all module knowledge databases
- Export consolidated reports to Excel
- Interactive query interface with 9 query options
- Recommendations engine for prioritized actions
- Category and severity-based filtering
- Violation insight lookup with cross-module data

#### Excel Reporting
- Multi-sheet consolidated reports
- All violations with cross-module stats
- Cross-module violations summary
- Statistics and metrics dashboard
- Fix success rate tracking

#### Documentation
- Complete guide: [CROSS_MODULE_KNOWLEDGE.md](CROSS_MODULE_KNOWLEDGE.md)
- Updated README with usage examples
- Integration with existing workflow

### 🎯 Benefits
- **Faster violation resolution** - Apply proven fixes from other modules
- **Knowledge sharing** - Leverage team expertise across projects
- **Pattern recognition** - Identify systemic coding issues
- **Prioritized remediation** - Focus on cross-module violations first
- **Metrics & reporting** - Track progress company-wide

---

## [2.1.1] - 2026-04-15

### ✨ Added - Auto-Generate Workflow

#### Smart Report Detection
- **Automatic MISRA/CERT report generation** when report_dev1.html not found
- Tool now works with or without Parasoft report
- Fallback workflow: Generate MISRA/CERT report from source code if no Parasoft report available

#### Enhanced Batch Script (Run.bat)
- Detects missing report_dev1.html automatically
- Prompts for source code path if report missing
- Generates MISRA/CERT HTML report on-the-fly
- Uses generated report as input for analysis
- Seamless integration with existing workflow

#### Report Parser Enhancement
- **Auto-detection** of report type (Parasoft vs MISRA/CERT)
- Added `parse_misra_cert_report()` function
- Unified parser handles both report formats
- Consistent violation data structure regardless of source

#### Workflow Options
1. **Traditional**: Place report_dev1.html → Run analysis
2. **New**: Provide source code path → Auto-generate report → Run analysis
3. **Hybrid**: Both Parasoft report + source code for comprehensive analysis

---

## [2.1.0] - 2026-04-15

### ✨ Added - MISRA/CERT Static Analyzer Integration

#### Pre-Analysis Workflow
- **Integrated MISRA-C:2012 and CERT-C static analyzer** as pre-analysis step
- Runs pattern-based violation detection on source code before Parasoft analysis
- Generates detailed MISRA/CERT violation reports (HTML, JSON, or text format)
- Seamless integration with existing Parasoft analysis workflow

#### Configuration Updates
- Added `source_code_path` to workspace configuration
- Added `run_misra_cert_checker` flag to analysis configuration
- Added `misra_cert_report_format` option (html/json/text)
- Configuration support in `config/config.json`

#### Command Line Support
- New `--source-code <path>` option to specify C/C++ source directory
- Automatic MISRA/CERT pre-analysis when source code path provided
- Optional integration - can be skipped if source path not provided
- Results included in analysis summary JSON

#### Features
- Pattern-based detection for 20+ MISRA-C:2012 rules
- Pattern-based detection for 15+ CERT-C rules
- Detailed violation reporting with:
  - Rule ID and description
  - Severity and category
  - File, line, and column location
  - Code snippets
  - Rule rationale
- Timestamped report generation
- Error handling with workflow continuation on failure

#### Documentation
- Added `MISRA_CERT_INTEGRATION.md` with comprehensive guide
- Updated `Run.bat` to prompt for source code path
- Updated command line help and examples
- Standalone checker usage examples

#### Enhanced Workflow
1. MISRA/CERT pre-analysis (optional)
2. Parasoft violation analysis
3. Qorix deviation mapping
4. Code fix generation
5. Justification creation
6. Knowledge database update

---

## [2.0.0] - 2026-04-09

### 🎉 Major Release - AI Agent with Knowledge Database

This is a major upgrade from version 1.0.0, introducing AI-powered analysis and intelligent knowledge management.

### ✨ Added

#### Core Features
- **ParasoftAIAgent.py** - Main AI agent orchestrator with Git integration
- **KnowledgeDatabaseManager.py** - Module-specific knowledge database management
- **ViolationAnalyzer.py** - Intelligent violation analysis with categorization
- **CodeFixGenerator.py** - Automated code fix and justification generation
- **KnowledgeDatabaseQueryTool.py** - Comprehensive query and reporting tool

#### Git Integration
- Automatic Git repository detection
- Branch and commit tracking in knowledge database
- Analysis history linked to Git commits
- Compatible with Visual Studio Code Git workflow

#### Knowledge Database Features
- Module-specific JSON databases (`{Module}_KnowledgeDatabase.json`)
- Automatic tracking of unique violations
- Occurrence counting and trending
- Historical analysis tracking
- Fix and justification status management
- Smart severity and category classification

#### Analysis Capabilities
- Security violation detection
- Memory safety issue identification
- Threading/concurrency problem recognition
- Fix complexity estimation
- Automatic categorization (CERT, MISRA, CWE, OTHER)
- Severity classification (HIGH, MEDIUM, LOW)

#### Code Fix Generation
- MISRA-specific fix suggestions
- CERT-specific fix suggestions
- Buffer safety fixes
- Null pointer check additions
- Memory management recommendations
- Type casting corrections

#### Justification System
- Parasoft-formatted justification comments
- Intelligent reason generation
- Template-based justification creation
- Multi-file justification tracking

#### Query & Reporting
- Interactive query mode
- Filter by severity, category, module
- Top violations analysis
- Unfixed/unjustified violation queries
- JSON and Excel export formats
- Statistical summaries

#### Documentation
- **README.md** - Comprehensive documentation (complete rewrite)
- **QUICKSTART.md** - Quick start guide for new users
- **EXAMPLE_WORKFLOW.md** - Real-world workflow demonstration
- Inline code documentation and docstrings

#### Configuration
- **config.json** - Centralized configuration file
- **requirements.txt** - Python dependency management
- **.gitignore** - Git repository integration

#### Automation
- Enhanced **Run.bat** with module name prompt
- Virtual environment support
- Automatic dependency installation
- User-friendly error messages

### 🔄 Changed

#### From Version 1.0.0
- **ParasoftAnalysisTool.py** - Now serves as base parser (unchanged for compatibility)
- **Run.bat** - Completely rewritten for AI agent integration
- **README.md** - Expanded from 6 lines to comprehensive documentation

### 🏗️ Architecture

```
┌─────────────────────────────────────────────────────────┐
│              Parasoft AI Agent (v2.0.0)                 │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌──────────────┐      ┌───────────────────────┐      │
│  │   Git Repo   │◄────►│  ParasoftAIAgent.py   │      │
│  │ Integration  │      │   (Orchestrator)      │      │
│  └──────────────┘      └───────────┬───────────┘      │
│                                    │                    │
│         ┌──────────────────────────┼───────────────┐   │
│         │                          │               │   │
│         ▼                          ▼               ▼   │
│  ┌──────────────┐    ┌──────────────────┐  ┌──────────┐
│  │  Knowledge   │◄───┤  Violation       │  │   Code   │
│  │   Database   │    │   Analyzer       │  │   Fix    │
│  │   Manager    │    │                  │  │Generator │
│  └──────┬───────┘    └──────────────────┘  └────┬─────┘
│         │                                        │      │
│         │            ┌──────────────────┐        │      │
│         └───────────►│  Query Tool      │◄───────┘      │
│                      │  (Reporting)     │               │
│                      └──────────────────┘               │
│                                                         │
│  Input: report_dev1.html  →  Output: Knowledge DB,     │
│                                       Fixes, Reports     │
└─────────────────────────────────────────────────────────┘
```

### 📦 Deliverables

All TODO items completed:
1. ✅ Git repo integrated with Visual Studio Code
2. ✅ Agent analyzes report_dev1 reports per module using ParasoftAnalysisTool
3. ✅ Each report creates/updates knowledge database for unique violations
4. ✅ Module-specific knowledge databases: `{Module}_KnowledgeDatabase.json`
5. ✅ Agent provides code fixes for individual violations
6. ✅ Parasoft-formatted justifications added
7. ✅ Python tool captures all issues from knowledge database

### 🎯 Benefits

- **Automated Analysis**: Reduces manual effort in violation review
- **Historical Tracking**: See violation trends over time
- **Team Collaboration**: Excel exports for easy sharing
- **Intelligent Fixes**: AI-powered fix suggestions
- **Knowledge Retention**: Build institutional knowledge over time
- **Git Integration**: Seamless workflow with version control
- **Scalable**: Handle multiple modules independently

### 🔧 Technical Details

#### Dependencies
- beautifulsoup4 >= 4.12.0
- lxml >= 4.9.0
- pandas >= 2.0.0
- openpyxl >= 3.1.0

#### Python Version
- Requires Python 3.8 or higher

#### Compatibility
- Windows (primary platform)
- Linux/macOS (compatible with minor path adjustments)

### 📝 Migration from v1.0.0

**For existing users:**
1. Install new dependencies: `pip install -r requirements.txt`
2. Use new command: `python ParasoftAIAgent.py report_dev1.html ModuleName`
3. Old tool still available as `ParasoftAnalysisTool.py` for compatibility

**Breaking Changes:**
- Run.bat now requires module name input
- Output directory structure changed (knowledge_base/, reports/, fixes/)

---

## [1.0.0] - 2024-XX-XX

### Added
- Initial release
- Basic Parasoft HTML report parsing
- Excel output generation
- Simple violation categorization
- Basic Run.bat automation

---

## Future Roadmap

### [2.1.0] - Planned
- OpenAI/Anthropic integration for advanced AI analysis
- Automated fix application to source files
- Web-based dashboard
- Team collaboration features

### [2.2.0] - Planned
- CI/CD pipeline integration
- Real-time analysis during development
- Custom rule definitions
- Multi-project support

### [3.0.0] - Vision
- Cloud-based knowledge sharing
- Machine learning for fix prediction
- Automatic code review integration
- Enterprise features

---

**For detailed information, see [README.md](README.md)**
