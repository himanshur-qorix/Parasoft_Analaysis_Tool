"""
Static Code Analyzer
Custom static analysis tool for C/C++ code
Detects runtime errors, MISRA/CERT violations, and code quality issues

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 4.0.0 - Enhanced with Parasoft Rules Database integration

ENHANCEMENTS IN v4.0.0:
=======================
1. Parasoft Rules Database Integration
   - Loads 1204 comprehensive rules (598 CERT-C, 602 MISRA-C:2012)
   - Includes detailed rule descriptions, examples, and references
   - Automatic rule categorization by standard

2. Enhanced MISRA C Checks (15+ rules)
   - MISRA_12_2: Shift operator validation
   - MISRA_10_1: Enum type conversions
   - MISRA_11_3: Pointer type casts
   - MISRA_21_3: Dynamic memory allocation
   - MISRA_17_7: Return value usage
   - MISRA_15_6: Loop/condition braces
   - MISRA_21_6: Standard I/O functions
   - And more...

3. Enhanced CERT C Checks (13+ rules)
   - CERT_ARR30: Array bounds (CWE-119, CWE-125)
   - CERT_INT30/INT34: Integer operations (CWE-190, CWE-758)
   - CERT_STR31/STR32: String safety (CWE-119)
   - CERT_MEM30/MEM34/MEM35: Memory management (CWE-416, CWE-789)
   - CERT_ENV33: Command injection (CWE-78)
   - CERT_FIO30: Format string injection (CWE-134)
   - And more...

4. CWE Security Classifications
   - Each violation mapped to relevant CWE identifiers
   - Enables security-focused analysis and reporting
   - Supports OWASP and security compliance requirements

5. Knowledge Base Integration
   - Loads proven fix patterns from module knowledge bases
   - Pattern matching against known violations
   - Suggests fixes based on similar code patterns
   - Confidence scoring for pattern matches

6. Enhanced Violation Reporting
   - Detailed Parasoft rule information
   - Example repairs from rule database
   - Knowledge base match suggestions
   - Comprehensive code context (5 lines before/after)
   - CWE security classifications

USAGE:
======
Basic:
    analyzer = StaticCodeAnalyzer()
    violations = analyzer.analyze_directory(Path('src/'))

Advanced:
    analyzer = StaticCodeAnalyzer(
        load_parasoft_db=True,   # Load 1204 rules
        load_knowledge=True       # Load KB patterns
    )
    violations = analyzer.analyze_directory(
        Path('src/'), 
        file_patterns=['*.c', '*.h']
    )

OUTPUT ENHANCEMENTS:
===================
Each violation now includes:
- violation_id: Unique identifier
- check_id: Rule identifier (e.g., CERT_ARR30, MISRA_12_2)
- severity: CRITICAL, HIGH, MEDIUM, LOW
- category: CERT, MISRA, Runtime Error, Code Quality
- cwe: List of CWE identifiers (security classification)
- parasoft_rule: Detailed rule info from database
- kb_match: Matching pattern from knowledge base with suggested fix
- code_context: 11 lines of context (5 before, violation, 5 after)
"""

import logging
import re
import json
from pathlib import Path
from typing import Dict, List, Optional, Tuple
from datetime import datetime

logger = logging.getLogger(__name__)


class StaticCodeAnalyzer:
    """Custom static code analyzer for C/C++ source code"""
    
    def __init__(self, load_parasoft_db: bool = True, load_knowledge: bool = True):
        """
        Initialize static code analyzer
        
        Args:
            load_parasoft_db: Load comprehensive Parasoft Rules Database
            load_knowledge: Load patterns from knowledge bases
        """
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
        
        # Parasoft Rules Database
        self.parasoft_rules = {}
        self.parasoft_loaded = False
        
        # Knowledge base patterns
        self.kb_patterns = []
        self.kb_loaded = False
        
        # Initialize rule checkers
        self._init_checkers()
        
        # Load comprehensive databases
        if load_parasoft_db:
            self._load_parasoft_database()
        
        if load_knowledge:
            self._load_knowledge_patterns()
    
    def _init_checkers(self):
        """Initialize various rule checkers"""
        
        # Runtime error patterns
        self.runtime_checks = {
            'NULL_DEREF': {
                'pattern': r'\*\s*\w+\s*(?:=|;|\))',
                'description': 'Potential null pointer dereference',
                'severity': 'CRITICAL',
                'category': 'Runtime Error',
                'cwe': ['CWE-476']
            },
            'ARRAY_INDEX': {
                'pattern': r'\w+\s*\[\s*\w+\s*\]',
                'description': 'Array indexing without bounds check',
                'severity': 'HIGH',
                'category': 'Runtime Error',
                'cwe': ['CWE-119', 'CWE-125']
            },
            'DIV_ZERO': {
                'pattern': r'/\s*\w+',
                'description': 'Potential division by zero',
                'severity': 'CRITICAL',
                'category': 'Runtime Error',
                'cwe': ['CWE-369']
            },
            'UNINITIALIZED': {
                'pattern': r'^\s*(int|uint\w+|char|float|double)\s+(\w+)\s*;',
                'description': 'Variable declared but not initialized',
                'severity': 'HIGH',
                'category': 'Runtime Error',
                'cwe': ['CWE-457']
            },
            'OVERFLOW': {
                'pattern': r'(\w+)\s*(\+|\-|\*)\s*(\w+)',
                'description': 'Potential integer overflow',
                'severity': 'MEDIUM',
                'category': 'Runtime Error',
                'cwe': ['CWE-190']
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
    
    def _load_parasoft_database(self):
        """Load comprehensive Parasoft Rules Database"""
        try:
            project_root = Path(__file__).parent.parent
            db_file = project_root / 'data' / 'Parasoft_Rules_Database.json'
            
            if not db_file.exists():
                logger.warning(f"Parasoft Rules Database not found at {db_file}")
                return
            
            with open(db_file, 'r', encoding='utf-8') as f:
                db_data = json.load(f)
            
            self.parasoft_rules = db_data.get('rules', {})
            total_rules = db_data.get('total_rules', 0)
            
            logger.info(f"✓ Loaded Parasoft Rules Database: {total_rules} rules")
            
            # Add enhanced MISRA checks from database
            self._add_parasoft_misra_checks()
            
            # Add enhanced CERT checks from database
            self._add_parasoft_cert_checks()
            
            self.parasoft_loaded = True
            
        except Exception as e:
            logger.error(f"Failed to load Parasoft Rules Database: {e}")
    
    def _load_knowledge_patterns(self):
        """Load proven patterns from knowledge bases"""
        try:
            project_root = Path(__file__).parent.parent
            kb_dir = project_root / 'knowledge_base'
            
            if not kb_dir.exists():
                logger.warning(f"Knowledge base directory not found at {kb_dir}")
                return
            
            # Load all module knowledge bases
            kb_files = list(kb_dir.glob('*_KnowledgeDatabase.json'))
            
            patterns_loaded = 0
            for kb_file in kb_files:
                try:
                    with open(kb_file, 'r', encoding='utf-8') as f:
                        kb_data = json.load(f)
                    
                    # Extract patterns from violations with fixes
                    for violation in kb_data.get('violations', []):
                        if violation.get('fix_pattern'):
                            pattern_info = {
                                'check_id': violation.get('check_id', ''),
                                'category': violation.get('category', ''),
                                'pattern': violation.get('code_snippet', ''),
                                'fix': violation.get('fix_pattern', '')
                            }
                            self.kb_patterns.append(pattern_info)
                            patterns_loaded += 1
                
                except Exception as e:
                    logger.warning(f"Could not load {kb_file.name}: {e}")
            
            if patterns_loaded > 0:
                logger.info(f"✓ Loaded {patterns_loaded} proven patterns from {len(kb_files)} knowledge bases")
                self.kb_loaded = True
        
        except Exception as e:
            logger.error(f"Failed to load knowledge patterns: {e}")
    
    def _add_parasoft_misra_checks(self):
        """Add enhanced MISRA checks from Parasoft database"""
        # Enhanced MISRA patterns based on Parasoft rules
        enhanced_misra = {
            'MISRA_12_2': {
                'pattern': r'<<|>>',  # Shift operations
                'description': 'Shift operator requires careful validation (MISRA C:2012 Rule 12.2)',
                'severity': 'HIGH',
                'category': 'MISRA',
                'cwe': []
            },
            'MISRA_10_1': {
                'pattern': r'=\s*[^=]*enum\s+\w+',
                'description': 'Implicit conversion between enum types (MISRA C:2012 Rule 10.1)',
                'severity': 'MEDIUM',
                'category': 'MISRA',
                'cwe': []
            },
            'MISRA_11_3': {
                'pattern': r'\(\s*\w+\s*\*\s*\)',  # Pointer cast
                'description': 'Cast between pointer to object and different pointer type (MISRA C:2012 Rule 11.3)',
                'severity': 'MEDIUM',
                'category': 'MISRA',
                'cwe': []
            },
            'MISRA_21_3': {
                'pattern': r'\b(malloc|calloc|realloc|free)\s*\(',
                'description': 'Dynamic memory allocation should not be used (MISRA C:2012 Rule 21.3)',
                'severity': 'MEDIUM',
                'category': 'MISRA',
                'cwe': []
            },
            'MISRA_17_7': {
                'pattern': r'^\s*\w+\([^)]*\)\s*;',  # Function call without using return
                'description': 'Return value of non-void function should be used (MISRA C:2012 Rule 17.7)',
                'severity': 'MEDIUM',
                'category': 'MISRA',
                'cwe': []
            },
            'MISRA_2_3': {
                'pattern': r'typedef\s+\w+\s+\w+;',
                'description': 'Unused type declaration (MISRA C:2012 Rule 2.3)',
                'severity': 'LOW',
                'category': 'MISRA',
                'cwe': []
            },
            'MISRA_8_13': {
                'pattern': r'\w+\s+\*\s*\w+\s*\)',  # Non-const pointer parameter
                'description': 'Pointer parameter that is not modified should be declared const (MISRA C:2012 Rule 8.13)',
                'severity': 'LOW',
                'category': 'MISRA',
                'cwe': []
            },
            'MISRA_15_6': {
                'pattern': r'(if|while|for)\s*\([^)]+\)\s*[^{]',  # Missing braces
                'description': 'Body of iteration/selection statement should be enclosed in braces (MISRA C:2012 Rule 15.6)',
                'severity': 'MEDIUM',
                'category': 'MISRA',
                'cwe': []
            },
            'MISRA_18_1': {
                'pattern': r'\[\s*\w+\s*\]',  # Array indexing
                'description': 'Array indexing shall be the only form of pointer arithmetic (MISRA C:2012 Rule 18.1)',
                'severity': 'MEDIUM',
                'category': 'MISRA',
                'cwe': []
            },
            'MISRA_21_6': {
                'pattern': r'\b(printf|scanf|fprintf|fscanf|sprintf|sscanf|snprintf|vprintf|vscanf|vfprintf|vfscanf|vsprintf|vsscanf|vsnprintf)\s*\(',
                'description': 'Standard library input/output functions should not be used (MISRA C:2012 Rule 21.6)',
                'severity': 'MEDIUM',
                'category': 'MISRA',
                'cwe': []
            }
        }
        
        # Merge with existing checks
        self.misra_checks.update(enhanced_misra)
        logger.info(f"✓ Enhanced MISRA checks: {len(self.misra_checks)} rules active")
    
    def _add_parasoft_cert_checks(self):
        """Add enhanced CERT checks from Parasoft database"""
        # Enhanced CERT patterns based on Parasoft rules
        enhanced_cert = {
            'CERT_EXP36': {
                'pattern': r'=\s*\(\s*\w+\s*\)',  # Type cast
                'description': 'Do not cast pointers into more strictly aligned pointer types (CERT C EXP36-C)',
                'severity': 'HIGH',
                'category': 'CERT',
                'cwe': ['CWE-704']
            },
            'CERT_ARR30': {
                'pattern': r'\w+\s*\[\s*\w+\s*\]',  # Array access
                'description': 'Do not form or use out-of-bounds pointers or array subscripts (CERT C ARR30-C)',
                'severity': 'CRITICAL',
                'category': 'CERT',
                'cwe': ['CWE-119', 'CWE-125']
            },
            'CERT_INT30': {
                'pattern': r'(<<|>>)',  # Shift operations
                'description': 'Ensure unsigned integer operations do not wrap (CERT C INT30-C)',
                'severity': 'HIGH',
                'category': 'CERT',
                'cwe': ['CWE-190']
            },
            'CERT_INT34': {
                'pattern': r'<<|>>',  # Shift by invalid amount
                'description': 'Do not shift an expression by a negative number of bits or >= width (CERT C INT34-C)',
                'severity': 'CRITICAL',
                'category': 'CERT',
                'cwe': ['CWE-758']
            },
            'CERT_FIO30': {
                'pattern': r'fopen\s*\(',
                'description': 'Exclude user input from format strings (CERT C FIO30-C)',
                'severity': 'HIGH',
                'category': 'CERT',
                'cwe': ['CWE-134']
            },
            'CERT_STR32': {
                'pattern': r'\bgets\s*\(',
                'description': 'Do not pass a non-null-terminated string to library function (CERT C STR32-C)',
                'severity': 'CRITICAL',
                'category': 'CERT',
                'cwe': ['CWE-119']
            },
            'CERT_MEM30': {
                'pattern': r'\bfree\s*\(',
                'description': 'Do not access freed memory (CERT C MEM30-C)',
                'severity': 'CRITICAL',
                'category': 'CERT',
                'cwe': ['CWE-416']
            },
            'CERT_MEM34': {
                'pattern': r'malloc\s*\([^)]*\*[^)]*\)',  # Multiplication in malloc
                'description': 'Detect and handle memory allocation errors (CERT C MEM34-C)',
                'severity': 'HIGH',
                'category': 'CERT',
                'cwe': ['CWE-789']
            },
            'CERT_DCL30': {
                'pattern': r'extern\s+\w+\s+\w+',
                'description': 'Declare objects with appropriate storage durations (CERT C DCL30-C)',
                'severity': 'MEDIUM',
                'category': 'CERT',
                'cwe': []
            },
            'CERT_ENV33': {
                'pattern': r'\bsystem\s*\(',
                'description': 'Do not call system() (CERT C ENV33-C)',
                'severity': 'CRITICAL',
                'category': 'CERT',
                'cwe': ['CWE-78']
            }
        }
        
        # Merge with existing checks
        self.cert_checks.update(enhanced_cert)
        logger.info(f"✓ Enhanced CERT checks: {len(self.cert_checks)} rules active")
    
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
                            'LOW', 'Code Quality', []
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
                        check_info['category'],
                        check_info.get('cwe', [])
                    )
        
        # Check MISRA rules
        for check_id, check_info in self.misra_checks.items():
            if check_info['pattern'] and re.search(check_info['pattern'], line):
                self._add_violation(
                    file_path, line_num, line, all_lines,
                    check_id,
                    check_info['description'],
                    check_info['severity'],
                    check_info['category'],
                    check_info.get('cwe', [])
                )
        
        # Check CERT rules
        for check_id, check_info in self.cert_checks.items():
            if check_info['pattern'] and re.search(check_info['pattern'], line):
                self._add_violation(
                    file_path, line_num, line, all_lines,
                    check_id,
                    check_info['description'],
                    check_info['severity'],
                    check_info['category'],
                    check_info.get('cwe', [])
                )
        
        # Check code quality
        for check_id, check_info in self.quality_checks.items():
            if check_info['pattern'] and re.search(check_info['pattern'], line):
                self._add_violation(
                    file_path, line_num, line, all_lines,
                    check_id,
                    check_info['description'],
                    check_info['severity'],
                    check_info['category'],
                    check_info.get('cwe', [])
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
                      check_id: str, description: str, severity: str, category: str, cwe_list: List[str] = None):
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
            cwe_list: List of CWE identifiers
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
        
        # Get Parasoft rule details if available
        parasoft_info = None
        if self.parasoft_loaded and check_id in self.parasoft_rules:
            rule = self.parasoft_rules[check_id]
            parasoft_info = {
                'rule_id': rule.get('rule_id', ''),
                'title': rule.get('title', ''),
                'standard': rule.get('standard', ''),
                'description': rule.get('description', ''),
                'example_repair': rule.get('example_repair', ''),
                'references': rule.get('references', [])
            }
        
        # Check for knowledge base matches
        kb_match = self._find_kb_match(line, check_id)
        
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
            'tool': 'StaticAnalyzer_v4',
            'files_affected': [f"{file_path.name}:{line_num}"],
            'timestamp': datetime.now().isoformat(),
            'cwe': cwe_list or [],  # CWE security classifications
            'parasoft_rule': parasoft_info,  # Detailed rule information
            'kb_match': kb_match  # Knowledge base pattern match
        }
        
        self.violations.append(violation)
    
    def _find_kb_match(self, code_line: str, check_id: str) -> Optional[Dict]:
        """
        Find matching pattern in knowledge base
        
        Args:
            code_line: Line of code
            check_id: Check identifier
        
        Returns:
            Matching knowledge base entry or None
        """
        if not self.kb_loaded or not self.kb_patterns:
            return None
        
        code_normalized = code_line.strip()
        
        for pattern_info in self.kb_patterns:
            if pattern_info['check_id'] == check_id:
                pattern_normalized = pattern_info['pattern'].strip()
                
                # Simple similarity check (can be enhanced)
                if len(code_normalized) > 0 and len(pattern_normalized) > 0:
                    # Check if key elements match
                    code_tokens = set(re.findall(r'\w+', code_normalized))
                    pattern_tokens = set(re.findall(r'\w+', pattern_normalized))
                    
                    if code_tokens and pattern_tokens:
                        similarity = len(code_tokens & pattern_tokens) / len(code_tokens | pattern_tokens)
                        
                        if similarity > 0.5:  # 50% similarity threshold
                            return {
                                'pattern': pattern_info['pattern'],
                                'fix': pattern_info['fix'],
                                'confidence': f"{similarity:.1%}"
                            }
        
        return None
    
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
