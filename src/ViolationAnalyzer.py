"""
Violation Analyzer
Analyzes Parasoft violations and integrates with AI for intelligent analysis
"""

import logging
from typing import Dict, List
from datetime import datetime
from KnowledgeDatabaseManager import KnowledgeDatabaseManager

logger = logging.getLogger(__name__)


class ViolationAnalyzer:
    """Analyzes Parasoft violations and updates knowledge base"""
    
    def __init__(self, module_name: str, kb_manager: KnowledgeDatabaseManager):
        """
        Initialize the Violation Analyzer
        
        Args:
            module_name: Name of the module being analyzed
            kb_manager: Knowledge database manager instance
        """
        self.module_name = module_name
        self.kb_manager = kb_manager
        self.kb_manager.load_knowledge_base(module_name)
        
        self.new_violations = []
        self.existing_violations = []
        
        logger.info(f"Violation Analyzer initialized for module: {module_name}")
    
    def analyze_violations(self, violations: List[Dict]) -> Dict:
        """
        Analyze a list of violations
        
        Args:
            violations: List of violation dictionaries from Parasoft report
        
        Returns:
            Analysis results dictionary
        """
        logger.info(f"Analyzing {len(violations)} violations")
        
        new_count = 0
        existing_count = 0
        
        for violation in violations:
            is_new = self.kb_manager.add_violation(violation)
            
            if is_new:
                new_count += 1
                self.new_violations.append(violation)
                self._analyze_new_violation(violation)
            else:
                existing_count += 1
                self.existing_violations.append(violation)
        
        # Update statistics
        analysis_info = {
            'violations_found': len(violations),
            'new_violations': new_count
        }
        self.kb_manager.update_statistics(analysis_info)
        
        results = {
            'total_violations': len(violations),
            'new_unique_count': new_count,
            'existing_count': existing_count,
            'new_violations': self.new_violations,
            'existing_violations': self.existing_violations
        }
        
        logger.info(f"Analysis complete: {new_count} new, {existing_count} existing violations")
        
        return results
    
    def _analyze_new_violation(self, violation: Dict):
        """
        Perform AI-powered analysis on a new violation
        
        Args:
            violation: Violation dictionary
        """
        violation_id = violation.get('Violation ID')
        violation_text = violation.get('Violation', '')
        
        # Analyze violation characteristics
        analysis_notes = []
        
        # Determine if violation is security-related
        if self._is_security_violation(violation_id, violation_text):
            analysis_notes.append("Security-related violation - high priority")
        
        # Determine if violation is memory-related
        if self._is_memory_violation(violation_text):
            analysis_notes.append("Memory-related violation - potential runtime issue")
        
        # Determine if violation is threading-related
        if self._is_threading_violation(violation_text):
            analysis_notes.append("Threading/concurrency violation - check synchronization")
        
        # Determine complexity of fix
        fix_complexity = self._estimate_fix_complexity(violation_text)
        analysis_notes.append(f"Estimated fix complexity: {fix_complexity}")
        
        # Add analysis notes to knowledge base
        for note in analysis_notes:
            self.kb_manager.add_analysis_note(violation_id, note)
        
        logger.debug(f"New violation analyzed: {violation_id}")
    
    def _is_security_violation(self, violation_id: str, violation_text: str) -> bool:
        """Check if violation is security-related"""
        security_keywords = [
            'buffer', 'overflow', 'underflow', 'injection', 'sanitize',
            'validate', 'authentication', 'authorization', 'encrypt',
            'security', 'CWE', 'CERT-C', 'CERT-CPP'
        ]
        
        text_combined = f"{violation_id} {violation_text}".upper()
        return any(keyword.upper() in text_combined for keyword in security_keywords)
    
    def _is_memory_violation(self, violation_text: str) -> bool:
        """Check if violation is memory-related"""
        memory_keywords = [
            'memory', 'malloc', 'free', 'null pointer', 'dereference',
            'leak', 'allocation', 'deallocation', 'dangling', 'uninitialized'
        ]
        
        text_upper = violation_text.upper()
        return any(keyword.upper() in text_upper for keyword in memory_keywords)
    
    def _is_threading_violation(self, violation_text: str) -> bool:
        """Check if violation is threading-related"""
        threading_keywords = [
            'thread', 'mutex', 'lock', 'race condition', 'deadlock',
            'synchronization', 'atomic', 'concurrent', 'parallel'
        ]
        
        text_upper = violation_text.upper()
        return any(keyword.upper() in text_upper for keyword in threading_keywords)
    
    def _estimate_fix_complexity(self, violation_text: str) -> str:
        """Estimate the complexity of fixing the violation"""
        text_upper = violation_text.upper()
        
        # High complexity indicators
        high_complexity_keywords = [
            'ARCHITECTURE', 'DESIGN', 'REFACTOR', 'MULTIPLE',
            'THROUGHOUT', 'ENTIRE', 'SYSTEM-WIDE'
        ]
        
        # Low complexity indicators
        low_complexity_keywords = [
            'ADD', 'REMOVE', 'CHANGE', 'REPLACE', 'SIMPLE',
            'SINGLE', 'ONE', 'CAST', 'DECLARATION'
        ]
        
        if any(keyword in text_upper for keyword in high_complexity_keywords):
            return 'HIGH'
        elif any(keyword in text_upper for keyword in low_complexity_keywords):
            return 'LOW'
        else:
            return 'MEDIUM'
    
    def get_violations_by_severity(self, severity: str) -> List[Dict]:
        """
        Get violations filtered by severity
        
        Args:
            severity: Severity level (HIGH, MEDIUM, LOW)
        
        Returns:
            List of violations with the specified severity
        """
        return self.kb_manager.get_all_violations({'severity': severity})
    
    def get_violations_by_category(self, category: str) -> List[Dict]:
        """
        Get violations filtered by category
        
        Args:
            category: Category (CERT, MISRA, CWE, OTHER)
        
        Returns:
            List of violations in the specified category
        """
        return self.kb_manager.get_all_violations({'category': category})
    
    def get_unfixed_violations(self) -> List[Dict]:
        """Get all violations that haven't been fixed"""
        return self.kb_manager.get_all_violations({'fix_applied': False})
    
    def get_unjustified_violations(self) -> List[Dict]:
        """Get all violations without justifications"""
        return self.kb_manager.get_all_violations({'justification_added': False})
    
    def generate_analysis_report(self) -> Dict:
        """
        Generate a comprehensive analysis report
        
        Returns:
            Dictionary containing analysis report data
        """
        all_violations = self.kb_manager.get_all_violations()
        
        # Count by severity
        severity_counts = {
            'HIGH': len([v for v in all_violations if v.get('severity') == 'HIGH']),
            'MEDIUM': len([v for v in all_violations if v.get('severity') == 'MEDIUM']),
            'LOW': len([v for v in all_violations if v.get('severity') == 'LOW'])
        }
        
        # Count by category
        category_counts = {
            'CERT': len([v for v in all_violations if v.get('category') == 'CERT']),
            'MISRA': len([v for v in all_violations if v.get('category') == 'MISRA']),
            'CWE': len([v for v in all_violations if v.get('category') == 'CWE']),
            'OTHER': len([v for v in all_violations if v.get('category') == 'OTHER'])
        }
        
        # Count fixes and justifications
        fixed_count = len([v for v in all_violations if v.get('fix_applied')])
        justified_count = len([v for v in all_violations if v.get('justification_added')])
        
        # Get most common violations
        top_violations = sorted(
            all_violations,
            key=lambda x: x.get('occurrence_count', 0),
            reverse=True
        )[:5]
        
        report = {
            'module_name': self.module_name,
            'timestamp': datetime.now().isoformat(),
            'total_unique_violations': len(all_violations),
            'severity_breakdown': severity_counts,
            'category_breakdown': category_counts,
            'fixes_applied': fixed_count,
            'justifications_added': justified_count,
            'top_violations': [
                {
                    'violation_id': v['violation_id'],
                    'occurrence_count': v['occurrence_count'],
                    'severity': v['severity']
                }
                for v in top_violations
            ],
            'priority_violations': [
                v for v in all_violations
                if v.get('severity') == 'HIGH' and not v.get('fix_applied')
            ]
        }
        
        return report
    
    def get_ai_analysis_prompt(self, violation: Dict) -> str:
        """
        Generate an AI analysis prompt for a specific violation
        
        Args:
            violation: Violation dictionary
        
        Returns:
            Formatted prompt for AI analysis
        """
        violation_obj = self.kb_manager.get_violation(violation.get('Violation ID'))
        
        if not violation_obj:
            return ""
        
        prompt = f"""
Analyze the following Parasoft violation and provide recommendations:

**Violation Details:**
- ID: {violation_obj['violation_id']}
- Text: {violation_obj['violation_text']}
- Severity: {violation_obj['severity']}
- Category: {violation_obj['category']}
- Occurrence Count: {violation_obj['occurrence_count']}

**Files Affected:**
{self._format_files_affected(violation_obj['files_affected'])}

**Analysis Questions:**
1. What is the root cause of this violation?
2. What are the potential risks if not fixed?
3. What is the recommended fix?
4. Should this violation be justified or fixed?
5. What Parasoft justification format should be used if justified?

Please provide:
- Detailed explanation
- Code fix (if applicable)
- Justification text (if applicable)
- Risk assessment
"""
        return prompt
    
    def _format_files_affected(self, files_affected: List[Dict]) -> str:
        """Format files affected for display"""
        if not files_affected:
            return "None"
        
        formatted = []
        for file_entry in files_affected[:10]:  # Limit to 10 files
            formatted.append(f"  - {file_entry['file']}:{file_entry['line']}")
        
        if len(files_affected) > 10:
            formatted.append(f"  ... and {len(files_affected) - 10} more files")
        
        return "\n".join(formatted)
    
    def export_analysis_report(self, output_path):
        """
        Export analysis report to file
        
        Args:
            output_path: Path for the output file
        """
        import json
        
        report = self.generate_analysis_report()
        
        with open(output_path, 'w', encoding='utf-8') as f:
            json.dump(report, f, indent=2, ensure_ascii=False)
        
        logger.info(f"Analysis report exported: {output_path}")
