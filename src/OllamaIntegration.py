"""
Ollama Integration Module
Provides lightweight local LLM integration for code fix suggestions
"""

import logging
import json
from typing import Optional, Dict
from pathlib import Path

logger = logging.getLogger(__name__)

# Try importing ollama, but don't fail if not installed
try:
    import ollama
    OLLAMA_AVAILABLE = True
except ImportError:
    OLLAMA_AVAILABLE = False
    logger.warning("Ollama library not installed. AI features will be disabled.")


class OllamaIntegration:
    """Lightweight Ollama integration for local LLM-powered code analysis"""
    
    def __init__(self, config: Dict):
        """
        Initialize Ollama integration
        
        Args:
            config: Configuration dictionary with ollama settings
        """
        self.enabled = config.get('enabled', False) and OLLAMA_AVAILABLE
        self.provider = config.get('provider', 'none')
        self.ai_mode = config.get('ai_mode', 'hybrid')  # ai_only, hybrid, rules_only
        
        if self.provider == 'ollama':
            ollama_config = config.get('ollama', {})
            self.base_url = ollama_config.get('base_url', 'http://localhost:11434')
            self.model = ollama_config.get('model', 'phi3:mini')
            self.timeout = ollama_config.get('timeout', 120)
            self.fallback_to_rules = ollama_config.get('fallback_to_rules', True)
            
            self.max_tokens = config.get('max_tokens', 1000)
            self.temperature = config.get('temperature', 0.3)
            self.use_ai_for = config.get('use_ai_for', {})
            
            # Few-shot learning configuration
            self.use_few_shot = ollama_config.get('use_few_shot_learning', True)
            self.examples_dir = Path(__file__).parent.parent / 'justifications'
            self.justification_examples = []
            
            # Adjust behavior based on mode
            if self.ai_mode == 'rules_only':
                self.enabled = False
                logger.info("[INFO] AI Mode: Rules Only - AI disabled")
            elif self.ai_mode == 'ai_only':
                self.enabled = self.enabled and OLLAMA_AVAILABLE
                logger.info("[INFO] AI Mode: AI Only - Will use AI for all violations")
            else:  # hybrid
                logger.info("[INFO] AI Mode: Hybrid - Smart AI + rule-based fixes")
            
            # Test connection
            if self.enabled:
                self._test_connection()
                
                # Load few-shot examples for justification learning
                if self.use_few_shot:
                    self._load_justification_examples()
        else:
            self.enabled = False
            logger.info(f"AI provider '{self.provider}' is not Ollama. AI features disabled.")
    
    def _test_connection(self) -> bool:
        """Test connection to Ollama server"""
        try:
            response = ollama.list()
            logger.info(f"[OK] Connected to Ollama at {self.base_url}")
            
            # Handle both dict and typed response objects from ollama library
            # Convert to dict if it's a typed object (e.g., ListResponse)
            if hasattr(response, '__dict__'):
                # It's a typed object, try to access models attribute directly
                if hasattr(response, 'models'):
                    models_list = response.models
                else:
                    # Try converting to dict
                    try:
                        response_dict = dict(response) if not isinstance(response, dict) else response
                        models_list = response_dict.get('models', [])
                    except:
                        logger.error(f"[ERROR] Cannot extract models from Ollama response type: {type(response)}")
                        self.enabled = False
                        return False
            elif isinstance(response, dict):
                models_list = response.get('models', [])
            else:
                logger.error(f"[ERROR] Unexpected response type from Ollama: {type(response)}")
                self.enabled = False
                return False
            
            # Check if models list is empty
            if not models_list:
                logger.warning(f"[WARNING] No models found in Ollama.")
                logger.warning(f"Run: ollama pull {self.model}")
                self.enabled = False
                return False
            
            # Extract model names with error handling
            available_models = []
            for model in models_list:
                # Handle typed model objects
                if hasattr(model, 'name'):
                    available_models.append(model.name)
                elif hasattr(model, 'model'):
                    available_models.append(model.model)
                elif isinstance(model, dict):
                    # Try different possible keys for model name
                    model_name = model.get('name') or model.get('model') or model.get('id')
                    if model_name:
                        available_models.append(model_name)
                    else:
                        logger.debug(f"Model entry missing name field: {model}")
                elif isinstance(model, str):
                    available_models.append(model)
            
            if not available_models:
                logger.warning(f"[WARNING] Could not extract model names from Ollama response.")
                logger.warning(f"Response type: {type(response)}, Models type: {type(models_list)}")
                if models_list:
                    logger.warning(f"First model structure: {models_list[0] if len(models_list) > 0 else 'N/A'}")
                self.enabled = False
                return False
            
            if self.model not in available_models:
                logger.warning(f"[WARNING] Model '{self.model}' not found in Ollama.")
                logger.warning(f"Available models: {', '.join(available_models)}")
                logger.warning(f"Run: ollama pull {self.model}")
                self.enabled = False
                return False
            else:
                logger.info(f"[OK] Model '{self.model}' is available")
            
            return True
            
        except Exception as e:
            logger.error(f"[ERROR] Failed to connect to Ollama: {str(e)}")
            logger.error("Make sure Ollama is running: ollama serve")
            self.enabled = False
            return False
    
    def should_use_ai(self, category: str, violation_text: str) -> bool:
        """
        Determine if AI should be used for this violation
        
        Args:
            category: Violation category (CERT, MISRA, etc.)
            violation_text: Violation description
        
        Returns:
            True if AI should be used
        """
        if not self.enabled:
            return False
        
        # AI Only mode - use AI for everything
        if self.ai_mode == 'ai_only':
            return True
        
        # Rules Only mode - never use AI
        if self.ai_mode == 'rules_only':
            return False
        
        # Hybrid mode - intelligent selection (more permissive to show AI capability)
        if self.ai_mode == 'hybrid':
            # Use AI for violations when Parasoft DB doesn't have a match
            # This method is called AFTER Parasoft DB check fails
            # So in hybrid mode, we should try AI as a fallback
            
            # First check if there's a category-specific override that disables AI
            if 'CERT' in category and not self.use_ai_for.get('cert_violations', True):
                logger.debug(f"AI disabled for CERT violations by config")
                return False
            
            if 'MISRA' in category and not self.use_ai_for.get('misra_violations', True):
                logger.debug(f"AI disabled for MISRA violations by config")
                return False
            
            # Default: Use AI for unknown/complex patterns in hybrid mode
            # This catches all violations that don't match Parasoft DB
            if self.use_ai_for.get('unknown_patterns', True):
                return True
            
            # If unknown_patterns is explicitly disabled, only use AI for enabled categories
            if 'CERT' in category and self.use_ai_for.get('cert_violations', False):
                return True
            
            if 'MISRA' in category and self.use_ai_for.get('misra_violations', False):
                return True
        
        return False
    
    def generate_fix_suggestion(self, violation: Dict) -> Optional[Dict]:
        """
        Generate fix suggestion using Ollama LLM
        
        Args:
            violation: Violation dictionary with details
        
        Returns:
            Fix suggestion dictionary or None if failed
        """
        if not self.enabled:
            logger.warning("❌ [AI] Ollama NOT enabled - skipping AI generation")
            logger.info(f"   Check config.json: ai_integration.enabled = {self.enabled}")
            return None
        
        try:
            violation_id = violation.get('violation_id', 'UNKNOWN')
            violation_text = violation.get('violation_text', '')
            category = violation.get('category', 'OTHER')
            severity = violation.get('severity', 'MEDIUM')
            code_context = violation.get('code_context')
            parasoft_reference = violation.get('parasoft_reference')
            
            logger.info(f"[AI] ⚡ Starting AI generation for {violation_id}")
            logger.info(f"[AI]   Model: {self.model} at {self.base_url}")
            logger.info(f"[AI]   Violation text: {violation_text[:100]}...")
            logger.info(f"[AI]   Category: {category}")
            logger.info(f"[AI]   Has code context: {code_context is not None}")
            if code_context:
                logger.info(f"[AI]   Code file: {code_context.get('file')}:{code_context.get('line')}")
                logger.info(f"[AI]   Target line: {code_context.get('target_line', '')[:80]}")
                logger.info(f"[AI]   Context block preview: {code_context.get('context', '')[:150]}...")
            else:
                logger.warning(f"[AI] ⚠️  NO CODE CONTEXT - will generate generic fix")
            
            logger.info(f"[AI] 📋 FULL VIOLATION DICT being passed to AI:")
            logger.info(f"     violation_id: {violation_id}")
            logger.info(f"     violation_text: {violation_text}")
            logger.info(f"     category: {category}")
            logger.info(f"     code_context: {code_context is not None}")
            
            # Build prompt with all available context
            prompt = self._build_prompt(violation_id, violation_text, category, severity, 
                                       code_context, parasoft_reference)
            
            logger.info(f"[AI] 📝 Built prompt ({len(prompt)} chars)")
            logger.info(f"[AI] 🔍 PROMPT PREVIEW (first 500 chars):")
            logger.info(f"{prompt[:500]}")
            logger.info(f"[AI] 🔍 PROMPT END (last 300 chars):")
            logger.info(f"{prompt[-300:]}")
            logger.debug(f"[AI] Full prompt:\n{prompt}\n{'='*70}")
            
            # Call Ollama
            logger.info(f"[AI] 🔄 Calling Ollama API...")
            logger.info(f"[AI]   Temperature: {self.temperature}, Max tokens: {self.max_tokens}")
            logger.info(f"[AI]   Timeout: {self.timeout} seconds")
            
            # Use lower temperature when we have code context for more precise fixes
            temperature = 0.1 if code_context else self.temperature
            if code_context:
                logger.info(f"[AI]   🎯 Using lower temperature (0.1) for specific code fix")
            
            # Create client with custom timeout
            from ollama import Client
            client = Client(host=self.base_url, timeout=self.timeout)
            
            response = client.generate(
                model=self.model,
                prompt=prompt,
                stream=False,  # Get complete response, not streaming
                options={
                    'temperature': temperature,
                    'num_predict': self.max_tokens,
                    'top_k': 40,
                    'top_p': 0.9,
                    'stop': ['\n\n\n']  # Stop after 3 newlines
                }
            )
            
            logger.info(f"[AI] ✅ Got response from Ollama")
            logger.debug(f"[AI] Response object type: {type(response)}")
            
            # Extract response text - handle both dict and typed objects
            if hasattr(response, 'response'):
                # Typed object with response attribute
                response_text = response.response
                logger.debug(f"[AI] Extracted from .response attribute")
            elif isinstance(response, dict):
                # Dictionary response
                response_text = response.get('response', '')
                logger.debug(f"[AI] Extracted from dict['response']")
            else:
                logger.error(f"[AI] ❌ Unexpected response type from Ollama: {type(response)}")
                return None
            
            logger.info(f"[AI] 📄 Response length: {len(response_text)} chars")
            
            # Check for empty response
            if not response_text or len(response_text.strip()) == 0:
                logger.error(f"[AI] ❌ EMPTY RESPONSE from Ollama!")
                logger.error(f"[AI] This usually means:")
                logger.error(f"[AI]   1. Model is a BASE model (not instruction-tuned)")
                logger.error(f"[AI]      → Your model: {self.model}")
                logger.error(f"[AI]      → BASE models don't follow instructions!")
                logger.error(f"[AI]      → Use an INSTRUCT model instead:")
                logger.error(f"[AI]         • qwen2.5-coder:1.5b (without -base)")
                logger.error(f"[AI]         • codellama:7b-instruct")
                logger.error(f"[AI]         • deepseek-coder:1.3b-instruct")
                logger.error(f"[AI]   2. Model timed out before generating")
                logger.error(f"[AI]   3. Model is still loading")
                logger.error(f"[AI] ")
                logger.error(f"[AI] 💡 FIX: Run 'ollama pull qwen2.5-coder:1.5b' (no -base)")
                logger.error(f"[AI]       Then update config.json model to 'qwen2.5-coder:1.5b'")
                return None
            
            logger.info(f"[AI] Response preview: {response_text[:300]}...")
            
            # Log the full response for debugging
            logger.info(f"[AI] 📄 FULL AI RESPONSE:")
            logger.info(f"{'='*70}")
            logger.info(response_text[:1000])  # First 1000 chars
            if len(response_text) > 1000:
                logger.info(f"... (response continues for {len(response_text)} total chars)")
            logger.info(f"{'='*70}")
            logger.debug(f"[AI] Full AI response:\n{response_text}\n{'='*70}")
            
            # Parse response
            logger.info(f"[AI] 🔍 Parsing response...")
            fix_data = self._parse_response(response_text, violation)
            
            if fix_data:
                logger.info(f"[AI] ✅ Successfully generated AI fix for {violation_id}")
                logger.info(f"[AI]   Type: {fix_data.get('type')}")
                logger.info(f"[AI]   Priority: {fix_data.get('priority')}")
                logger.info(f"[AI]   AI-generated: {fix_data.get('ai_generated', False)}")
                return fix_data
            else:
                logger.error(f"[AI] ❌ Failed to parse AI response for {violation_id}")
                logger.error(f"[AI] Raw response was:\n{response_text}")
                return None
            
        except Exception as e:
            logger.error(f"[AI] ❌ Ollama generation failed with exception: {str(e)}")
            import traceback
            logger.error(f"[AI] Traceback:\n{traceback.format_exc()}")
            if self.fallback_to_rules:
                logger.info("[AI] Falling back to rule-based generation")
            return None
    
    def generate_code_modification(self, violation: Dict, code_context: Dict) -> Optional[Dict]:
        """
        Generate actual code modification (not just suggestions) using AI
        This method is designed for the interactive code fixer to generate actual code changes
        
        Args:
            violation: Violation dictionary with details
            code_context: Code context with actual source code and line information
        
        Returns:
            Dictionary with modified_code and metadata, or None if failed
        """
        if not self.enabled:
            logger.debug("Ollama not enabled, skipping AI code modification")
            return None
        
        try:
            violation_id = violation.get('violation_id', 'UNKNOWN')
            violation_text = violation.get('violation_text', '')
            category = violation.get('category', 'OTHER')
            
            # Build enhanced prompt for code modification
            prompt = self._build_code_modification_prompt(violation_id, violation_text, category, code_context)
            
            logger.info(f"[AI-MOD] Generating code modification for {violation_id}...")
            logger.debug(f"   Context: {code_context['file']}:{code_context['line']}")
            
            # Create client with custom timeout
            from ollama import Client
            client = Client(host=self.base_url, timeout=self.timeout)
            
            # Call Ollama with lower temperature for precise code generation
            response = client.generate(
                model=self.model,
                prompt=prompt,
                options={
                    'temperature': 0.1,  # Lower temperature for more deterministic code
                    'num_predict': 2000,  # Allow longer responses for code blocks
                }
            )
            
            # Extract response text
            if hasattr(response, 'response'):
                response_text = response.response
            elif isinstance(response, dict):
                response_text = response.get('response', '')
            else:
                logger.error(f"[ERROR] Unexpected response type: {type(response)}")
                return None
            
            logger.debug(f"   AI response length: {len(response_text)} chars")
            
            # Parse response to extract modified code
            result = self._parse_code_modification_response(response_text, code_context)
            
            if result:
                logger.info(f"[OK] Generated code modification for {violation_id}")
                return result
            else:
                logger.warning(f"[WARNING] Could not parse code modification response")
                return None
        
        except Exception as e:
            logger.error(f"[ERROR] Code modification generation failed: {str(e)}")
            return None
    
    def _build_code_modification_prompt(self, violation_id: str, violation_text: str, 
                                        category: str, code_context: Dict) -> str:
        """
        Build prompt specifically for generating actual code modifications
        
        Args:
            violation_id: Violation ID
            violation_text: Violation description
            category: Category (MISRA, CERT, etc.)
            code_context: Code context with actual source code
        
        Returns:
            Prompt string optimized for code modification
        """
        prompt = f"""You are an expert C/C++ code refactoring assistant. Your task is to fix a specific coding standard violation by providing the COMPLETE modified code.

VIOLATION DETAILS:
- ID: {violation_id}
- Category: {category}
- Description: {violation_text}

CURRENT SOURCE CODE:
File: {code_context['file']}
Line {code_context['line']} contains the violation.

```c
{code_context['context']}
```

VIOLATION LINE:
>>> {code_context['target_code']}

YOUR TASK:
Provide the COMPLETE MODIFIED CODE that fixes this violation. You MUST:
1. Analyze the root cause of the violation
2. Apply the minimal necessary fix to resolve the violation
3. Return the COMPLETE corrected code block (replace the entire context shown above)
4. Ensure the fix follows {category} coding standards
5. Maintain all existing functionality and logic
6. Preserve formatting, indentation, and style as much as possible
7. Only change what's necessary to fix the violation

OUTPUT FORMAT (MANDATORY):
You must respond in this EXACT format:

ANALYSIS:
[Explain in 1-2 sentences what causes the violation and your fix approach]

MODIFIED_CODE:
```c
[The complete corrected code block - must be valid C/C++ code that replaces the context above]
```

TYPE: [One word: cast/bounds_check/initialization/refactor/validation/declaration/null_check]
PRIORITY: [One word: HIGH/MEDIUM/LOW]

CRITICAL RULES:
- The MODIFIED_CODE section must contain the COMPLETE replacement code
- Do NOT use placeholders like "...", "rest of code", or "/* unchanged code */"
- Include ALL lines from the original context, with your fixes applied
- The code must be syntactically correct and ready to compile
- Preserve all comments, except those that are directly related to the violation
- If you add any explanation, put it ONLY in the ANALYSIS section, NOT in the code"""
        
        return prompt
    
    def _parse_code_modification_response(self, response: str, code_context: Dict) -> Optional[Dict]:
        """
        Parse AI response to extract modified code and metadata
        
        Args:
            response: AI response text
            code_context: Original code context for validation
        
        Returns:
            Dictionary with modified_code and metadata, or None if parsing failed
        """
        import re
        
        try:
            # Extract analysis section
            analysis_match = re.search(r'ANALYSIS:\s*(.+?)(?=MODIFIED_CODE:|TYPE:|$)', response, re.DOTALL | re.IGNORECASE)
            analysis = analysis_match.group(1).strip() if analysis_match else "AI-generated code modification"
            
            # Extract modified code from code blocks
            # Look for ```c, ```cpp, or just ```
            code_block_pattern = r'```(?:c|cpp)?\s*\n(.+?)```'
            code_matches = re.findall(code_block_pattern, response, re.DOTALL)
            
            modified_code = None
            if code_matches:
                # Use the first (or largest) code block
                modified_code = max(code_matches, key=len).strip()
            else:
                # Try to find code after MODIFIED_CODE: without code fences
                code_text_match = re.search(r'MODIFIED_CODE:\s*\n(.+?)(?=\nTYPE:|$)', response, re.DOTALL | re.IGNORECASE)
                if code_text_match:
                    modified_code = code_text_match.group(1).strip()
            
            # Extract fix type
            type_match = re.search(r'TYPE:\s*(\w+)', response, re.IGNORECASE)
            fix_type = type_match.group(1).lower() if type_match else 'refactor'
            
            # Extract priority
            priority_match = re.search(r'PRIORITY:\s*(HIGH|MEDIUM|LOW)', response, re.IGNORECASE)
            priority = priority_match.group(1).upper() if priority_match else 'MEDIUM'
            
            # Validate extracted code
            if not modified_code:
                logger.warning("Could not extract modified code from AI response")
                logger.debug(f"Response preview: {response[:500]}")
                return None
            
            # Basic validation: check if modified code has some substance
            if len(modified_code.strip()) < 10:
                logger.warning("Extracted code is too short to be valid")
                return None
            
            # Check for placeholder text
            placeholder_markers = ['...', 'rest of code', 'unchanged code', 'your code here', 'add code here']
            code_lower = modified_code.lower()
            if any(marker in code_lower for marker in placeholder_markers):
                logger.warning("Modified code contains placeholder text, rejecting")
                return None
            
            return {
                'type': fix_type,
                'description': analysis,
                'modified_code': modified_code,
                'priority': priority,
                'ai_generated': True,
                'model': self.model
            }
        
        except Exception as e:
            logger.error(f"Error parsing code modification response: {e}")
            return None
    
    def _build_prompt(self, violation_id: str, violation_text: str, 
                     category: str, severity: str, code_context: Optional[Dict] = None,
                     parasoft_reference: Optional[Dict] = None) -> str:
        """
        Build prompt for Ollama with optional code context and Parasoft reference
        
        Args:
            violation_id: Violation ID
            violation_text: Violation description
            category: Category (CERT, MISRA)
            severity: Severity level
            code_context: Optional code context from source file
            parasoft_reference: Optional Parasoft DB reference information
        
        Returns:
            Prompt string
        """
        prompt = f"""You are a C/C++ code analysis expert. Generate a fix for this violation.

Violation: {violation_id}
Category: {category}
Description: {violation_text}"""
        
        # Add Parasoft reference if available
        if parasoft_reference:
            prompt += f"""

OFFICIAL PARASOFT GUIDANCE (Use as reference, generate your own fix):
Rule: {parasoft_reference.get('rule_title', 'N/A')}
"""
            if parasoft_reference.get('description'):
                prompt += f"Description: {parasoft_reference['description']}\n"
            
            if parasoft_reference.get('repair_strategy'):
                prompt += f"\nRepair Strategy: {parasoft_reference['repair_strategy']}\n"
            
            if parasoft_reference.get('example_repair'):
                prompt += f"\nExample Repair:\n{parasoft_reference['example_repair']}\n"
        
        # Add code context if available
        if code_context:
            target_line = code_context.get('target_line', code_context.get('target_code', 'N/A'))
            file_name = code_context.get('file', 'unknown')
            line_number = code_context.get('line', '?')
            context_block = code_context.get('context', 'N/A')
            
            # SIMPLIFIED, MORE DIRECT PROMPT for better AI adherence
            prompt += f"""

========================================
🎯 YOUR TASK: Fix THIS specific line of code
========================================

File: {file_name}, Line {line_number}

The problematic code is:
{target_line}

Context (surrounding code):
{context_block}

INSTRUCTIONS:
1. Look at the code above: {target_line}
2. Generate a fix for THIS EXACT code (not a generic example)
3. Your "example" field MUST contain: {target_line}
4. Show: "// Before:\\n{target_line}\\n\\n// After:\\n[fixed version]"

⚠️ CRITICAL: The "Before:" section MUST show: {target_line}
⚠️ DO NOT use placeholders like "myEnum" or "value"
⚠️ USE the actual identifier: {target_line.split()[0] if target_line else 'from code above'}
"""
        
        prompt += """

RESPONSE FORMAT - Provide ONLY valid JSON:

❌ WRONG EXAMPLE (generic placeholders):
{
  "type": "cast",
  "description": "Add explicit cast...",
  "example": "// Before:\\nuint8_t value = myEnum;\\n\\n// After:\\nuint8_t value = (uint8_t)myEnum;",
  "priority": "MEDIUM"
}

✅ CORRECT EXAMPLE (using actual code from above):
{
  "type": "cast",
  "description": "Add explicit cast to MKA_CRYPTO_PROCESSING_SYNC to convert enum to unsigned type...",
  "example": "// Before:\\nMKA_CRYPTO_PROCESSING_SYNC,\\n\\n// After:\\n(uint32_t)MKA_CRYPTO_PROCESSING_SYNC,",
  "priority": "MEDIUM"
}

REQUIREMENTS:
- "type": specific fix type (cast/check/refactor/etc)
- "description": minimum 20 words explaining the fix
- "example": MUST contain the actual code line I showed you
- "priority": HIGH/MEDIUM/LOW
- NO generic names: myEnum, value, ptr, variable, etc.
- USE actual names from the code

Generate your JSON response now (MUST use the actual code from above):"""
        
        return prompt
    
    def _parse_response(self, response: str, violation: Dict) -> Optional[Dict]:
        """
        Parse Ollama response into fix suggestion
        
        Args:
            response: Raw response from Ollama
            violation: Original violation dict
        
        Returns:
            Parsed fix suggestion or None
        """
        try:
            # Remove markdown code blocks if present
            response = response.replace('```json', '').replace('```', '')
            
            # Try to extract JSON from response
            start = response.find('{')
            end = response.rfind('}') + 1
            
            if start >= 0 and end > start:
                json_str = response[start:end]
                
                # Clean common JSON formatting issues
                json_str = self._clean_json(json_str)
                
                try:
                    fix_data = json.loads(json_str)
                    
                    # Validate required fields (priority is optional - AI often forgets it)
                    required = ['type', 'description']
                    if all(k in fix_data for k in required):
                        # Add default priority if missing
                        if 'priority' not in fix_data:
                            fix_data['priority'] = 'MEDIUM'
                            logger.debug("Added default priority: MEDIUM (AI forgot to include it)")
                        # Validate content is not placeholder text
                        if self._is_placeholder_content(fix_data):
                            logger.warning(f"❌ Rejected placeholder content in JSON response for {violation.get('violation_id')}")
                            logger.warning(f"   Description: {fix_data.get('description', '')[:150]}...")
                            logger.warning(f"   Example preview: {fix_data.get('example', '')[:150]}...")
                            logger.warning(f"   Example length: {len(fix_data.get('example', ''))} chars")
                            logger.warning(f"   Type: {fix_data.get('type', '')}")
                            logger.info(f"   Falling back to text extraction or default generation")
                            logger.info(f"   💡 TIP: Check _is_placeholder_content() validation - may be too strict")
                        else:
                            # ADDITIONAL CHECK: If code context was provided, verify the example uses actual code
                            code_context = violation.get('code_context')
                            if code_context:
                                target_line = code_context.get('target_line', '').strip()
                                example_text = fix_data.get('example', '').lower()
                                
                                # Extract key identifiers from target line (variable/function names)
                                # Simple heuristic: look for identifiers that are at least 3 chars long
                                import re
                                identifiers = re.findall(r'\b[a-zA-Z_][a-zA-Z0-9_]{2,}\b', target_line)
                                
                                if identifiers:
                                    # Check if at least one identifier from the actual code appears in the example
                                    found_actual_code = any(ident.lower() in example_text for ident in identifiers[:3])
                                    
                                    if not found_actual_code:
                                        logger.warning(f"❌ AI provided generic example instead of using actual code!")
                                        logger.warning(f"   Expected identifiers from code: {identifiers[:3]}")
                                        logger.warning(f"   But example doesn't contain them: {fix_data.get('example', '')[:200]}")
                                        logger.warning(f"   This is a GENERIC EXAMPLE, not a SPECIFIC FIX")
                                        # Don't reject - just warn and let it through with warning
                                        fix_data['warning'] = '⚠️ AI generated generic example. May need manual adaptation.'
                                    else:
                                        logger.info(f"   ✅ Example uses actual code identifiers: {[i for i in identifiers[:3] if i.lower() in example_text]}")
                                else:
                                    logger.warning(f"   ⚠️ No clear identifiers found in target line to validate")
                            
                            # POST-PROCESSING: ALWAYS check if we can improve the example with actual code
                            # (Not just when there's a warning - be proactive)
                            if code_context:
                                target_line = code_context.get('target_line', '').strip()
                                if target_line:
                                    example = fix_data.get('example', '')
                                    
                                    # Check if example contains actual code
                                    import re
                                    before_after_match = re.search(r'// Before:\s*\n(.+?)\s*\n\s*// After:\s*\n(.+?)(?:\n|$)', example, re.DOTALL)
                                    
                                    if before_after_match:
                                        generic_before = before_after_match.group(1).strip()
                                        generic_after = before_after_match.group(2).strip()
                                        
                                        # Check if the "Before" uses actual code or generic placeholder
                                        # Extract key identifier from target_line
                                        target_identifiers = re.findall(r'\b[A-Z_][A-Z0-9_]{3,}\b', target_line)  # Uppercase constants/enums
                                        
                                        uses_actual_code = False
                                        if target_identifiers:
                                            # Check if any identifier from actual code appears in example
                                            uses_actual_code = any(ident in generic_before for ident in target_identifiers[:2])
                                        
                                        if not uses_actual_code:
                                            logger.warning(f"   🔧 AI used generic example - replacing with actual code")
                                            logger.info(f"   Generic before: {generic_before}")
                                            logger.info(f"   Actual code: {target_line}")
                                            
                                            # Extract the fix pattern from generic_after
                                            fix_type = fix_data.get('type', '')
                                            actual_after = target_line  # Default: no change
                                            
                                            if 'cast' in fix_type.lower() or 'cast' in fix_data.get('description', '').lower():
                                                # Extract cast type from generic example
                                                cast_match = re.search(r'\(([a-zA-Z_]\w*)\)', generic_after)
                                                cast_type = None
                                                
                                                if cast_match:
                                                    cast_type = cast_match.group(1)
                                                    logger.info(f"   Found cast type in example: {cast_type}")
                                                
                                                # If cast type seems wrong, try to infer from description or context
                                                if not cast_type or cast_type in ['myEnum', 'value', 'ptr']:
                                                    # Try to infer from description
                                                    desc = fix_data.get('description', '')
                                                    type_match = re.search(r'(uint32_t|uint16_t|uint8_t|int32_t|int16_t|int8_t|size_t)', desc, re.IGNORECASE)
                                                    if type_match:
                                                        cast_type = type_match.group(1).lower()
                                                        logger.info(f"   Inferred cast type from description: {cast_type}")
                                                
                                                # Apply cast to actual code
                                                if cast_type:
                                                    actual_after = f"({cast_type}){target_line}"
                                                    logger.info(f"   Applied cast: {actual_after}")
                                                else:
                                                    # Default fallback
                                                    actual_after = f"(uint32_t){target_line}"
                                                    logger.info(f"   Using default cast: {actual_after}")
                                            
                                            elif 'check' in fix_type.lower() or 'null' in fix_type.lower():
                                                # Add null/bounds check
                                                actual_after = target_line + "\nif (" + target_line.split()[0] + " != NULL) { ... }"
                                            
                                            # Reconstruct example with actual code
                                            fix_data['example'] = f"// Before:\n{target_line}\n\n// After:\n{actual_after}"
                                            logger.info(f"   ✅ Fixed example now uses actual code")
                                            logger.info(f"   New example:\n{fix_data['example']}")
                                            
                                            # Remove generic warning if it exists
                                            fix_data.pop('warning', None)
                                        else:
                                            logger.info(f"   ✅ Example already uses actual code identifiers")
                            
                            # Add AI metadata (after post-processing)
                            fix_data['ai_generated'] = True
                            fix_data['model'] = self.model
                            fix_data['violation_id'] = violation.get('violation_id')
                            fix_data['category'] = violation.get('category')
                            
                            # Final summary after post-processing
                            logger.info(f"✅ FINAL fix ready for {violation.get('violation_id')}")
                            logger.info(f"   💻 Final Example AFTER post-processing:")
                            logger.info(f"{fix_data.get('example', '')[:350]}")
                            
                            return fix_data  # Return AFTER post-processing is complete
                    else:
                        logger.debug(f"JSON missing required fields. Found keys: {list(fix_data.keys())}")
                except json.JSONDecodeError as e:
                    logger.debug(f"JSON parse failed: {str(e)}")
                    logger.debug(f"Attempted JSON: {json_str[:200]}...")
            
            # If JSON parsing fails, try to extract key information
            logger.warning("❌ Could not parse JSON from AI response, attempting text extraction")
            logger.debug(f"   Full response was: {response[:500]}...")
            return self._extract_from_text(response, violation)
            
        except Exception as e:
            logger.error(f"Response parsing error: {str(e)}")
            return self._extract_from_text(response, violation)
    
    def _clean_json(self, json_str: str) -> str:
        """
        Clean common JSON formatting issues from LLM responses
        
        Args:
            json_str: Raw JSON string
        
        Returns:
            Cleaned JSON string
        """
        import re
        
        # Remove leading/trailing whitespace
        json_str = json_str.strip()
        
        # Remove trailing commas before } or ]
        json_str = re.sub(r',\s*}', '}', json_str)
        json_str = re.sub(r',\s*]', ']', json_str)
        
        # Fix common issues with unescaped characters in strings
        # Replace literal newlines within quoted strings with \n
        def escape_newlines_in_strings(match):
            content = match.group(1)
            content = content.replace('\n', '\\n')
            content = content.replace('\r', '\\r')
            content = content.replace('\t', '\\t')
            return f'"{content}"'
        
        # Match quoted strings and escape special chars
        try:
            # This regex finds content between quotes
            json_str = re.sub(r'"([^"\\]*(?:\\.[^"\\]*)*)"', escape_newlines_in_strings, json_str)
        except:
            # If regex fails, continue with original
            pass
        
        # Remove comments if present (shouldn't be in JSON but LLMs sometimes add them)
        json_str = re.sub(r'//.*?$', '', json_str, flags=re.MULTILINE)
        
        return json_str
    
    def _is_placeholder_content(self, fix_data: Dict) -> bool:
        """
        Check if fix data contains placeholder/template text instead of real content
        
        Args:
            fix_data: Parsed fix data dictionary
        
        Returns:
            True if content appears to be placeholder text
        """
        logger.debug(f"🔍 Validating fix content...")
        
        placeholder_phrases = [
            'brief description',
            'brief fix description',
            'fix description',
            'description of the fix',
            'before and after',
            'before and after code',
            'fix_type',
            'your fix here',
            'add fix here',
            'example code',
            'sample code',
            'code example here'
        ]
        
        # Check description field
        description = fix_data.get('description', '').lower()
        if any(phrase in description for phrase in placeholder_phrases):
            logger.debug(f"❌ Description has placeholder phrase")
            return True
        
        # Check if description contains JSON structure artifacts
        json_artifacts = ['"description":', '"type":', '"priority":', '"example":']
        if any(artifact in description for artifact in json_artifacts):
            logger.debug("Description contains JSON structure artifacts")
            return True
        
        # Check if description starts with a quote (likely extracted JSON value incorrectly)
        desc_raw = fix_data.get('description', '')
        if desc_raw.startswith(('"', "'")):
            logger.debug("Description starts with quote, likely malformed")
            return True
        
        # Check if description is too short (likely placeholder)
        if len(description.strip()) < 10:  # Reduced from 15 to 10
            logger.debug(f"❌ Description too short: {len(description.strip())} chars (min 10)")
            return True
        
        # Check example field if present
        example = fix_data.get('example', '').lower()
        if example and any(phrase in example for phrase in placeholder_phrases):
            logger.debug(f"❌ Example has placeholder phrase")
            return True
        
        # Check if example contains JSON structure (not actual code)
        json_structure_markers = ['"priority":', '"type":', '"description":', '"}', '",', '}\n}']
        if any(marker in example for marker in json_structure_markers):
            logger.debug("❌ Example contains JSON structure instead of code")
            return True
        
        # Check if example is just closing braces or brackets
        example_raw = fix_data.get('example', '').strip()
        if example_raw in ['}', '},', ']', '],', '"}', '"},']:
            logger.debug("❌ Example is just JSON closing syntax")
            return True
        
        # Check if example is missing or too short
        if not example or len(example.strip()) < 15:  # Reduced from 20 to 15
            logger.debug(f"❌ Example too short or missing: {len(example.strip()) if example else 0} chars (min 15)")
            return True
        
        # Check if example contains actual code markers
        if example:
            has_before = 'before' in example
            has_after = 'after' in example
            has_code_chars = any(char in example for char in [';', '=', '(', ')', '{', '}'])
            
            # If it claims to be a code example but has no code syntax at all, it's likely bad
            # But be lenient - don't require both before/after if there's actual code
            if not has_code_chars:
                logger.debug("Example has no code syntax characters")
                return True
            
            # If it has code chars but no before/after structure, just warn (don't reject)
            if has_code_chars and not (has_before or has_after):
                logger.debug("Example has code but no before/after markers (may still be valid)")
                # Don't return True here - let it through if it has code
        
        # Check if type is generic placeholder
        fix_type = fix_data.get('type', '').lower()
        if fix_type in ['fix_type', 'type', 'fix', '']:
            logger.debug(f"❌ Type is generic placeholder: '{fix_type}'")
            return True
        
        logger.debug(f"✅ Content validation PASSED")
        return False
    
    def _extract_from_text(self, text: str, violation: Dict) -> Optional[Dict]:
        """
        Fallback: Extract fix info from free-form text
        
        Args:
            text: Response text
            violation: Violation dict
        
        Returns:
            Basic fix suggestion or None
        """
        import re
        
        # Get violation info for more specific suggestions
        violation_id = violation.get('violation_id', 'UNKNOWN')
        violation_text = violation.get('violation_text', '')
        category = violation.get('category', 'OTHER')
        
        logger.info(f"⚠️  Extracting from text for {violation_id} (JSON parse failed)")
        
        # Simple extraction for common patterns
        lines = text.strip().split('\n')
        text_lower = text.lower()
        
        # Try to extract structured info
        fix_type = self._infer_fix_type(violation_id, violation_text)
        description = ''
        example = ''
        priority = 'MEDIUM'
        
        # Extract description (first meaningful paragraph that's not JSON or placeholder)
        placeholder_phrases = ['brief description', 'fix description', 'description of']
        json_keys = ['"description":', '"type":', '"example":', '"priority":', '"fix_type":']
        
        for line in lines:
            line = line.strip()
            # Skip JSON structure lines, braces, empty lines
            if not line or line.startswith('{') or line.startswith('}') or line.startswith('[') or line.startswith(']'):
                continue
            
            # If line contains JSON key like "description": "value", extract just the value
            is_json_line = False
            for key in json_keys:
                if key in line:
                    is_json_line = True
                    # Extract value part after the key
                    try:
                        # Find the key and extract everything after it
                        key_index = line.find(key)
                        value_part = line[key_index + len(key):].strip()
                        # Remove leading/trailing quotes and commas
                        value_part = value_part.strip('"').strip("'").strip(',').strip()
                        if len(value_part) > 25 and not any(ph in value_part.lower() for ph in placeholder_phrases):
                            description = value_part[:400]
                            break
                    except:
                        pass
            
            if description:
                break
            
            # If not a JSON line, check if it's a regular text description
            if not is_json_line:
                line_lower = line.lower()
                if len(line) > 25 and not any(ph in line_lower for ph in placeholder_phrases):
                    description = line[:400]
                    break
        
        # Clean up description - remove trailing JSON artifacts
        if description:
            # Remove trailing quotes, commas, braces
            description = description.rstrip('",}]').strip()
        
        # If no good description found, generate one based on violation
        if not description or len(description) < 25:
            orig_desc_len = len(description) if description else 0
            description = self._generate_default_description(violation_id, violation_text, category)
            logger.info(f"   📝 Generated default description (extraction found only {orig_desc_len} chars)")
        
        # Try to find code examples
        code_section = []
        in_code = False
        for line in lines:
            # Skip JSON structure lines
            if any(key in line for key in ['"description":', '"type":', '"priority":', '"example":']):
                continue
            # Skip lines that are just closing braces/brackets
            if line.strip() in ['}', '},', ']', '],', '{', '[']:
                continue
            
            if '```' in line or 'Before:' in line or 'After:' in line or '// Before' in line or '// After' in line:
                in_code = True
                continue  # Skip the marker line
            elif in_code:
                if line.strip():
                    if '```' in line:  # End of code block
                        in_code = False
                    elif not any(skip in line.lower() for skip in ['before:', 'after:', 'example:']):
                        code_section.append(line)
        
        if code_section:
            example = '\n'.join(code_section[:20])  # Limit to 20 lines
        else:
            # Look for inline code (anything with common C syntax)
            code_pattern = re.compile(r'[a-zA-Z_]\w*\s*\([^)]*\)|[a-zA-Z_]\w*\s*=|if\s*\(|for\s*\(|while\s*\(')
            code_lines = [
                line for line in lines 
                if code_pattern.search(line) 
                and not line.strip().startswith(('{', '}', '"', '[', ']'))
                and len(line.strip()) > 5
            ]
            if code_lines:
                example = '\n'.join(code_lines[:10])
        
        # If still no example, generate a default one
        if not example or len(example.strip()) < 10:
            orig_example_len = len(example) if example else 0
            example = self._generate_default_example(violation_id, violation_text, fix_type)
            logger.info(f"   💡 Generated default example (extraction found only {orig_example_len} chars)")
        
        # Determine priority from keywords
        if any(word in text_lower for word in ['critical', 'security', 'vulnerability', 'exploit']):
            priority = 'HIGH'
        elif any(word in text_lower for word in ['minor', 'style', 'cosmetic']):
            priority = 'LOW'
        
        fix_data = {
            'type': fix_type,
            'description': description if description else 'AI-generated fix suggestion',
            'example': example if example else 'See full response for details',
            'priority': priority,
            'ai_generated': True,
            'model': self.model,
            'violation_id': violation.get('violation_id'),
            'category': violation.get('category'),
            'note': 'Extracted from free-form AI response',
            'raw_response': text[:500]  # Keep first 500 chars for reference
        }
        
        logger.info(f"   ✅ Text extraction completed - Type: {fix_type}, Desc len: {len(description)}, Example len: {len(example)}")
        
        return fix_data
    
    def _infer_fix_type(self, violation_id: str, violation_text: str) -> str:
        """
        Infer fix type from violation ID and text
        
        Args:
            violation_id: Violation ID
            violation_text: Violation description
        
        Returns:
            Fix type string
        """
        text_lower = violation_text.lower()
        violation_id_lower = violation_id.lower()
        
        # Specific MISRA/CERT rules
        if 'rule_20_1' in violation_id_lower or ('#include' in text_lower and 'precede' in text_lower):
            return 'refactor'
        
        if 'rule_5_4' in violation_id_lower or ('macro' in text_lower and 'name' in text_lower):
            return 'refactor'
        
        # Type conversion issues (INT31, RULE_10_3)
        if any(kw in text_lower for kw in ['cast', 'conversion', 'implicit', 'explicit', 'essentially']):
            return 'cast'
        
        # Boundary/bounds issues
        if any(kw in text_lower for kw in ['bounds', 'buffer', 'overflow', 'array', 'index']):
            return 'bounds_check'
        
        # Null/validation issues
        if any(kw in text_lower for kw in ['null', 'nullptr', 'validate', 'check', 'pointer']):
            return 'validation'
        
        # Initialization issues
        if any(kw in text_lower for kw in ['initialize', 'uninitialized', 'init']):
            return 'initialization'
        
        # Declaration issues
        if any(kw in text_lower for kw in ['declare', 'declaration', 'unused']):
            return 'declaration'
        
        # Default
        return 'refactor'
    
    def _generate_default_description(self, violation_id: str, violation_text: str, category: str) -> str:
        """
        Generate a meaningful default description when AI fails
        
        Args:
            violation_id: Violation ID
            violation_text: Violation description
            category: Violation category
        
        Returns:
            Default description string
        """
        # Extract key info
        text_lower = violation_text.lower()
        
        # MISRA RULE_20_1: #include directive placement
        if 'rule_20_1' in violation_id.lower() or ('#include' in text_lower and 'precede' in text_lower):
            return "Move #include directives to the top of the file before any executable code. All #include statements must appear before function definitions or variable declarations to comply with MISRA C standards."
        
        # MISRA RULE_5_4: Macro name length/uniqueness
        if 'rule_5_4' in violation_id.lower() or ('macro' in text_lower and 'same name' in text_lower):
            return "Rename macro to ensure uniqueness within the first 31/63 characters. Use distinct macro names to avoid identifier collision issues required by MISRA C Rule 5.4."
        
        # Type conversion (INT31, RULE_10_3)
        if 'conversion' in text_lower or 'cast' in text_lower or 'essentially' in text_lower:
            if 'enum' in text_lower and 'unsigned' in text_lower:
                return "Add explicit type cast when assigning enum values to unsigned integer types. Use (uint8_t) or appropriate unsigned cast to ensure type safety and comply with MISRA C Rule 10.3 / CERT INT31-C."
            elif 'signed' in text_lower and 'unsigned' in text_lower:
                return "Add explicit cast when converting between signed and unsigned types. Use appropriate type cast to prevent implicit conversion warnings and ensure predictable behavior."
            else:
                return "Add explicit type cast to resolve implicit conversion. Use proper casting to ensure type safety and prevent implicit conversion issues per coding standards."
        
        # Bounds checking
        if 'bounds' in text_lower or 'buffer' in text_lower or 'overflow' in text_lower:
            return "Add bounds checking before array access. Verify array indices are within valid range to prevent buffer overflow vulnerabilities."
        
        # Null checks  
        if 'null' in text_lower or 'pointer' in text_lower:
            return "Add null pointer validation before dereferencing. Check pointer is not NULL to prevent crashes and undefined behavior."
        
        # Initialization
        if 'initialize' in text_lower or 'uninitialized' in text_lower:
            return "Initialize variable at declaration. Ensure all variables have defined values before being read to prevent undefined behavior."
        
        # Memory section pragmas
        if 'mem' in text_lower and 'section' in text_lower:
            return "Verify memory section pragma usage. Ensure linker section directives are properly paired and use unique identifiers."
        
        # Unused variables
        if 'unused' in text_lower:
            return "Remove unused variable or explicitly mark with (void) cast. Clean up dead code or indicate intentional non-use to suppress warnings."
        
        # Generic fallback - use actual violation text
        return f"Review and address {violation_id}: {violation_text[:150]}... Consult coding standard documentation for specific requirements and apply appropriate fix."
    
    def _generate_default_example(self, violation_id: str, violation_text: str, fix_type: str) -> str:
        """
        Generate a meaningful default code example when AI fails to provide one
        
        Args:
            violation_id: Violation ID
            violation_text: Violation description
            fix_type: Inferred fix type
        
        Returns:
            Code example string
        """
        text_lower = violation_text.lower()
        violation_id_lower = violation_id.lower()
        
        # MISRA RULE_20_1: #include directive placement
        if 'rule_20_1' in violation_id_lower or ('#include' in text_lower and 'precede' in text_lower):
            return """// Before:
void someFunction(void) {
    // function code
}
#include "header.h"  /* Violation: #include after code */

// After:
#include "header.h"  /* Move to top of file */

void someFunction(void) {
    // function code
}"""
        
        # MISRA RULE_5_4: Macro name uniqueness
        if 'rule_5_4' in violation_id_lower or ('macro' in text_lower and 'same name' in text_lower):
            # Extract macro name from violation text if possible
            import re
            macro_match = re.search(r"'([A-Z_][A-Z0-9_]*)'", violation_text)
            macro_name = macro_match.group(1) if macro_match else "MACRO_NAME"
            
            return f"""// Before:
#define {macro_name} value1
// ... later in file ...
#define {macro_name} value2  /* Duplicate name */

// After:
#define {macro_name}_FIRST value1
// ... later in file ...
#define {macro_name}_SECOND value2  /* Unique identifier */"""
        
        # Type conversion/cast examples (INT31-C, RULE_10_3)
        if fix_type == 'cast' or 'conversion' in text_lower or 'cast' in text_lower or 'essentially' in text_lower:
            if 'enum' in text_lower and 'unsigned' in text_lower:
                return """// Before:
uint8_t value = myEnum;

// After:
uint8_t value = (uint8_t)myEnum;"""
            elif 'signed' in text_lower and 'unsigned' in text_lower:
                return """// Before:
uint16_t result = signedValue;

// After:
uint16_t result = (uint16_t)signedValue;"""
            elif 'char' in text_lower:
                return """// Before:
int result = charValue;

// After:
int result = (int)charValue;"""
            else:
                return """// Before:
targetType result = sourceValue;

// After:
targetType result = (targetType)sourceValue;"""
        
        # Bounds checking examples
        elif fix_type == 'bounds_check' or 'bounds' in text_lower or 'buffer' in text_lower:
            return """// Before:
result = array[index];

// After:
if (index < ARRAY_SIZE) {
    result = array[index];
} else {
    /* Handle out-of-bounds case */
    result = 0;
}"""
        
        # Validation/null check examples
        elif fix_type == 'validation' or 'null' in text_lower or 'pointer' in text_lower:
            return """// Before:
*pointer = value;

// After:
if (pointer != NULL) {
    *pointer = value;
}"""
        
        # Initialization examples
        elif fix_type == 'initialization' or 'initialize' in text_lower:
            return """// Before:
uint8_t variable;
result = variable + 10;  /* Using uninitialized */

// After:
uint8_t variable = 0U;  /* Initialize at declaration */
result = variable + 10;"""
        
        # Declaration/unused variable examples
        elif fix_type == 'declaration' or 'unused' in text_lower:
            return """// Before:
static int unusedVar = 0;

// After:
static int unusedVar = 0;
(void)unusedVar;  /* Explicitly mark as intentionally unused */

// Or remove if truly unnecessary:
/* Removed: static int unusedVar = 0; */"""
        
        # Memory section pragmas
        elif 'mem' in text_lower and 'section' in text_lower:
            return """// Before:
#pragma section ".data"
/* code */
#pragma section ".data"  /* Duplicate */

// After:
#pragma section ".data"
/* code */
#pragma section  /* Close section properly */"""
        
        # Generic refactor - make it more specific based on violation text
        else:
            # Try to extract specific guidance from violation text
            if 'must' in text_lower or 'shall' in text_lower or 'should' in text_lower:
                # Extract the requirement for a more specific example
                return f"""// Before:
/* Code violating {violation_id} */
/* {violation_text[:80]}... */

// After:
/* Refactored code complying with {violation_id} */
/* Apply fix according to rule requirements */
/* Consult documentation: {violation_id} */"""
            else:
                return """// Before:
/* Original code with violation */

// After:
/* Refactored code addressing the violation */
/* Review specific requirements in rule documentation */"""
    
    def _load_justification_examples(self) -> None:
        """
        Load justification examples from existing suppression comment files
        for few-shot learning
        """
        try:
            if not self.examples_dir.exists():
                logger.warning(f"[WARNING] Justifications directory not found: {self.examples_dir}")
                return
            
            # Find all suppress comment files
            suppress_files = list(self.examples_dir.glob('*_suppress_comments_*.txt'))
            
            if not suppress_files:
                logger.info("[INFO] No justification example files found for few-shot learning")
                return
            
            # Load examples from up to 3 different files for diversity
            sampled_files = suppress_files[:3] if len(suppress_files) >= 3 else suppress_files
            
            for file_path in sampled_files:
                examples = self._parse_justification_examples(file_path)
                self.justification_examples.extend(examples)
            
            # Limit to 10 most diverse examples
            if len(self.justification_examples) > 10:
                self.justification_examples = self._select_diverse_examples(self.justification_examples, 10)
            
            logger.info(f"[OK] Loaded {len(self.justification_examples)} justification examples for few-shot learning")
            
        except Exception as e:
            logger.error(f"[ERROR] Failed to load justification examples: {str(e)}")
            self.justification_examples = []
    
    def _parse_justification_examples(self, file_path: Path) -> list:
        """
        Parse justification examples from a suppress comments file
        
        Args:
            file_path: Path to suppress comments file
        
        Returns:
            List of example dictionaries
        """
        examples = []
        
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            # Extract module name from filename (e.g., 'Mka_suppress_comments_...' -> 'Mka')
            module_name = file_path.stem.split('_suppress_comments')[0]
            
            # Find all suppression entries using regex
            import re
            pattern = r'/\*\s*parasoft-begin-suppress\s+([A-Z0-9_\-\s]+?)\s+"Reason:\s*(.+?)"\s*\*/'
            matches = re.findall(pattern, content)
            
            # Sample up to 3 examples from this file
            for match in matches[:3]:
                rule_ids = match[0].strip()
                reason = match[1].strip()
                
                examples.append({
                    'module': module_name,
                    'rules': rule_ids,
                    'reason': reason,
                    'format': f'/* parasoft-begin-suppress {rule_ids} "Reason: {reason}" */'
                })
        
        except Exception as e:
            logger.debug(f"Failed to parse examples from {file_path.name}: {str(e)}")
        
        return examples
    
    def _select_diverse_examples(self, examples: list, max_count: int) -> list:
        """Select diverse examples covering different rule types"""
        # Group by rule category (CERT, MISRA, etc.)
        cert_examples = [ex for ex in examples if 'CERT' in ex['rules']]
        misra_examples = [ex for ex in examples if 'MISRA' in ex['rules']]
        other_examples = [ex for ex in examples if 'CERT' not in ex['rules'] and 'MISRA' not in ex['rules']]
        
        # Select proportionally
        selected = []
        
        # Try to get balanced representation
        cert_quota = min(len(cert_examples), max_count // 3)
        misra_quota = min(len(misra_examples), max_count // 3)
        other_quota = max_count - cert_quota - misra_quota
        
        selected.extend(cert_examples[:cert_quota])
        selected.extend(misra_examples[:misra_quota])
        selected.extend(other_examples[:other_quota])
        
        return selected[:max_count]
    
    def _build_examples_section(self, violation_id: str, category: str) -> str:
        """
        Build few-shot examples section for the prompt
        
        Args:
            violation_id: Current violation ID
            category: Current violation category
        
        Returns:
            Formatted examples section string
        """
        if not self.justification_examples or not self.use_few_shot:
            return ""
        
        # Filter examples relevant to current violation category
        relevant_examples = []
        for ex in self.justification_examples:
            # Prioritize examples matching the category
            if category in ex['rules']:
                relevant_examples.append(ex)
        
        # If no category matches, use general examples
        if not relevant_examples:
            relevant_examples = self.justification_examples[:5]
        else:
            relevant_examples = relevant_examples[:5]
        
        if not relevant_examples:
            return ""
        
        examples_text = "\n\nEXAMPLE JUSTIFICATIONS FROM OTHER MODULES:\n"
        examples_text += "Learn from these examples - notice the format and how reasons are documented:\n\n"
        
        for i, ex in enumerate(relevant_examples, 1):
            examples_text += f"Example {i} (Module: {ex['module']}):\n"
            examples_text += f"  Rule(s): {ex['rules']}\n"
            examples_text += f"  Format: {ex['format']}\n"
            examples_text += f"  Reason Pattern: {ex['reason'][:100]}...\n\n"
        
        return examples_text
    
    def suggest_justification(self, violation: Dict, cross_module_info: list) -> Optional[Dict]:
        """
        Use AI to analyze whether a violation should be justified instead of fixed
        
        Args:
            violation: Violation dictionary
            cross_module_info: List of strings describing how other modules handled this violation
        
        Returns:
            Justification suggestion dict with 'should_justify', 'confidence', 'reason', 'suggested_rationale'
        """
        if not self.enabled:
            return None
        
        try:
            violation_id = violation.get('violation_id', 'UNKNOWN')
            violation_text = violation.get('violation_text', '')
            category = violation.get('category', 'OTHER')
            severity = violation.get('severity', 'MEDIUM')
            
            logger.info(f"[AI] Analyzing justification recommendation for {violation_id}...")
            
            # Build prompt for justification analysis
            prompt = self._build_justification_prompt(
                violation_id, violation_text, category, severity, cross_module_info
            )
            
            # Call Ollama
            response = ollama.generate(
                model=self.model,
                prompt=prompt,
                options={
                    'temperature': 0.2,  # Lower temperature for more consistent analysis
                    'num_predict': 500,  # Shorter response needed
                }
            )
            
            # Extract response text
            if hasattr(response, 'response'):
                response_text = response.response
            elif isinstance(response, dict):
                response_text = response.get('response', '')
            else:
                logger.error(f"[ERROR] Unexpected response type: {type(response)}")
                return None
            
            logger.debug(f"   AI justification analysis: {response_text[:200]}...")
            
            # Parse the response
            result = self._parse_justification_analysis(response_text, violation)
            
            if result:
                logger.info(f"[OK] Justification analysis: {'JUSTIFY' if result['should_justify'] else 'FIX'} (confidence: {result['confidence']})")
                return result
            else:
                logger.warning(f"[WARNING] Failed to parse justification analysis")
                return None
            
        except Exception as e:
            logger.error(f"[ERROR] Justification analysis failed: {str(e)}")
            return None
    
    def _build_justification_prompt(self, violation_id: str, violation_text: str,
                                   category: str, severity: str, cross_module_info: list) -> str:
        """
        Build prompt for AI justification analysis with few-shot learning examples
        
        Args:
            violation_id: Violation ID
            violation_text: Violation description
            category: Violation category
            severity: Severity level
            cross_module_info: How other modules handled this violation
        
        Returns:
            Prompt string
        """
        cross_module_context = ""
        if cross_module_info:
            cross_module_context = "\n\nCROSS-MODULE ANALYSIS:\n"
            for info in cross_module_info:
                cross_module_context += f"  - {info}\n"
        
        # Build few-shot examples section
        examples_section = self._build_examples_section(violation_id, category)
        
        prompt = f"""You are a code quality analyst helping decide whether a coding standard violation should be JUSTIFIED (suppressed) or FIXED.

VIOLATION DETAILS:
ID: {violation_id}
Category: {category}
Severity: {severity}
Description: {violation_text}
{cross_module_context}
{examples_section}

ANALYSIS CRITERIA:
1. If OTHER MODULES have already justified/suppressed this same violation → likely a common deviation → SUGGEST JUSTIFICATION
2. If violation is due to legacy code, third-party libraries, or platform constraints → SUGGEST JUSTIFICATION
3. If violation is design decision (e.g., performance, architecture) → SUGGEST JUSTIFICATION
4. If violation is legitimate code quality issue that should be fixed → SUGGEST FIX

STANDARD JUSTIFICATION FORMAT:
The justification should follow the pattern shown in the examples above:
- Use parasoft-begin-suppress and parasoft-end-suppress comments
- Include rule ID(s) being suppressed
- Provide a meaningful reason that explains WHY the violation is justified
- Reference specific constraints, design decisions, or cross-module consistency

REQUIRED OUTPUT FORMAT (JSON):
{{
  "should_justify": true/false,
  "confidence": "HIGH/MEDIUM/LOW",
  "reason": "Brief explanation (max 100 chars)",
  "suggested_rationale": "Meaningful justification following the format in examples (max 150 chars)"
}}

IMPORTANT:
- Learn from the examples above - use descriptive, meaningful rationales
- If multiple modules justified this → should_justify = true
- If it's a genuine bug or security issue → should_justify = false
- Make suggested_rationale specific to the violation context
- Focus on whether this is a COMMON DEVIATION that belongs in deviation Excel

Analyze and respond ONLY with the JSON object:"""
        
        return prompt
    
    def _parse_justification_analysis(self, response: str, violation: Dict) -> Optional[Dict]:
        """
        Parse AI justification analysis response
        
        Args:
            response: AI response text
            violation: Original violation dict
        
        Returns:
            Parsed analysis dict or None
        """
        try:
            # Remove markdown code blocks
            response = response.replace('```json', '').replace('```', '').strip()
            
            # Extract JSON
            start = response.find('{')
            end = response.rfind('}') + 1
            
            if start >= 0 and end > start:
                json_str = response[start:end]
                analysis = json.loads(json_str)
                
                # Validate required fields
                required = ['should_justify', 'confidence', 'reason']
                if all(k in analysis for k in required):
                    # Ensure boolean
                    analysis['should_justify'] = bool(analysis['should_justify'])
                    
                    # Add violation context
                    analysis['violation_id'] = violation.get('violation_id')
                    analysis['category'] = violation.get('category')
                    
                    # If suggested_rationale missing, generate default
                    if not analysis.get('suggested_rationale') and analysis['should_justify']:
                        analysis['suggested_rationale'] = "Common deviation - consistent with other modules"
                    
                    return analysis
                else:
                    logger.debug(f"Missing required fields in justification analysis. Found: {list(analysis.keys())}")
            
            # Fallback: parse from text
            response_lower = response.lower()
            should_justify = any(word in response_lower for word in ['justify', 'suppress', 'deviation', 'common'])
            
            return {
                'should_justify': should_justify,
                'confidence': 'LOW',
                'reason': 'Inferred from text analysis',
                'suggested_rationale': 'Review cross-module handling - may be common deviation',
                'violation_id': violation.get('violation_id'),
                'category': violation.get('category')
            }
            
        except json.JSONDecodeError as e:
            logger.debug(f"JSON parse failed for justification analysis: {str(e)}")
            return None
        except Exception as e:
            logger.error(f"Failed to parse justification analysis: {str(e)}")
            return None
    
    def get_status(self) -> Dict:
        """Get current status of Ollama integration"""
        return {
            'enabled': self.enabled,
            'provider': self.provider,
            'ai_mode': self.ai_mode if hasattr(self, 'ai_mode') else 'hybrid',
            'model': self.model if hasattr(self, 'model') else 'N/A',
            'base_url': self.base_url if hasattr(self, 'base_url') else 'N/A',
            'ollama_available': OLLAMA_AVAILABLE
        }
