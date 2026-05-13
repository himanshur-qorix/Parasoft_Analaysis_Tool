"""
Violation Enricher
Enriches violations with actual source code context for AI-powered fixes

Takes violations with just file names and line numbers, finds the actual
source files, reads the code, and adds detailed context for AI analysis.

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 4.1.0
"""

import logging
from pathlib import Path
from typing import Dict, List, Optional

logger = logging.getLogger(__name__)


class ViolationEnricher:
    """
    Enriches violation data with actual source code context
    """
    
    def __init__(self, source_code_path: Optional[Path] = None):
        """
        Initialize ViolationEnricher
        
        Args:
            source_code_path: Optional path to source code directory
        """
        self.source_code_path = Path(source_code_path) if source_code_path else None
        self.file_cache = {}  # Cache for found file paths
        
        if self.source_code_path and self.source_code_path.exists():
            logger.info(f"Violation Enricher initialized with source path: {self.source_code_path}")
            self._build_file_index()
        else:
            logger.warning("Violation Enricher initialized without valid source path - cannot enrich violations")
    
    def _build_file_index(self):
        """Build index of all C/C++/H files in source directory"""
        if not self.source_code_path or not self.source_code_path.exists():
            return
        
        extensions = ['.c', '.cpp', '.h', '.hpp', '.cc', '.cxx']
        
        try:
            for ext in extensions:
                for file_path in self.source_code_path.rglob(f'*{ext}'):
                    if file_path.is_file():
                        filename = file_path.name
                        if filename not in self.file_cache:
                            self.file_cache[filename] = []
                        self.file_cache[filename].append(file_path)
            
            logger.info(f"Indexed {len(self.file_cache)} unique filenames from source directory")
        
        except Exception as e:
            logger.error(f"Error building file index: {e}")
    
    def enrich_violation(self, violation: Dict) -> Dict:
        """
        Enrich a single violation with source code context
        
        Args:
            violation: Violation dictionary (may have limited info)
        
        Returns:
            Enriched violation dictionary with code context
        """
        if not self.source_code_path or not self.source_code_path.exists():
            logger.debug(f"Cannot enrich violation {violation.get('violation_id')} - no source path")
            return violation
        
        # Check if violation already has good data
        if violation.get('code_snippet') and violation.get('code_snippet') != 'No code snippet available':
            if violation.get('file_path') and violation.get('file_path') not in ['UNKNOWN', 'N/A', '']:
                logger.debug(f"Violation {violation.get('violation_id')} already has good data")
                return violation
        
        # Extract file and line info
        files_affected = violation.get('files_affected', [])
        if not files_affected:
            logger.debug(f"No files_affected for {violation.get('violation_id')}")
            return violation
        
        # Parse first file entry
        file_entry = files_affected[0]
        file_name, line_number = self._parse_file_entry(file_entry)
        
        if not file_name or not line_number:
            logger.debug(f"Could not parse file/line from: {file_entry}")
            return violation
        
        # Find actual source file
        source_file = self._find_source_file(file_name)
        if not source_file:
            logger.debug(f"Source file not found: {file_name}")
            return violation
        
        # Read code context
        code_data = self._read_code_context(source_file, line_number)
        if not code_data:
            logger.debug(f"Could not read code from {source_file}:{line_number}")
            return violation
        
        # Enrich the violation
        violation['file_path'] = str(source_file)
        violation['file'] = file_name
        violation['line'] = line_number
        violation['code_snippet'] = code_data['target_line']
        violation['code_context'] = code_data['context']
        violation['code_context_lines'] = code_data['context_lines']
        violation['enriched'] = True
        
        logger.debug(f"Enriched violation {violation.get('violation_id')} with code context")
        return violation
    
    def enrich_violations(self, violations: List[Dict]) -> List[Dict]:
        """
        Enrich multiple violations with source code context
        
        Args:
            violations: List of violation dictionaries
        
        Returns:
            List of enriched violations
        """
        if not self.source_code_path or not self.source_code_path.exists():
            logger.warning("Cannot enrich violations - no source path configured")
            return violations
        
        enriched = []
        enriched_count = 0
        skipped_count = 0
        
        for violation in violations:
            enriched_violation = self.enrich_violation(violation)
            enriched.append(enriched_violation)
            
            if enriched_violation.get('enriched'):
                enriched_count += 1
            else:
                skipped_count += 1
        
        logger.info(f"Enrichment complete: {enriched_count} enriched, {skipped_count} skipped")
        return enriched
    
    def _parse_file_entry(self, file_entry) -> tuple:
        """
        Parse file entry to extract filename and line number
        
        Args:
            file_entry: Can be:
                - String like "file.c:123" or "file.c"
                - Dict like {'file': 'file.c', 'line': 123}
        
        Returns:
            Tuple of (filename, line_number) or (None, None)
        """
        try:
            # Handle dict format (from KB)
            if isinstance(file_entry, dict):
                file_name = file_entry.get('file', '')
                line_number = file_entry.get('line')
                
                if file_name:
                    # Extract just filename from path
                    file_name = Path(file_name).name
                    return file_name, line_number
                return None, None
            
            # Handle string format
            if isinstance(file_entry, str):
                if ':' in file_entry:
                    parts = file_entry.rsplit(':', 1)
                    file_name = parts[0].strip()
                    
                    # Handle Windows paths like "C:\path\file.c:123"
                    if len(parts[0]) == 1 and len(file_entry.split(':')) > 2:
                        # Likely "C:\path\file.c:123"
                        parts = file_entry.rsplit(':', 1)
                        file_name = parts[0].strip()
                    
                    # Extract just the filename from path
                    file_name = Path(file_name).name
                    
                    try:
                        line_number = int(parts[1].strip())
                        return file_name, line_number
                    except ValueError:
                        return file_name, None
                else:
                    # No line number, just filename
                    file_name = Path(file_entry.strip()).name
                    return file_name, None
            
            return None, None
        
        except Exception as e:
            logger.debug(f"Error parsing file entry '{file_entry}': {e}")
            return None, None
    
    def _find_source_file(self, filename: str) -> Optional[Path]:
        """
        Find actual source file path
        
        Args:
            filename: Just the filename (e.g., "Mka.c")
        
        Returns:
            Full path to file or None
        """
        # Check cache first
        if filename in self.file_cache:
            candidates = self.file_cache[filename]
            if len(candidates) == 1:
                return candidates[0]
            elif len(candidates) > 1:
                # Multiple files with same name - use first one
                # Could be enhanced to use heuristics
                logger.debug(f"Multiple files found for {filename}, using: {candidates[0]}")
                return candidates[0]
        
        # Not in cache, try direct search
        try:
            for file_path in self.source_code_path.rglob(filename):
                if file_path.is_file():
                    return file_path
        except Exception as e:
            logger.debug(f"Error searching for {filename}: {e}")
        
        return None
    
    def _read_code_context(self, file_path: Path, line_number: int, 
                          context_lines: int = 5) -> Optional[Dict]:
        """
        Read code context around a specific line
        
        Args:
            file_path: Path to source file
            line_number: Line number (1-indexed)
            context_lines: Number of lines before/after to include
        
        Returns:
            Dictionary with code context or None
        """
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                all_lines = f.readlines()
            
            total_lines = len(all_lines)
            
            # Validate line number
            if line_number < 1 or line_number > total_lines:
                logger.debug(f"Line {line_number} out of range for {file_path} ({total_lines} lines)")
                return None
            
            # Calculate context range
            start_line = max(0, line_number - context_lines - 1)
            end_line = min(total_lines, line_number + context_lines)
            
            # Extract context
            context_block = all_lines[start_line:end_line]
            context_string = ''.join(context_block)
            
            # Extract target line
            target_line = all_lines[line_number - 1].strip()
            
            return {
                'target_line': target_line,
                'context': context_string,
                'context_lines': {
                    'start': start_line + 1,
                    'end': end_line,
                    'violation': line_number
                }
            }
        
        except Exception as e:
            logger.error(f"Error reading code from {file_path}:{line_number}: {e}")
            return None


def enrich_knowledge_base_violations(kb_path: Path, source_code_path: Path) -> int:
    """
    Enrich all violations in a knowledge base with source code context
    
    Args:
        kb_path: Path to knowledge base JSON file
        source_code_path: Path to source code directory
    
    Returns:
        Number of violations enriched
    """
    import json
    
    try:
        # Load knowledge base
        with open(kb_path, 'r', encoding='utf-8') as f:
            kb_data = json.load(f)
        
        violations = kb_data.get('violations', {})
        if not violations:
            logger.warning(f"No violations found in {kb_path}")
            return 0
        
        # Initialize enricher
        enricher = ViolationEnricher(source_code_path)
        
        # Enrich each violation
        enriched_count = 0
        for vid, vdata in violations.items():
            enriched = enricher.enrich_violation(vdata)
            if enriched.get('enriched'):
                violations[vid] = enriched
                enriched_count += 1
        
        # Save enriched knowledge base
        with open(kb_path, 'w', encoding='utf-8') as f:
            json.dump(kb_data, f, indent=2)
        
        logger.info(f"Enriched {enriched_count} violations in {kb_path}")
        return enriched_count
    
    except Exception as e:
        logger.error(f"Error enriching knowledge base {kb_path}: {e}")
        return 0


def main():
    """Command-line tool to enrich knowledge base violations"""
    import argparse
    import sys
    
    logging.basicConfig(
        level=logging.INFO,
        format='[%(levelname)s] %(message)s'
    )
    
    parser = argparse.ArgumentParser(
        description='Enrich violation data with source code context',
        epilog="""
Examples:
  # Enrich violations in Mka knowledge base
  python src/ViolationEnricher.py --kb knowledge_base/Mka_KnowledgeDatabase.json --source "D:/Source/Mka"
  
  # Enrich all knowledge bases in directory
  python src/ViolationEnricher.py --kb-dir knowledge_base --source "D:/Source"
        """
    )
    
    parser.add_argument('--kb', type=str, help='Path to knowledge base JSON file')
    parser.add_argument('--kb-dir', type=str, help='Directory containing knowledge base files')
    parser.add_argument('--source', type=str, required=True, help='Path to source code directory')
    
    args = parser.parse_args()
    
    if not args.kb and not args.kb_dir:
        print("ERROR: Must provide either --kb or --kb-dir")
        sys.exit(1)
    
    source_path = Path(args.source)
    if not source_path.exists():
        print(f"ERROR: Source path does not exist: {source_path}")
        sys.exit(1)
    
    total_enriched = 0
    
    if args.kb:
        # Enrich single KB file
        kb_path = Path(args.kb)
        if not kb_path.exists():
            print(f"ERROR: Knowledge base not found: {kb_path}")
            sys.exit(1)
        
        count = enrich_knowledge_base_violations(kb_path, source_path)
        total_enriched += count
        print(f"\n✅ Enriched {count} violations in {kb_path.name}")
    
    elif args.kb_dir:
        # Enrich all KB files in directory
        kb_dir = Path(args.kb_dir)
        if not kb_dir.exists():
            print(f"ERROR: Knowledge base directory not found: {kb_dir}")
            sys.exit(1)
        
        kb_files = list(kb_dir.glob('*_KnowledgeDatabase.json'))
        if not kb_files:
            print(f"WARNING: No knowledge base files found in {kb_dir}")
            sys.exit(0)
        
        print(f"\nEnriching {len(kb_files)} knowledge base files...")
        print("="*80)
        
        for kb_path in kb_files:
            print(f"\nProcessing: {kb_path.name}")
            count = enrich_knowledge_base_violations(kb_path, source_path)
            total_enriched += count
            print(f"  ✅ Enriched {count} violations")
        
        print("\n" + "="*80)
        print(f"Total violations enriched: {total_enriched}")
    
    print("\n✅ Enrichment complete!")
    print("\nNext step: Run Apply_Fixes.bat to apply AI-powered fixes")
    return 0


if __name__ == '__main__':
    sys.exit(main())
