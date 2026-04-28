"""
Diagnostic Tool for Justification Pattern Learning
Helps identify why reports are empty/showing zeros
"""

import sys
from pathlib import Path
import json

def check_ollama():
    """Check if Ollama is available and working"""
    print("\n" + "="*80)
    print("🔍 CHECKING OLLAMA")
    print("="*80)
    
    try:
        import ollama
        print("✅ Ollama Python package: INSTALLED")
        
        try:
            response = ollama.list()
            print("✅ Ollama server: RUNNING")
            
            # Check for models
            models = response.get('models', []) if isinstance(response, dict) else []
            if models:
                print(f"✅ Models found: {len(models)}")
                for model in models[:5]:
                    model_name = model.get('name', str(model))
                    print(f"   • {model_name}")
            else:
                print("⚠️  No models found!")
                print("   Run: ollama pull qwen2.5-coder:1.5b-base")
                return False
                
            return True
            
        except Exception as e:
            print(f"❌ Ollama server: NOT RUNNING")
            print(f"   Error: {e}")
            print("   Solution: Run 'ollama serve' in a separate terminal")
            return False
            
    except ImportError:
        print("❌ Ollama Python package: NOT INSTALLED")
        print("   Solution: pip install ollama")
        return False

def check_justifications_folder():
    """Check justifications folder for files"""
    print("\n" + "="*80)
    print("📁 CHECKING JUSTIFICATIONS FOLDER")
    print("="*80)
    
    justifications_dir = Path('justifications')
    
    if not justifications_dir.exists():
        print(f"❌ Directory NOT FOUND: {justifications_dir}")
        print("   Solution: Create the directory or it may be in a different location")
        return 0
    
    print(f"✅ Directory exists: {justifications_dir}")
    
    # Look for suppression comment files
    suppress_files = list(justifications_dir.glob('*_suppress_comments_*.txt'))
    
    if not suppress_files:
        print("❌ NO suppression comment files found!")
        print("   Expected pattern: *_suppress_comments_*.txt")
        print("   Solution: Run a justification batch file first to generate these files")
        return 0
    
    print(f"✅ Found {len(suppress_files)} suppression comment files:")
    total_lines = 0
    for file in suppress_files[:10]:  # Show first 10
        line_count = len(file.read_text(encoding='utf-8', errors='ignore').splitlines())
        total_lines += line_count
        print(f"   • {file.name} ({line_count} lines)")
    
    if len(suppress_files) > 10:
        print(f"   ... and {len(suppress_files) - 10} more files")
    
    return len(suppress_files)

def check_learning_folder():
    """Check learning folder for source files"""
    print("\n" + "="*80)
    print("📚 CHECKING LEARNING FOLDER")
    print("="*80)
    
    learning_dir = Path('learning')
    
    if not learning_dir.exists():
        print(f"❌ Directory NOT FOUND: {learning_dir}")
        print("   Solution: Create learning/ folder structure")
        return 0
    
    print(f"✅ Directory exists: {learning_dir}")
    
    # Look for source files recursively
    c_files = list(learning_dir.rglob('*.c'))
    h_files = list(learning_dir.rglob('*.h'))
    total_files = len(c_files) + len(h_files)
    
    if total_files == 0:
        print("❌ NO source files found!")
        print("   Expected: .c and .h files in learning/ folder")
        print("   Solution: Add source files to learning/ModuleName/ConfigName/source/")
        return 0
    
    print(f"✅ Found {total_files} source files:")
    print(f"   • {len(c_files)} .c files")
    print(f"   • {len(h_files)} .h files")
    
    # Check for actual suppressions in source files
    suppressions_found = 0
    files_with_suppressions = []
    
    print("\n   Checking for embedded suppressions...")
    for source_file in (c_files + h_files)[:20]:  # Check first 20
        try:
            content = source_file.read_text(encoding='utf-8', errors='ignore')
            if 'parasoft-suppress' in content or 'parasoft-begin-suppress' in content:
                suppressions_found += content.count('parasoft-suppress')
                files_with_suppressions.append(source_file.name)
        except:
            pass
    
    if suppressions_found > 0:
        print(f"   ✅ Found ~{suppressions_found} suppressions in {len(files_with_suppressions)} files")
        for fname in files_with_suppressions[:5]:
            print(f"      • {fname}")
    else:
        print("   ⚠️  No parasoft-suppress comments found in source files!")
        print("   Note: Source files must contain parasoft-suppress comments to analyze")
    
    return total_files

def check_previous_reports():
    """Check if previous reports exist"""
    print("\n" + "="*80)
    print("📄 CHECKING PREVIOUS REPORTS")
    print("="*80)
    
    kb_dir = Path('knowledge_base')
    reports_dir = Path('reports')
    
    # Check JSON report
    json_report = kb_dir / 'justification_patterns_report.json'
    if json_report.exists():
        print(f"✅ JSON report exists: {json_report}")
        try:
            data = json.loads(json_report.read_text(encoding='utf-8'))
            summary = data.get('summary', {})
            print(f"   Total justifications: {summary.get('total_justifications', 0)}")
            print(f"   Total modules: {summary.get('total_modules', 0)}")
            print(f"   Unique rules: {summary.get('unique_rules', 0)}")
        except:
            print("   ⚠️  Could not parse JSON")
    else:
        print(f"❌ JSON report NOT FOUND: {json_report}")
    
    # Check HTML report
    html_report = reports_dir / 'justification_quality_report.html'
    if html_report.exists():
        print(f"✅ HTML report exists: {html_report}")
        size = html_report.stat().st_size
        print(f"   Size: {size:,} bytes")
    else:
        print(f"❌ HTML report NOT FOUND: {html_report}")

def main():
    print("\n" + "="*80)
    print("🔬 DIAGNOSTIC TOOL FOR JUSTIFICATION PATTERN LEARNING")
    print("="*80)
    print("This tool will help identify why your report is empty\n")
    
    # Run all checks
    ollama_ok = check_ollama()
    justifications_count = check_justifications_folder()
    learning_count = check_learning_folder()
    check_previous_reports()
    
    # Summary and recommendations
    print("\n" + "="*80)
    print("📋 DIAGNOSIS SUMMARY")
    print("="*80)
    
    total_sources = justifications_count + learning_count
    
    if total_sources == 0:
        print("\n❌ PROBLEM FOUND: NO FILES TO ANALYZE!")
        print("\n🔧 SOLUTIONS:")
        print("   1. Run a justification batch file to generate suppression comments")
        print("   2. Add source files to learning/ folder structure:")
        print("      learning/ModuleName/ConfigName/source/*.c")
        print("      learning/ModuleName/ConfigName/source/*.h")
        print("   3. Make sure source files contain parasoft-suppress comments")
        print("\nExample:")
        print("   learning/")
        print("   └── Mka/")
        print("       └── Mka_Config_1/")
        print("           └── source/")
        print("               ├── Mka_Main.c   (with parasoft-suppress comments)")
        print("               └── Mka_Types.h  (with parasoft-suppress comments)")
        
    elif not ollama_ok:
        print("\n⚠️  WARNING: OLLAMA NOT AVAILABLE")
        print("\n💡 RECOMMENDATION:")
        print("   Running in rule-based mode (no AI)")
        print("   For AI analysis:")
        print("   1. pip install ollama")
        print("   2. ollama serve (in separate terminal)")
        print("   3. ollama pull qwen2.5-coder:1.5b-base")
        print("   4. python src\\learn_justification_patterns.py --ai")
        
    else:
        print("\n✅ EVERYTHING LOOKS GOOD!")
        print("\n🚀 RUN THE ANALYSIS:")
        print("   python src\\learn_justification_patterns.py --ai")
        print("\n📊 THEN VIEW REPORT:")
        print("   start reports\\justification_quality_report.html")
    
    print("\n" + "="*80)
    print()

if __name__ == "__main__":
    main()
