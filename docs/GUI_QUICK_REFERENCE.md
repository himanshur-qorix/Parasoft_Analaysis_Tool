# Parasoft Analysis Tool - Quick Reference Card

## 🚀 Launch GUI

**Windows:**
```bash
scripts\Launch_GUI.bat
```

**Direct Python (from project root):**
```bash
python src\parasoft_gui.py
```

**Or double-click:** `scripts\Launch_GUI.bat`

### 🔍 Automatic Startup Checks (NEW!)

On launch, the GUI automatically verifies:
- ✅ Python version & dependencies
- ✅ Project structure
- ✅ Ollama service status
- ✅ Available AI models

**Check the Output Log immediately after launch for system status!**

---

## 🎯 Quick Operations

### Main Tab - Most Common Operations

**Configuration:**
- **Module Name:** `Mka`, `EthIf`, etc.
- **Input Path:** Browse source code directory (persistent across operations)

| Button | What It Does | Output |
|--------|--------------|--------|
| 🚀 **Run Complete Analysis** | Full automated workflow | `reports/`, `fixes/`, `justifications/` |
| 📊 **Run Static Analyzer** | Analyze C/C++ code | `data/*.json` |
| 🛠️ **Generate Code Fixes** | AI-powered fixes | `fixes/*.txt` |
| 📝 **Apply Suppressions** | Add suppress comments | Modified source files |
| 📄 **Generate CERT/MISRA Report** | Compliance report | `reports/*.html` |

### Advanced Tab - Database Operations

| Button | What It Does |
|--------|--------------|
| 🏗️ **Build Rules Database** | Build Parasoft rules (1200+) |
| 📚 **Consolidate Knowledge** | Merge all module knowledge |
| 🔍 **Query Master Knowledge** | Search knowledge base |

### AI Chat Tab - Interactive Ollama 🤖 (NEW!)

**Features:**
- 🔄 **Dynamic model discovery** - Shows only YOUR installed models
- 💬 Chat with AI models (Qwen, CodeLlama, DeepSeek, etc.)
- 📊 Model sizes displayed (MB/GB)
- 🔄 Refresh button to reload model list
- Real-time streaming responses
- Code assistance and analysis help
- Context-aware conversations

**Quick Start:**
1. Run: `ollama serve`
2. Wait for models to load automatically
3. Select model from YOUR available models
4. Test connection: "✓ Test Connection"
5. Type prompt and press Send (or Ctrl+Enter)

**No Models?** Download one: `ollama pull qwen2.5:latest`

### Help Tab - Documentation Viewer 📚
- Quick access to 40+ documentation files
- No need to open external editors

---

## 📝 Basic Workflow

### First Time Setup
1. **Configure:**
   - Module name: `Mka` ← (example)
   - Input path: Browse to source code directory
2. Click "🚀 Run Complete Analysis"
3. Wait for completion (watch log)
4. Check `reports/` folder for results

### Subsequent Runs
1. Change module name/path if needed
2. Select operation from buttons
3. Monitor real-time log
4. Browse output folders

### Using AI Chat (Optional)
1. Ensure Ollama is running
2. Go to "🤖 AI Chat" tab
3. Test connection
4. Ask questions about code, rules, or analysis
5. Get real-time AI assistance

---

## 🎨 UI Layout

```
┌────────────────────────────────────────────────────┐
│  🔧 Parasoft Analysis Tool v4.0.0                  │
├────────────────────────────────────────────────────┤
│ [Main] [Advanced] [🤖 AI Chat] [About] [Help]     │
├──────────────┬─────────────────────────────────────┤
│ Module: Mka  │  📝 Output Log                      │
│ Input Path:  │  ▶️ Executing...                    │
│ [.../src] 📁 │  Processing files...                │
│              │  ✅ Success!                        │
│ [🚀 Run]     │                                     │
│ [📊 Analyze] │                                     │
│ [🛠️ Fixes]   │  [Clear] [Save] [Stop]              │
└──────────────┴─────────────────────────────────────┘
```

---

## 💡 Tips

### Module Names
- Use exact folder names from `Inputs/`
- Examples: `Mka`, `EthIf`, `TestModule`
- Case-sensitive on some operations

### Monitoring Progress
- ✅ Success markers in log
- ❌ Error markers in log
- ⚠️ Warning markers in log
- Log auto-scrolls to latest

### Saving Work
- Click "💾 Save Log" to save output
- Use folder browsers for quick access
- Reports saved automatically

### Troubleshooting
- Check log for error messages
- Stop process with "⏹️ Stop" button
- Clear log and try again
- Module name must match input folder

---

## 📁 Output Locations

| Type | Location |
|------|----------|
| Reports | `reports/` |
| Fixes | `fixes/` |
| Suppressions | `justifications/` |
| Knowledge Base | `knowledge_base/` |
| Raw Data | `data/` |

---

## 🔧 Advanced Features

### Batch Processing
Change module name between operations to process multiple modules

### Log Management
- Save logs with descriptive names
- Use Ctrl+A, Ctrl+C to copy log text
- Clear log before new operations

### Folder Quick Access
- Advanced tab → File Browser section
- One-click access to all output folders

---

## 📞 Need Help?

1. Check **Output Log** for errors
2. Read [GUI Guide](docs/GUI_GUIDE.md)
3. See [Documentation](docs/)
4. Contact: Himanshu R, Qorix India Pvt Ltd

---

## ⌨️ Keyboard Tips

- **Tab** - Move between fields
- **Enter** - Click focused button
- **Ctrl+C** - Copy from log (after selecting)
- **Ctrl+A** - Select all log text

---

**Made with ❤️ by Himanshu R | Qorix India Pvt Ltd**
