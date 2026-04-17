"""
Static Code Analysis Agent
Main entry point for static code analysis

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 3.0.0
"""

import sys
import logging
import argparse
import json
from pathlib import Path
from datetime import datetime

# Add parent directory to path for imports
sys.path.insert(0, str(Path(__file__).parent))

from StaticCodeAnalyzer import StaticCodeAnalyzer
from KnowledgeDatabaseManager import KnowledgeDatabaseManager
from StaticAnalysisReportGenerator import StaticAnalysisReportGenerator
from ViolationHistoryManager import ViolationHistoryManager
from ComparisonReportGenerator import ComparisonReportGenerator

# Setup logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    handlers=[
        logging.StreamHandler(sys.stdout),
        logging.FileHandler('static_analysis.log', mode='a')
    ]
)
logger = logging.getLogger(__name__)


def main():
    """Main entry point for static code analysis"""
    
    parser = argparse.ArgumentParser(
        description='Static Code Analysis Tool v3.0.0',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''
Examples:
  python run_static_analyzer.py D:\\Code\\Mka Mka
  python run_static_analyzer.py D:\\Projects\\Eep Eep --no-fixes
  python run_static_analyzer.py D:\\Code\\Can Can --file-patterns *.c *.h
        '''
    )
    
    parser.add_argument(
        'source_dir',
        type=str,
        help='Path to source code directory'
    )
    
    parser.add_argument(
        'module_name',
        type=str,
        help='Module name (e.g., Mka, Eep, Can)'
    )
    
    parser.add_argument(
        '--file-patterns',
        nargs='+',
        help='File patterns to analyze (default: *.c *.cpp *.h *.hpp)',
        default=None
    )
    
    parser.add_argument(
        '--no-fixes',
        action='store_true',
        help='Skip automatic fix generation (only analyze and create KB)'
    )
    
    parser.add_argument(
        '--output-dir',
        type=str,
        help='Output directory for reports (default: project reports folder)',
        default=None
    )
    
    args = parser.parse_args()
    
    # Setup paths
    project_root = Path(__file__).parent.parent
    reports_dir = Path(args.output_dir) if args.output_dir else project_root / 'reports'
    kb_dir = project_root / 'knowledge_base'
    data_dir = project_root / 'data'
    history_dir = project_root / 'history'  # NEW: History tracking
    
    # Create directories
    reports_dir.mkdir(exist_ok=True)
    kb_dir.mkdir(exist_ok=True)
    data_dir.mkdir(exist_ok=True)
    history_dir.mkdir(exist_ok=True)  # NEW: Create history directory
    
    logger.info("="*80)
    logger.info("Static Code Analysis Tool v3.0.0")
    logger.info("Qorix India Pvt Ltd")
    logger.info("="*80)
    logger.info(f"Module: {args.module_name}")
    logger.info(f"Source Code: {args.source_dir}")
    logger.info("="*80)
    
    try:
        # Validate source directory
        source_path = Path(args.source_dir)
        if not source_path.exists():
            logger.error(f"Source directory not found: {source_path}")
            return 1
        
        if not source_path.is_dir():
            logger.error(f"Path is not a directory: {source_path}")
            return 1
        
        # Step 1: Run static analysis
        logger.info("STEP 1: Running static code analysis...")
        analyzer = StaticCodeAnalyzer()
        violations = analyzer.analyze_directory(source_path, args.file_patterns)
        
        if not violations:
            logger.warning("No violations found")
            logger.info("This could mean:")
            logger.info("  - Code is very clean")
            logger.info("  - No source files found matching patterns")
            logger.info("  - File patterns need adjustment")
            return 0
        
        logger.info(f"Found {len(violations)} potential issues")
        
        # Display summary
        summary = analyzer.get_summary()
        logger.info("="*80)
        logger.info("Static Analysis Summary:")
        logger.info(f"  Files analyzed: {summary['files_analyzed']}")
        logger.info(f"  Lines analyzed: {summary['lines_analyzed']}")
        logger.info(f"  Total findings: {summary['total_findings']}")
        logger.info(f"  [CRITICAL]: {summary['critical']}")
        logger.info(f"  [HIGH]: {summary['high']}")
        logger.info(f"  [MEDIUM]: {summary['medium']}")
        logger.info(f"  [LOW]: {summary['low']}")
        logger.info("="*80)
        
        # Step 2: Save violations to JSON
        logger.info("STEP 2: Saving violations data...")
        violations_file = data_dir / f"{args.module_name}_violations.json"
        with open(violations_file, 'w', encoding='utf-8') as f:
            json.dump({
                'module_name': args.module_name,
                'tool': 'StaticAnalyzer',
                'source_directory': str(source_path),
                'timestamp': datetime.now().isoformat(),
                'summary': summary,
                'violations': violations
            }, f, indent=2)
        logger.info(f"Saved violations to: {violations_file}")
        
        # Step 3: Create/Update Knowledge Base
        logger.info("STEP 3: Creating knowledge base...")
        kb_manager = KnowledgeDatabaseManager(kb_dir)
        kb_manager.load_knowledge_base(args.module_name)
        
        # Convert violations to KB format
        for violation in violations:
            kb_data = {
                'Violation ID': violation['violation_id'],
                'Violation': violation['violation_text'],
                'File': violation['file'],
                'Line number': violation['line'],
                'Justifiable': 'Analyse'
            }
            kb_manager.add_violation(kb_data)
        
        kb_file = kb_manager.save_knowledge_base(args.module_name)
        logger.info(f"Knowledge base saved: {kb_file}")
        
        # Step 4: Generate Reports (HTML and Excel)
        logger.info("STEP 4: Generating analysis reports...")
        
        # Create report generator
        report_gen = StaticAnalysisReportGenerator(args.module_name, violations, summary)
        
        # Generate HTML report
        html_report = reports_dir / f"{args.module_name}_static_analysis_report.html"
        report_gen.generate_html_report(html_report)
        logger.info(f"HTML report saved: {html_report}")
        
        # Generate Excel report
        excel_report = reports_dir / f"{args.module_name}_violations_report.xlsx"
        report_gen.generate_excel_report(excel_report)
        logger.info(f"Excel report saved: {excel_report}")
        
        # Step 5: Record History and Generate Comparison Reports
        logger.info("STEP 5: Recording history and generating comparison reports...")
        
        # Initialize history manager
        history_manager = ViolationHistoryManager(history_dir)
        
        # Record this analysis run
        history_manager.record_analysis_run(
            module_name=args.module_name,
            violations=violations,
            summary=summary,
            source_code_path=str(source_path)
        )
        logger.info(f"Analysis run recorded in history")
        
        # Generate module history report
        module_history_report = reports_dir / f"{args.module_name}_history_report.html"
        comparison_gen = ComparisonReportGenerator(history_manager)
        comparison_gen.generate_module_comparison_report(args.module_name, module_history_report)
        logger.info(f"History report generated: {module_history_report}")
        
        # Generate cross-module comparison (all modules)
        all_modules = list(history_manager.master_history["modules"].keys())
        if len(all_modules) > 1:
            cross_module_report = reports_dir / "cross_module_comparison.html"
            comparison_gen.generate_cross_module_report(all_modules, cross_module_report)
            logger.info(f"Cross-module comparison generated: {cross_module_report}")
        
        # Step 6: Summary
        logger.info("="*80)
        logger.info("SUCCESS: Static code analysis completed successfully!")
        logger.info("="*80)
        logger.info("Output files:")
        logger.info(f"  [HTML Report]: {html_report}")
        logger.info(f"  [Excel Report]: {excel_report}")
        logger.info(f"  [Knowledge Base]: {kb_file}")
        logger.info(f"  [Violations JSON]: {violations_file}")
        logger.info(f"  [Module History Report]: {module_history_report}")
        if len(all_modules) > 1:
            logger.info(f"  [Cross-Module Comparison]: {cross_module_report}")
        logger.info("="*80)
        logger.info("")
        logger.info("Color Code Summary:")
        logger.info(f"  RED (Critical):    {summary['critical']} - Proven errors (null deref, div by zero)")
        logger.info(f"  ORANGE (High):     {summary['high']} - Likely errors (uninitialized, buffer overflow)")
        logger.info(f"  GREY (Quality):    {summary['medium'] + summary['low']} - Code quality issues (MISRA, CERT, style)")
        logger.info("="*80)
        
        if not args.no_fixes:
            logger.info("Next step: Generate code fixes using Generate_Code_Fixes.bat")
        
        return 0
        
    except Exception as e:
        logger.error(f"[ERROR] Analysis failed: {e}", exc_info=True)
        return 1


if __name__ == '__main__':
    sys.exit(main())
