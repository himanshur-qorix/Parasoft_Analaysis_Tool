#!/usr/bin/env python3
"""
MISRA-C:2012 and CERT-C Static Analyzer
Simulates Parasoft-style violation detection for C/C++ source files.

Usage:
    python misra_cert_checker.py <file_or_directory> [--report html|json|text] [--output report.html]
"""

import re
import os
import sys
import json
import argparse
import html
from dataclasses import dataclass, field
from typing import List, Optional
from datetime import datetime
from pathlib import Path


# ─────────────────────────────────────────────────────────────────────────────
# Data Models
# ─────────────────────────────────────────────────────────────────────────────

@dataclass
class Violation:
    rule_id: str          # e.g. "MISRA-C:2012 Rule 14.4" or "CERT-C: EXP34-C"
    standard: str         # "MISRA" or "CERT"
    severity: str         # "Required" / "Advisory" / "Mandatory" / "High" / "Medium" / "Low"
    category: str         # e.g. "Control Flow", "Memory Management"
    description: str      # Human-readable rule description
    message: str          # Specific violation message
    file: str
    line: int
    column: int
    snippet: str          # Source line content
    rationale: str        # Why this rule exists
    is_known: bool = False           # Found in knowledge base
    occurrence_count: int = 0        # Times seen before
    has_proven_fix: bool = False     # Has fix in knowledge base
    proven_fix: Optional[str] = None # Proven fix suggestion


@dataclass
class AnalysisResult:
    file: str
    violations: List[Violation] = field(default_factory=list)
    lines_analyzed: int = 0


# ─────────────────────────────────────────────────────────────────────────────
# Rule Definitions
# ─────────────────────────────────────────────────────────────────────────────

MISRA_RULES = [
    {
        "id": "MISRA-C:2012 Rule 1.1",
        "severity": "Required",
        "category": "Language",
        "description": "The program shall contain no violations of the standard C syntax.",
        "rationale": "Undefined behavior from non-standard constructs can cause unpredictable results."
    },
    {
        "id": "MISRA-C:2012 Rule 2.1",
        "severity": "Required",
        "category": "Unused Code",
        "description": "A project shall not contain unreachable code.",
        "rationale": "Unreachable code indicates a logical error and misleads maintainers."
    },
    {
        "id": "MISRA-C:2012 Rule 2.2",
        "severity": "Required",
        "category": "Unused Code",
        "description": "There shall be no dead code.",
        "rationale": "Dead code adds maintenance burden without providing functionality."
    },
    {
        "id": "MISRA-C:2012 Rule 10.1",
        "severity": "Required",
        "category": "Type Conversion",
        "description": "Operands shall not be of an inappropriate essential type.",
        "rationale": "Mixed-type operations can produce unintended results."
    },
    {
        "id": "MISRA-C:2012 Rule 11.1",
        "severity": "Required",
        "category": "Pointer Conversion",
        "description": "Conversions shall not be performed between a pointer to a function and any other type.",
        "rationale": "Such conversions are undefined behavior."
    },
    {
        "id": "MISRA-C:2012 Rule 12.1",
        "severity": "Advisory",
        "category": "Expressions",
        "description": "The precedence of operators within expressions should be made explicit.",
        "rationale": "Operator precedence mistakes are a frequent source of bugs."
    },
    {
        "id": "MISRA-C:2012 Rule 13.1",
        "severity": "Required",
        "category": "Side Effects",
        "description": "Initializer lists shall not contain persistent side effects.",
        "rationale": "Side effects in initializers can cause order-of-evaluation issues."
    },
    {
        "id": "MISRA-C:2012 Rule 14.4",
        "severity": "Required",
        "category": "Control Flow",
        "description": "The controlling expression of an if statement shall be essentially Boolean.",
        "rationale": "Non-Boolean conditions in if/while can mask unintentional assignments."
    },
    {
        "id": "MISRA-C:2012 Rule 15.1",
        "severity": "Advisory",
        "category": "Control Flow",
        "description": "The goto statement should not be used.",
        "rationale": "goto makes control flow difficult to follow and maintain."
    },
    {
        "id": "MISRA-C:2012 Rule 15.4",
        "severity": "Advisory",
        "category": "Control Flow",
        "description": "There should be no more than one break or goto statement used to terminate any iteration statement.",
        "rationale": "Multiple breaks complicate loop logic."
    },
    {
        "id": "MISRA-C:2012 Rule 15.5",
        "severity": "Advisory",
        "category": "Control Flow",
        "description": "A function should have a single point of exit at the end.",
        "rationale": "Multiple return points complicate analysis and testing."
    },
    {
        "id": "MISRA-C:2012 Rule 16.1",
        "severity": "Required",
        "category": "Switch",
        "description": "All switch statements shall be well-formed.",
        "rationale": "Malformed switch statements lead to fallthrough bugs."
    },
    {
        "id": "MISRA-C:2012 Rule 16.4",
        "severity": "Required",
        "category": "Switch",
        "description": "Every switch statement shall have a default clause.",
        "rationale": "Without default, unhandled cases may silently pass."
    },
    {
        "id": "MISRA-C:2012 Rule 17.7",
        "severity": "Required",
        "category": "Functions",
        "description": "The value returned by a function having non-void return type shall be used.",
        "rationale": "Ignoring return values can hide errors."
    },
    {
        "id": "MISRA-C:2012 Rule 18.1",
        "severity": "Required",
        "category": "Pointers",
        "description": "A pointer resulting from arithmetic on a pointer operand shall address an element of the same array.",
        "rationale": "Out-of-bounds pointer arithmetic causes undefined behavior."
    },
    {
        "id": "MISRA-C:2012 Rule 21.3",
        "severity": "Required",
        "category": "Standard Libraries",
        "description": "The memory allocation and deallocation functions of <stdlib.h> shall not be used.",
        "rationale": "Dynamic memory allocation is unsafe in safety-critical systems."
    },
    {
        "id": "MISRA-C:2012 Rule 21.6",
        "severity": "Required",
        "category": "Standard Libraries",
        "description": "The Standard Library I/O functions shall not be used.",
        "rationale": "stdio functions are non-deterministic in safety-critical contexts."
    },
    {
        "id": "MISRA-C:2012 Rule 22.1",
        "severity": "Required",
        "category": "Resources",
        "description": "All resources obtained dynamically shall be explicitly released.",
        "rationale": "Resource leaks degrade system reliability."
    },
]

CERT_RULES = [
    {
        "id": "CERT-C: DCL30-C",
        "severity": "High",
        "category": "Declarations",
        "description": "Declare objects with appropriate storage durations.",
        "rationale": "Using objects beyond their storage duration causes undefined behavior."
    },
    {
        "id": "CERT-C: EXP33-C",
        "severity": "High",
        "category": "Expressions",
        "description": "Do not read uninitialized memory.",
        "rationale": "Reading uninitialized variables causes unpredictable behavior."
    },
    {
        "id": "CERT-C: EXP34-C",
        "severity": "High",
        "category": "Expressions",
        "description": "Do not dereference null pointers.",
        "rationale": "Null pointer dereference causes crashes and security vulnerabilities."
    },
    {
        "id": "CERT-C: EXP35-C",
        "severity": "Medium",
        "category": "Expressions",
        "description": "Do not modify objects with temporary lifetime.",
        "rationale": "Modifying temporaries results in undefined behavior."
    },
    {
        "id": "CERT-C: INT30-C",
        "severity": "High",
        "category": "Integers",
        "description": "Ensure that unsigned integer operations do not wrap.",
        "rationale": "Integer wrap-around causes logical errors in arithmetic."
    },
    {
        "id": "CERT-C: INT32-C",
        "severity": "High",
        "category": "Integers",
        "description": "Ensure that operations on signed integers do not result in overflow.",
        "rationale": "Signed integer overflow is undefined behavior in C."
    },
    {
        "id": "CERT-C: INT33-C",
        "severity": "High",
        "category": "Integers",
        "description": "Ensure that division and remainder operations do not result in divide-by-zero errors.",
        "rationale": "Division by zero is undefined behavior."
    },
    {
        "id": "CERT-C: ARR30-C",
        "severity": "High",
        "category": "Arrays",
        "description": "Do not form or use out-of-bounds pointers or array subscripts.",
        "rationale": "Out-of-bounds access leads to crashes and security vulnerabilities."
    },
    {
        "id": "CERT-C: STR31-C",
        "severity": "High",
        "category": "Strings",
        "description": "Guarantee that storage for strings has sufficient space for character data and the null terminator.",
        "rationale": "Buffer overflows via string operations are a leading security vulnerability."
    },
    {
        "id": "CERT-C: MEM30-C",
        "severity": "High",
        "category": "Memory",
        "description": "Do not access freed memory.",
        "rationale": "Use-after-free causes undefined behavior and is exploitable."
    },
    {
        "id": "CERT-C: MEM31-C",
        "severity": "Medium",
        "category": "Memory",
        "description": "Free dynamically allocated memory when no longer needed.",
        "rationale": "Memory leaks degrade performance and reliability."
    },
    {
        "id": "CERT-C: MEM34-C",
        "severity": "High",
        "category": "Memory",
        "description": "Only free memory allocated dynamically.",
        "rationale": "Freeing non-heap memory causes undefined behavior."
    },
    {
        "id": "CERT-C: FIO30-C",
        "severity": "High",
        "category": "I/O",
        "description": "Exclude user input from format strings.",
        "rationale": "Format string vulnerabilities allow arbitrary code execution."
    },
    {
        "id": "CERT-C: ERR33-C",
        "severity": "Medium",
        "category": "Error Handling",
        "description": "Detect and handle standard library errors.",
        "rationale": "Ignoring return values of library calls masks failures."
    },
]


# ─────────────────────────────────────────────────────────────────────────────
# Pattern-Based Checkers
# ─────────────────────────────────────────────────────────────────────────────

def check_goto(lines, filepath) -> List[Violation]:
    violations = []
    rule = next(r for r in MISRA_RULES if "15.1" in r["id"])
    for i, line in enumerate(lines, 1):
        stripped = line.strip()
        if re.match(r'\bgoto\b', stripped) and not stripped.startswith('//') and not stripped.startswith('*'):
            violations.append(Violation(
                rule_id=rule["id"], standard="MISRA", severity=rule["severity"],
                category=rule["category"], description=rule["description"],
                message=f"Use of 'goto' statement detected.",
                file=filepath, line=i, column=line.find('goto') + 1,
                snippet=line.rstrip(), rationale=rule["rationale"]
            ))
    return violations


def check_switch_default(lines, filepath) -> List[Violation]:
    violations = []
    rule = next(r for r in MISRA_RULES if "16.4" in r["id"])
    in_switch = False
    switch_brace_depth = 0
    brace_depth = 0
    has_default = False
    switch_line = 0

    for i, line in enumerate(lines, 1):
        stripped = line.strip()
        if re.match(r'\bswitch\s*\(', stripped):
            in_switch = True
            switch_brace_depth = brace_depth
            has_default = False
            switch_line = i
        if in_switch:
            brace_depth += stripped.count('{') - stripped.count('}')
            if re.match(r'\bdefault\s*:', stripped):
                has_default = True
            if brace_depth <= switch_brace_depth and i > switch_line:
                if not has_default:
                    violations.append(Violation(
                        rule_id=rule["id"], standard="MISRA", severity=rule["severity"],
                        category=rule["category"], description=rule["description"],
                        message="switch statement missing 'default' clause.",
                        file=filepath, line=switch_line, column=1,
                        snippet=lines[switch_line - 1].rstrip(), rationale=rule["rationale"]
                    ))
                in_switch = False
        else:
            brace_depth += stripped.count('{') - stripped.count('}')
    return violations


def check_malloc_free(lines, filepath) -> List[Violation]:
    violations = []
    misra_rule = next(r for r in MISRA_RULES if "21.3" in r["id"])
    cert_mem31 = next(r for r in CERT_RULES if "MEM31" in r["id"])

    malloc_pattern = re.compile(r'\b(malloc|calloc|realloc)\s*\(')
    free_pattern = re.compile(r'\bfree\s*\(')

    for i, line in enumerate(lines, 1):
        stripped = line.strip()
        if stripped.startswith('//') or stripped.startswith('*'):
            continue
        if malloc_pattern.search(line):
            violations.append(Violation(
                rule_id=misra_rule["id"], standard="MISRA", severity=misra_rule["severity"],
                category=misra_rule["category"], description=misra_rule["description"],
                message="Dynamic memory allocation (malloc/calloc/realloc) used.",
                file=filepath, line=i, column=1, snippet=line.rstrip(),
                rationale=misra_rule["rationale"]
            ))
    return violations


def check_null_pointer_deref(lines, filepath) -> List[Violation]:
    violations = []
    rule = next(r for r in CERT_RULES if "EXP34" in r["id"])
    # Pattern: pointer assigned NULL then dereferenced without check
    null_assigned = {}
    for i, line in enumerate(lines, 1):
        stripped = line.strip()
        if stripped.startswith('//'):
            continue
        m = re.match(r'(\w+)\s*=\s*NULL\s*;', stripped)
        if m:
            null_assigned[m.group(1)] = i
        for ptr, assign_line in list(null_assigned.items()):
            if re.search(rf'\b{re.escape(ptr)}\s*->', line) or re.search(rf'\*\s*{re.escape(ptr)}\b', line):
                if not re.search(rf'\b{re.escape(ptr)}\s*!=\s*NULL', line) and \
                   not re.search(rf'NULL\s*!=\s*{re.escape(ptr)}', line) and \
                   not re.search(rf'if\s*\(\s*{re.escape(ptr)}', line):
                    violations.append(Violation(
                        rule_id=rule["id"], standard="CERT", severity=rule["severity"],
                        category=rule["category"], description=rule["description"],
                        message=f"Potential null pointer dereference of '{ptr}' (assigned NULL at line {assign_line}).",
                        file=filepath, line=i, column=1, snippet=line.rstrip(),
                        rationale=rule["rationale"]
                    ))
                    del null_assigned[ptr]
                    break
    return violations


def check_printf_format_string(lines, filepath) -> List[Violation]:
    violations = []
    rule = next(r for r in CERT_RULES if "FIO30" in r["id"])
    # Detect printf/sprintf with variable (non-literal) format string
    fmt_pattern = re.compile(r'\b(printf|fprintf|sprintf|snprintf)\s*\(\s*(?!\")')
    for i, line in enumerate(lines, 1):
        if fmt_pattern.search(line) and not line.strip().startswith('//'):
            violations.append(Violation(
                rule_id=rule["id"], standard="CERT", severity=rule["severity"],
                category=rule["category"], description=rule["description"],
                message="Non-literal format string passed to printf-family function (format string injection risk).",
                file=filepath, line=i, column=1, snippet=line.rstrip(),
                rationale=rule["rationale"]
            ))
    return violations


def check_uninitialized_vars(lines, filepath) -> List[Violation]:
    violations = []
    rule = next(r for r in CERT_RULES if "EXP33" in r["id"])
    # Heuristic: int/char/float declared without initializer, then used
    decl_pattern = re.compile(r'\b(int|char|float|double|uint\w*|int\w*)\s+(\w+)\s*;')
    declared = {}
    for i, line in enumerate(lines, 1):
        stripped = line.strip()
        if stripped.startswith('//'):
            continue
        m = decl_pattern.search(stripped)
        if m:
            declared[m.group(2)] = i
        for var, decl_line in list(declared.items()):
            # Check if used in expression before assignment
            if re.search(rf'[^=!<>]=\s*{re.escape(var)}\b', line) or \
               re.search(rf'\b{re.escape(var)}\s*[+\-\*/]', line):
                if not re.search(rf'\b{re.escape(var)}\s*=\s*[^=]', line):
                    violations.append(Violation(
                        rule_id=rule["id"], standard="CERT", severity=rule["severity"],
                        category=rule["category"], description=rule["description"],
                        message=f"Potential use of uninitialized variable '{var}' (declared at line {decl_line}).",
                        file=filepath, line=i, column=1, snippet=line.rstrip(),
                        rationale=rule["rationale"]
                    ))
                    del declared[var]
                    break
    return violations


def check_stdio_usage(lines, filepath) -> List[Violation]:
    violations = []
    rule = next(r for r in MISRA_RULES if "21.6" in r["id"])
    stdio_pattern = re.compile(r'\b(printf|scanf|fprintf|fscanf|fopen|fclose|fread|fwrite|gets|puts)\s*\(')
    for i, line in enumerate(lines, 1):
        if line.strip().startswith('//'):
            continue
        if stdio_pattern.search(line):
            violations.append(Violation(
                rule_id=rule["id"], standard="MISRA", severity=rule["severity"],
                category=rule["category"], description=rule["description"],
                message=f"Standard I/O library function used: forbidden in safety-critical code.",
                file=filepath, line=i, column=1, snippet=line.rstrip(),
                rationale=rule["rationale"]
            ))
    return violations


def check_non_boolean_condition(lines, filepath) -> List[Violation]:
    violations = []
    rule = next(r for r in MISRA_RULES if "14.4" in r["id"])
    # Detects: if (x = value) — assignment in condition
    assign_in_cond = re.compile(r'\bif\s*\(([^=!<>]*[^=!<>])=([^=][^\)]*)\)')
    for i, line in enumerate(lines, 1):
        if line.strip().startswith('//'):
            continue
        if assign_in_cond.search(line):
            violations.append(Violation(
                rule_id=rule["id"], standard="MISRA", severity=rule["severity"],
                category=rule["category"], description=rule["description"],
                message="Assignment used as controlling expression in 'if' statement.",
                file=filepath, line=i, column=1, snippet=line.rstrip(),
                rationale=rule["rationale"]
            ))
    return violations


def check_multiple_returns(lines, filepath) -> List[Violation]:
    violations = []
    rule = next(r for r in MISRA_RULES if "15.5" in r["id"])
    return_count = 0
    in_function = False
    brace_depth = 0
    func_line = 0

    for i, line in enumerate(lines, 1):
        stripped = line.strip()
        if re.match(r'^\w[\w\s\*]+\w\s*\([^)]*\)\s*\{', stripped):
            in_function = True
            brace_depth = 1
            return_count = 0
            func_line = i
            continue
        if in_function:
            brace_depth += stripped.count('{') - stripped.count('}')
            if re.match(r'\breturn\b', stripped):
                return_count += 1
            if brace_depth <= 0:
                if return_count > 1:
                    violations.append(Violation(
                        rule_id=rule["id"], standard="MISRA", severity=rule["severity"],
                        category=rule["category"], description=rule["description"],
                        message=f"Function starting at line {func_line} has {return_count} return statements (should have 1).",
                        file=filepath, line=func_line, column=1,
                        snippet=lines[func_line - 1].rstrip(), rationale=rule["rationale"]
                    ))
                in_function = False
    return violations


def check_integer_overflow_risk(lines, filepath) -> List[Violation]:
    violations = []
    rule = next(r for r in CERT_RULES if "INT32" in r["id"])
    # Look for signed int arithmetic without bounds check
    overflow_pattern = re.compile(r'\b(int|long)\s+\w+\s*=\s*\w+\s*[\+\-\*]\s*\w+')
    for i, line in enumerate(lines, 1):
        if line.strip().startswith('//'):
            continue
        if overflow_pattern.search(line):
            violations.append(Violation(
                rule_id=rule["id"], standard="CERT", severity=rule["severity"],
                category=rule["category"], description=rule["description"],
                message="Signed integer arithmetic without overflow check detected.",
                file=filepath, line=i, column=1, snippet=line.rstrip(),
                rationale=rule["rationale"]
            ))
    return violations


def check_array_bounds(lines, filepath) -> List[Violation]:
    violations = []
    rule = next(r for r in CERT_RULES if "ARR30" in r["id"])
    # Detect fixed-size array with index that could be out of bounds
    array_decl = re.compile(r'\b\w+\s+\w+\[(\d+)\]')
    array_sizes = {}
    for i, line in enumerate(lines, 1):
        m = array_decl.search(line)
        if m:
            # Capture array name and size
            nm = re.search(r'\b(\w+)\[(\d+)\]', line)
            if nm:
                array_sizes[nm.group(1)] = int(nm.group(2))
        for arr, size in array_sizes.items():
            idx = re.search(rf'\b{re.escape(arr)}\[(\d+)\]', line)
            if idx and int(idx.group(1)) >= size:
                violations.append(Violation(
                    rule_id=rule["id"], standard="CERT", severity=rule["severity"],
                    category=rule["category"], description=rule["description"],
                    message=f"Array '{arr}' accessed at index {idx.group(1)}, but size is {size} (out-of-bounds).",
                    file=filepath, line=i, column=1, snippet=line.rstrip(),
                    rationale=rule["rationale"]
                ))
    return violations


def check_strcpy_strcat(lines, filepath) -> List[Violation]:
    violations = []
    rule = next(r for r in CERT_RULES if "STR31" in r["id"])
    unsafe_str = re.compile(r'\b(strcpy|strcat|gets|sprintf)\s*\(')
    for i, line in enumerate(lines, 1):
        if line.strip().startswith('//'):
            continue
        if unsafe_str.search(line):
            violations.append(Violation(
                rule_id=rule["id"], standard="CERT", severity=rule["severity"],
                category=rule["category"], description=rule["description"],
                message="Unsafe string function used (strcpy/strcat/gets/sprintf). Use size-bounded alternatives.",
                file=filepath, line=i, column=1, snippet=line.rstrip(),
                rationale=rule["rationale"]
            ))
    return violations


# ─────────────────────────────────────────────────────────────────────────────
# Analyzer Engine
# ─────────────────────────────────────────────────────────────────────────────

ALL_CHECKERS = [
    check_goto,
    check_switch_default,
    check_malloc_free,
    check_null_pointer_deref,
    check_printf_format_string,
    check_uninitialized_vars,
    check_stdio_usage,
    check_non_boolean_condition,
    check_multiple_returns,
    check_integer_overflow_risk,
    check_array_bounds,
    check_strcpy_strcat,
]


def analyze_file(filepath: str) -> AnalysisResult:
    result = AnalysisResult(file=filepath)
    try:
        with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
            lines = f.readlines()
        result.lines_analyzed = len(lines)
        for checker in ALL_CHECKERS:
            result.violations.extend(checker(lines, filepath))
    except Exception as e:
        print(f"[ERROR] Could not analyze {filepath}: {e}", file=sys.stderr)
    # Sort violations by line number
    result.violations.sort(key=lambda v: v.line)
    return result


def analyze_path(path: str) -> List[AnalysisResult]:
    results = []
    p = Path(path)
    if p.is_file():
        results.append(analyze_file(str(p)))
    elif p.is_dir():
        for ext in ('*.c', '*.h', '*.cpp', '*.hpp', '*.cc'):
            for fp in p.rglob(ext):
                results.append(analyze_file(str(fp)))
    else:
        print(f"[ERROR] Path not found: {path}", file=sys.stderr)
    return results


# ─────────────────────────────────────────────────────────────────────────────
# Report Generators
# ─────────────────────────────────────────────────────────────────────────────

def report_text(results: List[AnalysisResult]) -> str:
    lines = []
    lines.append("=" * 80)
    lines.append("  MISRA-C / CERT-C Static Analysis Report")
    lines.append(f"  Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    lines.append("=" * 80)
    total = sum(len(r.violations) for r in results)
    for result in results:
        if not result.violations:
            lines.append(f"\n✅  {result.file}  — No violations found ({result.lines_analyzed} lines)")
            continue
        lines.append(f"\n📄  {result.file}  ({result.lines_analyzed} lines, {len(result.violations)} violations)")
        lines.append("-" * 70)
        for v in result.violations:
            lines.append(f"  [{v.severity}] {v.rule_id}")
            lines.append(f"  Line {v.line}:{v.column}  {v.message}")
            lines.append(f"  Code: {v.snippet.strip()}")
            lines.append(f"  Rationale: {v.rationale}")
            lines.append("")
    lines.append("=" * 80)
    lines.append(f"  TOTAL VIOLATIONS: {total}")
    misra_count = sum(1 for r in results for v in r.violations if v.standard == "MISRA")
    cert_count = sum(1 for r in results for v in r.violations if v.standard == "CERT")
    lines.append(f"  MISRA: {misra_count}   CERT: {cert_count}")
    lines.append("=" * 80)
    return "\n".join(lines)


def report_json(results: List[AnalysisResult]) -> str:
    data = {
        "generated": datetime.now().isoformat(),
        "summary": {
            "total_violations": sum(len(r.violations) for r in results),
            "files_analyzed": len(results),
            "misra_violations": sum(1 for r in results for v in r.violations if v.standard == "MISRA"),
            "cert_violations": sum(1 for r in results for v in r.violations if v.standard == "CERT"),
        },
        "files": [
            {
                "file": r.file,
                "lines_analyzed": r.lines_analyzed,
                "violations": [
                    {
                        "rule_id": v.rule_id,
                        "standard": v.standard,
                        "severity": v.severity,
                        "category": v.category,
                        "description": v.description,
                        "message": v.message,
                        "line": v.line,
                        "column": v.column,
                        "snippet": v.snippet,
                        "rationale": v.rationale,
                    }
                    for v in r.violations
                ]
            }
            for r in results
        ]
    }
    return json.dumps(data, indent=2)


def report_html(results: List[AnalysisResult]) -> str:
    total = sum(len(r.violations) for r in results)
    misra_count = sum(1 for r in results for v in r.violations if v.standard == "MISRA")
    cert_count = sum(1 for r in results for v in r.violations if v.standard == "CERT")
    files_count = len(results)
    now = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

    severity_color = {
        "Mandatory": "#c0392b", "Required": "#e67e22", "Advisory": "#2980b9",
        "High": "#c0392b", "Medium": "#e67e22", "Low": "#27ae60"
    }
    standard_color = {"MISRA": "#8e44ad", "CERT": "#16a085"}

    rows = ""
    for result in results:
        for v in result.violations:
            sc = severity_color.get(v.severity, "#555")
            stc = standard_color.get(v.standard, "#555")
            rows += f"""
        <tr>
          <td><span class="badge" style="background:{stc}">{html.escape(v.standard)}</span></td>
          <td><code>{html.escape(v.rule_id)}</code></td>
          <td><span class="badge" style="background:{sc}">{html.escape(v.severity)}</span></td>
          <td>{html.escape(v.category)}</td>
          <td>{html.escape(os.path.basename(result.file))}</td>
          <td style="text-align:center">{v.line}</td>
          <td>{html.escape(v.message)}</td>
          <td><pre class="snippet">{html.escape(v.snippet.strip())}</pre></td>
        </tr>"""

    return f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8"/>
<title>MISRA / CERT Static Analysis Report</title>
<style>
  body {{ font-family: 'Segoe UI', Arial, sans-serif; background: #f4f6f8; margin: 0; padding: 20px; color: #2c3e50; }}
  .header-logo {{ text-align: center; margin-bottom: 20px; }}
  .header-logo svg {{ width: 150px; height: auto; }}
  .company {{ text-align: center; color: #7f8c8d; font-size: 0.9em; margin-bottom: 10px; }}
  h1 {{ color: #2c3e50; border-bottom: 3px solid #3498db; padding-bottom: 10px; text-align: center; }}
  .meta {{ color: #7f8c8d; font-size: 0.9em; margin-bottom: 20px; text-align: center; }}
  .summary {{ display: flex; gap: 20px; margin-bottom: 30px; flex-wrap: wrap; }}
  .card {{ background: #fff; border-radius: 8px; padding: 20px 30px; box-shadow: 0 2px 8px rgba(0,0,0,.08);
            text-align: center; min-width: 130px; }}
  .card .num {{ font-size: 2.4em; font-weight: bold; }}
  .card .lbl {{ font-size: 0.85em; color: #7f8c8d; text-transform: uppercase; }}
  table {{ width: 100%; border-collapse: collapse; background: #fff; border-radius: 8px;
           box-shadow: 0 2px 8px rgba(0,0,0,.08); overflow: hidden; }}
  th {{ background: #2c3e50; color: #fff; padding: 12px 10px; text-align: left; font-size: 0.85em; }}
  td {{ padding: 10px; border-bottom: 1px solid #ecf0f1; font-size: 0.88em; vertical-align: top; }}
  tr:hover td {{ background: #f8f9fa; }}
  .badge {{ color: #fff; padding: 2px 8px; border-radius: 4px; font-size: 0.78em; font-weight: bold; }}
  pre.snippet {{ background: #2c3e50; color: #ecf0f1; padding: 6px 10px; border-radius: 4px;
                  margin: 0; font-size: 0.82em; white-space: pre-wrap; word-break: break-all; max-width: 300px; }}
  code {{ background: #ecf0f1; padding: 2px 6px; border-radius: 3px; font-size: 0.85em; }}
  .footer {{ text-align: center; margin-top: 40px; padding: 20px; border-top: 1px solid #ecf0f1; color: #7f8c8d; font-size: 0.9em; }}
</style>
</head>
<body>
<div class="header-logo">
  <svg width="150" height="38" viewBox="0 0 150 38" fill="none" xmlns="http://www.w3.org/2000/svg">
    <path d="M136.846 10.9347L127.134 0.970459L123.693 4.45571L133.449 14.4646L123.693 24.4289L127.134 27.9588L136.846 17.9499L146.559 27.9588L150 24.4289L140.243 14.4646L150 4.45571L146.559 0.970459L136.846 10.9347Z" fill="#3C00FF"/>
    <path d="M113.893 1.41748H109.102V27.1547H113.893V1.41748Z" fill="#3C00FF"/>
    <path d="M81.0102 5.75153H89.7645C91.2454 5.75153 92.3778 6.91328 92.3778 8.43249V10.7113C92.3778 12.837 90.913 13.4339 89.7645 13.3923H81.9248V15.8945C81.9248 22.1501 86.89 27.2886 92.9875 27.2886H96.7767V22.2841H92.9875C90.4178 22.2841 88.1966 20.6309 87.2384 18.352H89.7645C91.7244 18.352 93.6408 17.5924 95.0345 16.1626C96.4718 14.6881 97.2558 12.722 97.2558 10.7113V8.43249C97.2558 4.23232 93.8586 0.791748 89.7645 0.791748H81.0102C76.8726 0.791748 73.5189 4.23232 73.5189 8.43249V27.378H78.3098V8.43249C78.3098 6.91328 79.5294 5.75153 81.0102 5.75153Z" fill="#3C00FF"/>
    <path d="M50.2612 0.74707C42.5086 0.74707 36.1933 7.1367 36.1933 15.1349C36.1933 23.0884 42.5086 29.6121 50.2612 29.6121C58.0574 29.6121 64.3727 23.0884 64.3727 15.1349C64.3727 7.1367 58.0574 0.74707 50.2612 0.74707ZM50.2612 24.6523C45.209 24.6523 41.0713 20.3628 41.0713 15.1349C41.0713 9.90702 45.209 5.66217 50.2612 5.66217C55.357 5.66217 59.4946 9.90702 59.4946 15.1349C59.4946 20.3628 55.357 24.6523 50.2612 24.6523Z" fill="#3C00FF"/>
    <path d="M28.0487 15.2689C28.0487 7.31541 21.7334 0.881104 14.0244 0.881104C6.27176 0.881104 0 7.31541 0 15.2689C0 22.5969 5.31357 28.6291 12.2386 29.6121C13.763 33.991 17.9007 37.2528 22.6916 37.2528H26.4372V32.293H22.6916C20.5139 32.293 18.4669 31.0866 17.378 29.2993C23.5627 27.7354 28.0487 22.0607 28.0487 15.2689ZM14.0244 24.7417C8.9721 24.7417 4.83448 20.4968 4.83448 15.2689C4.83448 10.0411 8.9721 5.7962 14.0244 5.7962C19.1202 5.7962 23.2578 10.0411 23.2578 15.2689C23.2578 20.4968 19.1202 24.7417 14.0244 24.7417Z" fill="#3C00FF"/>
  </svg>
</div>
<p class="company">Qorix India Pvt Ltd</p>
<h1>🔍 MISRA-C / CERT-C Static Analysis Report</h1>
<p class="meta">Generated: {now} &nbsp;|&nbsp; Files analyzed: {files_count}</p>

<div class="summary">
  <div class="card"><div class="num" style="color:#c0392b">{total}</div><div class="lbl">Total Violations</div></div>
  <div class="card"><div class="num" style="color:#8e44ad">{misra_count}</div><div class="lbl">MISRA Violations</div></div>
  <div class="card"><div class="num" style="color:#16a085">{cert_count}</div><div class="lbl">CERT Violations</div></div>
  <div class="card"><div class="num" style="color:#2980b9">{files_count}</div><div class="lbl">Files Analyzed</div></div>
</div>

<table>
  <thead>
    <tr>
      <th>Standard</th><th>Rule ID</th><th>Severity</th><th>Category</th>
      <th>File</th><th>Line</th><th>Message</th><th>Code Snippet</th>
    </tr>
  </thead>
  <tbody>
    {rows if rows else '<tr><td colspan="8" style="text-align:center;padding:30px;color:#27ae60">✅ No violations found!</td></tr>'}
  </tbody>
</table>

<div class="footer">
  <p>© 2025 Qorix India Pvt Ltd. All rights reserved.</p>
  <p>MISRA/CERT Static Analyzer - Parasoft AI Agent</p>
</div>
</body>
</html>"""


def report_html_with_kb(results: List[AnalysisResult], known_violations: List[Violation], new_violations: List[Violation]) -> str:
    """Generate enhanced HTML report with knowledge base insights"""
    total = sum(len(r.violations) for r in results)
    misra_count = sum(1 for r in results for v in r.violations if v.standard == "MISRA")
    cert_count = sum(1 for r in results for v in r.violations if v.standard == "CERT")
    files_count = len(results)
    now = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    
    known_count = len(known_violations)
    new_count = len(new_violations)
    with_fixes = sum(1 for v in known_violations if v.has_proven_fix)

    severity_color = {
        "Mandatory": "#c0392b", "Required": "#e67e22", "Advisory": "#2980b9",
        "High": "#c0392b", "Medium": "#e67e22", "Low": "#27ae60"
    }
    standard_color = {"MISRA": "#8e44ad", "CERT": "#16a085"}

    # Generate rows for all violations
    all_violations = known_violations + new_violations
    rows = ""
    for result in results:
        for v in result.violations:
            sc = severity_color.get(v.severity, "#555")
            stc = standard_color.get(v.standard, "#555")
            
            # Add status badge
            status_badge = ""
            if v.is_known:
                if v.has_proven_fix:
                    status_badge = '<span class="badge" style="background:#27ae60;margin-left:5px" title="Has proven fix">✅ KNOWN (Fix Available)</span>'
                else:
                    status_badge = f'<span class="badge" style="background:#3498db;margin-left:5px" title="Seen {v.occurrence_count} times">📊 KNOWN ({v.occurrence_count}x)</span>'
            else:
                status_badge = '<span class="badge" style="background:#e74c3c;margin-left:5px">⚠️ NEW</span>'
            
            # Add proven fix tooltip
            fix_tooltip = ""
            if v.proven_fix:
                fix_info = html.escape(v.proven_fix[:200])
                fix_tooltip = f'<div style="font-size:0.85em;color:#27ae60;margin-top:3px">💡 Fix: {fix_info}...</div>'
            
            rows += f"""
        <tr>
          <td><span class="badge" style="background:{stc}">{html.escape(v.standard)}</span></td>
          <td><code>{html.escape(v.rule_id)}</code>{status_badge}</td>
          <td><span class="badge" style="background:{sc}">{html.escape(v.severity)}</span></td>
          <td>{html.escape(v.category)}</td>
          <td>{html.escape(os.path.basename(result.file))}</td>
          <td style="text-align:center">{v.line}</td>
          <td>{html.escape(v.message)}{fix_tooltip}</td>
          <td><pre class="snippet">{html.escape(v.snippet.strip())}</pre></td>
        </tr>"""

    return f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8"/>
<title>MISRA / CERT Static Analysis Report with Knowledge Base Insights</title>
<style>
  body {{ font-family: 'Segoe UI', Arial, sans-serif; background: #f4f6f8; margin: 0; padding: 20px; color: #2c3e50; }}
  .header-logo {{ text-align: center; margin-bottom: 20px; }}
  .header-logo svg {{ width: 150px; height: auto; }}
  .company {{ text-align: center; color: #7f8c8d; font-size: 0.9em; margin-bottom: 10px; }}
  h1 {{ color: #2c3e50; border-bottom: 3px solid #3498db; padding-bottom: 10px; text-align: center; }}
  .meta {{ color: #7f8c8d; font-size: 0.9em; margin-bottom: 20px; text-align: center; }}
  .summary {{ display: flex; gap: 20px; margin-bottom: 30px; flex-wrap: wrap; }}
  .card {{ background: #fff; border-radius: 8px; padding: 20px 30px; box-shadow: 0 2px 8px rgba(0,0,0,.08);
            text-align: center; min-width: 130px; }}
  .card .num {{ font-size: 2.4em; font-weight: bold; }}
  .card .lbl {{ font-size: 0.85em; color: #7f8c8d; text-transform: uppercase; }}
  .kb-alert {{ background: #fff3cd; border-left: 4px solid #ffc107; padding: 15px; margin: 20px 0; border-radius: 4px; }}
  .kb-success {{ background: #d4edda; border-left: 4px solid #28a745; padding: 15px; margin: 20px 0; border-radius: 4px; }}
  table {{ width: 100%; border-collapse: collapse; background: #fff; border-radius: 8px;
           box-shadow: 0 2px 8px rgba(0,0,0,.08); overflow: hidden; }}
  th {{ background: #2c3e50; color: #fff; padding: 12px 10px; text-align: left; font-size: 0.85em; }}
  td {{ padding: 10px; border-bottom: 1px solid #ecf0f1; font-size: 0.88em; vertical-align: top; }}
  tr:hover td {{ background: #f8f9fa; }}
  .badge {{ color: #fff; padding: 2px 8px; border-radius: 4px; font-size: 0.78em; font-weight: bold; }}
  pre.snippet {{ background: #2c3e50; color: #ecf0f1; padding: 6px 10px; border-radius: 4px;
                  margin: 0; font-size: 0.82em; white-space: pre-wrap; word-break: break-all; max-width: 300px; }}
  code {{ background: #ecf0f1; padding: 2px 6px; border-radius: 3px; font-size: 0.85em; }}
  .footer {{ text-align: center; margin-top: 40px; padding: 20px; border-top: 1px solid #ecf0f1; color: #7f8c8d; font-size: 0.9em; }}
</style>
</head>
<body>
<div class="header-logo">
  <svg width="150" height="38" viewBox="0 0 150 38" fill="none" xmlns="http://www.w3.org/2000/svg">
    <path d="M136.846 10.9347L127.134 0.970459L123.693 4.45571L133.449 14.4646L123.693 24.4289L127.134 27.9588L136.846 17.9499L146.559 27.9588L150 24.4289L140.243 14.4646L150 4.45571L146.559 0.970459L136.846 10.9347Z" fill="#3C00FF"/>
    <path d="M113.893 1.41748H109.102V27.1547H113.893V1.41748Z" fill="#3C00FF"/>
    <path d="M81.0102 5.75153H89.7645C91.2454 5.75153 92.3778 6.91328 92.3778 8.43249V10.7113C92.3778 12.837 90.913 13.4339 89.7645 13.3923H81.9248V15.8945C81.9248 22.1501 86.89 27.2886 92.9875 27.2886H96.7767V22.2841H92.9875C90.4178 22.2841 88.1966 20.6309 87.2384 18.352H89.7645C91.7244 18.352 93.6408 17.5924 95.0345 16.1626C96.4718 14.6881 97.2558 12.722 97.2558 10.7113V8.43249C97.2558 4.23232 93.8586 0.791748 89.7645 0.791748H81.0102C76.8726 0.791748 73.5189 4.23232 73.5189 8.43249V27.378H78.3098V8.43249C78.3098 6.91328 79.5294 5.75153 81.0102 5.75153Z" fill="#3C00FF"/>
    <path d="M50.2612 0.74707C42.5086 0.74707 36.1933 7.1367 36.1933 15.1349C36.1933 23.0884 42.5086 29.6121 50.2612 29.6121C58.0574 29.6121 64.3727 23.0884 64.3727 15.1349C64.3727 7.1367 58.0574 0.74707 50.2612 0.74707ZM50.2612 24.6523C45.209 24.6523 41.0713 20.3628 41.0713 15.1349C41.0713 9.90702 45.209 5.66217 50.2612 5.66217C55.357 5.66217 59.4946 9.90702 59.4946 15.1349C59.4946 20.3628 55.357 24.6523 50.2612 24.6523Z" fill="#3C00FF"/>
    <path d="M28.0487 15.2689C28.0487 7.31541 21.7334 0.881104 14.0244 0.881104C6.27176 0.881104 0 7.31541 0 15.2689C0 22.5969 5.31357 28.6291 12.2386 29.6121C13.763 33.991 17.9007 37.2528 22.6916 37.2528H26.4372V32.293H22.6916C20.5139 32.293 18.4669 31.0866 17.378 29.2993C23.5627 27.7354 28.0487 22.0607 28.0487 15.2689ZM14.0244 24.7417C8.9721 24.7417 4.83448 20.4968 4.83448 15.2689C4.83448 10.0411 8.9721 5.7962 14.0244 5.7962C19.1202 5.7962 23.2578 10.0411 23.2578 15.2689C23.2578 20.4968 19.1202 24.7417 14.0244 24.7417Z" fill="#3C00FF"/>
  </svg>
</div>
<p class="company">Qorix India Pvt Ltd</p>
<h1>🔍 MISRA-C / CERT-C Static Analysis Report with KB Insights</h1>
<p class="meta">Generated: {now} &nbsp;|&nbsp; Files analyzed: {files_count}</p>

<div class="summary">
  <div class="card"><div class="num" style="color:#c0392b">{total}</div><div class="lbl">Total Violations</div></div>
  <div class="card"><div class="num" style="color:#27ae60">{known_count}</div><div class="lbl">Known Violations</div></div>
  <div class="card"><div class="num" style="color:#e74c3c">{new_count}</div><div class="lbl">New Violations</div></div>
  <div class="card"><div class="num" style="color:#3498db">{with_fixes}</div><div class="lbl">With Proven Fixes</div></div>
  <div class="card"><div class="num" style="color:#8e44ad">{misra_count}</div><div class="lbl">MISRA Violations</div></div>
  <div class="card"><div class="num" style="color:#16a085">{cert_count}</div><div class="lbl">CERT Violations</div></div>
</div>

{f'<div class="kb-alert">⚠️ <strong>{new_count} NEW violations detected!</strong> These haven\'t been seen before - review carefully.</div>' if new_count > 0 else ''}
{f'<div class="kb-success">✅ <strong>{with_fixes} violations have proven fixes available</strong> from previous analyses. Apply these to resolve issues faster!</div>' if with_fixes > 0 else ''}

<table>
  <thead>
    <tr>
      <th>Standard</th><th>Rule ID / Status</th><th>Severity</th><th>Category</th>
      <th>File</th><th>Line</th><th>Message / Fix Suggestion</th><th>Code Snippet</th>
    </tr>
  </thead>
  <tbody>
    {rows if rows else '<tr><td colspan="8" style="text-align:center;padding:30px;color:#27ae60">✅ No violations found!</td></tr>'}
  </tbody>
</table>

<div style="margin-top:30px;padding:15px;background:#fff;border-radius:8px;box-shadow: 0 2px 8px rgba(0,0,0,.08)">
  <h3>Legend</h3>
  <p><span class="badge" style="background:#27ae60">✅ KNOWN (Fix Available)</span> - Violation seen before with proven fix</p>
  <p><span class="badge" style="background:#3498db">📊 KNOWN (Nx)</span> - Violation seen N times before</p>
  <p><span class="badge" style="background:#e74c3c">⚠️ NEW</span> - First time seeing this violation</p>
  <p>💡 <span style="color:#27ae60">Fix suggestions</span> are provided for known violations based on successful fixes from previous analyses</p>
</div>

<div class="footer">
  <p>© 2025 Qorix India Pvt Ltd. All rights reserved.</p>
  <p>MISRA/CERT Static Analyzer with Knowledge Base - Parasoft AI Agent</p>
</div>
</body>
</html>"""


# ─────────────────────────────────────────────────────────────────────────────
# Knowledge Base Integration
# ─────────────────────────────────────────────────────────────────────────────

def load_knowledge_bases(kb_dir: Path, module_name: Optional[str] = None):
    """
    Load module-specific and master knowledge bases
    
    Args:
        kb_dir: Knowledge base directory
        module_name: Optional module name for module-specific KB
    
    Returns:
        Tuple of (module_kb, master_kb)
    """
    module_kb = {}
    master_kb = {}
    
    # Load module-specific knowledge base
    if module_name:
        module_kb_path = kb_dir / f"{module_name}_KnowledgeDatabase.json"
        if module_kb_path.exists():
            try:
                with open(module_kb_path, 'r', encoding='utf-8') as f:
                    module_kb = json.load(f)
                print(f"[INFO] Loaded knowledge base for module: {module_name}", file=sys.stderr)
            except Exception as e:
                print(f"[WARNING] Could not load module KB: {e}", file=sys.stderr)
    
    # Load master knowledge base
    master_kb_path = kb_dir / "Master_KnowledgeDatabase.json"
    if master_kb_path.exists():
        try:
            with open(master_kb_path, 'r', encoding='utf-8') as f:
                master_kb = json.load(f)
            print(f"[INFO] Loaded master knowledge base", file=sys.stderr)
        except Exception as e:
            print(f"[WARNING] Could not load master KB: {e}", file=sys.stderr)
    
    return module_kb, master_kb


def check_violation_against_kb(violation: Violation, module_kb: dict, master_kb: dict):
    """
    Check if violation is known and get insights
    
    Args:
        violation: The violation to check
        module_kb: Module-specific knowledge base
        master_kb: Master knowledge base
    
    Returns:
        Updated violation with KB insights
    """
    rule_id = violation.rule_id
    
    # Check module-specific KB first
    if module_kb and 'violations' in module_kb:
        if rule_id in module_kb['violations']:
            vdata = module_kb['violations'][rule_id]
            violation.is_known = True
            violation.occurrence_count = vdata.get('occurrence_count', 0)
            violation.has_proven_fix = vdata.get('fix_applied', False)
            
            if vdata.get('fix_details'):
                fix_details = vdata['fix_details']
                if isinstance(fix_details, dict):
                    violation.proven_fix = fix_details.get('description', str(fix_details))
                else:
                    violation.proven_fix = str(fix_details)
            
            return violation
    
    # Check master KB for cross-module insights
    if master_kb and 'violations' in master_kb:
        if rule_id in master_kb['violations']:
            vdata = master_kb['violations'][rule_id]
            violation.is_known = True
            violation.occurrence_count = vdata.get('total_occurrences', 0)
            
            # Check for proven fixes from other modules
            fix_examples = vdata.get('fix_examples', [])
            if fix_examples:
                violation.has_proven_fix = True
                first_fix = fix_examples[0]
                fix_details = first_fix.get('fix_details', {})
                
                if isinstance(fix_details, dict):
                    fix_desc = fix_details.get('description', str(fix_details))
                    source_module = first_fix.get('module', 'another module')
                    violation.proven_fix = f"[From {source_module}] {fix_desc}"
                else:
                    violation.proven_fix = str(fix_details)
    
    return violation


def classify_violations(results: List[AnalysisResult], module_kb: dict, master_kb: dict):
    """
    Classify violations as known vs new based on knowledge bases
    
    Args:
        results: Analysis results
        module_kb: Module-specific knowledge base
        master_kb: Master knowledge base
    
    Returns:
        Tuple of (known_violations, new_violations, total_count)
    """
    known_violations = []
    new_violations = []
    
    for result in results:
        for violation in result.violations:
            check_violation_against_kb(violation, module_kb, master_kb)
            
            if violation.is_known:
                known_violations.append(violation)
            else:
                new_violations.append(violation)
    
    total_count = len(known_violations) + len(new_violations)
    
    return known_violations, new_violations, total_count


def print_knowledge_summary(known_violations: List[Violation], new_violations: List[Violation]):
    """
    Print summary of known vs new violations
    
    Args:
        known_violations: List of known violations
        new_violations: List of new violations
    """
    print("\n" + "="*60, file=sys.stderr)
    print("KNOWLEDGE BASE SUMMARY", file=sys.stderr)
    print("="*60, file=sys.stderr)
    print(f"  Known Violations (seen before): {len(known_violations)}", file=sys.stderr)
    print(f"  New Violations (first time): {len(new_violations)}", file=sys.stderr)
    print(f"  Total Violations: {len(known_violations) + len(new_violations)}", file=sys.stderr)
    
    # Count violations with proven fixes
    with_fixes = sum(1 for v in known_violations if v.has_proven_fix)
    if with_fixes > 0:
        print(f"  Known with Proven Fixes: {with_fixes}", file=sys.stderr)
    
    if new_violations:
        print(f"\n  ⚠️  {len(new_violations)} NEW violations detected!", file=sys.stderr)
        print(f"      Review these carefully as they haven't been seen before", file=sys.stderr)
    
    if with_fixes > 0:
        print(f"\n  ✅ {with_fixes} violations have proven fixes available", file=sys.stderr)
        print(f"      Apply these fixes to resolve issues faster", file=sys.stderr)
    
    print("="*60 + "\n", file=sys.stderr)


# ─────────────────────────────────────────────────────────────────────────────
# CLI Entry Point
# ─────────────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(
        description="MISRA-C:2012 and CERT-C Static Analyzer (Parasoft-style)"
    )
    parser.add_argument("path", help="C/C++ source file or directory to analyze")
    parser.add_argument("--report", choices=["text", "json", "html"], default="text",
                        help="Output report format (default: text)")
    parser.add_argument("--output", default=None,
                        help="Save report to file (e.g. report.html). If omitted, prints to stdout.")
    parser.add_argument("--module", default=None,
                        help="Module name to check against knowledge base")
    parser.add_argument("--kb-dir", default=None,
                        help="Knowledge base directory (default: ../knowledge_base)")
    parser.add_argument("--show-known", action="store_true",
                        help="Show detailed breakdown of known vs new violations")
    args = parser.parse_args()

    print(f"[INFO] Analyzing: {args.path}", file=sys.stderr)
    results = analyze_path(args.path)

    if not results:
        print("[ERROR] No C/C++ files found.", file=sys.stderr)
        sys.exit(1)

    total = sum(len(r.violations) for r in results)
    print(f"[INFO] Analyzed {len(results)} file(s), found {total} violation(s).", file=sys.stderr)
    
    # Knowledge base integration
    module_kb = {}
    master_kb = {}
    known_violations = []
    new_violations = []
    
    if args.module or args.kb_dir:
        # Determine knowledge base directory
        if args.kb_dir:
            kb_dir = Path(args.kb_dir)
        else:
            # Default: ../knowledge_base relative to script
            kb_dir = Path(__file__).parent.parent / "knowledge_base"
        
        if kb_dir.exists():
            module_kb, master_kb = load_knowledge_bases(kb_dir, args.module)
            
            # Classify violations
            if module_kb or master_kb:
                known_violations, new_violations, total_classified = classify_violations(
                    results, module_kb, master_kb
                )
                print_knowledge_summary(known_violations, new_violations)
                
                # Show detailed breakdown if requested
                if args.show_known:
                    print("\n" + "="*60, file=sys.stderr)
                    print("KNOWN VIOLATIONS DETAILS", file=sys.stderr)
                    print("="*60, file=sys.stderr)
                    for v in known_violations[:10]:  # Show first 10
                        print(f"\n  {v.rule_id}", file=sys.stderr)
                        print(f"    Previously seen: {v.occurrence_count} times", file=sys.stderr)
                        if v.has_proven_fix:
                            print(f"    ✅ Proven fix: {v.proven_fix[:80]}...", file=sys.stderr)
                    
                    if new_violations:
                        print("\n" + "="*60, file=sys.stderr)
                        print("NEW VIOLATIONS DETAILS", file=sys.stderr)
                        print("="*60, file=sys.stderr)
                        for v in new_violations[:10]:  # Show first 10
                            print(f"\n  {v.rule_id}", file=sys.stderr)
                            print(f"    Severity: {v.severity} | Category: {v.category}", file=sys.stderr)
                            print(f"    ⚠️  First time detected - review carefully", file=sys.stderr)
                    print("="*60 + "\n", file=sys.stderr)
        else:
            print(f"[INFO] Knowledge base directory not found: {kb_dir}", file=sys.stderr)
            print(f"[INFO] Proceeding without knowledge base integration", file=sys.stderr)

    if args.report == "json":
        output = report_json(results)
    elif args.report == "html":
        output = report_html_with_kb(results, known_violations, new_violations) if (known_violations or new_violations) else report_html(results)
    else:
        output = report_text(results)

    if args.output:
        # Create output directory if it doesn't exist
        output_path = Path(args.output)
        output_path.parent.mkdir(parents=True, exist_ok=True)
        
        with open(args.output, 'w', encoding='utf-8') as f:
            f.write(output)
        print(f"[INFO] Report saved to: {args.output}", file=sys.stderr)
    else:
        print(output)


if __name__ == "__main__":
    main()
