"""
Knowledge Base Diagnostic Tool
Checks violation data format and provides detailed debugging info

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 4.1.0
"""

import json
import sys
from pathlib import Path
from typing import Dict


def check_knowledge_base(kb_path: Path) -> Dict:
    """
    Check knowledge base violation data format
    
    Args:
        kb_path: Path to knowledge base JSON
    
    Returns:
        Diagnostic information
    """
    try:
        with open(kb_path, 'r', encoding='utf-8') as f:
            kb_data = json.load(f)
        
        violations = kb_data.get('violations', {})
        
        if not violations:
            return {
                'status': 'empty',
                'message': 'No violations found in knowledge base'
            }
        
        # Sample first few violations
        sample_violations = list(violations.items())[:5]
        
        diagnostics = {
            'status': 'ok',
            'total_violations': len(violations),
            'samples': []
        }
        
        for vid, vdata in sample_violations:
            files_affected = vdata.get('files_affected', [])
            
            sample_info = {
                'violation_id': vid,
                'files_affected_count': len(files_affected),
                'files_affected_type': type(files_affected[0]).__name__ if files_affected else 'N/A',
                'has_code_snippet': bool(vdata.get('code_snippet')) and vdata.get('code_snippet') != 'No code snippet available',
                'has_file_path': bool(vdata.get('file_path')) and vdata.get('file_path') not in ['UNKNOWN', 'N/A', ''],
                'enriched': vdata.get('enriched', False)
            }
            
            # Show sample file entry
            if files_affected:
                sample_file = files_affected[0]
                if isinstance(sample_file, dict):
                    sample_info['file_entry_format'] = 'dict'
                    sample_info['file'] = sample_file.get('file', 'N/A')
                    sample_info['line'] = sample_file.get('line', 'N/A')
                elif isinstance(sample_file, str):
                    sample_info['file_entry_format'] = 'string'
                    sample_info['file_entry'] = sample_file
                else:
                    sample_info['file_entry_format'] = 'unknown'
            
            diagnostics['samples'].append(sample_info)
        
        # Count enriched violations
        enriched_count = sum(1 for v in violations.values() if v.get('enriched'))
        needs_enrichment = sum(1 for v in violations.values() 
                               if not v.get('code_snippet') or v.get('file_path') in ['UNKNOWN', 'N/A', ''])
        
        diagnostics['enriched_count'] = enriched_count
        diagnostics['needs_enrichment'] = needs_enrichment
        diagnostics['enrichment_percentage'] = round(enriched_count / len(violations) * 100, 1)
        
        return diagnostics
    
    except Exception as e:
        return {
            'status': 'error',
            'message': str(e)
        }


def print_diagnostics(diagnostics: Dict):
    """Print diagnostic information in readable format"""
    print("\n" + "="*80)
    print("  KNOWLEDGE BASE DIAGNOSTICS")
    print("="*80)
    
    if diagnostics['status'] == 'error':
        print(f"\n❌ ERROR: {diagnostics['message']}")
        return
    
    if diagnostics['status'] == 'empty':
        print(f"\n⚠️  {diagnostics['message']}")
        return
    
    print(f"\n📊 Total Violations: {diagnostics['total_violations']}")
    print(f"✅ Enriched: {diagnostics['enriched_count']} ({diagnostics['enrichment_percentage']}%)")
    print(f"⚠️  Needs Enrichment: {diagnostics['needs_enrichment']}")
    
    print(f"\n📋 Sample Violations (first 5):")
    print("-"*80)
    
    for i, sample in enumerate(diagnostics['samples'], 1):
        print(f"\n[{i}] {sample['violation_id']}")
        print(f"    Files Affected: {sample['files_affected_count']}")
        
        if 'file_entry_format' in sample:
            print(f"    Format: {sample['file_entry_format']}")
            
            if sample['file_entry_format'] == 'dict':
                print(f"    File: {sample.get('file', 'N/A')}")
                print(f"    Line: {sample.get('line', 'N/A')}")
            elif sample['file_entry_format'] == 'string':
                print(f"    Entry: {sample.get('file_entry', 'N/A')}")
        
        print(f"    Has Code Snippet: {'✅' if sample['has_code_snippet'] else '❌'}")
        print(f"    Has File Path: {'✅' if sample['has_file_path'] else '❌'}")
        print(f"    Enriched: {'✅' if sample['enriched'] else '❌'}")
    
    print("\n" + "="*80)
    
    # Recommendations
    if diagnostics['needs_enrichment'] > 0:
        print("\n💡 RECOMMENDATION:")
        print(f"   {diagnostics['needs_enrichment']} violations need enrichment!")
        print("\n   Run:")
        print(f"   Enrich_Violations.bat MODULE_NAME \"SOURCE_CODE_PATH\"")
        print("\n   Or it will be done automatically when you run Apply_Fixes.bat")
    else:
        print("\n✅ All violations are enriched and ready for AI-powered fixes!")
        print("\n   Run:")
        print("   Apply_Fixes.bat MODULE_NAME \"SOURCE_CODE_PATH\"")


def main():
    """Main entry point"""
    if len(sys.argv) < 2:
        print("Usage: python src/check_kb.py knowledge_base/MODULE_KnowledgeDatabase.json")
        print("\nExample:")
        print("  python src/check_kb.py knowledge_base/Mka_KnowledgeDatabase.json")
        sys.exit(1)
    
    kb_path = Path(sys.argv[1])
    
    if not kb_path.exists():
        print(f"❌ ERROR: Knowledge base not found: {kb_path}")
        sys.exit(1)
    
    print(f"\nChecking knowledge base: {kb_path.name}")
    
    diagnostics = check_knowledge_base(kb_path)
    print_diagnostics(diagnostics)
    
    print()


if __name__ == '__main__':
    main()
