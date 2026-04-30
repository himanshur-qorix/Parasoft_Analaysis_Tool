"""
Justification Format Learner
Analyzes existing source files to learn justification patterns and formats

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 1.0.0

Purpose:
- Extract justification formats from existing source files (like Tls.c pattern)
- Learn header section format and inline reference patterns
- Provide AI-generated justifications following discovered formats
"""

import sys
import re
import json
import logging
from pathlib import Path
from typing import Dict, List, Optional, Tuple
from dataclasses import dataclass, asdict
from collections import defaultdict
from datetime import datetime

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

logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s'
)
logger = logging.getLogger(__name__)

# Try importing ollama
try:
    import ollama
    OLLAMA_AVAILABLE = True
except ImportError:
    OLLAMA_AVAILABLE = False
    logger.warning("Ollama not available - will use rule-based justifications only")


@dataclass
class JustificationReference:
    """Represents a justification reference (e.g., Tls_c_REF_1)"""
    ref_id: str  # e.g., "Tls_c_REF_1"
    rule_ids: List[str]  # e.g., ["CERT_C-DCL00-b-3"]
    justification_text: str  # The actual justification reason
    module_prefix: str  # e.g., "Tls_c"
    

@dataclass
class SourceFileFormat:
    """Represents the justification format discovered in a source file"""
    file_path: str
    has_header_section: bool
    header_section_format: Optional[str]  # Template for header section
    inline_reference_format: Optional[str]  # Template for inline references
    references: List[JustificationReference]  # Extracted references
    separator_pattern: str  # e.g., "="*80 or "*"*80


class JustificationFormatLearner:
    """Learns justification formats from existing source files"""
    
    def __init__(self, source_dirs: List[Path] = None, use_ai: bool = True):
        """
        Initialize the format learner
        
        Args:
            source_dirs: List of directories to scan for source files
            use_ai: Whether to use Ollama for AI-enhanced justifications
        """
        self.source_dirs = source_dirs or []
        self.use_ai = use_ai and OLLAMA_AVAILABLE
        
        # Discovered formats
        self.discovered_formats: List[SourceFileFormat] = []
        self.reference_patterns: Dict[str, List[JustificationReference]] = defaultdict(list)
        self.common_format: Optional[str] = None
        
        # AI model configuration
        self.ai_model = "qwen2.5-coder:1.5b-base"
        
        logger.info(f"Justification Format Learner initialized")
        logger.info(f"AI Mode: {'ENABLED' if self.use_ai else 'DISABLED'}")
        
    def scan_source_files(self, file_patterns: List[str] = None) -> int:
        """
        Scan source files to discover justification formats
        
        Args:
            file_patterns: File patterns to match (e.g., ['*.c', '*.h'])
        
        Returns:
            Number of files scanned
        """
        if file_patterns is None:
            file_patterns = ['*.c', '*.cpp', '*.h', '*.hpp']
        
        files_scanned = 0
        
        for source_dir in self.source_dirs:
            if not source_dir.exists():
                continue
                
            for pattern in file_patterns:
                source_files = list(source_dir.rglob(pattern))
                
                for source_file in source_files:
                    try:
                        format_info = self._extract_format_from_file(source_file)
                        if format_info and format_info.references:
                            self.discovered_formats.append(format_info)
                            
                            # Store references by module prefix
                            for ref in format_info.references:
                                self.reference_patterns[ref.module_prefix].append(ref)
                            
                            files_scanned += 1
                            logger.info(f"✓ Discovered format in: {source_file.name} "
                                       f"({len(format_info.references)} references)")
                    
                    except Exception as e:
                        logger.debug(f"Error analyzing {source_file}: {e}")
        
        if files_scanned > 0:
            self._determine_common_format()
            logger.info(f"\n✓ Total files with justification formats: {files_scanned}")
            logger.info(f"✓ Total unique references found: {sum(len(refs) for refs in self.reference_patterns.values())}")
        else:
            logger.warning("No justification formats discovered in source files")
        
        return files_scanned
    
    def _extract_format_from_file(self, file_path: Path) -> Optional[SourceFileFormat]:
        """
        Extract justification format from a source file
        
        Args:
            file_path: Path to source file
        
        Returns:
            SourceFileFormat object or None
        """
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
        except:
            return None
        
        # Check for header section (Parasoft violations Section)
        header_pattern = r'/\*{5,}[^*]*Parasoft violations Section[^*]*\*{5,}/\s*(.*?)(?=/\*{5,}|\Z)'
        header_match = re.search(header_pattern, content, re.DOTALL)
        
        has_header = header_match is not None
        header_content = header_match.group(1) if header_match else None
        
        # Extract references from header section
        references = []
        if header_content:
            references = self._parse_header_section(header_content, file_path.stem)
        
        # Find inline reference pattern
        inline_pattern = r'//\s*parasoft-suppress\s+([A-Z0-9_\-]+)\s+"Reason:\s*([^"]+)"'
        inline_matches = re.findall(inline_pattern, content)
        
        # Determine separator pattern
        separator_pattern = "*" * 80 if "*" * 80 in content else "=" * 80
        
        # Build format templates
        header_section_format = self._build_header_template() if has_header else None
        inline_reference_format = '// parasoft-suppress {rule_id} "Reason: {ref_id}."' if inline_matches else None
        
        # Return format info if any patterns found
        if references or inline_matches:
            return SourceFileFormat(
                file_path=str(file_path),
                has_header_section=has_header,
                header_section_format=header_section_format,
                inline_reference_format=inline_reference_format,
                references=references,
                separator_pattern=separator_pattern
            )
        
        return None
    
    def _parse_header_section(self, header_content: str, module_prefix: str) -> List[JustificationReference]:
        """
        Parse header section to extract justification references
        
        Args:
            header_content: Content of the header section
            module_prefix: Module prefix for reference IDs
        
        Returns:
            List of JustificationReference objects
        """
        references = []
        
        # Pattern to match individual reference sections
        # Example: #section Tls_c_REF_1
        #          Violates CERT_C-DCL00-b-3: ...
        #          Justification: ...
        ref_pattern = r'#section\s+([A-Za-z0-9_\-]+)\s+\* Violates\s+([A-Z0-9_\-,\s]+):[^\n]*\n\* Justification:\s*(.+?)(?=\*\s+#section|\*+/|\Z)'
        
        matches = re.findall(ref_pattern, header_content, re.DOTALL)
        
        for ref_id, rule_ids_str, justification in matches:
            # Parse multiple rule IDs
            rule_ids = [rid.strip() for rid in re.split(r'[,\s]+', rule_ids_str) if rid.strip()]
            
            # Clean up justification text
            justification = re.sub(r'\n\*\s*', ' ', justification).strip()
            
            ref = JustificationReference(
                ref_id=ref_id,
                rule_ids=rule_ids,
                justification_text=justification,
                module_prefix=module_prefix
            )
            references.append(ref)
        
        return references
    
    def _build_header_template(self) -> str:
        """Build a template for header section"""
        return """/*******************************************************************************
**                      Parasoft violations Section                           **
*******************************************************************************/

/*
* #section {module}_REF_{ref_num}
* Violates {rule_id}: {violation_description}
* Justification: {justification_text}
*/"""
    
    def _determine_common_format(self):
        """Determine the most common format pattern"""
        if not self.discovered_formats:
            return
        
        # Count format types
        header_count = sum(1 for fmt in self.discovered_formats if fmt.has_header_section)
        total_count = len(self.discovered_formats)
        
        if header_count / total_count > 0.5:
            self.common_format = "header_with_inline_references"
            logger.info(f"✓ Common format: Header section + inline references ({header_count}/{total_count} files)")
        else:
            self.common_format = "inline_only"
            logger.info(f"✓ Common format: Inline references only ({total_count - header_count}/{total_count} files)")
    
    def generate_justification(self, violation: Dict, module_name: str) -> Dict[str, str]:
        """
        Generate justification using discovered format and AI
        
        Args:
            violation: Violation dictionary with violation_text, code_snippet, etc.
            module_name: Module name for reference generation (e.g., "Tls", "EthIf", "Mka")
        
        Returns:
            Dictionary with 'header_entry', 'inline_comment', 'ref_id', and 'justification_text' keys
            
        Note:
            Reference format follows pattern: {ModuleName}_c_REF_{number}
            Example: Tls → Tls_c_REF_1, EthIf → EthIf_c_REF_1
        """
        check_id = violation.get('check_id', violation.get('violation_id', 'UNKNOWN'))
        violation_text = violation.get('violation_text', '')
        code_snippet = violation.get('code_snippet', '')
        
        # Generate AI-powered justification text if available
        if self.use_ai:
            justification_text = self._generate_ai_justification(
                check_id, violation_text, code_snippet
            )
        else:
            justification_text = self._generate_rule_based_justification(
                check_id, violation_text
            )
        
        # Construct module-specific reference prefix
        # Format: {ModuleName}_c (e.g., "Tls_c", "EthIf_c", "Mka_c")
        # Avoid double "_c" suffix if already present
        if module_name.endswith('_c'):
            module_prefix = module_name
        else:
            module_prefix = f"{module_name}_c"
        
        # Find next available reference number for this module
        existing_refs = self.reference_patterns.get(module_prefix, [])
        next_ref_num = len(existing_refs) + 1
        ref_id = f"{module_prefix}_REF_{next_ref_num}"
        
        logger.debug(f"Generating justification: Module={module_name}, Prefix={module_prefix}, RefID={ref_id}")
        
        # Generate header entry following discovered format
        header_entry = f"""/*
* #section {ref_id}
* Violates {check_id}: {violation_text[:80]}{"..." if len(violation_text) > 80 else ""}
* Justification: {justification_text}
*/"""
        
        # Generate inline comment with module-specific reference
        inline_comment = f'// parasoft-suppress {check_id} "Reason: {ref_id}."'
        
        # Store this reference for future reference number tracking
        new_ref = JustificationReference(
            ref_id=ref_id,
            rule_ids=[check_id],
            justification_text=justification_text,
            module_prefix=module_prefix
        )
        self.reference_patterns[module_prefix].append(new_ref)
        
        return {
            'header_entry': header_entry,
            'inline_comment': inline_comment,
            'ref_id': ref_id,
            'justification_text': justification_text,
            'module_name': module_name,
            'module_prefix': module_prefix
        }
        inline_comment = f'// parasoft-suppress {check_id} "Reason: {ref_id}."'
        
        return {
            'header_entry': header_entry,
            'inline_comment': inline_comment,
            'ref_id': ref_id,
            'justification_text': justification_text
        }
    
    def _generate_ai_justification(self, check_id: str, violation_text: str, 
                                   code_snippet: str) -> str:
        """
        Generate AI-powered justification using Ollama
        
        Args:
            check_id: Rule ID
            violation_text: Violation description
            code_snippet: Code that violates the rule
        
        Returns:
            Justification text
        """
        # Look for similar patterns in learned references
        similar_justifications = self._find_similar_justifications(check_id)
        
        prompt = f"""You are a code quality expert analyzing MISRA/CERT violations.

**Violation Details:**
- Rule ID: {check_id}
- Description: {violation_text}
- Code: {code_snippet[:200]}

**Similar existing justifications for this rule:**
{chr(10).join('- ' + j for j in similar_justifications[:3]) if similar_justifications else '(none found)'}

**Task:** Generate a concise, professional justification (1-2 sentences) explaining why this violation cannot be fixed.
Focus on: architecture constraints, AUTOSAR standards, platform limitations, or performance requirements.

**Format:** Single paragraph, no introduction, just the justification.
"""
        
        try:
            response = ollama.generate(
                model=self.ai_model,
                prompt=prompt
            )
            
            justification = response['response'].strip()
            
            # Clean up response
            justification = justification.replace('\n', ' ')
            if len(justification) > 300:
                justification = justification[:297] + "..."
            
            return justification
            
        except Exception as e:
            logger.warning(f"AI generation failed: {e}, using rule-based fallback")
            return self._generate_rule_based_justification(check_id, violation_text)
    
    def _find_similar_justifications(self, check_id: str) -> List[str]:
        """Find similar justifications for the same rule"""
        similar = []
        
        for refs in self.reference_patterns.values():
            for ref in refs:
                if check_id in ref.rule_ids:
                    similar.append(ref.justification_text)
        
        return similar[:5]  # Return top 5
    
    def _generate_rule_based_justification(self, check_id: str, violation_text: str) -> str:
        """
        Generate rule-based justification
        
        Args:
            check_id: Rule ID
            violation_text: Violation description
        
        Returns:
            Justification text
        """
        violation_upper = violation_text.upper()
        
        # AUTOSAR-specific patterns
        if 'CONST' in violation_upper and 'PARAMETER' in violation_upper:
            return "Standard AUTOSAR Interface provided cannot be tailored according to the violation."
        
        elif 'MACRO' in violation_upper or '#DEFINE' in violation_upper:
            return "Macros are defined by AUTOSAR standards and internal specifications; name length cannot be restricted."
        
        elif '#INCLUDE' in violation_upper or 'DIRECTIVE' in violation_upper:
            return "Macros are defined by AUTOSAR standards and internal specifications."
        
        elif 'VARIABLE' in violation_upper and 'DECLARED' in violation_upper:
            return "Coding guidelines suggest variables are declared at the start of the function."
        
        elif 'PARAMETER' in violation_upper and 'VALIDATED' in violation_upper:
            return "This parameter originates from configuration or is validated in main API; additional validation not required."
        
        # CERT-specific patterns
        elif 'CERT' in check_id:
            if 'DCL' in check_id:
                return "Declaration follows AUTOSAR interface specification which cannot be modified."
            elif 'STR' in check_id:
                return "String operation verified safe within module constraints; alternative approach not feasible."
            elif 'MEM' in check_id:
                return "Memory management follows AUTOSAR patterns; error handling at higher layer."
        
        # MISRA-specific patterns
        elif 'MISRA' in check_id:
            if 'RULE_5' in check_id or 'RULE_8' in check_id:
                return "Identifier naming follows project standards and AUTOSAR specifications."
            elif 'RULE_10' in check_id or 'RULE_11' in check_id:
                return "Type conversion necessary for hardware register access; manually verified safe."
            elif 'RULE_20' in check_id or 'RULE_21' in check_id:
                return "Follows AUTOSAR configuration and module architecture requirements."
        
        # Generic fallback
        return "Technical constraint: modification would require significant architectural changes and affect AUTOSAR compliance."
    
    def export_learned_formats(self, output_file: Path):
        """
        Export learned formats to JSON file
        
        Args:
            output_file: Path to output JSON file
        """
        export_data = {
            'common_format': self.common_format,
            'discovered_formats': [
                {
                    'file': fmt.file_path,
                    'has_header': fmt.has_header_section,
                    'references_count': len(fmt.references)
                }
                for fmt in self.discovered_formats
            ],
            'reference_patterns': {
                module: [asdict(ref) for ref in refs]
                for module, refs in self.reference_patterns.items()
            },
            'total_files_scanned': len(self.discovered_formats),
            'total_references': sum(len(refs) for refs in self.reference_patterns.values()),
            'timestamp': datetime.now().isoformat()
        }
        
        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(export_data, f, indent=2, ensure_ascii=False)
        
        logger.info(f"✓ Learned formats exported to: {output_file}")


if __name__ == '__main__':
    # Test/demo
    learner = JustificationFormatLearner(
        source_dirs=[Path('Inputs')],
        use_ai=True
    )
    
    files_scanned = learner.scan_source_files()
    
    if files_scanned > 0:
        # Export learned formats
        output_file = Path('knowledge_base/learned_justification_formats.json')
        output_file.parent.mkdir(exist_ok=True)
        learner.export_learned_formats(output_file)
        
        print("\n" + "=" * 80)
        print("LEARNED FORMATS SUMMARY")
        print("=" * 80)
        print(f"Common format: {learner.common_format}")
        print(f"Total patterns: {sum(len(refs) for refs in learner.reference_patterns.values())}")
        print("=" * 80)
