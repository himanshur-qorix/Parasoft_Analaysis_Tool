"""
Parasoft Rules Documentation Parser
Extracts rule details, examples, and repair strategies from Parasoft HTML docs
Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 1.0.0
"""

import re
from pathlib import Path
from typing import Dict, Optional, List
from bs4 import BeautifulSoup
import json
import logging

logger = logging.getLogger(__name__)


class ParasoftRule:
    """Represents a single Parasoft rule with all its details"""
    
    def __init__(self, rule_id: str):
        self.rule_id = rule_id
        self.title = ""
        self.description = ""
        self.security_relevance = ""
        self.benefits = ""
        self.example_violation = ""
        self.example_repair = ""
        self.references = []
        self.parameters = ""
        self.cwe_mappings = []
        self.standard = self._determine_standard(rule_id)
    
    def _determine_standard(self, rule_id: str) -> str:
        """Determine which standard this rule belongs to"""
        if "CERT_C" in rule_id or "CERT-C" in rule_id:
            return "CERT-C"
        elif "MISRAC2012" in rule_id:
            return "MISRA-C:2012"
        elif "CDD" in rule_id:
            return "CDD"
        elif "BD" in rule_id:
            return "BD"
        else:
            return "OTHER"
    
    def to_dict(self) -> Dict:
        """Convert to dictionary"""
        return {
            'rule_id': self.rule_id,
            'title': self.title,
            'standard': self.standard,
            'description': self.description,
            'security_relevance': self.security_relevance,
            'benefits': self.benefits,
            'example_violation': self.example_violation,
            'example_repair': self.example_repair,
            'parameters': self.parameters,
            'cwe_mappings': self.cwe_mappings,
            'references': self.references
        }


class ParasoftRulesParser:
    """Parser for Parasoft rule documentation HTML files"""
    
    def __init__(self, rules_dir: Path):
        """
        Initialize parser
        
        Args:
            rules_dir: Path to the gendoc directory containing HTML files
        """
        self.rules_dir = Path(rules_dir)
        self.rules_cache: Dict[str, ParasoftRule] = {}
        self.index: Dict[str, List[str]] = {
            'CERT-C': [],
            'MISRA-C:2012': [],
            'CDD': [],
            'BD': [],
            'OTHER': [],
            'ALL': []
        }
    
    def parse_rule_file(self, html_file: Path) -> Optional[ParasoftRule]:
        """
        Parse a single Parasoft rule HTML file
        
        Args:
            html_file: Path to HTML file
        
        Returns:
            ParasoftRule object or None if parsing fails
        """
        try:
            with open(html_file, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
            
            soup = BeautifulSoup(content, 'html.parser')
            
            # Extract rule ID from filename or title
            rule_id = html_file.stem
            
            # Create rule object
            rule = ParasoftRule(rule_id)
            
            # Extract title from <TITLE> tag
            title_tag = soup.find('title')
            if title_tag:
                rule.title = title_tag.get_text().strip()
            
            # Extract sections
            sections = self._extract_sections(content)
            
            rule.description = sections.get('DESCRIPTION', '').strip()
            rule.security_relevance = sections.get('SECURITY RELEVANCE', '').strip()
            rule.benefits = sections.get('BENEFITS', '').strip()
            rule.example_violation = sections.get('EXAMPLE', '').strip()
            rule.example_repair = sections.get('REPAIR', '').strip()
            rule.parameters = sections.get('PARAMETERS', '').strip()
            rule.references = self._extract_references(sections.get('REFERENCES', ''))
            rule.cwe_mappings = self._extract_cwe_mappings(sections.get('REFERENCES', ''))
            
            return rule
            
        except Exception as e:
            logger.warning(f"Failed to parse {html_file}: {e}")
            return None
    
    def _extract_sections(self, html_content: str) -> Dict[str, str]:
        """Extract sections from HTML content"""
        sections = {}
        
        # Pattern to match sections: <STRONG>SECTION_NAME</STRONG><PRE>content</PRE>
        pattern = r'<STRONG>\s*([A-Z\s]+)\s*</STRONG>\s*<PRE>(.*?)</PRE>'
        
        matches = re.findall(pattern, html_content, re.DOTALL | re.IGNORECASE)
        
        for section_name, section_content in matches:
            section_name = section_name.strip()
            # Clean HTML entities and tags
            content = re.sub(r'<[^>]+>', '', section_content)
            content = content.replace('&lt;', '<').replace('&gt;', '>')
            content = content.replace('&amp;', '&').replace('&quot;', '"')
            sections[section_name] = content.strip()
        
        return sections
    
    def _extract_references(self, references_text: str) -> List[str]:
        """Extract reference list from references section"""
        if not references_text:
            return []
        
        refs = []
        lines = references_text.split('\n')
        for line in lines:
            line = line.strip()
            if line and not line.startswith('http'):
                refs.append(line)
        
        return refs
    
    def _extract_cwe_mappings(self, references_text: str) -> List[str]:
        """Extract CWE IDs from references"""
        if not references_text:
            return []
        
        cwe_pattern = r'CWE-(\d+)'
        matches = re.findall(cwe_pattern, references_text)
        
        return [f"CWE-{cwe}" for cwe in matches]
    
    def load_all_rules(self) -> int:
        """
        Load all rule HTML files from the directory
        
        Returns:
            Number of rules loaded
        """
        if not self.rules_dir.exists():
            logger.error(f"Rules directory not found: {self.rules_dir}")
            return 0
        
        html_files = list(self.rules_dir.glob("*.html"))
        
        # Exclude index.html and category files
        html_files = [f for f in html_files if not f.stem.endswith('.html') and 
                      f.stem not in ['index', 'CERT_C', 'MISRAC2012', 'BD', 'CDD']]
        
        logger.info(f"Loading {len(html_files)} rule files...")
        
        loaded_count = 0
        for html_file in html_files:
            rule = self.parse_rule_file(html_file)
            if rule:
                self.rules_cache[rule.rule_id] = rule
                self.index[rule.standard].append(rule.rule_id)
                self.index['ALL'].append(rule.rule_id)
                loaded_count += 1
        
        logger.info(f"Loaded {loaded_count} rules successfully")
        return loaded_count
    
    def get_rule(self, rule_id: str) -> Optional[ParasoftRule]:
        """
        Get rule by ID
        
        Args:
            rule_id: Rule identifier (e.g., "CERT_C-STR31-a" or "MISRAC2012-RULE_8_7-a")
        
        Returns:
            ParasoftRule object or None
        """
        # Try exact match first
        if rule_id in self.rules_cache:
            return self.rules_cache[rule_id]
        
        # Try with variations (underscores vs hyphens)
        rule_id_alt = rule_id.replace('-', '_').replace('_', '-')
        if rule_id_alt in self.rules_cache:
            return self.rules_cache[rule_id_alt]
        
        # Try partial match (base rule without sub-rule)
        for cached_id, rule in self.rules_cache.items():
            if cached_id.startswith(rule_id) or rule_id.startswith(cached_id):
                return rule
        
        return None
    
    def get_fix_suggestion(self, rule_id: str) -> Optional[str]:
        """
        Get fix suggestion (repair example) for a rule
        
        Args:
            rule_id: Rule identifier
        
        Returns:
            Fix suggestion text or None
        """
        rule = self.get_rule(rule_id)
        if rule and rule.example_repair:
            return rule.example_repair
        return None
    
    def get_justification_rationale(self, rule_id: str) -> Optional[str]:
        """
        Get justification rationale (security relevance + benefits)
        
        Args:
            rule_id: Rule identifier
        
        Returns:
            Rationale text or None
        """
        rule = self.get_rule(rule_id)
        if rule:
            rationale = ""
            if rule.security_relevance:
                rationale += f"Security: {rule.security_relevance}\n\n"
            if rule.benefits:
                rationale += f"Benefits: {rule.benefits}\n"
            return rationale.strip()
        return None
    
    def export_rules_database(self, output_path: Path):
        """
        Export all parsed rules to JSON file
        
        Args:
            output_path: Path to save JSON file
        """
        rules_dict = {
            'total_rules': len(self.rules_cache),
            'standards': {
                'CERT-C': len(self.index['CERT-C']),
                'MISRA-C:2012': len(self.index['MISRA-C:2012']),
            },
            'rules': {rule_id: rule.to_dict() for rule_id, rule in self.rules_cache.items()}
        }
        
        with open(output_path, 'w', encoding='utf-8') as f:
            json.dump(rules_dict, f, indent=2)
        
        logger.info(f"Exported {len(self.rules_cache)} rules to {output_path}")
    
    def search_rules(self, keyword: str) -> List[ParasoftRule]:
        """
        Search rules by keyword in title or description
        
        Args:
            keyword: Search keyword
        
        Returns:
            List of matching rules
        """
        keyword_lower = keyword.lower()
        matches = []
        
        for rule in self.rules_cache.values():
            if (keyword_lower in rule.title.lower() or 
                keyword_lower in rule.description.lower() or
                keyword_lower in rule.rule_id.lower()):
                matches.append(rule)
        
        return matches


# ─────────────────────────────────────────────────────────────────────────────
# CLI Interface
# ─────────────────────────────────────────────────────────────────────────────

def main():
    """Main entry point for CLI usage"""
    import sys
    
    print("\n" + "="*80)
    print("  PARASOFT RULES PARSER - Version 1.0.0")
    print("  Qorix India Pvt Ltd")
    print("="*80 + "\n")
    
    # Default path
    workspace_path = Path(__file__).parent.parent
    rules_dir = workspace_path / "data" / "Parasoft_Enabled_Rules_List" / "gendoc"
    
    # Override if provided
    if len(sys.argv) > 1:
        rules_dir = Path(sys.argv[1])
    
    if not rules_dir.exists():
        print(f"[ERROR] Rules directory not found: {rules_dir}")
        sys.exit(1)
    
    # Initialize parser
    parser = ParasoftRulesParser(rules_dir)
    
    # Load all rules
    count = parser.load_all_rules()
    
    print(f"[OK] Loaded {count} Parasoft rules")
    print(f"  - CERT-C rules: {len(parser.index['CERT-C'])}")
    print(f"  - MISRA-C:2012 rules: {len(parser.index['MISRA-C:2012'])}")
    
    # Export to JSON
    output_path = workspace_path / "data" / "Parasoft_Rules_Database.json"
    parser.export_rules_database(output_path)
    print(f"\n[OK] Exported rules database to: {output_path}")
    
    # Demo: Get a specific rule
    test_rule_id = "CERT_C-STR31-a"
    rule = parser.get_rule(test_rule_id)
    if rule:
        print(f"\n[DEMO] Retrieved rule: {test_rule_id}")
        print(f"  Title: {rule.title}")
        print(f"  Standard: {rule.standard}")
        print(f"  CWE Mappings: {', '.join(rule.cwe_mappings) if rule.cwe_mappings else 'None'}")
        print(f"  Has Fix Example: {'Yes' if rule.example_repair else 'No'}")
    
    print("\n[SUCCESS] Parasoft rules parsing completed!\n")


if __name__ == "__main__":
    main()
