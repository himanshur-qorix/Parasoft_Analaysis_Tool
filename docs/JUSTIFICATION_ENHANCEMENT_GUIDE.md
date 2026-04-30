# Enhanced Justification System - User Guide

**Version:** 4.0.0  
**Updated:** April 30, 2026  
**Developer:** Himanshu R @ Qorix India Pvt Ltd

---

## 🎯 What's New

The justification system now **learns from your existing code** and uses **AI (Ollama)** to generate context-aware justifications following your project's format.

### Key Enhancements

✅ **Format Learning** - Automatically discovers your justification style from existing source files  
✅ **AI-Powered Justifications** - Uses Ollama to generate intelligent, context-aware justifications  
✅ **Header Section Support** - Recognizes and generates header section + inline reference format (like Tls.c)  
✅ **Pattern Matching** - Learns from similar justifications in your codebase  
✅ **Fallback Support** - Works with or without AI using rule-based generation

---

## 📋 Supported Formats

### Format 1: Header Section + Inline References (Tls.c Style)

**Header Section (at top of file):**
```c
/*******************************************************************************
**                      Parasoft violations Section                           **
*******************************************************************************/

/*
* #section Tls_c_REF_1
* Violates CERT_C-DCL00-b-3: Declare parameter '' as const
* Justification: Standard AUTOSAR Interface provided cannot be tailored
* according to the violation.
*
* #section Tls_c_REF_2
* Violates MISRAC2012-RULE_5_4-c-2: The '' macro has the same name...
* Justification: As macros are defined by AUTOSAR standards...
*/
```

**Inline Reference (on violation line):**
```c
void Tls_OpenConnection(
    TcpIp_TlsConnectionIdType TlsConnectionId, // parasoft-suppress CERT_C-DCL00-b-3 "Reason: Tls_c_REF_1."
    const Tls_ConnectionType  TlsConnectionType)
{
    // function body
}
```

### Format 2: Traditional Parasoft Format

```c
/* parasoft-begin-suppress CERT_C-DCL00-b-3 "Standard AUTOSAR Interface provided" */
void Tls_OpenConnection(
    TcpIp_TlsConnectionIdType TlsConnectionId,
    const Tls_ConnectionType  TlsConnectionType)
{
    // function body
}
/* parasoft-end-suppress CERT_C-DCL00-b-3 */
```

---

## 🚀 How It Works

### 1. **Format Discovery Phase**

When CodeFixGenerator initializes, it:
1. Scans your source files (`Inputs/` directory and configured source paths)
2. Extracts existing justification patterns
3. Learns the header section format and inline reference style
4. Builds a database of similar justifications by rule ID

### 2. **AI Justification Generation**

When generating a justification:
1. **AI Analysis** (if Ollama available):
   - Analyzes the violation context
   - Finds similar justifications from learned patterns
   - Generates contextual justification text
   
2. **Format Application**:
   - If header format discovered → Generates header entry + inline reference
   - If traditional format → Generates begin/end suppress blocks
   
3. **Fallback**:
   - If AI unavailable → Uses Parasoft Rules Database
   - If no rules found → Uses rule-based templates

---

## 💻 Usage

### Via GUI (Recommended)

1. Launch GUI: `scripts\Launch_GUI.bat`
2. Go to **🔍 Analysis** tab
3. Set **Module Name** and **Input Path**
4. Click **🛠️ Generate Code Fixes**
5. Justifications will be automatically formatted based on discovered patterns

### Via Command Line

```powershell
# Basic usage (with AI)
python src\generate_code_fixes.py Tls --ai-mode hybrid

# Without AI (rule-based only)
python src\generate_code_fixes.py Tls --ai-mode rules-only
```

### Programmatic Usage

```python
from pathlib import Path
from CodeFixGenerator import CodeFixGenerator
from KnowledgeDatabaseManager import KnowledgeDatabaseManager

# Initialize
kb_manager = KnowledgeDatabaseManager('knowledge_base')
fix_generator = CodeFixGenerator(
    module_name='Tls',
    kb_manager=kb_manager,
    fixes_dir=Path('fixes'),
    config={'source_code_path': 'path/to/source'}
)

# Generate justifications
results = fix_generator.generate_justifications()

print(f"Generated {results['justifications_generated']} justifications")
print(f"Output file: {results['justifications_file']}")
```

---

## 📊 Output Examples

### Example 1: AI-Generated with Learned Format

**Input Violation:**
- Rule: `CERT_C-DCL00-b-3`
- Description: "Declare parameter as const"
- Code: `void Tls_Init(TcpIp_TlsConnectionIdType id)`

**Generated Output:**

```
HEADER SECTION ENTRY (Add to top of file under "Parasoft violations Section"):
/*
* #section Tls_c_REF_3
* Violates CERT_C-DCL00-b-3: Declare parameter as const
* Justification: Standard AUTOSAR Interface provided cannot be tailored 
* according to the violation; parameter specification mandated by AUTOSAR 
* TLS module API design.
*/

INLINE COMMENT (Add directly before or after the violating line):
// parasoft-suppress CERT_C-DCL00-b-3 "Reason: Tls_c_REF_3."

JUSTIFICATION TEXT:
Standard AUTOSAR Interface provided cannot be tailored according to the violation; 
parameter specification mandated by AUTOSAR TLS module API design.
```

### Example 2: Rule-Based Fallback

**Input Violation:**
- Rule: `MISRAC2012-RULE_5_4-c-2`
- Description: "Macro name too long"

**Generated Output:**

```
/* parasoft-begin-suppress MISRAC2012-RULE_5_4-c-2 "Parasoft Official: Macros 
are defined by AUTOSAR standards and internal specifications; name length cannot 
be restricted" */
... your code here ...
/* parasoft-end-suppress MISRAC2012-RULE_5_4-c-2 */
```

---

## ⚙️ Configuration

### Enable/Disable AI

Set in `config/config.json`:

```json
{
  "ai_integration": {
    "enabled": true,
    "base_url": "http://localhost:11434",
    "model": "qwen2.5-coder:1.5b-base"
  }
}
```

### Configure Source Paths

Add source paths to scan for existing patterns:

```json
{
  "source_code_path": "D:\\MyProject\\ssc\\src"
}
```

---

## 🎯 Best Practices

### 1. **Let the Tool Learn First**

Before generating justifications:
```powershell
# Run analysis to let tool discover patterns
python src\run_agent.py parasoftReport\report_dev1.html Tls
```

### 2. **Review AI-Generated Justifications**

AI justifications are intelligent but should be reviewed for accuracy:
- ✅ Check if justification matches actual reason
- ✅ Verify AUTOSAR references are correct
- ✅ Ensure technical details are accurate

### 3. **Maintain Consistent Format**

If using header section format:
- ✅ Keep header section at top of file
- ✅ Use sequential reference numbers (REF_1, REF_2, etc.)
- ✅ Update inline references to match header entries

### 4. **Use Descriptive Justifications**

Good justification:
> "Standard AUTOSAR TLS specification requires non-const parameter for 
> compatibility with TcpIp_TlsConnectionIdType; modification would break 
> interface contract."

Poor justification:
> "Cannot fix - AUTOSAR requirement"

---

## 🔍 Troubleshooting

### Issue: No Patterns Discovered

**Symptom:** Tool says "No existing justification patterns found"

**Solution:**
1. Verify source files exist in `Inputs/` directory
2. Check `config.json` has correct `source_code_path`
3. Ensure source files contain justifications (comments with `parasoft-suppress`)

### Issue: AI Not Generating Justifications

**Symptom:** Only seeing rule-based generic justifications

**Solution:**
1. Check Ollama is running: `ollama list`
2. Verify AI enabled in `config/config.json`
3. Check logs for "AI-generated justification" messages

### Issue: Wrong Format Generated

**Symptom:** Getting traditional format instead of header section format

**Solution:**
1. Ensure existing source files have header section format
2. Majority of files must use header format for it to be detected as "common"
3. Check `knowledge_base/learned_justification_formats.json` to see discovered formats

---

## 📈 Performance

| Feature | Time | Impact |
|---------|------|--------|
| Format Discovery | ~500ms | One-time at initialization |
| AI Justification | ~2-3s per violation | High quality |
| Rule-Based | ~10ms per violation | Fallback |

**Recommendation:** Use AI mode for initial generation, then switch to rules-only for bulk operations.

---

## 🎓 Advanced Usage

### Export Learned Formats

```python
from JustificationFormatLearner import JustificationFormatLearner

learner = JustificationFormatLearner(
    source_dirs=[Path('Inputs')],
    use_ai=True
)

learner.scan_source_files()
learner.export_learned_formats(Path('learned_formats.json'))
```

### Custom Format Templates

Override default templates by modifying `JustificationFormatLearner._build_header_template()`.

---

## 📚 References

- [Tls.c Example](../Inputs/Tls_Config_1/ssc/src/Tls.c) - Reference implementation
- [AI Guide](AI_GUIDE.md) - Ollama setup and configuration
- [Knowledge Base Guide](KNOWLEDGE_BASE_GUIDE.md) - Pattern learning details

---

**Questions?** Check the logs at `static_analysis.log` or run with `--verbose` flag for detailed output.

**Updated:** April 30, 2026 | **Version:** 4.0.0 | **Developer:** Himanshu R @ Qorix India Pvt Ltd
