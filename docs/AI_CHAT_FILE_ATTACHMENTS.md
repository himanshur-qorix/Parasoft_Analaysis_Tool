# AI Chat File Attachments Guide

## Overview
The AI Chat feature supports attaching C/C++ source files for analysis. This guide helps you get the best results.

---

## File Size Limits

| **File Size** | **Processing Time** | **Recommended Model** | **Timeout** |
|---------------|---------------------|----------------------|-------------|
| < 10 KB       | 10-30 seconds       | qwen2.5-coder:1.5b-base | 120 sec |
| 10-30 KB      | 1-3 minutes         | qwen2.5:latest       | 300 sec |
| 30-50 KB      | 3-5 minutes         | qwen2.5:latest       | 300 sec |
| > 50 KB       | Not recommended*    | Analyze in sections  | N/A |

*Files larger than 50KB are automatically truncated to first 50KB.

---

## Model Recommendations

### **Small Files (<10KB)** - Quick Analysis
```bash
Model: qwen2.5-coder:1.5b-base
Size: 986 MB
Best for: Quick syntax checks, simple questions
```

### **Medium Files (10-30KB)** - Comprehensive Analysis
```bash
Model: qwen2.5:latest
Size: 4.7 GB  
Best for: Full file analysis, code reviews, complex questions
```

### **Large Files (30-50KB)** - Deep Analysis
```bash
Model: qwen2.5:latest
Size: 4.7 GB
Best for: Detailed analysis, violation explanations
Note: May take 3-5 minutes to process
```

---

## Troubleshooting

### ❌ **Error: Request Timed Out**

**Cause:** File too large for current model or model processing slowly.

**Solutions:**
1. **Switch to a larger model:**
   ```bash
   # If you don't have qwen2.5:latest
   ollama pull qwen2.5:latest
   ```
   Then select it in the Model dropdown.

2. **Analyze specific sections:**
   - Instead of attaching the full file, copy specific functions/sections
   - Paste them directly in the prompt

3. **Ask simpler questions:**
   - Instead of "Analyze this entire file"
   - Try "Find MISRA violations in this code"

---

### ❌ **Error: Cannot Connect to Ollama**

**Cause:** Ollama service is not running.

**Solution:**
```bash
# Start Ollama service
ollama serve
```

Or start Ollama from your application menu.

---

## Best Practices

### ✅ **DO:**
- Use **qwen2.5:latest** for files >10KB
- Be specific in your prompts: "Find CERT violations", "Explain this function"
- Attach clean, well-formatted code
- Wait patiently for large files (check loading message)

### ❌ **DON'T:**
- Don't attach binary files or compiled code
- Don't expect instant responses for large files
- Don't attach files with excessive comments (reduces useful context)
- Don't use tiny models (tinyllama) for code analysis

---

## Example Prompts

### **Good Prompts** ✅
```
📎 Attached: Mka.c (38KB)

"Identify MISRA-C:2012 violations in this file"
"What are the security issues in this code?"
"Explain the justification format used in this file"
"List all functions with incomplete implementations"
```

### **Poor Prompts** ❌
```
📎 Attached: Mka.c (38KB)

"Analyze" (too vague)
"What's this?" (not specific)
"Fix everything" (too broad)
```

---

## Performance Tips

### **1. Use Appropriate Models**
```python
# config.json
{
  "ai_integration": {
    "ollama": {
      "model": "qwen2.5:latest",  # Better for file analysis
      "timeout": 300               # 5 minutes for large files
    }
  }
}
```

### **2. Pre-process Large Files**
If your file is >50KB:
- Remove excessive comments
- Split into logical modules
- Analyze one module at a time

### **3. Monitor Progress**
Look for the system message:
```
🤖 Assistant:

[Processing large file... This may take 2-5 minutes]
```

### **4. Optimize Context**
Instead of full file analysis, focus on:
- Specific functions
- Violation patterns
- Code sections with issues

---

## Technical Details

### **Timeout Configuration**
The tool automatically adjusts timeouts based on file size:

```python
# Small files (< 10KB)
timeout = 120 seconds

# Large files (≥ 10KB)  
timeout = 300 seconds
```

### **Model Context Windows**
| **Model** | **Context Size** | **Max File Size** |
|-----------|------------------|-------------------|
| qwen2.5-coder:1.5b-base | ~8K tokens | ~15KB |
| qwen2.5:latest | ~32K tokens | ~60KB |

### **File Processing**
1. File is read as UTF-8 (up to 50KB)
2. Content is embedded in prompt:
   ```
   [User Prompt]
   
   [Attached File: filename.c]
   ```
   [File Content]
   ```
   ```
3. Sent to Ollama with streaming response
4. Response displayed in real-time

---

## FAQ

**Q: Why did my analysis timeout?**
A: File is too large for the model. Switch to `qwen2.5:latest` or analyze smaller sections.

**Q: Can I attach multiple files?**
A: Currently only one file at a time. Analyze files separately or combine them manually.

**Q: What file types are supported?**
A: Any text file (.c, .cpp, .h, .py, .java, etc.). Binary files won't work.

**Q: How do I know it's working?**
A: Look for the loading message: `[Processing large file... This may take 2-5 minutes]`

**Q: The response is incomplete/cut off**
A: Model ran out of tokens. Try a simpler question or use a larger model.

---

## Updates in v4.0

✅ **Extended timeout** for large files (300 seconds)  
✅ **Progress indicators** during processing  
✅ **Better error messages** with specific solutions  
✅ **Default model changed** to qwen2.5:latest  
✅ **File size warnings** for large attachments  

---

## See Also
- [AI_GUIDE.md](AI_GUIDE.md) - Complete AI features documentation
- [OLLAMA_SETUP.md](OLLAMA_SETUP.md) - Setting up Ollama
- [config.json](../config/config.json) - Configuration reference

---

**Last Updated:** April 30, 2026  
**Version:** 4.0
