"""
Generate Code Fixes Script
Generates code fix suggestions for violations using Parasoft Rules Database
Can regenerate fixes without re-running full analysis

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 2.3.0
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


def generate_code_fixes(module_name: str, specific_violations: list = None, ai_mode: str = 'hybrid'):
    """
    Generate code fix suggestions for violations
    
    Args:
        module_name: Module name (e.g., "Mka")
        specific_violations: Optional list of specific violation IDs to fix
        ai_mode: AI mode ('ai_only', 'hybrid', 'rules_only')
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
    
    # Initialize directories
    fixes_dir = workspace_path / 'fixes'
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
    fix_generator = CodeFixGenerator(module_name, kb_manager, fixes_dir, config)
    
    # Generate fixes
    print()
    if specific_violations:
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
    
    args = parser.parse_args()
    
    # Generate fixes
    success = generate_code_fixes(
        module_name=args.module,
        specific_violations=args.violations,
        ai_mode=args.ai_mode
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
