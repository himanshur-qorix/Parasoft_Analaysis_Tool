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
  h1 {{ color: #2c3e50; border-bottom: 3px solid #3498db; padding-bottom: 10px; }}
  .meta {{ color: #7f8c8d; font-size: 0.9em; margin-bottom: 20px; }}
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
</style>
</head>
<body>
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
</body>
</html>"""


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
    args = parser.parse_args()

    print(f"[INFO] Analyzing: {args.path}", file=sys.stderr)
    results = analyze_path(args.path)

    if not results:
        print("[ERROR] No C/C++ files found.", file=sys.stderr)
        sys.exit(1)

    total = sum(len(r.violations) for r in results)
    print(f"[INFO] Analyzed {len(results)} file(s), found {total} violation(s).", file=sys.stderr)

    if args.report == "json":
        output = report_json(results)
    elif args.report == "html":
        output = report_html(results)
    else:
        output = report_text(results)

    if args.output:
        with open(args.output, 'w', encoding='utf-8') as f:
            f.write(output)
        print(f"[INFO] Report saved to: {args.output}", file=sys.stderr)
    else:
        print(output)


if __name__ == "__main__":
    main()
