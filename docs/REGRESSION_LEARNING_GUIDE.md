# Regression Learning System Guide

## 🎯 Overview

The **Regression Learning System** is an intelligent feedback mechanism that learns from mistakes to continuously improve MISRA/CERT violation detection and suppression accuracy. It implements a feedback loop where the tool learns from:

- **False Positives** - Violations incorrectly flagged
- **False Negatives** - Violations missed by the tool
- **Correct Detections** - Violations correctly identified (for reinforcement)
- **Suppression Errors** - Incorrect suppression formats or applications

---

## 🚀 Key Features

### 1. **Automated Learning**
- Learns patterns from false positives to avoid future mistakes
- Generates new detection patterns from missed violations
- Adjusts confidence scores based on historical accuracy
- Creates exclusion rules for common false positive scenarios

### 2. **Pattern Refinement**
- Extracts features from code snippets (keywords, operators, structure)
- Builds exclusion patterns for false positives
- Generates additional detection patterns for false negatives
- Tracks confidence adjustments per rule

### 3. **Suppression Learning**
- Learns correct suppression comment formats
- Tracks module-specific reference patterns
- Identifies common suppression errors
- Improves reference ID generation

### 4. **Interactive Feedback**
- GUI for reviewing and providing feedback on violations
- Quick navigation through detected violations
- Easy classification (correct/false positive/false negative)
- Activity logging and statistics

---

## 📊 Architecture

```
┌──────────────────────────────────────────────────────────────┐
│                    REGRESSION LEARNING SYSTEM                │
└──────────────────────────────────────────────────────────────┘
                               │
          ┌────────────────────┴────────────────────┐
          │                                         │
┌─────────▼─────────┐                   ┌──────────▼──────────┐
│  FeedbackLearner  │                   │   Feedback GUI      │
│  (Core Engine)    │                   │  (User Interface)   │
└─────────┬─────────┘                   └──────────┬──────────┘
          │                                         │
          │  Learns patterns from feedback          │
          │  Stores in learning database            │
          │  Refines detection rules                │
          │                                         │
┌─────────▼─────────────────────────────────────────▼──────────┐
│                   INTEGRATION POINTS                          │
├───────────────────────────────────────────────────────────────┤
│  1. StaticCodeAnalyzer - Skip learned false positives        │
│  2. StaticCodeAnalyzer - Adjust confidence scores            │
│  3. CodeFixGenerator - Improve suppression formats           │
│  4. Knowledge Base - Store learning data                     │
└───────────────────────────────────────────────────────────────┘
```

---

## 📁 Learning Data Structure

### Feedback Database
**Location:** `learning/feedback/feedback_database.json`

```json
{
  "metadata": {
    "version": "1.0.0",
    "created": "2026-04-30T10:00:00",
    "total_feedback": 42
  },
  "feedback_entries": [
    {
      "feedback_id": "CERT_ARR30_Mka.c_265_20260430120000",
      "feedback_type": "false_positive",
      "rule_id": "CERT_ARR30",
      "code_snippet": "uint8 buffer[MAX_SIZE];",
      "file_path": "Mka.c",
      "line_number": 265,
      "reason": "Buffer size is constant defined by AUTOSAR spec",
      "module_name": "Mka",
      "pattern_used": "",
      "context": {"category": "CERT", "severity": "HIGH"},
      "timestamp": "2026-04-30T12:00:00"
    }
  ],
  "statistics": {
    "false_positive": 15,
    "false_negative": 8,
    "correct": 18,
    "suppression_error": 1
  }
}
```

### Pattern Refinements
**Location:** `learning/feedback/pattern_refinements.json`

```json
{
  "CERT_ARR30": {
    "false_positive_patterns": [
      {
        "pattern": {"has_macros": true, "keywords": ["const", "define"]},
        "code_snippet": "uint8 buffer[MAX_SIZE];",
        "reason": "Constant array size from macro",
        "timestamp": "2026-04-30T12:00:00"
      }
    ],
    "exclusion_patterns": [
      {
        "pattern": "#define\\s+\\w+",
        "reason": "Exclude macro definitions",
        "confidence": 0.8
      }
    ],
    "false_negative_patterns": [],
    "additional_patterns": [],
    "confidence_adjustment": -0.3
  }
}
```

### Suppression Learning
**Location:** `learning/feedback/suppression_learning.json`

```json
{
  "Mka_CERT_C-DCL00": {
    "errors": [
      {
        "code_snippet": "/* parasoft-suppress DCL00 */",
        "reason": "Missing 'CERT_C-' prefix",
        "context": {"rule": "CERT_C-DCL00-b-3"},
        "timestamp": "2026-04-30T12:00:00"
      }
    ],
    "corrections": [],
    "patterns_to_avoid": [
      {
        "error_type": "suppression_format",
        "module": "Mka",
        "rule": "CERT_C-DCL00",
        "lesson": "Always use full rule ID with prefix"
      }
    ]
  }
}
```

---

## 🔧 Usage

### 1. **Enable Learning in Static Analyzer**

```python
from StaticCodeAnalyzer import StaticCodeAnalyzer

# Create analyzer with feedback learning enabled
analyzer = StaticCodeAnalyzer(
    load_parasoft_db=True,
    load_knowledge=True,
    enable_feedback_learning=True  # ✅ Enable learning
)

# Analyze code
violations = analyzer.analyze_directory(Path('src/'))

# Review violations and provide feedback
analyzer.mark_false_positive(
    violation_id="STATIC-CERT_ARR30-1",
    reason="Array size is defined by AUTOSAR standard constant"
)

analyzer.mark_false_negative(
    rule_id="MISRA_10_3",
    code_snippet="result = enum_value;",
    file_path="Mka.c",
    line_number=142,
    reason="Implicit enum conversion not detected"
)

analyzer.mark_correct_detection(
    violation_id="STATIC-CERT_STR31-2",
    reason="Correctly identified buffer overflow risk"
)

# Generate learning report
report = analyzer.generate_feedback_report()
print(report)

# Export learning data
exported_files = analyzer.export_learning_data()
```

### 2. **Using the Feedback GUI**

```bash
# Launch the feedback GUI
python src/feedback_gui.py
```

**GUI Features:**
- 📂 Load violations from knowledge base
- ⬅️➡️ Navigate through violations
- ✅ Mark as correct detection
- ❌ Mark as false positive
- ⚠️ Add false negative
- 💾 Export learning data
- 📊 View learning statistics

### 3. **Command-Line Feedback**

```python
from FeedbackLearner import FeedbackLearner, FeedbackType
from pathlib import Path

# Initialize learner
project_root = Path(".")
learner = FeedbackLearner(project_root)

# Add false positive feedback
learner.add_feedback(
    feedback_type=FeedbackType.FALSE_POSITIVE,
    rule_id="MISRA_20_1",
    code_snippet="// Some comment\n#include \"header.h\"",
    file_path="test.c",
    line_number=42,
    reason="Include appears in comment, not actual code",
    module_name="TestModule"
)

# Check if should skip detection (based on learning)
should_skip = learner.should_skip_detection("MISRA_20_1", "// #include")
print(f"Skip detection: {should_skip}")

# Get confidence adjustment
adjustment = learner.get_confidence_adjustment("MISRA_20_1")
print(f"Confidence adjustment: {adjustment:+.2f}")

# Generate report
report = learner.generate_feedback_report()
print(report)
```

---

## 🔄 Learning Workflow

### Step 1: **Initial Analysis**
```
Static Analyzer detects violations → Generates violation reports
```

### Step 2: **Manual Review**
```
User reviews violations using GUI or code review
   ↓
Identifies false positives, false negatives, or confirms correct
   ↓
Provides feedback through GUI or API
```

### Step 3: **Pattern Learning**
```
FeedbackLearner extracts features from code
   ↓
Generates exclusion patterns (for false positives)
   ↓
Creates additional detection patterns (for false negatives)
   ↓
Adjusts confidence scores (±0.1 per feedback)
```

### Step 4: **Next Analysis**
```
Static Analyzer checks learned patterns
   ↓
Skips violations matching false positive patterns
   ↓
Applies additional patterns for better detection
   ↓
Adjusts severity based on confidence scores
```

---

## 📈 Learning Metrics

### Confidence Adjustment
- **Range:** -1.0 to +1.0
- **False Positive:** -0.1 per occurrence (makes detection more conservative)
- **False Negative:** +0.1 per occurrence (makes detection more aggressive)
- **Effect:** Confidence < -0.3 downgrades severity level

### Severity Downgrade Example
```python
# Original severity: CRITICAL
# After 4 false positive reports: confidence = -0.4
# New severity: HIGH (downgraded)
```

### Pattern Matching
- **Similarity Threshold:** 70%
- **Features Compared:** Keywords, operators, structure, comments, macros
- **Exclusion Confidence:** 80% (patterns are reliable)

---

## 🎓 Example Scenarios

### Scenario 1: False Positive - AUTOSAR Constants

**Initial Detection:**
```c
uint8 buffer[MAX_BUFFER_SIZE];  // Flagged: CERT_ARR30 (array bounds)
```

**User Feedback:**
```
FALSE POSITIVE
Reason: MAX_BUFFER_SIZE is AUTOSAR standard constant, always valid
```

**Learning Outcome:**
- Exclusion pattern added: `#define\s+MAX_\w+`
- Confidence adjustment: -0.1
- Future occurrences with AUTOSAR macros skipped

---

### Scenario 2: False Negative - Enum Conversion

**Missed Detection:**
```c
MyEnum_t result = some_integer_value;  // Not flagged: MISRA_10_3
```

**User Feedback:**
```
FALSE NEGATIVE
Reason: Implicit conversion from integer to enum should be detected
```

**Learning Outcome:**
- New pattern added: `\w+_t\s+\w+\s*=\s*\w+(?!_)`
- Confidence adjustment: +0.1
- Similar patterns detected in future analyses

---

### Scenario 3: Suppression Format Error

**Incorrect Suppression:**
```c
/* parasoft-suppress DCL00 */  // Missing CERT_C- prefix
```

**User Feedback:**
```
SUPPRESSION ERROR
Reason: Should be CERT_C-DCL00-b-3, not just DCL00
```

**Learning Outcome:**
- Lesson stored: Always use full rule ID with standard prefix
- Pattern to avoid: Shortened rule IDs
- Future suppressions verified against learned format

---

## 📊 Reports

### Generate Learning Report

```python
learner = FeedbackLearner(project_root)
report = learner.generate_feedback_report()
```

**Sample Output:**
```
================================================================================
FEEDBACK LEARNING SYSTEM REPORT
================================================================================

Generated: 2026-04-30 14:30:00
Total Feedback Entries: 42

STATISTICS:
-----------
False Positives: 15 (35.7%)
False Negatives: 8 (19.0%)
Correct Detections: 18 (42.9%)
Suppression Errors: 1 (2.4%)

PATTERN REFINEMENTS:
--------------------
Rules with refinements: 8

CERT_ARR30:
  - False positive patterns learned: 3
  - False negative patterns learned: 0
  - Confidence adjustment: -0.30
  - Exclusion patterns: 2
  - Additional patterns: 0

MISRA_10_3:
  - False positive patterns learned: 1
  - False negative patterns learned: 2
  - Confidence adjustment: +0.10
  - Exclusion patterns: 1
  - Additional patterns: 2

... (more rules)

================================================================================
```

---

## 🔍 Monitoring Learning Progress

### Check Statistics
```python
stats = learner.feedback_db['statistics']
print(f"False Positives: {stats['false_positive']}")
print(f"False Negatives: {stats['false_negative']}")
print(f"Correct: {stats['correct']}")
```

### Review Pattern Refinements
```python
for rule_id, refinements in learner.pattern_refinements.items():
    fp_count = len(refinements.get('false_positive_patterns', []))
    fn_count = len(refinements.get('false_negative_patterns', []))
    conf_adj = refinements.get('confidence_adjustment', 0.0)
    
    print(f"{rule_id}: FP={fp_count}, FN={fn_count}, Conf={conf_adj:+.2f}")
```

### Export for Analysis
```python
exported = learner.export_learning_data()
# Creates timestamped files in learning/feedback/export/
# - feedback_db_YYYYMMDD_HHMMSS.json
# - patterns_YYYYMMDD_HHMMSS.json
# - suppression_YYYYMMDD_HHMMSS.json
# - report_YYYYMMDD_HHMMSS.txt
```

---

## 🛠️ Integration with GUI

The main Parasoft GUI now includes feedback capabilities:

### From Violations Tab:
```python
# In parasoft_gui.py - add feedback button
def mark_selected_violation_false_positive():
    # Get selected violation
    violation_id = get_selected_violation_id()
    reason = ask_user_for_reason()
    
    # Mark as false positive
    self.analyzer.mark_false_positive(violation_id, reason)
    
    # Refresh display
    self.refresh_violations_list()
```

---

## 💡 Best Practices

### 1. **Regular Feedback**
- Review violations after each analysis session
- Provide feedback on at least 10-20 violations per module
- Focus on patterns that appear frequently

### 2. **Detailed Reasons**
- Explain WHY it's a false positive/negative
- Mention context (AUTOSAR standards, project conventions, etc.)
- Reference documentation when applicable

### 3. **Pattern Verification**
- After adding feedback, run analysis again to verify improvements
- Check that false positives are no longer flagged
- Confirm false negatives are now detected

### 4. **Export and Backup**
- Export learning data regularly
- Version control the learning databases
- Share learning data across team for consistency

### 5. **Monitor Confidence**
- Rules with confidence < -0.5 may need pattern review
- Rules with many false positives should be refined
- Rules with many false negatives need additional patterns

---

## 🔬 Advanced: Custom Pattern Features

### Extracting Custom Features
```python
def _extract_custom_features(code_snippet: str) -> Dict:
    """Extract custom features for your project"""
    features = {
        'has_autosar_keywords': bool(re.search(r'\b(AUTOSAR|STD_ON|STD_OFF)\b', code)),
        'has_safety_comment': '/* SAFETY */' in code,
        'function_depth': code.count('{') - code.count('}'),
        # Add your custom features
    }
    return features
```

### Custom Exclusion Logic
```python
def _custom_should_skip(self, rule_id: str, code: str) -> bool:
    """Custom skip logic for project-specific patterns"""
    if rule_id == 'MISRA_8_9':
        if '/* GLOBAL */' in code:
            return True  # Skip if marked as intentional global
    
    return False
```

---

## 📞 Support & Troubleshooting

### Learning Not Working
1. Check `enable_feedback_learning=True` in analyzer
2. Verify FeedbackLearner module is imported correctly
3. Check learning database permissions (read/write access)
4. Review logs for warning messages

### Patterns Not Matching
1. Lower similarity threshold (default: 70%)
2. Add more feedback entries for pattern
3. Verify feature extraction is working correctly
4. Check exclusion patterns for conflicts

### High False Positive Rate
1. Review confidence adjustments
2. Add more exclusion patterns
3. Refine detection patterns
4. Consider project-specific customizations

---

## 🎯 Summary

The Regression Learning System provides:
- ✅ **Continuous Improvement** - Learns from every feedback
- ✅ **Pattern Recognition** - Identifies common false positive/negative patterns
- ✅ **Automated Refinement** - Adjusts detection rules automatically
- ✅ **User-Friendly** - Interactive GUI for easy feedback
- ✅ **Transparent** - Full visibility into learning data and decisions
- ✅ **Exportable** - Learning data can be shared and version controlled

---

**Version:** 1.0.0  
**Last Updated:** April 30, 2026  
**Developer:** Himanshu R  
**Organization:** Qorix India Pvt Ltd
