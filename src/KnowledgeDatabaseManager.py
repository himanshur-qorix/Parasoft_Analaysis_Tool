"""
Knowledge Database Manager
Manages module-specific knowledge databases for Parasoft violations
"""

import json
import logging
from pathlib import Path
from datetime import datetime
from typing import Dict, List, Optional

logger = logging.getLogger(__name__)


class KnowledgeDatabaseManager:
    """Manages knowledge databases for Parasoft violations"""
    
    def __init__(self, knowledge_base_dir: Path):
        """
        Initialize the Knowledge Database Manager
        
        Args:
            knowledge_base_dir: Directory to store knowledge databases
        """
        self.knowledge_base_dir = Path(knowledge_base_dir)
        self.knowledge_base_dir.mkdir(exist_ok=True)
        self.current_module = None
        self.database = {}
    
    def get_database_path(self, module_name: str) -> Path:
        """Get the path to a module's knowledge database"""
        return self.knowledge_base_dir / f"{module_name}_KnowledgeDatabase.json"
    
    def load_knowledge_base(self, module_name: str) -> Dict:
        """
        Load knowledge base for a specific module
        
        Args:
            module_name: Name of the module
        
        Returns:
            Dictionary containing the knowledge base
        """
        self.current_module = module_name
        db_path = self.get_database_path(module_name)
        
        if db_path.exists():
            try:
                with open(db_path, 'r', encoding='utf-8') as f:
                    self.database = json.load(f)
                logger.info(f"Loaded existing knowledge base for {module_name}")
            except Exception as e:
                logger.error(f"Error loading knowledge base: {str(e)}")
                self.database = self._create_new_database(module_name)
        else:
            logger.info(f"Creating new knowledge base for {module_name}")
            self.database = self._create_new_database(module_name)
        
        return self.database
    
    def _create_new_database(self, module_name: str) -> Dict:
        """Create a new knowledge database structure"""
        return {
            "module_name": module_name,
            "created_date": datetime.now().isoformat(),
            "last_updated": datetime.now().isoformat(),
            "version": "1.0",
            "total_unique_violations": 0,
            "violations": {},
            "statistics": {
                "total_analyses": 0,
                "total_violations_processed": 0,
                "most_common_violations": []
            },
            "metadata": {
                "git_commits": [],
                "analysis_history": []
            }
        }
    
    def save_knowledge_base(self, module_name: Optional[str] = None) -> Path:
        """
        Save the current knowledge base to disk
        
        Args:
            module_name: Name of the module (uses current if not provided)
        
        Returns:
            Path to the saved database file
        """
        if module_name is None:
            module_name = self.current_module
        
        if module_name is None:
            raise ValueError("No module name specified")
        
        self.database["last_updated"] = datetime.now().isoformat()
        db_path = self.get_database_path(module_name)
        
        with open(db_path, 'w', encoding='utf-8') as f:
            json.dump(self.database, f, indent=2, ensure_ascii=False)
        
        logger.info(f"Knowledge base saved: {db_path}")
        return db_path
    
    def add_violation(self, violation_data: Dict) -> bool:
        """
        Add or update a violation in the knowledge base
        
        Args:
            violation_data: Dictionary containing violation information
        
        Returns:
            True if new violation, False if updated existing
        """
        violation_id = violation_data.get('Violation ID')
        violation_text = violation_data.get('Violation', '')
        
        if not violation_id:
            logger.warning("Violation ID missing, skipping")
            return False
        
        is_new = violation_id not in self.database['violations']
        
        if is_new:
            # New unique violation
            self.database['violations'][violation_id] = {
                'violation_id': violation_id,
                'violation_text': violation_text,
                'first_seen': datetime.now().isoformat(),
                'last_seen': datetime.now().isoformat(),
                'occurrence_count': 1,
                'files_affected': [],
                'line_numbers': [],
                'justifiable': violation_data.get('Justifiable', 'Analyse'),
                'fix_applied': False,
                'fix_details': None,
                'justification_added': False,
                'justification_text': None,
                'analysis_notes': [],
                'severity': self._determine_severity(violation_id),
                'category': self._determine_category(violation_id)
            }
            self.database['total_unique_violations'] += 1
            logger.info(f"New unique violation added: {violation_id}")
        else:
            # Update existing violation
            self.database['violations'][violation_id]['last_seen'] = datetime.now().isoformat()
            self.database['violations'][violation_id]['occurrence_count'] += 1
            logger.debug(f"Updated existing violation: {violation_id}")
        
        # Add file and line number information
        file_name = violation_data.get('File')
        line_number = violation_data.get('Line number')
        
        if file_name:
            file_entry = {
                'file': file_name,
                'line': line_number,
                'timestamp': datetime.now().isoformat()
            }
            
            if file_entry not in self.database['violations'][violation_id]['files_affected']:
                self.database['violations'][violation_id]['files_affected'].append(file_entry)
            
            if line_number and line_number not in self.database['violations'][violation_id]['line_numbers']:
                self.database['violations'][violation_id]['line_numbers'].append(line_number)
        
        # Update statistics
        self.database['statistics']['total_violations_processed'] += 1
        
        return is_new
    
    def _determine_severity(self, violation_id: str) -> str:
        """Determine violation severity based on ID"""
        violation_id_upper = violation_id.upper()
        
        # CERT rules
        if 'CERT' in violation_id_upper:
            if any(level in violation_id_upper for level in ['L1', 'HIGH', 'CRITICAL']):
                return 'HIGH'
            elif 'L2' in violation_id_upper or 'MEDIUM' in violation_id_upper:
                return 'MEDIUM'
            else:
                return 'LOW'
        
        # MISRA rules
        if 'MISRA' in violation_id_upper:
            if 'MANDATORY' in violation_id_upper or 'REQUIRED' in violation_id_upper:
                return 'HIGH'
            elif 'ADVISORY' in violation_id_upper:
                return 'LOW'
            else:
                return 'MEDIUM'
        
        return 'MEDIUM'
    
    def _determine_category(self, violation_id: str) -> str:
        """Determine violation category based on ID"""
        violation_id_upper = violation_id.upper()
        
        if 'CERT' in violation_id_upper:
            return 'CERT'
        elif 'MISRA' in violation_id_upper:
            return 'MISRA'
        elif 'CWE' in violation_id_upper:
            return 'CWE'
        else:
            return 'OTHER'
    
    def get_violation(self, violation_id: str) -> Optional[Dict]:
        """
        Get information about a specific violation
        
        Args:
            violation_id: The violation ID to lookup
        
        Returns:
            Violation data or None if not found
        """
        return self.database['violations'].get(violation_id)
    
    def get_all_violations(self, filter_by: Optional[Dict] = None) -> List[Dict]:
        """
        Get all violations, optionally filtered
        
        Args:
            filter_by: Dictionary of filters (e.g., {'severity': 'HIGH'})
        
        Returns:
            List of violations matching the filters
        """
        violations = list(self.database['violations'].values())
        
        if filter_by:
            for key, value in filter_by.items():
                violations = [v for v in violations if v.get(key) == value]
        
        return violations
    
    def update_fix_status(self, violation_id: str, fix_details: Dict):
        """
        Update fix status for a violation
        
        Args:
            violation_id: The violation ID
            fix_details: Dictionary containing fix information
        """
        if violation_id in self.database['violations']:
            self.database['violations'][violation_id]['fix_applied'] = True
            self.database['violations'][violation_id]['fix_details'] = fix_details
            logger.info(f"Fix status updated for {violation_id}")
        else:
            logger.warning(f"Violation {violation_id} not found in database")
    
    def update_justification_status(self, violation_id: str, justification_text: str):
        """
        Update justification status for a violation
        
        Args:
            violation_id: The violation ID
            justification_text: The justification text
        """
        if violation_id in self.database['violations']:
            self.database['violations'][violation_id]['justification_added'] = True
            self.database['violations'][violation_id]['justification_text'] = justification_text
            logger.info(f"Justification added for {violation_id}")
        else:
            logger.warning(f"Violation {violation_id} not found in database")
    
    def add_analysis_note(self, violation_id: str, note: str):
        """
        Add an analysis note to a violation
        
        Args:
            violation_id: The violation ID
            note: The note to add
        """
        if violation_id in self.database['violations']:
            note_entry = {
                'timestamp': datetime.now().isoformat(),
                'note': note
            }
            self.database['violations'][violation_id]['analysis_notes'].append(note_entry)
        else:
            logger.warning(f"Violation {violation_id} not found in database")
    
    def update_statistics(self, analysis_info: Dict):
        """
        Update database statistics
        
        Args:
            analysis_info: Dictionary containing analysis information
        """
        self.database['statistics']['total_analyses'] += 1
        
        # Update most common violations
        violation_counts = [(v['violation_id'], v['occurrence_count']) 
                           for v in self.database['violations'].values()]
        violation_counts.sort(key=lambda x: x[1], reverse=True)
        
        self.database['statistics']['most_common_violations'] = [
            {'violation_id': vid, 'count': count}
            for vid, count in violation_counts[:10]
        ]
        
        # Add to analysis history
        history_entry = {
            'timestamp': datetime.now().isoformat(),
            'violations_found': analysis_info.get('violations_found', 0),
            'new_violations': analysis_info.get('new_violations', 0)
        }
        
        if 'git_commit' in analysis_info:
            history_entry['git_commit'] = analysis_info['git_commit']
            
            if analysis_info['git_commit'] not in self.database['metadata']['git_commits']:
                self.database['metadata']['git_commits'].append(analysis_info['git_commit'])
        
        self.database['metadata']['analysis_history'].append(history_entry)
    
    def get_statistics(self) -> Dict:
        """Get database statistics"""
        return {
            'module_name': self.database.get('module_name'),
            'total_unique_violations': self.database.get('total_unique_violations', 0),
            'total_analyses': self.database['statistics'].get('total_analyses', 0),
            'total_violations_processed': self.database['statistics'].get('total_violations_processed', 0),
            'created_date': self.database.get('created_date'),
            'last_updated': self.database.get('last_updated'),
            'most_common_violations': self.database['statistics'].get('most_common_violations', [])
        }
    
    def export_to_excel(self, output_path: Path):
        """
        Export knowledge base to Excel format
        
        Args:
            output_path: Path for the output Excel file
        """
        try:
            import pandas as pd
            
            # Prepare violations data
            violations_data = []
            for violation in self.database['violations'].values():
                violations_data.append({
                    'Violation ID': violation['violation_id'],
                    'Violation Text': violation['violation_text'],
                    'Severity': violation['severity'],
                    'Category': violation['category'],
                    'Occurrence Count': violation['occurrence_count'],
                    'First Seen': violation['first_seen'],
                    'Last Seen': violation['last_seen'],
                    'Justifiable': violation['justifiable'],
                    'Fix Applied': violation['fix_applied'],
                    'Justification Added': violation['justification_added']
                })
            
            df = pd.DataFrame(violations_data)
            
            with pd.ExcelWriter(output_path, engine='openpyxl') as writer:
                df.to_excel(writer, sheet_name='Violations', index=False)
                
                # Add statistics sheet
                stats_df = pd.DataFrame([self.get_statistics()])
                stats_df.to_excel(writer, sheet_name='Statistics', index=False)
            
            logger.info(f"Knowledge base exported to Excel: {output_path}")
            
        except ImportError:
            logger.error("pandas and openpyxl required for Excel export")
            raise
        except Exception as e:
            logger.error(f"Error exporting to Excel: {str(e)}")
            raise
