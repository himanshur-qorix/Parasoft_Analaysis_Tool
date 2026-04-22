"""
AI-Powered Justification Pattern Learning & Quality Analysis
Uses Ollama LLM to intelligently analyze patterns and identify tool-generated mistakes
"""

import json
import re
from pathlib import Path
from collections import defaultdict
from datetime import datetime
import logging

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
    logger.warning("Ollama not available - will use rule-based analysis only")


class JustificationPatternAnalyzer:
    """AI-powered analyzer for justification patterns and quality assessment"""
    
    def __init__(self, justifications_dir: str = "justifications", use_ai: bool = True):
        self.justifications_dir = Path(justifications_dir)
        self.patterns = defaultdict(list)
        self.rule_statistics = defaultdict(int)
        self.module_statistics = defaultdict(int)
        self.reason_patterns = []
        self.use_ai = use_ai and OLLAMA_AVAILABLE
        
        # Quality tracking
        self.tool_generated = []  # Tool-generated justifications
        self.human_written = []   # Human-written justifications
        self.quality_issues = defaultdict(list)  # Identified quality problems
        
        # Pattern learning database (works without AI)
        self.learned_patterns = defaultdict(list)  # Rule -> learned good examples
        self.mistake_patterns = []  # Common mistake patterns to avoid
        self.quality_rules = []  # Learned quality indicators
        
        # Load AI config
        self.ai_config = self._load_ai_config()
        
        # Load existing learned patterns
        self._load_learned_patterns()
        
        if self.use_ai:
            logger.info("🤖 AI-Powered Analysis: ENABLED (using Ollama)")
            self._test_ollama_connection()
        else:
            logger.info("📊 Rule-Based Analysis: Using pattern matching")
        
    def analyze_all_modules(self):
        """Analyze all suppression comment files to learn patterns"""
        logger.info("=" * 80)
        logger.info("JUSTIFICATION PATTERN LEARNING SYSTEM")
        logger.info("=" * 80)
        logger.info(f"Scanning directory: {self.justifications_dir}")
        
        if not self.justifications_dir.exists():
            logger.error(f"Justifications directory not found: {self.justifications_dir}")
            return
        
        # Find all suppression comment files
        suppress_files = list(self.justifications_dir.glob('*_suppress_comments_*.txt'))
        
        if not suppress_files:
            logger.warning("No suppression comment files found!")
            return
        
        logger.info(f"Found {len(suppress_files)} suppression comment files\n")
        
        # Analyze each file
        total_justifications = 0
        for file_path in suppress_files:
            count = self._analyze_file(file_path)
            total_justifications += count
        
        logger.info(f"\n{'=' * 80}")
        logger.info(f"ANALYSIS COMPLETE: {total_justifications} justifications analyzed")
        logger.info(f"{'=' * 80}\n")
        
        # AI-powered quality analysis
        if self.use_ai:
            logger.info("\n🤖 Running AI-Powered Quality Analysis...")
            self._ai_quality_analysis()
        
        # Rule-based pattern learning (works without AI)
        if not self.use_ai:
            logger.info("\n📚 Building Pattern Database (Rule-Based Learning)...")
            self._build_pattern_database()
            self._detect_quality_issues_rule_based()
        
        # Generate learning report
        self._generate_learning_report()
        
    def _analyze_file(self, file_path: Path) -> int:
        """Analyze a single suppression comment file"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            # Extract module name
            module_name = file_path.stem.split('_suppress_comments')[0]
            
            # Pattern to match suppression comments
            pattern = r'/\*\s*parasoft-begin-suppress\s+([A-Z0-9_\-\s]+?)\s+"Reason:\s*(.+?)"\s*\*/'
            matches = re.findall(pattern, content)
            
            if matches:
                logger.info(f"\n📁 Module: {module_name}")
                logger.info(f"   File: {file_path.name}")
                logger.info(f"   Justifications found: {len(matches)}")
                
                for rule_ids, reason in matches:
                    rule_ids = rule_ids.strip()
                    reason = reason.strip()
                    pattern_entry = {
                        'module': module_name,
                        'rules': rule_ids,
                        'reason': reason,
                        'is_tool_generated': self._is_tool_generated(reason)
                    }
                    self.reason_patterns.append(pattern_entry)
                    
                    # Categorize by origin
                    if pattern_entry['is_tool_generated']:
                        self.tool_generated.append(pattern_entry)
                    else:
                        self.human_written.append(pattern_entry)
                    
                    # Track rule statistics
                    for rule in rule_ids.split():
                        self.rule_statistics[rule] += 1
                    
                    # Track module statistics
                    self.module_statistics[module_name] += 1
            
            return len(matches)
            
        except Exception as e:
            logger.error(f"Error analyzing {file_path.name}: {str(e)}")
            return 0
    
    def _generate_learning_report(self):
        """Generate comprehensive learning report"""
        
        # 1. Module Overview
        logger.info("\n" + "=" * 80)
        logger.info("📊 MODULE JUSTIFICATION OVERVIEW")
        logger.info("=" * 80)
        
        for module, count in sorted(self.module_statistics.items(), key=lambda x: x[1], reverse=True):
            logger.info(f"   {module:20s}: {count:3d} justifications")
        
        # 2. Most Commonly Justified Rules
        logger.info("\n" + "=" * 80)
        logger.info("🔝 TOP 15 MOST JUSTIFIED RULES")
        logger.info("=" * 80)
        logger.info(f"{'Rule ID':40s} | {'Count':5s} | Category")
        logger.info("-" * 80)
        
        for rule, count in sorted(self.rule_statistics.items(), key=lambda x: x[1], reverse=True)[:15]:
            category = self._categorize_rule(rule)
            logger.info(f"{rule:40s} | {count:5d} | {category}")
        
        # 3. Justification Scenarios
        logger.info("\n" + "=" * 80)
        logger.info("📝 COMMON JUSTIFICATION SCENARIOS")
        logger.info("=" * 80)
        
        scenarios = self._identify_scenarios()
        for i, scenario in enumerate(scenarios, 1):
            logger.info(f"\n{i}. {scenario['title']}")
            logger.info(f"   Rules: {scenario['rules']}")
            logger.info(f"   Frequency: {scenario['frequency']} occurrences")
            logger.info(f"   Typical Reason Pattern: {scenario['pattern']}")
            if scenario['modules']:
                logger.info(f"   Seen in modules: {', '.join(scenario['modules'])}")
        
        # 4. Reason Pattern Analysis
        logger.info("\n" + "=" * 80)
        logger.info("💡 JUSTIFICATION REASON PATTERNS")
        logger.info("=" * 80)
        
        if self.use_ai:
            logger.info("\n   (AI-powered categorization - see AI Categorization section above)")
        else:
            reason_types = self._categorize_reasons()
            for reason_type, info in reason_types.items():
                logger.info(f"\n   {reason_type}:")
                logger.info(f"      Count: {info['count']}")
                logger.info(f"      Example: {info['example'][:100]}...")
        
        # 4.5 Quality Analysis Results (AI)
        if self.use_ai and self.quality_issues:
            logger.info("\n" + "=" * 80)
            logger.info("⚠️  QUALITY ISSUES IN TOOL-GENERATED JUSTIFICATIONS")
            logger.info("=" * 80)
            
            total_issues = sum(len(v) for v in self.quality_issues.values())
            logger.info(f"\n   Total issues identified: {total_issues}")
            logger.info(f"   Affected justifications: {len([p for p in self.tool_generated if any(p['reason'] in str(self.quality_issues))])} / {len(self.tool_generated)}")
            
            for issue_type, examples in sorted(self.quality_issues.items(), key=lambda x: len(x[1]), reverse=True):
                logger.info(f"\n   {issue_type}: {len(examples)} occurrences")
                if examples:
                    logger.info(f"      Fix needed: {examples[0]['issue']}")
        
        # 5. Cross-Module Analysis
        logger.info("\n" + "=" * 80)
        logger.info("🔗 CROSS-MODULE CONSISTENCY ANALYSIS")
        logger.info("=" * 80)
        
        cross_module = self._find_cross_module_patterns()
        for pattern in cross_module[:10]:
            logger.info(f"\n   Rule: {pattern['rule']}")
            logger.info(f"   Justified in {len(pattern['modules'])} modules: {', '.join(pattern['modules'])}")
            logger.info(f"   Suggests: {pattern['suggestion']}")
        
        # 6. Generate JSON report
        self._save_json_report()
        
        # 7. Recommendations
        logger.info("\n" + "=" * 80)
        logger.info("✅ RECOMMENDATIONS FOR FUTURE JUSTIFICATIONS")
        logger.info("=" * 80)
        
        recommendations = self._generate_recommendations()
        for i, rec in enumerate(recommendations, 1):
            logger.info(f"\n{i}. {rec}")
        
        logger.info("\n" + "=" * 80)
        logger.info("📄 Detailed report saved to: knowledge_base/justification_patterns_report.json")
        logger.info("=" * 80 + "\n")
    
    def _categorize_rule(self, rule: str) -> str:
        """Categorize rule by type"""
        if 'CERT' in rule:
            return 'CERT C'
        elif 'MISRA' in rule:
            return 'MISRA C'
        elif 'CWE' in rule:
            return 'CWE'
        else:
            return 'Other'
    
    def _identify_scenarios(self) -> list:
        """Identify common justification scenarios"""
        scenarios = []
        
        # Group by rule
        rule_groups = defaultdict(list)
        for pattern in self.reason_patterns:
            for rule in pattern['rules'].split():
                rule_groups[rule].append(pattern)
        
        # Find top scenarios
        for rule, patterns in sorted(rule_groups.items(), key=lambda x: len(x[1]), reverse=True)[:10]:
            modules = list(set([p['module'] for p in patterns]))
            reasons = [p['reason'] for p in patterns]
            
            # Find common reason pattern
            common_pattern = self._find_common_pattern(reasons)
            
            scenarios.append({
                'title': f"Justification for {rule}",
                'rules': rule,
                'frequency': len(patterns),
                'pattern': common_pattern,
                'modules': modules
            })
        
        return scenarios
    
    def _load_ai_config(self) -> dict:
        """Load AI configuration from config.json"""
        try:
            config_path = Path('config/config.json')
            if config_path.exists():
                with open(config_path, 'r') as f:
                    config = json.load(f)
                    return config.get('ai_integration', {})
        except Exception as e:
            logger.debug(f"Could not load AI config: {e}")
        
        return {
            'ollama': {
                'model': 'qwen2.5-coder:1.5b-base',
                'base_url': 'http://localhost:11434'
            }
        }
    
    def _test_ollama_connection(self) -> bool:
        """Test connection to Ollama"""
        try:
            response = ollama.list()
            model = self.ai_config.get('ollama', {}).get('model', 'qwen2.5-coder:1.5b-base')
            logger.info(f"   Connected to Ollama - Model: {model}")
            return True
        except Exception as e:
            logger.warning(f"   Ollama not available: {e}")
            self.use_ai = False
            return False
    
    def _is_tool_generated(self, reason: str) -> bool:
        """Detect if justification was tool-generated vs human-written"""
        # Tool-generated patterns
        tool_patterns = [
            r'_Parasoft_REF_\d+',  # Generic reference pattern
            r'^Reason: \w+_Parasoft_REF',  # Starts with generic pattern
        ]
        
        for pattern in tool_patterns:
            if re.search(pattern, reason, re.IGNORECASE):
                return True
        
        # If very short and generic
        if len(reason) < 40 and 'ref' in reason.lower():
            return True
        
        return False
    
    def _ai_quality_analysis(self):
        """Use AI to analyze justification quality and identify mistakes"""
        logger.info("\n" + "=" * 80)
        logger.info("🔍 AI-POWERED QUALITY ANALYSIS")
        logger.info("=" * 80)
        
        total = len(self.reason_patterns)
        tool_count = len(self.tool_generated)
        human_count = len(self.human_written)
        
        logger.info(f"\n📊 Justification Origin Analysis:")
        logger.info(f"   Total justifications: {total}")
        logger.info(f"   Tool-generated: {tool_count} ({100*tool_count//total if total else 0}%)")
        logger.info(f"   Human-written: {human_count} ({100*human_count//total if total else 0}%)")
        
        # Analyze tool-generated quality
        if self.tool_generated:
            logger.info(f"\n🤖 Analyzing {len(self.tool_generated)} tool-generated justifications...")
            self._analyze_tool_mistakes()
        
        # AI categorization of all justifications
        if len(self.reason_patterns) > 0:
            logger.info(f"\n🧠 AI is categorizing {len(self.reason_patterns)} justifications...")
            self._ai_categorize_reasons()
    
    def _analyze_tool_mistakes(self):
        """Analyze tool-generated justifications to identify common mistakes"""
        mistakes = defaultdict(list)
        
        for pattern in self.tool_generated[:20]:  # Sample first 20
            issues = self._ai_evaluate_justification(pattern)
            
            if issues:
                for issue in issues:
                    mistakes[issue['type']].append({
                        'module': pattern['module'],
                        'rules': pattern['rules'],
                        'reason': pattern['reason'],
                        'issue': issue['description']
                    })
        
        # Report mistakes
        if mistakes:
            logger.info("\n" + "=" * 80)
            logger.info("⚠️  TOOL-GENERATED JUSTIFICATION ISSUES IDENTIFIED")
            logger.info("=" * 80)
            
            for issue_type, occurrences in sorted(mistakes.items(), key=lambda x: len(x[1]), reverse=True):
                logger.info(f"\n   {issue_type}: {len(occurrences)} occurrences")
                # Show first example
                if occurrences:
                    example = occurrences[0]
                    logger.info(f"      Example: {example['reason'][:80]}...")
                    logger.info(f"      Issue: {example['issue']}")
        
        self.quality_issues = mistakes
    
    def _ai_evaluate_justification(self, pattern: dict) -> list:
        """Use AI to evaluate a single justification and identify issues"""
        if not self.use_ai:
            return []
        
        try:
            model = self.ai_config.get('ollama', {}).get('model', 'qwen2.5-coder:1.5b-base')
            
            prompt = f"""Analyze this code violation justification for quality issues:

Rule: {pattern['rules']}
Justification: {pattern['reason']}

Identify specific issues (respond with JSON array):
1. "too_generic" - Generic reference without explanation
2. "missing_context" - Doesn't explain WHY justified
3. "vague" - Unclear reasoning
4. "good" - Well-written, descriptive

Respond ONLY with JSON array of issues:
[
  {{"type": "issue_type", "description": "brief explanation"}}
]

If good quality, return: []

Analysis:"""
            
            response = ollama.generate(
                model=model,
                prompt=prompt,
                options={'temperature': 0.1, 'num_predict': 200}
            )
            
            response_text = response.response if hasattr(response, 'response') else response.get('response', '')
            
            # Parse response
            issues = self._parse_ai_issues(response_text)
            return issues
            
        except Exception as e:
            logger.debug(f"AI evaluation failed: {e}")
            return []
    
    def _parse_ai_issues(self, response: str) -> list:
        """Parse AI response for quality issues"""
        try:
            # Extract JSON array
            response = response.strip().replace('```json', '').replace('```', '')
            start = response.find('[')
            end = response.rfind(']') + 1
            
            if start >= 0 and end > start:
                json_str = response[start:end]
                issues = json.loads(json_str)
                return issues if isinstance(issues, list) else []
        except:
            pass
        
        # Fallback: text analysis
        issues = []
        if 'generic' in response.lower():
            issues.append({'type': 'too_generic', 'description': 'Generic reference without context'})
        if 'missing' in response.lower() or 'context' in response.lower():
            issues.append({'type': 'missing_context', 'description': 'Missing WHY explanation'})
        
        return issues
    
    def _ai_categorize_reasons(self):
        """Use AI to intelligently categorize justification reasons"""
        # Sample diverse justifications for AI analysis
        sample_size = min(30, len(self.reason_patterns))
        sampled = self.reason_patterns[:sample_size]
        
        categorization = defaultdict(list)
        
        for pattern in sampled:
            category = self._ai_categorize_single(pattern)
            if category:
                categorization[category].append(pattern)
        
        # Report AI categorization
        logger.info("\n" + "=" * 80)
        logger.info("🏷️  AI JUSTIFICATION CATEGORIZATION")
        logger.info("=" * 80)
        
        for category, patterns in sorted(categorization.items(), key=lambda x: len(x[1]), reverse=True):
            logger.info(f"\n   {category}: {len(patterns)} justifications")
            if patterns:
                logger.info(f"      Example: {patterns[0]['reason'][:100]}...")
    
    def _ai_categorize_single(self, pattern: dict) -> str:
        """Use AI to categorize a single justification"""
        if not self.use_ai:
            return None
        
        try:
            model = self.ai_config.get('ollama', {}).get('model', 'qwen2.5-coder:1.5b-base')
            
            prompt = f"""Categorize this violation justification into ONE category:

Rule: {pattern['rules']}
Justification: {pattern['reason']}

Categories:
- API_COMPATIBILITY: API/interface compatibility requirements
- LEGACY_CODE: Legacy code or backward compatibility
- DESIGN_DECISION: Intentional design or architecture choice
- PLATFORM_CONSTRAINT: Platform, hardware, or AUTOSAR constraints
- PERFORMANCE: Performance optimization
- GENERIC_REFERENCE: Generic reference without explanation
- CROSS_MODULE: Cross-module consistency
- OTHER: Other valid reason

Respond with ONLY the category name:"""
            
            response = ollama.generate(
                model=model,
                prompt=prompt,
                options={'temperature': 0.1, 'num_predict': 30}
            )
            
            response_text = response.response if hasattr(response, 'response') else response.get('response', '')
            category = response_text.strip().upper().split('\n')[0]
            
            # Validate category
            valid_categories = ['API_COMPATIBILITY', 'LEGACY_CODE', 'DESIGN_DECISION', 
                              'PLATFORM_CONSTRAINT', 'PERFORMANCE', 'GENERIC_REFERENCE',
                              'CROSS_MODULE', 'OTHER']
            
            if any(cat in category for cat in valid_categories):
                return category.split()[0]  # Take first word if multiple
            
            return 'OTHER'
            
        except Exception as e:
            logger.debug(f"AI categorization failed: {e}")
            return None
    
    # ============================================================================
    # RULE-BASED PATTERN LEARNING (Works without AI)
    # ============================================================================
    
    def _load_learned_patterns(self):
        """Load previously learned patterns from knowledge base"""
        try:
            pattern_db_path = Path('knowledge_base/learned_patterns_db.json')
            if pattern_db_path.exists():
                with open(pattern_db_path, 'r', encoding='utf-8') as f:
                    data = json.load(f)
                    self.learned_patterns = defaultdict(list, data.get('rule_patterns', {}))
                    self.mistake_patterns = data.get('mistake_patterns', [])
                    self.quality_rules = data.get('quality_rules', [])
                logger.info(f"📚 Loaded {len(self.learned_patterns)} learned rule patterns")
        except Exception as e:
            logger.debug(f"No existing learned patterns found: {e}")
    
    def _build_pattern_database(self):
        """Build pattern database from human-written justifications"""
        logger.info(f"   Analyzing {len(self.human_written)} human-written justifications...")
        
        # Group human-written justifications by rule
        for pattern in self.human_written:
            rules = pattern['rules'].split()
            for rule in rules:
                # Store good examples for each rule
                self.learned_patterns[rule].append({
                    'reason': pattern['reason'],
                    'module': pattern['module'],
                    'length': len(pattern['reason']),
                    'has_specific_details': self._has_specific_details(pattern['reason']),
                    'quality_score': self._calculate_quality_score(pattern['reason'])
                })
        
        # Build quality rules from learned patterns
        self._extract_quality_rules()
        
        # Identify common mistake patterns from tool-generated
        self._identify_mistake_patterns()
        
        # Save learned patterns
        self._save_learned_patterns()
        
        logger.info(f"   ✅ Built pattern database for {len(self.learned_patterns)} rules")
        logger.info(f"   ✅ Identified {len(self.mistake_patterns)} mistake patterns")
        logger.info(f"   ✅ Extracted {len(self.quality_rules)} quality indicators")
    
    def _has_specific_details(self, reason: str) -> bool:
        """Check if justification has specific technical details"""
        specific_indicators = [
            r'\b[A-Z][a-z]+_[A-Z]',  # PascalCase_UPPERCASE (function/variable names)
            r'\bv?\d+\.\d+',  # Version numbers
            r'\b0x[0-9A-Fa-f]+',  # Hex addresses
            r'\bDR-\d{4}-\d+',  # Deviation request IDs
            r'\bASIL-[A-D]',  # Safety levels
            r'\b\d+\s*(KB|MB|ms|μs|bytes)',  # Technical measurements
            r'(?:tested|validated|verified) (?:in|via|through)',  # Validation mentions
        ]
        
        return any(re.search(pattern, reason) for pattern in specific_indicators)
    
    def _calculate_quality_score(self, reason: str) -> float:
        """Calculate quality score for a justification (0-10)"""
        score = 5.0  # Base score
        
        # Length indicators
        if len(reason) < 30:
            score -= 2.0  # Too short
        elif len(reason) > 100:
            score += 1.0  # Detailed
        elif len(reason) > 200:
            score += 2.0  # Very detailed
        
        # Specific details
        if self._has_specific_details(reason):
            score += 2.0
        
        # Explanation indicators
        explanation_words = ['because', 'due to', 'required for', 'needed for', 
                           'necessary', 'approved', 'validated', 'tested']
        if any(word in reason.lower() for word in explanation_words):
            score += 1.0
        
        # Technical context
        technical_words = ['hardware', 'platform', 'API', 'legacy', 'performance', 
                         'AUTOSAR', 'interrupt', 'real-time', 'safety']
        if any(word.lower() in reason.lower() for word in technical_words):
            score += 1.0
        
        # Negative indicators
        if re.search(r'_Parasoft_REF_\d+', reason):
            score -= 3.0  # Generic reference
        if re.search(r'^Reason:\s*[A-Z][a-z]+_Parasoft_REF', reason):
            score -= 2.0  # Starts with generic
        
        return max(0.0, min(10.0, score))  # Clamp to 0-10
    
    def _extract_quality_rules(self):
        """Extract quality indicators from human-written justifications"""
        self.quality_rules = [
            {
                'indicator': 'specific_details',
                'description': 'Contains function names, version numbers, or technical specifics',
                'examples': [p['reason'][:80] for p in sum(self.learned_patterns.values(), []) 
                           if p['has_specific_details']][:3]
            },
            {
                'indicator': 'adequate_length',
                'description': 'Sufficient explanation (>50 chars)',
                'threshold': 50,
                'avg_human': sum(p['length'] for p in sum(self.learned_patterns.values(), [])) / 
                           max(1, len(sum(self.learned_patterns.values(), [])))
            },
            {
                'indicator': 'high_quality',
                'description': 'Quality score > 7.0',
                'examples': [p['reason'][:80] for p in sum(self.learned_patterns.values(), []) 
                           if p['quality_score'] >= 7.0][:3]
            }
        ]
    
    def _identify_mistake_patterns(self):
        """Identify common mistakes from tool-generated justifications"""
        for pattern in self.tool_generated:
            reason = pattern['reason']
            quality = self._calculate_quality_score(reason)
            
            if quality < 4.0:  # Low quality
                mistake_type = None
                
                if re.search(r'_Parasoft_REF_\d+', reason):
                    mistake_type = 'generic_reference'
                elif len(reason) < 30:
                    mistake_type = 'too_short'
                elif not self._has_specific_details(reason):
                    mistake_type = 'missing_details'
                
                if mistake_type:
                    self.mistake_patterns.append({
                        'type': mistake_type,
                        'example': reason,
                        'rules': pattern['rules'],
                        'module': pattern['module']
                    })
    
    def _detect_quality_issues_rule_based(self):
        """Detect quality issues using learned patterns (no AI needed)"""
        logger.info(f"   Evaluating {len(self.tool_generated)} tool-generated justifications...")
        
        for pattern in self.tool_generated:
            reason = pattern['reason']
            quality_score = self._calculate_quality_score(reason)
            
            if quality_score < 5.0:
                # Determine specific issue
                if re.search(r'_Parasoft_REF_\d+', reason):
                    self.quality_issues['generic_reference'].append({
                        'module': pattern['module'],
                        'rules': pattern['rules'],
                        'reason': reason,
                        'issue': 'Uses generic reference instead of specific explanation',
                        'quality_score': quality_score
                    })
                elif len(reason) < 30:
                    self.quality_issues['too_short'].append({
                        'module': pattern['module'],
                        'rules': pattern['rules'],
                        'reason': reason,
                        'issue': 'Justification is too brief, lacks context',
                        'quality_score': quality_score
                    })
                elif not self._has_specific_details(reason):
                    self.quality_issues['vague'].append({
                        'module': pattern['module'],
                        'rules': pattern['rules'],
                        'reason': reason,
                        'issue': 'Lacks specific technical details or references',
                        'quality_score': quality_score
                    })
        
        # Report findings
        if self.quality_issues:
            logger.info("\n" + "=" * 80)
            logger.info("⚠️  QUALITY ISSUES DETECTED (Rule-Based Analysis)")
            logger.info("=" * 80)
            
            for issue_type, occurrences in sorted(self.quality_issues.items(), 
                                                 key=lambda x: len(x[1]), reverse=True):
                logger.info(f"\n   {issue_type.replace('_', ' ').title()}: {len(occurrences)} issues")
                if occurrences:
                    example = occurrences[0]
                    logger.info(f"      Example: {example['reason'][:80]}...")
                    logger.info(f"      Issue: {example['issue']}")
                    logger.info(f"      Quality Score: {example['quality_score']:.1f}/10")
    
    def _save_learned_patterns(self):
        """Save learned patterns to knowledge base for future use"""
        kb_dir = Path('knowledge_base')
        kb_dir.mkdir(exist_ok=True)
        
        pattern_db = {
            'timestamp': datetime.now().isoformat(),
            'source': 'rule_based_learning',
            'rule_patterns': dict(self.learned_patterns),
            'mistake_patterns': self.mistake_patterns,
            'quality_rules': self.quality_rules,
            'statistics': {
                'total_rules_learned': len(self.learned_patterns),
                'total_mistakes_identified': len(self.mistake_patterns),
                'human_written_analyzed': len(self.human_written),
                'tool_generated_analyzed': len(self.tool_generated)
            }
        }
        
        db_path = kb_dir / 'learned_patterns_db.json'
        with open(db_path, 'w', encoding='utf-8') as f:
            json.dump(pattern_db, f, indent=2)
        
        logger.info(f"   💾 Learned patterns saved to: {db_path}")
    
    # ============================================================================
    # END RULE-BASED LEARNING
    # ============================================================================
    
    def _find_common_pattern(self, reasons: list) -> str:
        """Find common pattern in reasons"""
        if not reasons:
            return "No pattern identified"
        
        # Return the most descriptive reason as example
        longest = max(reasons, key=len)
        return longest if len(longest) > 50 else reasons[0]
    
    def _categorize_reasons(self) -> dict:
        """Categorize justification reasons by type"""
        categories = {
            'Generic References': {'count': 0, 'example': ''},
            'API Compatibility': {'count': 0, 'example': ''},
            'Legacy Code': {'count': 0, 'example': ''},
            'Design Decision': {'count': 0, 'example': ''},
            'Platform Constraint': {'count': 0, 'example': ''},
            'Cross-Module Consistency': {'count': 0, 'example': ''},
            'Other': {'count': 0, 'example': ''}
        }
        
        for pattern in self.reason_patterns:
            reason = pattern['reason'].lower()
            
            if re.search(r'_ref_\d+|parasoft_ref', reason):
                categories['Generic References']['count'] += 1
                if not categories['Generic References']['example']:
                    categories['Generic References']['example'] = pattern['reason']
            elif 'api' in reason or 'interface' in reason or 'compatibility' in reason:
                categories['API Compatibility']['count'] += 1
                if not categories['API Compatibility']['example']:
                    categories['API Compatibility']['example'] = pattern['reason']
            elif 'legacy' in reason or 'backward' in reason:
                categories['Legacy Code']['count'] += 1
                if not categories['Legacy Code']['example']:
                    categories['Legacy Code']['example'] = pattern['reason']
            elif 'design' in reason or 'architecture' in reason:
                categories['Design Decision']['count'] += 1
                if not categories['Design Decision']['example']:
                    categories['Design Decision']['example'] = pattern['reason']
            elif 'platform' in reason or 'hardware' in reason or 'autosar' in reason:
                categories['Platform Constraint']['count'] += 1
                if not categories['Platform Constraint']['example']:
                    categories['Platform Constraint']['example'] = pattern['reason']
            elif 'module' in reason or 'consistent' in reason:
                categories['Cross-Module Consistency']['count'] += 1
                if not categories['Cross-Module Consistency']['example']:
                    categories['Cross-Module Consistency']['example'] = pattern['reason']
            else:
                categories['Other']['count'] += 1
                if not categories['Other']['example']:
                    categories['Other']['example'] = pattern['reason']
        
        return {k: v for k, v in categories.items() if v['count'] > 0}
    
    def _find_cross_module_patterns(self) -> list:
        """Find rules that are consistently justified across multiple modules"""
        rule_modules = defaultdict(set)
        
        for pattern in self.reason_patterns:
            for rule in pattern['rules'].split():
                rule_modules[rule].add(pattern['module'])
        
        cross_module_patterns = []
        for rule, modules in rule_modules.items():
            if len(modules) >= 2:  # Justified in at least 2 modules
                cross_module_patterns.append({
                    'rule': rule,
                    'modules': list(modules),
                    'suggestion': f'Common deviation - likely acceptable to justify in new modules'
                })
        
        return sorted(cross_module_patterns, key=lambda x: len(x['modules']), reverse=True)
    
    def _generate_recommendations(self) -> list:
        """Generate AI-powered recommendations for future justifications"""
        recommendations = []
        
        # Quality analysis from AI
        if self.use_ai and self.quality_issues:
            total_tool_issues = sum(len(v) for v in self.quality_issues.values())
            total_tool = len(self.tool_generated)
            
            if total_tool > 0:
                issue_rate = (total_tool_issues / total_tool) * 100
                recommendations.append(
                    f"🤖 AI Analysis: {total_tool_issues} issues found in {total_tool} tool-generated justifications "
                    f"({issue_rate:.1f}% error rate). Most common issues:"
                )
                
                for issue_type, examples in sorted(self.quality_issues.items(), key=lambda x: len(x[1]), reverse=True)[:3]:
                    recommendations.append(f"   • {issue_type}: {len(examples)} cases - Consider improving tool prompts")
        
        # Generic references warning
        generic_count = sum(1 for p in self.reason_patterns if re.search(r'_ref_\d+|parasoft_ref', p['reason'].lower()))
        total_count = len(self.reason_patterns)
        
        if generic_count > total_count * 0.5:
            recommendations.append(
                f"⚠️  {generic_count}/{total_count} justifications use generic references. "
                f"{'AI has identified this as a quality issue.' if self.use_ai else 'Consider more descriptive reasons.'}"
            )
        else:
            recommendations.append(
                f"✅ Good: Only {generic_count}/{total_count} justifications use generic references. "
                f"Most justifications provide meaningful context."
            )
        
        # Tool vs Human quality comparison
        if self.tool_generated and self.human_written:
            tool_avg_length = sum(len(p['reason']) for p in self.tool_generated) / len(self.tool_generated)
            human_avg_length = sum(len(p['reason']) for p in self.human_written) / len(self.human_written)
            
            recommendations.append(
                f"📊 Quality Comparison: Tool-generated avg length: {tool_avg_length:.0f} chars, "
                f"Human-written avg length: {human_avg_length:.0f} chars"
            )
            
            if human_avg_length > tool_avg_length * 1.5:
                recommendations.append(
                    f"💡 Human justifications are {((human_avg_length/tool_avg_length - 1) * 100):.0f}% more descriptive. "
                    f"Consider improving tool's justification generation."
                )
        
        # Cross-module consistency
        cross_module = self._find_cross_module_patterns()
        if cross_module:
            recommendations.append(
                f"✅ Found {len(cross_module)} rules consistently justified across multiple modules. "
                f"These are good candidates for project-wide deviations."
            )
        
        # Top rules
        top_rules = sorted(self.rule_statistics.items(), key=lambda x: x[1], reverse=True)[:5]
        recommendations.append(
            f"📌 Most frequently justified rules: {', '.join([r[0] for r in top_rules])}. "
            f"Consider documenting these as accepted deviations in project standards."
        )
        
        # AI-specific recommendations
        if self.use_ai:
            recommendations.append(
                f"🤖 AI few-shot learning is analyzing {len(self.reason_patterns)} examples. "
                f"Quality improvements in justifications will directly improve AI output."
            )
            
            if self.quality_issues:
                recommendations.append(
                    f"🔧 Action Items: Review tool-generated justifications. "
                    f"AI has identified specific improvement areas (see quality issues above)."
                )
        else:
            recommendations.append(
                f"💡 Enable Ollama AI for deeper quality analysis and intelligent categorization."
            )
        
        return recommendations
    
    def _save_json_report(self):
        """Save detailed analysis to JSON file"""
        report = {
            'timestamp': datetime.now().isoformat(),
            'ai_powered': self.use_ai,
            'summary': {
                'total_modules': len(self.module_statistics),
                'total_justifications': len(self.reason_patterns),
                'unique_rules': len(self.rule_statistics),
                'tool_generated': len(self.tool_generated),
                'human_written': len(self.human_written)
            },
            'quality_analysis': {
                'tool_generated_issues': {k: len(v) for k, v in self.quality_issues.items()},
                'total_issues_found': sum(len(v) for v in self.quality_issues.values()),
                'issue_details': dict(self.quality_issues) if self.quality_issues else {}
            },
            'module_statistics': dict(self.module_statistics),
            'rule_statistics': dict(sorted(self.rule_statistics.items(), key=lambda x: x[1], reverse=True)),
            'cross_module_patterns': self._find_cross_module_patterns(),
            'scenarios': self._identify_scenarios(),
            'reason_categories': self._categorize_reasons(),
            'all_patterns': self.reason_patterns,
            'recommendations': self._generate_recommendations()
        }
        
        # Save to knowledge base
        kb_dir = Path('knowledge_base')
        kb_dir.mkdir(exist_ok=True)
        
        report_path = kb_dir / 'justification_patterns_report.json'
        with open(report_path, 'w', encoding='utf-8') as f:
            json.dump(report, f, indent=2)
        
        logger.info(f"\n💾 Detailed JSON report saved to: {report_path}")


def main():
    """Main execution function"""
    print("\n" + "=" * 80)
    print("AI-POWERED JUSTIFICATION PATTERN LEARNING & QUALITY ANALYSIS")
    print("Intelligently analyzes patterns and identifies tool-generated mistakes")
    print("=" * 80 + "\n")
    
    # Check if Ollama is available
    if OLLAMA_AVAILABLE:
        print("🤖 Ollama AI: AVAILABLE - Will use AI for intelligent analysis")
    else:
        print("📊 Ollama AI: NOT AVAILABLE - Using rule-based analysis")
        print("   Install: pip install ollama")
        print("   And ensure Ollama server is running\n")
    
    analyzer = JustificationPatternAnalyzer(use_ai=OLLAMA_AVAILABLE)
    analyzer.analyze_all_modules()
    
    print("\n✅ Analysis complete! Review the report above.")
    print("📊 JSON report available at: knowledge_base/justification_patterns_report.json")
    
    if OLLAMA_AVAILABLE:
        print("\n🤖 AI Insights Generated:")
        print("   • Intelligent reason categorization")
        print("   • Quality assessment of tool-generated justifications")
        print("   • Specific mistake identification")
        print("   • Improvement recommendations")
    
    print("\n💡 Use these insights to:")
    print("   1. Understand what violations are commonly justified")
    print("   2. See quality issues in tool-generated justifications")
    print("   3. Identify cross-module justification patterns")
    print("   4. Improve future justification quality")
    print("   5. Train AI with better examples\n")


if __name__ == "__main__":
    main()
