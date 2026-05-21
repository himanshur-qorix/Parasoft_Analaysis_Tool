# Quick Setup Guide

## 🚀 5-Minute Setup

### Step 1: Install Dependencies (First time only)

```bash
cd vscode-extension
npm install
```

### Step 2: Configure VS Code Settings

1. Open VS Code Settings: `Ctrl+,` (or `Cmd+,` on Mac)
2. Search for "Parasoft"
3. Set **Tool Path**:
   ```
   D:/Task&Data/CY26Q2_SP2/ParsoftFixes/Parasoft_Analaysis_Tool
   ```
4. Set **Module Name**: `Mka` (or your module name)

### Step 3: Run in Development Mode

```bash
# Method A: From terminal
cd vscode-extension
code .
# Then press F5 in VS Code

# Method B: From VS Code
# File → Open Folder → select vscode-extension
# Press F5
```

### Step 4: Test the Extension

1. In the **Extension Development Host** window:
2. Open a `.c` or `.cpp` file from your source code
3. Save the file (`Ctrl+S`)
4. **Violations appear!** 🎉

---

## 📦 Build & Install (Production)

### Build VSIX Package

```bash
cd vscode-extension
npm install -g @vscode/vsce
vsce package
```

This creates: `parasoft-cert-misra-analyzer-1.0.0.vsix`

### Install in VS Code

1. Open Command Palette: `Ctrl+Shift+P`
2. Type: `Extensions: Install from VSIX`
3. Select the `.vsix` file
4. Reload VS Code

---

## ⚙️ Minimal Configuration

Create `.vscode/settings.json` in your workspace:

```json
{
  "parasoft.enabled": true,
  "parasoft.toolPath": "D:/Task&Data/CY26Q2_SP2/ParsoftFixes/Parasoft_Analaysis_Tool",
  "parasoft.moduleName": "Mka",
  "parasoft.analyzeOnSave": true
}
```

---

## 🧪 Quick Test

1. **Create test file**: `test.c`
   ```c
   int main() {
       int x;
       return x;  // Should trigger CERT-C-DCL37-a
   }
   ```

2. **Save file**: `Ctrl+S`

3. **Check Problems**: `Ctrl+Shift+M`
   - Should see: "Variable must be initialized"

4. **Apply fix**:
   - Click lightbulb (or `Ctrl+.`)
   - Select "Generate AI fix"
   - Click "Apply Fix"

---

## 🔍 Verify Installation

### Check Extension Status

1. Open Output Channel:
   - View → Output
   - Select "Parasoft Analyzer" from dropdown

2. Look for:
   ```
   ✓ Parasoft CERT/MISRA Analyzer activated
   ✓ Workspace: D:/YourWorkspace
   ✓ Loaded Knowledge Base: ...path.../Mka_KnowledgeDatabase.json
     Total violations: 1792
   ```

### Check Status Bar

- Bottom-left corner should show: `$(bug) Parasoft`
- Click it to analyze current file

---

## 🐛 Common Issues

### "Parasoft tool path not found"
**Fix**: Set `parasoft.toolPath` in settings (see Step 2)

### "Knowledge Base not found"
**Fix**: Verify `parasoft.moduleName` matches your JSON file name

### "No violations detected"
**Fix**: Check that your file is in the Knowledge Base (run GUI first)

### Extension not loading
**Fix**: 
1. Reload window: `Ctrl+Shift+P` → `Developer: Reload Window`
2. Check file language is "C" or "C++": Click language in status bar

---

## 📝 Settings Template

Copy this to your `.vscode/settings.json`:

```json
{
  "// === Parasoft Configuration ===": "",
  "parasoft.enabled": true,
  "parasoft.analyzeOnSave": true,
  "parasoft.analyzeOnType": false,
  
  "// === Paths ===": "",
  "parasoft.toolPath": "D:/Task&Data/CY26Q2_SP2/ParsoftFixes/Parasoft_Analaysis_Tool",
  "parasoft.pythonPath": "python",
  
  "// === Module ===": "",
  "parasoft.moduleName": "Mka",
  
  "// === Filters ===": "",
  "parasoft.severityFilter": [
    "CRITICAL",
    "HIGH",
    "MEDIUM"
  ],
  "parasoft.showCERT": true,
  "parasoft.showMISRA": true,
  
  "// === AI Features ===": "",
  "parasoft.aiEnabled": true
}
```

---

## 🎯 Next Steps

1. ✅ Extension installed and configured
2. ✅ Test with sample file
3. ✅ Verify violations appear
4. 📖 Read full README.md for advanced features
5. 🔧 Customize settings to your workflow

---

## 🆘 Need Help?

- **Detailed docs**: See `README.md`
- **Output logs**: View → Output → "Parasoft Analyzer"
- **Extension logs**: View → Output → "Extension Host"

---

**Ready to code!** 🚀
