"""
CERT and MISRA Violation Report Generator
Generates HTML comparison reports for CERT and MISRA violations from Knowledge Base
"""

import json
import sys
import os
from pathlib import Path
from datetime import datetime
from collections import defaultdict

# Configure UTF-8 encoding for Windows console
if sys.platform == 'win32':
    try:
        import ctypes
        kernel32 = ctypes.windll.kernel32
        kernel32.SetConsoleOutputCP(65001)  # UTF-8
        sys.stdout.reconfigure(encoding='utf-8', errors='replace')
        sys.stderr.reconfigure(encoding='utf-8', errors='replace')
    except:
        pass

def load_knowledge_base(kb_path):
    """Load knowledge base from JSON file"""
    try:
        with open(kb_path, 'r', encoding='utf-8') as f:
            return json.load(f)
    except FileNotFoundError:
        print(f"[ERROR] Knowledge base not found: {kb_path}")
        return None
    except json.JSONDecodeError as e:
        print(f"[ERROR] Invalid JSON in knowledge base: {e}")
        return None


def count_suppressions_from_files(module_name):
    """Count suppressions from justification files for this module"""
    justifications_dir = Path("justifications")
    if not justifications_dir.exists():
        return 0
    
    total_suppressions = 0
    
    # Find all suppression files for this module
    pattern = f"{module_name}_suppress_comments_*.txt"
    suppression_files = list(justifications_dir.glob(pattern))
    
    print(f"[INFO] Found {len(suppression_files)} suppression files for {module_name}")
    
    for file_path in suppression_files:
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
                # Count parasoft-suppress comments
                count = content.count('parasoft-suppress')
                total_suppressions += count
        except Exception as e:
            print(f"[WARNING] Error reading {file_path}: {e}")
            continue
    
    return total_suppressions


def count_suppressions_from_source_code(module_name, kb_data):
    """
    Count suppression comments from actual source code files
    
    Args:
        module_name: Module name
        kb_data: Knowledge base data (to get source file paths)
    
    Returns:
        Count of parasoft-suppress comments found in source code
    """
    # Try to find source code path from various locations
    source_paths = []
    
    # 1. PRIORITY: Check config.json for configured source_code_path
    config_path = Path("config") / "config.json"
    if config_path.exists():
        try:
            with open(config_path, 'r') as f:
                config = json.load(f)
                
            # Check root level source_code_path (set by GUI)
            source_path_str = config.get('source_code_path')
            if source_path_str and source_path_str.strip():
                source_path = Path(source_path_str.strip())
                if source_path.exists():
                    source_paths.append(source_path)
                    print(f"[INFO] Using configured source path from config.json: {source_path}")
                else:
                    print(f"[WARNING] Configured source path does not exist: {source_path}")
            
            # Fallback: Check workspace.source_code_path
            if not source_paths:
                workspace_path = config.get('workspace', {}).get('source_code_path')
                if workspace_path and workspace_path.strip():
                    source_path = Path(workspace_path.strip())
                    if source_path.exists():
                        source_paths.append(source_path)
                        print(f"[INFO] Using workspace source path: {source_path}")
        except Exception as e:
            print(f"[WARNING] Error reading config.json: {e}")
    
    # 2. Check KB metadata
    if not source_paths:
        metadata = kb_data.get('metadata', {})
        source_path_str = metadata.get('source_code_path')
        if source_path_str:
            source_path = Path(source_path_str)
            if source_path.exists():
                source_paths.append(source_path)
                print(f"[INFO] Using source path from KB metadata: {source_path}")
    
    # 3. Fallback: Check Input folder
    if not source_paths:
        print(f"[INFO] No configured source path found, checking Input/ folder...")
        input_dir = Path("Input")
        if input_dir.exists():
            # Look for module-specific folder
            module_patterns = [
                f"{module_name}_Config_*",
                f"{module_name}_*",
                module_name
            ]
            
            for pattern in module_patterns:
                matches = list(input_dir.glob(pattern))
                source_paths.extend(matches)
    
    if not source_paths:
        print(f"[ERROR] No source code directory found!")
        print(f"[INFO] Please set the source code path in the GUI (Input Path field)")
        print(f"[INFO] Or place source code in: Input/{module_name}_*, Input/{module_name}")
        return 0
    
    # Count suppressions in source files
    total_suppressions = 0
    files_scanned = 0
    
    for source_path in source_paths[:1]:  # Use first matching path
        if not source_path.is_dir():
            continue
            
        print(f"[INFO] Scanning source code in: {source_path}")
        
        # Scan all C/C++ files
        source_extensions = ['.c', '.cpp', '.h', '.hpp', '.cc', '.cxx']
        
        for ext in source_extensions:
            for file_path in source_path.rglob(f'*{ext}'):
                files_scanned += 1
                try:
                    with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                        content = f.read()
                        # Count parasoft-suppress comments (case-insensitive)
                        count = content.lower().count('parasoft-suppress')
                        if count > 0:
                            total_suppressions += count
                            print(f"  Found {count} suppression(s) in: {file_path.name}")
                except Exception as e:
                    # Skip files that can't be read
                    continue
        
        print(f"[INFO] Scanned {files_scanned} source files")
        print(f"[INFO] Found {total_suppressions} suppression comments in source code")
        
        return total_suppressions
    
    return 0

def filter_cert_misra_violations(kb_data):
    """Filter violations to only include CERT and MISRA"""
    cert_violations = []
    misra_violations = []
    
    # Violations are stored as a dictionary, not a list
    violations_dict = kb_data.get('violations', {})
    
    for violation_id, violation in violations_dict.items():
        category = violation.get('category', '').upper()
        
        if category == 'CERT':
            cert_violations.append(violation)
        elif category == 'MISRA':
            misra_violations.append(violation)
    
    return cert_violations, misra_violations

def analyze_violations(violations):
    """Analyze violations and generate statistics"""
    stats = {
        'total': len(violations),
        'by_severity': defaultdict(int),
        'by_rule': defaultdict(int),
        'by_file': defaultdict(int),
        'fixed': 0,
        'open': 0,
        'justified': 0
    }
    
    for v in violations:
        # Severity
        severity = v.get('severity', 'UNKNOWN')
        stats['by_severity'][severity] += 1
        
        # Rule (violation_id)
        rule_id = v.get('violation_id', 'UNKNOWN')
        stats['by_rule'][rule_id] += 1
        
        # Files - need to count unique files from files_affected list
        files_affected = v.get('files_affected', [])
        unique_files = set()
        for file_entry in files_affected:
            if isinstance(file_entry, dict):
                file_name = file_entry.get('file', 'UNKNOWN')
            else:
                file_name = str(file_entry)
            unique_files.add(file_name)
        
        for file_name in unique_files:
            stats['by_file'][file_name] += 1
        
        # Status - check for fix or justification
        if v.get('fix_applied'):
            stats['fixed'] += 1
        elif v.get('justification_added') or v.get('justification') or v.get('justified'):
            stats['justified'] += 1
        else:
            stats['open'] += 1
    
    return stats

def get_top_violations(violations, top_n=10):
    """Get top N most common violations"""
    rule_counts = defaultdict(lambda: {'count': 0, 'severity': '', 'message': ''})
    
    for v in violations:
        rule_id = v.get('violation_id', 'UNKNOWN')
        occurrence_count = v.get('occurrence_count', 1)
        
        rule_counts[rule_id]['count'] += occurrence_count
        if not rule_counts[rule_id]['severity']:
            rule_counts[rule_id]['severity'] = v.get('severity', 'UNKNOWN')
            rule_counts[rule_id]['message'] = v.get('violation_text', '')
    
    # Sort by count
    sorted_rules = sorted(rule_counts.items(), key=lambda x: x[1]['count'], reverse=True)
    return sorted_rules[:top_n]

def generate_html_report(module_name, cert_violations, misra_violations, cert_stats, misra_stats, output_path, total_suppressions=0):
    """Generate comprehensive HTML report"""
    
    cert_top = get_top_violations(cert_violations, 10)
    misra_top = get_top_violations(misra_violations, 10)
    
    total_violations = len(cert_violations) + len(misra_violations)
    total_justified = cert_stats['justified'] + misra_stats['justified'] + total_suppressions
    
    html_content = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CERT & MISRA Report - {module_name}</title>
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
            border-radius: 12px;
            box-shadow: 0 10px 40px rgba(0,0,0,0.2);
            overflow: hidden;
        }}
        
        .header {{
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 30px;
            text-align: center;
        }}
        
        .header h1 {{
            font-size: 2.5em;
            margin-bottom: 10px;
        }}
        
        .header .module-name {{
            font-size: 1.5em;
            opacity: 0.9;
            margin-bottom: 5px;
        }}
        
        .header .timestamp {{
            font-size: 0.9em;
            opacity: 0.8;
        }}
        
        .summary-cards {{
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 20px;
            padding: 30px;
            background: #f8f9fa;
        }}
        
        .card {{
            background: white;
            padding: 25px;
            border-radius: 8px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            text-align: center;
        }}
        
        .card-title {{
            font-size: 0.9em;
            color: #666;
            text-transform: uppercase;
            letter-spacing: 1px;
            margin-bottom: 10px;
        }}
        
        .card-value {{
            font-size: 2.5em;
            font-weight: bold;
            color: #667eea;
        }}
        
        .card-subtitle {{
            font-size: 0.85em;
            color: #999;
            margin-top: 5px;
        }}
        
        .section {{
            padding: 30px;
        }}
        
        .section-title {{
            font-size: 1.8em;
            margin-bottom: 20px;
            color: #667eea;
            border-bottom: 3px solid #667eea;
            padding-bottom: 10px;
        }}
        
        .subsection {{
            margin-bottom: 40px;
        }}
        
        .subsection-title {{
            font-size: 1.3em;
            margin-bottom: 15px;
            color: #764ba2;
            display: flex;
            align-items: center;
        }}
        
        .badge {{
            display: inline-block;
            padding: 4px 12px;
            border-radius: 12px;
            font-size: 0.75em;
            font-weight: bold;
            margin-left: 10px;
        }}
        
        .badge-cert {{
            background: #ff6b6b;
            color: white;
        }}
        
        .badge-misra {{
            background: #4ecdc4;
            color: white;
        }}
        
        table {{
            width: 100%;
            border-collapse: collapse;
            margin-top: 15px;
            background: white;
            box-shadow: 0 2px 8px rgba(0,0,0,0.1);
            border-radius: 8px;
            overflow: hidden;
        }}
        
        thead {{
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
        }}
        
        th {{
            padding: 15px;
            text-align: left;
            font-weight: 600;
            position: sticky;
            top: 0;
        }}
        
        td {{
            padding: 12px 15px;
            border-bottom: 1px solid #e0e0e0;
        }}
        
        tbody tr:hover {{
            background: #f8f9fa;
        }}
        
        tbody tr:nth-child(even) {{
            background: #fafbfc;
        }}
        
        tbody tr:nth-child(odd) {{
            background: #ffffff;
        }}
        
        code {{
            background: #f1f3f5;
            padding: 2px 6px;
            border-radius: 3px;
            font-family: 'Courier New', monospace;
            font-size: 0.9em;
            color: #495057;
        }}
        
        .severity-CRITICAL {{
            background: #ff4757;
            color: white;
            padding: 4px 8px;
            border-radius: 4px;
            font-size: 0.85em;
            font-weight: bold;
        }}
        
        .severity-HIGH {{
            background: #ffa502;
            color: white;
            padding: 4px 8px;
            border-radius: 4px;
            font-size: 0.85em;
            font-weight: bold;
        }}
        
        .severity-MEDIUM {{
            background: #95a5a6;
            color: white;
            padding: 4px 8px;
            border-radius: 4px;
            font-size: 0.85em;
            font-weight: bold;
        }}
        
        .severity-LOW {{
            background: #a4b0be;
            color: white;
            padding: 4px 8px;
            border-radius: 4px;
            font-size: 0.85em;
            font-weight: bold;
        }}
        
        .stats-grid {{
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 15px;
            margin-top: 15px;
        }}
        
        .stat-item {{
            background: #f8f9fa;
            padding: 15px;
            border-radius: 6px;
            border-left: 4px solid #667eea;
        }}
        
        .stat-label {{
            font-size: 0.85em;
            color: #666;
            margin-bottom: 5px;
        }}
        
        .stat-value {{
            font-size: 1.5em;
            font-weight: bold;
            color: #333;
        }}
        
        .empty-state {{
            text-align: center;
            padding: 40px;
            color: #999;
            font-style: italic;
        }}
        
        .footer {{
            background: #f8f9fa;
            padding: 20px;
            text-align: center;
            color: #666;
            font-size: 0.9em;
            border-top: 1px solid #e0e0e0;
        }}
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🔍 CERT & MISRA Violation Report</h1>
            <div class="module-name">Module: {module_name}</div>
            <div class="timestamp">Generated on {datetime.now().strftime("%B %d, %Y at %I:%M %p")}</div>
        </div>
        
        <div class="summary-cards">
            <div class="card">
                <div class="card-title">Total Violations</div>
                <div class="card-value">{total_violations}</div>
                <div class="card-subtitle">CERT + MISRA</div>
            </div>
            <div class="card">
                <div class="card-title">CERT Violations</div>
                <div class="card-value" style="color: #ff6b6b;">{len(cert_violations)}</div>
                <div class="card-subtitle">{cert_stats['open']} Open, {cert_stats['fixed']} Fixed</div>
            </div>
            <div class="card">
                <div class="card-title">MISRA Violations</div>
                <div class="card-value" style="color: #4ecdc4;">{len(misra_violations)}</div>
                <div class="card-subtitle">{misra_stats['open']} Open, {misra_stats['fixed']} Fixed</div>
            </div>
            <div class="card">
                <div class="card-title">Justified</div>
                <div class="card-value" style="color: #95a5a6;">{total_justified}</div>
                <div class="card-subtitle">With Rationale</div>
            </div>
        </div>
"""
    
    # Add info note if suppressions were found
    if total_suppressions > 0:
        html_content += f"""
        <div class="section" style="padding: 15px 30px;">
            <div style="background: #e8f5e9; border-left: 4px solid #4caf50; padding: 15px; border-radius: 4px;">
                <strong>ℹ️ Justified Count Details:</strong><br>
                • {cert_stats['justified'] + misra_stats['justified']} violations marked as justified in Knowledge Base<br>
                • {total_suppressions} parasoft-suppress comments found in source code files<br>
                • <strong>Total Justified: {total_justified}</strong>
            </div>
        </div>
"""
    else:
        html_content += f"""
        <div class="section" style="padding: 15px 30px;">
            <div style="background: #fff3cd; border-left: 4px solid #ffc107; padding: 15px; border-radius: 4px;">
                <strong>⚠️ No Suppression Comments Found:</strong><br>
                No parasoft-suppress comments were detected in the source code files.<br>
                <br>
                <strong>How to fix:</strong><br>
                1. Open the GUI and check the <strong>Input Path</strong> field<br>
                2. Make sure it points to your actual source code directory<br>
                3. Regenerate this report after setting the correct path<br>
                <br>
                Alternatively, place source code in: <code>Input/{module_name}*/</code>
            </div>
        </div>
"""

    # CERT Section
    html_content += f"""
        <div class="section">
            <div class="section-title">CERT C Violations <span class="badge badge-cert">{len(cert_violations)} Total</span></div>
"""
    
    if cert_violations:
        # CERT Statistics
        html_content += """
            <div class="subsection">
                <div class="subsection-title">Statistics</div>
                <div class="stats-grid">
"""
        for severity, count in sorted(cert_stats['by_severity'].items(), key=lambda x: x[1], reverse=True):
            html_content += f"""
                    <div class="stat-item">
                        <div class="stat-label">{severity}</div>
                        <div class="stat-value">{count}</div>
                    </div>
"""
        html_content += """
                </div>
            </div>
"""
        
        # CERT Top Violations
        html_content += """
            <div class="subsection">
                <div class="subsection-title">Top 10 CERT Violations (Summary)</div>
                <table>
                    <thead>
                        <tr>
                            <th>Rank</th>
                            <th>Rule ID</th>
                            <th>Severity</th>
                            <th>Count</th>
                            <th>Message</th>
                        </tr>
                    </thead>
                    <tbody>
"""
        for idx, (rule_id, data) in enumerate(cert_top, 1):
            html_content += f"""
                        <tr>
                            <td><strong>#{idx}</strong></td>
                            <td><strong>{rule_id}</strong></td>
                            <td><span class="severity-{data['severity']}">{data['severity']}</span></td>
                            <td><strong>{data['count']}</strong></td>
                            <td>{data['message'][:100]}...</td>
                        </tr>
"""
        html_content += """
                    </tbody>
                </table>
            </div>
"""
        
        # CERT All Violations
        html_content += f"""
            <div class="subsection">
                <div class="subsection-title">All CERT Violations ({len(cert_violations)} Total)</div>
                <table>
                    <thead>
                        <tr>
                            <th>#</th>
                            <th>Rule ID</th>
                            <th>Severity</th>
                            <th>File</th>
                            <th>Line</th>
                            <th>Message</th>
                            <th>Status</th>
                        </tr>
                    </thead>
                    <tbody>
"""
        for idx, violation in enumerate(cert_violations, 1):
            rule_id = violation.get('violation_id', 'UNKNOWN')
            severity = violation.get('severity', 'UNKNOWN')
            message = violation.get('violation_text', '')[:100]
            
            # Get file information
            files_affected = violation.get('files_affected', [])
            if files_affected:
                if isinstance(files_affected[0], dict):
                    file_name = files_affected[0].get('file', 'UNKNOWN')
                    line_number = files_affected[0].get('line', '-')  # Fixed: use 'line' not 'line_number'
                else:
                    file_name = str(files_affected[0])
                    line_number = '-'
            else:
                file_name = 'UNKNOWN'
                line_number = '-'
            
            # Determine status
            if violation.get('fix_applied'):
                status = '<span style="color: #27ae60; font-weight: bold;">✓ Fixed</span>'
            elif violation.get('justification') or violation.get('justified'):
                status = '<span style="color: #f39c12; font-weight: bold;">⚠ Justified</span>'
            else:
                status = '<span style="color: #e74c3c; font-weight: bold;">✗ Open</span>'
            
            html_content += f"""
                        <tr>
                            <td>{idx}</td>
                            <td><strong>{rule_id}</strong></td>
                            <td><span class="severity-{severity}">{severity}</span></td>
                            <td><code>{file_name}</code></td>
                            <td>{line_number}</td>
                            <td>{message}</td>
                            <td>{status}</td>
                        </tr>
"""
        html_content += """
                    </tbody>
                </table>
            </div>
"""
    else:
        html_content += '<div class="empty-state">No CERT violations found in knowledge base</div>'
    
    html_content += "</div>"
    
    # MISRA Section
    html_content += f"""
        <div class="section">
            <div class="section-title">MISRA C Violations <span class="badge badge-misra">{len(misra_violations)} Total</span></div>
"""
    
    if misra_violations:
        # MISRA Statistics
        html_content += """
            <div class="subsection">
                <div class="subsection-title">Statistics</div>
                <div class="stats-grid">
"""
        for severity, count in sorted(misra_stats['by_severity'].items(), key=lambda x: x[1], reverse=True):
            html_content += f"""
                    <div class="stat-item">
                        <div class="stat-label">{severity}</div>
                        <div class="stat-value">{count}</div>
                    </div>
"""
        html_content += """
                </div>
            </div>
"""
        
        # MISRA Top Violations
        html_content += """
            <div class="subsection">
                <div class="subsection-title">Top 10 MISRA Violations (Summary)</div>
                <table>
                    <thead>
                        <tr>
                            <th>Rank</th>
                            <th>Rule ID</th>
                            <th>Severity</th>
                            <th>Count</th>
                            <th>Message</th>
                        </tr>
                    </thead>
                    <tbody>
"""
        for idx, (rule_id, data) in enumerate(misra_top, 1):
            html_content += f"""
                        <tr>
                            <td><strong>#{idx}</strong></td>
                            <td><strong>{rule_id}</strong></td>
                            <td><span class="severity-{data['severity']}">{data['severity']}</span></td>
                            <td><strong>{data['count']}</strong></td>
                            <td>{data['message'][:100]}...</td>
                        </tr>
"""
        html_content += """
                    </tbody>
                </table>
            </div>
"""
        
        # MISRA All Violations
        html_content += f"""
            <div class="subsection">
                <div class="subsection-title">All MISRA Violations ({len(misra_violations)} Total)</div>
                <table>
                    <thead>
                        <tr>
                            <th>#</th>
                            <th>Rule ID</th>
                            <th>Severity</th>
                            <th>File</th>
                            <th>Line</th>
                            <th>Message</th>
                            <th>Status</th>
                        </tr>
                    </thead>
                    <tbody>
"""
        for idx, violation in enumerate(misra_violations, 1):
            rule_id = violation.get('violation_id', 'UNKNOWN')
            severity = violation.get('severity', 'UNKNOWN')
            message = violation.get('violation_text', '')[:100]
            
            # Get file information
            files_affected = violation.get('files_affected', [])
            if files_affected:
                if isinstance(files_affected[0], dict):
                    file_name = files_affected[0].get('file', 'UNKNOWN')
                    line_number = files_affected[0].get('line', '-')  # Fixed: use 'line' not 'line_number'
                else:
                    file_name = str(files_affected[0])
                    line_number = '-'
            else:
                file_name = 'UNKNOWN'
                line_number = '-'
            
            # Determine status
            if violation.get('fix_applied'):
                status = '<span style="color: #27ae60; font-weight: bold;">✓ Fixed</span>'
            elif violation.get('justification') or violation.get('justified'):
                status = '<span style="color: #f39c12; font-weight: bold;">⚠ Justified</span>'
            else:
                status = '<span style="color: #e74c3c; font-weight: bold;">✗ Open</span>'
            
            html_content += f"""
                        <tr>
                            <td>{idx}</td>
                            <td><strong>{rule_id}</strong></td>
                            <td><span class="severity-{severity}">{severity}</span></td>
                            <td><code>{file_name}</code></td>
                            <td>{line_number}</td>
                            <td>{message}</td>
                            <td>{status}</td>
                        </tr>
"""
        html_content += """
                    </tbody>
                </table>
            </div>
"""
    else:
        html_content += '<div class="empty-state">No MISRA violations found in knowledge base</div>'
    
    html_content += """
        </div>
        
        <div class="footer">
            <div style="margin-bottom: 10px;">
                Generated by <strong>Parasoft Analysis Tool v4.0.0</strong> | Qorix India Pvt Ltd | CERT & MISRA Report Generator
            </div>
            <div style="font-size: 0.85em; color: #999;">
                ✅ Showing ALL violations (Top 10 Summary + Complete Details) | Report generated on """ + datetime.now().strftime("%B %d, %Y at %I:%M %p") + """
            </div>
        </div>
    </div>
</body>
</html>
"""
    
    # Write HTML file
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(html_content)
    
    return output_path

def main():
    if len(sys.argv) < 2:
        print("Usage: python generate_cert_misra_report.py <ModuleName>")
        sys.exit(1)
    
    module_name = sys.argv[1]
    
    # Setup paths
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    kb_dir = project_root / "knowledge_base"
    reports_dir = project_root / "reports"
    
    # Ensure reports directory exists
    reports_dir.mkdir(exist_ok=True)
    
    # Load knowledge base
    kb_path = kb_dir / f"{module_name}_KnowledgeDatabase.json"
    
    print(f"\n{'='*80}")
    print(f"CERT & MISRA Violation Report Generator")
    print(f"{'='*80}")
    print(f"Module: {module_name}")
    print(f"Knowledge Base: {kb_path}")
    print(f"{'='*80}\n")
    
    kb_data = load_knowledge_base(kb_path)
    if not kb_data:
        print("[ERROR] Failed to load knowledge base")
        sys.exit(1)
    
    print(f"✓ Knowledge base loaded successfully")
    print(f"  Total violations in KB: {len(kb_data.get('violations', {}))}")
    
    # Filter CERT and MISRA violations
    cert_violations, misra_violations = filter_cert_misra_violations(kb_data)
    
    print(f"\n✓ Filtered violations:")
    print(f"  CERT violations: {len(cert_violations)}")
    print(f"  MISRA violations: {len(misra_violations)}")
    
    if len(cert_violations) == 0 and len(misra_violations) == 0:
        print("\n[WARNING] No CERT or MISRA violations found in knowledge base")
        print("The report will be generated but will show empty results")
    
    # Analyze violations
    cert_stats = analyze_violations(cert_violations)
    misra_stats = analyze_violations(misra_violations)
    
    # Count suppressions from actual source code files
    print(f"\n✓ Scanning source code for parasoft-suppress comments...")
    total_suppressions = count_suppressions_from_source_code(module_name, kb_data)
    
    if total_suppressions > 0:
        print(f"  ✓ Found {total_suppressions} suppression comments in source code")
    else:
        print(f"  ⚠️  No suppressions found in source code")
        print(f"  💡 Make sure source code is in Input/{module_name}*/ folder")
    
    # Add suppression count to stats (this represents justified violations)
    kb_justified = cert_stats['justified'] + misra_stats['justified']
    total_justified = kb_justified + total_suppressions
    
    if kb_justified > 0:
        print(f"  Justifications in KB: {kb_justified}")
    print(f"  Total justified violations: {total_justified}")
    
    # Generate HTML report
    output_path = reports_dir / f"{module_name}_CERT_MISRA_Report.html"
    
    print(f"\n✓ Generating HTML report...")
    generate_html_report(module_name, cert_violations, misra_violations, 
                         cert_stats, misra_stats, output_path, total_suppressions)
    
    print(f"\n{'='*80}")
    print(f"SUCCESS: Report generated successfully!")
    print(f"{'='*80}")
    print(f"Output file: {output_path}")
    print(f"{'='*80}\n")
    
    # Auto-open in browser
    try:
        os.startfile(str(output_path))
        print("✓ Opening report in browser...")
    except:
        print("Note: Could not auto-open browser")
    
    return 0

if __name__ == "__main__":
    sys.exit(main())
