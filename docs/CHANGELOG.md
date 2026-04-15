# Changelog - Parasoft Analysis Tool

All notable changes to this project will be documented in this file.

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
