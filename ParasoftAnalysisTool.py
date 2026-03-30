import sys
from pathlib import Path
from bs4 import BeautifulSoup
import pandas as pd
from collections import Counter
#------------------------------------------------
# Parasoft_Analysis_Tool
# Version:V1.0.0
# Developer: Himanshu R
# Platform: Windows
# CodeSet: Python
#------------------------------------------------

# -------------------------------------------------
# 1. Parse Parasoft HTML
# -------------------------------------------------
def parse_parasoft_report(report_path):
    with open(report_path, "r", encoding="utf-8", errors="ignore") as f:
        soup = BeautifulSoup(f, "lxml")

    records = []
    current_file = None

    for row in soup.find_all("tr"):
        tds = row.find_all("td")

        if len(tds) == 1:
            bold = tds[0].find("b")
            if bold:
                text = bold.get_text(strip=True)
                if text.endswith((".c", ".cpp", ".h", ".hpp")):
                    current_file = Path(text).name
            continue

        if len(tds) >= 4 and current_file:
            line_font = tds[1].find("font", class_="gray")
            rule_font = tds[-1].find("font", class_="gray")

            if not line_font or not rule_font:
                continue

            line = line_font.get_text(strip=True).replace(":", "")
            if not line.isdigit():
                continue

            if tds[2].find("pre"):
                continue

            records.append({
                "Violation": tds[2].get_text(" ", strip=True),
                "Violation ID": rule_font.get_text(strip=True),
                "File": current_file,
                "Line number": int(line)
            })

    return records


# -------------------------------------------------
# 2. Load Qorix deviations (CERT & MISRA only)
# -------------------------------------------------
# 3. Knowledge Base for Fixes (RAG)
# -------------------------------------------------
import json

KNOWLEDGE_BASE_PATH = "fix_knowledge_base.json"

def load_knowledge_base():
    if Path(KNOWLEDGE_BASE_PATH).exists():
        with open(KNOWLEDGE_BASE_PATH, "r", encoding="utf-8") as f:
            return json.load(f)
    return []

def save_knowledge_base(kb):
    with open(KNOWLEDGE_BASE_PATH, "w", encoding="utf-8") as f:
        json.dump(kb, f, indent=2)

def retrieve_similar_fix(violation_id, violation_text):
    kb = load_knowledge_base()
    # Simple retrieval: match by violation ID
    for entry in kb:
        if entry["Violation ID"] == violation_id:
            return entry["Fix"]
    return None

def log_fix(violation, fix):
    kb = load_knowledge_base()
    kb.append({
        "Violation": violation["Violation"],
        "Violation ID": violation["Violation ID"],
        "File": violation["File"],
        "Line number": violation["Line number"],
        "Fix": fix
    })
    save_knowledge_base(kb)

# -------------------------------------------------
# 4. Main RAG Workflow
# -------------------------------------------------
def analyze_and_fix(report_path, apply_fixes=False):
    violations = parse_parasoft_report(report_path)
    for v in violations:
        fix = retrieve_similar_fix(v["Violation ID"], v["Violation"])
        if fix:
            print(f"Suggested fix for {v['Violation ID']} at {v['File']}:{v['Line number']}: {fix}")
            if apply_fixes:
                # Placeholder: apply fix logic here
                print(f"Applying fix: {fix}")
                log_fix(v, fix)
        else:
            print(f"No fix found for {v['Violation ID']} at {v['File']}:{v['Line number']}. Please fix manually and log.")

# -------------------------------------------------
# 5. CLI Entry Point
# -------------------------------------------------
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python ParasoftAnalysisTool.py <parasoft_report.html> [--apply]")
        sys.exit(1)
    report = sys.argv[1]
    apply = "--apply" in sys.argv
    analyze_and_fix(report, apply_fixes=apply)
# -------------------------------------------------
def load_justifiable_mapping(qorix_excel):
    sheets = ["CERT Rule analysis", "MISRA Rule analysis"]
    mapping = {}

    for sheet in sheets:
        try:
            raw_df = pd.read_excel(qorix_excel, sheet_name=sheet, header=None)
        except ValueError:
            continue

        header_row = None
        for i, row in raw_df.iterrows():
            row_text = row.astype(str).str.lower()
            if ("parasoft" in " ".join(row_text)) and ("message" in " ".join(row_text)):
                header_row = i
                break

        if header_row is None:
            continue

        df = pd.read_excel(qorix_excel, sheet_name=sheet, header=header_row)
        df.columns = [str(c).strip().lower() for c in df.columns]

        pid_col = next((c for c in df.columns if "parasoft" in c and "message" in c), None)
        just_col = next((c for c in df.columns if "justifiable" in c), None)

        if not pid_col or not just_col:
            continue

        for _, r in df.iterrows():
            if pd.notna(r[pid_col]):
                mapping[str(r[pid_col]).strip()] = str(r[just_col]).strip()

    return mapping


# -------------------------------------------------
# 3. Resolve Yes / No / Analyse
# -------------------------------------------------
def resolve_justifiable(violation_id, mapping):
    val = mapping.get(violation_id)

    if val is None or str(val).strip() == "" or str(val).lower() == "nan":
        return "Analyse"

    return "Yes" if str(val).strip().lower() == "yes" else "No"


# -------------------------------------------------
# 4. Generate AI prompt file ✅
# -------------------------------------------------
def generate_ai_prompts(df, output_file="ai_prompts.txt"):
    with open(output_file, "w", encoding="utf-8") as f:
        for _, r in df.iterrows():

            if r["Justifiable"] == "Yes":
                f.write(
                    f"SKIP FIX (JUSTIFIED)\n"
                    f"Violation: {r['Violation']}\n"
                    f"Rule ID: {r['Violation ID']}\n"
                    f"File: {r['File']}:{r['Line number']}\n"
                    f"Action: No code change required.\n"
                    f"{'-'*50}\n\n"
                )
            else:
                f.write(
                    f"FIX REQUIRED\n"
                    f"Violation: {r['Violation']}\n"
                    f"Rule ID: {r['Violation ID']}\n"
                    f"File: {r['File']}:{r['Line number']}\n"
                    f"Justifiable: {r['Justifiable']}\n\n"
                    f"Task:\n"
                    f"- Explain the issue\n"
                    f"- Provide MISRA/CERT compliant fix\n"
                    f"- Do NOT suppress the warning\n"
                    f"{'-'*50}\n\n"
                )

    print(f"✅ AI prompt file generated: {output_file}")


# -------------------------------------------------
# 5. Generate Excel + prompts
# -------------------------------------------------
def generate_excel(records, output_excel, qorix_excel):
    df = pd.DataFrame(records)
    mapping = load_justifiable_mapping(qorix_excel)

    df["Justifiable"] = df["Violation ID"].apply(
        lambda x: resolve_justifiable(x, mapping)
    )

    detailed_df = df[
        ["Violation", "Violation ID", "File", "Line number", "Justifiable"]
    ].sort_values(by=["File", "Line number"])

    counter = Counter(zip(df["Violation"], df["Violation ID"]))
    unique_df = pd.DataFrame(
        [{"Violation": v, "Violation ID": r, "Violation Count": c}
         for (v, r), c in counter.items()]
    )

    with pd.ExcelWriter(output_excel, engine="openpyxl") as writer:
        unique_df.to_excel(writer, "Unique Violations", index=False)
        detailed_df.to_excel(writer, "Detailed Violations", index=False)

    # ✅ THIS WAS MISSING BEFORE
    generate_ai_prompts(detailed_df)


# -------------------------------------------------
# 6. Main
# -------------------------------------------------
def main():
    if len(sys.argv) != 3:
        print("Usage: python Tool_3.py report.html Qorix_CP_Common_Deviations.xlsx")
        sys.exit(1)

    report = Path(sys.argv[1])
    qorix = Path(sys.argv[2])
    output_excel = report.with_name(report.stem + "_output.xlsx")

    records = parse_parasoft_report(report)
    if not records:
        print("No violations found.")
        return

    generate_excel(records, output_excel, qorix)
    print(f"✅ Excel generated: {output_excel}")


if __name__ == "__main__":
    main()