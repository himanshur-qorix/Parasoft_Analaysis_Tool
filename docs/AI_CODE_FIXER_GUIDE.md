# AI-Powered Interactive Code Fixer - Quick Guide

## 🚀 What's New?

The code fix generator now has an **AI-powered interactive mode** that **actually applies fixes to your source code files**! No more copy-pasting from suggestion documents - the AI understands your code and makes the changes directly.

## 🎯 Key Features

- **Actually modifies source files** - Not just suggestions!
- **AI understands context** - Uses the actual code surrounding violations
- **Interactive review** - Shows before/after diffs for approval
- **Safe with backups** - Automatic backups before any changes
- **Rollback support** - Can undo all changes if needed
- **Smart AI prompts** - Generates precise, compilable code

## 📋 Prerequisites

1. **Knowledge base created** - Run `Run.bat MODULE_NAME` first
2. **Violations enriched** - Run `Enrich_Violations.bat MODULE_NAME "SOURCE_PATH"` (or done automatically)
3. **Ollama running** - Make sure Ollama service is active: `ollama serve`
4. **Source code path** - Know where your source files are located

## 🔧 Usage

### Quick Start (Automatic Enrichment)

The easiest way - Apply_Fixes.bat will automatically enrich violations if needed:

```batch
Apply_Fixes.bat MODULE_NAME "PATH\TO\SOURCE"
```

### Manual Enrichment (Optional)

If you want to enrich violations first before applying fixes:

```batch
# Step 1: Enrich violations with source code context
Enrich_Violations.bat Mka "D:\Source\Mka"

# Step 2: Apply AI-powered fixes
Apply_Fixes.bat Mka "D:\Source\Mka"
```

```batch
Apply_Fixes.bat MODULE_NAME "PATH\TO\SOURCE"
```

**Examples:**
```batch
# Fix all violations in Mka module (auto-enrichment)
Apply_Fixes.bat Mka "D:\Source\Mka"

# Manually enrich first, then fix
Enrich_Violations.bat Mka "D:\Source\Mka"
Apply_Fixes.bat Mka "D:\Source\Mka"

# Fix specific violations only
Apply_Fixes.bat Mka "D:\Source\Mka" --violations CERT_C-STR31-a MISRAC2012-RULE_8_7-a

# Use AI-only mode for all fixes
Apply_Fixes.bat Mka "D:\Source\Mka" --ai-mode ai_only

# Use rules only (no AI)
Apply_Fixes.bat Mka "D:\Source\Mka" --ai-mode rules_only
```

### Method 2: Using Python Directly

```bash
# Activate environment first
venv\Scripts\activate

# Run the interactive fixer
python src\generate_code_fixes.py MODULE_NAME --apply-fixes --source-code "PATH\TO\SOURCE"
```

## 🎮 Interactive Workflow

When you run the fixer, for each violation you'll see:

1. **Violation Details**
   ```
   Violation: CERT_C-STR31-a
   Category: CERT
   Severity: HIGH
   Description: Guarantee that storage for strings has sufficient space...
   ```

2. **Current Code**
   ```c
   // Shows the code with the violation highlighted
   ```

3. **AI-Generated Fix**
   ```c
   // Shows what the AI proposes to change
   ```

4. **Diff Preview**
   ```diff
   - uint8_t value = myEnum;          // Before (red)
   + uint8_t value = (uint8_t)myEnum; // After (green)
   ```

5. **Your Choice**
   ```
   Apply this fix? [y=yes, n=no, e=edit, s=skip all]:
   ```

   - **y** - Apply the fix to the file
   - **n** - Skip this violation
   - **e** - Manually edit the fix before applying
   - **s** - Skip all remaining violations

## 📂 What Gets Created

After running the fixer:

```
backups/
  └─ 20260511_143022/        # Timestamp
     └─ Mka/                  # Your source files backup
        ├─ file1.c
        └─ file2.c

Mka_fix_history_20260511_143022.json  # Complete history of all changes
```

## ⚙️ AI Modes

### **ai_only**
- Uses AI for ALL violations
- Best for complex, context-dependent issues
- Requires Ollama running
- Most intelligent but slower

### **hybrid** (default, recommended)
- Uses Parasoft Rules Database first
- Falls back to AI for complex cases
- Best balance of speed and intelligence

### **rules_only**
- Uses only Parasoft DB + patterns
- No AI, faster
- Good for standard violations
- Works without Ollama

## 🛡️ Safety Features

### Automatic Backups
Every file modified is backed up before changes:
```
backups/TIMESTAMP/MODULE_NAME/original_files...
```

### Rollback Support
At the end, you can rollback all changes:
```
Need to rollback all changes? [y/n]: y
↩️  Rolled back 5 fixes
```

### Fix History
All changes are logged to JSON:
```json
{
  "timestamp": "2026-05-11T14:30:22",
  "backup_dir": "backups/20260511_143022",
  "fixes_applied": 5,
  "fixes": [
    {
      "violation_id": "CERT_C-STR31-a",
      "file": "D:\\Source\\Mka\\file.c",
      "backup": "backups/20260511_143022/Mka/file.c",
      "timestamp": "2026-05-11T14:30:45",
      "original": "...",
      "modified": "..."
    }
  ]
}
```

## 📊 Results Summary

After completion, you'll see:
```
================================================================================
  📊 FIX APPLICATION RESULTS
================================================================================
  ✅ Fixes applied: 5
  ❌ Fixes rejected: 2
  ⚠️  Fixes failed: 1
  📝 Manual fixes required: 1
  ⏭️  Violations skipped: 3

  📁 Backups saved to: backups/20260511_143022
  📄 Fix history: Mka_fix_history_20260511_143022.json
================================================================================
```

## 🔄 Recommended Workflow

1. **Analyze your code** first:
   ```batch
   Run.bat Mka
   ```

2. **Enrich violations** with source code (optional, done automatically):
   ```batch
   Enrich_Violations.bat Mka "D:\Source\Mka"
   ```

3. **Review violations** in the reports

4. **Apply AI fixes** interactively:
   ```batch
   Apply_Fixes.bat Mka "D:\Source\Mka"
   ```

5. **Re-analyze** to verify:
   ```batch
   Run.bat Mka
   ```

6. **Compare results** to see improvement

## 💡 Tips

### For Best Results

1. **Use specific violations** when testing:
   ```batch
   Apply_Fixes.bat Mka "D:\Source" --violations CERT_C-STR31-a
   ```

2. **Start with high-priority** violations

3. **Review each fix carefully** - the AI is smart but not perfect

4. **Keep backups** until you've verified the changes

5. **Use ai_only mode** for complex violations:
   ```batch
   Apply_Fixes.bat Mka "D:\Source" --ai-mode ai_only --violations COMPLEX-RULE-123
   ```

### Troubleshooting

**AI generation fails:**
- Check Ollama is running: `ollama list`
- Try with smaller context (specific violations)
- Fall back to rules_only mode

**File not found errors:**
- Run enrichment first: `Enrich_Violations.bat MODULE "SOURCE_PATH"`
- Ensure source code path is correct
- Check file paths in knowledge base match actual filesse match actual files

**No code context extracted:**
- Source code path may be incorrect
- File may have been moved/renamed

## 🆚 Old vs New Approach

### Old Approach (Rule-Based Suggestions)
```batch
python src\generate_code_fixes.py Mka
# Creates: Mka_fixes_TIMESTAMP.html
# Action: You manually copy/paste suggested fixes
```
- Generated generic suggestions
- Required manual code editing
- No context awareness
- Pattern-matching only

### New Approach (AI-Powered Interactive)
```batch
Apply_Fixes.bat Mka "D:\Source\Mka"
# Modifies: Your actual source files
# Shows: Real-time diffs and previews
# Creates: Automatic backups
```
- Understands actual code context
- AI generates precise fixes
- Applies changes automatically
- Interactive review process
- Safe with backups & rollback

## 📚 Examples

### Example 1: Fix All Violations
```batch
Apply_Fixes.bat Mka "D:\Projects\Automotive\Mka\src"
```

### Example 2: Fix Specific Category
```batch
# First, list CERT violations from knowledge base
python src\query_master_knowledge.py --module Mka --category CERT

# Then fix them
Apply_Fixes.bat Mka "D:\Projects\Automotive\Mka\src" --ai-mode ai_only
```

### Example 3: Test on One Violation
```batch
Apply_Fixes.bat Mka "D:\Source\Mka" --violations MISRAC2012-RULE_8_7-a-1
```

## ⚠️ Important Notes

1. **Always commit your code** before running the fixer
2. **Review all changes** - AI can make mistakes
3. **Test your code** after applying fixes
4. **Keep backups** until verified
5. **Re-run analysis** to confirm violations are resolved

## 🔗 Related Files

- `src/InteractiveCodeFixer.py` - Main fixer implementation
- `src/OllamaIntegration.py` - AI code generation (enhanced)
- `src/generate_code_fixes.py` - Entry point script
- `Apply_Fixes.bat` - Convenience batch file

## 🎓 Advanced Usage

### Custom AI Model
Edit `config/config.json`:
```json
{
  "ai_integration": {
    "provider": "ollama",
    "enabled": true,
    "ollama": {
      "model": "codellama:13b",  // Use larger model
      "base_url": "http://localhost:11434"
    }
  }
}
```

### Batch Processing with Approval File
For CI/CD or automated workflows, you could extend the tool to accept a pre-approved violations list.

---

**Need Help?** Check the main documentation or contact the development team.

**Version:** 4.0.0  
**Developer:** Himanshu R  
**Organization:** Qorix India Pvt Ltd
