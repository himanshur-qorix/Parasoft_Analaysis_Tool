"""
Polyspace Report Parser
Parses Polyspace Code Prover/Bug Finder reports and extracts findings
Supports HTML, Excel, and TSV export formats

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 3.0.0
"""

import logging
import re
from pathlib import Path
from typing import Dict, List, Optional, Tuple
from bs4 import BeautifulSoup
import pandas as pd

logger = logging.getLogger(__name__)


class PolyspaceParser:
    """Parser for Polyspace static analysis reports"""
    
    def __init__(self):
        """Initialize Polyspace parser"""
        self.violations = []
        self.summary = {
            'total_findings': 0,
            'red_findings': 0,      # Proven errors
            'orange_findings': 0,   # Potential errors
            'green_findings': 0,    # Proven safe
            'gray_findings': 0,     # Unreachable/dead code
            'by_category': {},
            'by_severity': {}
        }
        
        # Polyspace color code mapping to severity
        self.color_to_severity = {
            'red': 'CRITICAL',
            'orange': 'HIGH',
            'gray': 'MEDIUM',
            'green': 'LOW'
        }
        
        # Polyspace check family categories
        self.check_categories = {
            'RTE': 'Runtime Error',
            'MISRA': 'MISRA C/C++',
            'CERT': 'CERT C/C++',
            'CWE': 'Common Weakness Enumeration',
            'AUTOSAR': 'AUTOSAR C++',
            'JSF': 'JSF C++',
            'DATA': 'Data Flow',
            'NUMERICAL': 'Numerical Accuracy',
            'CONCURRENCY': 'Concurrency',
            'OVERFLOW': 'Integer Overflow',
            'DIV': 'Division by Zero',
            'NIV': 'Non-initialized Variable',
            'OVFL': 'Overflow',
            'ZDV': 'Zero Division',
            'ABS': 'Absolute Value',
            'IDP': 'Illegal Dereference',
            'NTC': 'Non-terminating Call'
        }
    
    def parse_report(self, report_path: Path) -> List[Dict]:
        """
        Parse Polyspace report (auto-detect format)
        
        Args:
            report_path: Path to Polyspace report file
        
        Returns:
            List of violation dictionaries
        """
        if not report_path.exists():
            raise FileNotFoundError(f"Report file not found: {report_path}")
        
        # Check if it's a directory instead of a file
        if report_path.is_dir():
            raise ValueError(f"Expected a file path, but got a directory: {report_path}\n"
                           f"Please provide the full path to the Polyspace report file, not just the folder.\n"
                           f"Example: {report_path}\\polyspace_results.html")
        
        file_ext = report_path.suffix.lower()
        
        if file_ext == '.html':
            return self.parse_html_report(report_path)
        elif file_ext in ['.xlsx', '.xls']:
            return self.parse_excel_report(report_path)
        elif file_ext in ['.tsv', '.txt']:
            return self.parse_tsv_report(report_path)
        else:
            logger.warning(f"Unknown file format: {file_ext}. Attempting HTML parse...")
            return self.parse_html_report(report_path)
    
    def parse_html_report(self, html_path: Path) -> List[Dict]:
        """
        Parse Polyspace HTML report
        
        Args:
            html_path: Path to HTML report
        
        Returns:
            List of violation dictionaries
        """
        logger.info(f"Parsing Polyspace HTML report: {html_path}")
        
        with open(html_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        soup = BeautifulSoup(content, 'html.parser')
        violations = []
        
        # Find findings table (Polyspace uses specific table structures)
        tables = soup.find_all('table')
        
        for table in tables:
            rows = table.find_all('tr')
            
            # Skip if not enough rows
            if len(rows) < 2:
                continue
            
            # Check if this is a findings table (has Check, File, Line columns)
            headers = [th.get_text(strip=True).lower() for th in rows[0].find_all(['th', 'td'])]
            
            if not any(h in headers for h in ['check', 'file', 'line', 'finding']):
                continue
            
            logger.info(f"Found findings table with headers: {headers}")
            
            # Extract column indices
            check_idx = next((i for i, h in enumerate(headers) if 'check' in h), None)
            file_idx = next((i for i, h in enumerate(headers) if 'file' in h), None)
            line_idx = next((i for i, h in enumerate(headers) if 'line' in h), None)
            color_idx = next((i for i, h in enumerate(headers) if 'color' in h or 'status' in h), None)
            comment_idx = next((i for i, h in enumerate(headers) if 'comment' in h or 'description' in h), None)
            
            # Parse data rows
            for row in rows[1:]:
                cells = row.find_all(['td', 'th'])
                
                if len(cells) < 3:
                    continue
                
                try:
                    # Extract check ID and color
                    check_cell = cells[check_idx] if check_idx is not None else None
                    check_id = check_cell.get_text(strip=True) if check_cell else 'UNKNOWN'
                    
                    # Try to extract color from class or style
                    color = self._extract_color_from_cell(row, cells[color_idx] if color_idx is not None else check_cell)
                    
                    # Extract file and line
                    file_name = cells[file_idx].get_text(strip=True) if file_idx is not None else 'unknown'
                    line_num = cells[line_idx].get_text(strip=True) if line_idx is not None else '0'
                    
                    # Extract comment/description
                    comment = cells[comment_idx].get_text(strip=True) if comment_idx is not None else ''
                    
                    # Create violation dict
                    violation = self._create_violation_dict(
                        check_id, color, file_name, line_num, comment
                    )
                    violations.append(violation)
                    
                except Exception as e:
                    logger.debug(f"Error parsing row: {e}")
                    continue
        
        self.violations = violations
        self._update_summary()
        
        logger.info(f"Parsed {len(violations)} Polyspace findings")
        return violations
    
    def parse_excel_report(self, excel_path: Path) -> List[Dict]:
        """
        Parse Polyspace Excel export
        
        Args:
            excel_path: Path to Excel file
        
        Returns:
            List of violation dictionaries
        """
        logger.info(f"Parsing Polyspace Excel report: {excel_path}")
        
        try:
            # Try reading all sheets
            df_dict = pd.read_excel(excel_path, sheet_name=None)
            
            # Find the sheet with findings (usually 'Results' or 'Findings')
            df = None
            for sheet_name, sheet_df in df_dict.items():
                if any(col.lower() in ['check', 'file', 'line'] for col in sheet_df.columns):
                    df = sheet_df
                    logger.info(f"Found findings in sheet: {sheet_name}")
                    break
            
            if df is None:
                # Just use first sheet
                df = list(df_dict.values())[0]
            
            violations = []
            
            # Normalize column names
            df.columns = [col.lower().strip() for col in df.columns]
            
            for _, row in df.iterrows():
                try:
                    check_id = str(row.get('check', row.get('id', 'UNKNOWN')))
                    color = str(row.get('color', row.get('status', row.get('result', 'gray')))).lower()
                    file_name = str(row.get('file', row.get('filename', 'unknown')))
                    line_num = str(row.get('line', row.get('line number', '0')))
                    comment = str(row.get('comment', row.get('description', row.get('message', ''))))
                    
                    violation = self._create_violation_dict(
                        check_id, color, file_name, line_num, comment
                    )
                    violations.append(violation)
                    
                except Exception as e:
                    logger.debug(f"Error parsing Excel row: {e}")
                    continue
            
            self.violations = violations
            self._update_summary()
            
            logger.info(f"Parsed {len(violations)} Polyspace findings from Excel")
            return violations
            
        except Exception as e:
            logger.error(f"Failed to parse Excel report: {e}")
            return []
    
    def parse_tsv_report(self, tsv_path: Path) -> List[Dict]:
        """
        Parse Polyspace TSV/CSV export
        
        Args:
            tsv_path: Path to TSV/CSV file
        
        Returns:
            List of violation dictionaries
        """
        logger.info(f"Parsing Polyspace TSV report: {tsv_path}")
        
        try:
            # Try tab-separated first, then comma
            try:
                df = pd.read_csv(tsv_path, sep='\t')
            except:
                df = pd.read_csv(tsv_path)
            
            violations = []
            
            # Normalize column names
            df.columns = [col.lower().strip() for col in df.columns]
            
            for _, row in df.iterrows():
                try:
                    check_id = str(row.get('check', row.get('id', 'UNKNOWN')))
                    color = str(row.get('color', row.get('status', 'gray'))).lower()
                    file_name = str(row.get('file', row.get('filename', 'unknown')))
                    line_num = str(row.get('line', '0'))
                    comment = str(row.get('comment', row.get('description', '')))
                    
                    violation = self._create_violation_dict(
                        check_id, color, file_name, line_num, comment
                    )
                    violations.append(violation)
                    
                except Exception as e:
                    logger.debug(f"Error parsing TSV row: {e}")
                    continue
            
            self.violations = violations
            self._update_summary()
            
            logger.info(f"Parsed {len(violations)} Polyspace findings from TSV")
            return violations
            
        except Exception as e:
            logger.error(f"Failed to parse TSV report: {e}")
            return []
    
    def _extract_color_from_cell(self, row, cell) -> str:
        """
        Extract Polyspace color code from HTML cell
        
        Args:
            row: Table row element
            cell: Table cell element
        
        Returns:
            Color string (red/orange/green/gray)
        """
        if cell is None:
            return 'gray'
        
        # Check class attribute
        cell_class = cell.get('class', [])
        for cls in cell_class:
            cls_lower = str(cls).lower()
            if 'red' in cls_lower:
                return 'red'
            elif 'orange' in cls_lower:
                return 'orange'
            elif 'green' in cls_lower:
                return 'green'
            elif 'gray' in cls_lower or 'grey' in cls_lower:
                return 'gray'
        
        # Check style attribute for background-color
        style = cell.get('style', '')
        if 'background' in style.lower():
            style_lower = style.lower()
            if 'red' in style_lower or '#ff0000' in style_lower or 'rgb(255,0,0)' in style_lower:
                return 'red'
            elif 'orange' in style_lower or '#ffa500' in style_lower:
                return 'orange'
            elif 'green' in style_lower or '#00ff00' in style_lower or '#008000' in style_lower:
                return 'green'
        
        # Check row class as well
        row_class = row.get('class', [])
        for cls in row_class:
            cls_lower = str(cls).lower()
            if 'red' in cls_lower:
                return 'red'
            elif 'orange' in cls_lower:
                return 'orange'
        
        return 'gray'
    
    def _create_violation_dict(self, check_id: str, color: str, file_name: str, 
                               line_num: str, comment: str) -> Dict:
        """
        Create standardized violation dictionary
        
        Args:
            check_id: Polyspace check ID
            color: Color code (red/orange/green/gray)
            file_name: Source file name
            line_num: Line number
            comment: Finding comment/description
        
        Returns:
            Violation dictionary
        """
        # Extract line number
        try:
            line = int(re.search(r'\d+', str(line_num)).group())
        except:
            line = 0
        
        # Determine category from check ID
        category = self._determine_category(check_id)
        
        # Map color to severity
        severity = self.color_to_severity.get(color.lower(), 'MEDIUM')
        
        # Generate violation ID in Parasoft format
        violation_id = f"POLYSPACE-{check_id.replace(':', '-').replace(' ', '_')}"
        
        # Generate description
        description = self._generate_description(check_id, color, comment)
        
        return {
            'violation_id': violation_id,
            'check_id': check_id,
            'color': color,
            'severity': severity,
            'category': category,
            'violation_text': description,
            'file': file_name,
            'line': line,
            'comment': comment,
            'tool': 'Polyspace',
            'files_affected': [f"{file_name}:{line}"]
        }
    
    def _determine_category(self, check_id: str) -> str:
        """
        Determine category from Polyspace check ID
        
        Args:
            check_id: Check identifier
        
        Returns:
            Category string
        """
        check_upper = check_id.upper()
        
        # Check for known prefixes
        for prefix, category in self.check_categories.items():
            if check_upper.startswith(prefix):
                return category
        
        # Check for MISRA/CERT in the ID
        if 'MISRA' in check_upper:
            return 'MISRA C/C++'
        elif 'CERT' in check_upper:
            return 'CERT C/C++'
        elif 'CWE' in check_upper:
            return 'CWE'
        
        return 'Runtime Error'  # Default for Polyspace
    
    def _generate_description(self, check_id: str, color: str, comment: str) -> str:
        """
        Generate human-readable description
        
        Args:
            check_id: Check ID
            color: Color code
            comment: User comment
        
        Returns:
            Description string
        """
        color_descriptions = {
            'red': 'Proven runtime error',
            'orange': 'Potential runtime error (analysis inconclusive)',
            'green': 'Proven safe (no runtime error possible)',
            'gray': 'Unreachable code or deactivated check'
        }
        
        color_desc = color_descriptions.get(color.lower(), 'Analysis result')
        
        if comment and comment not in ['', 'nan', 'None']:
            return f"{check_id}: {color_desc}. {comment}"
        else:
            return f"{check_id}: {color_desc}"
    
    def _update_summary(self):
        """Update summary statistics from parsed violations"""
        self.summary['total_findings'] = len(self.violations)
        
        # Count by color
        for v in self.violations:
            color = v['color'].lower()
            if color == 'red':
                self.summary['red_findings'] += 1
            elif color == 'orange':
                self.summary['orange_findings'] += 1
            elif color == 'green':
                self.summary['green_findings'] += 1
            elif color == 'gray':
                self.summary['gray_findings'] += 1
            
            # Count by category
            category = v['category']
            self.summary['by_category'][category] = self.summary['by_category'].get(category, 0) + 1
            
            # Count by severity
            severity = v['severity']
            self.summary['by_severity'][severity] = self.summary['by_severity'].get(severity, 0) + 1
    
    def get_summary(self) -> Dict:
        """Get summary statistics"""
        return self.summary
    
    def filter_by_color(self, color: str) -> List[Dict]:
        """Filter violations by Polyspace color"""
        return [v for v in self.violations if v['color'].lower() == color.lower()]
    
    def filter_by_severity(self, severity: str) -> List[Dict]:
        """Filter violations by severity"""
        return [v for v in self.violations if v['severity'] == severity]
    
    def filter_by_category(self, category: str) -> List[Dict]:
        """Filter violations by category"""
        return [v for v in self.violations if v['category'] == category]


if __name__ == '__main__':
    # Test parser
    import sys
    
    logging.basicConfig(level=logging.INFO)
    
    if len(sys.argv) > 1:
        parser = PolyspaceParser()
        report_path = Path(sys.argv[1])
        
        violations = parser.parse_report(report_path)
        
        print(f"\n{'='*80}")
        print(f"Polyspace Report Summary")
        print(f"{'='*80}")
        print(f"Total Findings: {parser.summary['total_findings']}")
        print(f"  Red (Proven errors): {parser.summary['red_findings']}")
        print(f"  Orange (Potential errors): {parser.summary['orange_findings']}")
        print(f"  Green (Proven safe): {parser.summary['green_findings']}")
        print(f"  Gray (Unreachable): {parser.summary['gray_findings']}")
        print(f"\nBy Category:")
        for cat, count in parser.summary['by_category'].items():
            print(f"  {cat}: {count}")
        
        print(f"\nSample violations:")
        for v in violations[:5]:
            print(f"  {v['violation_id']} - {v['file']}:{v['line']} - {v['severity']}")
    else:
        print("Usage: python PolyspaceParser.py <report_file>")
