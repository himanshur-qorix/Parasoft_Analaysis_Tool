# Quick Start Guide: Contributing to Learning Database

**5-minute guide for module teams to contribute their code and reports for AI learning**

## Prerequisites

✅ Your module has been analyzed with Parasoft  
✅ Violations are either fixed or properly justified  
✅ Code is reviewed and ready to share  

## Step 1: Create Your Module Folder (2 minutes)

```bash
cd learning/
mkdir YourModuleName
cd YourModuleName
mkdir YourConfig
cd YourConfig
mkdir source reports
```

**Result:**
```
learning/
└── YourModuleName/
    └── YourConfig/
        ├── source/
        └── reports/
```

## Step 2: Copy Source Files (1 minute)

Copy your reviewed source files to `source/`:

```bash
# Copy from your module location
cp /path/to/your/module/*.c source/
cp /path/to/your/module/*.h source/
```

**Include:**
- Main implementation files (.c)
- Header files (.h)
- Files with good justification examples

**Limit:** 10-20 representative files (not entire codebase)

## Step 3: Export Parasoft Reports (1 minute)

### Option A: From Parasoft GUI
1. Open your Parasoft project
2. `File > Export > Report`
3. Select `XML (Full Report)`
4. Save as `YourModuleName_parasoft_report.xml` in `reports/`

### Option B: Copy Existing Reports
```bash
# If reports already exist
cp /path/to/parasoft/report.xml reports/YourModuleName_parasoft_report.xml
```

### Extract Suppression Comments
```bash
# From project root, run:
python src/interactive_viewer.py --extract-justifications --module YourModuleName
```

This generates: `justifications/YourModuleName_suppress_comments_YYYYMMDD.txt`

Copy to learning folder:
```bash
cp justifications/YourModuleName_suppress_comments_*.txt learning/YourModuleName/YourConfig/reports/
```

## Step 4: Add Module README (1 minute)

Create `README.md` in `YourModuleName/YourConfig/`:

```markdown
# YourModuleName Configuration

## Description
Brief description of your module (2-3 sentences)

## Common Justifications
- **MISRA2012-RULE-X**: Reason why this is commonly justified
- **CERT-XXX**: Another common justification scenario

## Special Considerations
- Hardware constraints
- Legacy API requirements
- Performance requirements

## Contact
Owner: Your Team Name
Last Updated: YYYY-MM-DD
```

**Copy template:**
```bash
cp learning/Mka/Mka_Config_1/README.md learning/YourModuleName/YourConfig/README.md
# Edit with your module details
```

## Step 5: Verify Structure

Check your folder looks like this:

```
learning/
└── YourModuleName/
    └── YourConfig/
        ├── source/
        │   ├── File1.c
        │   ├── File1.h
        │   └── ...
        ├── reports/
        │   ├── YourModuleName_parasoft_report.xml
        │   └── YourModuleName_suppress_comments_YYYYMMDD.txt
        └── README.md
```

**Verify script:**
```bash
# Run verification
python src/verify_learning_structure.py --module YourModuleName
```

## Step 6: Commit and Push

```bash
# Add files to git
git add learning/YourModuleName/

# Commit
git commit -m "Add YourModuleName to AI learning database"

# Push
git push origin main
```

## That's It! 🎉

Your module is now part of the AI learning database.

## What Happens Next?

1. **AI Processing**: The tool automatically scans your contribution
2. **Pattern Learning**: AI learns from your justification style
3. **Knowledge Update**: Patterns added to knowledge base
4. **Tool Improvement**: Future suggestions improve based on your examples

## See AI Learning Results

```bash
# Run pattern learning
scripts\Learn_Justification_Patterns.bat

# Check the report
type knowledge_base\justification_patterns_report.json
```

## Updating Your Contribution

When your module changes:

```bash
# Update source files
cp /path/to/updated/file.c learning/YourModuleName/YourConfig/source/

# Export new Parasoft report
# (Follow Step 3 again)

# Re-extract suppressions
python src/interactive_viewer.py --extract-justifications --module YourModuleName

# Commit updates
git add learning/YourModuleName/
git commit -m "Update YourModuleName learning examples"
git push
```

## Troubleshooting

### "Reports not found"
- Ensure XML report is in `reports/` folder
- Check filename matches pattern: `*_parasoft_report.xml`

### "No suppressions extracted"
- Verify source code has suppression comments
- Check format: `// parasoft-suppress RULE "reason"`
- Run extraction manually with debug flag

### "Structure validation failed"
- Run: `python src/verify_learning_structure.py --module YourModuleName --verbose`
- Check error messages for missing files/folders

## Best Practices

✅ **DO:**
- Include well-justified code
- Use descriptive reasons, not generic references
- Update when standards change
- Keep files under 10MB each

❌ **DON'T:**
- Include experimental/WIP code
- Use generic justifications like "Parasoft_REF_001"
- Add entire codebase (select representative files)
- Commit sensitive/proprietary code without approval

## Example: Mka Module

See `learning/Mka/Mka_Config_1/` for a complete example showing:
- Proper folder structure
- Well-documented justifications
- Comprehensive README
- Quality report files

## Questions?

- Check main documentation: `learning/README.md`
- Review example: `learning/Mka/Mka_Config_1/`
- Contact: Static Analysis Tool Team

---

**Time to contribute: ~5 minutes**  
**Benefit: Improved AI suggestions for your entire team**  
**Last Updated**: April 22, 2026
