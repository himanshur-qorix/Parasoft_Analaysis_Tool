# AI Modes Guide

## Overview

The Parasoft Analysis Tool supports **three AI modes** for fix generation, giving you full control over how violations are analyzed and fixed.

---

## AI Modes

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

---

### 2. **Hybrid** (`hybrid`) - **RECOMMENDED**

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

---

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

---

## How to Select AI Mode

### Method 1: Interactive (Run.bat)

```batch
.\scripts\Run.bat
```

You'll be prompted:
```
==================================================
AI Mode Selection
==================================================

Choose fix generation mode:
  [1] AI Only      - Use Ollama AI for all violations (requires Ollama)
  [2] Hybrid       - Smart: AI for complex, rules for standard (recommended)
  [3] Rules Only   - Use only rule-based fixes (no AI required)

Enter choice (1-3) [default: 2]:
```

### Method 2: Command Line

```bash
# AI Only
python scripts\run_agent.py report_dev1.html Mka --ai-mode ai_only

# Hybrid (default)
python scripts\run_agent.py report_dev1.html Mka --ai-mode hybrid

# Rules Only
python scripts\run_agent.py report_dev1.html Mka --ai-mode rules_only
```

### Method 3: Configuration File

Edit `config/config.json`:

```json
{
  "ai_integration": {
    "enabled": true,
    "provider": "ollama",
    "ai_mode": "hybrid",
    ...
  }
}
```

Options: `"ai_only"`, `"hybrid"`, `"rules_only"`

---

## Comparison Table

| Feature | AI Only | Hybrid | Rules Only |
|---------|---------|--------|------------|
| **Speed** | ⚫⚫⚪⚪⚪ | ⚫⚫⚫⚫⚪ | ⚫⚫⚫⚫⚫ |
| **Quality** | ⚫⚫⚫⚫⚫ | ⚫⚫⚫⚫⚪ | ⚫⚫⚫⚪⚪ |
| **Resources** | High | Medium | Low |
| **Setup** | Complex | Easy | None |
| **Offline** | ❌ | ⚠️ | ✅ |
| **Consistency** | Variable | Balanced | High |

---

## Decision Tree

```
┌─────────────────────────────────┐
│ Do you have Ollama installed?  │
└────────┬────────────────────────┘
         │
    ┌────┴────┐
   Yes        No
    │          │
    │          └─> Rules Only
    │
    ├─> Do you need maximum AI insights?
    │
    ├─ Yes ──> AI Only
    │
    └─ No ──> Hybrid (Recommended)
```

---

## Performance Examples

**Test Case:** 1000 violations analysis

| Mode | Time | AI Calls | Resource Usage |
|------|------|----------|----------------|
| AI Only | ~15 min | 1000 | 4GB RAM, 80% CPU |
| Hybrid | ~5 min | 150 | 2GB RAM, 40% CPU |
| Rules Only | ~2 min | 0 | 1GB RAM, 20% CPU |

*Note: Times vary based on model size and hardware*

---

## Ollama Setup (for AI modes)

**Quick Setup:**

```bash
# 1. Install Ollama
winget install Ollama.Ollama

# 2. Pull model
ollama pull qwen2.5-coder:1.5b-base

# 3. Start server
ollama serve

# 4. Verify
ollama list
```

See [OLLAMA_SETUP.md](OLLAMA_SETUP.md) for detailed instructions.

---

## FAQ

### Q: Which mode should I use?

**A:** Start with **Hybrid** (default). It provides excellent results with good performance.

### Q: What if Ollama is not running?

**A:** Hybrid automatically falls back to Rules Only. AI Only will fail if Ollama is unavailable.

### Q: Can I change modes during analysis?

**A:** No, select before starting analysis. You can re-run with a different mode.

### Q: Does Rules Only still work well?

**A:** Yes! Rule-based fixes handle 80%+ of common violations very effectively.

### Q: How do I know which mode was used?

**A:** Check the console output:
```
[INFO] AI Mode: hybrid
[OK] AI Mode: hybrid | Model: qwen2.5-coder:1.5b-base at http://localhost:11434
```

---

## Best Practices

### Use AI Only When:
- ✅ First-time analysis of complex codebase
- ✅ You have plenty of time
- ✅ Maximum quality is critical

### Use Hybrid When:
- ✅ Regular analysis workflow
- ✅ Production environments
- ✅ You want balance

### Use Rules Only When:
- ✅ Quick analysis needed
- ✅ No AI available
- ✅ Consistent results required
- ✅ CI/CD pipelines

---

## See Also

- [AI_QUICKSTART.md](AI_QUICKSTART.md) - Quick start guide
- [OLLAMA_SETUP.md](OLLAMA_SETUP.md) - Detailed Ollama setup
- [ARCHITECTURE_DIAGRAM.md](ARCHITECTURE_DIAGRAM.md) - System architecture

---

**Last Updated:** April 10, 2026
