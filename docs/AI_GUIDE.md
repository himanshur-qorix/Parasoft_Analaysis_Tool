# AI Features & Integration Guide

**Complete guide to AI capabilities, modes, setup, and learning features**

---

## Table of Contents

1. [AI Modes](#ai-modes)
2. [Ollama Setup](#ollama-setup)
3. [AI Chat Feature](#ai-chat-feature)
4. [Pattern Learning](#pattern-learning)
5. [Few-Shot Learning](#few-shot-learning)
6. [AI Quality & Validation](#ai-quality--validation)

---

## AI Modes

### Overview

The Parasoft Analysis Tool supports **three AI modes** for fix generation, giving you full control over how violations are analyzed and fixed.

### 1. **AI Only** (`ai_only`)

**Description:** Uses Ollama AI for **all** violations

**Best For:**
- Maximum AI-powered analysis
- Complex codebases with unusual patterns
- When you want AI suggestions for everything

**Requirements:**
- ✅ Ollama must be installed and running
- ✅ Model must be downloaded (`ollama pull qwen2.5-coder:1.5b-base`)
- ⚠️ Higher resource usage (CPU/RAM)
- ⚠️ Slower processing (AI inference for every violation)

**Pros:**
- Most intelligent suggestions
- Handles edge cases well
- Learns from context

**Cons:**
- Slower processing
- Requires Ollama running
- Higher resource consumption

### 2. **Hybrid** (`hybrid`) - **RECOMMENDED ⭐**

**Description:** Smart combination of AI and rule-based fixes

**How It Works:**
- **AI** for: Complex violations, unknown patterns, long descriptions, ambiguous cases
- **Rules** for: Standard CERT/MISRA violations, common patterns, well-known issues

**Best For:**
- Most use cases
- Balanced performance and quality
- Production environments

**Requirements:**
- ✅ Ollama installed for AI features (optional)
- ✅ Falls back to rules if AI unavailable

**Pros:**
- ✅ **Best performance** (fast + intelligent)
- ✅ **Automatic fallback** (works without AI)
- ✅ **Smart selection** (AI only when needed)
- ✅ Lower resource usage than AI Only

**Cons:**
- May miss some AI insights for standard violations

### 3. **Rules Only** (`rules_only`)

**Description:** Uses **only** rule-based fixes, no AI

**Best For:**
- Systems without AI resources
- Consistent, repeatable results
- Fast processing requirements
- Offline environments

**Requirements:**
- ✅ No Ollama required
- ✅ Works on any machine
- ✅ No internet needed

**Pros:**
- ✅ **Fastest processing**
- ✅ **Zero dependencies** (no AI setup)
- ✅ **Consistent results**
- ✅ **Minimal resources**

**Cons:**
- No AI insights for complex violations
- May miss edge cases

### How to Select AI Mode

**Method 1: GUI (Recommended)**
```
1. Launch scripts\Launch_GUI.bat
2. Enter module name
3. Mode automatically set to "hybrid" (best balance)
```

**Method 2: Batch File**
```powershell
.\batch_support\Run.bat
# When prompted, enter: hybrid / ai_only / rules_only
```

**Method 3: Command Line**
```bash
# Hybrid (recommended)
python src\run_agent.py parasoftReport\report_dev1.html Mka --ai-mode hybrid

# AI Only
python src\run_agent.py parasoftReport\report_dev1.html Mka --ai-mode ai_only

# Rules Only (no AI)
python src\run_agent.py parasoftReport\report_dev1.html Mka --ai-mode rules_only
```

### Performance Comparison

| Mode | Processing Speed | Fix Quality | Setup Required | Resource Usage |
|------|------------------|-------------|----------------|----------------|
| **Hybrid** ⭐ | Fast (2-5 min) | Excellent | Ollama optional | Low |
| AI Only | Slow (5-15 min) | Excellent | Ollama required | High |
| Rules Only | Very Fast (1-2 min) | Good | None | Very Low |

---

## Ollama Setup

### What is Ollama?

Ollama is a **lightweight local LLM runtime** that lets you run AI models on your own machine with:
- ✅ **Zero API costs** - Completely free
- ✅ **Data privacy** - All code stays on your machine
- ✅ **Offline capability** - Works without internet
- ✅ **Low resource usage** - Small models (2-4GB RAM)
- ✅ **Simple setup** - 3 commands to get running

### Installation (5 minutes)

#### Step 1: Install Ollama

**Windows:**
```powershell
# Option 1: Download installer
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

#### Step 2: Pull a Model

Choose one model based on your needs:

```bash
# Recommended: Qwen 2.5 Coder 1.5B (Code-specialized, ~900MB)
ollama pull qwen2.5-coder:1.5b-base

# Alternative: Qwen 2.5 7B (Better quality, ~4.7GB)
ollama pull qwen2.5:latest

# Alternative: Phi3 Mini (3.8B params, ~2.3GB)
ollama pull phi3:mini

# Alternative: Llama 3.2 3B (~2GB)
ollama pull llama3.2:3b
```

**Model Comparison:**

| Model | Size | RAM Required | Speed | Quality | Best For |
|-------|------|--------------|-------|---------|----------|
| qwen2.5-coder:1.5b-base ⭐ | 900MB | 2GB | Very Fast | Excellent | Code analysis |
| qwen2.5:latest | 4.7GB | 8GB | Medium | Outstanding | Best quality |
| phi3:mini | 2.3GB | 4GB | Fast | Good | General use |
| llama3.2:3b | 2.0GB | 4GB | Fast | Good | Balanced |

#### Step 3: Start Ollama Server

```bash
# Start the server (runs in background)
ollama serve
```

**Windows:** After installation, Ollama usually starts automatically. Check system tray for Ollama icon.

#### Step 4: Verify Installation

```bash
# Test the model
ollama run qwen2.5-coder:1.5b-base "Hello, can you help with C code analysis?"
```

### Configuration

The tool automatically detects Ollama settings, but you can customize in `config/config.json`:

```json
{
  "ai_enabled": true,
  "ollama_base_url": "http://localhost:11434",
  "ollama_model": "qwen2.5-coder:1.5b-base",
  "ai_mode": "hybrid"
}
```

### Troubleshooting

**Issue: "Ollama server not responding"**
```bash
# Check if Ollama is running
ollama list

# Restart Ollama
# Windows: Restart from system tray
# Linux/Mac: killall ollama && ollama serve
```

**Issue: "Model not found"**
```bash
# List installed models
ollama list

# Pull the model
ollama pull qwen2.5-coder:1.5b-base
```

**Issue: "Connection timeout"**
- Increase timeout in GUI (60 seconds default)
- Large models may take 10-30 seconds to load on first use

---

## AI Chat Feature

### Overview

The AI Chat feature provides an interactive chat interface for asking questions about:
- Code violations and fixes
- CERT/MISRA standards
- Best practices
- Tool usage

### Quick Start

#### Step 1: Launch the GUI
```bash
scripts\Launch_GUI.bat
```

#### Step 2: Go to AI Chat Tab
Click the **"🤖 AI Chat"** tab in the GUI

#### Step 3: Select and Test Model
1. Wait for models to load automatically (shows "Loading..." initially)
2. Dropdown will populate with YOUR installed models (with sizes)
3. Select your preferred model from the list
4. Click **"✓ Test Connection"**
5. Wait a few seconds for the status to show **"✅ Connected"**

**Note:** Only models actually installed on your system will appear!

**Note:** First connection test may take 5-10 seconds as Ollama loads the model into memory.

#### Step 4: Start Chatting!
Type your question and click **"📤 Send"** or press **Ctrl+Enter**

### Example Prompts

**For Code Analysis:**
```
Explain what CERT_C-INT31-i-2 violation means
```

**For Code Fixes:**
```
How do I fix "essentially signed to essentially unsigned" cast violations?
```

**For Best Practices:**
```
Show me an example of proper enum to uint16 casting in C
```

**For Parasoft Rules:**
```
What are the best practices for suppressing Parasoft violations?
```

**For General Help:**
```
How do I use the Parasoft Analysis Tool?
```

### Features

- **Dynamic Model Detection** - Shows only models installed on your system
- **Model Size Display** - See parameter count for each model
- **Refresh Models** - Click refresh button to reload available models
- **Connection Testing** - Verify Ollama connectivity before chatting
- **Streaming Responses** - Watch AI responses appear in real-time
- **Chat History** - Scroll through conversation history
- **Copy-Paste Support** - Easy copying of code snippets

### Troubleshooting

**Connection Test Fails:**
- Ensure Ollama is running (`ollama list`)
- Check if model is downloaded (`ollama pull qwen2.5-coder:1.5b-base`)
- Wait 30-60 seconds for large models to load

**No Models in Dropdown:**
- Click "🔄 Refresh Models" button
- Install at least one model: `ollama pull qwen2.5-coder:1.5b-base`
- Check Ollama service: `ollama list`

**Slow Responses:**
- Use smaller model (qwen2.5-coder:1.5b-base)
- Close other applications to free RAM
- Wait for first inference to complete (slower, then faster)

---

## Pattern Learning

### Overview

The Pattern Learning system uses AI to analyze how your team resolves violations and builds a learning database for future recommendations.

### How It Works

1. **Capture Resolution Patterns** - Analyzes justification files to understand how violations are resolved
2. **Build Learning Database** - Creates a pattern database with confidence scores
3. **Provide Recommendations** - Suggests resolutions based on learned patterns

### Usage

#### Option 1: GUI
```
1. Launch scripts\Launch_GUI.bat
2. Go to "Advanced" tab
3. Click "Learn Patterns" button
4. Select justification folder when prompted
```

#### Option 2: Batch File
```powershell
.\batch_support\Learn_Justification_Patterns.bat
```

#### Option 3: Command Line
```bash
python src\learn_justification_patterns.py justifications/
```

### What Gets Learned

- **Suppression Patterns** - Common reasons for suppressing violations
- **Fix Patterns** - Typical code changes made
- **Justification Templates** - Reusable justification text
- **Rule-Specific Resolutions** - How each rule is typically handled
- **Confidence Scores** - How often each pattern succeeds

### Example Output

```json
{
  "CERT_C-INT31-i-2": {
    "resolution": "suppressed",
    "confidence": 0.85,
    "common_justification": "Enum to unsigned cast is safe in this context",
    "occurrences": 42
  }
}
```

### Benefits

- **Faster Analysis** - AI learns from past decisions
- **Consistent Resolutions** - Team patterns are preserved
- **Confidence-Based Suggestions** - Only high-confidence recommendations
- **Cross-Module Learning** - Patterns apply across all modules

---

## Few-Shot Learning

### Overview

Few-Shot Learning allows you to teach the AI system custom violation patterns and fixes by providing examples. The AI learns from your examples and applies similar fixes to new violations.

### Quick Start

#### Step 1: Add Examples to Training Data

Place example files in the `learning/` folder:

```
learning/
├── fixes_dataset/
│   ├── example_fix_001.txt
│   └── example_fix_002.txt
├── justifications_dataset/
│   ├── example_justification_001.txt
│   └── example_justification_002.txt
└── violations_dataset/
    ├── example_violation_001.txt
    └── example_violation_002.txt
```

#### Step 2: Format Examples

Each example should include:
- Violation description
- Code context
- Fix or justification
- Rule ID

**Example Fix File:**
```
Violation: CERT_C-INT31-i-2 - essentially signed to essentially unsigned
Code Before:
  uint16 timeout = TIMEOUT_ENUM;

Code After:
  uint16 timeout = (uint16)TIMEOUT_ENUM;

Explanation: Added explicit cast to uint16 for type safety
```

#### Step 3: Run Learning

```bash
# Process learning examples
python src\learn_justification_patterns.py learning/
```

### Example Templates

**Fix Example Template:**
```
Rule: CERT_C-INT31-i-2
Violation: The expression of the 'essentially signed' type is assigned to an object with the 'essentially unsigned' type
Context: Enum assignment to unsigned integer
Before: uint16 value = MY_ENUM_VALUE;
After: uint16 value = (uint16)MY_ENUM_VALUE;
Rationale: Explicit cast ensures type safety and compliance
```

**Justification Example Template:**
```
Rule: MISRA-C:2012 Rule 10.3
Violation: Expression assigned to different essential type
Justification: False positive - The enum and target type have compatible value ranges. The conversion is safe and intentional for this use case.
Status: Suppressed
```

### Best Practices

1. **Provide 3-5 examples** minimum per violation type
2. **Use real code** from your codebase
3. **Include context** (surrounding code, function purpose)
4. **Explain reasoning** clearly in each example
5. **Update regularly** as new patterns emerge

### Benefits

- **Custom Fix Patterns** - Teach AI your coding standards
- **Domain-Specific Knowledge** - Apply company-specific practices
- **Improved Accuracy** - AI learns from your actual code
- **Reduced False Positives** - Better context understanding

---

## AI Quality & Validation

### Quality Metrics

The AI system tracks quality metrics for all generated fixes and justifications:

| Metric | Description | Target |
|--------|-------------|--------|
| **Accuracy** | Fixes that compile and pass tests | >90% |
| **Relevance** | Fixes addressing the actual violation | >95% |
| **Consistency** | Similar fixes for similar violations | >85% |
| **Confidence** | AI confidence score for suggestions | >0.70 |

### Validation Process

1. **Syntax Check** - All fixes are validated for syntax correctness
2. **Context Validation** - Ensures fix matches violation context
3. **Pattern Matching** - Compares to known successful patterns
4. **Confidence Scoring** - Assigns confidence based on similarity to training data

### Quick Reference

**AI Mode Recommendations:**
- **Development/Testing:** Use `hybrid` mode
- **Production/CI:** Use `rules_only` mode
- **Complex Analysis:** Use `ai_only` mode
- **No AI Available:** Use `rules_only` mode

**Model Recommendations:**
- **Fast Processing:** qwen2.5-coder:1.5b-base (900MB)
- **Best Quality:** qwen2.5:latest (4.7GB)
- **Balanced:** phi3:mini (2.3GB)
- **Low Memory:** tinyllama:latest (~637MB)

**Performance Tips:**
- Use hybrid mode for best speed/quality balance
- Start with smallest model, upgrade if quality insufficient
- Keep Ollama running in background for faster inference
- First run slower (model loading), subsequent runs faster

---

## Summary

The AI system provides:
- ✅ **Three Flexible Modes** - Choose AI, hybrid, or rules-only
- ✅ **Local & Private** - No data leaves your machine
- ✅ **Zero Cost** - Free open-source models
- ✅ **Pattern Learning** - Learns from your team's decisions
- ✅ **Few-Shot Learning** - Customizable with examples
- ✅ **Interactive Chat** - Ask questions anytime
- ✅ **Quality Validation** - Confidence scoring and validation

For more details, see [QUICKSTART.md](QUICKSTART.md) or launch the GUI with `scripts\Launch_GUI.bat`.
