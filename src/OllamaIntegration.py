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
            
            # Check category-based rules
            if 'CERT' in category and self.use_ai_for.get('cert_violations', True):
                return True
            
            if 'MISRA' in category and self.use_ai_for.get('misra_violations', True):
                return True
            
            # Use AI for unknown/complex patterns (default: true in hybrid)
            if self.use_ai_for.get('unknown_patterns', True):
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
            logger.debug("Ollama not enabled, skipping AI generation")
            return None
        
        try:
            violation_id = violation.get('violation_id', 'UNKNOWN')
            violation_text = violation.get('violation_text', '')
            category = violation.get('category', 'OTHER')
            severity = violation.get('severity', 'MEDIUM')
            code_context = violation.get('code_context')
            parasoft_reference = violation.get('parasoft_reference')
            
            # Build prompt with all available context
            prompt = self._build_prompt(violation_id, violation_text, category, severity, 
                                       code_context, parasoft_reference)
            
            logger.info(f"[AI] Generating fix for {violation_id} using {self.model}...")
            logger.debug(f"   Prompt length: {len(prompt)} chars")
            
            # Call Ollama
            response = ollama.generate(
                model=self.model,
                prompt=prompt,
                options={
                    'temperature': self.temperature,
                    'num_predict': self.max_tokens,
                }
            )
            
            # Extract response text - handle both dict and typed objects
            if hasattr(response, 'response'):
                # Typed object with response attribute
                response_text = response.response
            elif isinstance(response, dict):
                # Dictionary response
                response_text = response.get('response', '')
            else:
                logger.error(f"[ERROR] Unexpected response type from Ollama generate: {type(response)}")
                return None
            
            logger.debug(f"   AI response length: {len(response_text)} chars")
            logger.debug(f"   AI response preview: {response_text[:200]}...")
            
            # Parse response
            fix_data = self._parse_response(response_text, violation)
            
            if fix_data:
                logger.info(f"[OK] AI-generated fix for {violation_id}")
                return fix_data
            else:
                logger.warning(f"[WARNING] Failed to parse AI response for {violation_id}")
                return None
            
        except Exception as e:
            logger.error(f"[ERROR] Ollama generation failed: {str(e)}")
            if self.fallback_to_rules:
                logger.info("Falling back to rule-based generation")
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
            prompt += f"""

ACTUAL CODE CONTEXT:
File: {code_context.get('file', 'unknown')}
Line {code_context.get('line', '?')}:
{code_context.get('context', 'N/A')}

Target line with violation:
>>> {code_context.get('target_code', 'N/A')}"""
        
        prompt += """

CRITICAL: You MUST provide a CODE EXAMPLE showing before/after transformation.

Provide ONLY valid JSON with REAL, SPECIFIC content:
{
  "type": "explicit_cast",
  "description": "Add explicit type cast from enum to unsigned type to prevent implicit conversion warning and ensure type safety according to MISRA/CERT standards",
  "example": "// Before:\\nuint8_t value = myEnumValue;\\n\\n// After:\\nuint8_t value = (uint8_t)myEnumValue;",
  "priority": "MEDIUM"
}

MANDATORY REQUIREMENTS:
- type: MUST be specific (cast/check/refactor/declaration/bounds/validation/initialization)
- description: MUST explain what to do (minimum 20 words)
- example: MUST show C/C++ code with "// Before:" and "// After:" sections
- priority: HIGH/MEDIUM/LOW based on severity
- NO placeholder text like "brief description" or "your code here"
- If code context provided, use ACTUAL variable names from the code
- Code example must be valid C/C++ syntax

Generate ACTUAL fix with CODE EXAMPLE now:"""
        
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
                    
                    # Validate required fields
                    required = ['type', 'description', 'priority']
                    if all(k in fix_data for k in required):
                        # Validate content is not placeholder text
                        if self._is_placeholder_content(fix_data):
                            logger.warning(f"❌ Rejected placeholder content in JSON response for {violation.get('violation_id')}")
                            logger.warning(f"   Description: {fix_data.get('description', '')[:100]}...")
                            logger.warning(f"   Example: {fix_data.get('example', '')[:100]}...")
                            logger.info(f"   Falling back to text extraction or default generation")
                        else:
                            # Add AI metadata
                            fix_data['ai_generated'] = True
                            fix_data['model'] = self.model
                            fix_data['violation_id'] = violation.get('violation_id')
                            fix_data['category'] = violation.get('category')
                            
                            logger.info(f"✅ Successfully parsed AI JSON for {violation.get('violation_id')}")
                            logger.debug(f"   Type: {fix_data.get('type')}, Priority: {fix_data.get('priority')}")
                            return fix_data
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
        if len(description.strip()) < 15:
            return True
        
        # Check example field if present
        example = fix_data.get('example', '').lower()
        if example and any(phrase in example for phrase in placeholder_phrases):
            return True
        
        # Check if example contains JSON structure (not actual code)
        json_structure_markers = ['"priority":', '"type":', '"description":', '"}', '",', '}\n}']
        if any(marker in example for marker in json_structure_markers):
            logger.debug("Example contains JSON structure instead of code")
            return True
        
        # Check if example is just closing braces or brackets
        example_raw = fix_data.get('example', '').strip()
        if example_raw in ['}', '},', ']', '],', '"}', '"},']:
            logger.debug("Example is just JSON closing syntax")
            return True
        
        # Check if example is missing or too short
        if not example or len(example.strip()) < 20:
            logger.debug("Example is missing or too short")
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
            return True
        
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
        Build prompt for AI justification analysis
        
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
        
        prompt = f"""You are a code quality analyst helping decide whether a coding standard violation should be JUSTIFIED (suppressed) or FIXED.

VIOLATION DETAILS:
ID: {violation_id}
Category: {category}
Severity: {severity}
Description: {violation_text}
{cross_module_context}

ANALYSIS CRITERIA:
1. If OTHER MODULES have already justified/suppressed this same violation → likely a common deviation → SUGGEST JUSTIFICATION
2. If violation is due to legacy code, third-party libraries, or platform constraints → SUGGEST JUSTIFICATION
3. If violation is design decision (e.g., performance, architecture) → SUGGEST JUSTIFICATION
4. If violation is legitimate code quality issue that should be fixed → SUGGEST FIX

REQUIRED OUTPUT FORMAT (JSON):
{{
  "should_justify": true/false,
  "confidence": "HIGH/MEDIUM/LOW",
  "reason": "Brief explanation (max 100 chars)",
  "suggested_rationale": "Suggested justification text if should_justify=true, max 150 chars"
}}

IMPORTANT:
- If multiple modules justified this → should_justify = true
- If it's a genuine bug or security issue → should_justify = false
- Keep reason and suggested_rationale concise and specific
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
