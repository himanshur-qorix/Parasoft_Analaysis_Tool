"""
Parasoft AI Agent - Main Entry Point
Version: 2.0.0
Developer: Himanshu R
Description: AI-powered agent for Parasoft analysis with knowledge database management
"""

import sys
import os
import json
import logging
from pathlib import Path
from datetime import datetime
import subprocess

# Add src directory to Python path for imports
src_dir = Path(__file__).parent
if str(src_dir) not in sys.path:
    sys.path.insert(0, str(src_dir))

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler('parasoft_agent.log'),
        logging.StreamHandler()
    ]
)
logger = logging.getLogger(__name__)


class ParasoftAIAgent:
    """Main AI Agent for Parasoft Analysis"""
    
    def __init__(self, workspace_path=None):
        """
        Initialize the Parasoft AI Agent
        
        Args:
            workspace_path: Path to the workspace/repository
        """
        # If workspace_path is not specified, use parent of src directory (project root)
        if workspace_path:
            self.workspace_path = Path(workspace_path)
        else:
            # Get project root (parent of src directory)
            self.workspace_path = Path(__file__).parent.parent
        
        self.knowledge_base_dir = self.workspace_path / "knowledge_base"
        self.reports_dir = self.workspace_path / "reports"
        self.fixes_dir = self.workspace_path / "fixes"
        
        # Create directories if they don't exist
        self.knowledge_base_dir.mkdir(exist_ok=True)
        self.reports_dir.mkdir(exist_ok=True)
        self.fixes_dir.mkdir(exist_ok=True)
        
        logger.info(f"Parasoft AI Agent initialized at: {self.workspace_path}")
    
    def check_git_integration(self):
        """Check if the workspace is a Git repository"""
        try:
            result = subprocess.run(
                ['git', 'rev-parse', '--git-dir'],
                cwd=self.workspace_path,
                capture_output=True,
                text=True,
                timeout=5
            )
            
            if result.returncode == 0:
                logger.info("✓ Git repository detected")
                return True
            else:
                logger.warning("⚠ Not a Git repository. Consider initializing with 'git init'")
                return False
                
        except FileNotFoundError:
            logger.error("✗ Git is not installed or not in PATH")
            return False
        except Exception as e:
            logger.error(f"✗ Error checking Git: {str(e)}")
            return False
    
    def get_git_info(self):
        """Get current Git branch and commit info"""
        git_info = {}
        
        try:
            # Get current branch
            result = subprocess.run(
                ['git', 'branch', '--show-current'],
                cwd=self.workspace_path,
                capture_output=True,
                text=True,
                timeout=5
            )
            git_info['branch'] = result.stdout.strip() if result.returncode == 0 else 'unknown'
            
            # Get latest commit
            result = subprocess.run(
                ['git', 'rev-parse', '--short', 'HEAD'],
                cwd=self.workspace_path,
                capture_output=True,
                text=True,
                timeout=5
            )
            git_info['commit'] = result.stdout.strip() if result.returncode == 0 else 'unknown'
            
            logger.info(f"Git Info - Branch: {git_info['branch']}, Commit: {git_info['commit']}")
            
        except Exception as e:
            logger.error(f"Error getting Git info: {str(e)}")
            git_info = {'branch': 'unknown', 'commit': 'unknown'}
        
        return git_info
    
    def analyze_report(self, report_path, module_name):
        """
        Analyze a Parasoft report for a specific module
        
        Args:
            report_path: Path to the report_dev1.html file
            module_name: Name of the module (e.g., 'Mka')
        
        Returns:
            Analysis results dictionary
        """
        from KnowledgeDatabaseManager import KnowledgeDatabaseManager
        from ViolationAnalyzer import ViolationAnalyzer
        import pandas as pd
        from collections import Counter
        
        logger.info(f"Starting analysis for module: {module_name}")
        logger.info(f"Report: {report_path}")
        
        # Initialize managers
        kb_manager = KnowledgeDatabaseManager(self.knowledge_base_dir)
        analyzer = ViolationAnalyzer(module_name, kb_manager)
        
        # Parse the report
        from ParasoftAnalysisTool import parse_parasoft_report
        violations = parse_parasoft_report(report_path)
        
        if not violations:
            logger.warning("No violations found in the report")
            return {'status': 'no_violations', 'module': module_name}
        
        logger.info(f"Found {len(violations)} total violations")
        
        # Generate Excel report
        excel_path = self.reports_dir / f"{module_name}_violations_report.xlsx"
        self._generate_excel_report(violations, excel_path, module_name)
        logger.info(f"Excel report generated: {excel_path}")
        
        # Analyze violations and update knowledge base
        analysis_results = analyzer.analyze_violations(violations)
        
        # Save knowledge base
        kb_path = kb_manager.save_knowledge_base(module_name)
        logger.info(f"Knowledge base saved: {kb_path}")
        
        return {
            'status': 'success',
            'module': module_name,
            'total_violations': len(violations),
            'new_unique_violations': analysis_results['new_unique_count'],
            'existing_violations': analysis_results['existing_count'],
            'knowledge_base_path': str(kb_path),
            'excel_report_path': str(excel_path),
            'timestamp': datetime.now().isoformat()
        }
    
    def _generate_excel_report(self, violations, output_path, module_name):
        """
        Generate an Excel report from violations data
        
        Args:
            violations: List of violation dictionaries
            output_path: Path to save the Excel file
            module_name: Name of the module
        """
        import pandas as pd
        from collections import Counter
        
        df = pd.DataFrame(violations)
        
        # Create detailed violations sheet (sorted by file and line number)
        detailed_df = df[["Violation", "Violation ID", "File", "Line number"]].sort_values(
            by=["File", "Line number"]
        )
        
        # Create unique violations summary sheet
        counter = Counter(zip(df["Violation"], df["Violation ID"]))
        unique_df = pd.DataFrame(
            [{"Violation": v, "Violation ID": r, "Violation Count": c}
             for (v, r), c in counter.items()]
        ).sort_values(by="Violation Count", ascending=False)
        
        # Create summary statistics sheet
        stats_data = {
            'Metric': [
                'Total Violations',
                'Unique Violation Types',
                'Files Affected',
                'Module Name',
                'Analysis Date'
            ],
            'Value': [
                len(violations),
                len(unique_df),
                df['File'].nunique(),
                module_name,
                datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            ]
        }
        stats_df = pd.DataFrame(stats_data)
        
        # Write to Excel with multiple sheets
        with pd.ExcelWriter(output_path, engine="openpyxl") as writer:
            stats_df.to_excel(writer, sheet_name="Summary", index=False)
            unique_df.to_excel(writer, sheet_name="Unique Violations", index=False)
            detailed_df.to_excel(writer, sheet_name="Detailed Violations", index=False)
        
        logger.info(f"Excel report created with {len(violations)} violations across {df['File'].nunique()} files")
    
    def generate_fixes(self, module_name, violation_ids=None):
        """
        Generate fixes for violations in a module
        
        Args:
            module_name: Name of the module
            violation_ids: List of specific violation IDs to fix (None = all)
        
        Returns:
            Fix generation results
        """
        from CodeFixGenerator import CodeFixGenerator
        from KnowledgeDatabaseManager import KnowledgeDatabaseManager
        
        logger.info(f"Generating fixes for module: {module_name}")
        
        kb_manager = KnowledgeDatabaseManager(self.knowledge_base_dir)
        fix_generator = CodeFixGenerator(module_name, kb_manager, self.fixes_dir)
        
        results = fix_generator.generate_all_fixes(violation_ids)
        
        logger.info(f"Generated {results['fixes_generated']} fixes")
        
        return results
    
    def add_justifications(self, module_name, violation_ids=None):
        """
        Add Parasoft-formatted justifications for violations
        
        Args:
            module_name: Name of the module
            violation_ids: List of specific violation IDs to justify (None = all)
        
        Returns:
            Justification results
        """
        from CodeFixGenerator import CodeFixGenerator
        from KnowledgeDatabaseManager import KnowledgeDatabaseManager
        
        logger.info(f"Adding justifications for module: {module_name}")
        
        kb_manager = KnowledgeDatabaseManager(self.knowledge_base_dir)
        fix_generator = CodeFixGenerator(module_name, kb_manager, self.fixes_dir)
        
        results = fix_generator.generate_justifications(violation_ids)
        
        logger.info(f"Generated {results['justifications_generated']} justifications")
        
        return results
    
    def run_full_analysis(self, report_path, module_name, generate_fixes=True, add_justifications=True):
        """
        Run full analysis pipeline for a module
        
        Args:
            report_path: Path to the report_dev1.html file
            module_name: Name of the module
            generate_fixes: Whether to generate code fixes
            add_justifications: Whether to add justifications
        
        Returns:
            Complete analysis results
        """
        logger.info("="*60)
        logger.info(f"PARASOFT AI AGENT - FULL ANALYSIS")
        logger.info(f"Module: {module_name}")
        logger.info("="*60)
        
        # Check Git integration
        git_integrated = self.check_git_integration()
        git_info = self.get_git_info() if git_integrated else {}
        
        # Analyze report
        analysis_results = self.analyze_report(report_path, module_name)
        
        fix_results = None
        just_results = None
        
        if analysis_results['status'] == 'success':
            # Generate fixes if requested
            if generate_fixes:
                fix_results = self.generate_fixes(module_name)
            
            # Add justifications if requested
            if add_justifications:
                just_results = self.add_justifications(module_name)
        
        # Combine results
        full_results = {
            'git_info': git_info,
            'analysis': analysis_results,
            'fixes': fix_results,
            'justifications': just_results,
            'timestamp': datetime.now().isoformat()
        }
        
        # Save results summary
        summary_path = self.reports_dir / f"{module_name}_analysis_summary.json"
        with open(summary_path, 'w', encoding='utf-8') as f:
            json.dump(full_results, f, indent=2)
        
        logger.info(f"Analysis summary saved: {summary_path}")
        logger.info("="*60)
        logger.info("ANALYSIS COMPLETED SUCCESSFULLY")
        logger.info("="*60)
        
        return full_results


def main():
    """Main entry point for the Parasoft AI Agent"""
    
    print("\n" + "="*60)
    print("     PARASOFT AI AGENT - Version 2.0.0")
    print("="*60 + "\n")
    
    if len(sys.argv) < 3:
        print("Usage: python ParasoftAIAgent.py <report_path> <module_name> [options]")
        print("\nOptions:")
        print("  --no-fixes          Skip code fix generation")
        print("  --no-justifications Skip justification generation")
        print("  --workspace <path>  Specify workspace path")
        print("\nExample:")
        print("  python ParasoftAIAgent.py report_dev1.html Mka")
        print("  python ParasoftAIAgent.py report_dev1.html Mka --workspace D:/MyProject")
        sys.exit(1)
    
    report_path = Path(sys.argv[1])
    module_name = sys.argv[2]
    
    # Parse options
    generate_fixes = '--no-fixes' not in sys.argv
    add_justifications = '--no-justifications' not in sys.argv
    
    workspace_path = None
    if '--workspace' in sys.argv:
        idx = sys.argv.index('--workspace')
        if idx + 1 < len(sys.argv):
            workspace_path = sys.argv[idx + 1]
    
    # Validate report exists
    if not report_path.exists():
        print(f"✗ Error: Report file not found: {report_path}")
        sys.exit(1)
    
    # Initialize agent
    agent = ParasoftAIAgent(workspace_path)
    
    # Run full analysis
    try:
        results = agent.run_full_analysis(
            report_path,
            module_name,
            generate_fixes=generate_fixes,
            add_justifications=add_justifications
        )
        
        print(f"\n✓ Analysis completed successfully!")
        print(f"  Total violations: {results['analysis'].get('total_violations', 0)}")
        print(f"  New unique violations: {results['analysis'].get('new_unique_violations', 0)}")
        print(f"  Knowledge base: {results['analysis'].get('knowledge_base_path', 'N/A')}")
        print(f"  Excel report: {results['analysis'].get('excel_report_path', 'N/A')}")
        
        if results.get('fixes'):
            print(f"  Fixes generated: {results['fixes'].get('fixes_generated', 0)}")
        
        if results.get('justifications'):
            print(f"  Justifications: {results['justifications'].get('justifications_generated', 0)}")
        
        print(f"\n✓ All results saved in the workspace")
        
    except Exception as e:
        logger.error(f"✗ Analysis failed: {str(e)}", exc_info=True)
        print(f"\n✗ Error: {str(e)}")
        sys.exit(1)


if __name__ == "__main__":
    main()
