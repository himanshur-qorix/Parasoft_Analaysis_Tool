"""
Master Knowledge Base Manager
Aggregates knowledge from all module-specific databases for cross-module learning
Developer: Himanshu R
Version: 1.0.0
"""

import json
import logging
from pathlib import Path
from datetime import datetime
from typing import Dict, List, Optional, Set
from collections import Counter, defaultdict

logger = logging.getLogger(__name__)


class MasterKnowledgeBaseManager:
    """Manages consolidated knowledge base across all modules"""
    
    def __init__(self, knowledge_base_dir: Path):
        """
        Initialize the Master Knowledge Base Manager
        
        Args:
            knowledge_base_dir: Directory containing module knowledge databases
        """
        self.knowledge_base_dir = Path(knowledge_base_dir)
        self.knowledge_base_dir.mkdir(exist_ok=True)
        self.master_db_path = self.knowledge_base_dir / "Master_KnowledgeDatabase.json"
        self.master_database = {}
        self.module_databases = {}
    
    def load_all_module_databases(self) -> Dict[str, Dict]:
        """
        Load all module-specific knowledge databases
        
        Returns:
            Dictionary mapping module names to their databases
        """
        self.module_databases = {}
        
        for db_file in self.knowledge_base_dir.glob("*_KnowledgeDatabase.json"):
            # Skip master database
            if db_file.name == "Master_KnowledgeDatabase.json":
                continue
            
            module_name = db_file.stem.replace("_KnowledgeDatabase", "")
            
            try:
                with open(db_file, 'r', encoding='utf-8') as f:
                    self.module_databases[module_name] = json.load(f)
                logger.info(f"Loaded knowledge base for module: {module_name}")
            except Exception as e:
                logger.error(f"Error loading {db_file}: {str(e)}")
        
        logger.info(f"Loaded {len(self.module_databases)} module databases")
        return self.module_databases
    
    def consolidate_knowledge_bases(self) -> Dict:
        """
        Consolidate all module knowledge bases into a master database
        
        Returns:
            Master knowledge database
        """
        logger.info("Starting knowledge base consolidation...")
        
        # Load all module databases
        if not self.module_databases:
            self.load_all_module_databases()
        
        if not self.module_databases:
            logger.warning("No module databases found to consolidate")
            return self._create_empty_master_database()
        
        # Initialize master database
        self.master_database = self._create_empty_master_database()
        self.master_database["modules_included"] = list(self.module_databases.keys())
        self.master_database["total_modules"] = len(self.module_databases)
        
        # Aggregate violations from all modules
        violation_aggregator = defaultdict(lambda: {
            'violation_id': '',
            'violation_text': '',
            'modules_affected': [],
            'total_occurrences': 0,
            'first_seen': None,
            'last_seen': None,
            'severity': 'MEDIUM',
            'category': 'OTHER',
            'files_affected': [],
            'fix_success_rate': 0.0,
            'fix_examples': [],
            'justification_examples': [],
            'common_patterns': []
        })
        
        for module_name, module_db in self.module_databases.items():
            violations = module_db.get('violations', {})
            
            for violation_id, violation_data in violations.items():
                agg = violation_aggregator[violation_id]
                
                # Initialize if first time seeing this violation
                if not agg['violation_id']:
                    agg['violation_id'] = violation_id
                    agg['violation_text'] = violation_data.get('violation_text', '')
                    agg['severity'] = violation_data.get('severity', 'MEDIUM')
                    agg['category'] = violation_data.get('category', 'OTHER')
                    agg['first_seen'] = violation_data.get('first_seen')
                
                # Add module to affected list
                module_info = {
                    'module': module_name,
                    'occurrences': violation_data.get('occurrence_count', 0),
                    'fix_applied': violation_data.get('fix_applied', False),
                    'justification_added': violation_data.get('justification_added', False)
                }
                agg['modules_affected'].append(module_info)
                
                # Aggregate occurrences
                agg['total_occurrences'] += violation_data.get('occurrence_count', 0)
                
                # Update last seen
                violation_last_seen = violation_data.get('last_seen')
                if violation_last_seen:
                    if agg['last_seen'] is None or violation_last_seen > agg['last_seen']:
                        agg['last_seen'] = violation_last_seen
                
                # Collect fix examples
                if violation_data.get('fix_applied') and violation_data.get('fix_details'):
                    fix_example = {
                        'module': module_name,
                        'fix_details': violation_data['fix_details'],
                        'timestamp': violation_data.get('last_seen')
                    }
                    agg['fix_examples'].append(fix_example)
                
                # Collect justification examples
                if violation_data.get('justification_added') and violation_data.get('justification_text'):
                    just_example = {
                        'module': module_name,
                        'justification': violation_data['justification_text'],
                        'timestamp': violation_data.get('last_seen')
                    }
                    agg['justification_examples'].append(just_example)
                
                # Collect file patterns
                for file_entry in violation_data.get('files_affected', []):
                    file_name = file_entry.get('file', '') if isinstance(file_entry, dict) else str(file_entry)
                    if file_name:
                        agg['files_affected'].append({
                            'module': module_name,
                            'file': file_name
                        })
        
        # Calculate fix success rates and identify common patterns
        for violation_id, agg in violation_aggregator.items():
            # Calculate fix success rate
            modules_with_fixes = sum(1 for m in agg['modules_affected'] if m['fix_applied'])
            total_modules = len(agg['modules_affected'])
            agg['fix_success_rate'] = (modules_with_fixes / total_modules * 100) if total_modules > 0 else 0.0
            
            # Identify file patterns
            file_extensions = [Path(f['file']).suffix for f in agg['files_affected'] if f.get('file')]
            agg['common_file_types'] = dict(Counter(file_extensions))
            
            # Store in master database
            self.master_database['violations'][violation_id] = dict(agg)
        
        # Update statistics
        self.master_database['total_unique_violations'] = len(violation_aggregator)
        self.master_database['total_violations_across_modules'] = sum(
            agg['total_occurrences'] for agg in violation_aggregator.values()
        )
        
        # Identify cross-module violations (appear in 2+ modules)
        cross_module_violations = [
            {
                'violation_id': vid,
                'modules_count': len(data['modules_affected']),
                'total_occurrences': data['total_occurrences'],
                'severity': data['severity']
            }
            for vid, data in violation_aggregator.items()
            if len(data['modules_affected']) >= 2
        ]
        cross_module_violations.sort(key=lambda x: x['modules_count'], reverse=True)
        self.master_database['cross_module_violations'] = cross_module_violations[:50]  # Top 50
        
        # Find most common violations across all modules
        most_common_global = sorted(
            violation_aggregator.items(),
            key=lambda x: x[1]['total_occurrences'],
            reverse=True
        )[:50]
        
        self.master_database['statistics']['most_common_violations_global'] = [
            {
                'violation_id': vid,
                'total_occurrences': data['total_occurrences'],
                'modules_affected': len(data['modules_affected']),
                'severity': data['severity'],
                'category': data['category']
            }
            for vid, data in most_common_global
        ]
        
        # Category breakdown
        category_stats = defaultdict(int)
        severity_stats = defaultdict(int)
        
        for violation_data in violation_aggregator.values():
            category_stats[violation_data['category']] += violation_data['total_occurrences']
            severity_stats[violation_data['severity']] += violation_data['total_occurrences']
        
        self.master_database['statistics']['violations_by_category'] = dict(category_stats)
        self.master_database['statistics']['violations_by_severity'] = dict(severity_stats)
        
        logger.info(f"Consolidated {self.master_database['total_unique_violations']} unique violations")
        logger.info(f"Found {len(cross_module_violations)} cross-module violations")
        
        return self.master_database
    
    def _create_empty_master_database(self) -> Dict:
        """Create an empty master database structure"""
        return {
            "database_type": "master",
            "created_date": datetime.now().isoformat(),
            "last_updated": datetime.now().isoformat(),
            "version": "1.0",
            "modules_included": [],
            "total_modules": 0,
            "total_unique_violations": 0,
            "total_violations_across_modules": 0,
            "violations": {},
            "cross_module_violations": [],
            "statistics": {
                "most_common_violations_global": [],
                "violations_by_category": {},
                "violations_by_severity": {},
                "fix_success_metrics": {}
            },
            "recommendations": []
        }
    
    def save_master_database(self) -> Path:
        """
        Save the master knowledge database
        
        Returns:
            Path to the saved database file
        """
        self.master_database["last_updated"] = datetime.now().isoformat()
        
        with open(self.master_db_path, 'w', encoding='utf-8') as f:
            json.dump(self.master_database, f, indent=2, ensure_ascii=False)
        
        logger.info(f"Master knowledge base saved: {self.master_db_path}")
        return self.master_db_path
    
    def load_master_database(self) -> Dict:
        """
        Load the master knowledge database
        
        Returns:
            Master database dictionary
        """
        if self.master_db_path.exists():
            try:
                with open(self.master_db_path, 'r', encoding='utf-8') as f:
                    self.master_database = json.load(f)
                logger.info("Loaded existing master knowledge base")
            except Exception as e:
                logger.error(f"Error loading master database: {str(e)}")
                self.master_database = self._create_empty_master_database()
        else:
            logger.info("No existing master database found")
            self.master_database = self._create_empty_master_database()
        
        return self.master_database
    
    def get_violation_insights(self, violation_id: str) -> Optional[Dict]:
        """
        Get cross-module insights for a specific violation
        
        Args:
            violation_id: The violation ID to lookup
        
        Returns:
            Dictionary containing insights or None if not found
        """
        if not self.master_database:
            self.load_master_database()
        
        violation_data = self.master_database.get('violations', {}).get(violation_id)
        
        if not violation_data:
            return None
        
        insights = {
            'violation_id': violation_id,
            'severity': violation_data['severity'],
            'category': violation_data['category'],
            'total_occurrences': violation_data['total_occurrences'],
            'modules_affected': len(violation_data['modules_affected']),
            'fix_success_rate': violation_data['fix_success_rate'],
            'has_proven_fixes': len(violation_data['fix_examples']) > 0,
            'has_justifications': len(violation_data['justification_examples']) > 0,
            'recommended_fix': violation_data['fix_examples'][0] if violation_data['fix_examples'] else None,
            'recommended_justification': violation_data['justification_examples'][0] if violation_data['justification_examples'] else None
        }
        
        return insights
    
    def get_cross_module_patterns(self) -> List[Dict]:
        """
        Get violations that appear across multiple modules
        
        Returns:
            List of cross-module violation patterns
        """
        if not self.master_database:
            self.load_master_database()
        
        return self.master_database.get('cross_module_violations', [])
    
    def generate_recommendations(self) -> List[Dict]:
        """
        Generate recommendations based on master knowledge base
        
        Returns:
            List of recommendations
        """
        if not self.master_database or not self.master_database.get('violations'):
            return []
        
        recommendations = []
        
        # Recommendation 1: High-priority cross-module violations
        cross_module = self.get_cross_module_patterns()
        high_priority_cross = [v for v in cross_module if v['severity'] == 'HIGH'][:10]
        
        if high_priority_cross:
            recommendations.append({
                'type': 'CRITICAL',
                'title': 'High-Priority Cross-Module Violations',
                'description': f'Found {len(high_priority_cross)} HIGH severity violations appearing across multiple modules',
                'violations': high_priority_cross,
                'action': 'Review and apply proven fixes from other modules'
            })
        
        # Recommendation 2: Violations with proven fixes
        violations_with_fixes = [
            v for v in self.master_database['violations'].values()
            if len(v['fix_examples']) > 0 and v['fix_success_rate'] > 50
        ]
        
        if violations_with_fixes:
            recommendations.append({
                'type': 'OPTIMIZATION',
                'title': 'Apply Proven Fixes',
                'description': f'{len(violations_with_fixes)} violations have proven fixes from other modules',
                'count': len(violations_with_fixes),
                'action': 'Leverage existing fix patterns to accelerate resolution'
            })
        
        # Recommendation 3: Module-specific vs company-wide issues
        total_modules = self.master_database['total_modules']
        widespread_violations = [
            v for v in self.master_database['violations'].values()
            if len(v['modules_affected']) >= max(2, total_modules * 0.5)
        ]
        
        if widespread_violations:
            recommendations.append({
                'type': 'POLICY',
                'title': 'Company-Wide Coding Standards Review',
                'description': f'{len(widespread_violations)} violations appear in {max(2, total_modules * 0.5)}+ modules',
                'count': len(widespread_violations),
                'action': 'Consider updating coding guidelines or providing team training'
            })
        
        self.master_database['recommendations'] = recommendations
        return recommendations
    
    def export_consolidated_report(self, output_path: Path) -> Path:
        """
        Export consolidated knowledge base report to Excel
        
        Args:
            output_path: Path to save the Excel file
        
        Returns:
            Path to the exported file
        """
        import pandas as pd
        
        if not self.master_database:
            self.load_master_database()
        
        # Prepare data for export
        violations_data = []
        for vid, vdata in self.master_database.get('violations', {}).items():
            violations_data.append({
                'Violation ID': vid,
                'Violation': vdata.get('violation_text', '')[:100],
                'Category': vdata['category'],
                'Severity': vdata['severity'],
                'Total Occurrences': vdata['total_occurrences'],
                'Modules Affected': len(vdata['modules_affected']),
                'Fix Success Rate %': f"{vdata['fix_success_rate']:.1f}",
                'Has Proven Fixes': 'Yes' if vdata['fix_examples'] else 'No',
                'Has Justifications': 'Yes' if vdata['justification_examples'] else 'No'
            })
        
        # Create Excel file with multiple sheets
        with pd.ExcelWriter(output_path, engine='openpyxl') as writer:
            # Violations sheet
            df_violations = pd.DataFrame(violations_data)
            df_violations.to_excel(writer, sheet_name='All Violations', index=False)
            
            # Cross-module violations sheet
            df_cross = pd.DataFrame(self.master_database.get('cross_module_violations', []))
            df_cross.to_excel(writer, sheet_name='Cross-Module Violations', index=False)
            
            # Statistics sheet
            stats_data = []
            stats_data.append(['Total Modules', self.master_database.get('total_modules', 0)])
            stats_data.append(['Total Unique Violations', self.master_database.get('total_unique_violations', 0)])
            stats_data.append(['Total Violations Across Modules', self.master_database.get('total_violations_across_modules', 0)])
            
            df_stats = pd.DataFrame(stats_data, columns=['Metric', 'Value'])
            df_stats.to_excel(writer, sheet_name='Statistics', index=False)
        
        logger.info(f"Consolidated report exported to: {output_path}")
        return output_path
    
    def export_html_report(self, output_path: Path) -> Path:
        """
        Export consolidated knowledge base report to HTML
        
        Args:
            output_path: Path to save the HTML file
        
        Returns:
            Path to the exported file
        """
        from datetime import datetime
        
        if not self.master_database:
            self.load_master_database()
        
        # Prepare data
        violations = self.master_database.get('violations', {})
        cross_module = self.master_database.get('cross_module_violations', [])
        
        # Sort violations by total occurrences
        sorted_violations = sorted(
            violations.items(),
            key=lambda x: x[1]['total_occurrences'],
            reverse=True
        )
        
        # Generate HTML
        html_content = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Master Knowledge Base Report</title>
    <style>
        * {{
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }}
        body {{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            padding: 20px;
            color: #333;
        }}
        .container {{
            max-width: 1400px;
            margin: 0 auto;
            background: white;
            border-radius: 10px;
            box-shadow: 0 10px 40px rgba(0,0,0,0.2);
            overflow: hidden;
        }}
        .header {{
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 40px;
            text-align: center;
        }}
        .header h1 {{
            font-size: 2.5em;
            margin-bottom: 10px;
        }}
        .header p {{
            font-size: 1.1em;
            opacity: 0.9;
        }}
        .stats-grid {{
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 20px;
            padding: 30px;
            background: #f8f9fa;
        }}
        .stat-card {{
            background: white;
            padding: 25px;
            border-radius: 8px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            text-align: center;
            border-left: 4px solid #667eea;
        }}
        .stat-card .value {{
            font-size: 2.5em;
            font-weight: bold;
            color: #667eea;
            margin: 10px 0;
        }}
        .stat-card .label {{
            color: #666;
            font-size: 0.95em;
            text-transform: uppercase;
            letter-spacing: 1px;
        }}
        .section {{
            padding: 30px;
        }}
        .section h2 {{
            color: #667eea;
            font-size: 1.8em;
            margin-bottom: 20px;
            padding-bottom: 10px;
            border-bottom: 3px solid #667eea;
        }}
        table {{
            width: 100%;
            border-collapse: collapse;
            margin: 20px 0;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
        }}
        thead {{
            background: #667eea;
            color: white;
        }}
        th {{
            padding: 15px;
            text-align: left;
            font-weight: 600;
            text-transform: uppercase;
            font-size: 0.85em;
            letter-spacing: 1px;
        }}
        td {{
            padding: 12px 15px;
            border-bottom: 1px solid #e0e0e0;
        }}
        tbody tr:hover {{
            background: #f8f9fa;
            transition: background 0.3s;
        }}
        .badge {{
            display: inline-block;
            padding: 4px 12px;
            border-radius: 20px;
            font-size: 0.85em;
            font-weight: 600;
            text-transform: uppercase;
        }}
        .badge-high {{
            background: #ff4757;
            color: white;
        }}
        .badge-medium {{
            background: #ffa502;
            color: white;
        }}
        .badge-low {{
            background: #3742fa;
            color: white;
        }}
        .badge-critical {{
            background: #8b0000;
            color: white;
        }}
        .badge-yes {{
            background: #2ed573;
            color: white;
        }}
        .badge-no {{
            background: #dfe6e9;
            color: #666;
        }}
        .module-list {{
            display: inline-block;
            padding: 4px 8px;
            margin: 2px;
            background: #e8eaf6;
            border-radius: 4px;
            font-size: 0.85em;
            color: #5e35b1;
        }}
        .footer {{
            background: #f8f9fa;
            padding: 20px;
            text-align: center;
            color: #666;
            font-size: 0.9em;
        }}
        .top-violations {{
            background: #fff3e0;
            border-left: 4px solid #ffa502;
            padding: 15px;
            margin: 20px 0;
            border-radius: 5px;
        }}
        .proven-fixes {{
            background: #e8f5e9;
            border-left: 4px solid #2ed573;
            padding: 15px;
            margin: 20px 0;
            border-radius: 5px;
        }}
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🎯 Master Knowledge Base Report</h1>
            <p>Cross-Module Violation Analysis & Insights</p>
            <p style="font-size: 0.9em; margin-top: 10px;">Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}</p>
        </div>
        
        <div class="stats-grid">
            <div class="stat-card">
                <div class="label">Total Modules</div>
                <div class="value">{self.master_database.get('total_modules', 0)}</div>
            </div>
            <div class="stat-card">
                <div class="label">Unique Violations</div>
                <div class="value">{self.master_database.get('total_unique_violations', 0)}</div>
            </div>
            <div class="stat-card">
                <div class="label">Total Occurrences</div>
                <div class="value">{self.master_database.get('total_violations_across_modules', 0)}</div>
            </div>
            <div class="stat-card">
                <div class="label">Cross-Module Violations</div>
                <div class="value">{len(cross_module)}</div>
            </div>
        </div>
"""
        
        # Top 20 Most Common Violations
        html_content += """
        <div class="section">
            <h2>📊 Top 20 Most Common Violations</h2>
            <div class="top-violations">
                <p><strong>Note:</strong> These violations appear most frequently across all analyzed modules.</p>
            </div>
            <table>
                <thead>
                    <tr>
                        <th style="width: 5%">#</th>
                        <th style="width: 15%">Violation ID</th>
                        <th style="width: 35%">Description</th>
                        <th style="width: 10%">Severity</th>
                        <th style="width: 10%">Occurrences</th>
                        <th style="width: 15%">Modules</th>
                        <th style="width: 10%">Fix Available</th>
                    </tr>
                </thead>
                <tbody>
"""
        
        for idx, (vid, vdata) in enumerate(sorted_violations[:20], 1):
            severity_class = vdata['severity'].lower()
            fix_available = 'Yes' if vdata['fix_examples'] else 'No'
            fix_class = 'yes' if vdata['fix_examples'] else 'no'
            # Extract module names from modules_affected list of dicts
            module_names = [m['module'] if isinstance(m, dict) else m for m in vdata['modules_affected'][:3]]
            modules = ', '.join(module_names)
            if len(vdata['modules_affected']) > 3:
                modules += f" +{len(vdata['modules_affected']) - 3} more"
            
            html_content += f"""
                    <tr>
                        <td><strong>{idx}</strong></td>
                        <td><code>{vid}</code></td>
                        <td>{vdata.get('violation_text', '')[:80]}...</td>
                        <td><span class="badge badge-{severity_class}">{vdata['severity']}</span></td>
                        <td><strong>{vdata['total_occurrences']}</strong></td>
                        <td>{modules}</td>
                        <td><span class="badge badge-{fix_class}">{fix_available}</span></td>
                    </tr>
"""
        
        html_content += """
                </tbody>
            </table>
        </div>
"""
        
        # Violations with Proven Fixes
        violations_with_fixes = [(vid, vdata) for vid, vdata in violations.items() if vdata['fix_examples']]
        
        if violations_with_fixes:
            html_content += """
        <div class="section">
            <h2>✅ Violations with Proven Fixes</h2>
            <div class="proven-fixes">
                <p><strong>Note:</strong> These violations have documented fix examples from previous analyses.</p>
            </div>
            <table>
                <thead>
                    <tr>
                        <th style="width: 20%">Violation ID</th>
                        <th style="width: 40%">Description</th>
                        <th style="width: 10%">Severity</th>
                        <th style="width: 10%">Success Rate</th>
                        <th style="width: 20%">Modules</th>
                    </tr>
                </thead>
                <tbody>
"""
            
            for vid, vdata in sorted(violations_with_fixes, key=lambda x: x[1]['fix_success_rate'], reverse=True)[:15]:
                severity_class = vdata['severity'].lower()
                # Extract module names from modules_affected list of dicts
                module_names = [m['module'] if isinstance(m, dict) else m for m in vdata['modules_affected'][:5]]
                modules_html = ' '.join([f'<span class="module-list">{m}</span>' for m in module_names])
                
                html_content += f"""
                    <tr>
                        <td><code>{vid}</code></td>
                        <td>{vdata.get('violation_text', '')[:100]}...</td>
                        <td><span class="badge badge-{severity_class}">{vdata['severity']}</span></td>
                        <td><strong>{vdata['fix_success_rate']:.1f}%</strong></td>
                        <td>{modules_html}</td>
                    </tr>
"""
            
            html_content += """
                </tbody>
            </table>
        </div>
"""
        
        # Cross-Module Violations
        if cross_module:
            html_content += """
        <div class="section">
            <h2>🔄 Cross-Module Violations</h2>
            <p style="margin-bottom: 20px; color: #666;">Violations that appear in multiple modules (2 or more).</p>
            <table>
                <thead>
                    <tr>
                        <th style="width: 20%">Violation ID</th>
                        <th style="width: 40%">Description</th>
                        <th style="width: 10%">Severity</th>
                        <th style="width: 10%">Module Count</th>
                        <th style="width: 20%">Affected Modules</th>
                    </tr>
                </thead>
                <tbody>
"""
            
            for item in cross_module[:30]:
                vid = item.get('violation_id', 'N/A')
                vdata = violations.get(vid, {})
                severity = vdata.get('severity', 'MEDIUM')
                severity_class = severity.lower()
                # Extract module names from violation data
                if vdata and 'modules_affected' in vdata:
                    module_names = [m['module'] if isinstance(m, dict) else m for m in vdata['modules_affected'][:5]]
                    modules_html = ' '.join([f'<span class="module-list">{m}</span>' for m in module_names])
                    if len(vdata['modules_affected']) > 5:
                        modules_html += f' <span class="module-list">+{len(vdata["modules_affected"]) - 5} more</span>'
                else:
                    modules_html = 'N/A'
                
                html_content += f"""
                    <tr>
                        <td><code>{vid}</code></td>
                        <td>{vdata.get('violation_text', 'N/A')[:100]}...</td>
                        <td><span class="badge badge-{severity_class}">{severity}</span></td>
                        <td><strong>{item.get('module_count', 0)}</strong></td>
                        <td>{modules_html}</td>
                    </tr>
"""
            
            html_content += """
                </tbody>
            </table>
        </div>
"""
        
        # Footer
        html_content += f"""
        <div class="footer">
            <p><strong>Parasoft AI Agent</strong> - Master Knowledge Base Report</p>
            <p>Generated on {datetime.now().strftime('%Y-%m-%d at %H:%M:%S')}</p>
        </div>
    </div>
</body>
</html>
"""
        
        # Write HTML file
        output_path.parent.mkdir(parents=True, exist_ok=True)
        with open(output_path, 'w', encoding='utf-8') as f:
            f.write(html_content)
        
        logger.info(f"HTML report exported to: {output_path}")
        return output_path
