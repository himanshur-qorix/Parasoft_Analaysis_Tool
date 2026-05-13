"""
Interactive Code Fixer - AI-Powered Code Fix Applicator
Actually applies code fixes to source files with interactive review

This module transforms violation analysis from suggestion-based to action-based:
- Uses AI to generate actual code modifications
- Shows before/after diffs for each fix
- Allows interactive approval/rejection
- Applies fixes directly to source files
- Maintains backup and rollback capabilities

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 4.0.0
"""

import sys
import logging
import json
import shutil
from pathlib import Path
from typing import Dict, List, Optional, Tuple
from datetime import datetime
import difflib
import re

logger = logging.getLogger(__name__)


class InteractiveCodeFixer:
    """
    Interactive code fixer that actually applies AI-generated fixes to source files
    """
    
    def __init__(self, source_code_path: Path, ollama_integration, backup_dir: Optional[Path] = None):
        """
        Initialize Interactive Code Fixer
        
        Args:
            source_code_path: Path to source code directory
            ollama_integration: OllamaIntegration instance for AI-powered fixes
            backup_dir: Optional directory for file backups (default: source_code_path/../backups)
        """
        self.source_code_path = Path(source_code_path)
        self.ollama = ollama_integration
        
        # Setup backup directory
        if backup_dir:
            self.backup_dir = Path(backup_dir)
        else:
            self.backup_dir = self.source_code_path.parent / 'backups' / datetime.now().strftime('%Y%m%d_%H%M%S')
        
        self.backup_dir.mkdir(parents=True, exist_ok=True)
        
        # Track applied fixes for rollback
        self.applied_fixes = []
        self.fix_history = []
        
        logger.info(f"Interactive Code Fixer initialized")
        logger.info(f"  Source path: {self.source_code_path}")
        logger.info(f"  Backup path: {self.backup_dir}")
    
    def fix_violation_interactive(self, violation: Dict, code_context: Dict) -> Dict:
        """
        Interactively fix a single violation with AI assistance
        
        Args:
            violation: Violation dictionary
            code_context: Code context with file path, line number, and surrounding code
        
        Returns:
            Dictionary with fix results and status
        """
        violation_id = violation.get('violation_id', 'UNKNOWN')
        print(f"\n{'='*80}")
        print(f"  Violation: {violation_id}")
        print(f"  Category: {violation.get('category', 'UNKNOWN')}")
        print(f"  Severity: {violation.get('severity', 'MEDIUM')}")
        print(f"{'='*80}")
        print(f"\nDescription: {violation.get('violation_text', 'N/A')}")
        
        # Show current code
        print(f"\n📄 Current Code ({code_context['file']}:{code_context['line']}):")
        print("-" * 80)
        print(code_context['context'])
        print("-" * 80)
        
        # Generate AI-powered fix
        print(f"\n🤖 Generating AI-powered fix...")
        fix_suggestion = self._generate_contextual_fix(violation, code_context)
        
        if not fix_suggestion:
            print("❌ Could not generate AI fix for this violation")
            return {'status': 'failed', 'reason': 'AI generation failed'}
        
        # Show proposed fix
        print(f"\n✨ Proposed Fix:")
        print("-" * 80)
        print(f"Type: {fix_suggestion['type']}")
        print(f"Description: {fix_suggestion['description']}")
        print("-" * 80)
        
        # Generate and show diff
        modified_code = fix_suggestion.get('modified_code')
        if modified_code:
            print(f"\n📊 Changes Preview:")
            self._show_diff(code_context['context'], modified_code)
        else:
            print(f"\n⚠️  No specific code modification generated")
            print(f"Suggestion: {fix_suggestion.get('example', 'See description above')}")
        
        # Interactive prompt
        while True:
            choice = input(f"\n  Apply this fix? [y=yes, n=no, e=edit, s=skip all]: ").lower().strip()
            
            if choice == 'y':
                if modified_code:
                    result = self._apply_fix_to_file(code_context, modified_code, violation_id)
                    return result
                else:
                    print("⚠️  No code modification to apply. Manual fix required.")
                    return {'status': 'manual_required', 'suggestion': fix_suggestion}
            
            elif choice == 'n':
                print("⏭️  Fix rejected, moving to next violation")
                return {'status': 'rejected'}
            
            elif choice == 'e':
                print("\n📝 Manual Edit Mode")
                print("Enter the corrected code (type 'END' on a new line when done):")
                print("-" * 80)
                
                manual_lines = []
                while True:
                    line = input()
                    if line.strip() == 'END':
                        break
                    manual_lines.append(line)
                
                manual_code = '\n'.join(manual_lines)
                if manual_code.strip():
                    self._show_diff(code_context['context'], manual_code)
                    confirm = input("\nApply this manual edit? [y/n]: ").lower().strip()
                    if confirm == 'y':
                        result = self._apply_fix_to_file(code_context, manual_code, violation_id)
                        return result
            
            elif choice == 's':
                print("⏸️  Skipping remaining violations")
                return {'status': 'skip_all'}
            
            else:
                print("Invalid choice. Please enter y, n, e, or s.")
    
    def _generate_contextual_fix(self, violation: Dict, code_context: Dict) -> Optional[Dict]:
        """
        Generate context-aware fix using AI with actual code modification
        
        Args:
            violation: Violation dictionary
            code_context: Code context with actual source code
        
        Returns:
            Fix suggestion with modified_code field
        """
        if not self.ollama or not self.ollama.enabled:
            logger.warning("Ollama not enabled, cannot generate contextual fix")
            return None
        
        try:
            # Build enhanced prompt for actual code modification
            prompt = self._build_code_modification_prompt(violation, code_context)
            
            logger.info(f"[AI] Generating contextual fix for {violation['violation_id']}...")
            
            # Import ollama here to avoid issues if not installed
            import ollama
            
            response = ollama.generate(
                model=self.ollama.model,
                prompt=prompt,
                options={
                    'temperature': 0.1,  # Lower temperature for more precise code generation
                    'num_predict': 2000,  # Allow longer responses for code
                }
            )
            
            # Extract response text
            if hasattr(response, 'response'):
                response_text = response.response
            elif isinstance(response, dict):
                response_text = response.get('response', '')
            else:
                logger.error(f"Unexpected response type: {type(response)}")
                return None
            
            # Parse the AI response to extract modified code
            fix_data = self._parse_code_modification_response(response_text, code_context)
            
            if fix_data:
                logger.info(f"[OK] Generated contextual fix with code modification")
                return fix_data
            else:
                logger.warning(f"[WARNING] Could not parse code modification from AI response")
                return None
        
        except Exception as e:
            logger.error(f"Error generating contextual fix: {e}")
            return None
    
    def _build_code_modification_prompt(self, violation: Dict, code_context: Dict) -> str:
        """
        Build prompt for AI to generate actual code modifications
        
        Args:
            violation: Violation dictionary
            code_context: Code context with source code
        
        Returns:
            Prompt string
        """
        violation_id = violation['violation_id']
        violation_text = violation['violation_text']
        category = violation.get('category', 'OTHER')
        
        prompt = f"""You are an expert C/C++ code refactoring assistant. Your task is to fix a specific coding violation by modifying the actual source code.

VIOLATION DETAILS:
- ID: {violation_id}
- Category: {category}
- Description: {violation_text}

CURRENT SOURCE CODE:
File: {code_context['file']}
Line {code_context['line']} has the violation.

```c
{code_context['context']}
```

VIOLATION LINE:
>>> {code_context['target_code']}

YOUR TASK:
Provide the COMPLETE MODIFIED CODE that fixes this violation. You must:
1. Understand the root cause of the violation
2. Apply the appropriate fix to the violating line and surrounding context if needed
3. Return the COMPLETE corrected code block (not just the changed line)
4. Ensure the fix follows {category} standards
5. Maintain all existing functionality
6. Preserve formatting and style as much as possible

OUTPUT FORMAT:
Return your response in this EXACT format:

ANALYSIS:
[Brief explanation of what causes the violation and your fix strategy]

MODIFIED_CODE:
```c
[The complete corrected code block - replace the entire context shown above]
```

TYPE: [fix_type: cast/bounds_check/initialization/refactor/validation/declaration]
PRIORITY: [HIGH/MEDIUM/LOW]

IMPORTANT: 
- Provide the COMPLETE modified code block, not just snippets
- The modified code should be ready to replace the context shown above
- Do not use placeholders like "..." or "rest of code"
- Include all lines from the context, with your fixes applied"""
        
        return prompt
    
    def _parse_code_modification_response(self, response: str, code_context: Dict) -> Optional[Dict]:
        """
        Parse AI response to extract modified code and metadata
        
        Args:
            response: AI response text
            code_context: Original code context
        
        Returns:
            Dictionary with modified_code and metadata
        """
        try:
            # Extract analysis
            analysis_match = re.search(r'ANALYSIS:\s*(.+?)(?=MODIFIED_CODE:|TYPE:|$)', response, re.DOTALL | re.IGNORECASE)
            analysis = analysis_match.group(1).strip() if analysis_match else "AI-generated fix"
            
            # Extract modified code (look for code blocks)
            code_block_pattern = r'```c\s*(.+?)```|```cpp\s*(.+?)```|```\s*(.+?)```'
            code_match = re.search(code_block_pattern, response, re.DOTALL)
            
            modified_code = None
            if code_match:
                # Get whichever group matched
                modified_code = code_match.group(1) or code_match.group(2) or code_match.group(3)
                modified_code = modified_code.strip()
            else:
                # Try to find code after "MODIFIED_CODE:" marker without code blocks
                code_text_match = re.search(r'MODIFIED_CODE:\s*(.+?)(?=TYPE:|PRIORITY:|$)', response, re.DOTALL | re.IGNORECASE)
                if code_text_match:
                    modified_code = code_text_match.group(1).strip()
                    # Remove any remaining ``` markers
                    modified_code = modified_code.replace('```', '').strip()
            
            # Extract type
            type_match = re.search(r'TYPE:\s*(\w+)', response, re.IGNORECASE)
            fix_type = type_match.group(1) if type_match else 'refactor'
            
            # Extract priority
            priority_match = re.search(r'PRIORITY:\s*(HIGH|MEDIUM|LOW)', response, re.IGNORECASE)
            priority = priority_match.group(1).upper() if priority_match else 'MEDIUM'
            
            if modified_code:
                return {
                    'type': fix_type,
                    'description': analysis,
                    'modified_code': modified_code,
                    'priority': priority,
                    'ai_generated': True,
                    'model': self.ollama.model
                }
            else:
                logger.warning("Could not extract modified code from AI response")
                logger.debug(f"Response preview: {response[:500]}")
                return None
        
        except Exception as e:
            logger.error(f"Error parsing code modification response: {e}")
            return None
    
    def _show_diff(self, original: str, modified: str):
        """
        Show colored diff between original and modified code
        
        Args:
            original: Original code
            modified: Modified code
        """
        original_lines = original.splitlines(keepends=True)
        modified_lines = modified.splitlines(keepends=True)
        
        diff = difflib.unified_diff(
            original_lines,
            modified_lines,
            fromfile='Original',
            tofile='Modified',
            lineterm=''
        )
        
        print("\n🔍 Diff:")
        print("-" * 80)
        
        for line in diff:
            # Color code the diff
            if line.startswith('+') and not line.startswith('+++'):
                print(f"  \033[92m{line}\033[0m", end='')  # Green for additions
            elif line.startswith('-') and not line.startswith('---'):
                print(f"  \033[91m{line}\033[0m", end='')  # Red for deletions
            elif line.startswith('@@'):
                print(f"  \033[94m{line}\033[0m", end='')  # Blue for line numbers
            else:
                print(f"  {line}", end='')
        
        print()
        print("-" * 80)
    
    def _apply_fix_to_file(self, code_context: Dict, modified_code: str, violation_id: str) -> Dict:
        """
        Apply fix to the actual source file
        
        Args:
            code_context: Code context with file path and line information
            modified_code: The corrected code
            violation_id: Violation ID for tracking
        
        Returns:
            Result dictionary
        """
        try:
            file_path = self._find_source_file(code_context['file'])
            if not file_path:
                return {
                    'status': 'failed',
                    'reason': f"Source file not found: {code_context['file']}"
                }
            
            # Create backup
            backup_path = self._backup_file(file_path)
            
            # Read entire file
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                file_content = f.read()
            
            # Replace the context with modified code
            original_context = code_context['context']
            
            # Check if original context exists in file
            if original_context not in file_content:
                logger.warning("Original context not found exactly in file, attempting fuzzy match...")
                # Try line-by-line replacement for the specific violation line
                return self._apply_fix_line_based(file_path, code_context, modified_code, violation_id, backup_path)
            
            # Replace the context
            new_content = file_content.replace(original_context, modified_code, 1)
            
            # Write modified file
            with open(file_path, 'w', encoding='utf-8', errors='ignore') as f:
                f.write(new_content)
            
            # Track the fix
            fix_record = {
                'violation_id': violation_id,
                'file': str(file_path),
                'backup': str(backup_path),
                'timestamp': datetime.now().isoformat(),
                'original': original_context,
                'modified': modified_code
            }
            self.applied_fixes.append(fix_record)
            
            print(f"✅ Fix applied successfully to {code_context['file']}")
            print(f"   Backup saved to: {backup_path}")
            
            return {
                'status': 'applied',
                'file': str(file_path),
                'backup': str(backup_path),
                'fix_record': fix_record
            }
        
        except Exception as e:
            logger.error(f"Error applying fix to file: {e}")
            return {
                'status': 'failed',
                'reason': str(e)
            }
    
    def _apply_fix_line_based(self, file_path: Path, code_context: Dict, modified_code: str, 
                              violation_id: str, backup_path: Path) -> Dict:
        """
        Apply fix using line-based replacement when exact context match fails
        
        Args:
            file_path: Path to source file
            code_context: Code context information
            modified_code: Modified code to apply
            violation_id: Violation ID
            backup_path: Backup file path
        
        Returns:
            Result dictionary
        """
        try:
            # Read file lines
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                lines = f.readlines()
            
            target_line_num = code_context['line'] - 1  # 0-indexed
            context_start = code_context['start_line'] - 1
            context_end = code_context['end_line']
            
            # Replace the context lines with modified code
            modified_lines = modified_code.splitlines(keepends=True)
            
            # Ensure newlines
            if modified_lines and not modified_lines[-1].endswith('\n'):
                modified_lines[-1] += '\n'
            
            new_lines = lines[:context_start] + modified_lines + lines[context_end:]
            
            # Write modified file
            with open(file_path, 'w', encoding='utf-8', errors='ignore') as f:
                f.writelines(new_lines)
            
            # Track the fix
            fix_record = {
                'violation_id': violation_id,
                'file': str(file_path),
                'backup': str(backup_path),
                'timestamp': datetime.now().isoformat(),
                'line_range': f"{context_start+1}-{context_end}",
                'modified': modified_code
            }
            self.applied_fixes.append(fix_record)
            
            print(f"✅ Fix applied successfully (line-based) to {code_context['file']}")
            print(f"   Lines modified: {context_start+1} to {context_end}")
            
            return {
                'status': 'applied',
                'file': str(file_path),
                'backup': str(backup_path),
                'fix_record': fix_record
            }
        
        except Exception as e:
            logger.error(f"Error in line-based fix application: {e}")
            return {
                'status': 'failed',
                'reason': str(e)
            }
    
    def _backup_file(self, file_path: Path) -> Path:
        """
        Create backup of file before modification
        
        Args:
            file_path: Path to file to backup
        
        Returns:
            Path to backup file
        """
        # Preserve directory structure in backup
        rel_path = file_path.relative_to(self.source_code_path)
        backup_path = self.backup_dir / rel_path
        backup_path.parent.mkdir(parents=True, exist_ok=True)
        
        shutil.copy2(file_path, backup_path)
        logger.debug(f"Backed up {file_path} to {backup_path}")
        
        return backup_path
    
    def _find_source_file(self, filename: str) -> Optional[Path]:
        """
        Find source file in source code directory
        
        Args:
            filename: File name to search for
        
        Returns:
            Path to file or None
        """
        # Try direct path first if it contains directory separators
        if '/' in filename or '\\' in filename:
            full_path = self.source_code_path / filename.replace('\\', '/')
            if full_path.exists():
                return full_path
        
        # Search recursively
        for path in self.source_code_path.rglob(Path(filename).name):
            if path.is_file():
                return path
        
        logger.warning(f"Could not find source file: {filename}")
        return None
    
    def rollback_fix(self, fix_record: Dict) -> bool:
        """
        Rollback a previously applied fix
        
        Args:
            fix_record: Fix record from applied_fixes list
        
        Returns:
            True if rollback successful
        """
        try:
            file_path = Path(fix_record['file'])
            backup_path = Path(fix_record['backup'])
            
            if not backup_path.exists():
                logger.error(f"Backup file not found: {backup_path}")
                return False
            
            shutil.copy2(backup_path, file_path)
            logger.info(f"Rolled back fix for {fix_record['violation_id']}")
            return True
        
        except Exception as e:
            logger.error(f"Error rolling back fix: {e}")
            return False
    
    def rollback_all(self) -> int:
        """
        Rollback all applied fixes
        
        Returns:
            Number of fixes rolled back
        """
        count = 0
        for fix_record in reversed(self.applied_fixes):
            if self.rollback_fix(fix_record):
                count += 1
        
        logger.info(f"Rolled back {count} fixes")
        return count
    
    def save_fix_history(self, output_file: Path):
        """
        Save history of applied fixes to JSON file
        
        Args:
            output_file: Path to output JSON file
        """
        try:
            history_data = {
                'timestamp': datetime.now().isoformat(),
                'backup_dir': str(self.backup_dir),
                'fixes_applied': len(self.applied_fixes),
                'fixes': self.applied_fixes
            }
            
            with open(output_file, 'w', encoding='utf-8') as f:
                json.dump(history_data, f, indent=2)
            
            logger.info(f"Fix history saved to {output_file}")
        
        except Exception as e:
            logger.error(f"Error saving fix history: {e}")


def run_interactive_fixer(module_name: str, violations: List[Dict], source_code_path: Path,
                          ollama_integration, kb_manager) -> Dict:
    """
    Run interactive code fixer on a list of violations
    
    Args:
        module_name: Module name
        violations: List of violations to fix
        source_code_path: Path to source code
        ollama_integration: OllamaIntegration instance
        kb_manager: KnowledgeDatabaseManager instance
    
    Returns:
        Results dictionary with statistics
    """
    print("\n" + "="*80)
    print("  🚀 INTERACTIVE AI-POWERED CODE FIXER")
    print("  Actually applies fixes to your source code!")
    print("="*80)
    print(f"\nModule: {module_name}")
    print(f"Violations to fix: {len(violations)}")
    print(f"Source code path: {source_code_path}")
    print("\n⚠️  WARNING: This will modify your source files!")
    print("   Backups will be created automatically.")
    print("="*80)
    
    confirm = input("\n  Continue? [y/n]: ").lower().strip()
    if confirm != 'y':
        print("Aborted.")
        return {'status': 'aborted'}
    
    # Initialize fixer
    fixer = InteractiveCodeFixer(source_code_path, ollama_integration)
    
    # Statistics
    stats = {
        'total': len(violations),
        'applied': 0,
        'rejected': 0,
        'failed': 0,
        'manual_required': 0,
        'skipped': 0
    }
    
    # Process each violation
    for idx, violation in enumerate(violations, 1):
        print(f"\n{'#'*80}")
        print(f"  Progress: {idx}/{len(violations)}")
        print(f"{'#'*80}")
        
        # Get code context
        code_context = _extract_code_context(violation, source_code_path)
        if not code_context:
            print(f"⚠️  Could not extract code context for {violation['violation_id']}")
            print(f"   File: {violation.get('files_affected', ['Unknown'])[0]}")
            stats['failed'] += 1
            continue
        
        # Interactive fix
        result = fixer.fix_violation_interactive(violation, code_context)
        
        status = result['status']
        if status == 'applied':
            stats['applied'] += 1
            # Update knowledge base
            kb_manager.update_fix_status(violation['violation_id'], {
                'fix_applied': True,
                'fix_method': 'ai_interactive',
                'timestamp': datetime.now().isoformat()
            })
        elif status == 'rejected':
            stats['rejected'] += 1
        elif status == 'failed':
            stats['failed'] += 1
        elif status == 'manual_required':
            stats['manual_required'] += 1
        elif status == 'skip_all':
            stats['skipped'] = len(violations) - idx + 1
            break
    
    # Save fix history
    history_file = fixer.backup_dir.parent / f"{module_name}_fix_history_{datetime.now().strftime('%Y%m%d_%H%M%S')}.json"
    fixer.save_fix_history(history_file)
    
    # Display results
    print("\n" + "="*80)
    print("  📊 FIX APPLICATION RESULTS")
    print("="*80)
    print(f"  ✅ Fixes applied: {stats['applied']}")
    print(f"  ❌ Fixes rejected: {stats['rejected']}")
    print(f"  ⚠️  Fixes failed: {stats['failed']}")
    print(f"  📝 Manual fixes required: {stats['manual_required']}")
    print(f"  ⏭️  Violations skipped: {stats['skipped']}")
    print(f"\n  📁 Backups saved to: {fixer.backup_dir}")
    print(f"  📄 Fix history: {history_file}")
    print("="*80)
    
    # Offer rollback option
    if stats['applied'] > 0:
        rollback = input("\n  Need to rollback all changes? [y/n]: ").lower().strip()
        if rollback == 'y':
            count = fixer.rollback_all()
            print(f"  ↩️  Rolled back {count} fixes")
    
    return stats


def _extract_code_context(violation: Dict, source_code_path: Path) -> Optional[Dict]:
    """
    Extract code context from violation
    
    Args:
        violation: Violation dictionary
        source_code_path: Path to source code
    
    Returns:
        Code context dictionary or None
    """
    try:
        files_affected = violation.get('files_affected', [])
        if not files_affected:
            return None
        
        # Parse file and line number - handle both dict and string formats
        file_entry = files_affected[0]
        
        # Handle dict format (from KB)
        if isinstance(file_entry, dict):
            file_name = file_entry.get('file')
            line_number = file_entry.get('line')
            
            if not file_name or not line_number:
                return None
        
        # Handle string format
        elif isinstance(file_entry, str):
            if ':' in file_entry:
                file_name, line_str = file_entry.rsplit(':', 1)
                try:
                    line_number = int(line_str)
                except ValueError:
                    return None
            else:
                return None
        else:
            return None
        
        # Find source file
        source_file = _find_source_file(source_code_path, file_name)
        if not source_file:
            return None
        
        # Read code context
        context_lines = 10  # More context for better AI understanding
        with open(source_file, 'r', encoding='utf-8', errors='ignore') as f:
            all_lines = f.readlines()
        
        total_lines = len(all_lines)
        start_line = max(0, line_number - context_lines - 1)
        end_line = min(total_lines, line_number + context_lines)
        
        context_code = ''.join(all_lines[start_line:end_line])
        target_line_code = all_lines[line_number - 1] if 0 < line_number <= total_lines else ''
        
        return {
            'file': file_name,
            'line': line_number,
            'target_code': target_line_code.strip(),
            'context': context_code,
            'start_line': start_line + 1,
            'end_line': end_line
        }
    
    except Exception as e:
        logger.error(f"Error extracting code context: {e}")
        return None


def _find_source_file(source_code_path: Path, filename: str) -> Optional[Path]:
    """
    Find source file in directory
    
    Args:
        source_code_path: Root path to search
        filename: File name to find
    
    Returns:
        Path to file or None
    """
    # Try direct path
    if '/' in filename or '\\' in filename:
        full_path = source_code_path / filename.replace('\\', '/')
        if full_path.exists():
            return full_path
    
    # Search recursively
    for path in source_code_path.rglob(Path(filename).name):
        if path.is_file():
            return path
    
    return None
