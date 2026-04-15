# Ollama AI Integration - Quick Start Guide

**Version 2.2.0 | Last Updated: April 10, 2026**

---

## 🚀 What is Ollama?

Ollama is a **lightweight local LLM runtime** that lets you run AI models on your own machine with:
- ✅ **Zero API costs** - Completely free
- ✅ **Data privacy** - All code stays on your machine
- ✅ **Offline capability** - Works without internet
- ✅ **Low resource usage** - Small models (2-4GB RAM)
- ✅ **Simple setup** - 3 commands to get running

---

## 📦 Installation (5 minutes)

### Step 1: Install Ollama

**Windows:**
```powershell
# Option 1: Download installer from ollama.com
# Visit: https://ollama.com/download/windows

# Option 2: Use winget
winget install Ollama.Ollama
```

**Linux / WSL:**
```bash
curl -fsSL https://ollama.com/install.sh | sh
```

**macOS:**
```bash
brew install ollama
```

### Step 2: Pull a Lightweight Model

Choose one model based on your needs:

```bash
# Recommended: Qwen 2.5 Coder 1.5B (Code-specialized, ~900MB)
ollama pull qwen2.5-coder:1.5b-base

# Alternative: Phi3 Mini (3.8B params, ~2.3GB)
ollama pull phi3:mini

# Alternative: Llama 3.2 3B (~2GB)
ollama pull llama3.2:3b

# Alternative: Qwen 2.5 3B (~2GB, general purpose)
ollama pull qwen2.5:3b
```

**Model Comparison:**

| Model | Size | RAM Required | Speed | Quality | Best For |
|-------|------|--------------|-------|---------|----------|
| qwen2.5-coder:1.5b-base | 900MB | 2GB | Very Fast | Excellent | **Code analysis (recommended)** |
| phi3:mini | 2.3GB | 4GB | Fast | Good | General use |
| qwen2.5:3b | 2.0GB | 4GB | Fast | Excellent | General code |
| llama3.2:3b | 2.0GB | 4GB | Fast | Good | Balanced performance |

### Step 3: Start Ollama Server

```bash
# Start the server (runs in background)
ollama serve
```

**Windows:** After installation, Ollama usually starts automatically. Check system tray for Ollama icon.

### Step 4: Verify Installation

```bash
# Test the model
ollama run qwen2.5-coder:1.5b-base "Write a hello world in C"
```

You should see a C program response. Press `Ctrl+D` or type `/bye` to exit.

### Step 5: Install Python Dependencies

```bash
# From the project root directory
pip install -r requirements.txt
```

This installs the `ollama` Python library (already in requirements.txt).

---

##  Configuration

The tool is **pre-configured** for Ollama! Check `config/config.json`:

```json
{
  "ai_integration": {
    "enabled": true,
    "provider": "ollama",
    "ollama": {
      "base_url": "http://localhost:11434",
      "model": "qwen2.5-coder:1.5b-base",
      "timeout": 120,
      "fallback_to_rules": true
    },
    "max_tokens": 1000,
    "temperature": 0.3,
    "use_ai_for": {
      "complex_violations": true,
      "unknown_patterns": true,
      "cert_violations": false,
      "misra_violations": false
    }
  }
}
```

### Configuration Options Explained:

- **`enabled`**: Set to `false` to disable AI completely
- **`model`**: Change to any Ollama model you've pulled
- **`fallback_to_rules`**: If AI fails, use rule-based fixes
- **`use_ai_for`**: Control when AI is used:
  - `complex_violations`: Use AI for complex/ambiguous violations
  - `unknown_patterns`: Use AI for unknown patterns
  - `cert_violations`: Use AI for CERT violations (default: false, rules are good)
  - `misra_violations`: Use AI for MISRA violations (default: false, rules are good)

---

## 🎯 How It Works

### Hybrid Intelligence System

The tool uses a **smart hybrid approach**:

```
For each violation:
  │
  ├─► Is AI enabled? ──No──► Use rule-based fixes
  │                   
  └─► Yes ──► Is it complex/unknown?
              │
              ├─► Yes ──► Try AI ──► Success? ──► Use AI fix
              │                      │
              │                      └─► Failed ──► Fallback to rules
              │
              └─► No ──► Use rule-based fixes (faster)
```

**Benefits:**
- **Fast**: Standard MISRA/CERT violations use fast rule-based fixes
- **Smart**: Complex violations get AI analysis
- **Reliable**: Always has fallback to proven rules
- **Efficient**: AI only used when beneficial

### Example Session:

```
$ python src\run_agent.py report_dev1.html Mka

[INFO] Parasoft AI Agent initialized
[OK] Configuration loaded
[OK] Connected to Ollama at http://localhost:11434
[OK] Model 'qwen2.5-coder:1.5b-base' is available
[INFO] AI enabled: qwen2.5-coder:1.5b-base
[INFO] Analyzing report: report_dev1.html
[INFO] Found 147 violations

[INFO] Generating fixes...
[INFO] Violation CERT-MEM30-a: Using rule-based fix (standard pattern)
[INFO] Violation CERT-POS34-c: Using rule-based fix (standard pattern)
[AI] Violation CUSTOM-001: Complex pattern detected
[AI] Generating fix for CUSTOM-001 using phi3:mini...
[OK] AI-generated fix for CUSTOM-001
[INFO] Generated 147 fixes (3 AI, 144 rule-based)

[OK] Analysis complete!
```

---

## 🔧 Troubleshooting

### Ollama Not Found

**Error:** `Failed to connect to Ollama`

**Solution:**
```bash
# Check if Ollama is running
curl http://localhost:11434

# If not, start it
ollama serve
```

### Model Not Available

**Error:** `Model 'qwen2.5-coder:1.5b-base' not found in Ollama`

**Solution:**
```bash
# List available models
ollama list

# Pull the model
ollama pull qwen2.5-coder:1.5b-base
```

### AI Fixes Not Being Used

**Symptom:** All fixes are rule-based, no AI fixes

**Possible Causes:**
1. **AI disabled in config** - Check `config.json`, set `"enabled": true`
2. **Configuration too restrictive** - Try setting:
   ```json
   "use_ai_for": {
     "complex_violations": true,
     "unknown_patterns": true,
     "cert_violations": true,
     "misra_violations": true
   }
   ```
3. **All violations are standard** - This is normal! AI only used for complex cases

### High CPU/RAM Usage

**Solution:**
1. Already using the most efficient model!
2. If still high, reduce `max_tokens` in config:
   ```json
   "max_tokens": 500
   ```

### Slow AI Responses

**Solution:**
1. Reduce `max_tokens` in config:
   ```json
   "max_tokens": 500
   ```
2. qwen2.5-coder:1.5b-base is already one of the fastest models

---

## ⚙️ Advanced Configuration

### Using Different Models

```bash
# Pull different models for comparison
ollama pull codellama:7b      # Code-specialized (larger, 7B params)
ollama pull phi3:mini         # General purpose (3.8B params)
ollama pull qwen2.5:3b        # General code (3B params)
```

Update config.json:
```json
"model": "codellama:7b"
```

**Note:** qwen2.5-coder:1.5b-base is recommended for best balance of speed and quality.

### Running Ollama on Different Port

If port 11434 is taken:

```bash
# Start Ollama on custom port
OLLAMA_HOST=0.0.0.0:11435 ollama serve
```

Update config.json:
```json
"base_url": "http://localhost:11435"
```

### Disabling AI (Use Only Rules)

To completely disable AI and use only rule-based fixes:

**Option 1:** Edit config.json
```json
"ai_integration": {
  "enabled": false
}
```

**Option 2:** Don't start Ollama
- If Ollama is not running, the tool auto-falls back to rules

---

## 📊 Resource Requirements

### Minimum Requirements:
- **CPU:** Any modern CPU (x86_64 or ARM64)
- **RAM:** 2GB free (for qwen2.5-coder:1.5b-base)
- **Disk:** 1GB for model storage
- **OS:** Windows 10/11, Linux, macOS

### Recommended Requirements:
- **CPU:** 4+ cores
- **RAM:** 4GB free (for faster performance)
- **Disk:** 3GB for multiple models
- **OS:** Windows 11, Ubuntu 22.04+, macOS 12+

### Performance Benchmarks:

**qwen2.5-coder:1.5b-base on typical hardware:**
- Intel i5 (4 cores): ~1-2 seconds per fix
- AMD Ryzen 5 (6 cores): ~0.5-1 seconds per fix
- Apple M1: ~0.5 seconds per fix
- Intel i3 (2 cores): ~2-3 seconds per fix

**Note:** Much faster than larger models due to 1.5B parameter count!

**Note:** Rule-based fixes are instant (<0.1 seconds), so hybrid mode stays fast.

---

## 🔒 Privacy & Security

### Data Privacy
- ✅ **All processing is local** - Code never leaves your machine
- ✅ **No cloud API calls** - No data sent to external servers
- ✅ **No telemetry** - Ollama doesn't track usage
- ✅ **Corporate friendly** - Safe for proprietary/confidential code

### Security Best Practices
1. Keep Ollama updated: `ollama version`
2. Only pull models from official Ollama library
3. Run Ollama on localhost (not exposed to network)
4. Review AI-generated fixes before applying

---

## 🆘 Support

### Ollama Resources
- **Official Site:** https://ollama.com
- **Documentation:** https://github.com/ollama/ollama
- **Model Library:** https://ollama.com/library

### Parasoft AI Agent
- **GitHub Issues:** Report bugs or request features
- **README.md:** Full tool documentation
- **ARCHITECTURE_DIAGRAM.md:** System architecture

---

## 📝 Example Workflow

```bash
# 1. Start Ollama (if not running)
ollama serve

# 2. Run analysis
python src\run_agent.py report_dev1.html Mka

# 3. Review generated fixes
# Check: fixes/Mka/Mka_fixes_TIMESTAMP.txt

# 4. Apply suppress comments interactively
scripts\Apply_Suppressions.bat

# 5. Review AI-generated fixes vs rule-based
# AI fixes will have "ai_generated": true in knowledge database
```

---

## ✅ Verification Checklist

After setup, verify everything works:

- [ ] Ollama installed: `ollama --version`
- [ ] Model downloaded: `ollama list`
- [ ] Server running: `curl http://localhost:11434`
- [ ] Python library installed: `pip show ollama`
- [ ] Config enabled: Check `config.json` → `"enabled": true`
- [ ] Model matches: Config model name matches `ollama list` output
- [ ] Test run: `python src\run_agent.py report_dev1.html TestModule`
- [ ] AI status logged: Look for "[OK] AI enabled" in output

---

**Ready to use AI-powered Parasoft analysis! 🚀**

For questions or issues, check the troubleshooting section above or open a GitHub issue.

