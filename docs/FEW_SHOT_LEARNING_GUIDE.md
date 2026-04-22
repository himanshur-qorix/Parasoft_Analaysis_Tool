# Few-Shot Learning for Justification Generation

## Overview

The Parasoft Analysis Tool now supports **few-shot learning** for AI-powered justification generation. This feature enables the Ollama Qwen LLM to learn from existing justification examples in your codebase to generate better, more contextual, and standardized justifications.

## What is Few-Shot Learning?

Few-shot learning is a machine learning technique where the AI model is provided with a few examples of the desired output format before making predictions. In this context, the LLM learns from your existing justification patterns to generate new justifications that match your team's style and standards.

## How It Works

### 1. **Example Collection**
The system automatically scans the `justifications/` folder and loads examples from existing suppression comment files (`*_suppress_comments_*.txt`). These examples show:
- Rule IDs that were suppressed
- Justification format used
- Reasoning patterns from different modules

### 2. **Example Selection**
The system intelligently selects diverse examples by:
- Loading from multiple module files (up to 3 different modules)
- Balancing between CERT, MISRA, and other rule categories
- Limiting to 10 most relevant examples to avoid overwhelming the LLM

### 3. **Contextual Prompting**
When generating a justification for a new violation:
- Relevant examples matching the violation category are prioritized
- Examples are formatted into the prompt showing:
  - Module name where the suppression was used
  - Rule IDs involved
  - Exact suppression format
  - Reasoning pattern

### 4. **AI Generation**
The LLM analyzes:
- The violation details
- Cross-module consistency (how other modules handled similar violations)
- Example justification patterns
- Standard format requirements

Then generates a justification that:
- Follows your established format
- Provides meaningful, context-specific reasons
- Maintains consistency with existing patterns

## Configuration

### Enable/Disable Few-Shot Learning

Edit `config/config.json`:

```json
{
  "ai_integration": {
    "enabled": true,
    "provider": "ollama",
    "ollama": {
      "base_url": "http://localhost:11434",
      "model": "qwen2.5-coder:1.5b-base",
      "use_few_shot_learning": true    // Set to false to disable
    }
  }
}
```

### Default Settings
- **use_few_shot_learning**: `true` (enabled by default)
- **examples_dir**: `justifications/` (automatically configured)
- **max_examples**: 10 (hardcoded, can be modified in code)
- **files_sampled**: 3 (loads examples from up to 3 different modules)

## Benefits

### Before Few-Shot Learning (Rule-Based)
```c
/* parasoft-begin-suppress MISRAC2012-RULE_8_13-a-4 "Reason: Mka_Parasoft_REF_316" */
```
**Issues:**
- Generic, non-descriptive reason
- Just a reference ID, no context
- Doesn't explain WHY the violation is justified

### After Few-Shot Learning (AI-Powered)
```c
/* parasoft-begin-suppress MISRAC2012-RULE_8_13-a-4 "Reason: Parameter must remain non-const for API compatibility with legacy modules - consistent with NvM, SecOC suppression patterns" */
```
**Benefits:**
- Descriptive, meaningful reason
- Explains the justification context
- References cross-module consistency
- Maintains standard format

## Example Output

When few-shot learning is enabled, the AI generates structured justifications like:

```json
{
  "should_justify": true,
  "confidence": "HIGH",
  "reason": "Common deviation across modules - API compatibility requirement",
  "suggested_rationale": "Parameter constness deviation required for backward compatibility with legacy AUTOSAR stack - matches suppression patterns in NvM, SecOC modules"
}
```

## Justification Standard Format

The system learns and enforces this format:

```c
/* parasoft-begin-suppress RULE_ID_1 RULE_ID_2 "Reason: [Meaningful explanation]" */
... (your code at line X) ...
/* parasoft-end-suppress RULE_ID_1 RULE_ID_2 */
```

**Key Elements:**
- **parasoft-begin-suppress**: Opening suppression marker
- **RULE_ID(s)**: One or more rule identifiers being suppressed
- **"Reason: [text]"**: Enclosed in quotes, starts with "Reason:", followed by meaningful explanation
- **parasoft-end-suppress**: Closing suppression marker with matching rule IDs

## Cross-Module Learning

The few-shot system considers how violations are handled across different modules:

**Example Learning Path:**
1. Module `Mka` justifies `MISRAC2012-RULE_8_13` due to API constraints
2. Module `NvM` justifies the same rule for similar reasons
3. Module `SecOC` encounters the same violation

The AI learns from Mka and NvM examples and generates a consistent justification for SecOC, noting the cross-module pattern.

## Logs and Debugging

### Enable Detailed Logging

The few-shot learning system logs its operation:

```
[OK] Loaded 8 justification examples for few-shot learning
[AI] Analyzing justification recommendation for MISRAC2012-RULE_8_13-a-4...
[OK] Justification analysis: JUSTIFY (confidence: HIGH)
```

### Log Locations
- **Main log**: `parasoft_agent.log`
- **Level**: Set in `config.json` → `logging.level`

### Debug Information
Set log level to `DEBUG` to see:
- Example parsing details
- Prompt construction with examples
- AI response analysis
- Example selection logic

## Best Practices

### 1. **Maintain Quality Examples**
- Keep justification files in the `justifications/` folder
- Use descriptive, meaningful reasons in your manual justifications
- The AI learns from these patterns

### 2. **Review AI Suggestions**
- AI suggestions are recommendations, not automatic applications
- Review the suggested rationale before applying
- The "AI Justification Suggestions" Excel sheet lists all recommendations

### 3. **Update Examples Regularly**
- As you add new justified violations, they become training examples
- Remove outdated or incorrect justification files
- The system automatically loads the latest examples

### 4. **Use Cross-Module Analysis**
- Enable cross-module knowledge in your workflow
- Consistent justifications across modules strengthen the learning
- The AI recognizes patterns and suggests alignment

## Troubleshooting

### No Examples Loaded
```
[INFO] No justification example files found for few-shot learning
```
**Solution:** Ensure you have `*_suppress_comments_*.txt` files in the `justifications/` folder

### Poor Quality Justifications
**Solution:** 
- Improve existing justification quality in your source files
- Ensure examples use descriptive reasons, not just reference IDs
- Add more diverse examples from different modules

### Examples Not Applied
**Check:**
1. `use_few_shot_learning` is set to `true` in config
2. Ollama integration is enabled
3. Examples directory exists and contains valid files
4. Log level is set to INFO or DEBUG to see loading messages

## Advanced Customization

### Modify Example Selection

Edit `src/OllamaIntegration.py`:

```python
def _load_justification_examples(self) -> None:
    # Modify sampling logic
    sampled_files = suppress_files[:5]  # Load from 5 files instead of 3
    
    # Limit to different number of examples
    if len(self.justification_examples) > 15:  # 15 instead of 10
        ...
```

### Add Custom Example Formats

You can manually add exemplary justifications to guide the AI:

```python
self.justification_examples.append({
    'module': 'ExampleModule',
    'rules': 'MISRAC2012-RULE_X-Y',
    'reason': 'Detailed reasoning pattern to follow',
    'format': '/* parasoft-begin-suppress ... */'
})
```

## Integration with Existing Workflows

### Workflow with Few-Shot Learning

1. **Run Analysis**: `python src/ParasoftAnalysisTool.py <module_name>`
2. **AI analyzes violations** with few-shot examples
3. **Review suggestions** in Excel report ("AI Justification Suggestions" sheet)
4. **Apply justified suppressions** using the apply script
5. **New justifications** become training examples for future runs

### Hybrid Mode (Recommended)
- AI analyzes violations that don't match Parasoft DB rules
- Few-shot learning provides context-aware justifications
- Rule-based fixes for known patterns
- Best of both approaches

## Performance Considerations

- **Startup time**: +0.5-1s to load and parse examples
- **Memory**: Minimal (10 examples ≈ few KB)
- **Inference time**: No significant impact (examples already in prompt)
- **Token usage**: ~200-500 additional tokens per justification request

## Version Compatibility

- **Introduced**: v2.0.0
- **Ollama Models**: Compatible with all models (tested with qwen2.5-coder:1.5b-base)
- **Python**: 3.7+
- **Dependencies**: No additional packages required

## FAQ

**Q: Can I use my own example files?**  
A: Yes! Place any `*_suppress_comments_*.txt` file in the `justifications/` folder

**Q: Does this work without Ollama?**  
A: No, few-shot learning requires AI integration to be enabled

**Q: How often are examples reloaded?**  
A: Examples are loaded once at agent initialization. Restart the agent to reload

**Q: Can I see which examples are being used?**  
A: Yes, enable DEBUG logging to see example selection and prompt construction

**Q: Does this replace rule-based generation?**  
A: No, it enhances AI-powered justification analysis. Rule-based fixes still work for code fixes

## Related Documentation

- [AI_MODES.md](AI_MODES.md) - Understanding AI mode configuration
- [OLLAMA_SETUP.md](OLLAMA_SETUP.md) - Setting up Ollama integration
- [EXAMPLE_WORKFLOW.md](EXAMPLE_WORKFLOW.md) - Complete workflow with AI features
- [KNOWLEDGE_BASE_INTEGRATION.md](KNOWLEDGE_BASE_INTEGRATION.md) - Cross-module learning

## Summary

Few-shot learning transforms generic, rule-based justifications into meaningful, context-aware suppressions by learning from your existing codebase patterns. This ensures:
- **Consistency** across modules
- **Meaningful** justification reasons
- **Standard** format compliance
- **Time savings** through automated generation
- **Quality improvement** through learned patterns

Enable it today by setting `use_few_shot_learning: true` in your config!
