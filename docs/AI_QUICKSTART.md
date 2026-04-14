# 🚀 Quick Start with AI Integration

**Parasoft AI Agent v2.2.0** - Now with Ollama local AI integration!

---

## ⚡ 3-Minute Setup

### 1. Install Ollama

**Windows:**
```powershell
winget install Ollama.Ollama
```

**Linux/WSL:**
```bash
curl -fsSL https://ollama.com/install.sh | sh
```

### 2. Pull AI Model
```bash
# Recommended (code-specialized, very fast, ~900MB)
ollama pull qwen2.5-coder:1.5b-base

# Start Ollama
ollama serve
```

### 3. Install Dependencies
```bash
pip install -r requirements.txt
```

### 4. Run Analysis
```bash
# Windows
scripts\Run.bat

# Or command line
python run_agent.py report_dev1.html YourModule
```

---

## 🎯 What You Get

✅ **Hybrid AI + Rules** - Fast rule-based fixes for standard violations, AI for complex cases  
✅ **Zero Cost** - Runs locally, no API fees  
✅ **Data Privacy** - Your code never leaves your machine  
✅ **Low Resources** - 2-4GB RAM, works offline  
✅ **Auto-Fallback** - If AI unavailable, uses proven rule-based fixes  

---

## 📊 AI vs Rules Decision

The tool is smart about when to use AI:

| Violation Type | Method | Reason |
|----------------|--------|--------|
| Standard MISRA 10.1 | Rules | Fast, proven patterns |
| Standard CERT MEM30 | Rules | Fast, proven patterns |
| Complex/Unknown | **AI** | Better context understanding |
| Ambiguous patterns | **AI** | Needs analysis |

**Result:** You get speed + intelligence where it matters!

---

## 🛠️ Configuration (Optional)

Already configured in `config/config.json`:
```json
{
  "ai_integration": {
    "enabled": true,          // Set false to disable AI
    "provider": "ollama",
    "ollama": {
      "model": "qwen2.5-coder:1.5b-base",   // Change model here
      "fallback_to_rules": true
    }
  }
}
```

---

## 🔧 Troubleshooting

**AI not connecting?**
```bash
# Check if Ollama is running
curl http://localhost:11434

# If not, start it
ollama serve
```

**Want to disable AI?**
- Just don't start Ollama, tool auto-falls back to rules
- Or set `"enabled": false` in config.json

**Want to try different model?**
```bash
ollama pull phi3:mini       # Larger (2.3GB), more general
ollama pull qwen2.5:3b      # Larger (2GB), general code
```
Then update config: `"model": "phi3:mini"`

---

## 📚 Full Documentation

- **Ollama Setup:** [docs/OLLAMA_SETUP.md](docs/OLLAMA_SETUP.md)
- **Full Guide:** [README.md](README.md)
- **Architecture:** [ARCHITECTURE_DIAGRAM.md](ARCHITECTURE_DIAGRAM.md)

---

## ✅ Verify AI is Working

Look for these messages when running the tool:
```
[OK] Connected to Ollama at http://localhost:11434
[OK] Model 'qwen2.5-coder:1.5b-base' is available
[INFO] AI enabled: qwen2.5-coder:1.5b-base
[AI] Generating fix for CUSTOM-001 using qwen2.5-coder:1.5b-base...
[OK] AI-generated fix for CUSTOM-001
```

---

**Happy analyzing! 🎉**
