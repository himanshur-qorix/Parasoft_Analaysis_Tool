"""
Consolidate Knowledge Bases
Merges all module-specific knowledge databases into a master database
Developer: Himanshu R
Version: 1.0.0
"""

import sys
from pathlib import Path

# Add src directory to path
src_dir = Path(__file__).parent.parent / "src"
sys.path.insert(0, str(src_dir))

from MasterKnowledgeBaseManager import MasterKnowledgeBaseManager
import logging

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s'
)
logger = logging.getLogger(__name__)


def main():
    """Main consolidation function"""
    print("\n" + "="*60)
    print("     KNOWLEDGE BASE CONSOLIDATION TOOL")
    print("="*60 + "\n")
    
    # Get workspace path
    workspace_path = Path(__file__).parent.parent
    knowledge_base_dir = workspace_path / "knowledge_base"
    
    if not knowledge_base_dir.exists():
        print("[ERROR] Knowledge base directory not found")
        print(f"Expected: {knowledge_base_dir}")
        sys.exit(1)
    
    # Check for module databases
    module_dbs = list(knowledge_base_dir.glob("*_KnowledgeDatabase.json"))
    module_dbs = [db for db in module_dbs if "Master" not in db.name]
    
    if not module_dbs:
        print("[ERROR] No module knowledge databases found")
        print(f"Please run analysis on at least one module first")
        sys.exit(1)
    
    print(f"[INFO] Found {len(module_dbs)} module databases:")
    for db in module_dbs:
        module_name = db.stem.replace("_KnowledgeDatabase", "")
        print(f"  - {module_name}")
    
    print()
    
    # Initialize master knowledge base manager
    print("[INFO] Initializing Master Knowledge Base Manager...")
    master_kb = MasterKnowledgeBaseManager(knowledge_base_dir)
    
    # Load all module databases
    print("[INFO] Loading module databases...")
    master_kb.load_all_module_databases()
    
    # Consolidate knowledge bases
    print("[INFO] Consolidating knowledge bases...")
    master_db = master_kb.consolidate_knowledge_bases()
    
    # Save master database
    print("[INFO] Saving master database...")
    master_path = master_kb.save_master_database()
    
    # Generate recommendations
    print("[INFO] Generating recommendations...")
    recommendations = master_kb.generate_recommendations()
    master_kb.save_master_database()  # Save with recommendations
    
    # Display summary
    print("\n" + "="*60)
    print("CONSOLIDATION SUMMARY")
    print("="*60)
    print(f"  Total Modules: {master_db['total_modules']}")
    print(f"  Total Unique Violations: {master_db['total_unique_violations']}")
    print(f"  Total Violations Across Modules: {master_db['total_violations_across_modules']}")
    print(f"  Cross-Module Violations: {len(master_db['cross_module_violations'])}")
    print()
    print(f"[OK] Master database saved: {master_path}")
    
    # Display recommendations
    if recommendations:
        print("\n" + "="*60)
        print("RECOMMENDATIONS")
        print("="*60)
        for i, rec in enumerate(recommendations, 1):
            print(f"\n{i}. [{rec['type']}] {rec['title']}")
            print(f"   {rec['description']}")
            print(f"   Action: {rec['action']}")
    
    # Export to Excel
    print("\n[INFO] Exporting consolidated report to Excel...")
    excel_path = workspace_path / "reports" / "Master_Knowledge_Report.xlsx"
    excel_path.parent.mkdir(exist_ok=True)
    master_kb.export_consolidated_report(excel_path)
    print(f"[OK] Excel report saved: {excel_path}")
    
    # Export to HTML
    print("[INFO] Exporting consolidated report to HTML...")
    html_path = workspace_path / "reports" / "Master_Knowledge_Report.html"
    master_kb.export_html_report(html_path)
    print(f"[OK] HTML report saved: {html_path}")
    
    # Display top violations
    print("\n" + "="*60)
    print("TOP 10 MOST COMMON VIOLATIONS (ACROSS ALL MODULES)")
    print("="*60)
    
    most_common = master_db['statistics'].get('most_common_violations_global', [])[:10]
    for i, violation in enumerate(most_common, 1):
        print(f"\n{i}. {violation['violation_id']}")
        print(f"   Severity: {violation['severity']} | Category: {violation['category']}")
        print(f"   Total Occurrences: {violation['total_occurrences']}")
        print(f"   Modules Affected: {violation['modules_affected']}")
    
    print("\n" + "="*60)
    print("[SUCCESS] Consolidation completed successfully!")
    print("="*60 + "\n")
    
    print("Next steps:")
    print("  1. Review the master database: knowledge_base/Master_KnowledgeDatabase.json")
    print("  2. Check the Excel report: reports/Master_Knowledge_Report.xlsx")
    print("  3. Open HTML report in browser: reports/Master_Knowledge_Report.html")
    print("  4. Use the master database to guide future analyses")
    print("  5. Apply proven fixes from other modules to current work")
    print()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n\n[INFO] Consolidation cancelled by user")
        sys.exit(0)
    except Exception as e:
        logger.error(f"Error during consolidation: {str(e)}", exc_info=True)
        print(f"\n[ERROR] {str(e)}")
        sys.exit(1)
