"""
Parasoft Report Verification Tool
Compares tool's knowledge base against original Parasoft report to verify accuracy

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
"""

import json
import sys
from pathlib import Path
from collections import defaultdict
from ParasoftAnalysisTool import parse_parasoft_report


def load_knowledge_base(kb_path):
    """Load knowledge base JSON"""
    try:
        with open(kb_path, 'r', encoding='utf-8') as f:
            return json.load(f)
    except Exception as e:
        print(f"[ERROR] Failed to load KB: {e}")
        return None


def analyze_parasoft_report(report_path):
    """Analyze original Parasoft report"""
    print(f"\n📊 ANALYZING PARASOFT REPORT: {report_path}")
    print("=" * 70)
    
    violations = parse_parasoft_report(report_path)
    
    stats = {
        'total_violations': len(violations),
        'by_category': defaultdict(int),
        'by_rule': defaultdict(int),
        'by_file': defaultdict(int),
        'by_severity': defaultdict(int),
        'unique_rules': set()
    }
    
    for v in violations:
        rule_id = v.get('Violation ID', 'UNKNOWN')
        file_name = v.get('File', 'UNKNOWN')
        
        # Determine category from rule ID
        if 'CERT' in rule_id:
            category = 'CERT'
        elif 'MISRA' in rule_id:
            category = 'MISRA'
        else:
            category = 'OTHER'
        
        stats['by_category'][category] += 1
        stats['by_rule'][rule_id] += 1
        stats['by_file'][file_name] += 1
        stats['unique_rules'].add(rule_id)
        
        # Extract severity if available
        severity = v.get('Severity', 'UNKNOWN')
        stats['by_severity'][severity] += 1
    
    return violations, stats


def analyze_knowledge_base(kb_path):
    """Analyze knowledge base"""
    print(f"\n📚 ANALYZING KNOWLEDGE BASE: {kb_path}")
    print("=" * 70)
    
    kb = load_knowledge_base(kb_path)
    if not kb:
        return None, None
    
    violations_dict = kb.get('violations', {})
    
    stats = {
        'total_unique_violations': len(violations_dict),
        'total_occurrences': 0,
        'by_category': defaultdict(int),
        'by_status': defaultdict(int),
        'by_severity': defaultdict(int),
        'justified': 0,
        'fixed': 0,
        'open': 0,
        'unique_rules': set()
    }
    
    for violation_id, violation in violations_dict.items():
        category = violation.get('category', 'OTHER')
        occurrence_count = violation.get('occurrence_count', 1)
        severity = violation.get('severity', 'UNKNOWN')
        
        stats['total_occurrences'] += occurrence_count
        stats['by_category'][category] += occurrence_count
        stats['by_severity'][severity] += occurrence_count
        stats['unique_rules'].add(violation_id)
        
        # Status
        if violation.get('fix_applied'):
            stats['fixed'] += occurrence_count
        elif violation.get('justification_added'):
            stats['justified'] += occurrence_count
        else:
            stats['open'] += occurrence_count
    
    return violations_dict, stats


def compare_reports(parasoft_stats, kb_stats):
    """Compare Parasoft report with KB"""
    print(f"\n🔍 COMPARISON RESULTS")
    print("=" * 70)
    
    differences = []
    
    # Total violations
    print(f"\n📊 TOTAL VIOLATIONS:")
    print(f"   Parasoft Report: {parasoft_stats['total_violations']}")
    print(f"   Knowledge Base:  {kb_stats['total_occurrences']}")
    
    diff = abs(parasoft_stats['total_violations'] - kb_stats['total_occurrences'])
    if diff > 0:
        pct = (diff / parasoft_stats['total_violations']) * 100
        print(f"   ⚠️  Difference: {diff} violations ({pct:.1f}%)")
        differences.append(('Total', diff, pct))
    else:
        print(f"   ✅ Match!")
    
    # Category breakdown
    print(f"\n📑 BY CATEGORY:")
    for category in ['CERT', 'MISRA', 'OTHER']:
        parasoft_count = parasoft_stats['by_category'].get(category, 0)
        kb_count = kb_stats['by_category'].get(category, 0)
        
        print(f"\n   {category}:")
        print(f"      Parasoft: {parasoft_count}")
        print(f"      KB:       {kb_count}")
        
        diff = abs(parasoft_count - kb_count)
        if diff > 0:
            pct = (diff / parasoft_count * 100) if parasoft_count > 0 else 0
            print(f"      ⚠️  Difference: {diff} ({pct:.1f}%)")
            differences.append((category, diff, pct))
        else:
            print(f"      ✅ Match!")
    
    # Unique rules
    print(f"\n🎯 UNIQUE RULES:")
    print(f"   Parasoft Report: {len(parasoft_stats['unique_rules'])} unique")
    print(f"   Knowledge Base:  {kb_stats['total_unique_violations']} unique")
    
    diff = abs(len(parasoft_stats['unique_rules']) - kb_stats['total_unique_violations'])
    if diff > 0:
        print(f"   ⚠️  Difference: {diff} rules")
        differences.append(('Unique Rules', diff, 0))
    else:
        print(f"   ✅ Match!")
    
    # Status breakdown (KB only)
    print(f"\n📈 STATUS BREAKDOWN (KB):")
    print(f"   Open:      {kb_stats['open']}")
    print(f"   Justified: {kb_stats['justified']}")
    print(f"   Fixed:     {kb_stats['fixed']}")
    
    # Summary
    print(f"\n" + "=" * 70)
    if not differences:
        print(f"✅ PERFECT ALIGNMENT! Tool matches Parasoft report exactly.")
    else:
        print(f"⚠️  FOUND {len(differences)} DISCREPANCIES:")
        for area, diff, pct in differences:
            if pct > 0:
                print(f"   - {area}: {diff} violations off ({pct:.1f}%)")
            else:
                print(f"   - {area}: {diff} violations off")
        
        print(f"\n💡 POSSIBLE CAUSES:")
        print(f"   1. Parasoft report was regenerated after KB creation")
        print(f"   2. Some violations were filtered/ignored during parsing")
        print(f"   3. Report format differences (HTML vs XML)")
        print(f"   4. Violations were manually justified/fixed in KB")


def main():
    """Main verification routine"""
    if len(sys.argv) < 3:
        print("Usage: python verify_parasoft_alignment.py <parasoft_report.html> <module_name>")
        print("\nExample:")
        print("  python verify_parasoft_alignment.py parasoftReport/Mka.html Mka")
        sys.exit(1)
    
    report_path = Path(sys.argv[1])
    module_name = sys.argv[2]
    
    if not report_path.exists():
        print(f"[ERROR] Parasoft report not found: {report_path}")
        sys.exit(1)
    
    kb_path = Path("knowledge_base") / f"{module_name}_violations.json"
    if not kb_path.exists():
        print(f"[ERROR] Knowledge base not found: {kb_path}")
        print(f"\nGenerate KB first:")
        print(f"  python src/ParasoftAnalysisTool.py {report_path} Qorix_Deviations.xlsx {module_name}")
        sys.exit(1)
    
    print(f"\n{'='*70}")
    print(f"🔍 PARASOFT REPORT ALIGNMENT VERIFICATION")
    print(f"{'='*70}")
    print(f"\nModule: {module_name}")
    print(f"Report: {report_path}")
    print(f"KB:     {kb_path}")
    
    # Analyze both
    parasoft_violations, parasoft_stats = analyze_parasoft_report(report_path)
    kb_violations, kb_stats = analyze_knowledge_base(kb_path)
    
    if not kb_stats:
        print(f"\n[ERROR] Failed to load KB")
        sys.exit(1)
    
    # Compare
    compare_reports(parasoft_stats, kb_stats)
    
    # Additional analysis
    print(f"\n" + "=" * 70)
    print(f"📝 RECOMMENDATIONS:")
    print(f"=" * 70)
    
    if kb_stats['justified'] > 0:
        print(f"✅ {kb_stats['justified']} violations are justified")
        print(f"   Run CERT/MISRA report to verify they show up correctly")
    
    if kb_stats['fixed'] > 0:
        print(f"✅ {kb_stats['fixed']} violations have fixes applied")
    
    if kb_stats['open'] > 0:
        print(f"⚠️  {kb_stats['open']} violations still need attention")
    
    print(f"\n💡 NEXT STEPS:")
    print(f"   1. Generate CERT/MISRA report:")
    print(f"      python src/generate_cert_misra_report.py {module_name}")
    print(f"   2. Check that justified count shows correctly")
    print(f"   3. Compare numbers with original Parasoft report")
    
    print(f"\n{'='*70}")


if __name__ == "__main__":
    main()
