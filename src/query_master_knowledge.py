"""
Query Master Knowledge Base
Interactive tool to query and analyze the consolidated master knowledge base
Developer: Himanshu R
Version: 1.0.0
"""

import sys
from pathlib import Path
from datetime import datetime

# Add src directory to path
src_dir = Path(__file__).parent.parent / "src"
sys.path.insert(0, str(src_dir))

from MasterKnowledgeBaseManager import MasterKnowledgeBaseManager
import json


def display_menu():
    """Display the main menu"""
    print("\n" + "="*60)
    print("MASTER KNOWLEDGE BASE - QUERY TOOL")
    print("="*60)
    print("\n[1] Show Summary Statistics")
    print("[2] View Cross-Module Violations")
    print("[3] Get Violation Insights (by ID)")
    print("[4] Show Top Common Violations")
    print("[5] View Violations by Category")
    print("[6] View Violations by Severity")
    print("[7] Find Violations with Proven Fixes")
    print("[8] Show Recommendations")
    print("[9] Export to Excel")
    print("[10] Export to HTML")
    print("[0] Exit")
    print()


def show_summary(master_kb):
    """Display summary statistics"""
    master_db = master_kb.master_database
    
    print("\n" + "="*60)
    print("SUMMARY STATISTICS")
    print("="*60)
    print(f"  Total Modules Included: {master_db.get('total_modules', 0)}")
    print(f"  Modules: {', '.join(master_db.get('modules_included', []))}")
    print(f"  Total Unique Violations: {master_db.get('total_unique_violations', 0)}")
    print(f"  Total Violations Across All Modules: {master_db.get('total_violations_across_modules', 0)}")
    print(f"  Cross-Module Violations: {len(master_db.get('cross_module_violations', []))}")
    print(f"  Last Updated: {master_db.get('last_updated', 'N/A')}")
    
    # Category breakdown
    cat_stats = master_db.get('statistics', {}).get('violations_by_category', {})
    if cat_stats:
        print("\n  Violations by Category:")
        for category, count in sorted(cat_stats.items(), key=lambda x: x[1], reverse=True):
            print(f"    {category}: {count}")
    
    # Severity breakdown
    sev_stats = master_db.get('statistics', {}).get('violations_by_severity', {})
    if sev_stats:
        print("\n  Violations by Severity:")
        for severity, count in sorted(sev_stats.items(), key=lambda x: x[1], reverse=True):
            print(f"    {severity}: {count}")


def show_cross_module(master_kb):
    """Display cross-module violations"""
    cross_violations = master_kb.get_cross_module_patterns()
    
    if not cross_violations:
        print("\n[INFO] No cross-module violations found")
        return
    
    print("\n" + "="*60)
    print(f"CROSS-MODULE VIOLATIONS (Top {min(20, len(cross_violations))})")
    print("="*60)
    
    for i, violation in enumerate(cross_violations[:20], 1):
        print(f"\n{i}. {violation['violation_id']}")
        print(f"   Severity: {violation['severity']}")
        print(f"   Modules Affected: {violation['modules_count']}")
        print(f"   Total Occurrences: {violation['total_occurrences']}")


def get_violation_insights(master_kb):
    """Get insights for a specific violation"""
    violation_id = input("\nEnter Violation ID: ").strip()
    
    insights = master_kb.get_violation_insights(violation_id)
    
    if not insights:
        print(f"\n[ERROR] Violation '{violation_id}' not found in master database")
        return
    
    print("\n" + "="*60)
    print(f"INSIGHTS FOR: {violation_id}")
    print("="*60)
    print(f"  Category: {insights['category']}")
    print(f"  Severity: {insights['severity']}")
    print(f"  Total Occurrences: {insights['total_occurrences']}")
    print(f"  Modules Affected: {insights['modules_affected']}")
    print(f"  Fix Success Rate: {insights['fix_success_rate']:.1f}%")
    print(f"  Has Proven Fixes: {'Yes' if insights['has_proven_fixes'] else 'No'}")
    print(f"  Has Justifications: {'Yes' if insights['has_justifications'] else 'No'}")
    
    if insights['recommended_fix']:
        print(f"\n  Recommended Fix (from {insights['recommended_fix']['module']}):")
        fix_details = insights['recommended_fix'].get('fix_details', {})
        if isinstance(fix_details, dict):
            print(f"    {json.dumps(fix_details, indent=4)}")
        else:
            print(f"    {fix_details}")
    
    if insights['recommended_justification']:
        print(f"\n  Recommended Justification (from {insights['recommended_justification']['module']}):")
        print(f"    {insights['recommended_justification']['justification']}")


def show_top_violations(master_kb):
    """Show top common violations"""
    master_db = master_kb.master_database
    most_common = master_db.get('statistics', {}).get('most_common_violations_global', [])
    
    if not most_common:
        print("\n[INFO] No violation statistics available")
        return
    
    count = int(input("\nHow many top violations to show? [10]: ").strip() or "10")
    
    print("\n" + "="*60)
    print(f"TOP {min(count, len(most_common))} MOST COMMON VIOLATIONS")
    print("="*60)
    
    for i, violation in enumerate(most_common[:count], 1):
        print(f"\n{i}. {violation['violation_id']}")
        print(f"   Severity: {violation['severity']} | Category: {violation['category']}")
        print(f"   Total Occurrences: {violation['total_occurrences']}")
        print(f"   Modules Affected: {violation['modules_affected']}")


def show_by_category(master_kb):
    """Show violations filtered by category"""
    print("\nAvailable categories: CERT, MISRA, CWE, OTHER")
    category = input("Enter category: ").strip().upper()
    
    master_db = master_kb.master_database
    violations = [
        (vid, vdata) for vid, vdata in master_db.get('violations', {}).items()
        if vdata.get('category', '').upper() == category
    ]
    
    if not violations:
        print(f"\n[INFO] No violations found for category: {category}")
        return
    
    # Sort by occurrences
    violations.sort(key=lambda x: x[1]['total_occurrences'], reverse=True)
    
    print("\n" + "="*60)
    print(f"VIOLATIONS IN CATEGORY: {category} ({len(violations)} total)")
    print("="*60)
    
    for i, (vid, vdata) in enumerate(violations[:20], 1):
        print(f"\n{i}. {vid}")
        print(f"   Severity: {vdata['severity']}")
        print(f"   Total Occurrences: {vdata['total_occurrences']}")
        print(f"   Modules: {len(vdata['modules_affected'])}")


def show_by_severity(master_kb):
    """Show violations filtered by severity"""
    print("\nAvailable severities: HIGH, MEDIUM, LOW")
    severity = input("Enter severity: ").strip().upper()
    
    master_db = master_kb.master_database
    violations = [
        (vid, vdata) for vid, vdata in master_db.get('violations', {}).items()
        if vdata.get('severity', '').upper() == severity
    ]
    
    if not violations:
        print(f"\n[INFO] No violations found for severity: {severity}")
        return
    
    # Sort by occurrences
    violations.sort(key=lambda x: x[1]['total_occurrences'], reverse=True)
    
    print("\n" + "="*60)
    print(f"VIOLATIONS WITH SEVERITY: {severity} ({len(violations)} total)")
    print("="*60)
    
    for i, (vid, vdata) in enumerate(violations[:20], 1):
        print(f"\n{i}. {vid}")
        print(f"   Category: {vdata['category']}")
        print(f"   Total Occurrences: {vdata['total_occurrences']}")
        print(f"   Modules: {len(vdata['modules_affected'])}")


def show_with_fixes(master_kb):
    """Show violations that have proven fixes"""
    master_db = master_kb.master_database
    violations_with_fixes = [
        (vid, vdata) for vid, vdata in master_db.get('violations', {}).items()
        if len(vdata.get('fix_examples', [])) > 0
    ]
    
    if not violations_with_fixes:
        print("\n[INFO] No violations with proven fixes found")
        return
    
    # Sort by fix success rate
    violations_with_fixes.sort(key=lambda x: x[1]['fix_success_rate'], reverse=True)
    
    print("\n" + "="*60)
    print(f"VIOLATIONS WITH PROVEN FIXES ({len(violations_with_fixes)} total)")
    print("="*60)
    
    for i, (vid, vdata) in enumerate(violations_with_fixes[:20], 1):
        print(f"\n{i}. {vid}")
        print(f"   Severity: {vdata['severity']} | Category: {vdata['category']}")
        print(f"   Fix Success Rate: {vdata['fix_success_rate']:.1f}%")
        print(f"   Modules with Fixes: {len(vdata['fix_examples'])}")
        print(f"   Total Occurrences: {vdata['total_occurrences']}")


def show_recommendations(master_kb):
    """Display recommendations"""
    recommendations = master_kb.master_database.get('recommendations', [])
    
    if not recommendations:
        print("\n[INFO] No recommendations available. Regenerating...")
        recommendations = master_kb.generate_recommendations()
        master_kb.save_master_database()
    
    print("\n" + "="*60)
    print(f"RECOMMENDATIONS ({len(recommendations)} total)")
    print("="*60)
    
    for i, rec in enumerate(recommendations, 1):
        print(f"\n{i}. [{rec['type']}] {rec['title']}")
        print(f"   {rec['description']}")
        print(f"   Action: {rec['action']}")
        
        if 'count' in rec:
            print(f"   Count: {rec['count']}")
        
        if rec.get('violations'):
            print(f"   Violations: {len(rec['violations'])}")


def export_to_excel(master_kb):
    """Export master database to Excel"""
    workspace_path = Path(__file__).parent.parent
    default_path = workspace_path / "reports" / "Master_Knowledge_Report.xlsx"
    
    output = input(f"\nExport path [{default_path}]: ").strip()
    if not output:
        output = default_path
    else:
        output = Path(output)
        # If user provided a directory, append the default filename
        if output.is_dir() or (not output.exists() and not output.suffix):
            output = output / "Master_Knowledge_Report.xlsx"
    
    output.parent.mkdir(parents=True, exist_ok=True)
    
    print(f"\n[INFO] Exporting to {output}...")
    master_kb.export_consolidated_report(output)
    print(f"[OK] Export completed successfully!")


def export_to_html(master_kb):
    """Export master database to HTML"""
    workspace_path = Path(__file__).parent.parent
    default_path = workspace_path / "reports" / "Master_Knowledge_Report.html"
    
    output = input(f"\nExport path [{default_path}]: ").strip()
    if not output:
        output = default_path
    else:
        output = Path(output)
        # If user provided a directory, append the default filename
        if output.is_dir() or (not output.exists() and not output.suffix):
            output = output / "Master_Knowledge_Report.html"
    
    output.parent.mkdir(parents=True, exist_ok=True)
    
    print(f"\n[INFO] Exporting to {output}...")
    master_kb.export_html_report(output)
    print(f"[OK] HTML report created successfully!")
    print(f"[INFO] Open {output} in your browser to view the report.")


def main():
    """Main query function - supports both interactive and command-line modes"""
    import argparse
    
    # Parse command-line arguments
    parser = argparse.ArgumentParser(description='Query Master Knowledge Base')
    parser.add_argument('--summary', action='store_true', help='Show summary statistics')
    parser.add_argument('--cross_module', action='store_true', help='View cross-module violations')
    parser.add_argument('--violation_insights', type=str, help='Get insights for violation ID')
    parser.add_argument('--top_violations', type=str, default='10', help='Show top N violations')
    parser.add_argument('--by_category', type=str, help='Filter by category')
    parser.add_argument('--by_severity', type=str, help='Filter by severity')
    parser.add_argument('--proven_fixes', action='store_true', help='Show violations with proven fixes')
    parser.add_argument('--recommendations', action='store_true', help='Show recommendations')
    parser.add_argument('--export_excel', action='store_true', help='Export to Excel')
    parser.add_argument('--export_html', action='store_true', help='Export to HTML')
    
    args = parser.parse_args()
    
    workspace_path = Path(__file__).parent.parent
    knowledge_base_dir = workspace_path / "knowledge_base"
    master_db_path = knowledge_base_dir / "Master_KnowledgeDatabase.json"
    
    # Check if master database exists
    if not master_db_path.exists():
        print("\n" + "="*60)
        print("ERROR: Master Knowledge Database Not Found")
        print("="*60)
        print("\nThe master knowledge database has not been created yet.")
        print("\nPlease run the consolidation tool first:")
        print("  python src\\consolidate_knowledge.py")
        print()
        sys.exit(1)
    
    # Initialize master knowledge base manager
    master_kb = MasterKnowledgeBaseManager(knowledge_base_dir)
    master_kb.load_master_database()
    
    # Check if any command-line arguments were provided
    has_args = any([
        args.summary, args.cross_module, args.violation_insights,
        args.by_category, args.by_severity, args.proven_fixes,
        args.recommendations, args.export_excel, args.export_html
    ])
    
    if has_args:
        # Non-interactive mode - execute requested action and exit
        if args.summary:
            show_summary(master_kb)
        elif args.cross_module:
            show_cross_module(master_kb)
        elif args.violation_insights:
            # Create temporary function to handle non-interactive insights
            insights = master_kb.get_violation_insights(args.violation_insights)
            if insights:
                print("\n" + "="*60)
                print(f"INSIGHTS FOR: {args.violation_insights}")
                print("="*60)
                print(f"  Category: {insights['category']}")
                print(f"  Severity: {insights['severity']}")
                print(f"  Modules Affected: {', '.join(insights['modules'])}")
                print(f"  Total Occurrences: {insights['total_occurrences']}")
                if insights.get('proven_fixes'):
                    print(f"\n  ✅ Proven Fixes Available: {len(insights['proven_fixes'])}")
            else:
                print(f"\n[ERROR] Violation '{args.violation_insights}' not found")
        elif args.top_violations:
            try:
                count = int(args.top_violations)
                violations = master_kb.get_most_common_violations(count)
                print("\n" + "="*60)
                print(f"TOP {count} COMMON VIOLATIONS")
                print("="*60)
                for i, v in enumerate(violations, 1):
                    print(f"\n{i}. {v['violation_id']}")
                    print(f"   Occurrences: {v['total_occurrences']} across {v['modules_count']} modules")
                    print(f"   Severity: {v['severity']}")
            except ValueError:
                print(f"[ERROR] Invalid number: {args.top_violations}")
        elif args.by_category:
            violations = master_kb.get_violations_by_category(args.by_category.upper())
            print("\n" + "="*60)
            print(f"VIOLATIONS IN CATEGORY: {args.by_category.upper()}")
            print("="*60)
            if violations:
                for i, v in enumerate(violations[:20], 1):
                    print(f"\n{i}. {v['violation_id']}")
                    print(f"   Severity: {v['severity']}")
                    print(f"   Occurrences: {v['total_occurrences']}")
            else:
                print(f"\n[INFO] No violations found in category: {args.by_category}")
        elif args.by_severity:
            violations = master_kb.get_violations_by_severity(args.by_severity.upper())
            print("\n" + "="*60)
            print(f"VIOLATIONS WITH SEVERITY: {args.by_severity.upper()}")
            print("="*60)
            if violations:
                for i, v in enumerate(violations[:20], 1):
                    print(f"\n{i}. {v['violation_id']}")
                    print(f"   Category: {v.get('category', 'N/A')}")
                    print(f"   Occurrences: {v['total_occurrences']}")
            else:
                print(f"\n[INFO] No violations found with severity: {args.by_severity}")
        elif args.proven_fixes:
            show_with_fixes(master_kb)
        elif args.recommendations:
            show_recommendations(master_kb)
        elif args.export_excel:
            # Auto-generate filename
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            output_path = f"Master_Knowledge_Report_{timestamp}.xlsx"
            print(f"\n[INFO] Exporting to: {output_path}")
            master_kb.export_to_excel(output_path)
            print(f"[OK] Excel export complete: {output_path}")
        elif args.export_html:
            # Auto-generate filename
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            output_path = f"Master_Knowledge_Report_{timestamp}.html"
            print(f"\n[INFO] Exporting to: {output_path}")
            master_kb.export_to_html(output_path)
            print(f"[OK] HTML export complete: {output_path}")
        
        sys.exit(0)
    
    # Interactive mode - original behavior
    while True:
        display_menu()
        choice = input("Select option: ").strip()
        
        if choice == '1':
            show_summary(master_kb)
        elif choice == '2':
            show_cross_module(master_kb)
        elif choice == '3':
            get_violation_insights(master_kb)
        elif choice == '4':
            show_top_violations(master_kb)
        elif choice == '5':
            show_by_category(master_kb)
        elif choice == '6':
            show_by_severity(master_kb)
        elif choice == '7':
            show_with_fixes(master_kb)
        elif choice == '8':
            show_recommendations(master_kb)
        elif choice == '9':
            export_to_excel(master_kb)
        elif choice == '10':
            export_to_html(master_kb)
        elif choice == '0':
            print("\n[INFO] Exiting...\n")
            break
        else:
            print("\n[ERROR] Invalid option. Please try again.")
        
        input("\nPress Enter to continue...")


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\n[INFO] Query tool terminated by user")
        sys.exit(0)
    except Exception as e:
        print(f"\n[ERROR] {str(e)}")
        import traceback
        traceback.print_exc()
        sys.exit(1)
