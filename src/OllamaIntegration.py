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
            
            # Validate response structure
            if not isinstance(response, dict):
                logger.error(f"[ERROR] Unexpected response type from Ollama: {type(response)}")
                self.enabled = False
                return False
            
            # Check if model is available
            models_list = response.get('models', [])
            if not models_list:
                logger.warning(f"[WARNING] No models found in Ollama.")
                logger.warning(f"Run: ollama pull {self.model}")
                self.enabled = False
                return False
            
            # Extract model names with error handling
            available_models = []
            for model in models_list:
                if isinstance(model, dict):
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
                logger.warning(f"Response structure: {response}")
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
            
            # Build minimal prompt for efficiency
            prompt = self._build_prompt(violation_id, violation_text, category, severity)
            
            logger.info(f"[AI] Generating fix for {violation_id} using {self.model}...")
            
            # Call Ollama
            response = ollama.generate(
                model=self.model,
                prompt=prompt,
                options={
                    'temperature': self.temperature,
                    'num_predict': self.max_tokens,
                }
            )
            
            # Parse response
            fix_data = self._parse_response(response.get('response', ''), violation)
            
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
                     category: str, severity: str) -> str:
        """
        Build minimal prompt for Ollama
        
        Args:
            violation_id: Violation ID
            violation_text: Violation description
            category: Category (CERT, MISRA)
            severity: Severity level
        
        Returns:
            Prompt string
        """
        prompt = f"""You are a code analysis expert. Generate a concise fix suggestion for this violation.

Violation ID: {violation_id}
Category: {category}
Severity: {severity}
Description: {violation_text}

Provide:
1. Fix type (one word: cast, refactor, declaration, etc.)
2. Brief description (1-2 sentences)
3. Code example (before/after, keep it minimal)
4. Priority (HIGH/MEDIUM/LOW)

Format as JSON:
{{
  "type": "fix_type",
  "description": "brief description",
  "example": "// Before:\\n...\\n// After:\\n...",
  "priority": "PRIORITY"
}}

Keep response under 200 words. Focus on C/C++ code."""
        
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
            # Try to extract JSON from response
            # LLM might add extra text before/after JSON
            start = response.find('{')
            end = response.rfind('}') + 1
            
            if start >= 0 and end > start:
                json_str = response[start:end]
                fix_data = json.loads(json_str)
                
                # Validate required fields
                required = ['type', 'description', 'priority']
                if all(k in fix_data for k in required):
                    # Add AI metadata
                    fix_data['ai_generated'] = True
                    fix_data['model'] = self.model
                    fix_data['violation_id'] = violation.get('violation_id')
                    fix_data['category'] = violation.get('category')
                    
                    return fix_data
            
            # If JSON parsing fails, try to extract key information
            logger.warning("Could not parse JSON, attempting text extraction")
            return self._extract_from_text(response, violation)
            
        except json.JSONDecodeError as e:
            logger.error(f"JSON parse error: {str(e)}")
            return self._extract_from_text(response, violation)
        except Exception as e:
            logger.error(f"Response parsing error: {str(e)}")
            return None
    
    def _extract_from_text(self, text: str, violation: Dict) -> Optional[Dict]:
        """
        Fallback: Extract fix info from free-form text
        
        Args:
            text: Response text
            violation: Violation dict
        
        Returns:
            Basic fix suggestion or None
        """
        # Simple extraction for common patterns
        lines = text.strip().split('\n')
        
        fix_data = {
            'type': 'ai_suggestion',
            'description': text[:200],  # First 200 chars
            'example': '',
            'priority': 'MEDIUM',
            'ai_generated': True,
            'model': self.model,
            'violation_id': violation.get('violation_id'),
            'category': violation.get('category'),
            'note': 'Extracted from free-form AI response'
        }
        
        # Try to find code examples
        code_section = []
        in_code = False
        for line in lines:
            if '```' in line:
                in_code = not in_code
            elif in_code:
                code_section.append(line)
        
        if code_section:
            fix_data['example'] = '\n'.join(code_section)
        
        return fix_data
    
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
