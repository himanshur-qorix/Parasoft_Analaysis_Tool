# Parasoft AI Agent - Architecture & Design

## System Architecture

```
┌───────────────────────────────────────────────────────────────────┐
│                     PARASOFT AI AGENT v2.0.0                      │
│                  (Intelligent Violation Analysis System)          │
└───────────────────────────────────────────────────────────────────┘

┌─────────────────────┐
│   INPUT LAYER       │
├─────────────────────┤
│ • report_dev1.html  │ ◄─── Parasoft HTML Report
│ • Module Name       │ ◄─── User Input (e.g., "Mka")
│ • Config.json       │ ◄─── Configuration Settings
└──────────┬──────────┘
           │
           ▼
┌─────────────────────────────────────────────────────────────────┐
│                    PROCESSING LAYER                             │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌────────────────────────────────────────────────────────┐    │
│  │         ParasoftAIAgent.py (Orchestrator)              │    │
│  │  • Initializes workspace                               │    │
│  │  • Detects Git repository                              │    │
│  │  • Coordinates all components                          │    │
│  │  • Manages workflow                                    │    │
│  └────────────┬───────────────────────────────────────────┘    │
│               │                                                 │
│    ┌──────────┼──────────┬─────────────────┬─────────────┐     │
│    │          │          │                 │             │     │
│    ▼          ▼          ▼                 ▼             ▼     │
│  ┌──────┐ ┌──────┐ ┌──────────┐  ┌─────────────┐  ┌────────┐  │
│  │ Git  │ │Parse │ │Knowledge │  │ Violation   │  │  Code  │  │
│  │Check │ │Report│ │Database  │  │  Analyzer   │  │  Fix   │  │
│  │      │ │      │ │Manager   │  │             │  │Generate│  │
│  └──────┘ └───┬──┘ └────┬─────┘  └──────┬──────┘  └───┬────┘  │
│               │         │               │              │        │
│         ┌─────┴─────────┴───────────────┴──────────────┘        │
│         │                                                        │
└─────────┼────────────────────────────────────────────────────────┘
          │
          ▼
┌─────────────────────────────────────────────────────────────────┐
│                    STORAGE LAYER                                │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌────────────────────┐  ┌─────────────────┐  ┌─────────────┐  │
│  │  knowledge_base/   │  │    reports/     │  │   fixes/    │  │
│  │  ──────────────────│  │  ───────────────│  │  ────────────│  │
│  │  • Mka_KB.json     │  │  • summary.json │  │  • Mka/     │  │
│  │  • ModB_KB.json    │  │  • statistics   │  │    - fixes  │  │
│  │  • Violations DB   │  │  • Git info     │  │    - justs  │  │
│  └────────────────────┘  └─────────────────┘  └─────────────┘  │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
          │
          ▼
┌─────────────────────────────────────────────────────────────────┐
│                    QUERY & OUTPUT LAYER                         │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│        ┌─────────────────────────────────────────┐              │
│        │ KnowledgeDatabaseQueryTool.py           │              │
│        │  • Interactive queries                  │              │
│        │  • Filtering & searching                │              │
│        │  • Statistics generation                │              │
│        │  • Export capabilities                  │              │
│        └────────────┬────────────────────────────┘              │
│                     │                                           │
│         ┌───────────┼───────────┐                              │
│         │           │           │                              │
│         ▼           ▼           ▼                              │
│    ┌────────┐  ┌────────┐  ┌────────┐                         │
│    │ Excel  │  │  JSON  │  │Console │                         │
│    │ Export │  │ Report │  │ Output │                         │
│    └────────┘  └────────┘  └────────┘                         │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
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
┌─────────────┐
│   User      │
└──────┬──────┘
       │ Runs: python src\run_agent.py report.html Mka
       ▼
┌─────────────────────────┐
│  ParasoftAIAgent        │
│  1. Check Git           │
│  2. Parse report        │◄── ParasoftAnalysisTool.py
│  3. Analyze violations  │──► ViolationAnalyzer.py
└────────┬────────────────┘
         │
         ▼
┌────────────────────────────────────┐
│  ViolationAnalyzer                 │
│  • Categorize violations           │
│  • Detect patterns                 │
│  • Estimate complexity             │
└────────┬───────────────────────────┘
         │
         ▼
┌────────────────────────────────────┐
│  KnowledgeDatabaseManager          │
│  • Load existing DB                │
│  • Add new violations              │
│  • Update occurrence counts        │
│  • Save updated DB                 │
└────────┬───────────────────────────┘
         │
         ▼
┌────────────────────────────────────┐
│  CodeFixGenerator                  │
│  • Generate fix suggestions        │
│  • Create justifications           │
│  • Save to files                   │
└────────┬───────────────────────────┘
         │
         ▼
┌─────────────────────────┐
│  OUTPUT                 │
│  • Knowledge DB         │
│  • Analysis summary     │
│  • Fixes file           │
│  • Justifications file  │
└─────────────────────────┘
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

