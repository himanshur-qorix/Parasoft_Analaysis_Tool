"""
Knowledge Database Query Tool
Tool to capture and analyze all issues from the knowledge database
"""

import sys
import json
import logging
from pathlib import Path
from typing import Dict, List, Optional
from datetime import datetime
import pandas as pd

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s'
)
logger = logging.getLogger(__name__)


class KnowledgeDatabaseQueryTool:
    """Query tool for analyzing knowledge database"""
    
    def __init__(self, knowledge_base_dir: Path):
        """
        Initialize the Query Tool
        
        Args:
            knowledge_base_dir: Directory containing knowledge databases
        """
        self.knowledge_base_dir = Path(knowledge_base_dir)
        
        if not self.knowledge_base_dir.exists():
            raise ValueError(f"Knowledge base directory not found: {self.knowledge_base_dir}")
        
        self.databases = {}
        self._load_all_databases()
        
        logger.info(f"Query Tool initialized with {len(self.databases)} databases")
    
    def _load_all_databases(self):
        """Load all knowledge databases from the directory"""
        
        for db_file in self.knowledge_base_dir.glob("*_KnowledgeDatabase.json"):
            try:
                module_name = db_file.stem.replace("_KnowledgeDatabase", "")
                
                with open(db_file, 'r', encoding='utf-8') as f:
                    self.databases[module_name] = json.load(f)
                
                logger.info(f"Loaded database for module: {module_name}")
                
            except Exception as e:
                logger.error(f"Error loading {db_file}: {str(e)}")
    
    def list_modules(self) -> List[str]:
        """Get list of all modules"""
        return list(self.databases.keys())
    
    def get_module_summary(self, module_name: str) -> Optional[Dict]:
        """
        Get summary for a specific module
        
        Args:
            module_name: Name of the module
        
        Returns:
            Summary dictionary
        """
        if module_name not in self.databases:
            logger.warning(f"Module not found: {module_name}")
            return None
        
        db = self.databases[module_name]
        
        return {
            'module_name': module_name,
            'total_unique_violations': db.get('total_unique_violations', 0),
            'total_analyses': db['statistics'].get('total_analyses', 0),
            'total_violations_processed': db['statistics'].get('total_violations_processed', 0),
            'created_date': db.get('created_date'),
            'last_updated': db.get('last_updated'),
            'violations': len(db.get('violations', []))
        }
    
    def get_all_violations(self, module_name: Optional[str] = None) -> List[Dict]:
        """
        Get all violations, optionally for a specific module
        
        Args:
            module_name: Optional module name filter
        
        Returns:
            List of all violations
        """
        all_violations = []
        
        modules_to_query = [module_name] if module_name else self.databases.keys()
        
        for mod in modules_to_query:
            if mod not in self.databases:
                continue
            
            db = self.databases[mod]
            for violation in db.get('violations', {}).values():
                violation_copy = violation.copy()
                violation_copy['module'] = mod
                all_violations.append(violation_copy)
        
        return all_violations
    
    def query_by_severity(self, severity: str, module_name: Optional[str] = None) -> List[Dict]:
        """
        Query violations by severity
        
        Args:
            severity: Severity level (HIGH, MEDIUM, LOW)
            module_name: Optional module name filter
        
        Returns:
            List of matching violations
        """
        all_violations = self.get_all_violations(module_name)
        return [v for v in all_violations if v.get('severity') == severity]
    
    def query_by_category(self, category: str, module_name: Optional[str] = None) -> List[Dict]:
        """
        Query violations by category
        
        Args:
            category: Category (CERT, MISRA, CWE, OTHER)
            module_name: Optional module name filter
        
        Returns:
            List of matching violations
        """
        all_violations = self.get_all_violations(module_name)
        return [v for v in all_violations if v.get('category') == category]
    
    def query_unfixed_violations(self, module_name: Optional[str] = None) -> List[Dict]:
        """
        Get all violations that haven't been fixed
        
        Args:
            module_name: Optional module name filter
        
        Returns:
            List of unfixed violations
        """
        all_violations = self.get_all_violations(module_name)
        return [v for v in all_violations if not v.get('fix_applied')]
    
    def query_unjustified_violations(self, module_name: Optional[str] = None) -> List[Dict]:
        """
        Get all violations without justifications
        
        Args:
            module_name: Optional module name filter
        
        Returns:
            List of unjustified violations
        """
        all_violations = self.get_all_violations(module_name)
        return [v for v in all_violations if not v.get('justification_added')]
    
    def query_by_file(self, file_name: str, module_name: Optional[str] = None) -> List[Dict]:
        """
        Query violations affecting a specific file
        
        Args:
            file_name: Name of the file
            module_name: Optional module name filter
        
        Returns:
            List of violations affecting the file
        """
        all_violations = self.get_all_violations(module_name)
        
        matching_violations = []
        for violation in all_violations:
            files_affected = violation.get('files_affected', [])
            if any(file_name in file_entry.get('file', '') for file_entry in files_affected):
                matching_violations.append(violation)
        
        return matching_violations
    
    def query_top_violations(self, limit: int = 10, module_name: Optional[str] = None) -> List[Dict]:
        """
        Get top violations by occurrence count
        
        Args:
            limit: Number of top violations to return
            module_name: Optional module name filter
        
        Returns:
            List of top violations
        """
        all_violations = self.get_all_violations(module_name)
        sorted_violations = sorted(
            all_violations,
            key=lambda x: x.get('occurrence_count', 0),
            reverse=True
        )
        return sorted_violations[:limit]
    
    def generate_summary_report(self, output_file: Optional[Path] = None) -> Dict:
        """
        Generate comprehensive summary report
        
        Args:
            output_file: Optional path to save report
        
        Returns:
            Summary report dictionary
        """
        all_violations = self.get_all_violations()
        
        # Overall statistics
        total_violations = len(all_violations)
        total_unique = sum(db.get('total_unique_violations', 0) for db in self.databases.values())
        
        # Severity breakdown
        severity_counts = {
            'HIGH': len([v for v in all_violations if v.get('severity') == 'HIGH']),
            'MEDIUM': len([v for v in all_violations if v.get('severity') == 'MEDIUM']),
            'LOW': len([v for v in all_violations if v.get('severity') == 'LOW'])
        }
        
        # Category breakdown
        category_counts = {
            'CERT': len([v for v in all_violations if v.get('category') == 'CERT']),
            'MISRA': len([v for v in all_violations if v.get('category') == 'MISRA']),
            'CWE': len([v for v in all_violations if v.get('category') == 'CWE']),
            'OTHER': len([v for v in all_violations if v.get('category') == 'OTHER'])
        }
        
        # Fix status
        fixed_count = len([v for v in all_violations if v.get('fix_applied')])
        unfixed_count = total_violations - fixed_count
        
        # Justification status
        justified_count = len([v for v in all_violations if v.get('justification_added')])
        unjustified_count = total_violations - justified_count
        
        # Module breakdown
        module_stats = []
        for module_name in self.databases.keys():
            module_summary = self.get_module_summary(module_name)
            module_stats.append(module_summary)
        
        # Top violations
        top_violations = self.query_top_violations(10)
        
        report = {
            'generated_at': datetime.now().isoformat(),
            'total_modules': len(self.databases),
            'total_unique_violations': total_unique,
            'total_violations_all_modules': total_violations,
            'severity_breakdown': severity_counts,
            'category_breakdown': category_counts,
            'fix_status': {
                'fixed': fixed_count,
                'unfixed': unfixed_count,
                'percentage_fixed': round((fixed_count / total_violations * 100) if total_violations > 0 else 0, 2)
            },
            'justification_status': {
                'justified': justified_count,
                'unjustified': unjustified_count,
                'percentage_justified': round((justified_count / total_violations * 100) if total_violations > 0 else 0, 2)
            },
            'module_statistics': module_stats,
            'top_violations': [
                {
                    'violation_id': v['violation_id'],
                    'module': v['module'],
                    'occurrence_count': v['occurrence_count'],
                    'severity': v['severity']
                }
                for v in top_violations
            ]
        }
        
        if output_file:
            with open(output_file, 'w', encoding='utf-8') as f:
                json.dump(report, f, indent=2, ensure_ascii=False)
            logger.info(f"Summary report saved: {output_file}")
        
        return report
    
    def export_to_excel(self, output_file: Path, module_name: Optional[str] = None):
        """
        Export violations to Excel
        
        Args:
            output_file: Path for output Excel file
            module_name: Optional module name filter
        """
        all_violations = self.get_all_violations(module_name)
        
        # Prepare data for Excel
        excel_data = []
        for violation in all_violations:
            # Count files affected
            files_count = len(violation.get('files_affected', []))
            
            # Get first file for display
            first_file = ''
            if violation.get('files_affected'):
                first_file = violation['files_affected'][0].get('file', '')
            
            excel_data.append({
                'Module': violation.get('module', 'Unknown'),
                'Violation ID': violation.get('violation_id', ''),
                'Violation Text': violation.get('violation_text', ''),
                'Severity': violation.get('severity', ''),
                'Category': violation.get('category', ''),
                'Occurrence Count': violation.get('occurrence_count', 0),
                'Files Affected Count': files_count,
                'First File': first_file,
                'Fix Applied': violation.get('fix_applied', False),
                'Justification Added': violation.get('justification_added', False),
                'Justifiable': violation.get('justifiable', ''),
                'First Seen': violation.get('first_seen', ''),
                'Last Seen': violation.get('last_seen', '')
            })
        
        df = pd.DataFrame(excel_data)
        
        # Create Excel file with multiple sheets
        with pd.ExcelWriter(output_file, engine='openpyxl') as writer:
            # All violations
            df.to_excel(writer, sheet_name='All Violations', index=False)
            
            # High severity
            high_severity = df[df['Severity'] == 'HIGH']
            high_severity.to_excel(writer, sheet_name='High Severity', index=False)
            
            # Unfixed
            unfixed = df[df['Fix Applied'] == False]
            unfixed.to_excel(writer, sheet_name='Unfixed', index=False)
            
            # Summary
            summary_data = self.generate_summary_report()
            summary_df = pd.DataFrame([{
                'Total Modules': summary_data['total_modules'],
                'Total Unique Violations': summary_data['total_unique_violations'],
                'High Severity': summary_data['severity_breakdown']['HIGH'],
                'Medium Severity': summary_data['severity_breakdown']['MEDIUM'],
                'Low Severity': summary_data['severity_breakdown']['LOW'],
                'Fixed': summary_data['fix_status']['fixed'],
                'Unfixed': summary_data['fix_status']['unfixed'],
                'Percentage Fixed': summary_data['fix_status']['percentage_fixed']
            }])
            summary_df.to_excel(writer, sheet_name='Summary', index=False)
        
        logger.info(f"Excel export completed: {output_file}")
    
    def interactive_query(self):
        """Interactive CLI for querying the database"""
        
        print("\n" + "="*80)
        print("KNOWLEDGE DATABASE QUERY TOOL - Interactive Mode")
        print("="*80)
        
        while True:
            print("\nAvailable Commands:")
            print("  1. List modules")
            print("  2. Module summary")
            print("  3. Query by severity")
            print("  4. Query by category")
            print("  5. Query unfixed violations")
            print("  6. Query unjustified violations")
            print("  7. Query by file")
            print("  8. Top violations")
            print("  9. Generate summary report")
            print("  10. Export to Excel")
            print("  0. Exit")
            
            choice = input("\nEnter command number: ").strip()
            
            if choice == '0':
                print("Exiting...")
                break
            
            elif choice == '1':
                modules = self.list_modules()
                print(f"\nModules ({len(modules)}):")
                for mod in modules:
                    print(f"  - {mod}")
            
            elif choice == '2':
                module = input("Enter module name: ").strip()
                summary = self.get_module_summary(module)
                if summary:
                    print(f"\nModule Summary for {module}:")
                    for key, value in summary.items():
                        print(f"  {key}: {value}")
            
            elif choice == '3':
                severity = input("Enter severity (HIGH/MEDIUM/LOW): ").strip().upper()
                module = input("Enter module name (or press Enter for all): ").strip()
                violations = self.query_by_severity(severity, module if module else None)
                print(f"\nFound {len(violations)} violations")
                for v in violations[:10]:
                    print(f"  - {v['violation_id']} ({v['module']})")
            
            elif choice == '4':
                category = input("Enter category (CERT/MISRA/CWE/OTHER): ").strip().upper()
                module = input("Enter module name (or press Enter for all): ").strip()
                violations = self.query_by_category(category, module if module else None)
                print(f"\nFound {len(violations)} violations")
                for v in violations[:10]:
                    print(f"  - {v['violation_id']} ({v['module']})")
            
            elif choice == '5':
                module = input("Enter module name (or press Enter for all): ").strip()
                violations = self.query_unfixed_violations(module if module else None)
                print(f"\nFound {len(violations)} unfixed violations")
                for v in violations[:10]:
                    print(f"  - {v['violation_id']} ({v['module']})")
            
            elif choice == '6':
                module = input("Enter module name (or press Enter for all): ").strip()
                violations = self.query_unjustified_violations(module if module else None)
                print(f"\nFound {len(violations)} unjustified violations")
                for v in violations[:10]:
                    print(f"  - {v['violation_id']} ({v['module']})")
            
            elif choice == '7':
                file_name = input("Enter file name: ").strip()
                module = input("Enter module name (or press Enter for all): ").strip()
                violations = self.query_by_file(file_name, module if module else None)
                print(f"\nFound {len(violations)} violations in {file_name}")
                for v in violations[:10]:
                    print(f"  - {v['violation_id']} ({v['module']})")
            
            elif choice == '8':
                limit = input("Enter number of top violations (default 10): ").strip()
                limit = int(limit) if limit.isdigit() else 10
                module = input("Enter module name (or press Enter for all): ").strip()
                violations = self.query_top_violations(limit, module if module else None)
                print(f"\nTop {len(violations)} violations:")
                for i, v in enumerate(violations, 1):
                    print(f"  {i}. {v['violation_id']} - {v['occurrence_count']} occurrences ({v['module']})")
            
            elif choice == '9':
                output = input("Enter output file path (or press Enter to display only): ").strip()
                output_path = Path(output) if output else None
                report = self.generate_summary_report(output_path)
                print("\nSummary Report:")
                print(json.dumps(report, indent=2))
            
            elif choice == '10':
                output = input("Enter output Excel file path: ").strip()
                module = input("Enter module name (or press Enter for all): ").strip()
                if output:
                    self.export_to_excel(Path(output), module if module else None)
                    print(f"Excel file created: {output}")
            
            else:
                print("Invalid command")


def main():
    """Main entry point"""
    
    print("\n" + "="*80)
    print("KNOWLEDGE DATABASE QUERY TOOL - Version 1.0.0")
    print("="*80 + "\n")
    
    if len(sys.argv) < 2:
        print("Usage: python KnowledgeDatabaseQueryTool.py <knowledge_base_dir> [options]")
        print("\nOptions:")
        print("  --interactive              Launch interactive mode")
        print("  --summary <output_file>    Generate summary report")
        print("  --excel <output_file>      Export to Excel")
        print("  --module <name>            Filter by module")
        print("\nExamples:")
        print("  python KnowledgeDatabaseQueryTool.py knowledge_base --interactive")
        print("  python KnowledgeDatabaseQueryTool.py knowledge_base --summary report.json")
        print("  python KnowledgeDatabaseQueryTool.py knowledge_base --excel violations.xlsx")
        sys.exit(1)
    
    kb_dir = Path(sys.argv[1])
    
    try:
        query_tool = KnowledgeDatabaseQueryTool(kb_dir)
        
        # Interactive mode
        if '--interactive' in sys.argv:
            query_tool.interactive_query()
        
        # Generate summary
        elif '--summary' in sys.argv:
            idx = sys.argv.index('--summary')
            output_file = Path(sys.argv[idx + 1]) if idx + 1 < len(sys.argv) else Path('summary_report.json')
            report = query_tool.generate_summary_report(output_file)
            print(f"\n✓ Summary report generated: {output_file}")
            print(f"  Total modules: {report['total_modules']}")
            print(f"  Total unique violations: {report['total_unique_violations']}")
            print(f"  Percentage fixed: {report['fix_status']['percentage_fixed']}%")
        
        # Export to Excel
        elif '--excel' in sys.argv:
            idx = sys.argv.index('--excel')
            output_file = Path(sys.argv[idx + 1]) if idx + 1 < len(sys.argv) else Path('violations.xlsx')
            
            module_name = None
            if '--module' in sys.argv:
                mod_idx = sys.argv.index('--module')
                module_name = sys.argv[mod_idx + 1] if mod_idx + 1 < len(sys.argv) else None
            
            query_tool.export_to_excel(output_file, module_name)
            print(f"\n✓ Excel file generated: {output_file}")
        
        else:
            # Default: show summary
            modules = query_tool.list_modules()
            print(f"Loaded {len(modules)} modules:")
            for mod in modules:
                summary = query_tool.get_module_summary(mod)
                print(f"\n{mod}:")
                print(f"  Total unique violations: {summary['total_unique_violations']}")
                print(f"  Total analyses: {summary['total_analyses']}")
                print(f"  Last updated: {summary['last_updated']}")
    
    except Exception as e:
        logger.error(f"Error: {str(e)}", exc_info=True)
        print(f"\n✗ Error: {str(e)}")
        sys.exit(1)


if __name__ == "__main__":
    main()
