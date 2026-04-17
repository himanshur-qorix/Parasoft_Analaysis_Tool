"""
Comparison Report Generator
Generates HTML comparison reports for violation history and cross-module analysis

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 3.0.0
"""

import logging
from pathlib import Path
from datetime import datetime
from typing import Dict, List

logger = logging.getLogger(__name__)


class ComparisonReportGenerator:
    """Generates comparison HTML reports"""
    
    def __init__(self, history_manager):
        """
        Initialize report generator
        
        Args:
            history_manager: ViolationHistoryManager instance
        """
        self.history_manager = history_manager
    
    def generate_module_comparison_report(self, module_name: str, output_path: Path) -> Path:
        """
        Generate comparison report for a single module
        
        Args:
            module_name: Module name
            output_path: Output HTML file path
        
        Returns:
            Path to generated report
        """
        logger.info(f"Generating comparison report for {module_name}")
        
        module_history = self.history_manager.get_module_history(module_name)
        
        if not module_history:
            logger.warning(f"No history found for module {module_name}")
            return None
        
        html_content = self._generate_module_html(module_name, module_history)
        
        output_path.parent.mkdir(parents=True, exist_ok=True)
        
        with open(output_path, 'w', encoding='utf-8') as f:
            f.write(html_content)
        
        logger.info(f"Module comparison report generated: {output_path}")
        return output_path
    
    def generate_cross_module_report(self, modules: List[str], output_path: Path) -> Path:
        """
        Generate cross-module comparison report
        
        Args:
            modules: List of module names
            output_path: Output HTML file path
        
        Returns:
            Path to generated report
        """
        logger.info(f"Generating cross-module comparison for {len(modules)} modules")
        
        comparison_data = self.history_manager.get_comparison_data(modules)
        
        html_content = self._generate_cross_module_html(comparison_data)
        
        output_path.parent.mkdir(parents=True, exist_ok=True)
        
        with open(output_path, 'w', encoding='utf-8') as f:
            f.write(html_content)
        
        logger.info(f"Cross-module comparison report generated: {output_path}")
        return output_path
    
    def _generate_module_html(self, module_name: str, module_history: Dict) -> str:
        """Generate HTML for single module history"""
        
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        runs = module_history.get("runs", [])
        
        # Calculate statistics
        total_runs = len(runs)
        latest_run = runs[-1] if runs else None
        
        html = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Violation History - {module_name}</title>
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
            max-width: 1600px;
            margin: 0 auto;
            background: white;
            border-radius: 10px;
            box-shadow: 0 10px 40px rgba(0,0,0,0.1);
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
        
        .content {{
            padding: 40px;
        }}
        
        .section {{
            margin-bottom: 40px;
        }}
        
        .section-title {{
            font-size: 1.8em;
            color: #667eea;
            margin-bottom: 20px;
            padding-bottom: 10px;
            border-bottom: 3px solid #667eea;
        }}
        
        .stats-grid {{
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }}
        
        .stat-card {{
            background: #f8f9fa;
            padding: 20px;
            border-radius: 8px;
            text-align: center;
            border-left: 4px solid #667eea;
        }}
        
        .stat-value {{
            font-size: 2em;
            font-weight: bold;
            color: #667eea;
        }}
        
        .stat-label {{
            color: #666;
            margin-top: 5px;
        }}
        
        table {{
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
        }}
        
        th, td {{
            padding: 12px;
            text-align: left;
            border-bottom: 1px solid #ddd;
        }}
        
        th {{
            background: #667eea;
            color: white;
            font-weight: bold;
        }}
        
        tr:hover {{
            background: #f5f5f5;
        }}
        
        .trend-improving {{
            color: #28a745;
            font-weight: bold;
        }}
        
        .trend-worsening {{
            color: #dc3545;
            font-weight: bold;
        }}
        
        .trend-stable {{
            color: #6c757d;
        }}
        
        .severity-critical {{
            color: #dc3545;
            font-weight: bold;
        }}
        
        .severity-high {{
            color: #fd7e14;
            font-weight: bold;
        }}
        
        .severity-medium {{
            color: #ffc107;
        }}
        
        .severity-low {{
            color: #6c757d;
        }}
        
        .chart {{
            margin: 20px 0;
            padding: 20px;
            background: #f8f9fa;
            border-radius: 8px;
        }}
        
        .violation-row {{
            padding: 10px;
            margin: 5px 0;
            background: white;
            border-left: 4px solid #667eea;
            border-radius: 4px;
        }}
        
        .footer {{
            background: #f8f9fa;
            padding: 20px;
            text-align: center;
            color: #666;
            border-top: 1px solid #ddd;
        }}
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>Violation History Report</h1>
            <p>Module: {module_name}</p>
            <p>Generated: {timestamp}</p>
        </div>
        
        <div class="content">
            <div class="section">
                <div class="section-title">Overview Statistics</div>
                <div class="stats-grid">
                    <div class="stat-card">
                        <div class="stat-value">{total_runs}</div>
                        <div class="stat-label">Total Analysis Runs</div>
                    </div>"""
        
        if latest_run:
            html += f"""
                    <div class="stat-card">
                        <div class="stat-value">{latest_run['violation_count']}</div>
                        <div class="stat-label">Latest Violation Count</div>
                    </div>
                    <div class="stat-card">
                        <div class="stat-value severity-critical">{latest_run['violations_by_severity']['CRITICAL']}</div>
                        <div class="stat-label">Critical Issues</div>
                    </div>
                    <div class="stat-card">
                        <div class="stat-value severity-high">{latest_run['violations_by_severity']['HIGH']}</div>
                        <div class="stat-label">High Priority</div>
                    </div>"""
        
        html += """
                </div>
            </div>
            
            <div class="section">
                <div class="section-title">Historical Trend</div>
                <table>
                    <thead>
                        <tr>
                            <th>Run Date</th>
                            <th>Total Violations</th>
                            <th>Critical</th>
                            <th>High</th>
                            <th>Medium</th>
                            <th>Low</th>
                            <th>Trend</th>
                        </tr>
                    </thead>
                    <tbody>"""
        
        for i, run in enumerate(runs):
            trend = "N/A"
            trend_class = ""
            if i > 0:
                prev_count = runs[i-1]['violation_count']
                curr_count = run['violation_count']
                if curr_count < prev_count:
                    trend = f"↓ {prev_count - curr_count}"
                    trend_class = "trend-improving"
                elif curr_count > prev_count:
                    trend = f"↑ {curr_count - prev_count}"
                    trend_class = "trend-worsening"
                else:
                    trend = "→ Stable"
                    trend_class = "trend-stable"
            
            run_date = run['timestamp'][:16].replace('T', ' ')
            
            html += f"""
                        <tr>
                            <td>{run_date}</td>
                            <td>{run['violation_count']}</td>
                            <td class="severity-critical">{run['violations_by_severity']['CRITICAL']}</td>
                            <td class="severity-high">{run['violations_by_severity']['HIGH']}</td>
                            <td class="severity-medium">{run['violations_by_severity']['MEDIUM']}</td>
                            <td class="severity-low">{run['violations_by_severity']['LOW']}</td>
                            <td class="{trend_class}">{trend}</td>
                        </tr>"""
        
        html += """
                    </tbody>
                </table>
            </div>"""
        
        if latest_run and latest_run.get('top_violations'):
            html += """
            <div class="section">
                <div class="section-title">Most Common Violations (Latest Run)</div>"""
            
            for violation in latest_run['top_violations']:
                severity_class = f"severity-{violation['severity'].lower()}"
                html += f"""
                <div class="violation-row">
                    <strong>{violation['check_id']}</strong> 
                    <span class="{severity_class}">({violation['severity']})</span>
                    - {violation['count']} occurrences<br>
                    <small>{violation['description']}</small>
                </div>"""
            
            html += "</div>"
        
        html += """
        </div>
        
        <div class="footer">
            <p><strong>Qorix India Pvt Ltd</strong></p>
            <p>Static Code Analyzer v3.0.0</p>
        </div>
    </div>
</body>
</html>
"""
        return html
    
    def _generate_cross_module_html(self, comparison_data: Dict) -> str:
        """Generate HTML for cross-module comparison"""
        
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        modules = comparison_data.get("modules", [])
        comparison = comparison_data.get("comparison", {})
        
        # Get all unique violation types across modules
        all_violations = {}
        for module_name, data in comparison.items():
            for violation in data.get("top_violations", []):
                check_id = violation['check_id']
                if check_id not in all_violations:
                    all_violations[check_id] = {
                        'check_id': check_id,
                        'description': violation['description'],
                        'severity': violation['severity'],
                        'modules': {}
                    }
                all_violations[check_id]['modules'][module_name] = violation['count']
        
        html = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Cross-Module Comparison Report</title>
    <style>
        * {{
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }}
        
        body {{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-font;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            padding: 20px;
            color: #333;
        }}
        
        .container {{
            max-width: 1800px;
            margin: 0 auto;
            background: white;
            border-radius: 10px;
            box-shadow: 0 10px 40px rgba(0,0,0,0.1);
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
        
        .content {{
            padding: 40px;
        }}
        
        .section {{
            margin-bottom: 40px;
        }}
        
        .section-title {{
            font-size: 1.8em;
            color: #667eea;
            margin-bottom: 20px;
            padding-bottom: 10px;
            border-bottom: 3px solid #667eea;
        }}
        
        .module-grid {{
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }}
        
        .module-card {{
            background: #f8f9fa;
            padding: 20px;
            border-radius: 8px;
            border-left: 4px solid #667eea;
        }}
        
        .module-name {{
            font-size: 1.5em;
            font-weight: bold;
            color: #667eea;
            margin-bottom: 15px;
        }}
        
        .module-stat {{
            display: flex;
            justify-content: space-between;
            padding: 8px 0;
            border-bottom: 1px solid #ddd;
        }}
        
        .module-stat:last-child {{
            border-bottom: none;
        }}
        
        .trend-badge {{
            display: inline-block;
            padding: 4px 12px;
            border-radius: 12px;
            font-size: 0.85em;
            font-weight: bold;
            color: white;
        }}
        
        .trend-improving {{
            background: #28a745;
        }}
        
        .trend-worsening {{
            background: #dc3545;
        }}
        
        .trend-stable {{
            background: #6c757d;
        }}
        
        table {{
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
            font-size: 0.9em;
        }}
        
        th, td {{
            padding: 12px;
            text-align: left;
            border-bottom: 1px solid #ddd;
        }}
        
        th {{
            background: #667eea;
            color: white;
            font-weight: bold;
            position: sticky;
            top: 0;
        }}
        
        tr:hover {{
            background: #f5f5f5;
        }}
        
        .severity-critical {{
            color: #dc3545;
            font-weight: bold;
        }}
        
        .severity-high {{
            color: #fd7e14;
            font-weight: bold;
        }}
        
        .count-cell {{
            text-align: center;
            font-weight: bold;
        }}
        
        .footer {{
            background: #f8f9fa;
            padding: 30px;
            text-align: center;
            color: #666;
            border-top: 1px solid#ddd;
        }}
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>Cross-Module Comparison Report</h1>
            <p>Comparing {len(modules)} Modules</p>
            <p>Generated: {timestamp}</p>
        </div>
        
        <div class="content">
            <div class="section">
                <div class="section-title">Module Overview</div>
                <div class="module-grid">"""
        
        for module_name, data in comparison.items():
            trend_class = f"trend-{data['trend']}"
            trend_text = {
                'improving': '↓ Improving',
                'worsening': '↑ Worsening',
                'stable': '→ Stable',
                'insufficient_data': 'N/A'
            }.get(data['trend'], data['trend'])
            
            html += f"""
                    <div class="module-card">
                        <div class="module-name">{module_name}</div>
                        <div class="module-stat">
                            <span>Total Runs:</span>
                            <strong>{data['total_runs']}</strong>
                        </div>
                        <div class="module-stat">
                            <span>Latest Violations:</span>
                            <strong>{data['latest_violation_count']}</strong>
                        </div>
                        <div class="module-stat">
                            <span>Critical:</span>
                            <strong class="severity-critical">{data['latest_by_severity']['CRITICAL']}</strong>
                        </div>
                        <div class="module-stat">
                            <span>High:</span>
                            <strong class="severity-high">{data['latest_by_severity']['HIGH']}</strong>
                        </div>
                        <div class="module-stat">
                            <span>Trend:</span>
                            <span class="trend-badge {trend_class}">{trend_text}</span>
                        </div>
                    </div>"""
        
        html+= """
                </div>
            </div>
            
            <div class="section">
                <div class="section-title">Violation Comparison Across Modules</div>
                <p style="margin-bottom: 20px; color: #666;">
                    This table shows how common violations appear across different modules.
                    Use this to identify patterns and share resolution strategies.
                </p>
                <table>
                    <thead>
                        <tr>
                            <th>Violation Type</th>
                            <th>Severity</th>"""
        
        # Add module columns
        for module_name in modules:
            if module_name in comparison:
                html += f"<th>{module_name}</th>"
        
        html += """
                        </tr>
                    </thead>
                    <tbody>"""
        
        # Sort violations by severity and total count
        sorted_violations = sorted(
            all_violations.values(),
            key=lambda x: (
                {'CRITICAL': 0, 'HIGH': 1, 'MEDIUM': 2, 'LOW': 3}.get(x['severity'], 4),
                -sum(x['modules'].values())
            )
        )
        
        for violation in sorted_violations:
            severity_class = f"severity-{violation['severity'].lower()}"
            
            html += f"""
                        <tr>
                            <td>
                                <strong>{violation['check_id']}</strong><br>
                                <small>{violation['description']}</small>
                            </td>
                            <td class="{severity_class}">{violation['severity']}</td>"""
            
            for module_name in modules:
                if module_name in comparison:
                    count = violation['modules'].get(module_name, '-')
                    cell_class = "count-cell"
                    if count != '-' and count > 0:
                        cell_class += " " + severity_class
                    html += f"<td class='{cell_class}'>{count}</td>"
            
            html += "</tr>"
        
        html += """
                    </tbody>
                </table>
            </div>
        </div>
        
        <div class="footer">
            <p><strong>Qorix India Pvt Ltd</strong></p>
            <p>Static Code Analyzer v3.0.0 - Cross-Module Analysis</p>
            <p style="margin-top: 10px; font-size: 0.9em;">
                Use this report to identify common issues and share best practices across teams
            </p>
        </div>
    </div>
</body>
</html>
"""
        return html
