"""
Feedback GUI - Interactive interface for providing feedback on violations
Enables users to mark false positives/negatives for machine learning

Version: 1.0.0
Developer: Himanshu R
Date: April 30, 2026
"""

import tkinter as tk
from tkinter import ttk, messagebox, scrolledtext, filedialog
from pathlib import Path
import json
import sys

# Configure UTF-8 encoding for Windows console
if sys.platform == 'win32':
    try:
        import ctypes
        kernel32 = ctypes.windll.kernel32
        kernel32.SetConsoleOutputCP(65001)
        sys.stdout.reconfigure(encoding='utf-8', errors='replace')
        sys.stderr.reconfigure(encoding='utf-8', errors='replace')
    except:
        pass

from FeedbackLearner import FeedbackLearner, FeedbackType


class FeedbackGUI:
    """GUI for providing feedback on MISRA/CERT violations"""
    
    def __init__(self, root, project_root: Path):
        self.root = root
        self.root.title("🎯 Violation Feedback System - Machine Learning")
        self.root.geometry("1200x800")
        
        self.project_root = Path(project_root)
        self.feedback_learner = FeedbackLearner(self.project_root)
        
        # Load violations from knowledge base or recent analysis
        self.violations = []
        self.current_violation_index = 0
        
        self.setup_ui()
        self.load_violations()
    
    def setup_ui(self):
        """Setup the user interface"""
        # Main container
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)
        main_frame.columnconfigure(0, weight=1)
        main_frame.rowconfigure(1, weight=1)
        
        # Title
        title_label = ttk.Label(
            main_frame,
            text="🎯 Violation Feedback System",
            font=('Arial', 16, 'bold')
        )
        title_label.grid(row=0, column=0, pady=(0, 10), sticky=tk.W)
        
        # Stats panel
        stats_frame = ttk.LabelFrame(main_frame, text="📊 Learning Statistics", padding="10")
        stats_frame.grid(row=0, column=1, columnspan=2, pady=(0, 10), padx=(10, 0), sticky=tk.E)
        
        self.stats_text = tk.StringVar(value="Loading...")
        stats_label = ttk.Label(stats_frame, textvariable=self.stats_text, font=('Arial', 9))
        stats_label.pack()
        
        # Navigation frame
        nav_frame = ttk.Frame(main_frame)
        nav_frame.grid(row=1, column=0, columnspan=3, sticky=(tk.W, tk.E), pady=(0, 10))
        
        ttk.Button(nav_frame, text="⬅️ Previous", command=self.prev_violation).pack(side=tk.LEFT, padx=(0, 5))
        
        self.violation_counter = tk.StringVar(value="0 / 0")
        ttk.Label(nav_frame, textvariable=self.violation_counter, font=('Arial', 10, 'bold')).pack(side=tk.LEFT, padx=10)
        
        ttk.Button(nav_frame, text="Next ➡️", command=self.next_violation).pack(side=tk.LEFT, padx=(5, 0))
        
        ttk.Button(nav_frame, text="📂 Load Violations", command=self.load_violations_from_file).pack(side=tk.RIGHT)
        ttk.Button(nav_frame, text="💾 Export Learning Data", command=self.export_learning).pack(side=tk.RIGHT, padx=(0, 10))
        
        # Violation details panel
        violation_frame = ttk.LabelFrame(main_frame, text="Violation Details", padding="10")
        violation_frame.grid(row=2, column=0, columnspan=3, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 10))
        violation_frame.columnconfigure(0, weight=1)
        violation_frame.rowconfigure(2, weight=1)
        
        # Violation info
        info_frame = ttk.Frame(violation_frame)
        info_frame.grid(row=0, column=0, sticky=(tk.W, tk.E), pady=(0, 10))
        
        self.rule_id_var = tk.StringVar(value="Rule ID: -")
        ttk.Label(info_frame, textvariable=self.rule_id_var, font=('Arial', 11, 'bold')).pack(side=tk.LEFT, padx=(0, 20))
        
        self.severity_var = tk.StringVar(value="Severity: -")
        ttk.Label(info_frame, textvariable=self.severity_var, font=('Arial', 10)).pack(side=tk.LEFT, padx=(0, 20))
        
        self.file_var = tk.StringVar(value="File: -")
        ttk.Label(info_frame, textvariable=self.file_var, font=('Arial', 10)).pack(side=tk.LEFT)
        
        # Description
        desc_label = ttk.Label(violation_frame, text="Description:", font=('Arial', 10, 'bold'))
        desc_label.grid(row=1, column=0, sticky=tk.W, pady=(0, 5))
        
        self.desc_text = tk.StringVar(value="-")
        desc_value = ttk.Label(violation_frame, textvariable=self.desc_text, wraplength=1100, font=('Arial', 10))
        desc_value.grid(row=1, column=0, sticky=tk.W, padx=(20, 0), pady=(0, 10))
        
        # Code snippet
        code_label = ttk.Label(violation_frame, text="Code Snippet:", font=('Arial', 10, 'bold'))
        code_label.grid(row=2, column=0, sticky=(tk.W, tk.N), pady=(0, 5))
        
        self.code_text = scrolledtext.ScrolledText(
            violation_frame,
            wrap=tk.WORD,
            font=('Courier New', 10),
            height=15,
            bg='#F5F5F5'
        )
        self.code_text.grid(row=3, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 10))
        
        # Feedback panel
        feedback_frame = ttk.LabelFrame(main_frame, text="Provide Feedback", padding="10")
        feedback_frame.grid(row=3, column=0, columnspan=3, sticky=(tk.W, tk.E), pady=(0, 10))
        
        # Feedback type buttons
        button_frame = ttk.Frame(feedback_frame)
        button_frame.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Button(
            button_frame,
            text="✅ Correct Detection",
            command=self.mark_correct,
            width=25
        ).pack(side=tk.LEFT, padx=(0, 10))
        
        ttk.Button(
            button_frame,
            text="❌ False Positive",
            command=self.mark_false_positive,
            width=25
        ).pack(side=tk.LEFT, padx=(0, 10))
        
        ttk.Button(
            button_frame,
            text="⚠️ Add False Negative",
            command=self.add_false_negative_dialog,
            width=25
        ).pack(side=tk.LEFT)
        
        # Reason input
        reason_label = ttk.Label(feedback_frame, text="Reason:", font=('Arial', 10))
        reason_label.pack(anchor=tk.W, pady=(0, 5))
        
        self.reason_text = scrolledtext.ScrolledText(
            feedback_frame,
            wrap=tk.WORD,
            font=('Arial', 10),
            height=3
        )
        self.reason_text.pack(fill=tk.X, pady=(0, 10))
        
        # Log panel
        log_frame = ttk.LabelFrame(main_frame, text="📝 Activity Log", padding="10")
        log_frame.grid(row=4, column=0, columnspan=3, sticky=(tk.W, tk.E), pady=(0, 10))
        
        self.log_text = scrolledtext.ScrolledText(
            log_frame,
            wrap=tk.WORD,
            font=('Courier New', 9),
            height=6,
            bg='#FAFAFA'
        )
        self.log_text.pack(fill=tk.BOTH, expand=True)
        
        self.update_stats()
    
    def load_violations(self):
        """Load violations from knowledge base"""
        kb_dir = self.project_root / "knowledge_base"
        if not kb_dir.exists():
            self.log("⚠️ No knowledge base found. Use 'Load Violations' button.")
            return
        
        # Find most recent knowledge base file
        kb_files = list(kb_dir.glob("*_KnowledgeDatabase.json"))
        if not kb_files:
            self.log("⚠️ No knowledge base files found")
            return
        
        # Load from most recent file
        latest_kb = max(kb_files, key=lambda p: p.stat().st_mtime)
        self.load_violations_from_path(latest_kb)
    
    def load_violations_from_file(self):
        """Load violations from selected file"""
        file_path = filedialog.askopenfilename(
            title="Select Knowledge Base File",
            filetypes=[("JSON files", "*.json"), ("All files", "*.*")],
            initialdir=self.project_root / "knowledge_base"
        )
        
        if file_path:
            self.load_violations_from_path(Path(file_path))
    
    def load_violations_from_path(self, file_path: Path):
        """Load violations from specified path"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                kb_data = json.load(f)
            
            # Extract violations
            self.violations = []
            violations_dict = kb_data.get('violations', {})
            
            for vid, v in violations_dict.items():
                self.violations.append(v)
            
            self.current_violation_index = 0
            self.violation_counter.set(f"1 / {len(self.violations)}")
            self.display_current_violation()
            self.log(f"✅ Loaded {len(self.violations)} violations from {file_path.name}")
            
        except Exception as e:
            messagebox.showerror("Error", f"Could not load violations:\n{str(e)}")
            self.log(f"❌ Error loading: {str(e)}")
    
    def display_current_violation(self):
        """Display current violation details"""
        if not self.violations or self.current_violation_index >= len(self.violations):
            self.rule_id_var.set("Rule ID: No violations loaded")
            self.severity_var.set("Severity: -")
            self.file_var.set("File: -")
            self.desc_text.set("-")
            self.code_text.delete('1.0', tk.END)
            return
        
        v = self.violations[self.current_violation_index]
        
        self.rule_id_var.set(f"Rule ID: {v.get('violation_id', 'UNKNOWN')}")
        self.severity_var.set(f"Severity: {v.get('severity', 'UNKNOWN')}")
        
        files = v.get('files_affected', [])
        file_name = files[0] if files else "Unknown"
        if isinstance(file_name, dict):
            file_name = file_name.get('file', 'Unknown')
        self.file_var.set(f"File: {file_name}")
        
        self.desc_text.set(v.get('violation_text', 'No description'))
        
        # Display code snippet
        self.code_text.delete('1.0', tk.END)
        code_snippet = v.get('code_snippet_multiline', v.get('code_snippet', 'No code snippet'))
        self.code_text.insert('1.0', code_snippet)
        self.code_text.config(state='disabled')
    
    def next_violation(self):
        """Navigate to next violation"""
        if self.violations and self.current_violation_index < len(self.violations) - 1:
            self.current_violation_index += 1
            self.violation_counter.set(f"{self.current_violation_index + 1} / {len(self.violations)}")
            self.display_current_violation()
            self.reason_text.delete('1.0', tk.END)
    
    def prev_violation(self):
        """Navigate to previous violation"""
        if self.violations and self.current_violation_index > 0:
            self.current_violation_index -= 1
            self.violation_counter.set(f"{self.current_violation_index + 1} / {len(self.violations)}")
            self.display_current_violation()
            self.reason_text.delete('1.0', tk.END)
    
    def mark_correct(self):
        """Mark current violation as correct detection"""
        if not self.violations:
            return
        
        reason = self.reason_text.get('1.0', tk.END).strip() or "Verified as correct detection"
        v = self.violations[self.current_violation_index]
        
        self.feedback_learner.add_feedback(
            feedback_type=FeedbackType.CORRECT_DETECTION,
            rule_id=v.get('violation_id', 'UNKNOWN'),
            code_snippet=v.get('code_snippet', ''),
            file_path=v.get('full_path', 'unknown'),
            line_number=v.get('line', 0),
            reason=reason,
            module_name=v.get('module_name', '')
        )
        
        self.log(f"✅ Marked as CORRECT: {v.get('violation_id', 'UNKNOWN')}")
        self.update_stats()
        self.next_violation()
    
    def mark_false_positive(self):
        """Mark current violation as false positive"""
        if not self.violations:
            return
        
        reason = self.reason_text.get('1.0', tk.END).strip()
        if not reason:
            messagebox.showwarning("Missing Reason", "Please provide a reason why this is a false positive")
            return
        
        v = self.violations[self.current_violation_index]
        
        self.feedback_learner.add_feedback(
            feedback_type=FeedbackType.FALSE_POSITIVE,
            rule_id=v.get('violation_id', 'UNKNOWN'),
            code_snippet=v.get('code_snippet', ''),
            file_path=v.get('full_path', 'unknown'),
            line_number=v.get('line', 0),
            reason=reason,
            module_name=v.get('module_name', '')
        )
        
        self.log(f"❌ Marked as FALSE POSITIVE: {v.get('violation_id', 'UNKNOWN')} - {reason}")
        self.update_stats()
        self.next_violation()
    
    def add_false_negative_dialog(self):
        """Dialog to add a false negative (missed violation)"""
        dialog = tk.Toplevel(self.root)
        dialog.title("Add False Negative")
        dialog.geometry("700x500")
        
        ttk.Label(dialog, text="Report a Missed Violation", font=('Arial', 14, 'bold')).pack(pady=10)
        
        # Rule ID
        ttk.Label(dialog, text="Rule ID (e.g., CERT_ARR30, MISRA_10_3):").pack(anchor=tk.W, padx=10, pady=(10, 0))
        rule_entry = ttk.Entry(dialog, width=50)
        rule_entry.pack(padx=10, pady=5, fill=tk.X)
        
        # File path
        ttk.Label(dialog, text="File Path:").pack(anchor=tk.W, padx=10, pady=(10, 0))
        file_entry = ttk.Entry(dialog, width=50)
        file_entry.pack(padx=10, pady=5, fill=tk.X)
        
        # Line number
        ttk.Label(dialog, text="Line Number:").pack(anchor=tk.W, padx=10, pady=(10, 0))
        line_entry = ttk.Entry(dialog, width=50)
        line_entry.pack(padx=10, pady=5, fill=tk.X)
        
        # Code snippet
        ttk.Label(dialog, text="Code Snippet:").pack(anchor=tk.W, padx=10, pady=(10, 0))
        code_text = scrolledtext.ScrolledText(dialog, height=8, width=70)
        code_text.pack(padx=10, pady=5)
        
        # Reason
        ttk.Label(dialog, text="Reason (Why should this have been detected?):").pack(anchor=tk.W, padx=10, pady=(10, 0))
        reason_text = scrolledtext.ScrolledText(dialog, height=4, width=70)
        reason_text.pack(padx=10, pady=5)
        
        def submit_fn():
            rule_id = rule_entry.get().strip()
            file_path = file_entry.get().strip()
            line_num = line_entry.get().strip()
            code = code_text.get('1.0', tk.END).strip()
            reason = reason_text.get('1.0', tk.END).strip()
            
            if not all([rule_id, file_path, line_num, code, reason]):
                messagebox.showwarning("Incomplete", "Please fill all fields")
                return
            
            try:
                line_num = int(line_num)
            except ValueError:
                messagebox.showerror("Invalid", "Line number must be an integer")
                return
            
            self.feedback_learner.add_feedback(
                feedback_type=FeedbackType.FALSE_NEGATIVE,
                rule_id=rule_id,
                code_snippet=code,
                file_path=file_path,
                line_number=line_num,
                reason=reason,
                module_name=Path(file_path).stem
            )
            
            self.log(f"⚠️ Added FALSE NEGATIVE: {rule_id} at {file_path}:{line_num}")
            self.update_stats()
            dialog.destroy()
        
        ttk.Button(dialog, text="Submit", command=submit_fn).pack(pady=10)
    
    def update_stats(self):
        """Update statistics display"""
        stats = self.feedback_learner.feedback_db['statistics']
        total = self.feedback_learner.feedback_db['metadata']['total_feedback']
        
        stats_str = f"Total: {total} | ✅ Correct: {stats.get('correct', 0)} | ❌ FP: {stats.get('false_positive', 0)} | ⚠️ FN: {stats.get('false_negative', 0)}"
        self.stats_text.set(stats_str)
    
    def export_learning(self):
        """Export learning data"""
        exported = self.feedback_learner.export_learning_data()
        
        msg = "Learning data exported:\n\n"
        for key, path in exported.items():
            msg += f"{key}: {path.name}\n"
        
        messagebox.showinfo("Export Complete", msg)
        self.log(f"💾 Exported learning data to {len(exported)} files")
    
    def log(self, message: str):
        """Log message to activity log"""
        self.log_text.insert(tk.END, f"[{self.get_timestamp()}] {message}\n")
        self.log_text.see(tk.END)
    
    def get_timestamp(self):
        """Get current timestamp"""
        from datetime import datetime
        return datetime.now().strftime("%H:%M:%S")


def main():
    """Main entry point"""
    project_root = Path(__file__).parent.parent
    
    root = tk.Tk()
    app = FeedbackGUI(root, project_root)
    root.mainloop()


if __name__ == "__main__":
    main()
