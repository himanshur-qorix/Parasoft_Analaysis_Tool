# Learning Database for AI Pattern Recognition

This folder contains source code and Parasoft violation reports from various modules to train the AI system on proper justification patterns.

## Purpose

The AI tool learns from real examples to:
- Understand proper justification formats
- Learn module-specific coding patterns
- Recognize when violations should be justified vs fixed
- Improve suggestion quality based on actual codebase context

## Folder Structure

```
learning/
├── ModuleName/
│   ├── ConfigName/
│   │   ├── source/              # Source code files (.c, .h)
│   │   ├── reports/             # Parasoft reports (.xml, .json)
│   │   └── README.md            # Module-specific notes
│   └── ...
└── README.md                    # This file
```

## How to Contribute Your Module

### 1. Create Module Folder

```bash
learning/
└── YourModuleName/
    └── YourConfig/
```

### 2. Add Source Code

Place your source files in the `source/` subfolder:

```
learning/YourModuleName/YourConfig/source/
├── module_file1.c
├── module_file1.h
├── module_file2.c
└── module_file2.h
```

**What to include:**
- Representative source files with good coding practices
- Files that have been properly reviewed and justified
- Both .c and .h files for context

### 3. Add Parasoft Reports

Place Parasoft analysis reports in the `reports/` subfolder:

```
learning/YourModuleName/YourConfig/reports/
├── parasoft_report.xml          # Standard Parasoft XML report
├── violations_summary.json      # Optional: JSON format
└── suppression_comments.txt     # Justification comments
```

**Required reports:**
- **Parasoft XML Report**: Full analysis with violations
- **Suppression Comments**: Justifications for accepted violations
- **Optional**: Custom violation exports in JSON format

### 4. Document Module Context

Create a `README.md` in your module folder:

```markdown
# ModuleName Configuration

## Module Description
Brief description of what this module does.

## Coding Standards
- MISRA C 2012
- CERT C
- Custom project rules

## Common Justifications
- **Rule X**: Usually justified because [reason]
- **Rule Y**: Approved deviation for [scenario]

## Special Considerations
- Module uses legacy API (justifies certain deviations)
- Hardware-specific constraints
- Performance requirements

## Contact
Module Owner: [Name/Team]
Last Updated: YYYY-MM-DD
```

## Example: Mka Module

```
learning/
└── Mka/
    └── Mka_Config_1/
        ├── source/
        │   ├── Mka_Main.c
        │   ├── Mka_Types.h
        │   └── Mka_Utils.c
        ├── reports/
        │   ├── Mka_parasoft_report.xml
        │   ├── Mka_violations.json
        │   └── Mka_suppress_comments_20260422.txt
        └── README.md
```

## AI Learning Process

Once you commit your module:

1. **Pattern Recognition**: AI analyzes your justifications
2. **Context Learning**: Understands module-specific patterns
3. **Quality Assessment**: Learns what makes good vs poor justifications
4. **Few-Shot Training**: Uses your examples for future suggestions

## Quality Guidelines

### Good Learning Examples

✅ **Include:**
- Well-justified violations with clear reasoning
- Properly documented deviations
- Context-rich explanations
- Consistent coding style

❌ **Avoid:**
- Generic justifications like "Parasoft_REF_123"
- Unjustified violations
- Temporary/experimental code
- Auto-generated boilerplate

### Justification Quality Standards

**Good Example:**
```c
// Reason: MKA module uses legacy API for backward compatibility with CAN 
// stack v2.1. The void pointer is safely cast based on message type field 
// which is validated in Mka_ValidateMessage(). MISRA deviation approved 
// in DR-2024-045.
// parasoft-suppress MISRA2012-RULE-11_5 "Legacy API compatibility"
```

**Poor Example:**
```c
// Reason: Mka_Parasoft_REF_001
// parasoft-suppress MISRA2012-RULE-11_5
```

## Usage by AI Tool

The tool automatically scans this folder structure to:

```bash
# Learn from all modules
python src/learn_justification_patterns.py --learning-mode

# Update knowledge base
python src/OllamaIntegration.py --build-knowledge-base
```

The learned patterns are stored in:
- `knowledge_base/justification_patterns_report.json`
- `knowledge_base/Master_KnowledgeDatabase.json`

## Benefits

1. **Consistent Quality**: AI learns your team's standards
2. **Module-Specific**: Understands context per module
3. **Evolving Intelligence**: Continuously improves with more examples
4. **Team Knowledge**: Captures institutional knowledge
5. **Onboarding**: New team members learn from examples

## Automated Learning

The system can be integrated into CI/CD:

```yaml
# .github/workflows/ai-learning.yml
- name: Update AI Knowledge Base
  run: |
    python src/learn_justification_patterns.py --learning-mode
    git add knowledge_base/
    git commit -m "Update AI knowledge base"
```

## File Format Requirements

### Source Code
- **Format**: .c, .h, .cpp, .hpp
- **Encoding**: UTF-8
- **Line Endings**: LF or CRLF (auto-detected)

### Parasoft Reports
- **XML**: Standard Parasoft 10.x+ format
- **JSON**: Custom export with violation details
- **TXT**: Suppression comments extracted from source

### Size Limits
- Source files: < 10 MB per file
- Reports: < 50 MB per report
- Total per module: < 200 MB

## Maintenance

### Regular Updates
- Update when coding standards change
- Refresh after major refactoring
- Add new modules as they mature

### Cleanup
- Remove deprecated modules
- Archive old configurations
- Consolidate duplicate examples

## Support

For questions or issues:
1. Check documentation in `docs/AI_QUICKSTART.md`
2. Review examples in existing module folders
3. Contact the Static Analysis team

---

**Last Updated**: April 22, 2026  
**Version**: 1.0  
**Owner**: Static Analysis Tool Team
