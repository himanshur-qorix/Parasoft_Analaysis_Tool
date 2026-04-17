"""
Violation History Manager
Tracks violation history, resolution patterns, and cross-module learning

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 3.0.0
"""

import json
import logging
from pathlib import Path
from datetime import datetime
from typing import Dict, List, Optional, Tuple
from collections import defaultdict

logger = logging.getLogger(__name__)


class ViolationHistoryManager:
    """Manages violation history and cross-module learning"""
    
    def __init__(self, history_dir: Path):
        """
        Initialize history manager
        
        Args:
            history_dir: Directory to store history data
        """
        self.history_dir = Path(history_dir)
        self.history_dir.mkdir(exist_ok=True)
        
        # Master history file across all modules
        self.master_history_file = self.history_dir / "master_violation_history.json"
        
        # Resolution patterns (RAG data)
        self.resolution_patterns_file = self.history_dir / "resolution_patterns.json"
        
        self.master_history = self._load_master_history()
        self.resolution_patterns = self._load_resolution_patterns()
    
    def _load_master_history(self) -> Dict:
        """Load master history from file"""
        if self.master_history_file.exists():
            try:
                with open(self.master_history_file, 'r', encoding='utf-8') as f:
                    return json.load(f)
            except Exception as e:
                logger.error(f"Failed to load master history: {e}")
        
        return {
            "created_date": datetime.now().isoformat(),
            "last_updated": datetime.now().isoformat(),
            "modules": {},  # Module-level tracking
            "violation_patterns": {},  # Violation-level tracking across modules
            "statistics": {
                "total_runs": 0,
                "total_modules": 0,
                "total_unique_violations": 0
            }
        }
    
    def _load_resolution_patterns(self) -> Dict:
        """Load resolution patterns (RAG data)"""
        if self.resolution_patterns_file.exists():
            try:
                with open(self.resolution_patterns_file, 'r', encoding='utf-8') as f:
                    return json.load(f)
            except Exception as e:
                logger.error(f"Failed to load resolution patterns: {e}")
        
        return {
            "created_date": datetime.now().isoformat(),
            "last_updated": datetime.now().isoformat(),
            "patterns": {},  # Violation type -> resolution patterns
            "common_resolutions": {},  # Most common resolution per violation type
            "cross_module_insights": {}  # How modules handle same issues
        }
    
    def record_analysis_run(self, module_name: str, violations: List[Dict], 
                           summary: Dict, source_code_path: str = None) -> None:
        """
        Record a new analysis run
        
        Args:
            module_name: Name of the module
            violations: List of violations found
            summary: Summary statistics
            source_code_path: Path to source code (optional)
        """
        timestamp = datetime.now().isoformat()
        run_id = f"{module_name}_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        
        # Initialize module if doesn't exist
        if module_name not in self.master_history["modules"]:
            self.master_history["modules"][module_name] = {
                "first_run": timestamp,
                "last_run": timestamp,
                "total_runs": 0,
                "runs": [],
                "violation_trends": {},
                "source_code_path": source_code_path
            }
        
        module_data = self.master_history["modules"][module_name]
        
        # Record this run
        run_data = {
            "run_id": run_id,
            "timestamp": timestamp,
            "summary": summary,
            "violation_count": len(violations),
            "violations_by_severity": {
                "CRITICAL": summary.get('critical', 0),
                "HIGH": summary.get('high', 0),
                "MEDIUM": summary.get('medium', 0),
                "LOW": summary.get('low', 0)
            },
            "violations_by_category": summary.get('by_category', {}),
            "top_violations": self._get_top_violations(violations, len(violations) if violations else 0),
            "source_code_path": source_code_path
        }
        
        module_data["runs"].append(run_data)
        module_data["total_runs"] += 1
        module_data["last_run"] = timestamp
        if source_code_path:
            module_data["source_code_path"] = source_code_path
        
        # Update violation-level tracking
        self._update_violation_patterns(module_name, violations, timestamp)
        
        # Update statistics
        self.master_history["statistics"]["total_runs"] += 1
        self.master_history["statistics"]["total_modules"] = len(self.master_history["modules"])
        self.master_history["last_updated"] = timestamp
        
        # Save
        self._save_master_history()
        
        logger.info(f"Recorded analysis run for {module_name}: {len(violations)} violations")
    
    def _get_top_violations(self, violations: List[Dict], top_n: int = 10) -> List[Dict]:
        """Get top N most common violations"""
        violation_counts = defaultdict(int)
        violation_details = {}
        
        for v in violations:
            check_id = v.get('check_id', 'UNKNOWN')
            violation_counts[check_id] += 1
            
            if check_id not in violation_details:
                violation_details[check_id] = {
                    'check_id': check_id,
                    'severity': v.get('severity', 'UNKNOWN'),
                    'category': v.get('category', 'UNKNOWN'),
                    'description': v.get('violation_text', ''),
                    'count': 0
                }
            violation_details[check_id]['count'] += 1
        
        # Sort by count and return top N
        sorted_violations = sorted(
            violation_details.values(),
            key=lambda x: x['count'],
            reverse=True
        )
        
        return sorted_violations[:top_n]
    
    def _update_violation_patterns(self, module_name: str, violations: List[Dict], 
                                   timestamp: str) -> None:
        """Update cross-module violation patterns"""
        for v in violations:
            check_id = v.get('check_id', 'UNKNOWN')
            
            # Initialize pattern if doesn't exist
            if check_id not in self.master_history["violation_patterns"]:
                self.master_history["violation_patterns"][check_id] = {
                    "check_id": check_id,
                    "severity": v.get('severity', 'UNKNOWN'),
                    "category": v.get('category', 'UNKNOWN'),
                    "description": v.get('violation_text', ''),
                    "total_occurrences": 0,
                    "modules_affected": {},
                    "first_seen": timestamp,
                    "last_seen": timestamp
                }
            
            pattern = self.master_history["violation_patterns"][check_id]
            pattern["total_occurrences"] += 1
            pattern["last_seen"] = timestamp
            
            # Track per-module occurrences
            if module_name not in pattern["modules_affected"]:
                pattern["modules_affected"][module_name] = {
                    "count": 0,
                    "first_seen": timestamp,
                    "last_seen": timestamp,
                    "status": "open",  # open, suppressed, fixed, justified
                    "resolution_note": None
                }
            
            pattern["modules_affected"][module_name]["count"] += 1
            pattern["modules_affected"][module_name]["last_seen"] = timestamp
    
    def update_violation_status(self, module_name: str, check_id: str, 
                               status: str, note: str = None) -> None:
        """
        Update status of a violation (for learning)
        
        Args:
            module_name: Module name
            check_id: Violation check ID
            status: Status (open, suppressed, fixed, justified)
            note: Optional resolution note
        """
        if check_id in self.master_history["violation_patterns"]:
            pattern = self.master_history["violation_patterns"][check_id]
            
            if module_name in pattern["modules_affected"]:
                pattern["modules_affected"][module_name]["status"] = status
                pattern["modules_affected"][module_name]["resolution_note"] = note
                pattern["modules_affected"][module_name]["resolved_date"] = datetime.now().isoformat()
                
                # Learn from this resolution
                self._learn_resolution_pattern(check_id, status, note)
                
                self._save_master_history()
                logger.info(f"Updated {check_id} status in {module_name}: {status}")
    
    def _learn_resolution_pattern(self, check_id: str, status: str, note: str = None) -> None:
        """Learn from violation resolution (RAG)"""
        if check_id not in self.resolution_patterns["patterns"]:
            self.resolution_patterns["patterns"][check_id] = {
                "resolutions": [],
                "status_counts": defaultdict(int),
                "common_notes": []
            }
        
        pattern = self.resolution_patterns["patterns"][check_id]
        
        # Record resolution
        pattern["resolutions"].append({
            "status": status,
            "note": note,
            "timestamp": datetime.now().isoformat()
        })
        
        pattern["status_counts"][status] += 1
        
        if note and note not in pattern["common_notes"]:
            pattern["common_notes"].append(note)
        
        # Determine most common resolution
        most_common_status = max(pattern["status_counts"].items(), key=lambda x: x[1])
        self.resolution_patterns["common_resolutions"][check_id] = {
            "status": most_common_status[0],
            "count": most_common_status[1],
            "confidence": most_common_status[1] / sum(pattern["status_counts"].values())
        }
        
        self._save_resolution_patterns()
    
    def get_module_history(self, module_name: str) -> Optional[Dict]:
        """Get history for a specific module"""
        return self.master_history["modules"].get(module_name)
    
    def get_violation_insights(self, check_id: str) -> Optional[Dict]:
        """Get cross-module insights for a violation"""
        pattern = self.master_history["violation_patterns"].get(check_id)
        
        if not pattern:
            return None
        
        # Calculate insights
        total_modules = len(pattern["modules_affected"])
        status_summary = defaultdict(int)
        
        for module_data in pattern["modules_affected"].values():
            status_summary[module_data["status"]] += 1
        
        return {
            "check_id": check_id,
            "description": pattern["description"],
            "severity": pattern["severity"],
            "total_occurrences": pattern["total_occurrences"],
            "modules_affected": total_modules,
            "status_breakdown": dict(status_summary),
            "recommended_action": self._get_recommended_action(check_id),
            "modules_detail": pattern["modules_affected"]
        }
    
    def _get_recommended_action(self, check_id: str) -> str:
        """Get AI-recommended action based on historical data"""
        if check_id in self.resolution_patterns["common_resolutions"]:
            common_res = self.resolution_patterns["common_resolutions"][check_id]
            
            if common_res["confidence"] > 0.7:
                return f"Recommended: {common_res['status']} (based on {common_res['count']} similar cases, {common_res['confidence']:.0%} confidence)"
        
        return "No clear pattern - manual review recommended"
    
    def get_comparison_data(self, modules: List[str] = None) -> Dict:
        """
        Get comparison data for modules
        
        Args:
            modules: List of module names (None = all modules)
        
        Returns:
            Comparison data dictionary
        """
        if modules is None:
            modules = list(self.master_history["modules"].keys())
        
        comparison_data = {
            "timestamp": datetime.now().isoformat(),
            "modules": modules,
            "comparison": {}
        }
        
        for module_name in modules:
            if module_name not in self.master_history["modules"]:
                continue
            
            module_data = self.master_history["modules"][module_name]
            
            if not module_data["runs"]:
                continue
            
            latest_run = module_data["runs"][-1]
            
            comparison_data["comparison"][module_name] = {
                "total_runs": module_data["total_runs"],
                "first_run": module_data["first_run"],
                "last_run": module_data["last_run"],
                "latest_violation_count": latest_run["violation_count"],
                "latest_by_severity": latest_run["violations_by_severity"],
                "trend": self._calculate_trend(module_data["runs"]),
                "top_violations": latest_run["top_violations"]
            }
        
        return comparison_data
    
    def _calculate_trend(self, runs: List[Dict]) -> str:
        """Calculate trend direction"""
        if len(runs) < 2:
            return "insufficient_data"
        
        latest_count = runs[-1]["violation_count"]
        previous_count = runs[-2]["violation_count"]
        
        if latest_count < previous_count:
            return "improving"
        elif latest_count > previous_count:
            return "worsening"
        else:
            return "stable"
    
    def _save_master_history(self) -> None:
        """Save master history to file"""
        try:
            with open(self.master_history_file, 'w', encoding='utf-8') as f:
                json.dump(self.master_history, f, indent=2, ensure_ascii=False)
        except Exception as e:
            logger.error(f"Failed to save master history: {e}")
    
    def _save_resolution_patterns(self) -> None:
        """Save resolution patterns to file"""
        try:
            # Convert defaultdict to dict for JSON serialization
            patterns_copy = {}
            for check_id, pattern in self.resolution_patterns["patterns"].items():
                patterns_copy[check_id] = {
                    "resolutions": pattern["resolutions"],
                    "status_counts": dict(pattern["status_counts"]),
                    "common_notes": pattern["common_notes"]
                }
            
            output_data = {
                "created_date": self.resolution_patterns["created_date"],
                "last_updated": datetime.now().isoformat(),
                "patterns": patterns_copy,
                "common_resolutions": self.resolution_patterns["common_resolutions"],
                "cross_module_insights": self.resolution_patterns["cross_module_insights"]
            }
            
            with open(self.resolution_patterns_file, 'w', encoding='utf-8') as f:
                json.dump(output_data, f, indent=2, ensure_ascii=False)
        except Exception as e:
            logger.error(f"Failed to save resolution patterns: {e}")
