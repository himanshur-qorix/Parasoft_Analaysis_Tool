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
        self.justifications_dir = self.workspace_path / "justifications"
        
        # Default Qorix deviations file path
        self.qorix_file = self.workspace_path / "data" / "Qorix_CP_Common_Deviations.xlsx"
        
        # Load configuration
        config_path = self.workspace_path / "config" / "config.json"
        if config_path.exists():
            with open(config_path, 'r') as f:
                self.config = json.load(f)
            logger.info("[OK] Configuration loaded")
        else:
            logger.warning("[WARNING] config.json not found, using defaults")
            self.config = {}
        
        # Create directories if they don't exist
        self.knowledge_base_dir.mkdir(exist_ok=True)
        self.reports_dir.mkdir(exist_ok=True)
        self.fixes_dir.mkdir(exist_ok=True)
        self.justifications_dir.mkdir(exist_ok=True)
        
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
                logger.info("[OK] Git repository detected")
                return True
            else:
                logger.warning("[WARNING] Not a Git repository. Consider initializing with 'git init'")
                return False
                
        except FileNotFoundError:
            logger.error("[ERROR] Git is not installed or not in PATH")
            return False
        except Exception as e:
            logger.error(f"[ERROR] Error checking Git: {str(e)}")
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
    
    def analyze_report(self, report_path, module_name, qorix_file=None):
        """
        Analyze a Parasoft report for a specific module
        
        Args:
            report_path: Path to the report_dev1.html file
            module_name: Name of the module (e.g., 'Mka')
            qorix_file: Optional path to Qorix deviations Excel file
        
        Returns:
            Analysis results dictionary
        """
        from KnowledgeDatabaseManager import KnowledgeDatabaseManager
        from ViolationAnalyzer import ViolationAnalyzer
        import pandas as pd
        from collections import Counter
        
        logger.info(f"Starting analysis for module: {module_name}")
        logger.info(f"Report: {report_path}")
        
        # Use provided Qorix file or default
        if qorix_file:
            self.qorix_file = Path(qorix_file)
        
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
        
        # Load justifiable mapping from Qorix file
        justifiable_mapping = {}
        if self.qorix_file.exists():
            from ParasoftAnalysisTool import load_justifiable_mapping
            justifiable_mapping = load_justifiable_mapping(self.qorix_file)
            logger.info(f"Loaded {len(justifiable_mapping)} justifiable mappings from Qorix file")
        else:
            logger.warning(f"Qorix file not found: {self.qorix_file}")
        
        # Apply justifiable status to violations
        violations_with_status = self._apply_justifiable_status(violations, justifiable_mapping)
        
        # Generate Excel report
        excel_path = self.reports_dir / f"{module_name}_violations_report.xlsx"
        self._generate_excel_report(violations_with_status, excel_path, module_name)
        logger.info(f"Excel report generated: {excel_path}")
        
        # Generate suppress comments for justified violations
        suppress_file = self._generate_suppress_comments(violations_with_status, module_name)
        if suppress_file:
            logger.info(f"Suppress comments generated: {suppress_file}")
        
        # Analyze violations and update knowledge base
        analysis_results = analyzer.analyze_violations(violations)
        
        # Save knowledge base
        kb_path = kb_manager.save_knowledge_base(module_name)
        logger.info(f"Knowledge base saved: {kb_path}")
        
        # Calculate status statistics
        status_counts = self._get_status_statistics(violations_with_status)
        
        return {
            'status': 'success',
            'module': module_name,
            'total_violations': len(violations),
            'new_unique_violations': analysis_results['new_unique_count'],
            'existing_violations': analysis_results['existing_count'],
            'knowledge_base_path': str(kb_path),
            'excel_report_path': str(excel_path),
            'suppress_file': suppress_file,
            'status_counts': status_counts,
            'timestamp': datetime.now().isoformat()
        }
    
    def _apply_justifiable_status(self, violations, justifiable_mapping):
        """
        Apply justifiable status to violations based on Qorix mapping
        
        Args:
            violations: List of violation dictionaries
            justifiable_mapping: Mapping from violation ID to justifiable status
        
        Returns:
            List of violations with status field added
        """
        from ParasoftAnalysisTool import resolve_justifiable
        
        for v in violations:
            justifiable = resolve_justifiable(v['Violation ID'], justifiable_mapping)
            
            # Map to final status
            if justifiable == "Yes":
                v['Status'] = "Justified"
            elif justifiable == "No":
                v['Status'] = "Needs Code Update"
            else:  # "Analyse"
                v['Status'] = "Analysis Required"
        
        return violations
    
    def _get_status_statistics(self, violations):
        """
        Get statistics on violation statuses
        
        Args:
            violations: List of violations with status
        
        Returns:
            Dictionary with status counts
        """
        from collections import Counter
        statuses = [v.get('Status', 'Unknown') for v in violations]
        return dict(Counter(statuses))
    
    def _generate_suppress_comments(self, violations, module_name):
        """
        Generate Parasoft suppress comments for justified violations
        
        Args:
            violations: List of violations with status
            module_name: Name of the module
        
        Returns:
            Path to generated suppress comments file or None
        """
        justified_violations = [v for v in violations if v.get('Status') == 'Justified']
        
        if not justified_violations:
            logger.info("No justified violations to suppress")
            return None
        
        # Group by file and line number
        from collections import defaultdict
        violations_by_location = defaultdict(list)
        
        for v in justified_violations:
            key = (v['File'], v['Line number'])
            violations_by_location[key].append(v['Violation ID'])
        
        # Generate suppress comments file
        timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
        output_path = self.justifications_dir / f"{module_name}_suppress_comments_{timestamp}.txt"
        
        with open(output_path, 'w', encoding='utf-8') as f:
            f.write(f"Parasoft Suppress Comments for {module_name}\n")
            f.write(f"Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
            f.write("="*80 + "\n\n")
            f.write("Instructions:\n")
            f.write("1. Add the 'parasoft-begin-suppress' comment BEFORE the line with the violation\n")
            f.write("2. Add the 'parasoft-end-suppress' comment AFTER the line with the violation\n")
            f.write("3. Update the reason reference as needed\n\n")
            f.write("="*80 + "\n\n")
            
            for (file, line), rule_ids in sorted(violations_by_location.items()):
                # Remove duplicates and sort
                unique_ids = sorted(set(rule_ids))
                ids_str = ' '.join(unique_ids)
                
                # Generate reason reference based on module and file
                reason_ref = f"{module_name}_Parasoft_REF_{line}"
                
                f.write(f"File: {file}, Line: {line}\n")
                f.write("-" * 80 + "\n")
                f.write(f"/* parasoft-begin-suppress {ids_str} \"Reason: {reason_ref}\" */\n")
                f.write(f"... (your code at line {line}) ...\n")
                f.write(f"/* parasoft-end-suppress {ids_str} */\n")
                f.write("\n")
        
        logger.info(f"Generated suppress comments for {len(violations_by_location)} locations")
        return str(output_path)
    
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
        columns = ["Violation", "Violation ID", "File", "Line number"]
        if 'Status' in df.columns:
            columns.append("Status")
        
        detailed_df = df[columns].sort_values(by=["File", "Line number"])
        
        # Create unique violations summary sheet
        counter = Counter(zip(df["Violation"], df["Violation ID"]))
        unique_df = pd.DataFrame(
            [{"Violation": v, "Violation ID": r, "Violation Count": c}
             for (v, r), c in counter.items()]
        ).sort_values(by="Violation Count", ascending=False)
        
        # Create summary statistics sheet
        stats_metrics = [
            'Total Violations',
            'Unique Violation Types',
            'Files Affected',
            'Module Name',
            'Analysis Date'
        ]
        stats_values = [
            len(violations),
            len(unique_df),
            df['File'].nunique(),
            module_name,
            datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        ]
        
        # Add status counts if available
        if 'Status' in df.columns:
            status_counts = df['Status'].value_counts().to_dict()
            for status, count in sorted(status_counts.items()):
                stats_metrics.append(f"{status} Count")
                stats_values.append(count)
        
        stats_data = {
            'Metric': stats_metrics,
            'Value': stats_values
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
        fix_generator = CodeFixGenerator(module_name, kb_manager, self.fixes_dir, self.config)
        
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
        fix_generator = CodeFixGenerator(module_name, kb_manager, self.fixes_dir, self.config)
        
        results = fix_generator.generate_justifications(violation_ids)
        
        logger.info(f"Generated {results['justifications_generated']} justifications")
        
        return results
    
    def run_misra_cert_checker(self, source_code_path, module_name, report_format='html'):
        """
        Run MISRA-C and CERT-C static analyzer on source code
        
        Args:
            source_code_path: Path to C/C++ source code directory or file
            module_name: Name of the module being analyzed
            report_format: Output format ('html', 'json', or 'text')
        
        Returns:
            Dictionary with report path and analysis status
        """
        logger.info("="*60)
        logger.info("RUNNING MISRA/CERT STATIC ANALYZER")
        logger.info(f"Source Code Path: {source_code_path}")
        logger.info(f"Module: {module_name}")
        logger.info("="*60)
        
        source_path = Path(source_code_path)
        
        # Validate source path exists
        if not source_path.exists():
            error_msg = f"Source code path does not exist: {source_code_path}"
            logger.error(error_msg)
            return {
                'status': 'error',
                'message': error_msg,
                'report_path': None
            }
        
        # Determine report output path
        timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
        report_filename = f"{module_name}_misra_cert_report_{timestamp}.{report_format}"
        report_path = self.reports_dir / report_filename
        
        # Path to misra_cert_checker.py script
        src_dir = self.workspace_path / "src"
        checker_script = src_dir / "misra_cert_checker.py"
        
        if not checker_script.exists():
            error_msg = f"MISRA/CERT checker script not found: {checker_script}"
            logger.error(error_msg)
            return {
                'status': 'error',
                'message': error_msg,
                'report_path': None
            }
        
        try:
            # Run the MISRA/CERT checker
            logger.info(f"Running MISRA/CERT checker on: {source_path}")
            
            cmd = [
                sys.executable,
                str(checker_script),
                str(source_path),
                '--report', report_format,
                '--output', str(report_path)
            ]
            
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=300  # 5 minute timeout
            )
            
            if result.returncode == 0:
                logger.info(f"[OK] MISRA/CERT analysis completed successfully")
                logger.info(f"[OK] Report saved: {report_path}")
                
                # Extract violation count from stderr output
                violation_count = 0
                for line in result.stderr.split('\n'):
                    if 'found' in line and 'violation(s)' in line:
                        try:
                            parts = line.split('found')
                            if len(parts) > 1:
                                violation_count = int(parts[1].split('violation(s)')[0].strip())
                        except:
                            pass
                
                return {
                    'status': 'success',
                    'message': 'MISRA/CERT analysis completed',
                    'report_path': str(report_path),
                    'violations_found': violation_count,
                    'timestamp': datetime.now().isoformat()
                }
            else:
                error_msg = f"MISRA/CERT checker failed with return code {result.returncode}"
                logger.error(error_msg)
                if result.stderr:
                    logger.error(f"Error output: {result.stderr}")
                
                return {
                    'status': 'error',
                    'message': error_msg,
                    'error_output': result.stderr,
                    'report_path': None
                }
                
        except subprocess.TimeoutExpired:
            error_msg = "MISRA/CERT checker timed out after 5 minutes"
            logger.error(error_msg)
            return {
                'status': 'error',
                'message': error_msg,
                'report_path': None
            }
        except Exception as e:
            error_msg = f"Error running MISRA/CERT checker: {str(e)}"
            logger.error(error_msg, exc_info=True)
            return {
                'status': 'error',
                'message': error_msg,
                'report_path': None
            }
    
    def run_full_analysis(self, report_path, module_name, generate_fixes=True, add_justifications=True, qorix_file=None, source_code_path=None):
        """
        Run full analysis pipeline for a module
        
        Args:
            report_path: Path to the report_dev1.html file
            module_name: Name of the module
            generate_fixes: Whether to generate code fixes
            add_justifications: Whether to add justifications
            qorix_file: Optional path to Qorix deviations file
            source_code_path: Optional path to source code for MISRA/CERT pre-analysis
        
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
        
        # Run MISRA/CERT checker if source code path is provided
        misra_cert_results = None
        if source_code_path:
            run_checker = self.config.get('analysis', {}).get('run_misra_cert_checker', True)
            if run_checker:
                report_format = self.config.get('analysis', {}).get('misra_cert_report_format', 'html')
                misra_cert_results = self.run_misra_cert_checker(source_code_path, module_name, report_format)
                
                if misra_cert_results['status'] == 'error':
                    logger.warning(f"MISRA/CERT checker failed: {misra_cert_results.get('message', 'Unknown error')}")
                    logger.warning("Continuing with Parasoft analysis...")
                else:
                    logger.info(f"[OK] MISRA/CERT pre-analysis completed: {misra_cert_results.get('violations_found', 0)} violations found")
        else:
            logger.info("No source code path provided - skipping MISRA/CERT pre-analysis")
        
        # Analyze report
        analysis_results = self.analyze_report(report_path, module_name, qorix_file)
        
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
            'misra_cert_analysis': misra_cert_results,
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
        print("  --qorix <path>      Specify Qorix deviations file path")
        print("  --source-code <path> Specify source code path for MISRA/CERT pre-analysis")
        print("  --ai-mode <mode>    AI mode: ai_only | hybrid | rules_only (default: hybrid)")
        print("\nAI Modes:")
        print("  ai_only     - Use Ollama AI for all violations (requires Ollama running)")
        print("  hybrid      - Smart: AI for complex, rules for standard (recommended)")
        print("  rules_only  - Use only rule-based fixes (no AI required)")
        print("\nExample:")
        print("  python ParasoftAIAgent.py report_dev1.html Mka")
        print("  python ParasoftAIAgent.py report_dev1.html Mka --workspace D:/MyProject")
        print("  python ParasoftAIAgent.py report_dev1.html Mka --qorix data/Qorix.xlsx")
        print("  python ParasoftAIAgent.py report_dev1.html Mka --source-code D:/MyProject/src")
        print("  python ParasoftAIAgent.py report_dev1.html Mka --ai-mode hybrid")
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
    
    qorix_file = None
    if '--qorix' in sys.argv:
        idx = sys.argv.index('--qorix')
        if idx + 1 < len(sys.argv):
            qorix_file = sys.argv[idx + 1]
    
    source_code_path = None
    if '--source-code' in sys.argv:
        idx = sys.argv.index('--source-code')
        if idx + 1 < len(sys.argv):
            source_code_path = sys.argv[idx + 1]
            print(f"[INFO] Source Code Path: {source_code_path}")
    
    ai_mode = 'hybrid'  # Default mode
    if '--ai-mode' in sys.argv:
        idx = sys.argv.index('--ai-mode')
        if idx + 1 < len(sys.argv):
            mode = sys.argv[idx + 1].lower()
            if mode in ['ai_only', 'hybrid', 'rules_only']:
                ai_mode = mode
                print(f"[INFO] AI Mode: {ai_mode}")
            else:
                print(f"[WARNING] Invalid AI mode '{mode}'. Using default: hybrid")
    else:
        print(f"[INFO] AI Mode: {ai_mode} (default)")
    
    # Validate report exists
    if not report_path.exists():
        print(f"[ERROR] Report file not found: {report_path}")
        sys.exit(1)
    
    # Initialize agent
    agent = ParasoftAIAgent(workspace_path)
    
    # Set AI mode in agent config
    if not hasattr(agent, 'config') or agent.config is None:
        agent.config = {}
    if 'ai_integration' not in agent.config:
        agent.config['ai_integration'] = {}
    agent.config['ai_integration']['ai_mode'] = ai_mode
    
    # Run full analysis
    try:
        results = agent.run_full_analysis(
            report_path,
            module_name,
            generate_fixes=generate_fixes,
            add_justifications=add_justifications,
            qorix_file=qorix_file,
            source_code_path=source_code_path
        )
        
        print(f"\n[SUCCESS] Analysis completed successfully!")
        
        # Display MISRA/CERT pre-analysis results if available
        if results.get('misra_cert_analysis'):
            misra_cert = results['misra_cert_analysis']
            if misra_cert.get('status') == 'success':
                print(f"\n  MISRA/CERT Pre-Analysis:")
                print(f"    Violations found: {misra_cert.get('violations_found', 0)}")
                print(f"    Report: {misra_cert.get('report_path', 'N/A')}")
        
        print(f"\n  Parasoft Analysis:")
        print(f"  Total violations: {results['analysis'].get('total_violations', 0)}")
        print(f"  New unique violations: {results['analysis'].get('new_unique_violations', 0)}")
        print(f"  Knowledge base: {results['analysis'].get('knowledge_base_path', 'N/A')}")
        print(f"  Excel report: {results['analysis'].get('excel_report_path', 'N/A')}")
        
        # Display status statistics
        if 'status_counts' in results['analysis']:
            print("\n  Violation Status Breakdown:")
            for status, count in results['analysis']['status_counts'].items():
                print(f"    {status}: {count}")
        
        # Display suppress file if generated
        if results['analysis'].get('suppress_file'):
            print(f"  Suppress comments: {results['analysis']['suppress_file']}")
        
        if results.get('fixes'):
            print(f"  Fixes generated: {results['fixes'].get('fixes_generated', 0)}")
        
        if results.get('justifications'):
            print(f"  Justifications: {results['justifications'].get('justifications_generated', 0)}")
        
        print(f"\n[SUCCESS] All results saved in the workspace")
        
    except Exception as e:
        logger.error(f"[ERROR] Analysis failed: {str(e)}", exc_info=True)
        print(f"\n[ERROR] {str(e)}")
        sys.exit(1)


if __name__ == "__main__":
    main()
