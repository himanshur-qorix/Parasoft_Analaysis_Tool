import sys
from pathlib import Path
from bs4 import BeautifulSoup
import pandas as pd
from collections import Counter


# -------------------------------------------------
# 1. Parse Parasoft HTML
# -------------------------------------------------
def parse_parasoft_report(report_path):
    """
    Parse Parasoft HTML report
    Detects report type and calls appropriate parser
    """
    with open(report_path, "r", encoding="utf-8", errors="ignore") as f:
        content = f.read()
        soup = BeautifulSoup(content, "lxml")
    
    # Detect report type
    if "MISRA-C / CERT-C Static Analysis Report" in content or "MISRA / CERT Static Analysis Report" in content:
        print("[INFO] Detected MISRA/CERT report format")
        return parse_misra_cert_report(report_path, soup)
    else:
        print("[INFO] Detected Parasoft report format")
        return parse_parasoft_html(soup)


def parse_parasoft_html(soup):
    """Parse standard Parasoft HTML report"""
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


def parse_misra_cert_report(report_path, soup=None):
    """
    Parse MISRA/CERT HTML report
    Format: Standard | Rule ID | Severity | Category | File | Line | Message | Snippet
    """
    if soup is None:
        with open(report_path, "r", encoding="utf-8", errors="ignore") as f:
            soup = BeautifulSoup(f, "lxml")
    
    records = []
    table = soup.find("table")
    
    if not table:
        print("[WARNING] No table found in MISRA/CERT report")
        return records
    
    tbody = table.find("tbody")
    if not tbody:
        tbody = table
    
    rows = tbody.find_all("tr")
    
    for row in rows:
        tds = row.find_all("td")
        
        # Skip if not enough columns or "No violations" message
        if len(tds) < 7:
            continue
        
        try:
            # Extract data from columns
            # Standard | Rule ID | Severity | Category | File | Line | Message | Snippet
            standard = tds[0].get_text(strip=True)
            rule_id = tds[1].get_text(strip=True)
            severity = tds[2].get_text(strip=True)
            category = tds[3].get_text(strip=True)
            file_name = tds[4].get_text(strip=True)
            line_str = tds[5].get_text(strip=True)
            message = tds[6].get_text(strip=True)
            
            # Skip empty rows
            if not rule_id or not file_name or not line_str:
                continue
            
            # Parse line number
            try:
                line_num = int(line_str)
            except ValueError:
                continue
            
            # Create violation message in Parasoft-like format
            violation_msg = f"{message} [{category}]"
            
            records.append({
                "Violation": violation_msg,
                "Violation ID": rule_id,
                "File": file_name,
                "Line number": line_num
            })
            
        except Exception as e:
            # Skip rows that can't be parsed
            continue
    
    print(f"[INFO] Parsed {len(records)} violations from MISRA/CERT report")
    return records


# -------------------------------------------------
# 2. Load Qorix deviations (CERT & MISRA only)
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
# 4. Generate AI prompt file
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

    print(f"[OK] AI prompt file generated: {output_file}")


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

    generate_ai_prompts(detailed_df)


# -------------------------------------------------
# 6. Main
# -------------------------------------------------
def main():

    if len(sys.argv) < 2:
        print("Usage: python ParasoftAnalysisTool.py <report.html> [Qorix_CP_Common_Deviations.xlsx]")
        sys.exit(1)

    report = Path(sys.argv[1])
    output_excel = report.with_name(report.stem + "_output.xlsx")

    records = parse_parasoft_report(report)
    if not records:
        print("No violations found.")
        return

    if len(sys.argv) >= 3:
        qorix = Path(sys.argv[2])
        generate_excel(records, output_excel, qorix)
    else:
        # Generate Excel without justifiable mapping
        df = pd.DataFrame(records)
        detailed_df = df[["Violation", "Violation ID", "File", "Line number"]].sort_values(by=["File", "Line number"])
        counter = Counter(zip(df["Violation"], df["Violation ID"]))
        unique_df = pd.DataFrame(
            [{"Violation": v, "Violation ID": r, "Violation Count": c}
             for (v, r), c in counter.items()]
        )
        with pd.ExcelWriter(output_excel, engine="openpyxl") as writer:
            unique_df.to_excel(writer, "Unique Violations", index=False)
            detailed_df.to_excel(writer, "Detailed Violations", index=False)
        print(f"[OK] Excel generated (without justifiable mapping): {output_excel}")


if __name__ == "__main__":
    main()