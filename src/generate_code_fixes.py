"""
Generate Code Fixes Script
Generates code fix suggestions for violations using Parasoft Rules Database
Can regenerate fixes without re-running full analysis

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 3.0.0
"""

import sys
import json
from pathlib import Path
import logging
from datetime import datetime

# Add src directory to path
sys.path.insert(0, str(Path(__file__).parent))

from KnowledgeDatabaseManager import KnowledgeDatabaseManager
from CodeFixGenerator import CodeFixGenerator

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='[%(levelname)s] %(message)s'
)
logger = logging.getLogger(__name__)


def generate_code_fixes(module_name: str, specific_violations: list = None, ai_mode: str = 'hybrid', 
                       source_code_path: str = None, interactive: bool = False):
    """
    Generate code fix suggestions for violations
    
    Args:
        module_name: Module name (e.g., "Mka")
        specific_violations: Optional list of specific violation IDs to fix
        ai_mode: AI mode ('ai_only', 'hybrid', 'rules_only')
        source_code_path: Path to source code directory for context-aware fixes
        interactive: If True, prompt for each violation individually
    """
    print("\n" + "="*80)
    print("  CODE FIX GENERATOR - Version 2.3.0")
    print("  Qorix India Pvt Ltd")
    print("  Powered by Parasoft Rules Database (1200+ official rules)")
    print("="*80 + "\n")
    
    workspace_path = Path(__file__).parent.parent
    
    # Check if knowledge base exists
    kb_path = workspace_path / 'knowledge_base' / f'{module_name}_KnowledgeDatabase.json'
    if not kb_path.exists():
        logger.error(f"Knowledge base not found: {kb_path}")
        logger.error(f"Please run Run.bat first to create the knowledge base for module: {module_name}")
        return False
    
    # Load configuration
    config_path = workspace_path / 'config' / 'config.json'
    if config_path.exists():
        with open(config_path, 'r') as f:
            config = json.load(f)
    else:
        config = {}
    
    # Set AI mode
    if 'ai_integration' not in config:
        config['ai_integration'] = {}
    config['ai_integration']['ai_mode'] = ai_mode
    
    # Initialize directories - create module-specific code suggestion folder
    fixes_dir = workspace_path / f'{module_name}_code_suggestion'
    fixes_dir.mkdir(exist_ok=True)
    
    # Initialize Knowledge Database Manager
    logger.info(f"Loading knowledge base for module: {module_name}")
    kb_manager = KnowledgeDatabaseManager(workspace_path / 'knowledge_base')
    kb_manager.load_knowledge_base(module_name)
    
    # Get violation statistics
    all_violations = kb_manager.get_all_violations()
    unfixed_violations = [v for v in all_violations if not v.get('fix_applied')]
    
    logger.info(f"Knowledge Base Statistics:")
    logger.info(f"  Total violations: {len(all_violations)}")
    logger.info(f"  Already fixed: {len(all_violations) - len(unfixed_violations)}")
    logger.info(f"  Need fixes: {len(unfixed_violations)}")
    
    # Initialize Code Fix Generator
    logger.info(f"\nInitializing Code Fix Generator (AI Mode: {ai_mode})")
    if source_code_path:
        logger.info(f"Source code path: {source_code_path}")
        config['source_code_path'] = source_code_path
    fix_generator = CodeFixGenerator(module_name, kb_manager, fixes_dir, config)
    
    # Generate fixes
    print()
    
    # Interactive mode
    if interactive:
        logger.info("Interactive mode enabled - you'll be prompted for each violation")
        results = generate_fixes_interactive(fix_generator, unfixed_violations, kb_manager)
    elif specific_violations:
        logger.info(f"Generating fixes for {len(specific_violations)} specific violations...")
        results = fix_generator.generate_all_fixes(specific_violations)
    else:
        logger.info("Generating fixes for all unfixed violations...")
        results = fix_generator.generate_all_fixes()
    
    # Display results
    print("\n" + "="*80)
    print("  FIX GENERATION RESULTS")
    print("="*80)
    print(f"  ✅ Fixes generated: {results['fixes_generated']}")
    print(f"  ❌ Fixes failed: {results['fixes_failed']}")
    print(f"  📊 Total processed: {results['total_violations']}")
    
    if results.get('fixes_file'):
        print(f"\n  📁 Text file saved to:\n     {results['fixes_file']}")
    
    if results.get('html_file'):
        print(f"\n  🌐 HTML report saved to:\n     {results['html_file']}")
    
    print("\n" + "="*80)
    print("  WHAT'S IN THE FIX FILE?")
    print("="*80)
    print("  Each fix includes:")
    print("    • Official Parasoft repair examples (when available)")
    print("    • Before/after code snippets")
    print("    • Security relevance and CWE mappings")
    print("    • AI-generated suggestions (hybrid mode)")
    print("    • Rule-based patterns (fallback)")
    print()
    print("  Priority order:")
    print("    1. Parasoft Rules Database (1200+ official rules) ⭐")
    print("    2. AI generation (if enabled)")
    print("    3. Pattern-based rules (always available)")
    print("="*80 + "\n")
    
    return True


def generate_fixes_interactive(fix_generator, violations, kb_manager):
    """
    Generate fixes interactively, prompting user for each violation
    
    Args:
        fix_generator: CodeFixGenerator instance
        violations: List of violations to process
        kb_manager: KnowledgeDatabaseManager instance
    
    Returns:
        Results dictionary
    """
    print("\n" + "="*80)
    print("  INTERACTIVE FIX GENERATION")
    print("="*80)
    print("\nYou will be prompted for each violation.")
    print("Options: [y] Generate fix  [n] Skip  [a] Generate all remaining  [q] Quit\n")
    
    fixes_generated = 0
    fixes_skipped = 0
    all_fixes = []
    auto_mode = False
    
    for idx, violation in enumerate(violations, 1):
        violation_id = violation['violation_id']
        category = violation.get('category', 'UNKNOWN')
        severity = violation.get('severity', 'MEDIUM')
        files_list = violation.get('files_affected', [])
        file_info = files_list[0] if files_list else "Unknown file"
        
        print(f"\n[{idx}/{len(violations)}] Violation: {violation_id}")
        print(f"  Category: {category} | Severity: {severity}")
        print(f"  File: {file_info}")
        print(f"  Description: {violation.get('violation_text', 'N/A')[:100]}...")
        
        # Check if other modules have handled this violation
        cross_module_info = check_cross_module_handling(kb_manager, violation_id)
        if cross_module_info:
            print(f"\n  ℹ️  Cross-Module Info:")
            for info in cross_module_info:
                print(f"     {info}")
        
        if not auto_mode:
            choice = input(f"\n  Generate fix? [y/n/a/q]: ").lower().strip()
            
            if choice == 'q':
                print("\n[INFO] Quitting interactive mode...")
                break
            elif choice == 'a':
                print("\n[INFO] Switching to automatic mode for remaining violations...")
                auto_mode = True
            elif choice == 'n':
                print("[SKIPPED]")
                fixes_skipped += 1
                continue
        
        # Generate fix
        try:
            fix_data = fix_generator._generate_fix_for_violation(violation)
            if fix_data:
                all_fixes.append(fix_data)
                fixes_generated += 1
                print("[✓] Fix generated successfully")
            else:
                fixes_skipped += 1
                print("[✗] Could not generate fix")
        except Exception as e:
            logger.error(f"Error generating fix: {e}")
            fixes_skipped += 1
    
    # Save fixes if any were generated
    timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
    module_name = fix_generator.module_name
    fixes_dir = fix_generator.module_fixes_dir
    
    results = {
        'fixes_generated': fixes_generated,
        'fixes_failed': fixes_skipped,
        'total_violations': len(violations),
        'fixes_file': None,
        'html_file': None
    }
    
    if all_fixes:
        fixes_file = fixes_dir / f"{module_name}_fixes_{timestamp}.txt"
        html_file = fixes_dir / f"{module_name}_fixes_{timestamp}.html"
        
        fix_generator._save_fixes_file(all_fixes, fixes_file)
        fix_generator.save_fixes_html(all_fixes, html_file)
        
        results['fixes_file'] = str(fixes_file)
        results['html_file'] = str(html_file)
    
    return results


def check_cross_module_handling(kb_manager, violation_id):
    """
    Check if other modules have handled the same violation
    
    Args:
        kb_manager: KnowledgeDatabaseManager instance
        violation_id: Violation ID to check
    
    Returns:
        List of informational strings about cross-module handling
    """
    info = []
    
    try:
        # Get all knowledge bases
        kb_dir = kb_manager.kb_dir
        kb_files = list(kb_dir.glob('*_KnowledgeDatabase.json'))
        
        for kb_file in kb_files:
            # Skip current module
            if kb_file.stem == f"{kb_manager.module_name}_KnowledgeDatabase":
                continue
            
            try:
                with open(kb_file, 'r', encoding='utf-8') as f:
                    kb_data = json.load(f)
                
                module_name = kb_file.stem.replace('_KnowledgeDatabase', '')
                violations = kb_data.get('violations', {})
                
                # Extract base violation ID (remove trailing numbers)
                import re
                base_violation_id = re.sub(r'-\d+$', '', violation_id)
                
                # Check for exact or base match
                for vid, vdata in violations.items():
                    if vid == violation_id or vid.startswith(base_violation_id):
                        # Check if it was suppressed
                        if vdata.get('justification_added') or vdata.get('suppressed'):
                            info.append(f"Module '{module_name}': Suppressed/Justified")
                        # Check if it was fixed
                        elif vdata.get('fix_applied'):
                            fix_type = vdata.get('fix_type', 'unknown')
                            info.append(f"Module '{module_name}': Fixed ({fix_type})")
                        else:
                            info.append(f"Module '{module_name}': Also present")
                        
                        break  # Found match, no need to check other violations
            except Exception as e:
                # Silently skip if KB file is invalid
                continue
    except Exception as e:
        logger.debug(f"Could not check cross-module info: {e}")
    
    return info


def main():
    """Main entry point"""
    import argparse
    
    parser = argparse.ArgumentParser(
        description='Generate code fix suggestions for Parasoft violations',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Generate fixes for all unfixed violations in Mka module
  python src/generate_code_fixes.py Mka
  
  # Generate fixes with AI-only mode
  python src/generate_code_fixes.py Mka --ai-mode ai_only
  
  # Generate fixes for specific violations
  python src/generate_code_fixes.py Mka --violations CERT_C-STR31-a MISRAC2012-RULE_8_7-a
  
  # Generate fixes using only rules (no AI)
  python src/generate_code_fixes.py Mka --ai-mode rules_only

AI Modes:
  ai_only     - Use Ollama AI for all violations (requires Ollama)
  hybrid      - Smart: AI for complex, rules for standard (recommended, default)
  rules_only  - Use only Parasoft DB + rule-based fixes (no AI)
        """
    )
    
    parser.add_argument('module', help='Module name (e.g., Mka)')
    parser.add_argument(
        '--violations',
        nargs='+',
        help='Generate fixes only for specific violation IDs'
    )
    parser.add_argument(
        '--ai-mode',
        choices=['ai_only', 'hybrid', 'rules_only'],
        default='hybrid',
        help='AI mode for fix generation (default: hybrid)'
    )
    parser.add_argument(
        '--source-code',
        type=str,
        help='Path to source code directory for context-aware fixes'
    )
    parser.add_argument(
        '--interactive',
        action='store_true',
        help='Interactive mode - prompt for each violation'
    )
    
    args = parser.parse_args()
    
    # Generate fixes
    success = generate_code_fixes(
        module_name=args.module,
        specific_violations=args.violations,
        ai_mode=args.ai_mode,
        source_code_path=args.source_code,
        interactive=args.interactive
    )
    
    if success:
        print("\n[SUCCESS] Code fix generation completed!")
        print("\nNext steps:")
        print("  1. Review the generated fixes file")
        print("  2. Apply appropriate fixes to your source code")
        print("  3. Re-run analysis to verify fixes")
        print()
        return 0
    else:
        print("\n[ERROR] Code fix generation failed!")
        print("Please check the error messages above.")
        print()
        return 1


if __name__ == '__main__':
    sys.exit(main())
