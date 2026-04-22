# Rule-Based Pattern Learning System

## Overview

The enhanced justification pattern learning tool now includes **intelligent pattern learning without AI**. This means even without Ollama/LLM, the tool learns from your existing good justifications and identifies quality issues.

## Key Features

### 1. **Pattern Learning Database**
- Learns from human-written justifications
- Builds a knowledge base of good examples per rule
- Stores quality indicators and patterns
- Works completely offline (no AI/internet required)

### 2. **Quality Scoring System**
- Calculates quality score (0-10) for each justification
- Based on learned patterns and best practices
- Identifies specific weaknesses in tool-generated justifications

### 3. **Mistake Detection**
- Detects common mistakes without AI
- Categorizes issues: generic_reference, too_short, vague, missing_details
- Provides specific improvement recommendations

### 4. **Continuous Learning**
- Patterns improve as more modules contribute
- Stored in `knowledge_base/learned_patterns_db.json`
- Automatically loaded on subsequent runs

## How It Works

### Step 1: Classification
When analyzing justifications, the tool classifies each as:
- **Human-written**: Detailed, specific, contextual explanations
- **Tool-generated**: Generic references, short explanations

### Step 2: Pattern Extraction
From human-written justifications, the tool extracts:
```
For each rule:
  - Good example justifications
  - Average length
  - Common technical details
  - Quality indicators
  - Module context
```

### Step 3: Quality Analysis
Each tool-generated justification is scored on:

| Factor | Points | Description |
|--------|--------|-------------|
| **Length** | -2 to +2 | Too short (<30 chars) = -2, Detailed (>200 chars) = +2 |
| **Specific Details** | +2 | Contains function names, versions, hex addresses, DR numbers |
| **Explanation Words** | +1 | Uses "because", "required for", "approved", etc. |
| **Technical Context** | +1 | Mentions hardware, API, legacy, performance, safety |
| **Generic Reference** | -3 | Uses "_Parasoft_REF_NNN" pattern |

**Quality Score Range**: 0-10
- **0-3**: Poor quality, needs major improvement
- **4-6**: Acceptable, could be better
- **7-8**: Good quality
- **9-10**: Excellent, use as template

### Step 4: Mistake Identification
Low-quality justifications are categorized:

```python
Generic Reference (Score < 4.0 + has _Parasoft_REF_NNN):
  Issue: "Uses generic reference instead of specific explanation"
  
Too Short (Score < 4.0 + length < 30 chars):
  Issue: "Justification is too brief, lacks context"
  
Vague (Score < 4.0 + no specific details):
  Issue: "Lacks specific technical details or references"
```

### Step 5: Learning Storage
Patterns are saved to `knowledge_base/learned_patterns_db.json`:

```json
{
  "rule_patterns": {
    "MISRA2012-RULE-11_5": [
      {
        "reason": "Detailed human-written explanation...",
        "module": "Mka",
        "length": 185,
        "has_specific_details": true,
        "quality_score": 8.5
      }
    ]
  },
  "mistake_patterns": [
    {
      "type": "generic_reference",
      "example": "Reason: Mka_Parasoft_REF_001",
      "rules": "MISRA2012-RULE-8_9",
      "module": "Mka"
    }
  ],
  "quality_rules": [...]
}
```

## Using the Learning Database

### Module Teams: Contribute Your Patterns

1. **Add your module to `learning/` folder**
   ```bash
   # See learning/QUICK_START.md
   cp -r your_module/source learning/YourModule/Config/source/
   cp parasoft_report.xml learning/YourModule/Config/reports/
   ```

2. **Run pattern learning**
   ```bash
   scripts\Learn_Justification_Patterns.bat
   ```

3. **Review learned patterns**
   ```bash
   type knowledge_base\learned_patterns_db.json
   ```

### Tool Developers: Use Learned Patterns

The learned patterns can improve justification generation:

```python
from pathlib import Path
import json

# Load learned patterns
with open('knowledge_base/learned_patterns_db.json') as f:
    db = json.load(f)

# Get good examples for a rule
rule = "MISRA2012-RULE-11_5"
good_examples = db['rule_patterns'].get(rule, [])

# Filter high-quality examples (score >= 7.0)
high_quality = [p for p in good_examples if p['quality_score'] >= 7.0]

# Use as templates for new justifications
if high_quality:
    template = high_quality[0]['reason']
    # Adapt template to current context...
```

## Quality Indicators

### What Makes a Good Justification?

Based on learned patterns, high-quality justifications typically:

✅ **Specific Details**
```c
// ✓ GOOD: Contains specifics
// Reason: MKA module uses legacy CAN stack v2.1 API. The void pointer is 
// safely cast based on validated message type field. ASIL-D requirement 
// DR-2024-045.
// parasoft-suppress MISRA2012-RULE-11_5 "Legacy API compatibility"
```

✅ **Adequate Length** (>50 characters)
```c
// ✓ GOOD: Sufficient detail
// Reason: Global state required for interrupt-safe operation between main 
// context and CAN interrupt handler. Static allocation ensures predictable 
// memory usage per ASIL-D requirements.
```

✅ **Explanation Words** ("because", "required for", "due to")
```c
// ✓ GOOD: Explains WHY
// Reason: Pointer arithmetic needed for DMA buffer management because 
// hardware requires contiguous memory access. Validated in unit tests.
```

✅ **Technical Context**
```c
// ✓ GOOD: Shows technical context
// Reason: ARM Cortex-M4 hardware register at 0x40020000 requires volatile 
// pointer access. Memory-mapped I/O per platform specification.
```

### What to Avoid?

❌ **Generic References**
```c
// ✗ BAD: No explanation
// Reason: Mka_Parasoft_REF_001
// parasoft-suppress MISRA2012-RULE-11_5
```

❌ **Too Short**
```c
// ✗ BAD: Insufficient detail
// Reason: Required for API
```

❌ **Vague**
```c
// ✗ BAD: No specifics
// Reason: This is needed for the module to work properly
```

## Detailed Technical Indicators

The system recognizes these technical patterns:

| Pattern | Example | Points |
|---------|---------|--------|
| Function/Variable Names | `Mka_ProcessMessage()`, `g_StateFlag` | +2 |
| Version Numbers | `v2.1`, `API 3.0` | +2 |
| Hex Addresses | `0x40020000` | +2 |
| Deviation Requests | `DR-2024-045` | +2 |
| Safety Levels | `ASIL-D`, `ASIL-B` | +2 |
| Technical Measurements | `128KB`, `100μs`, `64 bytes` | +2 |
| Validation References | `tested in`, `validated via` | +2 |

## Continuous Improvement Workflow

```
┌─────────────────────────────────────────────────────────────┐
│  1. Teams add modules to learning/ folder                   │
└─────────────────┬───────────────────────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────────────────────┐
│  2. Run Learn_Justification_Patterns.bat                    │
│     - Analyzes all justifications                           │
│     - Classifies human vs tool-generated                    │
│     - Builds pattern database                               │
└─────────────────┬───────────────────────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────────────────────┐
│  3. Quality analysis (rule-based)                           │
│     - Scores each justification                             │
│     - Identifies specific issues                            │
│     - Categorizes mistakes                                  │
└─────────────────┬───────────────────────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────────────────────┐
│  4. Learned patterns saved                                  │
│     knowledge_base/learned_patterns_db.json                 │
└─────────────────┬───────────────────────────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────────────────────────────┐
│  5. Tool uses patterns for better suggestions               │
│     - Loads learned patterns                                │
│     - Uses high-quality examples as templates               │
│     - Avoids known mistake patterns                         │
└─────────────────────────────────────────────────────────────┘
```

## Report Outputs

### Console Report
Shows during execution:
```
📚 Building Pattern Database (Rule-Based Learning)...
   Analyzing 156 human-written justifications...
   ✅ Built pattern database for 42 rules
   ✅ Identified 23 mistake patterns
   ✅ Extracted 3 quality indicators
   
⚠️  QUALITY ISSUES DETECTED (Rule-Based Analysis)
   Generic Reference: 12 issues
      Example: Reason: Mka_Parasoft_REF_001...
      Issue: Uses generic reference instead of specific explanation
      Quality Score: 2.0/10
```

### JSON Database
`knowledge_base/learned_patterns_db.json`:
```json
{
  "timestamp": "2026-04-22T15:05:02.625740",
  "source": "rule_based_learning",
  "rule_patterns": {
    "MISRA2012-RULE-11_5": [...]
  },
  "mistake_patterns": [...],
  "quality_rules": [...],
  "statistics": {
    "total_rules_learned": 42,
    "total_mistakes_identified": 23,
    "human_written_analyzed": 156,
    "tool_generated_analyzed": 89
  }
}
```

### Pattern Report
`knowledge_base/justification_patterns_report.json`:
```json
{
  "quality_analysis": {
    "tool_generated_issues": {
      "generic_reference": 12,
      "too_short": 8,
      "vague": 3
    },
    "total_issues_found": 23,
    "issue_details": {...}
  }
}
```

## Integration with Tool

### In OllamaIntegration.py (Future)
```python
class OllamaIntegration:
    def __init__(self):
        # Load learned patterns for few-shot learning
        self.learned_db = self._load_learned_patterns()
    
    def generate_justification(self, rule, context):
        # Get high-quality examples for this rule
        examples = self.learned_db['rule_patterns'].get(rule, [])
        high_quality = [e for e in examples if e['quality_score'] >= 7.0]
        
        # Use as few-shot examples
        if high_quality:
            # Build prompt with examples
            prompt = self._build_few_shot_prompt(rule, high_quality, context)
            # Generate justification...
```

### In Justification Generator
```python
def validate_justification(justification):
    # Load learned patterns
    db = load_learned_patterns()
    
    # Calculate quality score
    score = calculate_quality_score(justification)
    
    # Check against known mistakes
    for mistake in db['mistake_patterns']:
        if matches_pattern(justification, mistake):
            return False, f"Matches known mistake: {mistake['type']}"
    
    # Require minimum quality
    if score < 5.0:
        return False, f"Quality score too low: {score}/10"
    
    return True, "Acceptable quality"
```

## Benefits

### Without AI (Rule-Based Learning)
- ✅ Works completely offline
- ✅ No external dependencies
- ✅ Fast execution
- ✅ Learns from your team's patterns
- ✅ Consistent quality enforcement
- ✅ Catches common mistakes

### With AI (Ollama Integration)
- ✅ All rule-based benefits +
- ✅ Natural language understanding
- ✅ Context-aware categorization
- ✅ More nuanced quality assessment
- ✅ Better few-shot learning

## Maintenance

### Regular Updates
Run pattern learning when:
- New modules added to `learning/` folder
- Coding standards updated
- After major code reviews
- Monthly quality audits

### Cleanup
Periodically review:
```bash
# Review learned patterns
python -c "import json; print(json.dumps(json.load(open('knowledge_base/learned_patterns_db.json')), indent=2))"

# Check for outdated patterns
# Remove low-quality examples
# Update quality thresholds
```

## Troubleshooting

### No Patterns Learned
**Cause**: No human-written justifications found  
**Solution**: 
1. Check suppression format matches expected pattern
2. Ensure justifications aren't all tool-generated
3. Add modules to `learning/` folder

### Low Quality Scores
**Cause**: Learned patterns are low quality  
**Solution**:
1. Review and improve existing justifications
2. Add well-documented modules to `learning/`
3. Follow quality guidelines in this document

### Pattern Database Empty
**Cause**: No matching justification format  
**Solution**:
1. Verify suppression comment format
2. Check regex pattern in `_analyze_file()`
3. Add debug logging to see what's being parsed

## Future Enhancements

Planned improvements:
1. **Learning from `learning/` folder**: Scan source code directly
2. **Module-specific patterns**: Different quality rules per module
3. **Auto-improvement**: Suggest specific improvements
4. **Web dashboard**: Visual quality reports
5. **CI/CD integration**: Automated quality gates

---

**Last Updated**: April 22, 2026  
**Version**: 2.0  
**Feature**: Rule-Based Pattern Learning (No AI Required)
