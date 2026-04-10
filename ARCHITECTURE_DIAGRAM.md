# Parasoft AI Agent - Architecture Diagram
**Version 2.2.0 | Complete System Architecture with Ollama AI Integration**

---

## 🏗️ High-Level System Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                     PARASOFT AI AGENT ECOSYSTEM                             │
│                   (Version 2.2.0 - with Ollama AI)                          │
└─────────────────────────────────────────────────────────────────────────────┘

                                    ┌──────────────┐
                                    │    USER      │
                                    └──────┬───────┘
                                           │
                    ┌──────────────────────┼──────────────────────┬─────────────┐
                    │                      │                      │             │
                    ▼                      ▼                      ▼             ▼
            ┌──────────────┐      ┌──────────────┐      ┌──────────────┐  ┌────────────┐
            │  Run.bat     │      │ scripts/     │      │ scripts/     │  │ Apply_     │
            │  (Windows)   │      │ run_agent.py │      │ run_query.py │  │ Suppress.  │
            └──────┬───────┘      └──────┬───────┘      └──────┬───────┘  │ bat        │
                   │                      │                      │          └─────┬──────┘
                   └──────────────────────┼──────────────────────┘                │
                                          │                                       │
                                          ▼                                       │
┌─────────────────────────────────────────────────────────────────────────────┐ │
│                              CORE ENGINE                                    │ │
├─────────────────────────────────────────────────────────────────────────────┤ │
│                                                                             │ │
│   ┌────────────────────────────────────────────────────────────────────┐   │ │
│   │              ParasoftAIAgent.py (ORCHESTRATOR)                     │   │ │
│   │  ┌──────────────────────────────────────────────────────────────┐ │   │ │
│   │  │ • Initialize workspace (with justifications/ folder)        │ │   │ │
│   │  │ • Check Git integration                                      │ │   │ │
│   │  │ • Load Qorix deviations (Qorix_CP_Common_Deviations.xlsx)   │ │   │ │
│   │  │ • Apply justifiable status to violations                    │ │   │ │
│   │  │ • Generate Excel reports with status column                 │ │   │ │
│   │  │ • Generate Parasoft suppress comments                       │ │   │ │
│   │  │ • Coordinate all components                                  │ │   │ │
│   │  │ • Manage workflow pipeline                                   │ │   │ │
│   │  │ • Generate summary reports                                   │ │   │ │
│   │  └──────────────────────────────────────────────────────────────┘ │   │ │
│   └────────────────────────┬───────────────────────────────────────────┘   │ │
│                            │                                               │ │
│                            │ Delegates to:                                 │ │
│                            │                                               │ │
│         ┌──────────────────┼──────────────────┬──────────────────────┐     │ │
│         │                  │                  │                      │     │ │
│         ▼                  ▼                  ▼                      ▼     │ │
│  ┌─────────────┐  ┌──────────────┐  ┌──────────────┐  ┌───────────────┐  │ │
│  │ Git Check   │  │ Report Parse │  │  Knowledge   │  │  Violation    │  │ │
│  │ Module      │  │ + Qorix      │  │  Database    │  │  Analyzer     │  │ │
│  │             │  │ Integration  │  │  Manager     │  │               │  │ │
│  └─────────────┘  └──────┬───────┘  └──────┬───────┘  └───────┬───────┘  │ │
│                          │                  │                   │          │ │
│                          └──────────┬───────┴───────────────────┘          │ │
│                                     │                                      │ │
│                                     ▼                                      │ │
│                          ┌──────────────────┐                              │ │
│                          │  Code Fix        │                              │ │
│                          │  Generator       │                              │ │
│                          └──────────────────┘                              │ │
│                                                                             │ │
└─────────────────────────────────────────────────────────────────────────────┘ │
                                          │                                     │
                                          │ Writes to:                          │
                                          │                                     │
                                          ▼                                     │
┌─────────────────────────────────────────────────────────────────────────────┐ │
│                           DATA LAYER (OUTPUT)                               │ │
├─────────────────────────────────────────────────────────────────────────────┤ │
│                                                                             │ │
│  ┌────────────────┐  ┌─────────────────┐  ┌──────────────┐  ┌────────────┐│ │
│  │ knowledge_base/│  │    reports/     │  │   fixes/     │  │justificat/ ││ │
│  │ ──────────────│  │  ──────────────│  │  ───────────│  │───────────││ │
│  │ • {Module}_   │  │  • summary.json │  │  • {Module}/ │  │ suppress_  ││ │
│  │   KB.json     │  │  • Excel report │  │    - fixes   │  │ comments_  ││ │
│  │ • Violations  │  │    (w/ Status)  │  │    - justs.  │  │ {time}.txt ││ │
│  └────────────────┘  └─────────────────┘  └──────────────┘  └────────────┘│ │
│                                                                             │ │
└─────────────────────────────────────────────────────────────────────────────┘ │
                                          │                                     │
                                          │ Queried/Applied by:                 │
                                          │                                     │
              ┌───────────────────────────┴──────────────┐                      │
              │                                          │                      │
              ▼                                          ▼                      │
┌─────────────────────────────┐       ┌──────────────────────────────────────┐ │
│  QUERY & REPORTING LAYER    │       │  SUPPRESS APPLICATOR (INTERACTIVE)   │ │
├─────────────────────────────┤       ├──────────────────────────────────────┤ │
│                             │       │                                      │◄┘
│ KnowledgeDatabaseQueryTool  │       │  scripts/apply_suppress_comments.py  │
│ • Load all databases        │       │  ┌────────────────────────────────┐  │
│ • Interactive queries       │       │  │ 1. Parse suppress file         │  │
│ • Filter & search           │       │  │ 2. Find source files           │  │
│ • Statistical analysis      │       │  │ 3. Check for comment blocks    │  │
│ • Export to Excel/JSON      │       │  │ 4. Show preview                │  │
│                             │       │  │ 5. Get user approval           │  │
│                             │       │  │ 6. Apply suppressions          │  │
│                             │       │  │ 7. Create backups in folder    │  │
│                             │       │  └────────────────────────────────┘  │
└─────────────────────────────┘       └──────────────────────────────────────┘
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
│  │  │   • Initialize directories (including justifications/)│ │  │
│  │  │   • Set up logging                                    │ │  │
│  │  │   • Set Qorix file path                               │ │  │
│  │  │                                                        │ │  │
│  │  │ check_git_integration()                               │ │  │
│  │  │   • Detect Git repository                            │ │  │
│  │  │   • Verify Git availability                          │ │  │
│  │  │                                                        │ │  │
│  │  │ get_git_info()                                        │ │  │
│  │  │   • Get branch name                                   │ │  │
│  │  │   • Get commit hash                                   │ │  │
│  │  │                                                        │ │  │
│  │  │ analyze_report(report_path, module_name, qorix_file)  │ │  │
│  │  │   • Parse Parasoft report                            │ │  │
│  │  │   • Load Qorix justifiable mapping                   │ │  │
│  │  │   • Apply justifiable status to violations           │ │  │
│  │  │   • Generate Excel report with status column         │ │  │
│  │  │   • Generate suppress comments for justified         │ │  │
│  │  │   • Analyze violations                               │ │  │
│  │  │   • Update knowledge base                            │ │  │
│  │  │                                                        │ │  │
│  │  │ _apply_justifiable_status(violations, mapping)        │ │  │
│  │  │   • Map status: Justified/Needs Code Update/Analyse  │ │  │
│  │  │                                                        │ │  │
│  │  │ _generate_suppress_comments(violations, module_name)  │ │  │
│  │  │   • Group by file and line                           │ │  │
│  │  │   • Generate Parasoft format comments                │ │  │
│  │  │   • Save to justifications/ folder                   │ │  │
│  │  │                                                        │ │  │
│  │  │ _generate_excel_report(violations, path, module)      │ │  │
│  │  │   • Create Summary sheet (with status counts)        │ │  │
│  │  │   • Create Unique Violations sheet                   │ │  │
│  │  │   • Create Detailed Violations sheet (w/ Status)     │ │  │
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
│ 4. FIX GENERATION LAYER (CodeFixGenerator.py + OllamaIntegration)  │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │ CodeFixGenerator Class (with AI Integration)                │  │
│  │ ────────────────────────────────────────────────────────────│  │
│  │                                                              │  │
│  │  Initialization:                                             │  │
│  │  ┌────────────────────────────────────────────────────────┐ │  │
│  │  │ __init__(module, kb_manager, fixes_dir, config)       │ │  │
│  │  │    ├─► Load config from config.json                   │ │  │
│  │  │    ├─► Initialize OllamaIntegration(ai_config)        │ │  │
│  │  │    ├─► Check AI status (enabled/disabled)             │ │  │
│  │  │    └─► Set up rule-based fallback                     │ │  │
│  │  └────────────────────────────────────────────────────────┘ │  │
│  │                                                              │  │
│  │  Fix Generation Pipeline (Hybrid AI + Rules):               │  │
│  │                                                              │  │
│  │  ┌────────────────────────────────────────────────────────┐ │  │
│  │  │ 1. generate_all_fixes(violation_ids)                  │ │  │
│  │  │    ├─► Get violations to fix                         │ │  │
│  │  │    ├─► Generate fix for each                         │ │  │
│  │  │    └─► Save to files                                 │ │  │
│  │  │                                                        │ │  │
│  │  │ 2. _generate_fix_for_violation(violation)             │ │  │
│  │  │    ├─► Determine fix type                            │ │  │
│  │  │    ├─► Get fix suggestion (AI/Rules)                 │ │  │
│  │  │    └─► Update knowledge base                         │ │  │
│  │  │                                                        │ │  │
│  │  │ 3. _get_fix_suggestion(violation_id, text, category)  │ │  │
│  │  │    │                                                  │ │  │
│  │  │    ├─► [AI PATH] - If should_use_ai():               │ │  │
│  │  │    │   ├─► ollama.generate_fix_suggestion()          │ │  │
│  │  │    │   ├─► Parse AI response (JSON/text)             │ │  │
│  │  │    │   └─► Return AI fix OR fallback to rules        │ │  │
│  │  │    │                                                  │ │  │
│  │  │    └─► [RULE-BASED PATH]:                            │ │  │
│  │  │        ├─► MISRA fixes (_get_misra_fix)              │ │  │
│  │  │        │   • Type conversions                        │ │  │
│  │  │        │   • Pointer handling                        │ │  │
│  │  │        │   • Control flow                            │ │  │
│  │  │        │   • Standard library                        │ │  │
│  │  │        │                                              │ │  │
│  │  │        ├─► CERT fixes (_get_cert_fix)                │ │  │
│  │  │        │   • Buffer safety                           │ │  │
│  │  │        │   • Null checks                             │ │  │
│  │  │        │   • Memory management                       │ │  │
│  │  │        │                                              │ │  │
│  │  │        └─► Generic fixes                             │ │  │
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
│  │                                                              │  │
│  │  ┌──────────────────────────────────────────────────────┐   │  │
│  │  │ OllamaIntegration Class (AI Component)              │   │  │
│  │  │ ───────────────────────────────────────────────────│   │  │
│  │  │                                                      │   │  │
│  │  │ __init__(config):                                   │   │  │
│  │  │    ├─► Read config (provider, model, base_url)      │   │  │
│  │  │    ├─► Test connection to Ollama server             │   │  │
│  │  │    ├─► Verify model availability (ollama.list())    │   │  │
│  │  │    └─► Set enabled status                           │   │  │
│  │  │                                                      │   │  │
│  │  │ should_use_ai(category, violation_text):            │   │  │
│  │  │    ├─► Check category rules (CERT, MISRA)           │   │  │
│  │  │    ├─► Check complexity heuristics                  │   │  │
│  │  │    └─► Return True/False                            │   │  │
│  │  │                                                      │   │  │
│  │  │ generate_fix_suggestion(violation):                 │   │  │
│  │  │    ├─► Build minimal prompt (200 words max)         │   │  │
│  │  │    ├─► Call ollama.generate(model, prompt)          │   │  │
│  │  │    ├─► Parse response (JSON preferred)              │   │  │
│  │  │    ├─► Fallback parser for free-text                │   │  │
│  │  │    └─► Return fix dict or None                      │   │  │
│  │  │                                                      │   │  │
│  │  │ _build_prompt(id, text, category, severity):        │   │  │
│  │  │    ├─► Format violation details                     │   │  │
│  │  │    ├─► Request JSON response                        │   │  │
│  │  │    └─► Keep under max_tokens limit                  │   │  │
│  │  │                                                      │   │  │
│  │  │ _parse_response(response, violation):               │   │  │
│  │  │    ├─► Extract JSON from response                   │   │  │
│  │  │    ├─► Validate required fields                     │   │  │
│  │  │    ├─► Add AI metadata (model, timestamp)           │   │  │
│  │  │    └─► Fallback to text extraction                  │   │  │
│  │  │                                                      │   │  │
│  │  │ get_status():                                        │   │  │
│  │  │    └─► Return enabled, provider, model, base_url    │   │  │
│  │  └──────────────────────────────────────────────────────┘   │  │
│  │                                                              │  │
│  │  Hybrid Decision Tree:                                      │  │
│  │  ┌──────────────────────────────────────────────────────┐   │  │
│  │  │ For each violation:                                 │   │  │
│  │  │   │                                                  │   │  │
│  │  │   ├─► Is AI enabled? Yes ─┐                         │   │  │
│  │  │   │                         ▼                        │   │  │
│  │  │   │                 Is complex/unknown?              │   │  │
│  │  │   │                 Yes ─┐        No ─┐             │   │  │
│  │  │   │                      ▼            ▼             │   │  │
│  │  │   │                  Use AI    Use Rules            │   │  │
│  │  │   │                      │            │             │   │  │
│  │  │   │                      └─► Success? ──┐           │   │  │
│  │  │   │                              │     Fail         │   │  │
│  │  │   │                             Yes     │           │   │  │
│  │  │   │                              │      ▼           │   │  │
│  │  │   │                              │  Fallback to     │   │  │
│  │  │   │                              │  Rule-based      │   │  │
│  │  │   │                              │      │           │   │  │
│  │  │   └─► No AI ─────────────────────┴──────┴──────►   │   │  │
│  │  │                                              │       │   │  │
│  │  │                                              ▼       │   │  │
│  │  │                                        Generate Fix  │   │  │
│  │  └──────────────────────────────────────────────────────┘   │  │
│  │                                                              │  │
│  │  Config Structure (config/config.json):                     │  │
│  │  ┌──────────────────────────────────────────────────────┐   │  │
│  │  │ {                                                    │   │  │
│  │  │   "ai_integration": {                               │   │  │
│  │  │     "enabled": true,                                │   │  │
│  │  │     "provider": "ollama",                           │   │  │
│  │  │     "ollama": {                                     │   │  │
│  │  │       "base_url": "http://localhost:11434",        │   │  │
│  │  │       "model": "phi3:mini",                         │   │  │
│  │  │       "timeout": 120,                               │   │  │
│  │  │       "fallback_to_rules": true                    │   │  │
│  │  │     },                                              │   │  │
│  │  │     "max_tokens": 1000,                             │   │  │
│  │  │     "temperature": 0.3,                             │   │  │
│  │  │     "use_ai_for": {                                 │   │  │
│  │  │       "complex_violations": true,                   │   │  │
│  │  │       "unknown_patterns": true,                     │   │  │
│  │  │       "cert_violations": false,                     │   │  │
│  │  │       "misra_violations": false                     │   │  │
│  │  │     }                                                │   │  │
│  │  │   }                                                  │   │  │
│  │  │ }                                                    │   │  │
│  │  └──────────────────────────────────────────────────────┘   │  │
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
│ 6. PARSER & QORIX INTEGRATION (ParasoftAnalysisTool.py)            │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │ HTML Report Parser + Qorix Deviations (v2.1.0)               │  │
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
│  │  │   ├─► Read "CERT Rule analysis" sheet               │ │  │
│  │  │   ├─► Read "MISRA Rule analysis" sheet              │ │  │
│  │  │   ├─► Find "Parasoft Message" column                │ │  │
│  │  │   ├─► Find "Justifiable" column                     │ │  │
│  │  │   └─► Create mapping: {violation_id: yes/no}        │ │  │
│  │  │                                                        │ │  │
│  │  │ resolve_justifiable(violation_id, mapping)            │ │  │
│  │  │   ├─► If Yes → "Justified"                          │ │  │
│  │  │   ├─► If No → "Needs Code Update"                   │ │  │
│  │  │   └─► If missing/Analyse → "Analysis Required"      │ │  │
│  │  └────────────────────────────────────────────────────────┘ │  │
│  └──────────────────────────────────────────────────────────────┘  │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│ 7. INTERACTIVE APPLICATOR (scripts/apply_suppress_comments.py)     │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌──────────────────────────────────────────────────────────────┐  │
│  │ SuppressCommentApplicator Class                              │  │
│  │ ────────────────────────────────────────────────────────────│  │
│  │                                                              │  │
│  │  Workflow:                                                   │  │
│  │  ┌────────────────────────────────────────────────────────┐ │  │
│  │  │ 1. parse_suppress_file(suppress_file_path)            │ │  │
│  │  │    ├─► Read {Module}_suppress_comments_{time}.txt    │ │  │
│  │  │    ├─► Extract file, line, begin/end comments        │ │  │
│  │  │    └─► Return list of suppressions                   │ │  │
│  │  │                                                        │ │  │
│  │  │ 2. find_source_file(file_name)                        │ │  │
│  │  │    ├─► Search recursively in target_repo             │ │  │
│  │  │    ├─► Handle multiple matches (user selection)      │ │  │
│  │  │    └─► Return file path                              │ │  │
│  │  │                                                        │ │  │
│  │  │ 3. is_inside_comment_block(lines, line_num)           │ │  │
│  │  │    ├─► Scan for /* and */ markers                    │ │  │
│  │  │    ├─► Track comment state                           │ │  │
│  │  │    └─► Return True if in comment block               │ │  │
│  │  │                                                        │ │  │
│  │  │ 4. show_preview(file, line, begin, end)               │ │  │
│  │  │    ├─► Show context (3 lines before/after)           │ │  │
│  │  │    ├─► Highlight target line                         │ │  │
│  │  │    ├─► Show proposed changes                         │ │  │
│  │  │    └─► Warn if inside comment block                  │ │  │
│  │  │                                                        │ │  │
│  │  │ 5. apply_suppression(file, line, begin, end)          │ │  │
│  │  │    ├─► Create backup in dedicated folder             │ │  │
│  │  │    ├─► Insert begin comment before line              │ │  │
│  │  │    ├─► Insert end comment after line                 │ │  │
│  │  │    └─► Write modified file                           │ │  │
│  │  │                                                        │ │  │
│  │  │ 6. create_backup(file_path)                           │ │  │
│  │  │    ├─► Create parasoft_backups_{timestamp}/ folder   │ │  │
│  │  │    ├─► Maintain directory structure                  │ │  │
│  │  │    └─► Copy original file                            │ │  │
│  │  │                                                        │ │  │
│  │  │ Interactive Options:                                  │ │  │
│  │  │   y = Apply this suppression                         │ │  │
│  │  │   n = Skip this suppression                          │ │  │
│  │  │   a = Apply all remaining (with validation)          │ │  │
│  │  │   q = Quit                                            │ │  │
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
│ STEP 3A: QORIX INTEGRATION & STATUS ASSIGNMENT                     │
│ ─────────────────────────────────────────────────────────────────── │
│                                                                     │
│  load_justifiable_mapping(qorix_file)                              │
│      │  [ParasoftAnalysisTool.py]                                  │
│      │                                                              │
│      ├─► Read Qorix_CP_Common_Deviations.xlsx                      │
│      ├─► Load "CERT Rule analysis" sheet                           │
│      ├─► Load "MISRA Rule analysis" sheet                          │
│      ├─► Find "Parasoft Message" column                            │
│      ├─► Find "Justifiable" column                                 │
│      └─► Return: {violation_id: "Yes"/"No"/"Analyse"}              │
│              │                                                      │
│              └─► _apply_justifiable_status(violations, mapping)    │
│                      │                                              │
│                      └─► FOR EACH violation:                       │
│                          ├─► IF mapping[id] == "Yes"               │
│                          │   └─► Set Status = "Justified"          │
│                          ├─► IF mapping[id] == "No"                │
│                          │   └─► Set Status = "Needs Code Update"  │
│                          └─► ELSE (missing/Analyse)                │
│                              └─► Set Status = "Analysis Required"  │
│                                                                     │
│  violations_with_status = [{                                        │
│    "Violation": "Buffer overflow...",                              │
│    "Violation ID": "CERT-C.STR31-C",                               │
│    "File": "handler.c",                                            │
│    "Line number": 145,                                             │
│    "Status": "Justified"  ← NEW FIELD                              │
│  }, ...]                                                           │
│                                                                     │
└────────────────────────────┬────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│ STEP 3B: EXCEL REPORT GENERATION                                   │
│ ─────────────────────────────────────────────────────────────────── │
│                                                                     │
│  _generate_excel_report(violations_with_status, path, module)      │
│      │                                                              │
│      ├─► Create Summary Sheet:                                     │
│      │   • Total Violations                                        │
│      │   • Unique Violation Types                                  │
│      │   • Files Affected                                          │
│      │   • Justified Count                                         │
│      │   • Needs Code Update Count                                 │
│      │   • Analysis Required Count                                 │
│      │                                                              │
│      ├─► Create Unique Violations Sheet:                           │
│      │   • Violation description                                   │
│      │   • Violation ID                                            │
│      │   • Count (sorted by frequency)                             │
│      │                                                              │
│      └─► Create Detailed Violations Sheet:                         │
│          • Violation                                                │
│          • Violation ID                                             │
│          • File                                                     │
│          • Line number                                              │
│          • Status (Justified/Needs Code Update/Analysis Required) ← │
│                                                                     │
│  OUTPUT: reports/Mka_violations_report.xlsx                        │
│                                                                     │
└────────────────────────────┬────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────────────┐
│ STEP 3C: SUPPRESS COMMENTS GENERATION                              │
│ ─────────────────────────────────────────────────────────────────── │
│                                                                     │
│  _generate_suppress_comments(violations_with_status, module)       │
│      │                                                              │
│      ├─► Filter violations where Status == "Justified"             │
│      │                                                              │
│      ├─► Group by (File, Line number)                              │
│      │                                                              │
│      ├─► FOR EACH location:                                        │
│      │   ├─► Collect all violation IDs at that line                │
│      │   ├─► Generate begin suppress comment:                      │
│      │   │   /* parasoft-begin-suppress ID1 ID2 "Reason: Ref" */   │
│      │   ├─► Generate end suppress comment:                        │
│      │   │   /* parasoft-end-suppress ID1 ID2 */                   │
│      │   └─► Write to output file                                  │
│      │                                                              │
│      └─► Save to: justifications/{Module}_suppress_comments_       │
│                   {timestamp}.txt                                   │
│                                                                     │
│  OUTPUT FILE FORMAT:                                               │
│  File: Mka_Internal.c, Line: 3454                                  │
│  ──────────────────────────────────────────────────────             │
│  /* parasoft-begin-suppress CERT_C-INT02-b-2 "Reason: Ref_3454" */ │
│  ... (your code at line 3454) ...                                  │
│  /* parasoft-end-suppress CERT_C-INT02-b-2 */                      │
│                                                                     │
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
│   ├── CodeFixGenerator.py       [Fix generator with AI - 475 lines]
│   ├── OllamaIntegration.py      [AI integration (Ollama) - 320 lines]
│   ├── KnowledgeDatabaseQueryTool.py [Query tool - 613 lines]
│   └── ParasoftAnalysisTool.py   [HTML parser - legacy]
│
├── 📂 SCRIPTS & LAUNCHERS
│   └── scripts/
│       ├── run_agent.py              [Main launcher script]
│       ├── run_query.py              [Query launcher script]
│       ├── apply_suppress_comments.py [Suppress applicator]
│       ├── capture_violations.py     [Violation capture tool]
│       ├── Run.bat                   [Windows automation]
│       └── Apply_Suppressions.bat    [Windows suppress applicator]
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

run_agent.py / run_query.py (scripts/)
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
