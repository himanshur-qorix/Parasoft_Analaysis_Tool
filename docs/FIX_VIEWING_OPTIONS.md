# 🎨 Code Fix Viewing Options

**Organization:** Qorix India Pvt Ltd  
**Version:** 2.3.0

---

## 📋 Overview

After generating code fix suggestions, you have **3 ways** to view and interact with them:

1. **Interactive Console Viewer** ⚡ *(Recommended)*
2. **HTML Report** 🌐 *(Best for sharing)*
3. **Text File** 📄 *(Simple viewing)*

---

## 🎮 Option 1: Interactive Console Viewer (RECOMMENDED)

### **What It Is**
A keyboard-driven terminal interface for browsing fixes one at a time with full-screen display.

### **How to Launch**
```batch
# Automatically after Run.bat or Generate_Code_Fixes.bat
# Or manually:
python src\view_fixes_interactive.py fixes\Mka\Mka_fixes_20260415_132653.txt
```

### **Features**
✅ **Keyboard Navigation** - Arrow keys or A/D to move between fixes  
✅ **Color-Coded Display** - Priority, severity, fix type highlighted  
✅ **Full-Screen View** - One fix at a time, easy to read  
✅ **Search Function** - Find fixes by violation ID or keyword  
✅ **Quick HTML Access** - Press H to open HTML report  
✅ **Code Examples** - Side-by-side violation and repair code  

### **Navigation Controls**

| Key | Action |
|-----|--------|
| **→** or **D** | Next fix |
| **←** or **A** | Previous fix |
| **S** | Search for fix |
| **H** | Open HTML report |
| **Q** or **ESC** | Quit viewer |

### **Screen Layout**
```
════════════════════════════════════════════════════════════════════════════════════
  INTERACTIVE FIX VIEWER - Qorix India Pvt Ltd
  Fix 15 of 111
════════════════════════════════════════════════════════════════════════════════════

┌─ VIOLATION INFO ────────────────────────────────────────────────────────────────┐
│ ID: CERT_C-STR31-a-2
│ Severity: HIGH  │ Category: CERT  │ Priority: HIGH
│ Fix Type: parasoft_official
└─────────────────────────────────────────────────────────────────────────────────┘

┌─ DESCRIPTION ───────────────────────────────────────────────────────────────────┐
  The expression may result in array out of bounds access
└─────────────────────────────────────────────────────────────────────────────────┘

┌─ FIX SUGGESTION ────────────────────────────────────────────────────────────────┐
  Description: Detects array access operations which may result in out of bounds...
  Security Impact: Array out of bounds can lead to buffer overflow attacks...
└─────────────────────────────────────────────────────────────────────────────────┘

┌─ CODE EXAMPLE ──────────────────────────────────────────────────────────────────┐
  ❌ VIOLATION:
    for (i = 0; i <= size; i++) {  // Out of bounds!
        array[i] = 0;
    }

  ✅ REPAIR:
    for (i = 0; i < size; i++) {  // NO VIOLATION
        array[i] = 0;
    }
└─────────────────────────────────────────────────────────────────────────────────┘

┌─ AFFECTED FILES (1155 total) ───────────────────────────────────────────────────┐
  1. Mka_Internal.c:1234
  2. Mka_Utils.c:567
  ... and 1153 more files
└─────────────────────────────────────────────────────────────────────────────────┘

────────────────────────────────────────────────────────────────────────────────────
  NAVIGATION:  ←→ or A/D = Prev/Next  │  H = HTML Report  │  S = Search  │  Q = Quit
────────────────────────────────────────────────────────────────────────────────────
```

### **Advantages**
- ⚡ **Fast** - Keyboard navigation, no mouse needed
- 🎯 **Focused** - One fix at a time, no distractions
- 🔍 **Searchable** - Instant search without leaving viewer
- 🎨 **Color-Coded** - Easy identification of priority/severity
- 💻 **Developer-Friendly** - Terminal-based, integrates with workflow

---

## 🌐 Option 2: HTML Report (Best for Sharing)

### **What It Is**
A beautiful, interactive web page with Qorix branding, filtering, and syntax highlighting.

### **How to Open**
```batch
# Automatically after generation (select option H)
# Or manually:
start fixes\Mka\Mka_fixes_20260415_132653.html
```

### **Features**
✅ **Professional Design** - Qorix branding with gradient headers  
✅ **Live Filtering** - Search, filter by priority/type in real-time  
✅ **Syntax Highlighting** - Code blocks with color coding  
✅ **Collapsible Sections** - Click headers to expand/collapse  
✅ **Dashboard Stats** - Total fixes, Parasoft official count, high priority count  
✅ **Shareable** - Send to team members, attach to reports  
✅ **Print-Friendly** - Clean print layout  
✅ **CWE Tags** - Security vulnerability mappings displayed  

### **Interface Elements**

**Dashboard Stats:**
```
┌─────────────┬────────────────────┬───────────────┬────────────┐
│ Total Fixes │ Parasoft Official  │ High Priority │ Categories │
│     111     │        85          │      42       │     12     │
└─────────────┴────────────────────┴───────────────┴────────────┘
```

**Filters:**
- 🔍 **Search Bar** - Filter by violation ID or description
- 🎯 **Priority Filter** - Critical, High, Medium, Low
- 🏷️ **Type Filter** - Parasoft Official, AI Generated, Rule Based

**Fix Cards:**
Each fix is displayed as a card with:
- Color-coded header (gradient Qorix colors)
- Badges for priority, type, severity
- Expandable sections
- Before/after code blocks
- CWE security tags
- List of affected files

### **Advantages**
- 📊 **Visual Overview** - See all stats at a glance
- 🔎 **Advanced Filtering** - Multiple filter combinations
- 👥 **Team Sharing** - Easy to send to colleagues
- 📱 **Responsive** - Works on any device
- 🖨️ **Printable** - Generate PDF reports
- 🎨 **Professional** - Qorix branding for presentations

---

## 📄 Option 3: Text File (Simple Viewing)

### **What It Is**
Plain text format for quick viewing or scripting.

### **How to Open**
```batch
notepad fixes\Mka\Mka_fixes_20260415_132653.txt
```

### **Format**
```
════════════════════════════════════════════════════════════════════════════════
PARASOFT FIXES - Module: Mka
Generated: 2026-04-15 13:26:53
Total Fixes: 111
════════════════════════════════════════════════════════════════════════════════

FIX #1
────────────────────────────────────────────────────────────────────────────────
Violation ID: CERT_C-INT31-i-2
Severity: LOW
Category: CERT

Description:
The expression of the 'essentially Enum' type is assigned to an object with 
the 'essentially unsigned' type

Fix Type: parasoft_official
Priority: HIGH

Fix Description:
Description: Detects operations on expressions...

Example:

--- VIOLATION EXAMPLE ---
unsigned int sum = a + b;  // May overflow

--- OFFICIAL REPAIR ---
if (a > UINT_MAX - b) {
    // Handle overflow
} else {
    unsigned int sum = a + b;
}

Files Affected:
  - Mka_Internal.c:1234
  - Mka_Utils.c:567
  ... and 1153 more files

════════════════════════════════════════════════════════════════════════════════
```

### **Advantages**
- 📝 **Simple** - No special tools needed
- 📋 **Copy-Paste** - Easy to extract code
- 🔧 **Scriptable** - Parse with scripts/tools
- 💾 **Lightweight** - Small file size
- 📤 **Portable** - Works anywhere

---

## 🎯 Which Option Should I Use?

### **For Daily Development Work:**
👉 **Interactive Viewer** - Fast keyboard navigation, focused viewing

### **For Team Reviews:**
👉 **HTML Report** - Professional formatting, easy sharing

### **For Automation/Scripting:**
👉 **Text File** - Parse with tools, integrate with CI/CD

### **For Management Reports:**
👉 **HTML Report** - Print to PDF, professional presentation

---

## 🔄 Workflow Examples

### **Example 1: Quick Review (Developer)**
```batch
1. Run.bat → Select "Y" (Interactive)
2. Navigate with arrow keys
3. Press "S" to search for specific violations
4. Press "H" if HTML detail needed
5. Press "Q" when done
```

### **Example 2: Team Meeting (Manager)**
```batch
1. Generate_Code_Fixes.bat → Select "2" (HTML)
2. HTML opens in browser
3. Use filters to show only HIGH priority
4. Share screen for discussion
5. Export to PDF for documentation
```

### **Example 3: Automation (CI/CD)**
```batch
1. Generate fixes via command line
2. Parse text file with script
3. Generate custom reports
4. Integrate with ticketing system
```

---

## 💡 Pro Tips

### **Interactive Viewer:**
- Use **Search (S)** to jump to specific violation IDs
- Press **H** to instantly open HTML for detailed view
- **A/D keys** are often faster than arrow keys

### **HTML Report:**
- Use **Ctrl+F** in browser for even more search power
- **Click headers** to collapse sections you've reviewed
- **Filter by Parasoft Official** to see gold-standard fixes first
- **Right-click → Save Page As** to archive the report

### **Text File:**
- Use **Ctrl+F** in Notepad for search
- **Copy code examples** directly to your editor
- **Save as template** for repetitive fixes

---

## 📞 Support

**Qorix India Pvt Ltd**  
📧 Email: support@qorix.com  
🌐 Website: www.qorix.com

---

**Last Updated:** April 15, 2026  
**Tool Version:** 2.3.0
