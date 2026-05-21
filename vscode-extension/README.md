# Parasoft CERT/MISRA Analyzer - VS Code Extension

Real-time CERT and MISRA violation detection for C/C++ code with AI-powered fix suggestions.

![Version](https://img.shields.io/badge/version-1.0.0-blue)
![VS Code](https://img.shields.io/badge/VS%20Code-1.80+-green)
![License](https://img.shields.io/badge/license-MIT-orange)

## 🚀 Features

### ✅ Real-Time Violation Detection
- **Live Analysis**: Violations appear instantly as you code
- **Problems Panel Integration**: All violations in one place
- **Inline Warnings**: Squiggly underlines in your code
- **File & Workspace Scanning**: Analyze individual files or entire projects

### 🎯 CERT & MISRA Support
- **CERT C Standards**: Complete coverage of CERT C secure coding rules
- **MISRA C Standards**: Full MISRA C:2012 compliance checking
- **Severity Filtering**: Filter by CRITICAL, HIGH, MEDIUM, LOW
- **Customizable Rules**: Enable/disable specific rule categories

### 🤖 AI-Powered Fixes
- **Intelligent Suggestions**: AI generates context-aware fixes
- **One-Click Apply**: Apply fixes directly from the editor
- **Code Actions**: Quick fixes via right-click menu
- **Fix Preview**: Review fixes before applying

### 📋 Quick Actions
- **Suppress Violations**: Add `parasoft-suppress` comments with justification
- **Generate Fixes**: Get AI-powered code suggestions
- **View Knowledge Base**: Browse all detected violations
- **Open Reports**: View comprehensive CERT/MISRA HTML reports

### ⚙️ Flexible Configuration
- **Analyze on Save**: Automatic analysis when files are saved
- **Analyze on Type**: Real-time analysis while coding (optional)
- **Custom Module Names**: Support for multi-module projects
- **Python Integration**: Seamless connection to Parasoft backend

---

## 📦 Installation

### Method 1: From VSIX (Local Install)

1. **Build the Extension**
   ```bash
   cd vscode-extension
   npm install
   npm install -g @vscode/vsce
   vsce package
   ```

2. **Install in VS Code**
   - Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on Mac)
   - Type: `Extensions: Install from VSIX`
   - Select the generated `.vsix` file

### Method 2: Development Mode

1. **Open Extension Folder**
   ```bash
   cd vscode-extension
   code .
   ```

2. **Install Dependencies**
   ```bash
   npm install
   ```

3. **Launch Extension**
   - Press `F5` to open Extension Development Host
   - Your extension will be active in the new VS Code window

---

## ⚙️ Configuration

### 1. Open Settings
- Press `Ctrl+,` (or `Cmd+,` on Mac)
- Search for "Parasoft"

### 2. Configure Extension

#### **Essential Settings**

| Setting | Description | Default |
|---------|-------------|---------|
| `parasoft.enabled` | Enable/disable analyzer | `true` |
| `parasoft.toolPath` | Path to Parasoft tool directory | Auto-detected |
| `parasoft.moduleName` | Module name for Knowledge Base | `"Mka"` |
| `parasoft.pythonPath` | Path to Python executable | `"python"` |

#### **Analysis Settings**

| Setting | Description | Default |
|---------|-------------|---------|
| `parasoft.analyzeOnSave` | Run analysis on file save | `true` |
| `parasoft.analyzeOnType` | Run analysis while typing | `false` |
| `parasoft.severityFilter` | Show only these severities | `["CRITICAL", "HIGH", "MEDIUM"]` |

#### **Display Settings**

| Setting | Description | Default |
|---------|-------------|---------|
| `parasoft.showCERT` | Show CERT violations | `true` |
| `parasoft.showMISRA` | Show MISRA violations | `true` |
| `parasoft.aiEnabled` | Enable AI fix generation | `true` |

### 3. Example settings.json

```json
{
  "parasoft.enabled": true,
  "parasoft.toolPath": "D:/Task&Data/CY26Q2_SP2/ParsoftFixes/Parasoft_Analaysis_Tool",
  "parasoft.moduleName": "Mka",
  "parasoft.analyzeOnSave": true,
  "parasoft.analyzeOnType": false,
  "parasoft.severityFilter": ["CRITICAL", "HIGH", "MEDIUM"],
  "parasoft.showCERT": true,
  "parasoft.showMISRA": true,
  "parasoft.aiEnabled": true,
  "parasoft.pythonPath": "python"
}
```

---

## 📖 Usage

### 🔍 Analyzing Code

#### **Analyze Current File**
- **Command**: `Ctrl+Shift+P` → `Parasoft: Analyze Current File`
- **Button**: Click `$(bug)` icon in editor title bar
- **Automatic**: Saves file (if `analyzeOnSave` is enabled)

#### **Analyze Entire Workspace**
- **Command**: `Ctrl+Shift+P` → `Parasoft: Analyze Entire Workspace`
- **Result**: Scans all `.c`, `.cpp`, `.h`, `.hpp` files

### 🛠️ Fixing Violations

#### **Method 1: Quick Fix Menu** (Recommended)
1. Click on the violation (red/yellow squiggle)
2. Click the lightbulb icon or press `Ctrl+.`
3. Select:
   - **"Generate AI fix"** - Get intelligent code suggestion
   - **"Suppress"** - Add justification comment

#### **Method 2: Context Menu**
1. Right-click on violation line
2. Select:
   - **"Parasoft: Generate AI Fix"**
   - **"Parasoft: Suppress This Violation"**

#### **Method 3: Command Palette**
1. Place cursor on violation line
2. Press `Ctrl+Shift+P`
3. Run command:
   - `Parasoft: Generate AI Fix`
   - `Parasoft: Suppress This Violation`

### 📊 Viewing Results

#### **Problems Panel**
- **Open**: `Ctrl+Shift+M` (or View → Problems)
- **Filter**: Click "Parasoft" to show only violations
- **Navigate**: Click violation to jump to code

#### **Status Bar**
- **Location**: Bottom-left corner
- **Display**: `$(bug) X violations`
- **Click**: Analyzes current file

#### **Output Channel**
- **Open**: View → Output → Select "Parasoft Analyzer"
- **Content**: Detailed analysis logs

### 📑 Reports & Knowledge Base

#### **Open CERT/MISRA Report**
- **Command**: `Ctrl+Shift+P` → `Parasoft: Open CERT/MISRA Report`
- **File**: Opens HTML report in browser

#### **View Knowledge Base**
- **Command**: `Ctrl+Shift+P` → `Parasoft: Open Knowledge Base`
- **File**: Opens JSON database in editor

---

## 🎨 Screenshots

### Real-Time Violation Detection
```
mycode.c
────────
int main() {
    int x;              // ⚠️ CERT-C-DCL37-a: Variable not initialized
    return x + 1;       // 🔴 MISRA-C-12.1: Expression has no effect
}
```

### Problems Panel
```
Problems (2)
└─ Parasoft (2)
   ├─ [ERROR] mycode.c:2 - Variable must be initialized (CERT-C-DCL37-a)
   └─ [WARN]  mycode.c:3 - Expression result unused (MISRA-C-12.1)
```

### Quick Fix Menu
```
💡 Quick Fix
   ⚡ Generate AI fix for CERT-C-DCL37-a  (Preferred)
   🚫 Suppress CERT-C-DCL37-a
```

---

## 🧪 Example Workflow

### Scenario: Fix a CERT Violation

1. **Write Code**
   ```c
   int calculate() {
       int result;
       return result * 2;  // ⚠️ CERT-C-DCL37-a appears
   }
   ```

2. **Click Lightbulb** → **"Generate AI fix"**

3. **Review Fix**
   ```
   Fix Generated:
   Description: Initialize variable before use
   Priority: HIGH
   
   Suggested Code:
   int result = 0;  // Initialize to zero
   
   [Apply Fix] [Show Details] [Cancel]
   ```

4. **Apply Fix**
   ```c
   int calculate() {
       int result = 0;  // ✅ Fixed
       return result * 2;
   }
   ```

### Scenario: Suppress False Positive

1. **Code with False Positive**
   ```c
   extern int initialized_elsewhere;
   return initialized_elsewhere + 1;  // ⚠️ False positive
   ```

2. **Right-click** → **"Parasoft: Suppress This Violation"**

3. **Enter Justification**
   ```
   Justification: Variable is initialized in external module ModuleX
   ```

4. **Result**
   ```c
   /* parasoft-suppress CERT-C-DCL37-a "Variable is initialized in external module ModuleX" */
   extern int initialized_elsewhere;
   return initialized_elsewhere + 1;  // ✅ Suppressed
   ```

---

## 🔧 Troubleshooting

### Issue: "Parasoft tool path not found"

**Solution:**
1. Open VS Code Settings (`Ctrl+,`)
2. Search: `parasoft.toolPath`
3. Set to: `D:/Task&Data/CY26Q2_SP2/ParsoftFixes/Parasoft_Analaysis_Tool`

### Issue: "Knowledge Base not found"

**Solution:**
1. Verify module name matches your KB file
2. Check: `knowledge_base/YourModule_KnowledgeDatabase.json` exists
3. Update `parasoft.moduleName` setting

### Issue: "No violations detected"

**Checklist:**
- ✅ Extension enabled? (`parasoft.enabled = true`)
- ✅ File is C/C++? (`.c`, `.cpp`, `.h`)
- ✅ Knowledge Base loaded? (Check Output channel)
- ✅ Severity filter correct? (Check `parasoft.severityFilter`)
- ✅ CERT/MISRA enabled? (`showCERT`, `showMISRA`)

### Issue: "AI fix generation failed"

**Solution:**
1. Check Python path: `parasoft.pythonPath`
2. Verify Ollama is running: `ollama serve`
3. Check model installed: `ollama list`
4. Review Output channel for detailed error

### Issue: Extension not activating

**Solution:**
1. Check language ID: File must be detected as `c` or `cpp`
2. Force reload: `Ctrl+Shift+P` → `Developer: Reload Window`
3. Check extension logs: Output → Extension Host

---

## 🎯 Best Practices

### 1. **Start with High Severity**
   ```json
   "parasoft.severityFilter": ["CRITICAL", "HIGH"]
   ```
   Focus on critical issues first, then expand to MEDIUM.

### 2. **Use Analyze on Save**
   ```json
   "parasoft.analyzeOnSave": true,
   "parasoft.analyzeOnType": false
   ```
   Balances performance and feedback.

### 3. **Organize Suppressions**
   ```c
   /* parasoft-suppress RULE-ID "Clear justification"
    * Additional context if needed
    */
   ```
   Always document WHY you're suppressing.

### 4. **Review AI Fixes**
   - Always click "Show Details" first
   - Understand the fix before applying
   - Test after applying

### 5. **Regular Workspace Analysis**
   - Run weekly: `Parasoft: Analyze Entire Workspace`
   - Check new violations in Problems panel
   - Update Knowledge Base regularly

---

## 🔌 Integration with Existing Workflow

### Git Hooks (Pre-commit)

Create `.git/hooks/pre-commit`:
```bash
#!/bin/bash
# Analyze staged C/C++ files
STAGED_FILES=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(c|cpp|h|hpp)$')

if [ -n "$STAGED_FILES" ]; then
    echo "Running Parasoft analysis..."
    # Your analysis command here
fi
```

### CI/CD Pipeline

```yaml
# .github/workflows/parasoft.yml
name: Parasoft Analysis
on: [push, pull_request]

jobs:
  analyze:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Run Parasoft
        run: |
          python src/ParasoftAnalysisTool.py analyze
          python src/generate_cert_misra_report.py YourModule
```

---

## 📚 Additional Resources

- **Main Tool Documentation**: `docs/INDEX.md`
- **AI Integration Guide**: `docs/AI_INTEGRATION_FIX_SUMMARY.md`
- **GUI Guide**: `docs/GUI_GUIDE.md`
- **Knowledge Base Guide**: `docs/KNOWLEDGE_BASE_GUIDE.md`

---

## 🤝 Support

### Common Commands Quick Reference

| Action | Keyboard Shortcut | Command |
|--------|-------------------|---------|
| Analyze File | - | `Ctrl+Shift+P` → Parasoft: Analyze Current File |
| Quick Fix | `Ctrl+.` | Opens quick fix menu |
| Problems Panel | `Ctrl+Shift+M` | Shows all violations |
| Suppress | - | Right-click → Parasoft: Suppress |
| Generate Fix | - | Right-click → Parasoft: Generate AI Fix |

### Status Bar Icons

| Icon | Meaning |
|------|---------|
| `$(bug) Parasoft` | Ready |
| `$(sync~spin) Analyzing...` | Analysis in progress |
| `$(bug) X violations` | Violations detected |
| `$(bug) Error` | Analysis failed |

---

## 📄 License

MIT License - See LICENSE file for details

---

## 🎉 Get Started

1. **Install the extension**
2. **Configure `parasoft.toolPath`** in settings
3. **Open a C/C++ file**
4. **Watch violations appear in real-time!**

Happy coding! 🚀
