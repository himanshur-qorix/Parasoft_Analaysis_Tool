"""
Static Code Analyzer
Custom static analysis tool for C/C++ code
Detects runtime errors, MISRA/CERT violations, and code quality issues

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 3.0.0
"""

import logging
import re
from pathlib import Path
from typing import Dict, List, Optional, Tuple
from datetime import datetime

logger = logging.getLogger(__name__)


class StaticCodeAnalyzer:
    """Custom static code analyzer for C/C++ source code"""
    
    def __init__(self):
        """Initialize static code analyzer"""
        self.violations = []
        self.files_analyzed = 0
        self.lines_analyzed = 0
        
        # Analysis summary
        self.summary = {
            'total_findings': 0,
            'critical': 0,      # Definite errors
            'high': 0,          # Likely errors
            'medium': 0,        # Code quality
            'low': 0,           # Style/conventions
            'by_category': {},
            'by_check': {}
        }
        
        # Initialize rule checkers
        self._init_checkers()
    
    def _init_checkers(self):
        """Initialize various rule checkers"""
        
        # Runtime error patterns
        self.runtime_checks = {
            'NULL_DEREF': {
                'pattern': r'\*\s*\w+\s*(?:=|;|\))',
                'description': 'Potential null pointer dereference',
                'severity': 'CRITICAL',
                'category': 'Runtime Error'
            },
            'ARRAY_INDEX': {
                'pattern': r'\w+\s*\[\s*\w+\s*\]',
                'description': 'Array indexing without bounds check',
                'severity': 'HIGH',
                'category': 'Runtime Error'
            },
            'DIV_ZERO': {
                'pattern': r'/\s*\w+',
                'description': 'Potential division by zero',
                'severity': 'CRITICAL',
                'category': 'Runtime Error'
            },
            'UNINITIALIZED': {
                'pattern': r'^\s*(int|uint\w+|char|float|double)\s+(\w+)\s*;',
                'description': 'Variable declared but not initialized',
                'severity': 'HIGH',
                'category': 'Runtime Error'
            },
            'OVERFLOW': {
                'pattern': r'(\w+)\s*(\+|\-|\*)\s*(\w+)',
                'description': 'Potential integer overflow',
                'severity': 'MEDIUM',
                'category': 'Runtime Error'
            }
        }
        
        # MISRA C checks
        self.misra_checks = {
            'MISRA_10_3': {
                'pattern': r'=\s*\w+\s*;.*enum',
                'description': 'Implicit conversion from enum to different type',
                'severity': 'MEDIUM',
                'category': 'MISRA'
            },
            'MISRA_20_1': {
                'pattern': r'#include.*(?:after|below).*\w+\s*\(',
                'description': '#include directive appears after code',
                'severity': 'MEDIUM',
                'category': 'MISRA'
            },
            'MISRA_5_4': {
                'pattern': r'#define\s+(\w{31,})',
                'description': 'Macro name exceeds 31 significant characters',
                'severity': 'MEDIUM',
                'category': 'MISRA'
            },
            'MISRA_8_9': {
                'pattern': r'static\s+\w+\s+\w+\s*=',
                'description': 'Static variable should be defined at file scope',
                'severity': 'LOW',
                'category': 'MISRA'
            }
        }
        
        # CERT C checks
        self.cert_checks = {
            'CERT_INT31': {
                'pattern': r'(uint\d+_t|unsigned)\s+\w+\s*=\s*\w+\s*;',
                'description': 'Integer conversion without explicit cast',
                'severity': 'MEDIUM',
                'category': 'CERT'
            },
            'CERT_STR31': {
                'pattern': r'(strcpy|strcat|sprintf)\s*\(',
                'description': 'Use of unsafe string function',
                'severity': 'HIGH',
                'category': 'CERT'
            },
            'CERT_MEM35': {
                'pattern': r'malloc\s*\([^)]*\)',
                'description': 'Memory allocation without null check',
                'severity': 'HIGH',
                'category': 'CERT'
            }
        }
        
        # Code quality checks
        self.quality_checks = {
            'DEAD_CODE': {
                'pattern': r'(if|while)\s*\(\s*0\s*\)',
                'description': 'Dead code - condition always false',
                'severity': 'LOW',
                'category': 'Code Quality'
            },
            'MAGIC_NUMBER': {
                'pattern': r'(?<!["\'])(?<!\w)\d{2,}(?!\w)(?!["\'])',
                'description': 'Magic number - should use named constant',
                'severity': 'LOW',
                'category': 'Code Quality'
            },
            'LONG_FUNCTION': {
                'pattern': None,  # Handled separately
                'description': 'Function exceeds recommended length',
                'severity': 'LOW',
                'category': 'Code Quality'
            },
            'COMPLEX_EXPR': {
                'pattern': r'.*&&.*\|\|.*&&',
                'description': 'Complex boolean expression',
                'severity': 'LOW',
                'category': 'Code Quality'
            }
        }
    
    def analyze_directory(self, source_dir: Path, file_patterns: List[str] = None) -> List[Dict]:
        """
        Analyze all C/C++ files in a directory
        
        Args:
            source_dir: Directory containing source code
            file_patterns: File patterns to match (default: *.c, *.cpp, *.h)
        
        Returns:
            List of violation dictionaries
        """
        if file_patterns is None:
            file_patterns = ['*.c', '*.cpp', '*.h', '*.hpp']
        
        logger.info(f"Analyzing source code in: {source_dir}")
        
        source_files = []
        for pattern in file_patterns:
            source_files.extend(source_dir.rglob(pattern))
        
        logger.info(f"Found {len(source_files)} source files")
        
        for source_file in source_files:
            try:
                self.analyze_file(source_file)
                self.files_analyzed += 1
            except Exception as e:
                logger.error(f"Error analyzing {source_file}: {e}")
        
        self._update_summary()
        
        logger.info(f"Analysis complete: {len(self.violations)} violations found")
        return self.violations
    
    def analyze_file(self, file_path: Path):
        """
        Analyze a single source file
        
        Args:
            file_path: Path to source file
        """
        logger.debug(f"Analyzing: {file_path.name}")
        
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                lines = f.readlines()
        except Exception as e:
            logger.error(f"Failed to read {file_path}: {e}")
            return
        
        self.lines_analyzed += len(lines)
        
        # Track function length for complexity analysis
        in_function = False
        function_start = 0
        function_name = ''
        function_lines = 0
        
        # Track multi-line comments
        in_multiline_comment = False
        
        for line_num, line in enumerate(lines, 1):
            # Track multi-line comment state
            stripped = line.strip()
            
            # Check if we're entering a multi-line comment
            if '/*' in line:
                in_multiline_comment = True
            
            # Check if we're exiting a multi-line comment
            if in_multiline_comment and '*/' in line:
                in_multiline_comment = False
                continue  # Skip this line entirely
            # Detect function start
            func_match = re.match(r'^\s*(?:static\s+)?(?:const\s+)?(\w+)\s+(\w+)\s*\([^)]*\)\s*\{', line)
            if func_match and not in_function:
                in_function = True
                function_start = line_num
                function_name = func_match.group(2)
                function_lines = 0
            
            # Count function lines
            if in_function:
                function_lines += 1
                if '}' in line and line.strip() == '}':
                    # Function end
                    if function_lines > 50:  # Configurable threshold
                        self._add_violation(
                            file_path, line_num, line, lines,
                            'LONG_FUNCTION',
                            f'Function {function_name} has {function_lines} lines (recommended: < 50)',
                            'LOW', 'Code Quality'
                        )
                    in_function = False
            
            # Run all checkers on this line (skip if in comment)
            if not in_multiline_comment:
                self._check_line(file_path, line_num, line, lines)
    
    def _check_line(self, file_path: Path, line_num: int, line: str, all_lines: List[str]):
        """
        Run all checks on a single line
        
        Args:
            file_path: Path to source file
            line_num: Line number
            line: Line content
            all_lines: All lines in file (for context)
        """
        # Skip single-line comments and preprocessor directives
        stripped = line.strip()
        if stripped.startswith('//') or stripped.startswith('#'):
            return
        
        # Skip lines that are only comment markers
        if stripped == '/*' or stripped == '*/' or stripped.startswith('*'):
            return
        
        # Check runtime errors
        for check_id, check_info in self.runtime_checks.items():
            if check_info['pattern'] and re.search(check_info['pattern'], line):
                # Contextual validation
                if self._is_likely_violation(check_id, line, all_lines, line_num):
                    self._add_violation(
                        file_path, line_num, line, all_lines,
                        check_id,
                        check_info['description'],
                        check_info['severity'],
                        check_info['category']
                    )
        
        # Check MISRA rules
        for check_id, check_info in self.misra_checks.items():
            if check_info['pattern'] and re.search(check_info['pattern'], line):
                self._add_violation(
                    file_path, line_num, line, all_lines,
                    check_id,
                    check_info['description'],
                    check_info['severity'],
                    check_info['category']
                )
        
        # Check CERT rules
        for check_id, check_info in self.cert_checks.items():
            if check_info['pattern'] and re.search(check_info['pattern'], line):
                self._add_violation(
                    file_path, line_num, line, all_lines,
                    check_id,
                    check_info['description'],
                    check_info['severity'],
                    check_info['category']
                )
        
        # Check code quality
        for check_id, check_info in self.quality_checks.items():
            if check_info['pattern'] and re.search(check_info['pattern'], line):
                self._add_violation(
                    file_path, line_num, line, all_lines,
                    check_id,
                    check_info['description'],
                    check_info['severity'],
                    check_info['category']
                )
    
    def _is_likely_violation(self, check_id: str, line: str, all_lines: List[str], line_num: int) -> bool:
        """
        Context-aware validation to reduce false positives
        
        Args:
            check_id: Check identifier
            line: Current line
            all_lines: All lines in file
            line_num: Current line number
        
        Returns:
            True if likely a real violation
        """
        # For NULL_DEREF, check if there's a null check nearby
        if check_id == 'NULL_DEREF':
            # Look 5 lines back for null check
            start = max(0, line_num - 6)
            context = all_lines[start:line_num-1]
            for ctx_line in context:
                if 'if' in ctx_line and ('NULL' in ctx_line or '!=' in ctx_line):
                    return False  # Has null check
        
        # For array indexing, skip if there's obvious bounds check
        if check_id == 'ARRAY_INDEX':
            start = max(0, line_num - 6)
            context = all_lines[start:line_num-1]
            for ctx_line in context:
                if re.search(r'if\s*\(.*<.*\)', ctx_line):
                    return False  # Has bounds check
        
        # For division, check if divisor is checked
        if check_id == 'DIV_ZERO':
            match = re.search(r'/\s*(\w+)', line)
            if match:
                divisor = match.group(1)
                # Check if it's a constant
                if divisor.isdigit() and divisor != '0':
                    return False
        
        return True
    
    def _add_violation(self, file_path: Path, line_num: int, line: str, all_lines: List[str],
                      check_id: str, description: str, severity: str, category: str):
        """
        Add a violation to the results with code context
        
        Args:
            file_path: Source file path
            line_num: Line number (1-indexed)
            line: Line content
            all_lines: All lines in file (for context)
            check_id: Check identifier
            description: Violation description
            severity: Severity level
            category: Category
        """
        violation_id = f"STATIC-{check_id}-{len(self.violations) + 1}"
        
        # Extract code context (5 lines before, violation line, 5 lines after)
        context_before = 5
        context_after = 5
        start_line = max(0, line_num - context_before - 1)  # Convert to 0-indexed
        end_line = min(len(all_lines), line_num + context_after)
        
        code_context_lines = []
        for i in range(start_line, end_line):
            line_number = i + 1  # Convert back to 1-indexed
            is_violation_line = (line_number == line_num)
            code_context_lines.append({
                'line_num': line_number,
                'content': all_lines[i].rstrip(),
                'is_violation': is_violation_line
            })
        
        # Create multi-line code snippet
        code_snippet_multiline = '\n'.join(
            f"{'>>> ' if ctx['is_violation'] else '    '}{ctx['line_num']:4d}: {ctx['content']}"
            for ctx in code_context_lines
        )
        
        # Map severity to color code (Polyspace-style)
        color_code = self._get_color_code(severity)
        
        violation = {
            'violation_id': violation_id,
            'check_id': check_id,
            'severity': severity,
            'color_code': color_code,  # RED, ORANGE, GREY
            'category': category,
            'violation_text': description,
            'file': str(file_path.name),
            'full_path': str(file_path),
            'line': line_num,
            'code_snippet': line.strip(),
            'code_context': code_context_lines,  # Structured context
            'code_snippet_multiline': code_snippet_multiline,  # Formatted for display
            'tool': 'StaticAnalyzer',
            'files_affected': [f"{file_path.name}:{line_num}"],
            'timestamp': datetime.now().isoformat()
        }
        
        self.violations.append(violation)
    
    def _get_color_code(self, severity: str) -> str:
        """
        Map severity to Polyspace-style color code
        
        Args:
            severity: Severity level
        
        Returns:
            Color code: RED, ORANGE, or GREY
        """
        color_mapping = {
            'CRITICAL': 'RED',      # Proven errors
            'HIGH': 'ORANGE',       # Likely errors
            'MEDIUM': 'GREY',       # Code quality issues
            'LOW': 'GREY'           # Style/conventions
        }
        return color_mapping.get(severity, 'GREY')
    
    def _update_summary(self):
        """Update summary statistics"""
        self.summary['total_findings'] = len(self.violations)
        
        for v in self.violations:
            severity = v['severity']
            category = v['category']
            check_id = v['check_id']
            
            # Count by severity
            if severity == 'CRITICAL':
                self.summary['critical'] += 1
            elif severity == 'HIGH':
                self.summary['high'] += 1
            elif severity == 'MEDIUM':
                self.summary['medium'] += 1
            elif severity == 'LOW':
                self.summary['low'] += 1
            
            # Count by category
            self.summary['by_category'][category] = self.summary['by_category'].get(category, 0) + 1
            
            # Count by check
            self.summary['by_check'][check_id] = self.summary['by_check'].get(check_id, 0) + 1
    
    def get_summary(self) -> Dict:
        """Get analysis summary"""
        return {
            **self.summary,
            'files_analyzed': self.files_analyzed,
            'lines_analyzed': self.lines_analyzed
        }
    
    def filter_by_severity(self, severity: str) -> List[Dict]:
        """Filter violations by severity"""
        return [v for v in self.violations if v['severity'] == severity]
    
    def filter_by_category(self, category: str) -> List[Dict]:
        """Filter violations by category"""
        return [v for v in self.violations if v['category'] == category]
    
    def filter_by_file(self, filename: str) -> List[Dict]:
        """Filter violations by file"""
        return [v for v in self.violations if filename in v['file']]


if __name__ == '__main__':
    # Test analyzer
    import sys
    
    logging.basicConfig(level=logging.INFO)
    
    if len(sys.argv) > 1:
        analyzer = StaticCodeAnalyzer()
        source_dir = Path(sys.argv[1])
        
        violations = analyzer.analyze_directory(source_dir)
        
        summary = analyzer.get_summary()
        
        print(f"\n{'='*80}")
        print(f"Static Code Analysis Summary")
        print(f"{'='*80}")
        print(f"Files analyzed: {summary['files_analyzed']}")
        print(f"Lines analyzed: {summary['lines_analyzed']}")
        print(f"Total findings: {summary['total_findings']}")
        print(f"  Critical: {summary['critical']}")
        print(f"  High: {summary['high']}")
        print(f"  Medium: {summary['medium']}")
        print(f"  Low: {summary['low']}")
        print(f"\nBy Category:")
        for cat, count in summary['by_category'].items():
            print(f"  {cat}: {count}")
        
        print(f"\nSample violations:")
        for v in violations[:10]:
            print(f"  [{v['severity']}] {v['file']}:{v['line']} - {v['violation_text']}")
    else:
        print("Usage: python StaticCodeAnalyzer.py <source_directory>")
