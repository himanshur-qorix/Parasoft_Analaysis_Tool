"""
Direct Violation Capture Tool
Scans C/C++ source code for common coding violations
Version: 1.0.0
Developer: Himanshu R

This tool performs basic static analysis on C/C++ source files to detect:
- Buffer safety issues
- Null pointer violations
- Memory management issues
- Unsafe standard library usage
- Type casting issues
- MISRA/CERT coding standard violations
"""

import re
import os
import sys
import json
from pathlib import Path
from datetime import datetime
from typing import List, Dict, Tuple
import pandas as pd


class ViolationCapture:
    """Captures coding violations from C/C++ source files"""
    
    def __init__(self):
        self.violations = []
        self.file_count = 0
        self.line_count = 0
        
        # Violation patterns (regex-based detection)
        self.patterns = {
            # Buffer-related violations
            'BUFFER-001': {
                'pattern': r'\bgets\s*\(',
                'message': 'Unsafe use of gets() - buffer overflow risk',
                'severity': 'HIGH',
                'category': 'CERT',
                'suggestion': 'Use fgets() with size limit instead'
            },
            'BUFFER-002': {
                'pattern': r'\bstrcpy\s*\(',
                'message': 'Unsafe use of strcpy() - buffer overflow risk',
                'severity': 'HIGH',
                'category': 'CERT',
                'suggestion': 'Use strncpy() or strcpy_s() with size limit'
            },
            'BUFFER-003': {
                'pattern': r'\bstrcat\s*\(',
                'message': 'Unsafe use of strcat() - buffer overflow risk',
                'severity': 'HIGH',
                'category': 'CERT',
                'suggestion': 'Use strncat() or strcat_s() with size limit'
            },
            'BUFFER-004': {
                'pattern': r'\bsprintf\s*\(',
                'message': 'Unsafe use of sprintf() - buffer overflow risk',
                'severity': 'HIGH',
                'category': 'CERT',
                'suggestion': 'Use snprintf() with size limit'
            },
            
            # Memory management violations
            'MEM-001': {
                'pattern': r'\bmalloc\s*\([^)]+\)\s*;(?!\s*if)',
                'message': 'malloc() return value not checked for NULL',
                'severity': 'HIGH',
                'category': 'CERT',
                'suggestion': 'Check if malloc() returned NULL before using'
            },
            'MEM-002': {
                'pattern': r'\bcalloc\s*\([^)]+\)\s*;(?!\s*if)',
                'message': 'calloc() return value not checked for NULL',
                'severity': 'HIGH',
                'category': 'CERT',
                'suggestion': 'Check if calloc() returned NULL before using'
            },
            'MEM-003': {
                'pattern': r'\brealloc\s*\([^)]+\)\s*;(?!\s*if)',
                'message': 'realloc() return value not checked for NULL',
                'severity': 'HIGH',
                'category': 'CERT',
                'suggestion': 'Check if realloc() returned NULL before using'
            },
            'MEM-004': {
                'pattern': r'\bfree\s*\(\s*\w+\s*\)',
                'message': 'Pointer not set to NULL after free()',
                'severity': 'MEDIUM',
                'category': 'CERT',
                'suggestion': 'Set pointer to NULL after free() to prevent double-free'
            },
            
            # Null pointer violations
            'NULL-001': {
                'pattern': r'(\w+)\s*=\s*\w+\([^)]*\)\s*;\s*\1\s*->',
                'message': 'Pointer dereferenced without NULL check',
                'severity': 'HIGH',
                'category': 'CERT',
                'suggestion': 'Add NULL check before dereferencing pointer'
            },
            
            # MISRA violations
            'MISRA-001': {
                'pattern': r'\bgoto\s+\w+',
                'message': 'Use of goto statement (MISRA-C:2012-15.1)',
                'severity': 'MEDIUM',
                'category': 'MISRA',
                'suggestion': 'Restructure code to avoid goto'
            },
            'MISRA-002': {
                'pattern': r'\batoi\s*\(',
                'message': 'Use of atoi() - undefined behavior on error (MISRA-C:2012-21.7)',
                'severity': 'MEDIUM',
                'category': 'MISRA',
                'suggestion': 'Use strtol() with error checking'
            },
            'MISRA-003': {
                'pattern': r'\batof\s*\(',
                'message': 'Use of atof() - undefined behavior on error (MISRA-C:2012-21.7)',
                'severity': 'MEDIUM',
                'category': 'MISRA',
                'suggestion': 'Use strtod() with error checking'
            },
            
            # Type casting issues
            'CAST-001': {
                'pattern': r'\(\s*void\s*\*\s*\)\s*\d+',
                'message': 'Integer cast to void pointer without proper type',
                'severity': 'MEDIUM',
                'category': 'CERT',
                'suggestion': 'Use proper pointer types or uintptr_t'
            },
            
            # Format string vulnerabilities
            'FORMAT-001': {
                'pattern': r'printf\s*\(\s*[a-zA-Z_]\w*\s*\)',
                'message': 'Format string not constant - security risk',
                'severity': 'HIGH',
                'category': 'CERT',
                'suggestion': 'Use printf("%s", var) instead of printf(var)'
            },
            'FORMAT-002': {
                'pattern': r'fprintf\s*\([^,]+,\s*[a-zA-Z_]\w*\s*\)',
                'message': 'Format string not constant - security risk',
                'severity': 'HIGH',
                'category': 'CERT',
                'suggestion': 'Use constant format string'
            },
            
            # Array bounds
            'ARRAY-001': {
                'pattern': r'(\w+)\[(\w+)\](?:(?!\bif\b.*\2\s*<).{0,100}$)',
                'message': 'Array access without bounds check',
                'severity': 'MEDIUM',
                'category': 'CERT',
                'suggestion': 'Add bounds checking before array access'
            },
            
            # Comparison issues
            'COMP-001': {
                'pattern': r'if\s*\(\s*\w+\s*=\s*[^=]',
                'message': 'Assignment in if condition - possible typo for ==',
                'severity': 'HIGH',
                'category': 'MISRA',
                'suggestion': 'Use == for comparison, not ='
            },
            
            # Uninitialized variables (basic detection)
            'INIT-001': {
                'pattern': r'^\s*(int|char|float|double|long|short)\s+(\w+)\s*;',
                'message': 'Variable declared without initialization',
                'severity': 'MEDIUM',
                'category': 'MISRA',
                'suggestion': 'Initialize variable at declaration'
            },
        }
    
    def scan_file(self, file_path: Path) -> List[Dict]:
        """
        Scan a single source file for violations
        
        Args:
            file_path: Path to the source file
            
        Returns:
            List of violations found
        """
        violations_found = []
        
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                lines = f.readlines()
            
            self.file_count += 1
            self.line_count += len(lines)
            
            # Check each line against patterns
            for line_num, line in enumerate(lines, start=1):
                # Skip comments and empty lines
                if self._is_comment_or_empty(line):
                    continue
                
                # Check against all violation patterns
                for violation_id, pattern_info in self.patterns.items():
                    pattern = pattern_info['pattern']
                    
                    if re.search(pattern, line):
                        violation = {
                            'Violation ID': violation_id,
                            'Violation': pattern_info['message'],
                            'File': file_path.name,
                            'Full Path': str(file_path),
                            'Line number': line_num,
                            'Line content': line.strip(),
                            'Severity': pattern_info['severity'],
                            'Category': pattern_info['category'],
                            'Suggestion': pattern_info['suggestion']
                        }
                        violations_found.append(violation)
                        self.violations.append(violation)
            
        except Exception as e:
            print(f"Error scanning {file_path}: {str(e)}")
        
        return violations_found
    
    def _is_comment_or_empty(self, line: str) -> bool:
        """Check if line is a comment or empty"""
        stripped = line.strip()
        return (not stripped or 
                stripped.startswith('//') or 
                stripped.startswith('/*') or 
                stripped.startswith('*'))
    
    def scan_directory(self, directory: Path, extensions: Tuple[str] = ('.c', '.cpp', '.h', '.hpp')):
        """
        Scan all source files in a directory
        
        Args:
            directory: Directory to scan
            extensions: File extensions to scan
        """
        print(f"\n🔍 Scanning directory: {directory}")
        print(f"Looking for files: {', '.join(extensions)}\n")
        
        for ext in extensions:
            for file_path in directory.rglob(f'*{ext}'):
                print(f"  Scanning: {file_path.name}")
                self.scan_file(file_path)
    
    def generate_report(self, output_format='json') -> str:
        """
        Generate violation report
        
        Args:
            output_format: Output format ('json', 'text', 'csv', 'excel')
            
        Returns:
            Report content as string (None for Excel - saves directly)
        """
        if output_format == 'json':
            return self._generate_json_report()
        elif output_format == 'text':
            return self._generate_text_report()
        elif output_format == 'csv':
            return self._generate_csv_report()
        elif output_format == 'excel':
            return None  # Excel is handled separately in save_report
        else:
            return self._generate_json_report()
    
    def _generate_json_report(self) -> str:
        """Generate JSON format report"""
        report = {
            'scan_timestamp': datetime.now().isoformat(),
            'files_scanned': self.file_count,
            'total_lines': self.line_count,
            'total_violations': len(self.violations),
            'violations': self.violations,
            'summary': self._get_summary()
        }
        return json.dumps(report, indent=2)
    
    def _generate_text_report(self) -> str:
        """Generate text format report"""
        lines = []
        lines.append("=" * 80)
        lines.append("VIOLATION CAPTURE REPORT")
        lines.append("=" * 80)
        lines.append(f"Scan Date: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        lines.append(f"Files Scanned: {self.file_count}")
        lines.append(f"Total Lines: {self.line_count}")
        lines.append(f"Total Violations: {len(self.violations)}")
        lines.append("=" * 80)
        lines.append("")
        
        # Summary by severity
        summary = self._get_summary()
        lines.append("SEVERITY BREAKDOWN:")
        lines.append(f"  HIGH:   {summary['severity']['HIGH']}")
        lines.append(f"  MEDIUM: {summary['severity']['MEDIUM']}")
        lines.append(f"  LOW:    {summary['severity']['LOW']}")
        lines.append("")
        
        lines.append("CATEGORY BREAKDOWN:")
        for category, count in summary['category'].items():
            lines.append(f"  {category}: {count}")
        lines.append("")
        
        # Detailed violations
        lines.append("DETAILED VIOLATIONS:")
        lines.append("-" * 80)
        
        for i, violation in enumerate(self.violations, 1):
            lines.append(f"\nVIOLATION #{i}")
            lines.append(f"  ID:       {violation['Violation ID']}")
            lines.append(f"  Severity: {violation['Severity']}")
            lines.append(f"  Category: {violation['Category']}")
            lines.append(f"  Message:  {violation['Violation']}")
            lines.append(f"  File:     {violation['File']}:{violation['Line number']}")
            lines.append(f"  Code:     {violation['Line content']}")
            lines.append(f"  Fix:      {violation['Suggestion']}")
            lines.append("-" * 80)
        
        return '\n'.join(lines)
    
    def _generate_csv_report(self) -> str:
        """Generate CSV format report"""
        lines = []
        lines.append("Violation ID,Severity,Category,File,Line,Message,Code,Suggestion")
        
        for violation in self.violations:
            line = f"{violation['Violation ID']},"
            line += f"{violation['Severity']},"
            line += f"{violation['Category']},"
            line += f"{violation['File']},"
            line += f"{violation['Line number']},"
            line += f"\"{violation['Violation']}\","
            line += f"\"{violation['Line content']}\","
            line += f"\"{violation['Suggestion']}\""
            lines.append(line)
        
        return '\n'.join(lines)
    
    def _get_summary(self) -> Dict:
        """Get summary statistics"""
        summary = {
            'severity': {'HIGH': 0, 'MEDIUM': 0, 'LOW': 0},
            'category': {}
        }
        
        for violation in self.violations:
            # Count by severity
            severity = violation.get('Severity', 'MEDIUM')
            summary['severity'][severity] = summary['severity'].get(severity, 0) + 1
            
            # Count by category
            category = violation.get('Category', 'OTHER')
            summary['category'][category] = summary['category'].get(category, 0) + 1
        
        return summary
    
    def _generate_excel_report(self, output_path: Path):
        """
        Generate Excel format report with multiple sheets
        
        Args:
            output_path: Output file path for Excel
        """
        # Prepare data for DataFrame
        df_data = []
        for violation in self.violations:
            df_data.append({
                'Violation ID': violation['Violation ID'],
                'Severity': violation['Severity'],
                'Category': violation['Category'],
                'Violation': violation['Violation'],
                'File': violation['File'],
                'Line': violation['Line number'],
                'Code': violation['Line content'],
                'Suggestion': violation['Suggestion'],
                'Full Path': violation['Full Path']
            })
        
        df = pd.DataFrame(df_data)
        
        # Create Excel file with multiple sheets
        with pd.ExcelWriter(output_path, engine='openpyxl') as writer:
            # All violations
            df.to_excel(writer, sheet_name='All Violations', index=False)
            
            # High severity violations
            high_severity = df[df['Severity'] == 'HIGH']
            if not high_severity.empty:
                high_severity.to_excel(writer, sheet_name='High Severity', index=False)
            
            # Medium severity violations
            medium_severity = df[df['Severity'] == 'MEDIUM']
            if not medium_severity.empty:
                medium_severity.to_excel(writer, sheet_name='Medium Severity', index=False)
            
            # By category
            summary = self._get_summary()
            for category in summary['category'].keys():
                category_df = df[df['Category'] == category]
                if not category_df.empty:
                    sheet_name = f'{category} Violations'
                    category_df.to_excel(writer, sheet_name=sheet_name[:31], index=False)  # Excel sheet name limit
            
            # Summary sheet
            summary_data = [{
                'Metric': 'Files Scanned',
                'Value': self.file_count
            }, {
                'Metric': 'Lines Scanned',
                'Value': self.line_count
            }, {
                'Metric': 'Total Violations',
                'Value': len(self.violations)
            }, {
                'Metric': 'High Severity',
                'Value': summary['severity']['HIGH']
            }, {
                'Metric': 'Medium Severity',
                'Value': summary['severity']['MEDIUM']
            }, {
                'Metric': 'Low Severity',
                'Value': summary['severity']['LOW']
            }]
            
            summary_df = pd.DataFrame(summary_data)
            summary_df.to_excel(writer, sheet_name='Summary', index=False)
            
            # Category breakdown
            category_data = [{'Category': cat, 'Count': count} 
                           for cat, count in summary['category'].items()]
            category_df = pd.DataFrame(category_data)
            category_df.to_excel(writer, sheet_name='Category Breakdown', index=False)
    
    def save_report(self, output_path: Path, output_format='json'):
        """
        Save report to file
        
        Args:
            output_path: Output file path
            output_format: Output format ('json', 'text', 'csv', 'excel')
        """
        if output_format == 'excel':
            self._generate_excel_report(output_path)
            print(f"\n[+] Excel report saved: {output_path}")
        else:
            report = self.generate_report(output_format)
            
            with open(output_path, 'w', encoding='utf-8') as f:
                f.write(report)
            
            print(f"\n[+] Report saved: {output_path}")
    
    def export_to_knowledge_base_format(self, module_name: str, output_path: Path):
        """
        Export violations in knowledge base format
        Compatible with KnowledgeDatabaseManager
        
        Args:
            module_name: Name of the module
            output_path: Output file path
        """
        # Convert violations to knowledge base format
        kb_violations = {}
        
        for violation in self.violations:
            vid = violation['Violation ID']
            
            if vid not in kb_violations:
                kb_violations[vid] = {
                    'violation_id': vid,
                    'violation_text': violation['Violation'],
                    'first_seen': datetime.now().isoformat(),
                    'last_seen': datetime.now().isoformat(),
                    'occurrence_count': 1,
                    'files_affected': [],
                    'severity': violation['Severity'],
                    'category': violation['Category'],
                    'fix_suggestion': violation['Suggestion'],
                    'fix_applied': False,
                    'justification_added': False
                }
            else:
                kb_violations[vid]['occurrence_count'] += 1
                kb_violations[vid]['last_seen'] = datetime.now().isoformat()
            
            # Add file information
            kb_violations[vid]['files_affected'].append({
                'file': violation['File'],
                'full_path': violation['Full Path'],
                'line': violation['Line number'],
                'code': violation['Line content'],
                'timestamp': datetime.now().isoformat()
            })
        
        # Create knowledge base structure
        knowledge_base = {
            'module_name': module_name,
            'created_date': datetime.now().isoformat(),
            'last_updated': datetime.now().isoformat(),
            'version': '1.0',
            'source': 'Direct Violation Capture Tool',
            'total_unique_violations': len(kb_violations),
            'violations': kb_violations,
            'statistics': {
                'total_files_scanned': self.file_count,
                'total_lines_scanned': self.line_count,
                'total_violations_found': len(self.violations)
            }
        }
        
        with open(output_path, 'w', encoding='utf-8') as f:
            json.dump(knowledge_base, f, indent=2)
        
        print(f"\n[+] Knowledge base export saved: {output_path}")


def main():
    """Main entry point"""
    
    print("\n" + "=" * 80)
    print("  DIRECT VIOLATION CAPTURE TOOL - Version 1.0.0")
    print("  Scans C/C++ source code for coding violations")
    print("=" * 80 + "\n")
    
    if len(sys.argv) < 2 or '--help' in sys.argv or '-h' in sys.argv:
        print("Usage: python capture_violations.py <source_directory> [options]")
        print("\nOptions:")
        print("  --module <name>      Specify module name (default: 'Unknown')")
        print("  --format <fmt>       Output format: json, text, csv, excel (default: json)")
        print("  --output <file>      Output file (default: violations_report.json)")
        print("  --kb                 Export in knowledge base format")
        print("  --extensions <exts>  File extensions to scan (default: .c,.cpp,.h,.hpp)")
        print("  --help, -h           Show this help message")
        print("\nExamples:")
        print("  python capture_violations.py src/")
        print("  python capture_violations.py src/ --module Mka --kb")
        print("  python capture_violations.py src/ --format excel --output report.xlsx")
        print("  python capture_violations.py src/ --format text --output report.txt")
        sys.exit(0 if '--help' in sys.argv or '-h' in sys.argv else 1)
    
    # Parse arguments
    source_dir = Path(sys.argv[1])
    module_name = 'Unknown'
    output_format = 'json'
    output_file = None  # Will be set based on format
    kb_export = False
    extensions = ('.c', '.cpp', '.h', '.hpp')
    
    i = 2
    while i < len(sys.argv):
        if sys.argv[i] == '--module' and i + 1 < len(sys.argv):
            module_name = sys.argv[i + 1]
            i += 2
        elif sys.argv[i] == '--format' and i + 1 < len(sys.argv):
            output_format = sys.argv[i + 1]
            i += 2
        elif sys.argv[i] == '--output' and i + 1 < len(sys.argv):
            output_file = sys.argv[i + 1]
            i += 2
        elif sys.argv[i] == '--kb':
            kb_export = True
            i += 1
        elif sys.argv[i] == '--extensions' and i + 1 < len(sys.argv):
            extensions = tuple(sys.argv[i + 1].split(','))
            i += 2
        else:
            i += 1
    
    # Set default output file based on format if not specified
    if output_file is None:
        if output_format == 'excel':
            output_file = 'violations_report.xlsx'
        elif output_format == 'text':
            output_file = 'violations_report.txt'
        elif output_format == 'csv':
            output_file = 'violations_report.csv'
        else:
            output_file = 'violations_report.json'
    
    # Validate source directory
    if not source_dir.exists():
        print(f"[x] Error: Directory not found: {source_dir}")
        sys.exit(1)
    
    # Initialize scanner
    scanner = ViolationCapture()
    
    # Scan directory
    scanner.scan_directory(source_dir, extensions)
    
    # Print summary
    print(f"\n{'=' * 80}")
    print("SCAN COMPLETE")
    print(f"{'=' * 80}")
    print(f"Files Scanned:      {scanner.file_count}")
    print(f"Lines Scanned:      {scanner.line_count}")
    print(f"Violations Found:   {len(scanner.violations)}")
    
    summary = scanner._get_summary()
    print(f"\nSeverity Breakdown:")
    print(f"  HIGH:   {summary['severity']['HIGH']}")
    print(f"  MEDIUM: {summary['severity']['MEDIUM']}")
    print(f"  LOW:    {summary['severity']['LOW']}")
    
    print(f"\nCategory Breakdown:")
    for category, count in summary['category'].items():
        print(f"  {category}: {count}")
    
    # Save report
    output_path = Path(output_file)
    scanner.save_report(output_path, output_format)
    
    # Export to knowledge base format if requested
    if kb_export:
        kb_path = Path(f"{module_name}_ViolationCapture.json")
        scanner.export_to_knowledge_base_format(module_name, kb_path)
    
    print(f"\n{'=' * 80}")
    print("[+] All operations completed successfully!")
    print(f"{'=' * 80}\n")


if __name__ == "__main__":
    main()
