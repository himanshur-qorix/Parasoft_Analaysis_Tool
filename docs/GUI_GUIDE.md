# Parasoft Analysis Tool - GUI Guide

## 📱 Graphical User Interface

Version 4.0.0 | Developer: Himanshu R | Organization: Qorix India Pvt Ltd

---

## 🚀 Quick Start

### Launch the GUI

**Windows:**
```bash
scripts\Launch_GUI.bat
```

**Or directly with Python:**
```bash
python src\parasoft_gui.py
```

### Automatic Startup Checks 🔍 (NEW!)

When the GUI launches, it automatically performs comprehensive system checks:

**What's Checked:**
- ✅ Python version compatibility (3.8+)
- ✅ Project folder structure
- ✅ Required Python libraries (beautifulsoup4, pandas, openpyxl, requests)
- ✅ Optional AI libraries (ollama, PIL)
- ✅ Ollama service status and version
- ✅ Available AI models
- ✅ Input configuration folders

**Where to See Results:**
- Check the **Output Log** immediately after launch
- Shows detailed status of all components
- Warns about missing dependencies
- Suggests fixes for any issues

**Example Output:**
```
==================================================================
🚀 PARASOFT ANALYSIS TOOL - STARTUP CHECKS
==================================================================

🔍 Checking Python version...
  ✅ Python 3.13.5 (Compatible)

🔍 Checking Python dependencies...
  ✅ tkinter - GUI framework (built-in)
  ✅ beautifulsoup4 - HTML parsing
  ✅ pandas - Data processing
  ✅ openpyxl - Excel reports
  ✅ requests - Ollama API communication

🔍 Checking Ollama AI service...
  ✅ Ollama service running (v0.21.0)
  ✅ 4 AI model(s) available

📊 STARTUP CHECK SUMMARY
  ✅ Passed: 15
  ⚠️ Warnings: 2

✅ System ready! All critical components available.
```

**If Issues Are Found:**
- Missing dependencies will show installation commands
- Ollama service issues will suggest starting the service
- Critical failures trigger a warning dialog

---

## 🎯 Features

### 1. **Main Operations Tab**
The primary interface for running analysis workflows:

**Module Configuration:**
- **Module Name** - Specify the module to analyze (e.g., Mka, EthIf)
- **Input Path** - Persistent source code directory path for analysis
  - Browsebutton to select directory
  - Used across multiple operations
  - Auto-populated with default Inputs/ModuleName structure

**Quick Actions:**
- **🚀 Run Complete Analysis** - Full automated workflow from analysis to fix generation
- **📊 Run Static Analyzer** - Analyze code with Parasoft/Polyspace reports
- **🛠️ Generate Code Fixes** - AI-powered code fix suggestions
- **📝 Apply Suppressions** - Apply suppress comments to source files
- **📄 Generate CERT/MISRA Report** - Create compliance reports
- **🧠 Learn Justification Patterns** - Train AI on justification patterns

### 2. **Advanced Operations Tab**
Database and knowledge management:

- **🏗️ Build Parasoft Rules Database** - Build comprehensive rules database (1200+ rules)
- **📚 Consolidate Knowledge** - Merge knowledge from multiple modules
- **🔍 Query Master Knowledge** - Search consolidated knowledge base
- **📁 Quick Folder Access** - Open reports, knowledge base, fixes, and justifications folders

### 3. **AI Chat Tab** 🤖 (NEW!)
Interactive chat interface with Ollama/Qwen models:

**Features:**
- **🔄 Dynamic Model Selection** - Automatically discovers models installed on your system:
  - Dropdown populated with YOUR available models
  - Shows model size (MB/GB) for each model
  - "🔄 Refresh Models" button to reload list
  - No hardcoded models - always up to date!
- **💬 Interactive Chat** - Real-time conversation with AI models
- **📝 Code Assistance** - Get help with code analysis, fixes, and best practices
- **🔄 Streaming Responses** - See AI responses as they're generated
- **📚 Context-Aware** - Maintains chat history for coherent conversations
- **✅ Connection Testing** - Test Ollama connection before chatting

**How to Use:**
1. Ensure Ollama is running: `ollama serve`
2. Wait for models to load automatically (or click "🔄 Refresh Models")
3. Select your preferred model from the dropdown (shows only YOUR models)
4. Click "✓ Test Connection" to verify the model is ready
5. Type your prompt in the input area
6. Press "📤 Send" or Ctrl+Enter to submit
6. View responses in the chat history
7. Continue the conversation with follow-up questions

**Requirements:**
- Ollama installed and running (see OLLAMA_SETUP.md)
- Python `requests` library: `pip install requests`
- At least one model downloaded: `ollama pull qwen2.5-coder:7b`

### 4. **About Tab**
System information and documentation

### 5. **Help Tab** 📚
Built-in documentation viewer:

- **📖 Browse Documentation** - Access all markdown documentation files from within the GUI
- **📄 README.md** - Main project documentation
- **📝 All Guides** - Complete list of docs/ folder documentation
- **Real-time Preview** - View documentation content directly in the GUI
- **Easy Navigation** - Click any document to view its content
- **Search-Friendly** - Quickly find the guide you need

**Available Documentation:**
- Quick Start guides
- Feature documentation
- Architecture guides
- Troubleshooting guides
- API documentation
- And 40+ more documentation files!

**How to Use:**
1. Click the "📚 Help" tab
2. Browse the list of documentation files on the left
3. Click any file to view its content on the right
4. Scroll through the content to read

---

## 💡 How to Use

### Step-by-Step Guide:

#### 1. **Configure Module Settings**
   - **Module Name:** Enter your module name (e.g., `Mka`, `EthIf`, `TestModule`)
   - **Input Path:** Browse and select the source code directory for analysis
     - Click "📁 Browse" to select folder
     - Path is remembered for all operations
     - Default: `Inputs/ModuleName_Config_1/PC_Config/src`
   - This name should match your input folder structure in `Inputs/`

#### 2. **Select Operation**
   - Click on any operation button from the Main or Advanced tabs
   - The script will automatically use your module name
   - All batch files are located in the `batch_support/` folder

#### 3. **Monitor Progress**
   - Watch the **Output Log** for real-time progress
   - Status bar shows current operation status
   - Process can be stopped using the "Stop Process" button

#### 4. **View Results**
   - Check the output log for completion status
   - Use folder browser buttons to open result folders
   - Save logs for documentation using "Save Log" button

---

## 🎨 User Interface Layout

```
┌──────────────────────────────────────────────────────────────┐
│  🔧 Parasoft Analysis Tool                                   │
│  Qorix India Pvt Ltd | Version 4.0.0                         │
├──────────────────────────────────────────────────────────────┤
│ ┌─────────────┬─────────────┬──────────────┬──────────────┐ │
│ │ Main Ops    │ Advanced    │ About        │ Help         │ │
│ └─────────────┴─────────────┴──────────────┴──────────────┘ │
│                                                               │
│ ┌─────────────────────┐ ┌──────────────────────────────┐   │
│ │ Module Config       │ │ 📝 Output Log                │   │
│ │                     │ │                              │   │
│ │ Module Name: [Mka]  │ │ [Real-time output here]      │   │
│ │                     │ │                              │   │
│ ├─────────────────────┤ │                              │   │
│ │ Quick Actions       │ │                              │   │
│ │                     │ │                              │   │
│ │ [Run Analysis]      │ │                              │   │
│ │ [Static Analyzer]   │ │                              │   │
│ │ [Generate Fixes]    │ │                              │   │
│ │ [Apply Suppress]    │ │                              │   │
│ │ [CERT/MISRA Rpt]    │ │                              │   │
│ │ [Learn Patterns]    │ └──────────────────────────────┘   │
│ └─────────────────────┘                                     │
│                                                              │
│ [Clear] [Save Log]                      [Stop Process]      │
├──────────────────────────────────────────────────────────────┤
│ Ready                                                        │
└──────────────────────────────────────────────────────────────┘
```

---

## 🔧 Features Explained

### Real-Time Output Log
- Shows live output from batch scripts
- Color-coded status messages (✅ ❌ ⚠️)
- Auto-scrolls to latest output
- Can be cleared or saved to file

### Module Configuration
- Persistent module name entry
- Pre-filled with example (Mka)
- Validates before running scripts

### Quick Actions Buttons
- One-click access to all operations
- Tooltips show operation details
- Disabled during script execution

### Status Bar
- Shows current operation status
- Displays button tooltips on hover
- Updates in real-time

---

## 📋 Operation Details

### Main Operations

| Operation | Description | Output Location |
|-----------|-------------|-----------------|
| **Run Complete Analysis** | Full workflow: analyze → generate fixes → apply suppressions | `reports/`, `fixes/`, `justifications/` |
| **Run Static Analyzer** | Capture violations from Parasoft/Polyspace | `data/` |
| **Generate Code Fixes** | AI-powered fix suggestions using rules database | `fixes/` |
| **Apply Suppressions** | Add suppress comments to source code | Modified source files |
| **Generate CERT/MISRA Report** | Compliance report with fix summaries | `reports/` |
| **Learn Justification Patterns** | Train AI on justification patterns | `learning/` |

### Advanced Operations

| Operation | Description | Output Location |
|-----------|-------------|-----------------|
| **Build Rules Database** | Build Parasoft rules database (1200+ rules) | `data/Parasoft_Rules_Database.json` |
| **Consolidate Knowledge** | Merge knowledge from all modules | `knowledge_base/Master_Knowledge.json` |
| **Query Master Knowledge** | Interactive query of consolidated knowledge | Console output |

---

## 🎯 Best Practices

### 1. **Always Start Fresh**
   - Clear log before starting new operations
   - Check module name is correct
   - Ensure required input files exist

### 2. **Monitor Progress**
   - Watch output log for errors
   - Don't close window while script is running
   - Save logs for troubleshooting

### 3. **Organize Results**
   - Use folder browser to check outputs
   - Review generated files after each operation
   - Keep backups of important results

### 4. **Sequential Workflow**
   ```
   1. Run Static Analyzer
        ↓
   2. Generate Code Fixes
        ↓
   3. Apply Suppressions
        ↓
   4. Generate Reports
   ```

---

## ⚙️ Configuration

### Prerequisites
- Python 3.8 or higher
- Required packages (automatically installed)
- Parasoft or Polyspace analysis reports

### Input Structure
```
Inputs/
└── ModuleName_Config_1/
    └── PC_Config/
        └── src/
            └── [source files]
```

### Output Structure
```
reports/           # Analysis reports
fixes/             # Generated code fixes
justifications/    # Suppression comments
knowledge_base/    # Module knowledge databases
data/              # Violation data
```

---

## 🐛 Troubleshooting

### GUI Won't Launch
```
Problem: "Python is not installed or not in PATH"
Solution: Install Python 3.8+ and add to system PATH
```

### Script Execution Fails
```
Problem: "Module name is required"
Solution: Enter module name before clicking buttons
```

### No Output in Log
```
Problem: Script runs but no output visible
Solution: Check if script is waiting for input, try stopping and restarting
```

### Process Hangs
```
Problem: Script seems stuck
Solution: Click "Stop Process" button and check log for errors
```

---

## 🔍 Tips & Tricks

1. **Keyboard Shortcuts**
   - The log window supports standard text selection (Ctrl+A, Ctrl+C)
   - Module name field accepts paste (Ctrl+V)

2. **Batch Processing**
   - Run multiple modules by changing module name between operations
   - Save logs for each module separately

3. **Quick Access**
   - Use folder browser buttons for instant access to results
   - Logs can be saved with descriptive names for documentation

4. **Error Recovery**
   - If operation fails, check the log for specific error messages
   - Most scripts can be re-run after fixing issues

---

## 📚 Related Documentation

- [Main README](README.md) - Project overview
- [Quick Start Guide](docs/QUICKSTART.md) - Command-line usage
- [Batch Files Guide](docs/BATCH_FILES_GUIDE.md) - Detailed script documentation
- [Architecture](docs/ARCHITECTURE.md) - System design

---

## 🆘 Support

For issues or questions:
1. Check the output log for error messages
2. Review documentation in `docs/` folder
3. Contact: Himanshu R, Qorix India Pvt Ltd

---

## 📝 Version History
4.0.0 (2026-04-29)
- **GUI Release** - Complete graphical user interface
- Reorganized project structure - batch files moved to `batch_support/`
- GUI source moved to `src/parasoft_gui.py`
- Added Qorix logo display in header
- Enhanced real-time logging with color-coded messages
- Process control improvements
- Documentation updates

### Version 1.0.0 (2026-04-29)
- Initial GUI prototype
- Basic functionality with all batch scripts
- Real-time output logging
- Folder quick accessy
- About/information tab

---

**Made with ❤️ by Himanshu R | Qorix India Pvt Ltd**
