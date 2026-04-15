"""
Code Fix Generator
Generates code fixes and justifications for Parasoft violations
Enhanced with Parasoft Rules Database integration

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 2.0.0
"""

import logging
import json
from pathlib import Path
from typing import Dict, List, Optional
from datetime import datetime
from KnowledgeDatabaseManager import KnowledgeDatabaseManager
from OllamaIntegration import OllamaIntegration
from ParasoftRulesParser import ParasoftRulesParser

logger = logging.getLogger(__name__)


class CodeFixGenerator:
    """Generates fixes and justifications for Parasoft violations"""
    
    def __init__(self, module_name: str, kb_manager: KnowledgeDatabaseManager, 
                 fixes_dir: Path, config: Optional[Dict] = None):
        """
        Initialize the Code Fix Generator
        
        Args:
            module_name: Name of the module
            kb_manager: Knowledge database manager instance
            fixes_dir: Directory to store generated fixes
            config: Optional configuration dictionary
        """
        self.module_name = module_name
        self.kb_manager = kb_manager
        self.kb_manager.load_knowledge_base(module_name)
        self.fixes_dir = Path(fixes_dir)
        self.fixes_dir.mkdir(exist_ok=True)
        
        # Create module-specific fixes directory
        self.module_fixes_dir = self.fixes_dir / module_name
        self.module_fixes_dir.mkdir(exist_ok=True)
        
        # Initialize Ollama integration
        if config is None:
            # Load config from file
            config_path = Path(__file__).parent.parent / 'config' / 'config.json'
            if config_path.exists():
                with open(config_path, 'r') as f:
                    config = json.load(f)
            else:
                config = {}
        
        # Initialize Parasoft Rules Database (PRIMARY SOURCE)
        rules_dir = Path(__file__).parent.parent / 'data' / 'Parasoft_Enabled_Rules_List' / 'gendoc'
        if rules_dir.exists():
            try:
                self.rules_parser = ParasoftRulesParser(rules_dir)
                rules_count = self.rules_parser.load_all_rules()
                logger.info(f"[OK] Parasoft Rules Database loaded: {rules_count} rules available")
                self.use_rules_db = True
            except Exception as e:
                logger.warning(f"Failed to load Parasoft Rules Database: {e}")
                self.rules_parser = None
                self.use_rules_db = False
        else:
            logger.warning(f"Parasoft Rules directory not found: {rules_dir}")
            self.rules_parser = None
            self.use_rules_db = False
        
        ai_config = config.get('ai_integration', {})
        self.ollama = OllamaIntegration(ai_config)
        
        # Log AI status
        status = self.ollama.get_status()
        ai_mode = status.get('ai_mode', 'hybrid')
        if status['enabled']:
            logger.info(f"[OK] AI Mode: {ai_mode} | Model: {status['model']} at {status['base_url']}")
        else:
            if ai_mode == 'rules_only':
                logger.info(f"[INFO] AI Mode: {ai_mode} - Using rule-based fixes only")
            else:
                logger.info(f"[INFO] AI disabled - Using rule-based fixes")
        
        logger.info(f"Code Fix Generator initialized for module: {module_name}")
    
    def generate_all_fixes(self, violation_ids: Optional[List[str]] = None) -> Dict:
        """
        Generate fixes for violations
        
        Args:
            violation_ids: List of specific violation IDs (None = all unfixed)
        
        Returns:
            Results dictionary
        """
        if violation_ids:
            violations_to_fix = [
                self.kb_manager.get_violation(vid)
                for vid in violation_ids
                if self.kb_manager.get_violation(vid)
            ]
        else:
            violations_to_fix = [
                v for v in self.kb_manager.get_all_violations()
                if not v.get('fix_applied')
            ]
        
        logger.info(f"Generating fixes for {len(violations_to_fix)} violations")
        
        fixes_generated = 0
        fixes_failed = 0
        
        all_fixes = []
        
        for violation in violations_to_fix:
            try:
                fix_data = self._generate_fix_for_violation(violation)
                if fix_data:
                    all_fixes.append(fix_data)
                    fixes_generated += 1
                else:
                    fixes_failed += 1
            except Exception as e:
                logger.error(f"Error generating fix for {violation['violation_id']}: {str(e)}")
                fixes_failed += 1
        
        # Save all fixes to a single file
        if all_fixes:
            fixes_file = self.module_fixes_dir / f"{self.module_name}_fixes_{datetime.now().strftime('%Y%m%d_%H%M%S')}.txt"
            self._save_fixes_file(all_fixes, fixes_file)
            logger.info(f"Fixes saved to: {fixes_file}")
        
        return {
            'fixes_generated': fixes_generated,
            'fixes_failed': fixes_failed,
            'total_violations': len(violations_to_fix),
            'fixes_file': str(fixes_file) if all_fixes else None
        }
    
    def _generate_fix_for_violation(self, violation: Dict) -> Optional[Dict]:
        """
        Generate fix for a specific violation
        
        Args:
            violation: Violation dictionary
        
        Returns:
            Fix data dictionary
        """
        violation_id = violation['violation_id']
        violation_text = violation['violation_text']
        severity = violation['severity']
        category = violation['category']
        
        # Generate fix based on violation type
        fix_suggestion = self._get_fix_suggestion(violation_id, violation_text, category)
        
        if not fix_suggestion:
            logger.warning(f"No fix suggestion available for {violation_id}")
            return None
        
        fix_data = {
            'violation_id': violation_id,
            'violation_text': violation_text,
            'severity': severity,
            'category': category,
            'files_affected': violation['files_affected'],
            'fix_suggestion': fix_suggestion,
            'timestamp': datetime.now().isoformat()
        }
        
        # Update knowledge base
        self.kb_manager.update_fix_status(violation_id, {
            'fix_generated': True,
            'fix_type': fix_suggestion['type'],
            'timestamp': datetime.now().isoformat()
        })
        
        return fix_data
    
    def _get_fix_suggestion(self, violation_id: str, violation_text: str, category: str) -> Optional[Dict]:
        """
        Get fix suggestion using Parasoft Rules Database (PRIORITY) -> AI -> Rule-based fallback
        
        Strategy:
        1. Try Parasoft Official Rules Database (official examples and repairs)
        2. Fall back to AI generation (if enabled)
        3. Fall back to generic rule-based patterns
        
        Args:
            violation_id: Violation ID
            violation_text: Violation description
            category: Violation category
        
        Returns:
            Fix suggestion dictionary
        """
        # PRIORITY: Try Parasoft Rules Database for official fix
        if self.use_rules_db and self.rules_parser:
            parasoft_fix = self._get_parasoft_official_fix(violation_id, violation_text, category)
            if parasoft_fix:
                logger.info(f"[PARASOFT-DB] Using official Parasoft fix for {violation_id}")
                return parasoft_fix
        
        # FALLBACK 1: Try AI generation (if enabled and appropriate)
        if self.ollama.should_use_ai(category, violation_text):
            violation_dict = {
                'violation_id': violation_id,
                'violation_text': violation_text,
                'category': category,
                'severity': 'MEDIUM'  # Default severity
            }
            
            ai_fix = self.ollama.generate_fix_suggestion(violation_dict)
            if ai_fix:
                logger.info(f"[AI] Using AI-generated fix for {violation_id}")
                return ai_fix
            else:
                logger.info(f"[FALLBACK] Using rule-based fix for {violation_id}")
        
        # FALLBACK 2: Generic rule-based fixes
        text_upper = violation_text.upper()
        
        # Common MISRA fixes
        if 'MISRA' in category:
            return self._get_misra_fix(violation_id, text_upper, violation_text)
        
        # Common CERT fixes
        elif 'CERT' in category:
            return self._get_cert_fix(violation_id, text_upper, violation_text)
        
        # Generic fixes
        else:
            return self._get_generic_fix(text_upper, violation_text)
    
    def _get_parasoft_official_fix(self, violation_id: str, violation_text: str, category: str) -> Optional[Dict]:
        """
        Extract official fix from Parasoft Rules Database
        
        Args:
            violation_id: Violation ID (e.g., "CERT_C-STR31-a")
            violation_text: Violation description
            category: Violation category
        
        Returns:
            Fix suggestion dictionary with official Parasoft repair example, or None
        """
        try:
            # Extract base rule ID from violation ID
            # Example: "CERT_C-STR31-a-2" -> "CERT_C-STR31-a"
            # Example: "MISRAC2012-RULE_8_7-a" -> "MISRAC2012-RULE_8_7-a"
            
            # Try exact match first
            rule = self.rules_parser.get_rule(violation_id)
            
            # If not found, try extracting base rule (remove trailing numbers/variant)
            if not rule:
                import re
                # Try removing trailing -\d+ suffix
                base_id = re.sub(r'-\d+$', '', violation_id)
                rule = self.rules_parser.get_rule(base_id)
            
            if not rule:
                return None
            
            # Build fix suggestion from official Parasoft documentation
            fix_parts = []
            
            # Add description
            if rule.description:
                fix_parts.append(f"Description: {rule.description[:200]}...")
            
            # Add security relevance if available
            if rule.security_relevance and rule.security_relevance != "N/A":
                fix_parts.append(f"\nSecurity Impact: {rule.security_relevance[:150]}...")
            
            # Add repair example (MOST IMPORTANT)
            repair_code = ""
            if rule.example_repair:
                repair_code = rule.example_repair
                # Limit repair code length for display
                if len(repair_code) > 500:
                    repair_code = repair_code[:500] + "\n... (truncated)"
            
            # Add violation example for context
            violation_code = ""
            if rule.example_violation:
                violation_code = rule.example_violation
                if len(violation_code) > 300:
                    violation_code = violation_code[:300] + "\n... (truncated)"
            
            # Build formatted fix suggestion
            fix_description = "\n".join(fix_parts)
            
            example_text = ""
            if violation_code:
                example_text += f"\n--- VIOLATION EXAMPLE ---\n{violation_code}\n"
            if repair_code:
                example_text += f"\n--- OFFICIAL REPAIR ---\n{repair_code}\n"
            
            # Add CWE mappings if available
            cwe_info = ""
            if rule.cwe_mappings:
                cwe_info = f"CWE Mappings: {', '.join(rule.cwe_mappings[:5])}"
            
            return {
                'type': 'parasoft_official',
                'standard': rule.standard,
                'description': fix_description,
                'example': example_text,
                'cwe_mappings': rule.cwe_mappings,
                'repair_code': repair_code,
                'violation_code': violation_code,
                'references': rule.references[:3] if rule.references else [],  # Limit to 3
                'source': f'Parasoft Official Documentation: {rule.title}'
            }
            
        except Exception as e:
            logger.debug(f"Could not extract Parasoft fix for {violation_id}: {e}")
            return None
    
    def _get_misra_fix(self, violation_id: str, text_upper: str, violation_text: str) -> Dict:
        """Get MISRA-specific fix suggestions"""
        
        # Rule 10.1 - Type conversions
        if '10.1' in violation_id or 'IMPLICIT CONVERSION' in text_upper:
            return {
                'type': 'explicit_cast',
                'description': 'Add explicit type cast to avoid implicit conversion',
                'example': '''
// Before:
uint8_t a = 5;
uint16_t b = a + 10;  // Implicit conversion

// After:
uint8_t a = 5;
uint16_t b = (uint16_t)a + 10U;  // Explicit cast
''',
                'priority': 'HIGH'
            }
        
        # Rule 11.x - Pointer conversions
        elif '11.' in violation_id or 'POINTER' in text_upper:
            return {
                'type': 'pointer_cast',
                'description': 'Use proper pointer casting or avoid incompatible pointer types',
                'example': '''
// Before:
int* ptr = (int*)some_void_ptr;

// After:
int* ptr = (int*)(uintptr_t)some_void_ptr;  // Use intermediate cast
''',
                'priority': 'HIGH'
            }
        
        # Rule 14.x - Control flow
        elif '14.' in violation_id or 'FOR LOOP' in text_upper or 'WHILE' in text_upper:
            return {
                'type': 'control_flow',
                'description': 'Ensure proper loop structure and counter usage',
                'example': '''
// Ensure loop counter is not modified within the loop body
// Use appropriate loop bounds and increment
''',
                'priority': 'MEDIUM'
            }
        
        # Rule 21.x - Standard library
        elif '21.' in violation_id or 'STANDARD LIBRARY' in text_upper:
            return {
                'type': 'stdlib_replacement',
                'description': 'Avoid or replace restricted standard library function',
                'example': '''
// Replace unsafe library functions with safe alternatives
// Example: Use strncpy instead of strcpy
''',
                'priority': 'HIGH'
            }
        
        # Default MISRA fix
        else:
            return {
                'type': 'misra_compliance',
                'description': f'Review MISRA rule and apply appropriate fix: {violation_text}',
                'example': 'Refer to MISRA C guidelines for specific rule details',
                'priority': 'MEDIUM'
            }
    
    def _get_cert_fix(self, violation_id: str, text_upper: str, violation_text: str) -> Dict:
        """Get CERT-specific fix suggestions"""
        
        # Buffer overflow issues
        if 'BUFFER' in text_upper or 'OVERFLOW' in text_upper:
            return {
                'type': 'buffer_safety',
                'description': 'Ensure buffer boundaries are checked before access',
                'example': '''
// Before:
char buffer[10];
strcpy(buffer, user_input);  // Unsafe

// After:
char buffer[10];
strncpy(buffer, user_input, sizeof(buffer) - 1);
buffer[sizeof(buffer) - 1] = '\\0';  // Ensure null termination
''',
                'priority': 'CRITICAL'
            }
        
        # Null pointer issues
        elif 'NULL' in text_upper or 'DEREFERENCE' in text_upper:
            return {
                'type': 'null_check',
                'description': 'Add null pointer check before dereference',
                'example': '''
// Before:
result = ptr->value;

// After:
if (ptr != NULL) {
    result = ptr->value;
} else {
    // Handle null pointer case
    result = default_value;
}
''',
                'priority': 'HIGH'
            }
        
        # Memory management
        elif 'MEMORY' in text_upper or 'LEAK' in text_upper or 'FREE' in text_upper:
            return {
                'type': 'memory_management',
                'description': 'Ensure proper memory allocation and deallocation',
                'example': '''
// Ensure every malloc has a corresponding free
// Check allocation success before use
void* ptr = malloc(size);
if (ptr != NULL) {
    // Use memory
    free(ptr);
    ptr = NULL;  // Prevent double-free
}
''',
                'priority': 'HIGH'
            }
        
        # Default CERT fix
        else:
            return {
                'type': 'cert_compliance',
                'description': f'Review CERT rule and apply appropriate fix: {violation_text}',
                'example': 'Refer to CERT Secure Coding Standards',
                'priority': 'MEDIUM'
            }
    
    def _get_generic_fix(self, text_upper: str, violation_text: str) -> Dict:
        """Get generic fix suggestions"""
        
        return {
            'type': 'generic',
            'description': violation_text,
            'example': 'Review the violation and apply appropriate fix based on context',
            'priority': 'MEDIUM'
        }
    
    def _save_fixes_file(self, fixes: List[Dict], output_file: Path):
        """
        Save fixes to a text file
        
        Args:
            fixes: List of fix dictionaries
            output_file: Path to output file
        """
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write("="*80 + "\n")
            f.write(f"PARASOFT FIXES - Module: {self.module_name}\n")
            f.write(f"Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
            f.write(f"Total Fixes: {len(fixes)}\n")
            f.write("="*80 + "\n\n")
            
            for i, fix in enumerate(fixes, 1):
                f.write(f"FIX #{i}\n")
                f.write("-"*80 + "\n")
                f.write(f"Violation ID: {fix['violation_id']}\n")
                f.write(f"Severity: {fix['severity']}\n")
                f.write(f"Category: {fix['category']}\n\n")
                
                f.write(f"Description:\n{fix['violation_text']}\n\n")
                
                f.write(f"Fix Type: {fix['fix_suggestion']['type']}\n")
                f.write(f"Priority: {fix['fix_suggestion']['priority']}\n\n")
                
                f.write(f"Fix Description:\n{fix['fix_suggestion']['description']}\n\n")
                
                if 'example' in fix['fix_suggestion']:
                    f.write(f"Example:\n{fix['fix_suggestion']['example']}\n\n")
                
                f.write(f"Files Affected:\n")
                for file_entry in fix['files_affected'][:5]:
                    f.write(f"  - {file_entry['file']}:{file_entry['line']}\n")
                
                if len(fix['files_affected']) > 5:
                    f.write(f"  ... and {len(fix['files_affected']) - 5} more files\n")
                
                f.write("\n" + "="*80 + "\n\n")
    
    def generate_justifications(self, violation_ids: Optional[List[str]] = None) -> Dict:
        """
        Generate Parasoft-formatted justifications
        
        Args:
            violation_ids: List of specific violation IDs (None = all unjustified)
        
        Returns:
            Results dictionary
        """
        if violation_ids:
            violations_to_justify = [
                self.kb_manager.get_violation(vid)
                for vid in violation_ids
                if self.kb_manager.get_violation(vid)
            ]
        else:
            violations_to_justify = [
                v for v in self.kb_manager.get_all_violations()
                if not v.get('justification_added') and v.get('justifiable') == 'Yes'
            ]
        
        logger.info(f"Generating justifications for {len(violations_to_justify)} violations")
        
        justifications_generated = 0
        all_justifications = []
        
        for violation in violations_to_justify:
            just_data = self._generate_justification(violation)
            if just_data:
                all_justifications.append(just_data)
                justifications_generated += 1
        
        # Save justifications to file
        if all_justifications:
            just_file = self.module_fixes_dir / f"{self.module_name}_justifications_{datetime.now().strftime('%Y%m%d_%H%M%S')}.txt"
            self._save_justifications_file(all_justifications, just_file)
            logger.info(f"Justifications saved to: {just_file}")
        
        return {
            'justifications_generated': justifications_generated,
            'total_violations': len(violations_to_justify),
            'justifications_file': str(just_file) if all_justifications else None
        }
    
    def _generate_justification(self, violation: Dict) -> Optional[Dict]:
        """
        Generate Parasoft-formatted justification
        
        Args:
            violation: Violation dictionary
        
        Returns:
            Justification data dictionary
        """
        violation_id = violation['violation_id']
        
        # Generate justification text in Parasoft format
        justification_text = self._format_parasoft_justification(violation)
        
        just_data = {
            'violation_id': violation_id,
            'violation_text': violation['violation_text'],
            'justification': justification_text,
            'files_affected': violation['files_affected'],
            'timestamp': datetime.now().isoformat()
        }
        
        # Update knowledge base
        self.kb_manager.update_justification_status(violation_id, justification_text)
        
        return just_data
    
    def _format_parasoft_justification(self, violation: Dict) -> str:
        """
        Format justification in Parasoft format
        
        Args:
            violation: Violation dictionary
        
        Returns:
            Formatted justification text
        """
        # Parasoft justification format:
        # /* parasoft-begin-suppress RULE_ID "Reason for suppression" */
        # ... code ...
        # /* parasoft-end-suppress RULE_ID */
        
        violation_id = violation['violation_id']
        
        # Generate reason based on violation type
        reason = self._generate_justification_reason(violation)
        
        justification = f'''/* parasoft-begin-suppress {violation_id} "{reason}" */
/* 
 * Justification: {reason}
 * Reviewed by: [Developer Name]
 * Date: {datetime.now().strftime('%Y-%m-%d')}
 * Ticket/Issue: [Reference]
 */
... your code here ...
/* parasoft-end-suppress {violation_id} */'''
        
        return justification
    
    def _generate_justification_reason(self, violation: Dict) -> str:
        """
        Generate justification reason using Parasoft Rules Database (PRIORITY) -> Fallback to generic
        
        Args:
            violation: Violation dictionary
        
        Returns:
            Justification reason text
        """
        violation_id = violation['violation_id']
        violation_text = violation['violation_text'].upper()
        
        # PRIORITY: Try to get official rationale from Parasoft Rules Database
        if self.use_rules_db and self.rules_parser:
            official_reason = self._get_parasoft_justification_rationale(violation_id)
            if official_reason:
                # Use official security relevance + benefits as justification reason
                return f"Parasoft Official: {official_reason[:200]}"
        
        # FALLBACK: Common generic justification reasons
        if 'DESIGN' in violation_text or 'ARCHITECTURE' in violation_text:
            return "Design decision - alternative approach not feasible in current architecture"
        
        elif 'LEGACY' in violation_text or 'THIRD PARTY' in violation_text:
            return "Legacy code compatibility - cannot be changed without breaking existing functionality"
        
        elif 'PERFORMANCE' in violation_text:
            return "Performance optimization - critical path where alternative approach impacts performance"
        
        elif 'STANDARD LIBRARY' in violation_text:
            return "Required for platform compatibility - no safer alternative available"
        
        else:
            return "Technical constraint - fix would require significant architectural changes"
    
    def _get_parasoft_justification_rationale(self, violation_id: str) -> Optional[str]:
        """
        Get official justification rationale from Parasoft Rules Database
        
        Args:
            violation_id: Violation ID
        
        Returns:
            Justification rationale text combining security relevance and benefits, or None
        """
        try:
            # Get rule from database
            rationale = self.rules_parser.get_justification_rationale(violation_id)
            if rationale:
                # Clean up and format
                rationale = rationale.replace('\n', ' ').strip()
                # Limit length
                if len(rationale) > 250:
                    rationale = rationale[:247] + "..."
                return rationale
            return None
        except Exception as e:
            logger.debug(f"Could not extract justification rationale for {violation_id}: {e}")
            return None
    
    def _save_justifications_file(self, justifications: List[Dict], output_file: Path):
        """
        Save justifications to a text file
        
        Args:
            justifications: List of justification dictionaries
            output_file: Path to output file
        """
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write("="*80 + "\n")
            f.write(f"PARASOFT JUSTIFICATIONS - Module: {self.module_name}\n")
            f.write(f"Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
            f.write(f"Total Justifications: {len(justifications)}\n")
            f.write("="*80 + "\n\n")
            
            for i, just in enumerate(justifications, 1):
                f.write(f"JUSTIFICATION #{i}\n")
                f.write("-"*80 + "\n")
                f.write(f"Violation ID: {just['violation_id']}\n\n")
                
                f.write(f"Description:\n{just['violation_text']}\n\n")
                
                f.write(f"Parasoft Justification Format:\n")
                f.write(just['justification'] + "\n\n")
                
                f.write(f"Apply to files:\n")
                for file_entry in just['files_affected'][:5]:
                    f.write(f"  - {file_entry['file']}:{file_entry['line']}\n")
                
                if len(just['files_affected']) > 5:
                    f.write(f"  ... and {len(just['files_affected']) - 5} more files\n")
                
                f.write("\n" + "="*80 + "\n\n")
