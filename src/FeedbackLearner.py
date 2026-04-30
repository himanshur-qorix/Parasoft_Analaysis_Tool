"""
Feedback Learning System
Learns from false positives/negatives to improve MISRA/CERT detection and suppression accuracy

Version: 1.0.0
Developer: Himanshu R
Date: April 30, 2026
"""

import json
import logging
from pathlib import Path
from datetime import datetime
from typing import Dict, List, Optional, Tuple
from collections import defaultdict
import re

# Configure UTF-8 encoding for Windows console
import sys
if sys.platform == 'win32':
    try:
        import ctypes
        kernel32 = ctypes.windll.kernel32
        kernel32.SetConsoleOutputCP(65001)  # UTF-8
        sys.stdout.reconfigure(encoding='utf-8', errors='replace')
        sys.stderr.reconfigure(encoding='utf-8', errors='replace')
    except:
        pass

logger = logging.getLogger(__name__)


class FeedbackType:
    """Feedback types for learning"""
    FALSE_POSITIVE = "false_positive"  # Tool flagged incorrectly
    FALSE_NEGATIVE = "false_negative"  # Tool missed real violation
    CORRECT_DETECTION = "correct"      # Tool flagged correctly
    SUPPRESSION_ERROR = "suppression_error"  # Suppression applied incorrectly


class FeedbackEntry:
    """Single feedback entry for learning"""
    def __init__(self, 
                 feedback_type: str,
                 rule_id: str,
                 code_snippet: str,
                 file_path: str,
                 line_number: int,
                 reason: str,
                 module_name: str = "",
                 pattern_used: str = "",
                 context: Dict = None):
        self.feedback_type = feedback_type
        self.rule_id = rule_id
        self.code_snippet = code_snippet
        self.file_path = file_path
        self.line_number = line_number
        self.reason = reason
        self.module_name = module_name
        self.pattern_used = pattern_used
        self.context = context or {}
        self.timestamp = datetime.now().isoformat()
        self.feedback_id = f"{rule_id}_{file_path}_{line_number}_{datetime.now().strftime('%Y%m%d%H%M%S')}"
    
    def to_dict(self) -> Dict:
        """Convert to dictionary for JSON storage"""
        return {
            'feedback_id': self.feedback_id,
            'feedback_type': self.feedback_type,
            'rule_id': self.rule_id,
            'code_snippet': self.code_snippet,
            'file_path': self.file_path,
            'line_number': self.line_number,
            'reason': self.reason,
            'module_name': self.module_name,
            'pattern_used': self.pattern_used,
            'context': self.context,
            'timestamp': self.timestamp
        }


class FeedbackLearner:
    """
    Regression learning system for MISRA/CERT detection and suppression
    
    Features:
    - Tracks false positives/negatives
    - Learns from user feedback
    - Refines detection patterns
    - Improves suppression accuracy
    - Generates reports on learning progress
    """
    
    def __init__(self, project_root: Path):
        """
        Initialize feedback learner
        
        Args:
            project_root: Project root directory
        """
        self.project_root = Path(project_root)
        self.learning_dir = self.project_root / "learning" / "feedback"
        self.learning_dir.mkdir(parents=True, exist_ok=True)
        
        # Learning databases
        self.feedback_db_path = self.learning_dir / "feedback_database.json"
        self.pattern_refinements_path = self.learning_dir / "pattern_refinements.json"
        self.suppression_learning_path = self.learning_dir / "suppression_learning.json"
        
        # Load existing data
        self.feedback_db = self._load_json(self.feedback_db_path, default={
            'metadata': {
                'version': '1.0.0',
                'created': datetime.now().isoformat(),
                'total_feedback': 0
            },
            'feedback_entries': [],
            'statistics': {
                'false_positives': 0,
                'false_negatives': 0,
                'correct_detections': 0,
                'suppression_errors': 0
            }
        })
        
        self.pattern_refinements = self._load_json(self.pattern_refinements_path, default={})
        self.suppression_learning = self._load_json(self.suppression_learning_path, default={})
        
        logger.info(f"[INFO] Feedback Learner initialized: {len(self.feedback_db['feedback_entries'])} feedback entries loaded")
    
    def _load_json(self, path: Path, default: Dict = None) -> Dict:
        """Load JSON file with fallback to default"""
        if path.exists():
            try:
                with open(path, 'r', encoding='utf-8') as f:
                    return json.load(f)
            except Exception as e:
                logger.warning(f"[WARNING] Could not load {path}: {e}")
                return default or {}
        return default or {}
    
    def _save_json(self, data: Dict, path: Path):
        """Save data to JSON file"""
        try:
            with open(path, 'w', encoding='utf-8') as f:
                json.dump(data, f, indent=2, ensure_ascii=False)
        except Exception as e:
            logger.error(f"[ERROR] Could not save {path}: {e}")
    
    def add_feedback(self, 
                     feedback_type: str,
                     rule_id: str,
                     code_snippet: str,
                     file_path: str,
                     line_number: int,
                     reason: str,
                     module_name: str = "",
                     pattern_used: str = "",
                     context: Dict = None) -> str:
        """
        Add feedback entry for learning
        
        Args:
            feedback_type: Type of feedback (false_positive, false_negative, etc.)
            rule_id: MISRA/CERT rule ID
            code_snippet: Code that triggered the violation/suppression
            file_path: File where violation occurred
            line_number: Line number
            reason: Reason for feedback (why it's wrong/correct)
            module_name: Module name (for context)
            pattern_used: Pattern that was used for detection
            context: Additional context information
        
        Returns:
            Feedback ID for reference
        """
        entry = FeedbackEntry(
            feedback_type=feedback_type,
            rule_id=rule_id,
            code_snippet=code_snippet,
            file_path=file_path,
            line_number=line_number,
            reason=reason,
            module_name=module_name,
            pattern_used=pattern_used,
            context=context
        )
        
        # Add to database
        self.feedback_db['feedback_entries'].append(entry.to_dict())
        self.feedback_db['metadata']['total_feedback'] += 1
        self.feedback_db['statistics'][feedback_type] = self.feedback_db['statistics'].get(feedback_type, 0) + 1
        
        # Save to disk
        self._save_json(self.feedback_db, self.feedback_db_path)
        
        # Process feedback for learning
        self._process_feedback(entry)
        
        logger.info(f"[INFO] Feedback recorded: {feedback_type} for {rule_id} at {file_path}:{line_number}")
        return entry.feedback_id
    
    def _process_feedback(self, entry: FeedbackEntry):
        """Process feedback to refine patterns and improve detection"""
        if entry.feedback_type == FeedbackType.FALSE_POSITIVE:
            self._refine_pattern_for_false_positive(entry)
        elif entry.feedback_type == FeedbackType.FALSE_NEGATIVE:
            self._add_pattern_for_false_negative(entry)
        elif entry.feedback_type == FeedbackType.SUPPRESSION_ERROR:
            self._learn_suppression_error(entry)
    
    def _refine_pattern_for_false_positive(self, entry: FeedbackEntry):
        """
        Refine pattern to avoid false positive
        
        Strategy:
        - Add exclusion patterns for common false positive cases
        - Create negative examples for ML training
        - Adjust pattern confidence scores
        """
        rule_id = entry.rule_id
        
        if rule_id not in self.pattern_refinements:
            self.pattern_refinements[rule_id] = {
                'false_positive_patterns': [],
                'exclusion_patterns': [],
                'confidence_adjustment': 0.0
            }
        
        # Extract pattern from code snippet
        false_positive_pattern = self._extract_pattern_features(entry.code_snippet)
        
        self.pattern_refinements[rule_id]['false_positive_patterns'].append({
            'pattern': false_positive_pattern,
            'code_snippet': entry.code_snippet,
            'reason': entry.reason,
            'timestamp': entry.timestamp
        })
        
        # Lower confidence for this rule (more conservative detection)
        current_adjustment = self.pattern_refinements[rule_id]['confidence_adjustment']
        self.pattern_refinements[rule_id]['confidence_adjustment'] = max(current_adjustment - 0.1, -1.0)
        
        # Generate exclusion pattern
        exclusion = self._generate_exclusion_pattern(entry)
        if exclusion:
            self.pattern_refinements[rule_id]['exclusion_patterns'].append(exclusion)
        
        self._save_json(self.pattern_refinements, self.pattern_refinements_path)
        logger.info(f"[LEARN] Refined pattern for {rule_id} to avoid false positives")
    
    def _add_pattern_for_false_negative(self, entry: FeedbackEntry):
        """
        Add new pattern to catch missed violations
        
        Strategy:
        - Create new detection patterns from missed cases
        - Increase sensitivity for this rule
        - Add to positive examples for ML training
        """
        rule_id = entry.rule_id
        
        if rule_id not in self.pattern_refinements:
            self.pattern_refinements[rule_id] = {
                'false_negative_patterns': [],
                'additional_patterns': [],
                'confidence_adjustment': 0.0
            }
        
        # Extract pattern from missed code
        missed_pattern = self._extract_pattern_features(entry.code_snippet)
        
        if 'false_negative_patterns' not in self.pattern_refinements[rule_id]:
            self.pattern_refinements[rule_id]['false_negative_patterns'] = []
        
        self.pattern_refinements[rule_id]['false_negative_patterns'].append({
            'pattern': missed_pattern,
            'code_snippet': entry.code_snippet,
            'reason': entry.reason,
            'timestamp': entry.timestamp
        })
        
        # Increase confidence for this rule (more aggressive detection)
        current_adjustment = self.pattern_refinements[rule_id].get('confidence_adjustment', 0.0)
        self.pattern_refinements[rule_id]['confidence_adjustment'] = min(current_adjustment + 0.1, 1.0)
        
        # Generate additional pattern
        new_pattern = self._generate_detection_pattern(entry)
        if new_pattern:
            if 'additional_patterns' not in self.pattern_refinements[rule_id]:
                self.pattern_refinements[rule_id]['additional_patterns'] = []
            self.pattern_refinements[rule_id]['additional_patterns'].append(new_pattern)
        
        self._save_json(self.pattern_refinements, self.pattern_refinements_path)
        logger.info(f"[LEARN] Added new pattern for {rule_id} to catch missed violations")
    
    def _learn_suppression_error(self, entry: FeedbackEntry):
        """
        Learn from suppression errors
        
        Strategy:
        - Track where suppressions were applied incorrectly
        - Learn correct suppression format patterns
        - Improve reference generation logic
        """
        rule_id = entry.rule_id
        module_name = entry.module_name
        
        key = f"{module_name}_{rule_id}"
        
        if key not in self.suppression_learning:
            self.suppression_learning[key] = {
                'errors': [],
                'corrections': [],
                'patterns_to_avoid': []
            }
        
        self.suppression_learning[key]['errors'].append({
            'code_snippet': entry.code_snippet,
            'reason': entry.reason,
            'context': entry.context,
            'timestamp': entry.timestamp
        })
        
        # Extract lesson from error
        lesson = self._extract_suppression_lesson(entry)
        if lesson:
            self.suppression_learning[key]['patterns_to_avoid'].append(lesson)
        
        self._save_json(self.suppression_learning, self.suppression_learning_path)
        logger.info(f"[LEARN] Learned suppression error for {module_name} - {rule_id}")
    
    def _extract_pattern_features(self, code_snippet: str) -> Dict[str, any]:
        """Extract features from code snippet for pattern learning"""
        features = {
            'length': len(code_snippet),
            'has_comments': '//' in code_snippet or '/*' in code_snippet,
            'has_macros': '#define' in code_snippet or '#include' in code_snippet,
            'has_functions': '(' in code_snippet and ')' in code_snippet,
            'keywords': self._extract_keywords(code_snippet),
            'operators': self._extract_operators(code_snippet),
            'indent_level': len(code_snippet) - len(code_snippet.lstrip())
        }
        return features
    
    def _extract_keywords(self, code: str) -> List[str]:
        """Extract C/C++ keywords from code"""
        c_keywords = ['if', 'else', 'for', 'while', 'do', 'switch', 'case', 
                      'return', 'break', 'continue', 'void', 'int', 'char', 
                      'float', 'double', 'const', 'static', 'typedef']
        found = []
        for keyword in c_keywords:
            if re.search(r'\b' + keyword + r'\b', code):
                found.append(keyword)
        return found
    
    def _extract_operators(self, code: str) -> List[str]:
        """Extract operators from code"""
        operators = ['+', '-', '*', '/', '%', '=', '==', '!=', '<', '>', '<=', '>=', '&&', '||', '!']
        found = []
        for op in operators:
            if op in code:
                found.append(op)
        return found
    
    def _generate_exclusion_pattern(self, entry: FeedbackEntry) -> Optional[Dict]:
        """Generate exclusion pattern from false positive"""
        # Analyze why it's a false positive and create exclusion rule
        code = entry.code_snippet
        reason = entry.reason.lower()
        
        exclusion = {
            'pattern': '',
            'reason': entry.reason,
            'confidence': 0.8
        }
        
        # Common false positive patterns
        if 'comment' in reason:
            exclusion['pattern'] = r'//.*|/\*.*?\*/'
            exclusion['description'] = 'Exclude comments'
        elif 'macro' in reason or 'define' in reason:
            exclusion['pattern'] = r'#define\s+\w+'
            exclusion['description'] = 'Exclude macro definitions'
        elif 'autosar' in reason or 'standard' in reason:
            exclusion['pattern'] = r'\b(AUTOSAR|STD_ON|STD_OFF)\b'
            exclusion['description'] = 'Exclude AUTOSAR standard patterns'
        else:
            # Generic exclusion based on code context
            if code.strip().startswith('//') or code.strip().startswith('/*'):
                exclusion['pattern'] = r'^\s*(/\*|//)'
                exclusion['description'] = 'Exclude comment lines'
            else:
                return None
        
        return exclusion
    
    def _generate_detection_pattern(self, entry: FeedbackEntry) -> Optional[Dict]:
        """Generate new detection pattern from false negative"""
        code = entry.code_snippet
        rule_id = entry.rule_id
        
        # Try to generalize the pattern
        pattern_info = {
            'pattern': '',
            'description': f'Learned from missed violation in {entry.file_path}',
            'confidence': 0.7,
            'source': 'feedback_learning'
        }
        
        # Extract variable names, function calls, etc.
        # This is a simplified version - can be enhanced with more sophisticated analysis
        
        # Look for function calls
        func_match = re.search(r'(\w+)\s*\(', code)
        if func_match:
            func_name = func_match.group(1)
            pattern_info['pattern'] = rf'\b{func_name}\s*\('
            pattern_info['description'] += f' - Function call: {func_name}'
            return pattern_info
        
        # Look for specific constructs based on rule type
        if 'CERT' in rule_id or 'MISRA' in rule_id:
            # Generic pattern based on code structure
            # Remove literals and create pattern
            generalized = re.sub(r'"[^"]*"', r'\".*?\"', code)
            generalized = re.sub(r'\d+', r'\\d+', generalized)
            pattern_info['pattern'] = generalized
            return pattern_info
        
        return None
    
    def _extract_suppression_lesson(self, entry: FeedbackEntry) -> Optional[Dict]:
        """Extract lesson from suppression error"""
        return {
            'error_type': 'suppression_format',
            'module': entry.module_name,
            'rule': entry.rule_id,
            'lesson': entry.reason,
            'example': entry.code_snippet
        }
    
    def should_skip_detection(self, rule_id: str, code_snippet: str) -> bool:
        """
        Check if detection should be skipped based on learned exclusions
        
        Args:
            rule_id: Rule ID to check
            code_snippet: Code to analyze
        
        Returns:
            True if should skip (likely false positive)
        """
        if rule_id not in self.pattern_refinements:
            return False
        
        refinements = self.pattern_refinements[rule_id]
        
        # Check exclusion patterns
        for exclusion in refinements.get('exclusion_patterns', []):
            pattern = exclusion.get('pattern', '')
            if pattern and re.search(pattern, code_snippet):
                logger.debug(f"[SKIP] Skipping {rule_id} - matches exclusion pattern")
                return True
        
        # Check against false positive patterns
        code_features = self._extract_pattern_features(code_snippet)
        for fp_pattern in refinements.get('false_positive_patterns', []):
            if self._matches_pattern_features(code_features, fp_pattern.get('pattern', {})):
                logger.debug(f"[SKIP] Skipping {rule_id} - similar to known false positive")
                return True
        
        return False
    
    def get_confidence_adjustment(self, rule_id: str) -> float:
        """Get confidence adjustment for rule based on learning"""
        if rule_id not in self.pattern_refinements:
            return 0.0
        return self.pattern_refinements[rule_id].get('confidence_adjustment', 0.0)
    
    def get_additional_patterns(self, rule_id: str) -> List[Dict]:
        """Get additional detection patterns learned from false negatives"""
        if rule_id not in self.pattern_refinements:
            return []
        return self.pattern_refinements[rule_id].get('additional_patterns', [])
    
    def _matches_pattern_features(self, features1: Dict, features2: Dict) -> bool:
        """Check if two pattern feature sets are similar"""
        if not features2:
            return False
        
        # Simple similarity check - can be enhanced
        similarity_score = 0
        total_features = 0
        
        for key in ['has_comments', 'has_macros', 'has_functions']:
            if key in features1 and key in features2:
                total_features += 1
                if features1[key] == features2[key]:
                    similarity_score += 1
        
        # Check keyword overlap
        if 'keywords' in features1 and 'keywords' in features2:
            keywords1 = set(features1['keywords'])
            keywords2 = set(features2['keywords'])
            if keywords1 and keywords2:
                overlap = len(keywords1 & keywords2) / len(keywords1 | keywords2)
                similarity_score += overlap
                total_features += 1
        
        if total_features == 0:
            return False
        
        similarity = similarity_score / total_features
        return similarity > 0.7  # 70% similarity threshold
    
    def generate_feedback_report(self, output_path: Path = None) -> str:
        """
        Generate comprehensive feedback report
        
        Args:
            output_path: Optional path to save report
        
        Returns:
            Report text
        """
        stats = self.feedback_db['statistics']
        total = self.feedback_db['metadata']['total_feedback']
        
        report = f"""
{'='*80}
FEEDBACK LEARNING SYSTEM REPORT
{'='*80}

Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}
Total Feedback Entries: {total}

STATISTICS:
-----------
False Positives: {stats.get('false_positive', 0)} ({stats.get('false_positive', 0)/max(total,1)*100:.1f}%)
False Negatives: {stats.get('false_negative', 0)} ({stats.get('false_negative', 0)/max(total,1)*100:.1f}%)
Correct Detections: {stats.get('correct', 0)} ({stats.get('correct', 0)/max(total,1)*100:.1f}%)
Suppression Errors: {stats.get('suppression_error', 0)} ({stats.get('suppression_error', 0)/max(total,1)*100:.1f}%)

PATTERN REFINEMENTS:
--------------------
Rules with refinements: {len(self.pattern_refinements)}

"""
        
        # Add details for each refined rule
        for rule_id, refinements in self.pattern_refinements.items():
            fp_count = len(refinements.get('false_positive_patterns', []))
            fn_count = len(refinements.get('false_negative_patterns', []))
            confidence_adj = refinements.get('confidence_adjustment', 0.0)
            
            report += f"\n{rule_id}:\n"
            report += f"  - False positive patterns learned: {fp_count}\n"
            report += f"  - False negative patterns learned: {fn_count}\n"
            report += f"  - Confidence adjustment: {confidence_adj:+.2f}\n"
            report += f"  - Exclusion patterns: {len(refinements.get('exclusion_patterns', []))}\n"
            report += f"  - Additional patterns: {len(refinements.get('additional_patterns', []))}\n"
        
        report += f"\n{'='*80}\n"
        
        # Save if path provided
        if output_path:
            output_path = Path(output_path)
            output_path.parent.mkdir(parents=True, exist_ok=True)
            with open(output_path, 'w', encoding='utf-8') as f:
                f.write(report)
            logger.info(f"[INFO] Feedback report saved to: {output_path}")
        
        return report
    
    def export_learning_data(self, output_dir: Path = None) -> Dict[str, Path]:
        """
        Export learning data for analysis or backup
        
        Args:
            output_dir: Directory to export to (default: learning/feedback/export)
        
        Returns:
            Dictionary of exported file paths
        """
        if output_dir is None:
            output_dir = self.learning_dir / "export"
        
        output_dir = Path(output_dir)
        output_dir.mkdir(parents=True, exist_ok=True)
        
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        
        exported_files = {}
        
        # Export feedback database
        feedback_export = output_dir / f"feedback_db_{timestamp}.json"
        self._save_json(self.feedback_db, feedback_export)
        exported_files['feedback_db'] = feedback_export
        
        # Export pattern refinements
        patterns_export = output_dir / f"patterns_{timestamp}.json"
        self._save_json(self.pattern_refinements, patterns_export)
        exported_files['patterns'] = patterns_export
        
        # Export suppression learning
        suppression_export = output_dir / f"suppression_{timestamp}.json"
        self._save_json(self.suppression_learning, suppression_export)
        exported_files['suppression'] = suppression_export
        
        # Generate and export report
        report_export = output_dir / f"report_{timestamp}.txt"
        report = self.generate_feedback_report(report_export)
        exported_files['report'] = report_export
        
        logger.info(f"[INFO] Learning data exported to: {output_dir}")
        return exported_files


if __name__ == "__main__":
    # Example usage
    project_root = Path(__file__).parent.parent
    learner = FeedbackLearner(project_root)
    
    # Example: Add false positive feedback
    learner.add_feedback(
        feedback_type=FeedbackType.FALSE_POSITIVE,
        rule_id="MISRAC2012-RULE_2_3",
        code_snippet="// Unused type definition",
        file_path="test.c",
        line_number=42,
        reason="This is a comment, not actual code",
        module_name="TestModule",
        pattern_used=r"typedef\s+\w+"
    )
    
    # Generate report
    report = learner.generate_feedback_report()
    print(report)
