# 📋 Batch Files Quick Reference

**Organization:** Qorix India Pvt Ltd  
**Version:** 2.3.0

---

## 🎯 Which Batch File Should I Run?

| Batch File | Purpose | When to Use | Output |
|------------|---------|-------------|---------|
| **scripts/Run.bat** | Full analysis workflow | First time or complete re-analysis | Excel reports, KB, fixes, justifications |
| **scripts/Generate_Code_Fixes.bat** ⭐ | Generate code fix suggestions | Regenerate fixes without full analysis | Code fix suggestions file |
| **scripts/Learn_Justification_Patterns.bat** 🆕 | Learn from existing justifications | Understand justification patterns | Pattern analysis report |
| **scripts/Apply_Suppressions.bat** | Apply suppress comments | After justifications are ready | Modified source code files |
| **scripts/Build_Parasoft_Rules_Database.bat** | Build rules database | One-time setup or after Parasoft update | Parasoft_Rules_Database.json |
| **scripts/Consolidate_Knowledge.bat** | Merge knowledge bases | Combine learnings from multiple modules | Master knowledge base |
| **scripts/Query_Master_Knowledge.bat** | Query consolidated KB | Search/analyze cross-module insights | HTML/Excel reports |

---

## 🔄 Typical Workflow

### **Initial Analysis**
```
1. Run.bat
   ↓
   Creates: Excel reports + Knowledge base + Fixes + Justifications
```

### **Regenerate Fixes (NEW!)**
```
Generate_Code_Fixes.bat
   ↓
   Creates: Updated fix suggestions with latest Parasoft DB
```

### **Apply Suppressions**
```
Apply_Suppressions.bat
   ↓
   Modifies: Source code files with suppress comments
```

### **Learn from Existing Justifications (NEW!)**
```
Learn_Justification_Patterns.bat
   ↓
   Analyzes: What gets justified, how, and why
   Generates: Pattern report for AI training
```

### **Cross-Module Learning**
```
Consolidate_Knowledge.bat
   ↓
Query_Master_Knowledge.bat
   ↓
   Analyzes: Proven fixes across all modules
```

---

## 📖 Detailed Descriptions

### **1. Run.bat** - Main Analysis Workflow

**Purpose:** Complete end-to-end analysis of Parasoft violations

**What it does:**
- ✅ Parses Parasoft HTML report (or generates MISRA/CERT report)
- ✅ Checks violations against Qorix deviations
- ✅ Creates/updates knowledge base
- ✅ Generates Excel reports with status column
- ✅ Generates code fix suggestions
- ✅ Creates justifications for violations

**Outputs:**
```
reports/
  ├── {Module}_violations_report.xlsx
  ├── {Module}_violations_report_UPDATED.xlsx
  └── {Module}_analysis_summary.json

knowledge_base/
  └── {Module}_KnowledgeDatabase.json

fixes/{Module}/
  └── {Module}_fixes_*.txt

justifications/
  └── {Module}_suppress_comments_*.txt
```

**When to use:**
- 🔹 First time analyzing a module
- 🔹 After receiving new Parasoft report
- 🔹 Complete re-analysis needed
- 🔹 Knowledge base doesn't exist yet

---

### **2. Generate_Code_Fixes.bat** ⭐ **NEW!**

**Purpose:** Generate/regenerate code fix suggestions for violations

**What it does:**
- ✅ Loads existing knowledge base
- ✅ Queries Parasoft Rules Database (1200+ rules) FIRST
- ✅ Uses AI for complex violations (hybrid mode)
- ✅ Generates detailed fix suggestions with examples
- ✅ MUCH FASTER than full analysis (no report parsing)

**Outputs:**
```
fixes/{Module}/
  └── {Module}_fixes_{timestamp}.txt
      Contains:
        • Official Parasoft repair examples
        • Before/after code snippets
        • Security relevance and CWE mappings
        • AI-generated suggestions
        • Rule-based patterns
```

**When to use:**
- 🔹 **After running Run.bat** (knowledge base exists)
- 🔹 **Regenerate fixes** with different AI mode
- 🔹 **Get updated fixes** after building Parasoft Rules DB
- 🔹 **Quick fix suggestions** without re-analysis
- 🔹 **Focus only on code fixes** (no Excel reports needed)

**Advantages:**
- ⚡ **Fast** - Doesn't re-parse reports
- 🎯 **Focused** - Only generates fixes
- 🔄 **Repeatable** - Run multiple times with different modes
- ⭐ **Uses Parasoft DB** - Official fix examples

**Example scenarios:**
```
Scenario 1: Try different AI modes
  1. Generate fixes with hybrid mode
  2. Re-run with ai_only to compare
  3. Choose best suggestions

Scenario 2: After updating Parasoft DB
  1. Run Build_Parasoft_Rules_Database.bat
  2. Run Generate_Code_Fixes.bat
  3. Get updated official fixes

Scenario 3: Quick regeneration
  1. Knowledge base already exists
  2. Need fresh fix suggestions
  3. Don't want to re-run full analysis
```

---

### **3. Apply_Suppressions.bat** - Apply Suppress Comments

**Purpose:** Automatically insert Parasoft suppress comments into source code

**What it does:**
- ✅ Reads justification file
- ✅ Finds violation locations in source code
- ✅ Inserts `/* parasoft-begin-suppress ... */` comments
- ✅ Interactive approval for each change
- ✅ Creates backups before modifying

**When to use:**
- 🔹 After running Run.bat (justifications exist)
- 🔹 Ready to suppress justified violations
- 🔹 Want to clean up Parasoft report

**Prerequisites:**
- Knowledge base exists
- Justification file exists
- Source code path available

---

### **4. Build_Parasoft_Rules_Database.bat** - Build Rules DB

**Purpose:** Parse Parasoft official documentation into indexed database

**What it does:**
- ✅ Parses 1200+ Parasoft HTML files
- ✅ Extracts repair examples, descriptions, CWE mappings
- ✅ Creates indexed JSON database
- ✅ Enables official Parasoft fix suggestions

**When to use:**
- 🔹 **One-time setup** - Before first use
- 🔹 **After Parasoft update** - When documentation changes
- 🔹 **New rule packs** - When new rules are added

**Note:** Only need to run ONCE, then rebuild after Parasoft updates

---

### **4.5. Learn_Justification_Patterns.bat** 🆕 - Learn from Existing Justifications & Quality Analysis

**Purpose:** Analyze existing suppression comments to understand justification patterns, identify quality issues, and build persistent learning database (works WITHOUT AI!)

**What it does:**
- ✅ Scans all existing suppression comment files
- ✅ **NEW!** Classifies justifications: human-written vs tool-generated
- ✅ **NEW!** Calculates quality scores (0-10) for each justification
- ✅ **NEW!** Detects common mistakes: generic references, too short, vague
- ✅ **NEW!** Builds persistent pattern database (learned_patterns_db.json)
- ✅ **NEW!** Works completely offline - no AI/Ollama required
- ✅ Identifies common justification scenarios
- ✅ Analyzes which rules are most frequently justified
- ✅ Finds cross-module consistency patterns
- ✅ Categorizes justification reasons
- ✅ Generates comprehensive pattern report
- ✅ Provides training data for AI few-shot learning

**Outputs:**
```
Console:
  ├── Module overview and statistics
  ├── 📚 Pattern database building (NEW!)
  ├── ⚠️  Quality issues detected (NEW!)
  │   ├── Generic reference issues
  │   ├── Too-short justifications
  │   └── Vague explanations (with quality scores)
  ├── Top 15 most justified rules
  ├── Common justification scenarios
  ├── Reason pattern analysis
  ├── Cross-module consistency findings
  └── Recommendations for future justifications

knowledge_base/
  ├── justification_patterns_report.json (ENHANCED)
  │   Contains:
  │     • All identified patterns
  │     • Quality analysis with issue details (NEW!)
  │     • Tool vs human comparison (NEW!)
  │     • Rule statistics
  │     • Cross-module analysis
  │     • Scenario classification
  │     • Reason categorization
  │
  └── learned_patterns_db.json (NEW!)
      Contains:
        • Good examples per rule (quality score ≥7.0)
        • Common mistake patterns
        • Quality indicators
        • Statistics on learning progress
```

**When to use:**
- 🔹 **After analyzing multiple modules** - Have justification examples
- 🔹 **NEW! Quality review** - Check tool-generated justification quality
- 🔹 **NEW! Team contributions** - After adding modules to learning/ folder
- 🔹 **Understand justification patterns** - Learn what gets justified and why
- 🔹 **Improve justification quality** - See good vs generic examples with scores
- 🔹 **Cross-module consistency** - Find rules justified across modules
- 🔹 **Train AI few-shot learning** - Provide better examples for AI
- 🔹 **Project-wide deviations** - Identify candidates for standard deviations

**What you'll learn:**
- 📊 **Statistics**: How many justifications per module/rule
- 🔝 **Top Rules**: Most frequently justified violations
- 🔗 **Cross-Module**: Rules justified consistently across modules
- 💡 **Scenarios**: Common justification use cases (API, legacy, design)
- ✅ **Quality**: Generic vs descriptive justification reasons
- 🆕 **Quality Scores**: 0-10 rating for each justification
- 🆕 **Mistake Patterns**: Common issues to avoid
- 🆕 **Best Practices**: Technical details that improve quality

**Quality Scoring (NEW!):**
```
High Quality (8.5/10):
  "MKA module uses legacy CAN stack v2.1 API for backward compatibility.
   Void pointer safely cast based on validated message type. ASIL-D
   requirement tested in DR-2024-045."
   ✓ Specific details: v2.1, DR-2024-045, ASIL-D
   ✓ Explanation: "for backward compatibility"
   ✓ Technical context: legacy, API, ASIL-D

Low Quality (2.0/10):
  "Reason: Mka_Parasoft_REF_001"
   ✗ Generic reference
   ✗ Too short
   ✗ No explanation
```

**Example output insights:**
Module Overview:
  Mka    : 45 justifications
  NvM    : 32 justifications
  SecOC  : 28 justifications

Top Justified Rules:
  MISRAC2012-RULE_8_13-a-4 : 15 occurrences (API compatibility)
  CERT_C-DCL00-b-3         : 12 occurrences (Legacy interfaces)
  MISRAC2012-RULE_20_1-a-4 : 8 occurrences  (Include order)

Cross-Module Patterns:
  MISRAC2012-RULE_8_13 justified in 3 modules
    → Common deviation - API compatibility constraint
    → Candidate for project-wide accepted deviation
```

**Recommendations generated:**
- ⚠️ Identifies if too many generic references (Module_REF_XXX)
- ✅ Highlights good descriptive justification patterns
- 📌 Suggests project-wide deviation candidates
- 🤖 Shows how AI will learn from these patterns

**Integration with Few-Shot Learning:**
This analysis report shows exactly what the AI few-shot learning system uses as training examples. Running this helps you:
1. See what patterns the AI is learning from
2. Identify areas to improve justification quality
3. Understand why AI generates certain suggestions
4. Validate cross-module consistency

---

### **5. Consolidate_Knowledge.bat** - Merge Knowledge Bases

**Purpose:** Combine learnings from multiple modules into master KB

**What it does:**
- ✅ Merges all module-specific knowledge bases
- ✅ Identifies proven fixes across modules
- ✅ Tracks violation patterns
- ✅ Creates master insights

**When to use:**
- 🔹 After analyzing multiple modules
- 🔹 Want to learn from cross-module patterns
- 🔹 Building organizational knowledge

---

### **6. Query_Master_Knowledge.bat** - Query Consolidated KB

**Purpose:** Search and analyze master knowledge base

**What it does:**
- ✅ Interactive query tool
- ✅ Search by violation ID, keyword, category
- ✅ Export to HTML or Excel
- ✅ View proven fixes from other modules

**When to use:**
- 🔹 After consolidating knowledge bases
- 🔹 Research how others fixed similar violations
- 🔹 Generate reports for management

---

## 💡 Common Use Cases

### **Use Case 1: New Module Analysis**
```batch
# Step 1: Full analysis
Run.bat

# Step 2: Review fixes and regenerate if needed
Generate_Code_Fixes.bat

# Step 3: Apply suppressions
Apply_Suppressions.bat
```

### **Use Case 2: Experimenting with AI Modes**
```batch
# Generate with hybrid (default)
Generate_Code_Fixes.bat
  → Select: [2] Hybrid

# Compare with AI-only
Generate_Code_Fixes.bat
  → Select: [1] AI Only

# Compare with rules-only (no AI)
Generate_Code_Fixes.bat
  → Select: [3] Rules Only
```

### **Use Case 3: Understanding Justification Patterns** 🆕
```batch
# Step 1: Analyze existing patterns
Learn_Justification_Patterns.bat

# Step 2: Review the insights
# - See what rules are commonly justified
# - Understand cross-module consistency
# - Identify project-wide deviations

# Step 3: Use insights to improve future work
# - Apply learnings to new justifications
# - AI will learn from good examples automatically
```

### **Use Case 4: After Parasoft Update**
```batch
# Step 1: Rebuild Parasoft DB with new rules
Build_Parasoft_Rules_Database.bat

# Step 2: Regenerate fixes with new official examples
Generate_Code_Fixes.bat
```

---

## ❓ FAQ

### **Q: When should I use Generate_Code_Fixes.bat vs Run.bat?**

**Run.bat:**
- First time analysis
- Need Excel reports
- Knowledge base doesn't exist
- Need justifications generated

**Generate_Code_Fixes.bat:**
- Knowledge base already exists
- Only need fix suggestions
- Want to try different AI modes
- Faster regeneration

### **Q: Can I run Generate_Code_Fixes.bat multiple times?**

Yes! It's designed for repeated use:
- Try different AI modes
- Regenerate after DB updates
- Get fresh suggestions anytime

### **Q: Do I need to run Build_Parasoft_Rules_Database.bat every time?**

No! Only run:
- ✅ First time setup (once)
- ✅ After Parasoft updates
- ✅ When new rule documentation added

### **Q: What's the difference in speed?**

**Run.bat:** ~2-5 minutes (full analysis)  
**Generate_Code_Fixes.bat:** ~10-30 seconds (fixes only) ⚡  
**Learn_Justification_Patterns.bat:** ~5-10 seconds (pattern analysis) ⚡

### **Q: When should I run Learn_Justification_Patterns.bat?** 🆕

**Run it when:**
- ✅ You've analyzed multiple modules (have justification examples)
- ✅ Want to understand what violations are commonly justified
- ✅ Need to identify project-wide deviation candidates
- ✅ Want to improve justification quality in your team
- ✅ Curious what the AI few-shot learning is learning from

**Benefits:**
- 📊 See statistics on justified violations
- 🔗 Find cross-module consistency patterns
- 💡 Identify common justification scenarios
- ✅ Improve future justification quality
- 🤖 Understand AI training data

**Don't run it if:**
- ❌ No justification files exist yet (run analysis first)
- ❌ Only analyzed one module (limited insights)

### **Q: Does AI few-shot learning use the pattern analysis automatically?**

Yes! The AI few-shot learning automatically loads examples from your justifications folder. The `Learn_Justification_Patterns.bat` helps YOU understand what the AI is learning from, but the AI doesn't need the analysis to function.

**Think of it this way:**
- **AI few-shot learning** = Automatically learns from examples (always on)
- **Learn_Justification_Patterns.bat** = Shows YOU what patterns exist (for human insight)

---

## 📞 Support

**Qorix India Pvt Ltd**  
📧 Email: support@qorix.com  
🌐 Website: www.qorix.com

---

**Last Updated:** April 22, 2026  
**Tool Version:** 2.4.0
