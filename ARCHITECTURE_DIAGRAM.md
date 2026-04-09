# Parasoft AI Agent - Architecture Diagram
**Version 2.0.0 | Complete System Architecture**

---

## 🏗️ High-Level System Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                     PARASOFT AI AGENT ECOSYSTEM                             │
│                          (Version 2.0.0)                                    │
└─────────────────────────────────────────────────────────────────────────────┘

                                    ┌──────────────┐
                                    │    USER      │
                                    └──────┬───────┘
                                           │
                    ┌──────────────────────┼──────────────────────┐
                    │                      │                      │
                    ▼                      ▼                      ▼
            ┌──────────────┐      ┌──────────────┐      ┌──────────────┐
            │  Run.bat     │      │ run_agent.py │      │ run_query.py │
            │  (Windows)   │      │  (Launcher)  │      │  (Launcher)  │
            └──────┬───────┘      └──────┬───────┘      └──────┬───────┘
                   │                      │                      │
                   └──────────────────────┼──────────────────────┘
                                          │
                                          ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                              CORE ENGINE                                    │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│   ┌────────────────────────────────────────────────────────────────────┐   │
│   │              ParasoftAIAgent.py (ORCHESTRATOR)                     │   │
│   │  ┌──────────────────────────────────────────────────────────────┐ │   │
│   │  │ • Initialize workspace                                       │ │   │
│   │  │ • Check Git integration                                      │ │   │
│   │  │ • Coordinate all components                                  │ │   │
│   │  │ • Manage workflow pipeline                                   │ │   │
│   │  │ • Generate summary reports                                   │ │   │
│   │  └──────────────────────────────────────────────────────────────┘ │   │
│   └────────────────────────┬───────────────────────────────────────────┘   │
│                            │                                               │
│                            │ Delegates to:                                 │
│                            │                                               │
│         ┌──────────────────┼──────────────────┬──────────────────────┐     │
│         │                  │                  │                      │     │
│         ▼                  ▼                  ▼                      ▼     │
│  ┌─────────────┐  ┌──────────────┐  ┌──────────────┐  ┌───────────────┐  │
│  │ Git Check   │  │ Report Parse │  │  Knowledge   │  │  Violation    │  │
│  │ Module      │  │ (Legacy Tool)│  │  Database    │  │  Analyzer     │  │
│  │             │  │              │  │  Manager     │  │               │  │
│  └─────────────┘  └──────────────┘  └──────┬───────┘  └───────┬───────┘  │
│                                             │                   │          │
│                                             └───────┬───────────┘          │
│                                                     │                      │
│                                                     ▼                      │
│                                          ┌──────────────────┐              │
│                                          │  Code Fix        │              │
│                                          │  Generator       │              │
│                                          └──────────────────┘              │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
                                          │
                                          │ Writes to:
                                          │
                                          ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                           DATA LAYER (OUTPUT)                               │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│  ┌────────────────┐  ┌─────────────────┐  ┌──────────────┐                │
│  │ knowledge_base/│  │    reports/     │  │   fixes/     │                │
│  │ ──────────────│  │  ──────────────│  │  ───────────│                │
│  │ • {Module}_   │  │  • summary.json │  │  • {Module}/ │                │
│  │   KB.json     │  │  • statistics   │  │    - fixes   │                │
│  │ • Violations  │  │  • Git metadata │  │    - justs.  │                │
│  └────────────────┘  └─────────────────┘  └──────────────┘                │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
                                          │
                                          │ Queried by:
                                          │
                                          ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                    QUERY & REPORTING LAYER                                  │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│        ┌─────────────────────────────────────────────────────┐             │
│        │    KnowledgeDatabaseQueryTool.py                    │             │
│        │    • Load all databases                             │             │
│        │    • Interactive queries                            │             │
│        │    • Filter & search                                │             │
│        │    • Statistical analysis                           │             │
│        │    • Export to Excel/JSON                           │             │
│        └────────────┬────────────────────────────────────────┘             │
│                     │                                                       │
│         ┌───────────┼──────────┐                                           │
│         │           │          │                                           │
│         ▼           ▼          ▼                                           │
│    ┌────────┐  ┌────────┐  ┌──────────┐                                   │
│    │ Excel  │  │  JSON  │  │ Console  │                                   │
│    │ Report │  │ Summary│  │  Output  │                                   │
│    └────────┘  └────────┘  └──────────┘                                   │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## 📦 Component Architecture (Detailed)

```
┌───────────────────────────────────────────────────────────────────────┐
│                         COMPONENT BREAKDOWN                           │
└───────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│ 1. ORCHESTRATOR LAYER (ParasoftAIAgent.py)                         │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │ ParasoftAIAgent Class                                        │  │
│  │ ────────────────────────────────────────────────────────────│  │
│  │                                                              │  │
│  │  Methods:                                                    │  │
│  │  ┌────────────────────────────────────────────────────────┐ │  │
│  │  │ __init__(workspace_path)                              │ │  │
│  │  │   • Initialize directories                            │ │  │
│  │  │   • Set up logging                                    │ │  │
│  │  │                                                        │ │  │
│  │  │ check_git_integration()                               │ │  │
│  │  │   • Detect Git repository                            │ │  │
│  │  │   • Verify Git availability                          │ │  │
│  │  │                                                        │ │  │
│  │  │ get_git_info()                                        │ │  │
│  │  │   • Get branch name                                   │ │  │
│  │  │   • Get commit hash                                   │ │  │
│  │  │                                                        │ │  │
│  │  │ analyze_report(report_path, module_name)              │ │  │
│  │  │   • Parse Parasoft report                            │ │  │
│  │  │   • Analyze violations                               │ │  │
│  │  │   • Update knowledge base                            │ │  │
│  │  │                                                        │ │  │
│  │  │ generate_fixes(module_name, violation_ids)            │ │  │
│  │  │   • Create fix suggestions                           │ │  │
│  │  │   • Save to files                                    │ │  │
│  │  │                                                        │ │  │
│  │  │ add_justifications(module_name, violation_ids)        │ │  │
│  │  │   • Generate Parasoft justifications                 │ │  │
│  │  │   • Format according to standards                    │ │  │
│  │  │                                                        │ │  │
│  │  │ run_full_analysis()                                   │ │  │
│  │  │   • Execute complete workflow                        │ │  │
│  │  │   • Coordinate all components                        │ │  │
│  │  │   • Generate summary                                 │ │  │
│  │  └────────────────────────────────────────────────────────┘ │  │
│  └──────────────────────────────────────────────────────────────┘  │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│ 2. DATA MANAGEMENT LAYER (KnowledgeDatabaseManager.py)             │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │ KnowledgeDatabaseManager Class                               │  │
│  │ ────────────────────────────────────────────────────────────│  │
│  │                                                              │  │
│  │  Core Functions:                                             │  │
│  │  ┌────────────────────────────────────────────────────────┐ │  │
│  │  │ • load_knowledge_base(module_name)                    │ │  │
│  │  │ • save_knowledge_base(module_name)                    │ │  │
│  │  │ • add_violation(violation_data)                       │ │  │
│  │  │ • get_violation(violation_id)                         │ │  │
│  │  │ • get_all_violations(filter_by)                       │ │  │
│  │  │ • update_fix_status(violation_id, fix_details)        │ │  │
│  │  │ • update_justification_status(violation_id, text)     │ │  │
│  │  │ • update_statistics(analysis_info)                    │ │  │
│  │  │ • export_to_excel(output_path)                        │ │  │
│  │  └────────────────────────────────────────────────────────┘ │  │
│  │                                                              │  │
│  │  Database Schema:                                            │  │
│  │  {                                                           │  │
│  │    "module_name": "string",                                 │  │
│  │    "total_unique_violations": int,                          │  │
│  │    "violations": {                                          │  │
│  │      "VIOLATION_ID": {                                      │  │
│  │        "violation_id": "string",                            │  │
│  │        "severity": "HIGH|MEDIUM|LOW",                       │  │
│  │        "category": "CERT|MISRA|CWE|OTHER",                  │  │
│  │        "occurrence_count": int,                             │  │
│  │        "files_affected": [...],                             │  │
│  │        "fix_applied": bool,                                 │  │
│  │        "justification_added": bool                          │  │
│  │      }                                                       │  │
│  │    },                                                        │  │
│  │    "statistics": {...},                                     │  │
│  │    "metadata": {...}                                        │  │
│  │  }                                                           │  │
│  └──────────────────────────────────────────────────────────────┘  │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│ 3. ANALYSIS LAYER (ViolationAnalyzer.py)                           │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │ ViolationAnalyzer Class                                      │  │
│  │ ────────────────────────────────────────────────────────────│  │
│  │                                                              │  │
│  │  Analysis Functions:                                         │  │
│  │  ┌────────────────────────────────────────────────────────┐ │  │
│  │  │ analyze_violations(violations)                        │ │  │
│  │  │   ├─► Categorize new vs existing                     │ │  │
│  │  │   ├─► Update knowledge base                          │ │  │
│  │  │   └─► Generate statistics                            │ │  │
│  │  │                                                        │ │  │
│  │  │ _analyze_new_violation(violation)                     │ │  │
│  │  │   ├─► Detect security issues                         │ │  │
│  │  │   ├─► Detect memory issues                           │ │  │
│  │  │   ├─► Detect threading issues                        │ │  │
│  │  │   └─► Estimate fix complexity                        │ │  │
│  │  │                                                        │ │  │
│  │  │ Classification:                                        │ │  │
│  │  │   • _is_security_violation()                          │ │  │
│  │  │   • _is_memory_violation()                            │ │  │
│  │  │   • _is_threading_violation()                         │ │  │
│  │  │   • _estimate_fix_complexity()                        │ │  │
│  │  │                                                        │ │  │
│  │  │ Queries:                                               │ │  │
│  │  │   • get_violations_by_severity(severity)              │ │  │
│  │  │   • get_violations_by_category(category)              │ │  │
│  │  │   • get_unfixed_violations()                          │ │  │
│  │  │   • get_unjustified_violations()                      │ │  │
│  │  │                                                        │ │  │
│  │  │ generate_analysis_report()                            │ │  │
│  │  │   ├─► Severity breakdown                             │ │  │
│  │  │   ├─► Category breakdown                             │ │  │
│  │  │   ├─► Top violations                                 │ │  │
│  │  │   └─► Priority violations                            │ │  │
│  │  └────────────────────────────────────────────────────────┘ │  │
│  └──────────────────────────────────────────────────────────────┘  │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│ 4. FIX GENERATION LAYER (CodeFixGenerator.py)                      │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │ CodeFixGenerator Class                                       │  │
│  │ ────────────────────────────────────────────────────────────│  │
│  │                                                              │  │
│  │  Fix Generation Pipeline:                                    │  │
│  │                                                              │  │
│  │  ┌────────────────────────────────────────────────────────┐ │  │
│  │  │ 1. generate_all_fixes(violation_ids)                  │ │  │
│  │  │    ├─► Get violations to fix                         │ │  │
│  │  │    ├─► Generate fix for each                         │ │  │
│  │  │    └─► Save to files                                 │ │  │
│  │  │                                                        │ │  │
│  │  │ 2. _generate_fix_for_violation(violation)             │ │  │
│  │  │    ├─► Determine fix type                            │ │  │
│  │  │    ├─► Get fix suggestion                            │ │  │
│  │  │    └─► Update knowledge base                         │ │  │
│  │  │                                                        │ │  │
│  │  │ 3. _get_fix_suggestion(violation_id, text, category)  │ │  │
│  │  │    ├─► MISRA fixes (_get_misra_fix)                  │ │  │
│  │  │    │   • Type conversions                            │ │  │
│  │  │    │   • Pointer handling                            │ │  │
│  │  │    │   • Control flow                                │ │  │
│  │  │    │   • Standard library                            │ │  │
│  │  │    │                                                  │ │  │
│  │  │    ├─► CERT fixes (_get_cert_fix)                    │ │  │
│  │  │    │   • Buffer safety                               │ │  │
│  │  │    │   • Null checks                                 │ │  │
│  │  │    │   • Memory management                           │ │  │
│  │  │    │                                                  │ │  │
│  │  │    └─► Generic fixes                                 │ │  │
│  │  │                                                        │ │  │
│  │  │ Justification Generation:                             │ │  │
│  │  │                                                        │ │  │
│  │  │ 1. generate_justifications(violation_ids)             │ │  │
│  │  │    ├─► Filter justifiable violations                 │ │  │
│  │  │    ├─► Generate Parasoft format                      │ │  │
│  │  │    └─► Save to files                                 │ │  │
│  │  │                                                        │ │  │
│  │  │ 2. _format_parasoft_justification(violation)          │ │  │
│  │  │    ├─► parasoft-begin-suppress                       │ │  │
│  │  │    ├─► Justification text                            │ │  │
│  │  │    └─► parasoft-end-suppress                         │ │  │
│  │  └────────────────────────────────────────────────────────┘ │  │
│  └──────────────────────────────────────────────────────────────┘  │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│ 5. QUERY LAYER (KnowledgeDatabaseQueryTool.py)                     │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │ KnowledgeDatabaseQueryTool Class                             │  │
│  │ ────────────────────────────────────────────────────────────│  │
│  │                                                              │  │
│  │  Query Functions:                                            │  │
│  │  ┌────────────────────────────────────────────────────────┐ │  │
│  │  │ • list_modules()                                      │ │  │
│  │  │ • get_module_summary(module_name)                     │ │  │
│  │  │ • get_all_violations(module_name)                     │ │  │
│  │  │ • query_by_severity(severity)                         │ │  │
│  │  │ • query_by_category(category)                         │ │  │
│  │  │ • query_unfixed_violations()                          │ │  │
│  │  │ • query_unjustified_violations()                      │ │  │
│  │  │ • query_by_file(file_name)                            │ │  │
│  │  │ • query_top_violations(limit)                         │ │  │
│  │  └────────────────────────────────────────────────────────┘ │  │
│  │                                                              │  │
│  │  Reporting Functions:                                        │  │
│  │  ┌────────────────────────────────────────────────────────┐ │  │
│  │  │ • generate_summary_report(output_file)                │ │  │
│  │  │ • export_to_excel(output_file)                        │ │  │
│  │  │ • interactive_query()  [CLI mode]                     │ │  │
│  │  └────────────────────────────────────────────────────────┘ │  │
│  └──────────────────────────────────────────────────────────────┘  │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│ 6. LEGACY PARSER (ParasoftAnalysisTool.py)                         │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │ HTML Report Parser (v1.0.0)                                  │  │
│  │ ────────────────────────────────────────────────────────────│  │
│  │                                                              │  │
│  │  Functions:                                                  │  │
│  │  ┌────────────────────────────────────────────────────────┐ │  │
│  │  │ parse_parasoft_report(report_path)                    │ │  │
│  │  │   ├─► Parse HTML with BeautifulSoup                  │ │  │
│  │  │   ├─► Extract violations                             │ │  │
│  │  │   ├─► Extract file names                             │ │  │
│  │  │   ├─► Extract line numbers                           │ │  │
│  │  │   └─► Return structured data                         │ │  │
│  │  │                                                        │ │  │
│  │  │ load_justifiable_mapping(qorix_excel)                 │ │  │
│  │  │   ├─► Load CERT rules                                │ │  │
│  │  │   ├─► Load MISRA rules                               │ │  │
│  │  │   └─► Create justifiable mapping                     │ │  │
│  │  │                                                        │ │  │
│  │  │ resolve_justifiable(violation_id, mapping)            │ │  │
│  │  │   └─► Return Yes/No/Analyse                          │ │  │
│  │  └────────────────────────────────────────────────────────┘ │  │
│  └──────────────────────────────────────────────────────────────┘  │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

---

## 🔄 Complete Data Flow Diagram

```
┌───────────────────────────────────────────────────────────────────────────┐
│                          EXECUTION FLOW                                   │
└───────────────────────────────────────────────────────────────────────────┘

USER INPUT:
  │
  ├─► report_dev1.html (Parasoft HTML Report)
  └─► Module Name (e.g., "Mka")
      │
      ▼
┌─────────────────────────────────────────────────────────────────────┐
│ STEP 1: INITIALIZATION                                             │
│ ─────────────────────────────────────────────────────────────────── │
│                                                                     │
│  run_agent.py                                                      │
│      │                                                              │
│      ├─► Add src/ to Python path                                   │
│      └─► Import ParasoftAIAgent                                    │
│              │                                                      │
│              └─► ParasoftAIAgent.__init__()                        │
│                      │                                              │
│                      ├─► workspace_path = project root             │
│                      ├─► Create knowledge_base/ dir                │
│                      ├─► Create reports/ dir                       │
│                      └─► Create fixes/ dir                         │
│                                                                     │
└────────────────────────────┬────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│ STEP 2: GIT INTEGRATION CHECK                                      │
│ ─────────────────────────────────────────────────────────────────── │
│                                                                     │
│  check_git_integration()                                           │
│      │                                                              │
│      ├─► Run: git rev-parse --git-dir                              │
│      └─► Return: True/False                                        │
│              │                                                      │
│              └─► get_git_info()                                    │
│                      │                                              │
│                      ├─► git branch --show-current                 │
│                      ├─► git rev-parse --short HEAD                │
│                      └─► Return: {branch, commit}                  │
│                                                                     │
└────────────────────────────┬────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│ STEP 3: REPORT PARSING                                             │
│ ─────────────────────────────────────────────────────────────────── │
│                                                                     │
│  analyze_report(report_path, module_name)                          │
│      │                                                              │
│      └─► parse_parasoft_report(report_path)                        │
│              │  [ParasoftAnalysisTool.py]                          │
│              │                                                      │
│              ├─► Open HTML file                                    │
│              ├─► Parse with BeautifulSoup                          │
│              ├─► Extract table rows                                │
│              ├─► Identify violations                               │
│              ├─► Extract file names                                │
│              ├─► Extract line numbers                              │
│              └─► Return: List[Dict]                                 │
│                      │                                              │
│                      │  Example:                                    │
│                      │  [{                                          │
│                      │    "Violation": "Buffer overflow...",        │
│                      │    "Violation ID": "CERT-C.STR31-C",         │
│                      │    "File": "handler.c",                      │
│                      │    "Line number": 145                        │
│                      │  }, ...]                                     │
│                      │                                              │
└────────────────────────────┬────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│ STEP 4: VIOLATION ANALYSIS                                         │
│ ─────────────────────────────────────────────────────────────────── │
│                                                                     │
│  ViolationAnalyzer.analyze_violations(violations)                  │
│      │                                                              │
│      ├─► FOR EACH violation:                                       │
│      │       │                                                      │
│      │       ├─► kb_manager.add_violation(violation)               │
│      │       │       │                                              │
│      │       │       ├─► Check if violation_id exists              │
│      │       │       │                                              │
│      │       │       ├─► IF NEW:                                   │
│      │       │       │   ├─► Create new entry                      │
│      │       │       │   ├─► Determine severity                    │
│      │       │       │   ├─► Determine category                    │
│      │       │       │   ├─► Set occurrence_count = 1              │
│      │       │       │   └─► _analyze_new_violation()              │
│      │       │       │         │                                    │
│      │       │       │         ├─► _is_security_violation()        │
│      │       │       │         ├─► _is_memory_violation()          │
│      │       │       │         ├─► _is_threading_violation()       │
│      │       │       │         └─► _estimate_fix_complexity()      │
│      │       │       │                                              │
│      │       │       └─► IF EXISTS:                                │
│      │       │           ├─► Increment occurrence_count            │
│      │       │           ├─► Update last_seen                      │
│      │       │           └─► Add file/line info                    │
│      │       │                                                      │
│      │       └─► Return: is_new (True/False)                       │
│      │                                                              │
│      ├─► Count new_violations                                      │
│      ├─► Count existing_violations                                 │
│      └─► update_statistics()                                       │
│                                                                     │
└────────────────────────────┬────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│ STEP 5: SAVE KNOWLEDGE BASE                                        │
│ ─────────────────────────────────────────────────────────────────── │
│                                                                     │
│  kb_manager.save_knowledge_base(module_name)                       │
│      │                                                              │
│      ├─► Update last_updated timestamp                             │
│      ├─► file_path = knowledge_base/{Module}_KnowledgeDatabase.json│
│      ├─► Write JSON with indent=2                                  │
│      └─► Return: file_path                                         │
│                                                                     │
│  OUTPUT FILE: Mka_KnowledgeDatabase.json                           │
│  {                                                                  │
│    "module_name": "Mka",                                           │
│    "total_unique_violations": 42,                                  │
│    "violations": {                                                 │
│      "CERT-C.STR31-C": {                                           │
│        "violation_id": "CERT-C.STR31-C",                           │
│        "severity": "HIGH",                                         │
│        "category": "CERT",                                         │
│        "occurrence_count": 5,                                      │
│        "files_affected": [...],                                    │
│        "fix_applied": false,                                       │
│        ...                                                         │
│      },                                                            │
│      ...                                                           │
│    },                                                              │
│    "statistics": {...},                                            │
│    "metadata": {...}                                               │
│  }                                                                  │
│                                                                     │
└────────────────────────────┬────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│ STEP 6: FIX GENERATION (if enabled)                                │
│ ─────────────────────────────────────────────────────────────────── │
│                                                                     │
│  CodeFixGenerator.generate_all_fixes(violation_ids)                │
│      │                                                              │
│      ├─► Get unfixed violations from knowledge base                │
│      │                                                              │
│      ├─► FOR EACH violation:                                       │
│      │       │                                                      │
│      │       └─► _generate_fix_for_violation(violation)            │
│      │               │                                              │
│      │               ├─► _get_fix_suggestion()                     │
│      │               │       │                                      │
│      │               │       ├─► IF MISRA: _get_misra_fix()        │
│      │               │       │   • Explicit casting                │
│      │               │       │   • Pointer handling                │
│      │               │       │   • Control flow fixes              │
│      │               │       │                                      │
│      │               │       ├─► IF CERT: _get_cert_fix()          │
│      │               │       │   • Buffer safety                   │
│      │               │       │   • Null checks                     │
│      │               │       │   • Memory management               │
│      │               │       │                                      │
│      │               │       └─► ELSE: _get_generic_fix()          │
│      │               │                                              │
│      │               ├─► Create fix_data object                    │
│      │               └─► Update knowledge base                     │
│      │                                                              │
│      └─► _save_fixes_file(all_fixes, output_file)                 │
│              │                                                      │
│              └─► OUTPUT: fixes/Mka/Mka_fixes_TIMESTAMP.txt         │
│                                                                     │
└────────────────────────────┬────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│ STEP 7: JUSTIFICATION GENERATION (if enabled)                      │
│ ─────────────────────────────────────────────────────────────────── │
│                                                                     │
│  CodeFixGenerator.generate_justifications(violation_ids)           │
│      │                                                              │
│      ├─► Get justifiable violations (justifiable='Yes')            │
│      │                                                              │
│      ├─► FOR EACH violation:                                       │
│      │       │                                                      │
│      │       └─► _generate_justification(violation)                │
│      │               │                                              │
│      │               ├─► _format_parasoft_justification()          │
│      │               │       │                                      │
│      │               │       ├─► /* parasoft-begin-suppress ... */ │
│      │               │       ├─► /* Justification: ... */          │
│      │               │       ├─► ... code ...                      │
│      │               │       └─► /* parasoft-end-suppress ... */   │
│      │               │                                              │
│      │               ├─► Create justification_data                 │
│      │               └─► Update knowledge base                     │
│      │                                                              │
│      └─► _save_justifications_file(all_justs, output_file)        │
│              │                                                      │
│              └─► OUTPUT: fixes/Mka/Mka_justifications_TIMESTAMP.txt│
│                                                                     │
└────────────────────────────┬────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│ STEP 8: SUMMARY REPORT                                             │
│ ─────────────────────────────────────────────────────────────────── │
│                                                                     │
│  Combine results:                                                  │
│      │                                                              │
│      ├─► git_info (branch, commit)                                 │
│      ├─► analysis_results (violations count, new, existing)        │
│      ├─► fix_results (fixes_generated)                             │
│      └─► justification_results (justifications_generated)          │
│              │                                                      │
│              └─► Save to: reports/Mka_analysis_summary.json        │
│                                                                     │
│  OUTPUT FILE: Mka_analysis_summary.json                            │
│  {                                                                  │
│    "git_info": {...},                                              │
│    "analysis": {                                                   │
│      "status": "success",                                          │
│      "total_violations": 157,                                      │
│      "new_unique_violations": 42,                                  │
│      "existing_violations": 115,                                   │
│      "knowledge_base_path": "..."                                  │
│    },                                                              │
│    "fixes": {...},                                                 │
│    "justifications": {...},                                        │
│    "timestamp": "2026-04-09T14:30:00"                              │
│  }                                                                  │
│                                                                     │
└────────────────────────────┬────────────────────────────────────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │  ANALYSIS       │
                    │  COMPLETE ✓     │
                    └─────────────────┘
```

---

## 🔍 Query Tool Flow Diagram

```
┌───────────────────────────────────────────────────────────────────────────┐
│                      QUERY TOOL EXECUTION FLOW                            │
└───────────────────────────────────────────────────────────────────────────┘

USER INPUT:
  │
  └─► python run_query.py knowledge_base --interactive
      │
      ▼
┌─────────────────────────────────────────────────────────────────────┐
│ INITIALIZATION                                                      │
│ ─────────────────────────────────────────────────────────────────── │
│                                                                     │
│  KnowledgeDatabaseQueryTool.__init__(knowledge_base_dir)           │
│      │                                                              │
│      └─► _load_all_databases()                                     │
│              │                                                      │
│              ├─► Find all *_KnowledgeDatabase.json files           │
│              │                                                      │
│              ├─► FOR EACH database file:                           │
│              │       │                                              │
│              │       ├─► Extract module_name from filename         │
│              │       ├─► Load JSON                                 │
│              │       └─► Store in self.databases[module_name]      │
│              │                                                      │
│              └─► Return: databases dictionary                      │
│                                                                     │
└────────────────────────────┬────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│ INTERACTIVE MODE                                                    │
│ ─────────────────────────────────────────────────────────────────── │
│                                                                     │
│  interactive_query()                                               │
│      │                                                              │
│      └─► LOOP:                                                     │
│              │                                                      │
│              ├─► Display menu:                                     │
│              │   1. List modules                                   │
│              │   2. Module summary                                 │
│              │   3. Query by severity                              │
│              │   4. Query by category                              │
│              │   5. Query unfixed violations                       │
│              │   6. Query unjustified violations                   │
│              │   7. Query by file                                  │
│              │   8. Top violations                                 │
│              │   9. Generate summary report                        │
│              │   10. Export to Excel                               │
│              │   0. Exit                                           │
│              │                                                      │
│              ├─► Get user input                                    │
│              │                                                      │
│              ├─► EXECUTE selected query:                           │
│              │       │                                              │
│              │       ├─► Call appropriate method                   │
│              │       ├─► Get results                               │
│              │       └─► Display results                           │
│              │                                                      │
│              └─► Repeat until user exits                           │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘

QUERY METHODS:
  │
  ├─► list_modules()
  │   └─► Return: List of all module names
  │
  ├─► get_module_summary(module_name)
  │   └─► Return: Summary dict with statistics
  │
  ├─► query_by_severity(severity, module_name)
  │   ├─► Get all violations
  │   ├─► Filter by severity
  │   └─► Return: Filtered list
  │
  ├─► query_by_category(category, module_name)
  │   ├─► Get all violations
  │   ├─► Filter by category
  │   └─► Return: Filtered list
  │
  ├─► query_top_violations(limit, module_name)
  │   ├─► Get all violations
  │   ├─► Sort by occurrence_count
  │   └─► Return: Top N violations
  │
  ├─► generate_summary_report(output_file)
  │   ├─► Aggregate statistics
  │   ├─► Calculate totals
  │   ├─► Create report dict
  │   └─► Save to JSON
  │
  └─► export_to_excel(output_file, module_name)
      ├─► Get all violations
      ├─► Create DataFrame
      ├─► Create Excel with multiple sheets:
      │   • All Violations
      │   • High Severity
      │   • Unfixed
      │   • Summary
      └─► Save file
```

---

## 📁 File System Architecture

```
Parasoft_Analaysis_Tool/
│
├── 📂 INPUT FILES
│   ├── report_dev1.html          [Parasoft HTML Report]
│   └── data/
│       └── Qorix_CP_Common_Deviations.xlsx
│
├── 📂 CONFIGURATION
│   ├── config/
│   │   └── config.json           [Agent configuration]
│   └── requirements.txt          [Python dependencies]
│
├── 📂 SOURCE CODE (src/)
│   ├── ParasoftAIAgent.py        [Main orchestrator - 265 lines]
│   ├── KnowledgeDatabaseManager.py [DB management - 374 lines]
│   ├── ViolationAnalyzer.py      [Analysis engine - 303 lines]
│   ├── CodeFixGenerator.py       [Fix generator - 475 lines]
│   ├── KnowledgeDatabaseQueryTool.py [Query tool - 613 lines]
│   └── ParasoftAnalysisTool.py   [HTML parser - legacy]
│
├── 📂 LAUNCHERS
│   ├── run_agent.py              [Main launcher script]
│   └── run_query.py              [Query launcher script]
│
├── 📂 SCRIPTS
│   └── scripts/
│       └── Run.bat               [Windows automation]
│
├── 📂 OUTPUT FILES
│   ├── knowledge_base/
│   │   ├── {Module}_KnowledgeDatabase.json    [Per-module DB]
│   │   ├── Mka_KnowledgeDatabase.json
│   │   └── ...
│   │
│   ├── reports/
│   │   ├── {Module}_analysis_summary.json     [Summary reports]
│   │   └── ...
│   │
│   └── fixes/
│       ├── {Module}/
│       │   ├── {Module}_fixes_TIMESTAMP.txt
│       │   └── {Module}_justifications_TIMESTAMP.txt
│       └── ...
│
└── 📂 DOCUMENTATION (docs/)
    ├── QUICKSTART.md
    ├── EXAMPLE_WORKFLOW.md
    ├── ARCHITECTURE.md
    ├── CHANGELOG.md
    ├── PROJECT_SUMMARY.md
    └── INDEX.md
```

---

## 🔗 Component Dependencies

```
┌─────────────────────────────────────────────────────────────────┐
│                    DEPENDENCY GRAPH                             │
└─────────────────────────────────────────────────────────────────┘

run_agent.py / run_query.py
      │
      ├─► sys, pathlib
      └─► Adds src/ to Python path
              │
              ▼
┌─────────────────────────────────────────────────────────────────┐
│ ParasoftAIAgent.py                                              │
│   ├─► IMPORTS:                                                  │
│   │   ├─► sys, os, json, logging                               │
│   │   ├─► pathlib.Path                                         │
│   │   ├─► datetime                                             │
│   │   └─► subprocess                                           │
│   │                                                             │
│   └─► USES:                                                     │
│       ├─► KnowledgeDatabaseManager                              │
│       ├─► ViolationAnalyzer                                     │
│       ├─► CodeFixGenerator                                      │
│       └─► ParasoftAnalysisTool.parse_parasoft_report()         │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ KnowledgeDatabaseManager.py                                     │
│   ├─► IMPORTS:                                                  │
│   │   ├─► json, logging                                        │
│   │   ├─► pathlib.Path                                         │
│   │   ├─► datetime                                             │
│   │   ├─► typing (Dict, List, Optional)                        │
│   │   └─► pandas (optional, for Excel export)                  │
│   │                                                             │
│   └─► USES:                                                     │
│       └─► openpyxl (via pandas for Excel)                      │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ ViolationAnalyzer.py                                            │
│   ├─► IMPORTS:                                                  │
│   │   ├─► logging                                              │
│   │   ├─► typing (Dict, List)                                  │
│   │   └─► datetime                                             │
│   │                                                             │
│   └─► USES:                                                     │
│       └─► KnowledgeDatabaseManager                              │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ CodeFixGenerator.py                                             │
│   ├─► IMPORTS:                                                  │
│   │   ├─► logging                                              │
│   │   ├─► pathlib.Path                                         │
│   │   ├─► typing (Dict, List, Optional)                        │
│   │   └─► datetime                                             │
│   │                                                             │
│   └─► USES:                                                     │
│       └─► KnowledgeDatabaseManager                              │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ KnowledgeDatabaseQueryTool.py                                   │
│   ├─► IMPORTS:                                                  │
│   │   ├─► sys, json, logging                                   │
│   │   ├─► pathlib.Path                                         │
│   │   ├─► typing (Dict, List, Optional)                        │
│   │   ├─► datetime                                             │
│   │   └─► pandas                                               │
│   │                                                             │
│   └─► USES:                                                     │
│       └─► openpyxl (via pandas)                                │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ ParasoftAnalysisTool.py                                         │
│   ├─► IMPORTS:                                                  │
│   │   ├─► sys                                                   │
│   │   ├─► pathlib.Path                                         │
│   │   ├─► beautifulsoup4 (BeautifulSoup)                       │
│   │   ├─► pandas                                               │
│   │   └─► collections.Counter                                  │
│   │                                                             │
│   └─► USES:                                                     │
│       ├─► lxml (HTML parser for BeautifulSoup)                 │
│       └─► openpyxl (via pandas)                                │
└─────────────────────────────────────────────────────────────────┘

EXTERNAL DEPENDENCIES (from requirements.txt):
  • beautifulsoup4 >= 4.12.0
  • lxml >= 4.9.0
  • pandas >= 2.0.0
  • openpyxl >= 3.1.0
```

---

## 🎯 Key Design Patterns

```
┌─────────────────────────────────────────────────────────────────┐
│ 1. ORCHESTRATOR PATTERN                                        │
│    ParasoftAIAgent orchestrates all components                 │
│    Single entry point for all operations                       │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ 2. MANAGER PATTERN                                             │
│    KnowledgeDatabaseManager handles all DB operations          │
│    Encapsulates database logic                                 │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ 3. ANALYZER PATTERN                                            │
│    ViolationAnalyzer processes and categorizes violations      │
│    Separates analysis logic from storage                       │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ 4. GENERATOR PATTERN                                           │
│    CodeFixGenerator creates fixes and justifications           │
│    Template-based generation                                   │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ 5. QUERY INTERFACE PATTERN                                     │
│    KnowledgeDatabaseQueryTool provides flexible queries        │
│    Multiple output formats (JSON, Excel, Console)              │
└─────────────────────────────────────────────────────────────────┘
```

---

**End of Architecture Diagram**

For implementation details, see source code in `src/` folder.
For usage examples, see `docs/EXAMPLE_WORKFLOW.md`.
For project structure, see `STRUCTURE.md`.
