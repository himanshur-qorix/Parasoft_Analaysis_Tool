# Quick Start: Few-Shot Learning for Justifications

## 🚀 Getting Started in 3 Steps

### Step 1: Verify Configuration (Already Done!)
The feature is **already enabled** in your `config/config.json`:

```json
"ollama": {
  "use_few_shot_learning": true  // ✅ Already set!
}
```

### Step 2: Ensure Example Files Exist
Check that you have justification examples in the `justifications/` folder:
- ✅ `Mka_suppress_comments_*.txt`
- ✅ `NvM_suppress_comments_*.txt`
- ✅ `SecOC_suppress_comments_*.txt`

**You already have these!** The system will automatically learn from them.

### Step 3: Run Analysis and See the Improvement

```powershell
# Run your normal analysis
python src/ParasoftAnalysisTool.py <module_name>

# Or generate code fixes
python src/generate_code_fixes.py <module_name>
```

## ✨ What You'll See

### In the Logs
```
[OK] Loaded 8 justification examples for few-shot learning
[AI] Analyzing justification recommendation for MISRAC2012-RULE_8_13-a-4...
[OK] Justification analysis: JUSTIFY (confidence: HIGH)
```

### In the Excel Report
New sheet: **"AI Justification Suggestions"**

**Before (Rule-Based):**
| Violation ID | Reason | Suggested Rationale |
|--------------|--------|---------------------|
| MISRAC2012-RULE_8_13-a-4 | Common deviation | Common deviation - consistent with other modules |

**After (Few-Shot Learning):**
| Violation ID | Reason | Suggested Rationale |
|--------------|--------|---------------------|
| MISRAC2012-RULE_8_13-a-4 | API compatibility - matches NvM, SecOC | Parameter constness deviation required for backward compatibility with legacy AUTOSAR APIs - consistent with NvM module pattern |

## 🎯 Expected Benefits

✅ **More Descriptive**: Real reasons instead of generic "Reason: Module_REF_LineNum"  
✅ **Cross-Module Consistency**: References patterns from other modules  
✅ **Context-Aware**: Understands API constraints, legacy code, design decisions  
✅ **Standard Format**: Follows the parasoft-begin/end-suppress format  
✅ **Time Savings**: Auto-generates meaningful justifications

## 🔍 Verify It's Working

### Quick Check
Run this command to see recent logs:
```powershell
Select-String -Path "parasoft_agent.log" -Pattern "justification" | Select-Object -Last 10
```

Look for:
```
[OK] Loaded X justification examples for few-shot learning
```

### Full Test
1. Run analysis on a module
2. Open the generated Excel report
3. Go to "AI Justification Suggestions" sheet
4. Compare `suggested_rationale` column - should be detailed and specific

## ⚙️ Optional: Customize

### Increase Debug Logging
Edit `config/config.json`:
```json
"logging": {
  "level": "DEBUG"  // See detailed few-shot learning process
}
```

### Disable if Needed
```json
"ollama": {
  "use_few_shot_learning": false  // Revert to rule-based only
}
```

## 📖 More Information

- **Full Guide**: [docs/FEW_SHOT_LEARNING_GUIDE.md](docs/FEW_SHOT_LEARNING_GUIDE.md)
- **Implementation Details**: [FEW_SHOT_LEARNING_IMPLEMENTATION.md](FEW_SHOT_LEARNING_IMPLEMENTATION.md)
- **Ollama Setup**: [docs/OLLAMA_SETUP.md](docs/OLLAMA_SETUP.md)

## ❓ Troubleshooting

**No examples loaded?**
```
[INFO] No justification example files found for few-shot learning
```
→ Add `*_suppress_comments_*.txt` files to `justifications/` folder

**Still seeing generic justifications?**
→ Check that `use_few_shot_learning: true` in config  
→ Verify Ollama is running: `ollama list`  
→ Check logs for example loading confirmation

**Want better examples?**
→ Add more descriptive reasons to your existing suppression comments  
→ The AI learns from YOUR codebase patterns

## 🎉 That's It!

The feature is **ready to use** - just run your normal analysis workflow and enjoy improved justification quality!

---
**Pro Tip**: Review the first few AI-generated justifications and provide feedback. The system learns from your existing patterns, so improving example quality improves AI output!
