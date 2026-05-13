"""
Parasoft Analysis Tool - GUI Application
Version: 4.0.0
Developer: Himanshu R
Organization: Qorix India Pvt Ltd

Simple and user-friendly interface for Parasoft static analysis workflows
"""

import tkinter as tk
from tkinter import ttk, scrolledtext, filedialog, messagebox
import subprocess
import threading
import time
import os
import sys
import re
import json
import logging
from pathlib import Path
from datetime import datetime

# Configure logging to show INFO level messages
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    handlers=[
        logging.StreamHandler(sys.stdout)
    ]
)

# Optional PIL support for logo images
try:
    from PIL import Image, ImageTk
    PIL_AVAILABLE = True
except ImportError:
    PIL_AVAILABLE = False


class ParasoftGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Parasoft Analysis Tool v4.0.0 - GUI")
        self.root.geometry("1000x750")
        self.root.resizable(True, True)
        
        # Set up logger for this class
        self.logger = logging.getLogger('ParasoftGUI')
        
        # Set project root directory (parent of src)
        self.project_root = Path(__file__).parent.parent.absolute()
        self.scripts_dir = self.project_root / "batch_support"
        
        # Configure style
        self.setup_styles()
        
        # Create menu bar
        self.create_menu_bar()
        
        # Create main layout
        self.create_widgets()
        
        # Track running process
        self.running_process = None
        
        # Run initialization checks after GUI is ready
        self.root.after(500, self.run_startup_checks)
        
        # Load available Ollama models after a brief delay
        self.root.after(1500, self.refresh_ollama_models)
        
    def setup_styles(self):
        """Configure ttk styles"""
        style = ttk.Style()
        style.theme_use('clam')
        
        # Configure colors
        style.configure('Title.TLabel', font=('Arial', 16, 'bold'), foreground='#2C3E50')
        style.configure('Section.TLabel', font=('Arial', 12, 'bold'), foreground='#34495E')
        style.configure('Action.TButton', font=('Arial', 10), padding=10)
        style.configure('Primary.TButton', font=('Arial', 11, 'bold'), padding=12)
    
    def create_menu_bar(self):
        """Create menu bar with File, Tools, and Help menus"""
        menubar = tk.Menu(self.root)
        self.root.config(menu=menubar)
        
        # File menu
        file_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="File", menu=file_menu)
        file_menu.add_command(label="Exit", command=self.root.quit)
        
        # Tools menu
        tools_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="Tools", menu=tools_menu)
        tools_menu.add_command(label="🔍 Check Knowledge Base", command=self.check_knowledge_base)
        tools_menu.add_command(label="✨ Enrich Violations", command=self.enrich_violations_dialog)
        tools_menu.add_separator()
        tools_menu.add_command(label="🔄 Refresh Ollama Models", command=self.refresh_ollama_models)
        
        # Help menu
        help_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="Help", menu=help_menu)
        help_menu.add_command(label="📖 Documentation", command=self.show_documentation)
        help_menu.add_command(label="ℹ️ About", command=self.show_about)
    
    def check_knowledge_base(self):
        """Check knowledge base for enrichment status"""
        module_name = self.module_entry.get().strip()
        if not module_name:
            messagebox.showwarning("Module Required", "Please enter a module name first")
            return
        
        kb_path = self.project_root / "knowledge_base" / f"{module_name}_KnowledgeDatabase.json"
        if not kb_path.exists():
            messagebox.showerror("Knowledge Base Not Found", 
                               f"Knowledge base for {module_name} not found.\n\n"
                               "Please run 'Run Complete Analysis' first.")
            return
        
        # Check KB status
        try:
            with open(kb_path, 'r', encoding='utf-8') as f:
                kb_data = json.load(f)
            
            violations = kb_data.get('violations', {})
            if not violations:
                messagebox.showinfo("Empty Knowledge Base", 
                                  "Knowledge base has no violations.")
                return
            
            total = len(violations)
            enriched = sum(1 for v in violations.values() if v.get('enriched'))
            needs_enrichment = sum(1 for v in violations.values() 
                                 if not v.get('code_snippet') or v.get('file_path') in ['UNKNOWN', 'N/A', ''])
            
            percentage = round(enriched / total * 100, 1) if total > 0 else 0
            
            message = f"Knowledge Base Status for {module_name}:\n\n"
            message += f"Total Violations: {total}\n"
            message += f"✅ Enriched: {enriched} ({percentage}%)\n"
            message += f"⚠️  Needs Enrichment: {needs_enrichment}\n\n"
            
            if needs_enrichment > 0:
                message += "💡 Tip: Use 'Tools > Enrich Violations' to add source code context"
            else:
                message += "✅ All violations are enriched and ready!"
            
            messagebox.showinfo("Knowledge Base Status", message)
        
        except Exception as e:
            messagebox.showerror("Error", f"Failed to check knowledge base:\n{str(e)}")
    
    def enrich_violations_dialog(self):
        """Show dialog to enrich violations with source code"""
        module_name = self.module_entry.get().strip()
        if not module_name:
            messagebox.showwarning("Module Required", "Please enter a module name first")
            return
        
        kb_path = self.project_root / "knowledge_base" / f"{module_name}_KnowledgeDatabase.json"
        if not kb_path.exists():
            messagebox.showerror("Knowledge Base Not Found", 
                               f"Knowledge base for {module_name} not found.\n\n"
                               "Please run 'Run Complete Analysis' first.")
            return
        
        # Get source code path
        source_path = self.input_path_entry.get().strip()
        if not source_path:
            messagebox.showinfo("Source Path Required", 
                              "Please enter or browse to the source code directory first.\n\n"
                              "The enricher needs to find your actual .c/.cpp/.h files.")
            return
        
        if not Path(source_path).exists():
            messagebox.showerror("Invalid Path", f"Source path does not exist:\n{source_path}")
            return
        
        # Confirm enrichment
        response = messagebox.askyesno(
            "Enrich Violations",
            f"Enrich violations in {module_name} with source code from:\n"
            f"{source_path}\n\n"
            "This will:\n"
            "• Find source files\n"
            "• Read code at violation lines\n"
            "• Add code snippets and context\n\n"
            "Continue?"
        )
        
        if not response:
            return
        
        # Run enrichment
        self.run_enrichment(module_name, source_path)
    
    def run_enrichment(self, module_name, source_path):
        """Run violation enrichment"""
        # Create progress dialog
        progress_dialog = tk.Toplevel(self.root)
        progress_dialog.title("Enriching Violations")
        progress_dialog.geometry("500x300")
        progress_dialog.transient(self.root)
        progress_dialog.grab_set()
        
        ttk.Label(progress_dialog, text="🔍 Enriching Violations", 
                 font=('Arial', 14, 'bold')).pack(pady=10)
        
        output_text = scrolledtext.ScrolledText(progress_dialog, height=12, wrap=tk.WORD)
        output_text.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        button_frame = ttk.Frame(progress_dialog)
        button_frame.pack(fill=tk.X, padx=10, pady=5)
        
        close_button = ttk.Button(button_frame, text="Close", command=progress_dialog.destroy)
        close_button.pack(side=tk.RIGHT)
        close_button.config(state='disabled')
        
        def run_enrichment_thread():
            try:
                output_text.insert(tk.END, f"Enriching violations for {module_name}...\n")
                output_text.insert(tk.END, f"Source path: {source_path}\n\n")
                output_text.see(tk.END)
                
                # Import and run enricher
                sys.path.insert(0, str(self.project_root / "src"))
                from ViolationEnricher import enrich_knowledge_base_violations
                
                kb_path = self.project_root / "knowledge_base" / f"{module_name}_KnowledgeDatabase.json"
                
                output_text.insert(tk.END, "Building file index...\n")
                output_text.see(tk.END)
                
                count = enrich_knowledge_base_violations(kb_path, Path(source_path))
                
                output_text.insert(tk.END, f"\n✅ Enriched {count} violations!\n")
                output_text.insert(tk.END, "\nYou can now:\n")
                output_text.insert(tk.END, "• View violations with code snippets\n")
                output_text.insert(tk.END, "• Generate better AI-powered fixes\n")
                output_text.see(tk.END)
                
                progress_dialog.after(0, lambda: close_button.config(state='normal'))
                
                messagebox.showinfo("Enrichment Complete", 
                                  f"Successfully enriched {count} violations!\n\n"
                                  "Violations now have source code context.")
            
            except Exception as e:
                output_text.insert(tk.END, f"\n❌ Error: {str(e)}\n")
                output_text.see(tk.END)
                progress_dialog.after(0, lambda: close_button.config(state='normal'))
                messagebox.showerror("Enrichment Failed", f"Error:\n{str(e)}")
        
        thread = threading.Thread(target=run_enrichment_thread, daemon=True)
        thread.start()
    
    def show_documentation(self):
        """Show link to documentation"""
        messagebox.showinfo("Documentation", 
                          "Documentation available in docs/ folder:\n\n"
                          "• AI_CODE_FIXER_GUIDE.md\n"
                          "• VIOLATION_ENRICHMENT_GUIDE.md\n"
                          "• GUI_GUIDE.md\n"
                          "• And more...")
    
    def show_about(self):
        """Show about dialog"""
        messagebox.showinfo("About", 
                          "Parasoft Analysis Tool v4.1.0\n\n"
                          "AI-powered static analysis and code fixing\n\n"
                          "Developer: Himanshu R\n"
                          "Organization: Qorix India Pvt Ltd\n\n"
                          "© 2026 Qorix India Pvt Ltd")
        
    def create_widgets(self):
        """Create all GUI widgets"""
        
        # Header Frame with Logo
        header_frame = ttk.Frame(self.root, padding="10")
        header_frame.pack(fill=tk.X)
        
        # Try to load and display Qorix logo
        try:
            logo_path = self.project_root / "assets" / "logo.svg"
            if logo_path.exists():
                # For SVG, we'll display a placeholder or convert
                # Since PIL doesn't support SVG directly, we'll show text
                logo_label = ttk.Label(
                    header_frame,
                    text="🔧 QORIX",
                    font=('Arial', 20, 'bold'),
                    foreground='#2C3E50'
                )
                logo_label.pack()
            else:
                # Fallback to text logo
                logo_label = ttk.Label(
                    header_frame,
                    text="🔧 QORIX",
                    font=('Arial', 20, 'bold'),
                    foreground='#2C3E50'
                )
                logo_label.pack()
        except Exception:
            # If logo loading fails, use text
            logo_label = ttk.Label(
                header_frame,
                text="🔧 QORIX",
                font=('Arial', 20, 'bold'),
                foreground='#2C3E50'
            )
            logo_label.pack()
        
        title_label = ttk.Label(
            header_frame,
            text="Parasoft Analysis Tool v4.0.0",
            style='Title.TLabel'
        )
        title_label.pack()
        
        subtitle_label = ttk.Label(
            header_frame,
            text="Qorix India Pvt Ltd | GUI-Enabled Platform",
            font=('Arial', 9)
        )
        subtitle_label.pack()
        
        # Separator
        ttk.Separator(self.root, orient='horizontal').pack(fill=tk.X, pady=5)
        
        # Main content area with notebook (tabs)
        self.notebook = ttk.Notebook(self.root)
        self.notebook.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
        
        # Create tabs
        self.create_main_tab()
        self.create_advanced_tab()
        self.create_ai_chat_tab()
        self.create_settings_tab()
        self.create_help_tab()
        
        # Status bar
        self.create_status_bar()
        
    def create_main_tab(self):
        """Create main operations tab"""
        main_frame = ttk.Frame(self.notebook, padding="10")
        self.notebook.add(main_frame, text="📋 Main Operations")
        
        # Left panel - Controls
        left_panel = ttk.Frame(main_frame)
        left_panel.pack(side=tk.LEFT, fill=tk.BOTH, expand=False, padx=(0, 10))
        
        # Module input section
        input_section = ttk.LabelFrame(left_panel, text="Module Configuration", padding="10")
        input_section.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Label(input_section, text="Module Name:").pack(anchor=tk.W)
        self.module_entry = ttk.Entry(input_section, width=30, font=('Arial', 10))
        self.module_entry.pack(fill=tk.X, pady=(5, 5))
        # Leave empty - user will enter module name
        
        ttk.Label(input_section, text="(e.g., Mka, EthIf, etc.)", 
                 font=('Arial', 8), foreground='gray').pack(anchor=tk.W)
        
        # Input path section
        ttk.Label(input_section, text="Input Path:", font=('Arial', 10, 'bold')).pack(anchor=tk.W, pady=(10, 0))
        
        path_frame = ttk.Frame(input_section)
        path_frame.pack(fill=tk.X, pady=(5, 5))
        
        self.input_path_entry = ttk.Entry(path_frame, font=('Arial', 9))
        self.input_path_entry.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 5))
        # Leave empty - user will browse or enter path
        
        ttk.Button(
            path_frame,
            text="📁 Browse",
            command=self.browse_input_path,
            width=10
        ).pack(side=tk.RIGHT)
        
        ttk.Label(input_section, text="(Source code directory for analysis)", 
                 font=('Arial', 8), foreground='gray').pack(anchor=tk.W)
        
        # Quick Actions section
        actions_section = ttk.LabelFrame(left_panel, text="Quick Actions", padding="10")
        actions_section.pack(fill=tk.BOTH, expand=True)
        
        # Define main operations - call Python scripts directly instead of batch files
        operations = [
            ("🚀 Run Complete Analysis", "run_agent", 
             "Run full analysis workflow", True),
            ("📊 Run Static Analyzer", "run_static_analyzer",
             "Analyze code with Parasoft/Polyspace", True),
            ("🛠️ Generate Code Fixes", "generate_code_fixes",
             "Generate AI-powered code fixes", True),
            ("📝 Apply Suppressions", "apply_suppressions",
             "Apply suppress comments to code", True),
            ("📄 Generate CERT/MISRA Report", "generate_cert_misra_report",
             "Create compliance report", True),
            ("🧠 Learn Justification Patterns", "learn_justification_patterns",
             "Train AI on justification patterns", False),
        ]
        
        for idx, (text, operation_name, tooltip, needs_module) in enumerate(operations):
            btn = ttk.Button(
                actions_section,
                text=text,
                command=lambda op=operation_name, nm=needs_module: self.run_operation(op, nm),
                style='Action.TButton'
            )
            btn.pack(fill=tk.X, pady=5)
            self.create_tooltip(btn, tooltip)
        
        # Right panel - Output
        right_panel = ttk.Frame(main_frame)
        right_panel.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
        
        output_label = ttk.Label(right_panel, text="📝 Output Log", style='Section.TLabel')
        output_label.pack(anchor=tk.W, pady=(0, 5))
        
        # Output text area
        self.output_text = scrolledtext.ScrolledText(
            right_panel,
            wrap=tk.WORD,
            font=('Consolas', 9),
            bg='#F8F9FA',
            fg='#2C3E50',
            relief=tk.SUNKEN,
            borderwidth=2
        )
        self.output_text.pack(fill=tk.BOTH, expand=True)
        
        # Control buttons
        control_frame = ttk.Frame(right_panel)
        control_frame.pack(fill=tk.X, pady=(10, 0))
        
        ttk.Button(
            control_frame,
            text="🗑️ Clear Log",
            command=self.clear_output
        ).pack(side=tk.LEFT, padx=5)
        
        ttk.Button(
            control_frame,
            text="💾 Save Log",
            command=self.save_log
        ).pack(side=tk.LEFT, padx=5)
        
        self.stop_btn = ttk.Button(
            control_frame,
            text="⏹️ Stop Process",
            command=self.stop_process,
            state='disabled'
        )
        self.stop_btn.pack(side=tk.RIGHT, padx=5)
        
    def create_advanced_tab(self):
        """Create advanced operations tab"""
        advanced_frame = ttk.Frame(self.notebook, padding="10")
        self.notebook.add(advanced_frame, text="⚙️ Advanced")
        
        # AI Mode Settings
        ai_section = ttk.LabelFrame(advanced_frame, text="AI Configuration", padding="10")
        ai_section.pack(fill=tk.X, pady=(0, 10))
        
        mode_frame = ttk.Frame(ai_section)
        mode_frame.pack(fill=tk.X, pady=5)
        
        ttk.Label(mode_frame, text="AI Mode:").pack(side=tk.LEFT, padx=(0, 10))
        
        self.ai_mode_var = tk.StringVar(value="hybrid")
        ai_mode_combo = ttk.Combobox(
            mode_frame,
            textvariable=self.ai_mode_var,
            values=["ai-only", "hybrid", "rules-only"],
            state="readonly",
            width=15
        )
        ai_mode_combo.pack(side=tk.LEFT)
        
        mode_info = ttk.Label(
            ai_section,
            text="• AI-Only: Use LLM for all analysis\n"
                 "• Hybrid: Combine rule-based + AI justifications (Recommended)\n"
                 "• Rules-Only: Use only pattern-based rules",
            foreground="gray",
            font=("Segoe UI", 8)
        )
        mode_info.pack(anchor=tk.W, padx=5, pady=(0, 5))
        
        # System Setup Section
        setup_section = ttk.LabelFrame(advanced_frame, text="System Setup", padding="10")
        setup_section.pack(fill=tk.X, pady=(0, 10))
        
        # Dependencies Installation
        deps_frame = ttk.Frame(setup_section)
        deps_frame.pack(fill=tk.X, pady=5)
        
        ttk.Button(
            deps_frame,
            text="📦 Install Python Dependencies",
            command=self.install_dependencies
        ).pack(side=tk.LEFT, padx=(0, 10))
        
        deps_info = ttk.Label(
            deps_frame,
            text="Install/Update packages from requirements.txt",
            foreground="gray",
            font=("Segoe UI", 9)
        )
        deps_info.pack(side=tk.LEFT)
        
        # Check Dependencies Status
        ttk.Button(
            setup_section,
            text="🔍 Check Dependencies Status",
            command=self.check_dependencies_status
        ).pack(fill=tk.X, pady=5)
        
        # Database operations
        db_section = ttk.LabelFrame(advanced_frame, text="Database Operations", padding="10")
        db_section.pack(fill=tk.X, pady=(0, 10))
        
        operations = [
            ("🏗️ Build Parasoft Rules Database", "build_parasoft_rules",
             "Build comprehensive rules database", False),
            ("📚 Consolidate Knowledge", "consolidate_knowledge",
             "Merge knowledge from multiple modules", False),
            ("🔍 Query Master Knowledge", "query_master_knowledge",
             "Search consolidated knowledge base", False),
        ]
        
        for text, operation, tooltip, needs_module in operations:
            btn = ttk.Button(
                db_section,
                text=text,
                command=lambda op=operation, nm=needs_module: self.run_operation(op, nm)
            )
            btn.pack(fill=tk.X, pady=5)
            self.create_tooltip(btn, tooltip)
        
        # File operations
        file_section = ttk.LabelFrame(advanced_frame, text="Quick Access", padding="10")
        file_section.pack(fill=tk.X, pady=(0, 10))
        
        browse_buttons = [
            ("📁 Open Reports Folder", "reports"),
            ("📁 Open Knowledge Base Folder", "knowledge_base"),
            ("📁 Open Fixes Folder", "fixes"),
            ("📁 Open Justifications Folder", "justifications"),
        ]
        
        for text, folder in browse_buttons:
            ttk.Button(
                file_section,
                text=text,
                command=lambda f=folder: self.open_folder(f)
            ).pack(fill=tk.X, pady=5)
        
    def create_settings_tab(self):
        """Create settings and info tab"""
        settings_frame = ttk.Frame(self.notebook, padding="10")
        self.notebook.add(settings_frame, text="ℹ️ About")
        
        # Info section
        info_text = """
Parasoft Analysis Tool - GUI Version 4.0.0

Developer: Himanshu R
Organization: Qorix India Pvt Ltd

What's New in v4.0.0:
• Graphical User Interface (GUI) - Simple point-and-click interface
• Reorganized project structure for better maintainability
• Batch files moved to batch_support/ folder
• Real-time output logging and process control
• Quick folder access and log management

Features:
• Automated static code analysis
• AI-powered code fix generation
• MISRA/CERT compliance reporting
• Knowledge base management
• Justification pattern learning
• Violation history tracking
• RAG learning system
• Comparison reports

Supported Standards:
• MISRA C:2012
• CERT C
• Parasoft Rules Database (1200+ rules)

System Requirements:
• Python 3.8 or higher
• Required packages (see requirements.txt)
• Parasoft/Polyspace analysis reports

Quick Start:
1. Enter your module name (e.g., Mka)
2. Select an operation from Main Operations
3. View output in the log window
4. Check generated files in respective folders

For detailed documentation, see the docs/ folder.
        """
        
        info_display = tk.Text(
            settings_frame,
            wrap=tk.WORD,
            font=('Arial', 10),
            bg='#FFFFFF',
            relief=tk.FLAT,
            padx=20,
            pady=20
        )
        info_display.pack(fill=tk.BOTH, expand=True)
        info_display.insert('1.0', info_text.strip())
        info_display.config(state='disabled')
        
        # Project info
        info_frame = ttk.Frame(settings_frame)
        info_frame.pack(fill=tk.X, pady=10)
        
        ttk.Label(
            info_frame,
            text=f"Project Root: {self.project_root}",
            font=('Arial', 8),
            foreground='gray'
        ).pack(anchor=tk.W)
    
    def create_ai_chat_tab(self):
        """Create interactive AI chat tab for Ollama/Qwen"""
        chat_frame = ttk.Frame(self.notebook, padding="10")
        self.notebook.add(chat_frame, text="🤖 AI Chat")
        
        # Top section - Model configuration
        config_frame = ttk.LabelFrame(chat_frame, text="Ollama Configuration", padding="10")
        config_frame.pack(fill=tk.X, pady=(0, 10))
        
        config_row = ttk.Frame(config_frame)
        config_row.pack(fill=tk.X)
        
        ttk.Label(config_row, text="Model:").pack(side=tk.LEFT, padx=(0, 5))
        
        # Dynamic model dropdown - will be populated with available models
        self.model_var = tk.StringVar(value="Loading...")
        self.model_combo = ttk.Combobox(
            config_row,
            textvariable=self.model_var,
            values=["Loading models..."],
            state="readonly",
            width=30
        )
        self.model_combo.pack(side=tk.LEFT, padx=(0, 10))
        
        ttk.Button(
            config_row,
            text="🔄 Refresh Models",
            command=self.refresh_ollama_models
        ).pack(side=tk.LEFT, padx=(0, 5))
        
        ttk.Button(
            config_row,
            text="📥 Install Models",
            command=self.show_install_models_dialog
        ).pack(side=tk.LEFT, padx=(0, 5))
        
        ttk.Button(
            config_row,
            text="✓ Test Connection",
            command=self.test_ollama_connection
        ).pack(side=tk.LEFT, padx=(0, 5))
        
        ttk.Button(
            config_row,
            text="🗑️ Clear Chat",
            command=self.clear_chat
        ).pack(side=tk.LEFT)
        
        self.ollama_status_label = ttk.Label(
            config_row,
            text="⚪ Not tested",
            font=('Arial', 9)
        )
        self.ollama_status_label.pack(side=tk.RIGHT, padx=(10, 0))
        
        # Chat history area
        chat_label = ttk.Label(chat_frame, text="💬 Chat History", font=('Arial', 11, 'bold'))
        chat_label.pack(anchor=tk.W, pady=(0, 5))
        
        self.chat_text = scrolledtext.ScrolledText(
            chat_frame,
            wrap=tk.WORD,
            font=('Consolas', 10),
            bg='#F8F9FA',
            fg='#2C3E50',
            relief=tk.SUNKEN,
            borderwidth=2,
            height=20
        )
        self.chat_text.pack(fill=tk.BOTH, expand=True, pady=(0, 10))
        self.chat_text.config(state='disabled')
        
        # Configure tags for formatting
        self.chat_text.tag_config('user', foreground='#2E86DE', font=('Consolas', 10, 'bold'))
        self.chat_text.tag_config('assistant', foreground='#10AC84', font=('Consolas', 10, 'bold'))
        self.chat_text.tag_config('error', foreground='#EE5A6F', font=('Consolas', 10, 'bold'))
        self.chat_text.tag_config('system', foreground='#95A5A6', font=('Consolas', 9, 'italic'))
        
        # Prompt input area
        prompt_label = ttk.Label(chat_frame, text="✏️ Your Prompt:", font=('Arial', 10, 'bold'))
        prompt_label.pack(anchor=tk.W)
        
        # Attachment display area (optional)
        attachment_frame = ttk.Frame(chat_frame)
        attachment_frame.pack(fill=tk.X, pady=(5, 0))
        
        ttk.Label(attachment_frame, text="📎 Attachment:", font=('Arial', 9)).pack(side=tk.LEFT, padx=(0, 5))
        
        self.attachment_label = ttk.Label(
            attachment_frame,
            text="No file attached",
            font=('Arial', 9),
            foreground='gray'
        )
        self.attachment_label.pack(side=tk.LEFT, padx=(0, 10))
        
        ttk.Button(
            attachment_frame,
            text="📎 Attach File",
            command=self.attach_file_to_chat,
            width=15
        ).pack(side=tk.LEFT, padx=(0, 5))
        
        ttk.Button(
            attachment_frame,
            text="🗑️ Clear",
            command=self.clear_attachment,
            width=10
        ).pack(side=tk.LEFT)
        
        self.attached_file_path = None
        self.attached_file_content = None
        
        # Help text for file attachments
        help_frame = ttk.Frame(chat_frame)
        help_frame.pack(fill=tk.X, pady=(2, 5))
        
        help_text = ttk.Label(
            help_frame,
            text="💡 Tip: Large files (>10KB) may take 2-5 minutes to analyze. Use 'qwen2.5:latest' model for best results.",
            font=('Arial', 8),
            foreground='#7F8C8D'
        )
        help_text.pack(anchor=tk.W, padx=(25, 0))
        
        prompt_input_frame = ttk.Frame(chat_frame)
        prompt_input_frame.pack(fill=tk.X, pady=(5, 0))
        
        self.prompt_text = scrolledtext.ScrolledText(
            prompt_input_frame,
            wrap=tk.WORD,
            font=('Consolas', 10),
            height=4,
            relief=tk.SUNKEN,
            borderwidth=2
        )
        self.prompt_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=(0, 10))
        
        button_column = ttk.Frame(prompt_input_frame)
        button_column.pack(side=tk.RIGHT, fill=tk.Y)
        
        self.send_btn = ttk.Button(
            button_column,
            text="📤 Send",
            command=self.send_prompt,
            style='Primary.TButton'
        )
        self.send_btn.pack(fill=tk.X, pady=(0, 5))
        
        self.stop_chat_btn = ttk.Button(
            button_column,
            text="🔄 Stop",
            command=self.stop_chat_generation,
            state='disabled'
        )
        self.stop_chat_btn.pack(fill=tk.X)
        
        # Bind Enter key to send
        self.prompt_text.bind('<Control-Return>', lambda e: self.send_prompt())
        
        # Initialize chat history
        self.chat_history = []
        self.chat_running = False
    
    def create_help_tab(self):
        """Create help/documentation tab"""
        help_frame = ttk.Frame(self.notebook, padding="10")
        self.notebook.add(help_frame, text="📚 Help")
        
        # Split into two panels: file list and content viewer
        paned = ttk.PanedWindow(help_frame, orient=tk.HORIZONTAL)
        paned.pack(fill=tk.BOTH, expand=True)
        
        # Left panel - Documentation file list
        left_panel = ttk.Frame(paned)
        paned.add(left_panel, weight=1)
        
        ttk.Label(
            left_panel,
            text="📖 Documentation Files",
            font=('Arial', 11, 'bold')
        ).pack(anchor=tk.W, pady=(0, 10))
        
        # Scrollable listbox for files
        list_frame = ttk.Frame(left_panel)
        list_frame.pack(fill=tk.BOTH, expand=True)
        
        scrollbar = ttk.Scrollbar(list_frame)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        self.doc_listbox = tk.Listbox(
            list_frame,
            font=('Arial', 10),
            yscrollcommand=scrollbar.set,
            activestyle='none',
            selectmode=tk.SINGLE
        )
        self.doc_listbox.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.config(command=self.doc_listbox.yview)
        
        # Bind selection event
        self.doc_listbox.bind('<<ListboxSelect>>', self.on_doc_select)
        
        # Right panel - Content viewer
        right_panel = ttk.Frame(paned)
        paned.add(right_panel, weight=3)
        
        # Header for selected file
        self.doc_title_label = ttk.Label(
            right_panel,
            text="Select a document to view",
            font=('Arial', 12, 'bold')
        )
        self.doc_title_label.pack(anchor=tk.W, pady=(0, 10))
        
        # Text widget for content
        text_frame = ttk.Frame(right_panel)
        text_frame.pack(fill=tk.BOTH, expand=True)
        
        text_scrollbar = ttk.Scrollbar(text_frame)
        text_scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        self.doc_text = tk.Text(
            text_frame,
            wrap=tk.WORD,
            font=('Consolas', 10),
            bg='#F8F9FA',
            fg='#2C3E50',
            padx=15,
            pady=15,
            yscrollcommand=text_scrollbar.set
        )
        self.doc_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        text_scrollbar.config(command=self.doc_text.yview)
        
        # Populate documentation files
        self.load_documentation_files()
        
    def load_documentation_files(self):
        """Load list of documentation files from docs/ folder"""
        docs_dir = self.project_root / "docs"
        
        if not docs_dir.exists():
            self.doc_listbox.insert(tk.END, "No documentation folder found")
            return
        
        # Get all .md files and README
        doc_files = []
        
        # Add README from root
        readme = self.project_root / "README.md"
        if readme.exists():
            doc_files.append(("📄 README.md", readme))
        
        # Add all docs/*.md files
        for md_file in sorted(docs_dir.glob("*.md")):
            doc_files.append((f"📝 {md_file.name}", md_file))
        
        # Store file paths for later access
        self.doc_files = doc_files
        
        # Populate listbox
        for display_name, _ in doc_files:
            self.doc_listbox.insert(tk.END, display_name)
        
        # Select first item by default if available
        if doc_files:
            self.doc_listbox.selection_set(0)
            self.on_doc_select(None)
    
    def on_doc_select(self, event):
        """Handle documentation file selection"""
        selection = self.doc_listbox.curselection()
        if not selection:
            return
        
        index = selection[0]
        if index >= len(self.doc_files):
            return
        
        display_name, file_path = self.doc_files[index]
        
        try:
            # Update title
            self.doc_title_label.config(text=f"📖 {file_path.name}")
            
            # Read and display content
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            # Clear and insert new content
            self.doc_text.config(state='normal')
            self.doc_text.delete('1.0', tk.END)
            self.doc_text.insert('1.0', content)
            self.doc_text.config(state='disabled')
            
            # Scroll to top
            self.doc_text.see('1.0')
            
        except Exception as e:
            self.doc_text.config(state='normal')
            self.doc_text.delete('1.0', tk.END)
            self.doc_text.insert('1.0', f"Error loading file:\n{str(e)}")
            self.doc_text.config(state='disabled')
        
    def create_status_bar(self):
        """Create status bar at bottom"""
        self.status_bar = ttk.Label(
            self.root,
            text="Ready",
            relief=tk.SUNKEN,
            anchor=tk.W,
            padding=(10, 5)
        )
        self.status_bar.pack(side=tk.BOTTOM, fill=tk.X)
        
    def create_tooltip(self, widget, text):
        """Create tooltip for widget"""
        def on_enter(event):
            self.status_bar.config(text=text)
        
        def on_leave(event):
            self.status_bar.config(text="Ready")
        
        widget.bind('<Enter>', on_enter)
        widget.bind('<Leave>', on_leave)
    
    def run_operation(self, operation_name, needs_module=True):
        """Run Python script directly in separate thread"""
        module_name = self.module_entry.get().strip()
        
        if needs_module and not module_name:
            messagebox.showwarning(
                "Missing Input",
                "Please enter a module name before running this operation."
            )
            return
        
        # Run in separate thread to keep UI responsive
        thread = threading.Thread(
            target=self._execute_operation,
            args=(operation_name, module_name),
            daemon=True
        )
        thread.start()
    
    def _execute_operation(self, operation_name, module_name):
        """Execute Python script directly (runs in separate thread)"""
        self.update_status(f"Running: {operation_name}")
        self.log_output(f"\n{'='*70}\n")
        self.log_output(f"▶️ Executing: {operation_name}\n")
        if module_name:
            self.log_output(f"📦 Module: {module_name}\n")
        self.log_output(f"{'='*70}\n\n")
        
        try:
            # Disable stop button initially, enable after process starts
            self.root.after(0, lambda: self.stop_btn.config(state='normal'))
            
            # Prepare environment and command
            env = os.environ.copy()
            src_dir = self.project_root / "src"
            
            # Build command based on operation
            if operation_name == "run_agent":
                # Run complete analysis
                report_file = self.project_root / "parasoftReport" / "report_dev1.html"
                script = src_dir / "run_agent.py"
                ai_mode = self.ai_mode_var.get() if hasattr(self, 'ai_mode_var') else "hybrid"
                cmd = [
                    sys.executable, str(script),
                    str(report_file), module_name,
                    "--ai-mode", ai_mode,
                    "--no-fixes"
                ]
                
            elif operation_name == "run_static_analyzer":
                # Run static analyzer - use input path from UI
                input_path_str = self.input_path_entry.get().strip()
                if input_path_str:
                    source_path = Path(input_path_str)
                    if not source_path.exists():
                        self.log_output(f"[WARNING] Input path does not exist: {source_path}\n")
                        source_path = self.project_root / "Inputs" / f"{module_name}_Config_1" / "PC_Config" / "src"
                else:
                    source_path = self.project_root / "Inputs" / f"{module_name}_Config_1" / "PC_Config" / "src"
                
                if not source_path.exists():
                    source_path = self.project_root
                    self.log_output(f"[INFO] Using project root as source path\n")
                else:
                    self.log_output(f"[INFO] Using source path: {source_path}\n")
                
                script = src_dir / "run_static_analyzer.py"
                cmd = [
                    sys.executable, str(script),
                    str(source_path), module_name,
                    "--no-fixes"
                ]
                
            elif operation_name == "generate_code_fixes":
                # Show interactive code fix review dialog
                self._generate_fixes_dialog(module_name)
                return  # Don't use subprocess for this
                
            elif operation_name == "apply_suppressions":
                # Apply suppressions - needs user to select file
                self.root.after(0, lambda: self._apply_suppressions_dialog(module_name))
                return
                
            elif operation_name == "generate_cert_misra_report":
                # Generate CERT/MISRA report
                script = src_dir / "generate_cert_misra_report.py"
                cmd = [sys.executable, str(script), module_name]
                
            elif operation_name == "learn_justification_patterns":
                # Learn patterns
                script = src_dir / "learn_justification_patterns.py"
                cmd = [sys.executable, str(script)]
                
            elif operation_name == "build_parasoft_rules":
                # Build Parasoft rules database
                script = src_dir / "ParasoftRulesParser.py"
                cmd = [sys.executable, str(script)]
                
            elif operation_name == "consolidate_knowledge":
                # Consolidate knowledge bases
                script = src_dir / "consolidate_knowledge.py"
                cmd = [sys.executable, str(script)]
                
            elif operation_name == "query_master_knowledge":
                # Query master knowledge base - show dialog first
                self._query_master_knowledge_dialog()
                return  # Dialog will handle the execution
                
            else:
                self.log_output(f"[ERROR] Unknown operation: {operation_name}\n")
                return
            
            # Log the command
            self.log_output(f"[DEBUG] Working directory: {self.project_root}\n")
            self.log_output(f"[DEBUG] Command: {' '.join(cmd)}\n\n")
            
            # Execute the Python script
            self.running_process = subprocess.Popen(
                cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                encoding='utf-8',
                errors='replace',
                bufsize=1,
                cwd=str(self.project_root),
                env=env
            )
            
            # Read output line by line
            for line in iter(self.running_process.stdout.readline, ''):
                if line:
                    self.log_output(line)
            
            # Wait for process to complete
            return_code = self.running_process.wait()
            
            self.log_output(f"\n{'='*70}\n")
            if return_code == 0:
                self.log_output("✅ Process completed successfully!\n")
                self.update_status(f"Completed: {operation_name}")
            else:
                self.log_output(f"⚠️ Process exited with code: {return_code}\n")
                self.update_status(f"Failed: {operation_name}")
            self.log_output(f"{'='*70}\n\n")
            
        except Exception as e:
            self.log_output(f"\n❌ Error: {str(e)}\n")
            self.update_status("Error occurred")
        finally:
            self.running_process = None
            self.root.after(0, lambda: self.stop_btn.config(state='disabled'))
    
    def _apply_suppressions_dialog(self, module_name):
        """Show dialog to select suppression file and apply interactively"""
        justifications_dir = self.project_root / "justifications"
        
        file_path = filedialog.askopenfilename(
            title="Select Suppression File",
            initialdir=justifications_dir,
            filetypes=[("Text files", "*.txt"), ("All files", "*.*")]
        )
        
        if not file_path:
            return
        
        # Ask for target repository
        target_repo = filedialog.askdirectory(
            title="Select Target Source Code Repository"
        )
        
        if not target_repo:
            return
        
        # Load suppressions and show interactive dialog
        self._show_interactive_suppression_dialog(file_path, target_repo)
    
    def _show_interactive_suppression_dialog(self, suppress_file, target_repo):
        """Show interactive dialog for applying suppressions one by one"""
        try:
            # Parse suppressions file
            from apply_suppress_comments import SuppressCommentApplicator
            applier = SuppressCommentApplicator(suppress_file, target_repo)
            
            # Parse the file
            suppressions_data = applier.parse_suppress_file()
            
            if not suppressions_data:
                messagebox.showinfo("No Suppressions", "No suppressions found in the selected file.")
                return
            
            # Count total suppressions
            total_suppressions = sum(len(data['suppressions']) for data in suppressions_data.values())
            
            if total_suppressions == 0:
                messagebox.showinfo("No Suppressions", "No suppressions to apply.")
                return
            
            # Create interactive dialog
            self._create_suppression_review_dialog(applier, suppressions_data, total_suppressions)
            
        except Exception as e:
            messagebox.showerror("Error", f"Failed to load suppressions:\n{str(e)}")
            self.log_output(f"❌ Error: {str(e)}\n")
    
    def _create_suppression_review_dialog(self, applier, suppressions_data, total_count):
        """Create dialog for reviewing and applying suppressions"""
        dialog = tk.Toplevel(self.root)
        dialog.title("📝 Review & Apply Suppressions")
        dialog.geometry("1000x700")
        
        # State
        current_file_idx = [0]
        current_supp_idx = [0]
        files_list = list(suppressions_data.keys())
        
        # Extract just the suppressions from each file data
        files_suppressions = {fname: fdata['suppressions'] for fname, fdata in suppressions_data.items()}
        
        # Stats
        stats = {
            'applied': 0,
            'skipped': 0,
            'rejected': 0,
            'already_justified': 0,
            'failed': 0
        }
        
        # Rejection feedback storage
        rejection_feedback = []
        
        # Header
        header_frame = ttk.Frame(dialog, padding="10")
        header_frame.pack(fill=tk.X)
        
        title_label = ttk.Label(
            header_frame,
            text="📝 Review Suppressions",
            font=('Arial', 14, 'bold')
        )
        title_label.pack(side=tk.LEFT)
        
        progress_var = tk.StringVar(value="0 / 0")
        progress_label = ttk.Label(
            header_frame,
            textvariable=progress_var,
            font=('Arial', 11)
        )
        progress_label.pack(side=tk.RIGHT)
        
        # Stats frame
        stats_frame = ttk.LabelFrame(dialog, text="📊 Statistics", padding="10")
        stats_frame.pack(fill=tk.X, padx=10, pady=(0, 10))
        
        stats_text = tk.StringVar(value="Applied: 0 | Skipped: 0 | Rejected: 0 | Already Justified: 0 | Failed: 0")
        stats_label = ttk.Label(stats_frame, textvariable=stats_text, font=('Arial', 10))
        stats_label.pack()
        
        # Content frame
        content_frame = ttk.LabelFrame(dialog, text="Suppression Details", padding="10")
        content_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=(0, 10))
        
        # File info
        file_info_var = tk.StringVar(value="File: -")
        file_label = ttk.Label(content_frame, textvariable=file_info_var, font=('Arial', 11, 'bold'))
        file_label.pack(anchor=tk.W, pady=(0, 5))
        
        line_info_var = tk.StringVar(value="Line: -")
        line_label = ttk.Label(content_frame, textvariable=line_info_var, font=('Arial', 10))
        line_label.pack(anchor=tk.W, pady=(0, 5))
        
        # Code preview
        ttk.Label(content_frame, text="Code Preview:", font=('Arial', 10, 'bold')).pack(anchor=tk.W, pady=(10, 5))
        
        code_text = scrolledtext.ScrolledText(
            content_frame,
            wrap=tk.WORD,
            font=('Courier New', 9),
            height=10,
            bg='#F5F5F5'
        )
        code_text.pack(fill=tk.BOTH, expand=True, pady=(0, 10))
        
        # Suppression comment
        ttk.Label(content_frame, text="Suppression Comment:", font=('Arial', 10, 'bold')).pack(anchor=tk.W, pady=(10, 5))
        
        supp_text = scrolledtext.ScrolledText(
            content_frame,
            wrap=tk.WORD,
            font=('Courier New', 9),
            height=3,
            bg='#E8F5E9'
        )
        supp_text.pack(fill=tk.X, pady=(0, 10))
        
        # Button frame
        button_frame = ttk.Frame(dialog, padding="10")
        button_frame.pack(fill=tk.X)
        
        def update_stats():
            stats_text.set(
                f"Applied: {stats['applied']} | Skipped: {stats['skipped']} | "
                f"Rejected: {stats['rejected']} | Already Justified: {stats['already_justified']} | Failed: {stats['failed']}"
            )
        
        def load_current_suppression():
            """Load and display current suppression"""
            if current_file_idx[0] >= len(files_list):
                # Done!
                # Save rejection feedback if any
                if rejection_feedback:
                    self._save_rejection_feedback(rejection_feedback)
                
                messagebox.showinfo(
                    "Complete",
                    f"All suppressions reviewed!\n\n"
                    f"Applied: {stats['applied']}\n"
                    f"Skipped: {stats['skipped']}\n"
                    f"Rejected: {stats['rejected']}\n"
                    f"Already Justified: {stats['already_justified']}\n"
                    f"Failed: {stats['failed']}"
                )
                dialog.destroy()
                return False
            
            file_name = files_list[current_file_idx[0]]
            suppressions = files_suppressions[file_name]
            
            if current_supp_idx[0] >= len(suppressions):
                # Move to next file
                current_file_idx[0] += 1
                current_supp_idx[0] = 0
                return load_current_suppression()
            
            supp = suppressions[current_supp_idx[0]]
            
            # Update progress
            total_processed = sum(
                len(files_suppressions[f]) for f in files_list[:current_file_idx[0]]
            ) + current_supp_idx[0] + 1
            progress_var.set(f"{total_processed} / {total_count}")
            
            # Find source file
            source_file = applier.find_source_file(file_name)
            
            if not source_file:
                file_info_var.set(f"File: {file_name} (NOT FOUND)")
                line_info_var.set("Line: -")
                code_text.delete('1.0', tk.END)
                code_text.insert('1.0', "[ERROR] File not found in target repository")
                supp_text.delete('1.0', tk.END)
                return True
            
            # Update file info
            file_info_var.set(f"File: {file_name}")
            line_num = supp['line']
            line_info_var.set(f"Line: {line_num}")
            
            # Read and show code preview
            try:
                with open(source_file, 'r', encoding='utf-8', errors='ignore') as f:
                    lines = f.readlines()
                
                # Show context (5 lines before and after)
                start = max(0, line_num - 6)
                end = min(len(lines), line_num + 5)
                
                code_text.delete('1.0', tk.END)
                for i in range(start, end):
                    line_content = lines[i].rstrip()
                    if i == line_num - 1:
                        code_text.insert(tk.END, f">>> {i+1:4d}: {line_content}\n", 'highlight')
                    else:
                        code_text.insert(tk.END, f"    {i+1:4d}: {line_content}\n")
                
                code_text.tag_config('highlight', background='yellow', font=('Courier New', 9, 'bold'))
                
                # Check if already has suppression
                if 'parasoft-suppress' in lines[line_num - 1]:
                    code_text.insert(tk.END, "\n⚠️ This line already has a suppression comment!", 'warning')
                    code_text.tag_config('warning', foreground='orange', font=('Courier New', 9, 'bold'))
                
            except Exception as e:
                code_text.delete('1.0', tk.END)
                code_text.insert('1.0', f"[ERROR] Could not read file: {str(e)}")
            
            # Show suppression comment
            supp_text.delete('1.0', tk.END)
            suppress_comment = supp.get('comment', supp.get('begin_comment', ''))
            supp_text.insert('1.0', suppress_comment)
            
            return True
        
        def apply_current():
            """Apply current suppression"""
            file_name = files_list[current_file_idx[0]]
            supp = files_suppressions[file_name][current_supp_idx[0]]
            source_file = applier.find_source_file(file_name)
            
            if not source_file:
                stats['failed'] += 1
                update_stats()
                messagebox.showerror("Error", f"File not found: {file_name}")
                current_supp_idx[0] += 1
                load_current_suppression()
                return
            
            try:
                # Check if already has suppression
                with open(source_file, 'r', encoding='utf-8', errors='ignore') as f:
                    lines = f.readlines()
                
                line_num = supp['line']
                if 'parasoft-suppress' in lines[line_num - 1]:
                    stats['already_justified'] += 1
                    update_stats()
                    messagebox.showinfo("Already Justified", "This line already has a suppression comment.")
                    current_supp_idx[0] += 1
                    load_current_suppression()
                    return
                
                # Apply suppression
                suppress_comment = supp.get('comment', '')
                if applier.apply_inline_suppression(source_file, line_num, suppress_comment):
                    stats['applied'] += 1
                    update_stats()
                    self.log_output(f"✅ Applied suppression to {file_name}:{line_num}\n")
                else:
                    stats['failed'] += 1
                    update_stats()
                    messagebox.showerror("Error", "Failed to apply suppression")
                
            except Exception as e:
                stats['failed'] += 1
                update_stats()
                messagebox.showerror("Error", f"Failed to apply: {str(e)}")
            
            current_supp_idx[0] += 1
            load_current_suppression()
        
        def skip_current():
            """Skip current suppression"""
            stats['skipped'] += 1
            update_stats()
            current_supp_idx[0] += 1
            load_current_suppression()
        
        def reject_current():
            """Reject current suppression with reason for learning"""
            file_name = files_list[current_file_idx[0]]
            supp = files_suppressions[file_name][current_supp_idx[0]]
            suppress_comment = supp.get('comment', '')
            
            # Extract rule ID from suppression comment
            rule_match = re.search(r'parasoft-suppress\s+([A-Z0-9_\-]+)', suppress_comment)
            rule_id = rule_match.group(1) if rule_match else 'UNKNOWN'
            
            # Show reason dialog
            reason_dialog = tk.Toplevel(dialog)
            reason_dialog.title("🚫 Rejection Reason")
            reason_dialog.geometry("600x400")
            reason_dialog.transient(dialog)
            
            ttk.Label(
                reason_dialog,
                text=f"Why are you rejecting this suppression for {rule_id}?",
                font=('Arial', 11, 'bold')
            ).pack(padx=10, pady=(10, 5))
            
            ttk.Label(
                reason_dialog,
                text="Your feedback will help improve future suggestions.",
                font=('Arial', 9),
                foreground='gray'
            ).pack(padx=10, pady=(0, 10))
            
            # Show suppression being rejected
            ttk.Label(reason_dialog, text="Suppression:", font=('Arial', 9, 'bold')).pack(padx=10, anchor=tk.W)
            supp_preview = scrolledtext.ScrolledText(
                reason_dialog,
                wrap=tk.WORD,
                font=('Courier New', 9),
                height=3,
                bg='#FFE5E5'
            )
            supp_preview.pack(padx=10, pady=(0, 10), fill=tk.X)
            supp_preview.insert('1.0', suppress_comment)
            supp_preview.config(state='disabled')
            
            # Reason categories
            ttk.Label(reason_dialog, text="Select reason category:", font=('Arial', 9, 'bold')).pack(padx=10, anchor=tk.W)
            
            reason_category = tk.StringVar(value="incorrect_justification")
            categories = [
                ("incorrect_justification", "Incorrect justification/reasoning"),
                ("wrong_rule", "Wrong rule applied to this violation"),
                ("false_positive", "This is not actually a violation"),
                ("wrong_format", "Suppression format is incorrect"),
                ("duplicate", "Already suppressed elsewhere"),
                ("other", "Other (specify below)")
            ]
            
            for value, label in categories:
                ttk.Radiobutton(
                    reason_dialog,
                    text=label,
                    variable=reason_category,
                    value=value
                ).pack(padx=20, anchor=tk.W)
            
            # Additional details
            ttk.Label(reason_dialog, text="Additional details:", font=('Arial', 9, 'bold')).pack(padx=10, pady=(10, 5), anchor=tk.W)
            reason_text = scrolledtext.ScrolledText(
                reason_dialog,
                wrap=tk.WORD,
                font=('Arial', 9),
                height=4
            )
            reason_text.pack(padx=10, pady=(0, 10), fill=tk.BOTH, expand=True)
            
            def submit_rejection():
                category = reason_category.get()
                details = reason_text.get('1.0', tk.END).strip()
                
                if not details and category == "other":
                    messagebox.showwarning("Missing Details", "Please provide details for 'Other' category.")
                    return
                
                # Store rejection feedback
                feedback_entry = {
                    'file': file_name,
                    'line': supp.get('line', 0),
                    'rule_id': rule_id,
                    'suppression_comment': suppress_comment,
                    'rejection_category': category,
                    'rejection_reason': details,
                    'timestamp': datetime.now().isoformat()
                }
                rejection_feedback.append(feedback_entry)
                
                # Log to GUI
                self.log_output(f"🚫 Rejected suppression for {rule_id} at {file_name}:{supp.get('line')}\n")
                self.log_output(f"   Category: {category}\n")
                self.log_output(f"   Reason: {details[:100]}...\n" if len(details) > 100 else f"   Reason: {details}\n")
                
                stats['rejected'] += 1
                update_stats()
                reason_dialog.destroy()
                current_supp_idx[0] += 1
                load_current_suppression()
            
            # Buttons
            btn_frame = ttk.Frame(reason_dialog)
            btn_frame.pack(fill=tk.X, padx=10, pady=10)
            
            ttk.Button(btn_frame, text="Submit Rejection", command=submit_rejection).pack(side=tk.LEFT, padx=5)
            ttk.Button(btn_frame, text="Cancel", command=reason_dialog.destroy).pack(side=tk.LEFT, padx=5)
            
            reason_dialog.grab_set()
        
        def apply_all_remaining():
            """Apply all remaining suppressions"""
            if not messagebox.askyesno(
                "Apply All",
                "Apply all remaining suppressions without prompting?\n\n"
                "This will skip suppressions that already exist."
            ):
                return
            
            # Apply all remaining
            while current_file_idx[0] < len(files_list):
                file_name = files_list[current_file_idx[0]]
                suppressions = files_suppressions[file_name]
                source_file = applier.find_source_file(file_name)
                
                if not source_file:
                    stats['failed'] += len(suppressions) - current_supp_idx[0]
                    current_file_idx[0] += 1
                    current_supp_idx[0] = 0
                    continue
                
                for i in range(current_supp_idx[0], len(suppressions)):
                    supp = suppressions[i]
                    line_num = supp['line']
                    suppress_comment = supp.get('comment', '')
                    
                    try:
                        # Check if already has suppression
                        with open(source_file, 'r', encoding='utf-8', errors='ignore') as f:
                            lines = f.readlines()
                        
                        if 'parasoft-suppress' in lines[line_num - 1]:
                            stats['already_justified'] += 1
                        elif applier.apply_inline_suppression(source_file, line_num, suppress_comment):
                            stats['applied'] += 1
                            self.log_output(f"✅ Applied suppression to {file_name}:{line_num}\n")
                        else:
                            stats['failed'] += 1
                    except:
                        stats['failed'] += 1
                    
                    update_stats()
                
                current_file_idx[0] += 1
                current_supp_idx[0] = 0
            
            # Save rejection feedback if any
            if rejection_feedback:
                self._save_rejection_feedback(rejection_feedback)
            
            messagebox.showinfo(
                "Complete",
                f"All suppressions applied!\n\n"
                f"Applied: {stats['applied']}\n"
                f"Skipped: {stats['skipped']}\n"
                f"Rejected: {stats['rejected']}\n"
                f"Already Justified: {stats['already_justified']}\n"
                f"Failed: {stats['failed']}"
            )
            dialog.destroy()
        
        # Buttons
        ttk.Button(button_frame, text="✅ Apply", command=apply_current, width=15).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="⏭️ Skip", command=skip_current, width=15).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="🚫 Reject", command=reject_current, width=15).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="⚡ Apply All Remaining", command=apply_all_remaining, width=20).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="❌ Cancel", command=dialog.destroy, width=15).pack(side=tk.RIGHT, padx=5)
        
        # Load first suppression
        if not load_current_suppression():
            return
        
        dialog.transient(self.root)
        dialog.grab_set()
    
    def _generate_fixes_dialog(self, module_name):
        """Show dialog to generate and review code fixes interactively"""
        if not module_name:
            messagebox.showwarning("Module Required", "Please enter a module name.")
            return
        
        self.log_output(f"\n{'='*70}\n")
        self.log_output(f"▶️ Generating Code Fixes for Module: {module_name}\n")
        self.log_output(f"{'='*70}\n\n")
        
        try:
            # Import required modules
            from KnowledgeDatabaseManager import KnowledgeDatabaseManager
            from CodeFixGenerator import CodeFixGenerator
            
            # Check if knowledge base exists
            kb_path = self.project_root / 'knowledge_base' / f'{module_name}_KnowledgeDatabase.json'
            if not kb_path.exists():
                messagebox.showerror(
                    "Knowledge Base Not Found",
                    f"Knowledge base not found for module: {module_name}\n\n"
                    f"Please run 'Run Complete Analysis' first to create the knowledge base."
                )
                return
            
            # Load knowledge base
            self.log_output(f"📚 Loading knowledge base for module: {module_name}...\n")
            kb_manager = KnowledgeDatabaseManager(self.project_root / 'knowledge_base')
            kb_manager.load_knowledge_base(module_name)
            
            # Get unfixed violations
            all_violations = kb_manager.get_all_violations()
            unfixed_violations = [v for v in all_violations if not v.get('fix_applied')]
            
            if not unfixed_violations:
                messagebox.showinfo(
                    "No Fixes Needed",
                    f"All violations have already been fixed for module: {module_name}"
                )
                return
            
            self.log_output(f"✓ Found {len(unfixed_violations)} violations needing fixes\n")
            self.log_output(f"  Total violations: {len(all_violations)}\n")
            self.log_output(f"  Already fixed: {len(all_violations) - len(unfixed_violations)}\n\n")
            
            # AUTO-CHECK: Do violations need enrichment?
            needs_enrichment = sum(1 for v in unfixed_violations 
                                 if not v.get('code_snippet') or v.get('file_path') in ['UNKNOWN', 'N/A', ''])
            
            if needs_enrichment > 0:
                self.log_output(f"🔍 Checking enrichment status...\n")
                self.log_output(f"  ⚠️  {needs_enrichment} violations need source code context\n")
                
                # Get source code path
                source_path = self.input_path_entry.get().strip()
                
                if not source_path or not Path(source_path).exists():
                    # Ask user for source path
                    response = messagebox.askyesno(
                        "Source Code Context Needed",
                        f"{needs_enrichment} violations don't have source code context.\n\n"
                        "For better AI-powered fixes, the tool needs to read your actual code.\n\n"
                        "Would you like to select the source code directory now?\n\n"
                        "(You can skip this and use generic fixes instead)"
                    )
                    
                    if response:
                        source_path = filedialog.askdirectory(
                            title="Select Source Code Directory",
                            initialdir=self.project_root / "Input"
                        )
                        
                        if source_path:
                            self.input_path_entry.delete(0, tk.END)
                            self.input_path_entry.insert(0, source_path)
                        else:
                            self.log_output(f"  ⏭️  Skipping enrichment, using generic fixes\n\n")
                            source_path = None
                    else:
                        source_path = None
                
                # AUTO-ENRICH if source path available
                if source_path and Path(source_path).exists():
                    self.log_output(f"  ✨ Auto-enriching violations with source code...\n")
                    self.log_output(f"     Source: {source_path}\n")
                    
                    try:
                        from ViolationEnricher import enrich_knowledge_base_violations
                        
                        count = enrich_knowledge_base_violations(kb_path, Path(source_path))
                        
                        self.log_output(f"  ✅ Enriched {count} violations with source code context!\n")
                        self.log_output(f"     AI can now see your actual code for better fixes\n\n")
                        
                        # Reload knowledge base to get enriched data
                        kb_manager.load_knowledge_base(module_name)
                        all_violations = kb_manager.get_all_violations()
                        unfixed_violations = [v for v in all_violations if not v.get('fix_applied')]
                        
                    except Exception as e:
                        self.log_output(f"  ⚠️  Enrichment warning: {str(e)}\n")
                        self.log_output(f"     Continuing with existing data...\n\n")
                else:
                    self.log_output(f"  ⏭️  Proceeding without enrichment\n\n")
            else:
                self.log_output(f"✅ Violations already have source code context\n\n")
            
            # Initialize Code Fix Generator
            fixes_dir = self.project_root / f'{module_name}_code_suggestion'
            fixes_dir.mkdir(exist_ok=True)
            
            ai_mode = self.ai_mode_var.get() if hasattr(self, 'ai_mode_var') else "hybrid"
            
            # Get source code path from input field (critical for AI code context)
            source_path = self.input_path_entry.get().strip()
            
            # Load full config from config.json to preserve all settings
            config_path = self.project_root / 'config' / 'config.json'
            if config_path.exists():
                with open(config_path, 'r') as f:
                    config = json.load(f)
            else:
                config = {}
            
            # Update with GUI overrides
            if 'ai_integration' not in config:
                config['ai_integration'] = {}
            config['ai_integration']['ai_mode'] = ai_mode
            config['source_code_path'] = source_path if source_path else None
            
            if source_path:
                self.log_output(f"📂 Source code path: {source_path}\n")
            else:
                self.log_output(f"⚠️  No source code path set - AI will use generic patterns\n")
            
            self.log_output(f"📋 Config loaded from: {config_path}\n")
            self.log_output(f"   AI Integration enabled in config: {config.get('ai_integration', {}).get('enabled', False)}\n")
            
            fix_generator = CodeFixGenerator(module_name, kb_manager, fixes_dir, config)
            
            # Check Ollama status
            self.log_output(f"🤖 AI Configuration:\n")
            self.log_output(f"   Mode: {ai_mode}\n")
            self.log_output(f"   Ollama enabled: {fix_generator.ollama.enabled}\n")
            if fix_generator.ollama.enabled:
                self.log_output(f"   Model: {fix_generator.ollama.model}\n")
                self.log_output(f"   URL: {fix_generator.ollama.base_url}\n")
            else:
                self.log_output(f"   ⚠️  Ollama is NOT enabled - will use pattern-based fixes only\n")
                if ai_mode in ['ai_only', 'hybrid']:
                    self.log_output(f"   💡 TIP: To enable AI, install Ollama and run 'ollama serve'\n")
            self.log_output(f"\n")
            
            # Show interactive review dialog
            self._show_interactive_fix_review_dialog(fix_generator, unfixed_violations, kb_manager, module_name)
            
        except Exception as e:
            messagebox.showerror("Error", f"Failed to load code fixes:\n{str(e)}")
            self.log_output(f"❌ Error: {str(e)}\n")
    
    def _show_interactive_fix_review_dialog(self, fix_generator, violations, kb_manager, module_name):
        """Show interactive dialog for reviewing code fixes one by one"""
        dialog = tk.Toplevel(self.root)
        dialog.title("🛠️ Review & Apply Code Fixes")
        dialog.geometry("1200x800")
        
        # State
        current_idx = [0]
        
        # Stats
        stats = {
            'accepted': 0,
            'rejected': 0,
            'skipped': 0,
            'failed': 0
        }
        
        # Rejection feedback storage
        rejection_feedback = []
        
        # Generate fix for current violation (lazy loading)
        current_fix_data = [None]
        
        # Header
        header_frame = ttk.Frame(dialog, padding="10")
        header_frame.pack(fill=tk.X)
        
        title_label = ttk.Label(
            header_frame,
            text=f"🛠️ Code Fixes for {module_name}",
            font=('Arial', 14, 'bold')
        )
        title_label.pack(side=tk.LEFT)
        
        progress_var = tk.StringVar(value="0 / 0")
        progress_label = ttk.Label(
            header_frame,
            textvariable=progress_var,
            font=('Arial', 11)
        )
        progress_label.pack(side=tk.RIGHT)
        
        # Stats frame
        stats_frame = ttk.LabelFrame(dialog, text="📊 Statistics", padding="10")
        stats_frame.pack(fill=tk.X, padx=10, pady=(0, 10))
        
        stats_text = tk.StringVar(value="Accepted: 0 | Rejected: 0 | Skipped: 0 | Failed: 0")
        stats_label = ttk.Label(stats_frame, textvariable=stats_text, font=('Arial', 10))
        stats_label.pack()
        
        # Content frame with notebook for tabs
        content_frame = ttk.LabelFrame(dialog, text="Fix Details", padding="10")
        content_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=(0, 10))
        
        # Notebook for tabs
        notebook = ttk.Notebook(content_frame)
        notebook.pack(fill=tk.BOTH, expand=True)
        
        # Tab 1: Violation Info
        violation_frame = ttk.Frame(notebook, padding="10")
        notebook.add(violation_frame, text="📋 Violation")
        
        violation_text = scrolledtext.ScrolledText(
            violation_frame,
            wrap=tk.WORD,
            font=('Courier New', 9),
            bg='#FFF5E1'
        )
        violation_text.pack(fill=tk.BOTH, expand=True)
        
        # Tab 2: Suggested Fix
        fix_frame = ttk.Frame(notebook, padding="10")
        notebook.add(fix_frame, text="✨ Suggested Fix")
        
        fix_text = scrolledtext.ScrolledText(
            fix_frame,
            wrap=tk.WORD,
            font=('Courier New', 9),
            bg='#E8F5E9'
        )
        fix_text.pack(fill=tk.BOTH, expand=True)
        
        # Tab 3: Context & Examples
        context_frame = ttk.Frame(notebook, padding="10")
        notebook.add(context_frame, text="💡 Context")
        
        context_text = scrolledtext.ScrolledText(
            context_frame,
            wrap=tk.WORD,
            font=('Courier New', 9),
            bg='#E3F2FD'
        )
        context_text.pack(fill=tk.BOTH, expand=True)
        
        # Button frame
        button_frame = ttk.Frame(dialog, padding="10")
        button_frame.pack(fill=tk.X)
        
        def update_stats():
            stats_text.set(
                f"Accepted: {stats['accepted']} | Rejected: {stats['rejected']} | "
                f"Skipped: {stats['skipped']} | Failed: {stats['failed']}"
            )
        
        def load_current_fix():
            """Load and display current fix"""
            if current_idx[0] >= len(violations):
                # Done!
                if rejection_feedback:
                    self._save_fix_rejection_feedback(rejection_feedback)
                
                messagebox.showinfo(
                    "Complete",
                    f"All fixes reviewed!\n\n"
                    f"Accepted: {stats['accepted']}\n"
                    f"Rejected: {stats['rejected']}\n"
                    f"Skipped: {stats['skipped']}\n"
                    f"Failed: {stats['failed']}"
                )
                dialog.destroy()
                return False
            
            violation = violations[current_idx[0]]
            
            # Update progress
            progress_var.set(f"{current_idx[0] + 1} / {len(violations)}")
            
            # Load violation details
            violation_text.delete('1.0', tk.END)
            
            # Extract file and line from files_affected (handles dict format)
            file_info = 'UNKNOWN'
            line_info = 'UNKNOWN'
            
            # Try direct fields first (enriched violations)
            if violation.get('file'):
                file_info = violation['file']
            if violation.get('line'):
                line_info = violation['line']
            
            # Fall back to files_affected
            if file_info == 'UNKNOWN' or line_info == 'UNKNOWN':
                files_affected = violation.get('files_affected', [])
                if files_affected:
                    first_file = files_affected[0]
                    if isinstance(first_file, dict):
                        file_info = first_file.get('file', 'UNKNOWN')
                        line_info = first_file.get('line', 'UNKNOWN')
                    elif isinstance(first_file, str):
                        # Handle string format "file.c:123"
                        if ':' in first_file:
                            parts = first_file.rsplit(':', 1)
                            file_info = parts[0]
                            try:
                                line_info = int(parts[1])
                            except:
                                pass
                        else:
                            file_info = first_file
            
            # Get code context (with surrounding lines for better understanding)
            code_display = ""
            code_context = violation.get('code_context')
            code_context_lines = violation.get('code_context_lines', {})
            
            if code_context and code_context_lines:
                # Display full context with line numbers
                start_line = code_context_lines.get('start', 1)
                violation_line = code_context_lines.get('violation', line_info)
                
                code_display = "CODE CONTEXT (with surrounding lines):\n"
                code_display += "=" * 60 + "\n"
                
                for i, line in enumerate(code_context.split('\n')):
                    current_line_num = start_line + i
                    if current_line_num == violation_line:
                        # Highlight the violation line
                        code_display += f">>> {current_line_num:4d} | {line}\n"
                    else:
                        code_display += f"    {current_line_num:4d} | {line}\n"
                
                code_display += "=" * 60 + "\n"
                code_display += f"(Line {violation_line} contains the violation)"
            else:
                # Fallback to single line snippet
                code_snippet = violation.get('code_snippet', 'No code snippet available')
                if code_snippet == 'No code snippet available':
                    code_display = code_snippet + '\n\n💡 TIP: Set "Input Path" to your source code directory.\n   Enrichment will happen automatically when you generate fixes.'
                else:
                    code_display = f"CODE SNIPPET:\n{code_snippet}\n\n⚠️  Limited context. Enable enrichment for better context."
            
            violation_info = f"""
VIOLATION ID: {violation.get('violation_id', 'UNKNOWN')}
RULE: {violation.get('rule', 'UNKNOWN')}
SEVERITY: {violation.get('severity', 'UNKNOWN')}
FILE: {file_info}
LINE: {line_info}

DESCRIPTION:
{violation.get('violation_text', 'No description available')}

{code_display}
            """.strip()
            
            violation_text.insert('1.0', violation_info)
            
            # Generate fix for this violation
            fix_text.delete('1.0', tk.END)
            fix_text.insert('1.0', "⏳ Generating fix suggestion...\n\nPlease wait...")
            context_text.delete('1.0', tk.END)
            
            # Log to main window
            self.log_output(f"\n{'='*70}\n")
            self.log_output(f"🔧 Generating fix for: {violation.get('violation_id')}\n")
            self.log_output(f"   Rule: {violation.get('rule', 'UNKNOWN')}\n")
            self.log_output(f"   File: {file_info}:{line_info}\n")
            
            # Check if code context exists
            has_code_context = bool(violation.get('code_context'))
            self.log_output(f"   Code context available: {has_code_context}\n")
            if has_code_context:
                self.log_output(f"   ✅ AI will analyze your actual code\n")
            else:
                self.log_output(f"   ⚠️  No code context - will use generic patterns\n")
            
            # Use thread to generate fix without blocking UI
            import threading
            def generate_fix_async():
                try:
                    self.log_output(f"   Calling fix generator...\n")
                    fix_data = fix_generator._generate_fix_for_violation(violation)
                    
                    if fix_data:
                        self.log_output(f"   ✅ Fix generated successfully\n")
                        fix_type = fix_data.get('fix_suggestion', {})
                        if isinstance(fix_type, dict):
                            self.log_output(f"      Type: {fix_type.get('type', 'unknown')}\n")
                            self.log_output(f"      AI-generated: {fix_type.get('ai_generated', False)}\n")
                    else:
                        self.log_output(f"   ❌ No fix data returned (None)\n")
                    
                    current_fix_data[0] = fix_data
                    
                    # Update UI on main thread
                    dialog.after(0, lambda: display_generated_fix(fix_data))
                except Exception as e:
                    self.log_output(f"   ❌ ERROR: {str(e)}\n")
                    import traceback
                    self.log_output(f"   Traceback:\n{traceback.format_exc()}\n")
                    dialog.after(0, lambda: show_fix_error(str(e)))
            
            thread = threading.Thread(target=generate_fix_async, daemon=True)
            thread.start()
            
            return True
        
        def display_generated_fix(fix_data):
            """Display the generated fix in UI"""
            # Get current violation first (needed for multiple places below)
            violation = violations[current_idx[0]]
            
            if not fix_data:
                fix_text.delete('1.0', tk.END)
                fix_text.insert('1.0', "❌ No fix could be generated for this violation.\n\n")
                fix_text.insert(tk.END, "=" * 70 + "\n\n")
                fix_text.insert(tk.END, "POSSIBLE REASONS:\n\n")
                fix_text.insert(tk.END, "1. ⚠️  Ollama AI is not running or not responding\n")
                fix_text.insert(tk.END, "   - Start Ollama: 'ollama serve'\n")
                fix_text.insert(tk.END, "   - Check model is installed: 'ollama list'\n\n")
                fix_text.insert(tk.END, "2. ⚠️  No matching fix pattern found\n")
                fix_text.insert(tk.END, "   - Check main log window for details\n\n")
                fix_text.insert(tk.END, "3. ⚠️  Violation type not supported\n")
                fix_text.insert(tk.END, "   - Try enabling Parasoft Rules Database\n\n")
                fix_text.insert(tk.END, "💡 Check the main log window above for detailed error messages.\n")
                return
            
            # Display fix suggestions (can be multiple)
            fix_text.delete('1.0', tk.END)
            
            fix_suggestion = fix_data.get('fix_suggestion', 'No fix suggestion available')
            
            # Handle dict-based fix suggestion
            if isinstance(fix_suggestion, dict):
                fix_type = fix_suggestion.get('type', 'unknown')
                description = fix_suggestion.get('description', 'No description')
                example = fix_suggestion.get('example', '')
                priority = fix_suggestion.get('priority', 'MEDIUM')
                ai_generated = fix_suggestion.get('ai_generated', False)
                warning = fix_suggestion.get('warning', '')
                
                # Header showing fix source
                if ai_generated:
                    fix_display = "🤖 AI-GENERATED SPECIFIC FIX\n"
                    # Get line info from the current violation
                    viol_line = 'UNKNOWN'
                    if violation.get('line'):
                        viol_line = violation['line']
                    elif violation.get('files_affected'):
                        files_affected = violation.get('files_affected', [])
                        if files_affected:
                            first_file = files_affected[0]
                            if isinstance(first_file, dict):
                                viol_line = first_file.get('line', 'UNKNOWN')
                    fix_display += f"   (Analyzed your actual code at line {viol_line})\n"
                else:
                    fix_display = "📚 PARASOFT GENERIC EXAMPLES\n"
                    if warning:
                        fix_display += f"   ⚠️  {warning}\n"
                    else:
                        # Show why AI wasn't used
                        fix_display += f"   ⚠️  AI fix generation failed or returned generic patterns\n"
                        fix_display += f"   💡 Check main log window for AI debug messages\n"
                
                fix_display += "=" * 70 + "\n\n"
                fix_display += f"FIX TYPE: {fix_type.upper()}\n"
                fix_display += f"PRIORITY: {priority}\n\n"
                fix_display += f"DESCRIPTION:\n{description}\n\n"
                
                if example:
                    if ai_generated:
                        fix_display += f"SPECIFIC FIX FOR YOUR CODE:\n{'-'*70}\n{example}\n\n"
                    else:
                        fix_display += f"GENERIC EXAMPLES (adapt to your code):\n{'-'*70}\n{example}\n\n"
                
                # If it's a justification recommendation
                if fix_type == 'justification_recommended':
                    fix_display += "\n⚠️  RECOMMENDATION:\n"
                    fix_display += "This appears to be a common deviation across multiple modules.\n"
                    fix_display += "Consider justifying instead of fixing.\n"
                
                fix_text.insert('1.0', fix_display)
            else:
                # String-based fix suggestion
                fix_text.insert('1.0', str(fix_suggestion))
            
            # Display context (Parasoft examples, CWEs, rule documentation)
            context_text.delete('1.0', tk.END)
            context_info = []
            
            # Add Parasoft repair example
            if fix_data.get('parasoft_repair'):
                context_info.append(f"📚 PARASOFT REPAIR EXAMPLE:\n{'-'*70}\n{fix_data['parasoft_repair']}\n")
            
            # Add CWE mappings
            if fix_data.get('cwe_ids'):
                context_info.append(f"🔗 CWE MAPPINGS: {', '.join(fix_data['cwe_ids'])}\n")
            
            # Add fix type
            if fix_data.get('fix_type'):
                context_info.append(f"🛠️  FIX TYPE: {fix_data['fix_type']}\n")
            
            # Load HTML rule documentation (violation already loaded at top of function)
            rule_id = violation.get('rule', 'UNKNOWN')
            violation_id = violation.get('violation_id', 'UNKNOWN')
            
            html_doc = self._load_rule_html_documentation(rule_id, violation_id)
            if html_doc:
                context_info.append(f"\n📖 PARASOFT RULE DOCUMENTATION:\n{'-'*70}\n{html_doc}\n")
            
            if context_info:
                context_text.insert('1.0', '\n'.join(context_info))
            else:
                context_text.insert('1.0', "ℹ️  No additional context available.\n\n"
                                           "Enable Parasoft Rules Database for repair examples and documentation.")
            
            # ADD CODE SUGGESTION to the Violation tab
            if isinstance(fix_suggestion, dict) and fix_suggestion.get('ai_generated'):
                example_text = fix_suggestion.get('example', '')
                self.logger.info(f"Attempting to extract code suggestion from AI example")
                self.logger.info(f"Example text length: {len(example_text)} chars")
                self.logger.info(f"Example preview: {example_text[:200] if example_text else 'EMPTY'}")
                
                # Extract the "After" code from the example
                after_code = None
                
                # Try multiple patterns to find the fixed code
                for pattern in ['// After:', 'After:']:
                    if pattern in example_text:
                        self.logger.info(f"Found pattern: {pattern}")
                        parts = example_text.split(pattern)
                        if len(parts) > 1:
                            # Get everything after the pattern
                            after_section = parts[1].strip()
                            self.logger.info(f"After section: {after_section[:100]}")
                            
                            # Extract just the code line (first non-empty line)
                            for line in after_section.split('\n'):
                                line = line.strip()
                                # Skip empty lines, comments, and markdown
                                if line and not line.startswith('//') and not line.startswith('```'):
                                    after_code = line
                                    self.logger.info(f"Extracted after_code: {after_code}")
                                    break
                            
                            if after_code:
                                break
                
                self.logger.info(f"Final after_code: {after_code}")
                
                # Extract code context from violation for display
                code_context_data = violation.get('code_context')
                code_context = None
                code_context_lines = None
                
                if code_context_data:
                    code_context = code_context_data.get('context', '')
                    code_context_lines = {
                        'start': code_context_data.get('start_line', 1),
                        'violation': code_context_data.get('line', line_info)
                    }
                
                self.logger.info(f"code_context exists: {bool(code_context)}")
                self.logger.info(f"code_context_lines exists: {bool(code_context_lines)}")
                
                # If we have code context and a fix suggestion, show the suggested code
                if after_code and code_context and code_context_lines:
                    self.logger.info("✅ Displaying CODE SUGGESTION in Violation tab")
                    violation_text.insert(tk.END, "\n\n")
                    violation_text.insert(tk.END, "CODE SUGGESTION (with surrounding lines):\n")
                    violation_text.insert(tk.END, "=" * 60 + "\n")
                    
                    # Show the same context but with the fix applied to the violation line
                    start_line = code_context_lines.get('start', 1)
                    violation_line = code_context_lines.get('violation', line_info)
                    
                    for i, line in enumerate(code_context.split('\n')):
                        current_line_num = start_line + i
                        if current_line_num == violation_line:
                            # Show the fixed line
                            # Preserve indentation from original line
                            original_line = line
                            indent = len(original_line) - len(original_line.lstrip())
                            indented_fix = ' ' * indent + after_code
                            violation_text.insert(tk.END, f">>> {current_line_num:4d} | {indented_fix}\n")
                        else:
                            violation_text.insert(tk.END, f"    {current_line_num:4d} | {line}\n")
                    
                    violation_text.insert(tk.END, "=" * 60 + "\n")
                    violation_text.insert(tk.END, f"(Line {violation_line} shows the suggested fix)\n")
                else:
                    self.logger.warning(f"⚠️ Cannot display CODE SUGGESTION - missing data:")
                    self.logger.warning(f"  after_code: {bool(after_code)}")
                    self.logger.warning(f"  code_context: {bool(code_context)}")
                    self.logger.warning(f"  code_context_lines: {bool(code_context_lines)}")
            else:
                self.logger.info("No AI-generated fix to display in Violation tab")
                if isinstance(fix_suggestion, dict):
                    self.logger.info(f"  ai_generated flag: {fix_suggestion.get('ai_generated')}")
        
        def show_fix_error(error_msg):
            """Show error if fix generation failed"""
            fix_text.delete('1.0', tk.END)
            fix_text.insert('1.0', f"❌ Error generating fix:\n\n{error_msg}\n\n")
            fix_text.insert(tk.END, "=" * 70 + "\n\n")
            fix_text.insert(tk.END, "💡 TROUBLESHOOTING:\n\n")
            fix_text.insert(tk.END, "1. Check main log window for detailed error messages\n")
            fix_text.insert(tk.END, "2. If using AI mode, ensure Ollama is running:\n")
            fix_text.insert(tk.END, "   - Run 'ollama serve' in terminal\n")
            fix_text.insert(tk.END, "   - Run 'ollama pull codellama' to download model\n")
            fix_text.insert(tk.END, "3. Check config.json AI settings\n")
            fix_text.insert(tk.END, "4. Try 'Rules Only' mode if AI is unavailable\n")
        
        def accept_current():
            """Accept current fix"""
            if current_fix_data[0]:
                violation = violations[current_idx[0]]
                violation_id = violation.get('violation_id')
                fix_data = current_fix_data[0]
                
                # Mark as fixed in knowledge base
                fix_details = {
                    'fix_applied': True,
                    'fix_type': fix_data.get('fix_suggestion', {}).get('type', 'unknown') if isinstance(fix_data.get('fix_suggestion'), dict) else 'unknown',
                    'timestamp': datetime.now().isoformat(),
                    'accepted_by': 'user'
                }
                kb_manager.update_fix_status(violation_id, fix_details, 'Fix accepted by user')
                
                stats['accepted'] += 1
                update_stats()
                self.log_output(f"✅ Accepted fix for {violation_id}\n")
            else:
                stats['failed'] += 1
                update_stats()
            
            current_idx[0] += 1
            current_fix_data[0] = None  # Clear cached fix to force regeneration
            load_current_fix()
        
        def skip_current():
            """Skip current fix"""
            stats['skipped'] += 1
            update_stats()
            current_idx[0] += 1
            current_fix_data[0] = None  # Clear cached fix to force regeneration
            load_current_fix()
        
        def reject_current():
            """Reject current fix with reason"""
            violation = violations[current_idx[0]]
            fix_data = current_fix_data[0]
            
            if not fix_data:
                messagebox.showinfo("No Fix", "No fix was generated for this violation.")
                return
            
            # Show reason dialog
            reason_dialog = tk.Toplevel(dialog)
            reason_dialog.title("🚫 Rejection Reason")
            reason_dialog.geometry("600x400")
            reason_dialog.transient(dialog)
            
            ttk.Label(
                reason_dialog,
                text=f"Why are you rejecting this fix for {violation.get('violation_id')}?",
                font=('Arial', 11, 'bold')
            ).pack(padx=10, pady=(10, 5))
            
            ttk.Label(
                reason_dialog,
                text="Your feedback will help improve future suggestions.",
                font=('Arial', 9),
                foreground='gray'
            ).pack(padx=10, pady=(0, 10))
            
            # Reason categories
            ttk.Label(reason_dialog, text="Select reason category:", font=('Arial', 9, 'bold')).pack(padx=10, anchor=tk.W)
            
            reason_category = tk.StringVar(value="incorrect_fix")
            categories = [
                ("incorrect_fix", "Fix doesn't solve the violation"),
                ("breaks_code", "Fix would break existing functionality"),
                ("poor_quality", "Fix quality is poor/incomplete"),
                ("not_applicable", "Fix not applicable to this context"),
                ("better_alternative", "I have a better solution"),
                ("other", "Other (specify below)")
            ]
            
            for value, label in categories:
                ttk.Radiobutton(
                    reason_dialog,
                    text=label,
                    variable=reason_category,
                    value=value
                ).pack(padx=20, anchor=tk.W)
            
            # Additional details
            ttk.Label(reason_dialog, text="Additional details:", font=('Arial', 9, 'bold')).pack(padx=10, pady=(10, 5), anchor=tk.W)
            reason_text = scrolledtext.ScrolledText(
                reason_dialog,
                wrap=tk.WORD,
                font=('Arial', 9),
                height=6
            )
            reason_text.pack(padx=10, pady=(0, 10), fill=tk.BOTH, expand=True)
            
            def submit_rejection():
                category = reason_category.get()
                details = reason_text.get('1.0', tk.END).strip()
                
                if not details and category == "other":
                    messagebox.showwarning("Missing Details", "Please provide details for 'Other' category.")
                    return
                
                # Store rejection feedback
                feedback_entry = {
                    'violation_id': violation.get('violation_id'),
                    'rule': violation.get('rule'),
                    'file': violation.get('file'),
                    'line': violation.get('line'),
                    'fix_suggestion': fix_data.get('fix_suggestion', '')[:200],
                    'rejection_category': category,
                    'rejection_reason': details,
                    'timestamp': datetime.now().isoformat()
                }
                rejection_feedback.append(feedback_entry)
                
                # Log to GUI
                self.log_output(f"🚫 Rejected fix for {violation.get('violation_id')}\n")
                self.log_output(f"   Category: {category}\n")
                self.log_output(f"   Reason: {details[:100]}...\n" if len(details) > 100 else f"   Reason: {details}\n")
                
                stats['rejected'] += 1
                update_stats()
                reason_dialog.destroy()
                current_idx[0] += 1
                current_fix_data[0] = None  # Clear cached fix to force regeneration
                load_current_fix()
            
            # Buttons
            btn_frame = ttk.Frame(reason_dialog)
            btn_frame.pack(fill=tk.X, padx=10, pady=10)
            
            ttk.Button(btn_frame, text="Submit Rejection", command=submit_rejection).pack(side=tk.LEFT, padx=5)
            ttk.Button(btn_frame, text="Cancel", command=reason_dialog.destroy).pack(side=tk.LEFT, padx=5)
            
            reason_dialog.grab_set()
        
        # Buttons
        ttk.Button(button_frame, text="✅ Accept", command=accept_current, width=15).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="⏭️ Skip", command=skip_current, width=15).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="🚫 Reject", command=reject_current, width=15).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="❌ Cancel", command=dialog.destroy, width=15).pack(side=tk.RIGHT, padx=5)
        
        # Load first fix
        if not load_current_fix():
            return
        
        dialog.transient(self.root)
        dialog.grab_set()
    
    def _load_rule_html_documentation(self, rule_id: str, violation_id: str) -> str:
        """
        Load and parse HTML documentation for a Parasoft rule
        
        Args:
            rule_id: The rule ID (e.g., 'CERT_C-INT31-i')
            violation_id: Full violation ID (e.g., 'CERT_C-INT31-i-2')
        
        Returns:
            Formatted text documentation or empty string if not found
        """
        try:
            import re
            from html.parser import HTMLParser
            
            # Extract base rule ID from violation_id if rule_id is UNKNOWN
            if rule_id == 'UNKNOWN' and violation_id != 'UNKNOWN':
                # Extract pattern like CERT_C-INT31-i from CERT_C-INT31-i-2
                match = re.match(r'^([A-Z_]+-[A-Z0-9_]+-[a-z]+)', violation_id)
                if match:
                    rule_id = match.group(1)
            
            if rule_id == 'UNKNOWN':
                return ""
            
            # Build HTML file path
            html_file = self.project_root / 'data' / 'Parasoft_Enabled_Rules_List' / 'gendoc' / f'{rule_id}.html'
            
            if not html_file.exists():
                # Try without the sub-rule letter (e.g., CERT_C-INT31 instead of CERT_C-INT31-i)
                base_rule_id = re.sub(r'-[a-z]+$', '', rule_id)
                html_file = self.project_root / 'data' / 'Parasoft_Enabled_Rules_List' / 'gendoc' / f'{base_rule_id}.html'
                
                if not html_file.exists():
                    return ""
            
            # Simple HTML parser to extract text content
            class SimpleHTMLParser(HTMLParser):
                def __init__(self):
                    super().__init__()
                    self.text_parts = []
                    self.in_title = False
                    self.in_strong = False
                    self.in_pre = False
                    self.current_section = None
                    
                def handle_starttag(self, tag, attrs):
                    if tag == 'title':
                        self.in_title = True
                    elif tag == 'strong':
                        self.in_strong = True
                    elif tag == 'pre':
                        self.in_pre = True
                    
                def handle_endtag(self, tag):
                    if tag == 'title':
                        self.in_title = False
                        self.text_parts.append('\n')
                    elif tag == 'strong':
                        self.in_strong = False
                        self.text_parts.append('\n')
                    elif tag == 'pre':
                        self.in_pre = False
                        self.text_parts.append('\n')
                    elif tag == 'br':
                        self.text_parts.append('\n')
                
                def handle_data(self, data):
                    data = data.strip()
                    if data:
                        if self.in_strong:
                            # Section headers
                            if data in ['DESCRIPTION', 'BENEFITS', 'EXAMPLE', 'REPAIR', 'NOTES', 'SECURITY RELEVANCE']:
                                self.text_parts.append(f"\n{'='*70}\n")
                                self.text_parts.append(f"{data}\n")
                                self.text_parts.append(f"{'='*70}\n")
                                self.current_section = data
                            else:
                                self.text_parts.append(f"{data}\n")
                        else:
                            self.text_parts.append(f"{data}\n")
            
            # Read and parse HTML
            with open(html_file, 'r', encoding='utf-8', errors='ignore') as f:
                html_content = f.read()
            
            parser = SimpleHTMLParser()
            parser.feed(html_content)
            
            # Join text parts and clean up
            doc_text = ''.join(parser.text_parts)
            
            # Limit to first 2000 characters to avoid overwhelming the UI
            if len(doc_text) > 2000:
                doc_text = doc_text[:2000] + "\n\n... (documentation truncated, see HTML file for full details)"
            
            return doc_text
            
        except Exception as e:
            return f"⚠️  Error loading rule documentation: {str(e)}"
    
    def _save_fix_rejection_feedback(self, rejection_feedback):
        """Save fix rejection feedback for regression learning"""
        try:
            # Create learning directory if it doesn't exist
            learning_dir = self.project_root / "learning"
            learning_dir.mkdir(exist_ok=True)
            
            # Save to rejection feedback file
            timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
            feedback_file = learning_dir / f"fix_rejection_feedback_{timestamp}.json"
            
            with open(feedback_file, 'w', encoding='utf-8') as f:
                json.dump({
                    'timestamp': datetime.now().isoformat(),
                    'feedback_count': len(rejection_feedback),
                    'rejections': rejection_feedback
                }, f, indent=2)
            
            self.log_output(f"\n✅ Saved {len(rejection_feedback)} fix rejection feedback entries to:\n")
            self.log_output(f"   {feedback_file}\n\n")
            
            # Also integrate with FeedbackLearner if available
            try:
                from FeedbackLearner import FeedbackLearner, FeedbackType
                
                learner = FeedbackLearner(
                    learning_db_path=learning_dir / "fix_learning.json"
                )
                
                for feedback in rejection_feedback:
                    # Map rejection categories to feedback types
                    category_map = {
                        'incorrect_fix': FeedbackType.FALSE_POSITIVE,
                        'breaks_code': FeedbackType.SUPPRESSION_ERROR,
                        'poor_quality': FeedbackType.SUPPRESSION_ERROR,
                        'not_applicable': FeedbackType.FALSE_POSITIVE,
                        'better_alternative': FeedbackType.SUPPRESSION_ERROR,
                        'other': FeedbackType.SUPPRESSION_ERROR
                    }
                    
                    feedback_type = category_map.get(
                        feedback['rejection_category'],
                        FeedbackType.SUPPRESSION_ERROR
                    )
                    
                    # Add to learning system
                    learner.add_feedback(
                        feedback_type=feedback_type,
                        rule_id=feedback['rule'],
                        file_path=feedback['file'],
                        line_number=feedback['line'],
                        code_snippet=feedback['fix_suggestion'],
                        user_comment=f"{feedback['rejection_category']}: {feedback['rejection_reason']}",
                        severity='medium'
                    )
                
                # Save updated learning database
                learner.save_databases()
                
                self.log_output(f"✅ Integrated {len(rejection_feedback)} rejections into FeedbackLearner\n\n")
                
            except ImportError:
                self.log_output("⚠️ FeedbackLearner not available - feedback saved to JSON only\n\n")
            except Exception as e:
                self.log_output(f"⚠️ Could not integrate with FeedbackLearner: {str(e)}\n\n")
        
        except Exception as e:
            self.log_output(f"❌ Error saving fix rejection feedback: {str(e)}\n\n")
    
    def _save_rejection_feedback(self, rejection_feedback):
        """Save rejection feedback for regression learning"""
        try:
            # Create learning directory if it doesn't exist
            learning_dir = self.project_root / "learning"
            learning_dir.mkdir(exist_ok=True)
            
            # Save to rejection feedback file
            timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
            feedback_file = learning_dir / f"suppression_rejection_feedback_{timestamp}.json"
            
            with open(feedback_file, 'w', encoding='utf-8') as f:
                json.dump({
                    'timestamp': datetime.now().isoformat(),
                    'feedback_count': len(rejection_feedback),
                    'rejections': rejection_feedback
                }, f, indent=2)
            
            self.log_output(f"\n✅ Saved {len(rejection_feedback)} rejection feedback entries to:\n")
            self.log_output(f"   {feedback_file}\n\n")
            
            # Also integrate with FeedbackLearner if available
            try:
                from FeedbackLearner import FeedbackLearner, FeedbackType
                
                learner = FeedbackLearner(
                    learning_db_path=learning_dir / "suppression_learning.json"
                )
                
                for feedback in rejection_feedback:
                    # Map rejection categories to feedback types
                    category_map = {
                        'false_positive': FeedbackType.FALSE_POSITIVE,
                        'incorrect_justification': FeedbackType.SUPPRESSION_ERROR,
                        'wrong_rule': FeedbackType.FALSE_POSITIVE,
                        'wrong_format': FeedbackType.SUPPRESSION_ERROR,
                        'duplicate': FeedbackType.SUPPRESSION_ERROR,
                        'other': FeedbackType.SUPPRESSION_ERROR
                    }
                    
                    feedback_type = category_map.get(
                        feedback['rejection_category'],
                        FeedbackType.SUPPRESSION_ERROR
                    )
                    
                    # Add to learning system
                    learner.add_feedback(
                        feedback_type=feedback_type,
                        rule_id=feedback['rule_id'],
                        file_path=feedback['file'],
                        line_number=feedback['line'],
                        code_snippet=feedback['suppression_comment'],
                        user_comment=f"{feedback['rejection_category']}: {feedback['rejection_reason']}",
                        severity='medium'
                    )
                
                # Save updated learning database
                learner.save_databases()
                
                self.log_output(f"✅ Integrated {len(rejection_feedback)} rejections into FeedbackLearner\n\n")
                
            except ImportError:
                self.log_output("⚠️ FeedbackLearner not available - feedback saved to JSON only\n\n")
            except Exception as e:
                self.log_output(f"⚠️ Could not integrate with FeedbackLearner: {str(e)}\n\n")
        
        except Exception as e:
            self.log_output(f"❌ Error saving rejection feedback: {str(e)}\n\n")
    
    
    def _query_master_knowledge_dialog(self):
        """Show interactive dialog for querying master knowledge base"""
        # Create a new top-level window
        dialog = tk.Toplevel(self.root)
        dialog.title("Query Master Knowledge Base")
        dialog.geometry("500x600")
        dialog.transient(self.root)
        dialog.grab_set()
        
        # Center the dialog
        dialog.update_idletasks()
        x = (dialog.winfo_screenwidth() // 2) - (500 // 2)
        y = (dialog.winfo_screenheight() // 2) - (600 // 2)
        dialog.geometry(f"500x600+{x}+{y}")
        
        # Title
        title_frame = ttk.Frame(dialog)
        title_frame.pack(fill='x', padx=20, pady=20)
        
        title_label = ttk.Label(
            title_frame,
            text="🔍 Query Master Knowledge Base",
            font=('Segoe UI', 16, 'bold')
        )
        title_label.pack()
        
        subtitle_label = ttk.Label(
            title_frame,
            text="Select an operation to perform on the consolidated knowledge database",
            font=('Segoe UI', 9)
        )
        subtitle_label.pack(pady=(5, 0))
        
        # Separator
        ttk.Separator(dialog, orient='horizontal').pack(fill='x', padx=20)
        
        # Button frame with scrollbar
        canvas_frame = ttk.Frame(dialog)
        canvas_frame.pack(fill='both', expand=True, padx=20, pady=10)
        
        canvas = tk.Canvas(canvas_frame, highlightthickness=0)
        scrollbar = ttk.Scrollbar(canvas_frame, orient="vertical", command=canvas.yview)
        scrollable_frame = ttk.Frame(canvas)
        
        scrollable_frame.bind(
            "<Configure>",
            lambda e: canvas.configure(scrollregion=canvas.bbox("all"))
        )
        
        canvas.create_window((0, 0), window=scrollable_frame, anchor="nw")
        canvas.configure(yscrollcommand=scrollbar.set)
        
        canvas.pack(side="left", fill="both", expand=True)
        scrollbar.pack(side="right", fill="y")
        
        # Query options with descriptions
        options = [
            ("📊 Summary Statistics", "summary", "View overall statistics and module breakdown"),
            ("🔄 Cross-Module Violations", "cross_module", "Show violations appearing in multiple modules"),
            ("🔍 Violation Insights", "violation_insights", "Get detailed insights for a specific violation ID"),
            ("📈 Top Common Violations", "top_violations", "Display most frequently occurring violations"),
            ("📂 Violations by Category", "by_category", "Filter violations by category (e.g., Type Safety)"),
            ("⚠️ Violations by Severity", "by_severity", "Filter violations by severity (HIGH/MEDIUM/LOW)"),
            ("✅ Proven Fixes Available", "proven_fixes", "Find violations with successful fix examples"),
            ("💡 Recommendations", "recommendations", "Get AI-powered recommendations"),
            ("📑 Export to Excel", "export_excel", "Export master knowledge to Excel file"),
            ("🌐 Export to HTML", "export_html", "Export master knowledge to HTML report"),
        ]
        
        def execute_query(action, extra_param=None):
            """Execute the selected query"""
            dialog.destroy()
            
            # Build command based on action
            script = self.project_root / "src" / "query_master_knowledge.py"
            cmd = [sys.executable, str(script), f"--{action}"]
            
            # Add extra parameter if needed
            if extra_param:
                cmd.append(extra_param)
            
            # Execute in a thread
            thread = threading.Thread(
                target=self._execute_query_command,
                args=(cmd, action),
                daemon=True
            )
            thread.start()
        
        def prompt_and_execute(action, prompt_text, default_value=""):
            """Prompt user for input then execute"""
            dialog.destroy()
            
            # Create simple input dialog
            input_dialog = tk.Toplevel(self.root)
            input_dialog.title("Input Required")
            input_dialog.geometry("400x150")
            input_dialog.transient(self.root)
            input_dialog.grab_set()
            
            # Center the dialog
            input_dialog.update_idletasks()
            x = (input_dialog.winfo_screenwidth() // 2) - (200)
            y = (input_dialog.winfo_screenheight() // 2) - (75)
            input_dialog.geometry(f"400x150+{x}+{y}")
            
            ttk.Label(input_dialog, text=prompt_text, font=('Segoe UI', 10)).pack(pady=20)
            
            entry_var = tk.StringVar(value=default_value)
            entry = ttk.Entry(input_dialog, textvariable=entry_var, width=40)
            entry.pack(pady=10)
            entry.focus()
            
            def on_ok():
                value = entry_var.get().strip()
                input_dialog.destroy()
                if value:
                    execute_query(action, value)
            
            def on_cancel():
                input_dialog.destroy()
            
            btn_frame = ttk.Frame(input_dialog)
            btn_frame.pack(pady=10)
            
            ttk.Button(btn_frame, text="OK", command=on_ok).pack(side='left', padx=5)
            ttk.Button(btn_frame, text="Cancel", command=on_cancel).pack(side='left', padx=5)
            
            entry.bind('<Return>', lambda e: on_ok())
            entry.bind('<Escape>', lambda e: on_cancel())
        
        # Create buttons for each option
        for i, (label, action, description) in enumerate(options):
            btn_frame = ttk.Frame(scrollable_frame)
            btn_frame.pack(fill='x', pady=5)
            
            if action in ["violation_insights", "by_category", "by_severity", "top_violations"]:
                # These need user input
                if action == "violation_insights":
                    cmd_func = lambda a=action: prompt_and_execute(a, "Enter Violation ID (e.g., CERT_C-INT31-i-2):")
                elif action == "by_category":
                    cmd_func = lambda a=action: prompt_and_execute(a, "Enter Category (e.g., Type Safety):")
                elif action == "by_severity":
                    cmd_func = lambda a=action: prompt_and_execute(a, "Enter Severity (HIGH/MEDIUM/LOW):", "HIGH")
                elif action == "top_violations":
                    cmd_func = lambda a=action: prompt_and_execute(a, "How many violations to show?", "10")
            else:
                cmd_func = lambda a=action: execute_query(a)
            
            btn = ttk.Button(
                btn_frame,
                text=label,
                command=cmd_func,
                width=45
            )
            btn.pack(side='left')
            
            # Description label
            desc_label = ttk.Label(
                scrollable_frame,
                text=f"   → {description}",
                font=('Segoe UI', 8),
                foreground='gray'
            )
            desc_label.pack(anchor='w', pady=(0, 10))
        
        # Close button
        close_frame = ttk.Frame(dialog)
        close_frame.pack(fill='x', padx=20, pady=(0, 20))
        
        ttk.Button(
            close_frame,
            text="❌ Close",
            command=dialog.destroy
        ).pack()
    
    def _execute_query_command(self, cmd, action_name):
        """Execute a query command in background"""
        self.update_status(f"Querying: {action_name}")
        self.log_output(f"\n{'='*70}\n")
        self.log_output(f"▶️ Querying Master Knowledge Base\n")
        self.log_output(f"🔍 Action: {action_name}\n")
        self.log_output(f"{'='*70}\n\n")
        
        try:
            self.running_process = subprocess.Popen(
                cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                encoding='utf-8',
                errors='replace',
                bufsize=1,
                cwd=str(self.project_root)
            )
            
            # Enable stop button
            self.root.after(0, lambda: self.stop_btn.config(state='normal'))
            
            # Read output
            for line in iter(self.running_process.stdout.readline, ''):
                if line:
                    self.log_output(line)
            
            return_code = self.running_process.wait()
            
            self.log_output(f"\n{'='*70}\n")
            if return_code == 0:
                self.log_output("✅ Query completed successfully!\n")
            else:
                self.log_output(f"⚠️ Process exited with code: {return_code}\n")
            self.log_output(f"{'='*70}\n\n")
            
        except Exception as e:
            self.log_output(f"\n❌ Error: {str(e)}\n")
        finally:
            self.running_process = None
            self.root.after(0, lambda: self.stop_btn.config(state='disabled'))
            self.update_status("Query completed")
    
    def stop_process(self):
        """Stop the running process"""
        if self.running_process:
            try:
                self.running_process.terminate()
                self.log_output("\n⏹️ Process stopped by user\n")
                self.update_status("Process stopped")
            except:
                pass
        
    def log_output(self, text):
        """Add text to output log (thread-safe)"""
        self.root.after(0, self._append_text, text)
        
    def _append_text(self, text):
        """Append text to output widget (must run in main thread)"""
        self.output_text.insert(tk.END, text)
        self.output_text.see(tk.END)
        
    def update_status(self, text):
        """Update status bar (thread-safe)"""
        self.root.after(0, lambda: self.status_bar.config(text=text))
        
    def clear_output(self):
        """Clear output log"""
        self.output_text.delete('1.0', tk.END)
        self.log_output("🗑️ Log cleared\n\n")
        
    def save_log(self):
        """Save output log to file"""
        filename = filedialog.asksaveasfilename(
            defaultextension=".txt",
            filetypes=[("Text files", "*.txt"), ("All files", "*.*")],
            initialfile="parasoft_log.txt"
        )
        
        if filename:
            try:
                with open(filename, 'w', encoding='utf-8') as f:
                    content = self.output_text.get('1.0', tk.END)
                    f.write(content)
                messagebox.showinfo("Success", f"Log saved to:\n{filename}")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to save log:\n{str(e)}")
    
    def browse_input_path(self):
        """Browse and select input path directory"""
        current_path = self.input_path_entry.get().strip()
        if current_path and Path(current_path).exists():
            initial_dir = current_path
        else:
            initial_dir = str(self.project_root / "Inputs")
        
        directory = filedialog.askdirectory(
            title="Select Input Source Code Directory",
            initialdir=initial_dir
        )
        
        if directory:
            self.input_path_entry.delete(0, tk.END)
            self.input_path_entry.insert(0, directory)
            self.log_output(f"[INFO] Input path updated: {directory}\n")
    
    def refresh_ollama_models(self):
        """Fetch and populate available Ollama models dynamically"""
        def fetch_models():
            try:
                import requests
                
                # Query Ollama for available models
                response = requests.get('http://localhost:11434/api/tags', timeout=5)
                
                if response.status_code == 200:
                    models_data = response.json()
                    models = models_data.get('models', [])
                    
                    if models:
                        # Extract model names and sort them
                        model_names = [model.get('name', 'unknown') for model in models]
                        model_names.sort()
                        
                        # Get model sizes for display
                        model_info = []
                        for model in models:
                            name = model.get('name', 'unknown')
                            size = model.get('size', 0)
                            size_mb = size / (1024 * 1024)
                            
                            # Add size info for user-friendly display
                            if size_mb < 1024:
                                display_name = f"{name} ({size_mb:.0f}MB)"
                            else:
                                size_gb = size_mb / 1024
                                display_name = f"{name} ({size_gb:.1f}GB)"
                            
                            model_info.append((name, display_name))
                        
                        # Update dropdown in main thread
                        self.root.after(0, lambda: self._update_model_dropdown(model_names, model_info))
                        self.root.after(0, lambda: self.log_output(
                            f"[INFO] ✅ Loaded {len(model_names)} Ollama model(s)\n"))
                    else:
                        # No models found
                        self.root.after(0, lambda: self._update_model_dropdown(
                            ["No models found"], [("No models found", "No models found")]))
                        self.root.after(0, lambda: self.log_output(
                            f"[WARNING] No Ollama models found. Download a model:\n"))
                        self.root.after(0, lambda: self.log_output(
                            f"[INFO]   ollama pull qwen2.5:latest\n"))
                else:
                    raise Exception(f"HTTP {response.status_code}")
                    
            except ImportError:
                self.root.after(0, lambda: self._update_model_dropdown(
                    ["requests library required"], [("error", "Install requests library")]))
                self.root.after(0, lambda: self.log_output(
                    f"[ERROR] Cannot load models: requests library not installed\n"))
            except Exception as e:
                # Ollama not running or connection failed
                self.root.after(0, lambda: self._update_model_dropdown(
                    ["Ollama not running"], [("error", "Start Ollama first")]))
                self.root.after(0, lambda: self.log_output(
                    f"[WARNING] Cannot load Ollama models: {str(e)}\n"))
                self.root.after(0, lambda: self.log_output(
                    f"[INFO] Start Ollama service to see available models\n"))
        
        # Run in background thread
        thread = threading.Thread(target=fetch_models, daemon=True)
        thread.start()
    
    def _update_model_dropdown(self, model_names, model_info):
        """Update the model dropdown with fetched models (runs in main thread)"""
        # Extract just the names for the combobox values
        display_names = [info[1] for info in model_info] if model_info else model_names
        
        # Update combobox
        self.model_combo['values'] = display_names
        
        # Set default selection to first model
        if model_names and model_names[0] not in ["No models found", "Ollama not running", "requests library required"]:
            # Store the mapping of display name to actual model name
            if not hasattr(self, 'model_name_map'):
                self.model_name_map = {}
            
            for actual_name, display_name in model_info:
                self.model_name_map[display_name] = actual_name
            
            # Select first model
            self.model_combo.current(0)
            self.model_var.set(display_names[0])
        else:
            self.model_var.set(model_names[0] if model_names else "No models available")
    
    def show_install_models_dialog(self):
        """Show dialog to install popular Ollama models"""
        dialog = tk.Toplevel(self.root)
        dialog.title("📥 Install Ollama Models")
        dialog.geometry("700x550")
        dialog.transient(self.root)
        dialog.grab_set()
        
        # Center the dialog
        dialog.update_idletasks()
        x = (dialog.winfo_screenwidth() // 2) - (700 // 2)
        y = (dialog.winfo_screenheight() // 2) - (550 // 2)
        dialog.geometry(f'+{x}+{y}')
        
        # Main container
        main_frame = ttk.Frame(dialog, padding="15")
        main_frame.pack(fill=tk.BOTH, expand=True)
        
        # Header
        header_label = ttk.Label(
            main_frame,
            text="📦 Install Ollama Models",
            font=('Arial', 14, 'bold')
        )
        header_label.pack(pady=(0, 10))
        
        info_label = ttk.Label(
            main_frame,
            text="Select models to install. Installation may take several minutes depending on model size.",
            font=('Arial', 9),
            foreground='#7F8C8D',
            wraplength=650
        )
        info_label.pack(pady=(0, 15))
        
        # Check if Ollama is running
        ollama_running = self._check_ollama_service()
        
        if not ollama_running:
            warning_frame = ttk.Frame(main_frame)
            warning_frame.pack(fill=tk.X, pady=(0, 15))
            
            warning_label = ttk.Label(
                warning_frame,
                text="⚠️ WARNING: Ollama service is not running!",
                font=('Arial', 11, 'bold'),
                foreground='#E74C3C'
            )
            warning_label.pack(anchor=tk.W)
            
            help_label = ttk.Label(
                warning_frame,
                text="Please start Ollama by running 'ollama serve' in a terminal, then click 'Check Again'.",
                font=('Arial', 9),
                foreground='#E67E22'
            )
            help_label.pack(anchor=tk.W, pady=(5, 0))
            
            button_frame = ttk.Frame(warning_frame)
            button_frame.pack(anchor=tk.W, pady=(10, 0))
            
            ttk.Button(
                button_frame,
                text="🔄 Check Again",
                command=lambda: [dialog.destroy(), self.show_install_models_dialog()]
            ).pack(side=tk.LEFT, padx=(0, 5))
            
            ttk.Button(
                button_frame,
                text="❌ Close",
                command=dialog.destroy
            ).pack(side=tk.LEFT)
            
            return
        
        # Model selection area
        models_frame = ttk.LabelFrame(main_frame, text="Available Models", padding="10")
        models_frame.pack(fill=tk.BOTH, expand=True, pady=(0, 15))
        
        # Define popular models with descriptions
        models = [
            {
                'name': 'qwen2.5:latest',
                'size': '4.7GB',
                'description': 'Qwen 2.5 (7B) - Best for code analysis, RECOMMENDED',
                'recommended': True
            },
            {
                'name': 'qwen2.5:1.5b',
                'size': '986MB',
                'description': 'Qwen 2.5 (1.5B) - Smaller, faster, good for quick tasks',
                'recommended': False
            },
            {
                'name': 'llama3.2:latest',
                'size': '2.0GB',
                'description': 'Llama 3.2 (3B) - Good alternative, balanced performance',
                'recommended': False
            },
            {
                'name': 'llama3.2:1b',
                'size': '1.3GB',
                'description': 'Llama 3.2 (1B) - Lightweight, fast responses',
                'recommended': False
            }
        ]
        
        # Store checkbox variables
        checkbox_vars = []
        
        for i, model in enumerate(models):
            model_frame = ttk.Frame(models_frame)
            model_frame.pack(fill=tk.X, pady=5)
            
            var = tk.BooleanVar(value=False)
            checkbox_vars.append((var, model['name']))
            
            checkbox = ttk.Checkbutton(
                model_frame,
                variable=var,
                text=""
            )
            checkbox.pack(side=tk.LEFT, padx=(0, 5))
            
            # Model name and size
            name_label = ttk.Label(
                model_frame,
                text=f"{model['name']}",
                font=('Consolas', 10, 'bold'),
                foreground='#2C3E50'
            )
            name_label.pack(side=tk.LEFT, padx=(0, 10))
            
            size_label = ttk.Label(
                model_frame,
                text=f"({model['size']})",
                font=('Arial', 9),
                foreground='#7F8C8D'
            )
            size_label.pack(side=tk.LEFT, padx=(0, 10))
            
            # Recommended badge
            if model.get('recommended'):
                rec_label = ttk.Label(
                    model_frame,
                    text="⭐ RECOMMENDED",
                    font=('Arial', 8, 'bold'),
                    foreground='#27AE60'
                )
                rec_label.pack(side=tk.LEFT, padx=(0, 10))
            
            # Description
            desc_label = ttk.Label(
                model_frame,
                text=model['description'],
                font=('Arial', 9),
                foreground='#555555'
            )
            desc_label.pack(side=tk.LEFT)
        
        # Progress area
        progress_frame = ttk.LabelFrame(main_frame, text="Installation Progress", padding="10")
        progress_frame.pack(fill=tk.X, pady=(0, 15))
        
        progress_text = scrolledtext.ScrolledText(
            progress_frame,
            height=6,
            font=('Consolas', 9),
            bg='#F8F9FA',
            wrap=tk.WORD
        )
        progress_text.pack(fill=tk.BOTH, expand=True)
        progress_text.config(state='disabled')
        
        # Buttons
        button_frame = ttk.Frame(main_frame)
        button_frame.pack(fill=tk.X)
        
        def start_installation():
            selected_models = [model_name for var, model_name in checkbox_vars if var.get()]
            
            if not selected_models:
                progress_text.config(state='normal')
                progress_text.delete('1.0', tk.END)
                progress_text.insert('1.0', "⚠️ Please select at least one model to install.\n")
                progress_text.config(state='disabled')
                return
            
            # Disable checkboxes and install button during installation
            for widget in models_frame.winfo_children():
                for child in widget.winfo_children():
                    if isinstance(child, ttk.Checkbutton):
                        child.config(state='disabled')
            
            install_btn.config(state='disabled')
            
            # Start installation
            self._install_selected_models(selected_models, progress_text, dialog, install_btn, models_frame)
        
        install_btn = ttk.Button(
            button_frame,
            text="📥 Install Selected",
            command=start_installation
        )
        install_btn.pack(side=tk.LEFT, padx=(0, 10))
        
        ttk.Button(
            button_frame,
            text="❌ Close",
            command=dialog.destroy
        ).pack(side=tk.LEFT)
        
        # Tip
        tip_label = ttk.Label(
            main_frame,
            text="💡 Tip: After installation, click '🔄 Refresh Models' in the AI Chat tab to see new models.",
            font=('Arial', 8),
            foreground='#7F8C8D',
            wraplength=650
        )
        tip_label.pack(side=tk.BOTTOM, pady=(10, 0))
    
    def _check_ollama_service(self):
        """Check if Ollama service is running"""
        try:
            import requests
            response = requests.get('http://localhost:11434/api/tags', timeout=2)
            return response.status_code == 200
        except:
            return False
    
    def _install_selected_models(self, models, progress_widget, dialog, install_btn, models_frame):
        """Install selected Ollama models"""
        def update_progress(message):
            """Thread-safe progress update"""
            def _update():
                progress_widget.config(state='normal')
                progress_widget.insert(tk.END, message)
                progress_widget.see(tk.END)
                progress_widget.config(state='disabled')
            self.root.after(0, _update)
        
        def install_models_thread():
            total = len(models)
            update_progress(f"📦 Starting installation of {total} model(s)...\n\n")
            
            for idx, model_name in enumerate(models, 1):
                update_progress(f"[{idx}/{total}] Installing {model_name}...\n")
                update_progress("=" * 60 + "\n")
                
                try:
                    # Use PowerShell to run ollama pull
                    import subprocess
                    
                    process = subprocess.Popen(
                        ['ollama', 'pull', model_name],
                        stdout=subprocess.PIPE,
                        stderr=subprocess.STDOUT,
                        text=True,
                        bufsize=1,
                        shell=True
                    )
                    
                    # Stream output in real-time
                    for line in process.stdout:
                        update_progress(line)
                    
                    process.wait()
                    
                    if process.returncode == 0:
                        update_progress(f"\n✅ Successfully installed {model_name}\n\n")
                        self.root.after(0, lambda: self.log_output(f"[SUCCESS] ✅ Installed Ollama model: {model_name}\n"))
                    else:
                        update_progress(f"\n❌ Failed to install {model_name} (exit code: {process.returncode})\n\n")
                        self.root.after(0, lambda m=model_name: self.log_output(f"[ERROR] ❌ Failed to install model: {m}\n"))
                
                except FileNotFoundError:
                    update_progress(f"\n❌ ERROR: 'ollama' command not found!\n")
                    update_progress("Make sure Ollama is installed and added to PATH.\n")
                    update_progress("Download from: https://ollama.com/download\n\n")
                    self.root.after(0, lambda: self.log_output("[ERROR] Ollama CLI not found in PATH\n"))
                    break
                
                except Exception as e:
                    update_progress(f"\n❌ ERROR installing {model_name}: {str(e)}\n\n")
                    self.root.after(0, lambda m=model_name, e=str(e): self.log_output(f"[ERROR] Failed to install {m}: {e}\n"))
            
            # Installation complete
            update_progress("\n" + "=" * 60 + "\n")
            update_progress("🎉 Installation process completed!\n")
            update_progress("\n💡 Click '🔄 Refresh Models' in AI Chat to see your new models.\n")
            
            # Re-enable UI elements
            def enable_ui():
                install_btn.config(state='normal')
                for widget in models_frame.winfo_children():
                    for child in widget.winfo_children():
                        if isinstance(child, ttk.Checkbutton):
                            child.config(state='normal')
            
            self.root.after(0, enable_ui)
        
        # Run installation in background thread
        thread = threading.Thread(target=install_models_thread, daemon=True)
        thread.start()
    
    def test_ollama_connection(self):
        """Test connection to Ollama server"""
        # Get actual model name from display name
        display_name = self.model_var.get()
        
        if hasattr(self, 'model_name_map') and display_name in self.model_name_map:
            model = self.model_name_map[display_name]
        else:
            model = display_name
        self.ollama_status_label.config(text="🔄 Testing...", foreground='#F39C12')
        self.log_output(f"[INFO] Testing Ollama connection (model: {model})...\n")
        
        def test():
            try:
                # Try to connect to Ollama
                import requests
                
                # First, check if Ollama is running at all (quick check)
                try:
                    version_response = requests.get('http://localhost:11434/api/version', timeout=3)
                    if version_response.status_code != 200:
                        raise Exception("Ollama service not responding")
                except Exception as e:
                    raise Exception(f"Ollama not running or not accessible: {str(e)}")
                
                # Now test the model with increased timeout (model loading can take 30+ seconds)
                self.root.after(0, lambda: self.log_output(
                    f"[INFO] Ollama service detected. Loading model '{model}'...\n"))
                self.root.after(0, lambda: self.log_output(
                    f"[INFO] This may take 10-30 seconds for first load...\n"))
                
                response = requests.post(
                    'http://localhost:11434/api/generate',
                    json={
                        'model': model,
                        'prompt': 'Hi',
                        'stream': False
                    },
                    timeout=60  # Increased timeout to 60 seconds for model loading
                )
                
                if response.status_code == 200:
                    self.root.after(0, lambda: self.ollama_status_label.config(
                        text="✅ Connected", foreground='#27AE60'))
                    self.root.after(0, lambda: self.log_output(
                        f"[INFO] ✅ Ollama connection successful! Model '{model}' is ready.\n"))
                else:
                    self.root.after(0, lambda: self.ollama_status_label.config(
                        text="❌ Failed", foreground='#E74C3C'))
                    self.root.after(0, lambda: self.log_output(
                        f"[ERROR] Ollama returned status {response.status_code}\n"))
                    self.root.after(0, lambda: self.log_output(
                        f"[ERROR] Response: {response.text[:200]}\n"))
            except ImportError:
                self.root.after(0, lambda: self.ollama_status_label.config(
                    text="❌ Install requests", foreground='#E74C3C'))
                self.root.after(0, lambda: messagebox.showerror(
                    "Missing Dependency",
                    "Please install 'requests' library:\npip install requests"))
            except requests.exceptions.Timeout:
                self.root.after(0, lambda: self.ollama_status_label.config(
                    text="⏱️ Timeout", foreground='#E67E22'))
                self.root.after(0, lambda: self.log_output(
                    f"[WARNING] Connection timed out after 60 seconds.\n"))
                self.root.after(0, lambda: self.log_output(
                    f"[INFO] Model '{model}' may be too large or not downloaded.\n"))
                self.root.after(0, lambda: self.log_output(
                    f"[INFO] Try a smaller model like 'tinyllama:latest' or download this model:\n"))
                self.root.after(0, lambda: self.log_output(
                    f"[INFO]   ollama pull {model}\n"))
                self.root.after(0, lambda: messagebox.showwarning(
                    "Connection Timeout",
                    f"Connection timed out after 60 seconds.\n\n"
                    f"Possible causes:\n"
                    f"• Model '{model}' is too large and taking long to load\n"
                    f"• Model not downloaded yet\n"
                    f"• System resources insufficient\n\n"
                    f"Try:\n"
                    f"1. Select a smaller model (tinyllama:latest)\n"
                    f"2. Download the model: ollama pull {model}\n"
                    f"3. Wait longer and try again"))
            except Exception as e:
                error_msg = str(e)
                self.root.after(0, lambda: self.ollama_status_label.config(
                    text="❌ Error", foreground='#E74C3C'))
                self.root.after(0, lambda: self.log_output(
                    f"[ERROR] Ollama connection failed: {error_msg}\n"))
                
                # Provide helpful error messages
                if "Connection refused" in error_msg or "not running" in error_msg:
                    self.root.after(0, lambda: messagebox.showerror(
                        "Ollama Not Running",
                        f"Could not connect to Ollama.\n\n"
                        f"Make sure Ollama is running:\n"
                        f"• Check Task Manager for 'ollama.exe'\n"
                        f"• Or start: ollama serve\n\n"
                        f"Error: {error_msg}"))
                else:
                    self.root.after(0, lambda: messagebox.showerror(
                        "Connection Error",
                        f"Failed to connect to Ollama.\n\n"
                        f"Error: {error_msg}\n\n"
                        f"Check the Output Log for details."))
        
        thread = threading.Thread(target=test, daemon=True)
        thread.start()
    
    def clear_chat(self):
        """Clear chat history"""
        if messagebox.askyesno("Clear Chat", "Clear all chat history?"):
            self.chat_text.config(state='normal')
            self.chat_text.delete('1.0', tk.END)
            self.chat_text.config(state='disabled')
            self.chat_history = []
            self.clear_attachment()  # Also clear any attachments
            self.log_output("[INFO] Chat history cleared\n")
    
    def attach_file_to_chat(self):
        """Attach a file to the chat for analysis"""
        file_path = filedialog.askopenfilename(
            title="Select File to Attach",
            filetypes=[
                ("C/C++ Files", "*.c *.cpp *.h *.hpp"),
                ("Text Files", "*.txt"),
                ("Log Files", "*.log"),
                ("All Files", "*.*")
            ]
        )
        
        if not file_path:
            return
        
        try:
            # Read file content (limit to 50KB to avoid token limits)
            max_size = 50 * 1024  # 50KB
            with open(file_path, 'r', encoding='utf-8', errors='replace') as f:
                content = f.read(max_size)
            
            self.attached_file_path = file_path
            self.attached_file_content = content
            
            # Update label
            filename = Path(file_path).name
            file_size = len(content)
            self.attachment_label.config(
                text=f"{filename} ({file_size} bytes)",
                foreground='#27AE60'
            )
            
            self.log_output(f"[INFO] File attached: {filename}\n")
            
            if file_size >= max_size:
                messagebox.showwarning(
                    "File Truncated",
                    f"File is large. Only first 50KB will be included.\n\n"
                    f"For better results, analyze specific sections or use smaller files."
                )
        
        except Exception as e:
            messagebox.showerror("Error", f"Could not read file:\n{str(e)}")
            self.clear_attachment()
    
    def clear_attachment(self):
        """Clear attached file"""
        self.attached_file_path = None
        self.attached_file_content = None
        self.attachment_label.config(
            text="No file attached",
            foreground='gray'
        )
    
    def send_prompt(self):
        """Send prompt to Ollama"""
        prompt = self.prompt_text.get('1.0', tk.END).strip()
        
        if not prompt:
            messagebox.showwarning("Empty Prompt", "Please enter a prompt")
            return
        
        if self.chat_running:
            messagebox.showwarning("Busy", "Please wait for current response to complete")
            return
        
        # Build full prompt with attachment if present
        full_prompt = prompt
        has_large_file = False
        if self.attached_file_content:
            filename = Path(self.attached_file_path).name
            file_size = len(self.attached_file_content)
            full_prompt = f"{prompt}\n\n[Attached File: {filename}]\n```\n{self.attached_file_content}\n```"
            
            # Flag if file is large (>10KB) for extended timeout
            if file_size > 10000:
                has_large_file = True
        
        # Add user message to chat
        self.chat_text.config(state='normal')
        if len(self.chat_history) > 0:
            self.chat_text.insert(tk.END, "\n" + "="*70 + "\n\n")
        self.chat_text.insert(tk.END, "👤 You:\n", 'user')
        self.chat_text.insert(tk.END, prompt)
        
        # Show attachment indicator if present
        if self.attached_file_content:
            filename = Path(self.attached_file_path).name
            self.chat_text.insert(tk.END, f"\n📎 Attached: {filename}", 'system')
        
        self.chat_text.insert(tk.END, "\n\n")
        self.chat_text.config(state='disabled')
        self.chat_text.see(tk.END)
        
        # Clear prompt input and attachment
        self.prompt_text.delete('1.0', tk.END)
        if self.attached_file_content:
            self.clear_attachment()
        
        # Disable send button
        self.send_btn.config(state='disabled')
        self.stop_chat_btn.config(state='normal')
        
        # Add to history
        self.chat_history.append({'role': 'user', 'content': full_prompt})
        
        # Send request in thread with timeout flag
        thread = threading.Thread(
            target=self._execute_chat_request,
            args=(full_prompt, has_large_file),
            daemon=True
        )
        thread.start()
    
    def stop_chat_generation(self):
        """Stop ongoing chat generation"""
        self.chat_running = False
    
    def _execute_chat_request(self, prompt, large_file=False):
        """Execute chat request to Ollama (runs in thread)"""
        self.chat_running = True
        
        # Get actual model name from display name
        display_name = self.model_var.get()
        if hasattr(self, 'model_name_map') and display_name in self.model_name_map:
            model = self.model_name_map[display_name]
        else:
            model = display_name
        
        try:
            import requests
            
            # Ensure chat widget is editable and add assistant header
            def add_assistant_header():
                self.chat_text.config(state='normal')
                self.chat_text.insert(tk.END, "🤖 Assistant:\n", 'assistant')
                if large_file:
                    self.chat_text.insert(tk.END, "\n[Processing large file... This may take 2-5 minutes]\n", 'system')
                self.chat_text.see(tk.END)
            
            self.root.after(0, add_assistant_header)
            
            # Small delay to ensure UI updates
            import time
            time.sleep(0.1)
            
            # Determine timeout based on file size
            # Large files need more time for model processing
            timeout_seconds = 300 if large_file else 120
            
            # Stream response from Ollama
            response = requests.post(
                'http://localhost:11434/api/generate',
                json={
                    'model': model,
                    'prompt': prompt,
                    'stream': True
                },
                stream=True,
                timeout=timeout_seconds
            )
            
            full_response = ""
            
            for line in response.iter_lines():
                if not self.chat_running:
                    def add_stop_message():
                        self.chat_text.insert(tk.END, "\n\n[Generation stopped by user]\n", 'system')
                        self.chat_text.see(tk.END)
                    self.root.after(0, add_stop_message)
                    break
                
                if line:
                    import json
                    chunk = json.loads(line)
                    if 'response' in chunk:
                        text = chunk['response']
                        full_response += text
                        
                        def add_text(t=text):
                            self.chat_text.insert(tk.END, t)
                            self.chat_text.see(tk.END)
                        
                        self.root.after(0, add_text)
            
            # Add assistant response to history
            if full_response:
                self.chat_history.append({'role': 'assistant', 'content': full_response})
            
            # Final newline and disable
            def finalize():
                self.chat_text.insert(tk.END, "\n")
                self.chat_text.config(state='disabled')
                self.chat_text.see(tk.END)
            
            self.root.after(0, finalize)
            
        except ImportError:
            def show_import_error():
                self.chat_text.insert(
                    tk.END, "\n[ERROR] requests library not installed. Install with: pip install requests\n", 'error')
                self.chat_text.config(state='disabled')
                self.chat_text.see(tk.END)
            self.root.after(0, show_import_error)
        except requests.exceptions.Timeout:
            timeout_msg = (
                "\n[ERROR] Request timed out. This can happen when:\n"
                "  • File is too large for the current model\n"
                "  • Model is processing slowly\n\n"
                "Solutions:\n"
                "  1. Use a larger model: qwen2.5:latest (4.7GB)\n"
                "  2. Attach smaller file sections\n"
                "  3. Simplify your prompt\n\n"
            )
            def show_timeout_error():
                self.chat_text.insert(tk.END, timeout_msg, 'error')
                self.chat_text.config(state='disabled')
                self.chat_text.see(tk.END)
            self.root.after(0, show_timeout_error)
        except requests.exceptions.ConnectionError:
            conn_msg = (
                "\n[ERROR] Cannot connect to Ollama.\n\n"
                "Make sure Ollama is running:\n"
                "  • Open a terminal and run: ollama serve\n"
                "  • Or start Ollama from the application menu\n\n"
            )
            def show_conn_error():
                self.chat_text.insert(tk.END, conn_msg, 'error')
                self.chat_text.config(state='disabled')
                self.chat_text.see(tk.END)
            self.root.after(0, show_conn_error)
        except Exception as e:
            error_msg = f"\n[ERROR] {str(e)}\n\nMake sure Ollama is running (ollama serve)\n"
            def show_error():
                self.chat_text.insert(tk.END, error_msg, 'error')
                self.chat_text.config(state='disabled')
                self.chat_text.see(tk.END)
            self.root.after(0, show_error)
        finally:
            self.chat_running = False
            self.root.after(0, lambda: self.send_btn.config(state='normal'))
            self.root.after(0, lambda: self.stop_chat_btn.config(state='disabled'))
    
    def _build_context(self):
        """Build context from chat history for Ollama"""
        # Limit context to last 5 exchanges to avoid token limits
        recent_history = self.chat_history[-10:] if len(self.chat_history) > 10 else self.chat_history
        
        context_text = ""
        for msg in recent_history:
            role = msg['role']
            content = msg['content']
            if role == 'user':
                context_text += f"User: {content}\n\n"
            else:
                context_text += f"Assistant: {content}\n\n"
        
        return context_text
    
    def run_startup_checks(self):
        """Run comprehensive startup checks for dependencies and services"""
        self.log_output("\n" + "="*70 + "\n")
        self.log_output("🚀 PARASOFT ANALYSIS TOOL - STARTUP CHECKS\n")
        self.log_output("="*70 + "\n\n")
        
        checks_passed = 0
        checks_failed = 0
        warnings = 0
        
        # Check 1: Python Version
        self.log_output("🔍 Checking Python version...\n")
        python_version = sys.version.split()[0]
        major, minor = sys.version_info[:2]
        if major == 3 and minor >= 8:
            self.log_output(f"  ✅ Python {python_version} (Compatible)\n")
            checks_passed += 1
        else:
            self.log_output(f"  ⚠️ Python {python_version} (Requires 3.8+)\n")
            warnings += 1
        
        # Check 2: Project Structure
        self.log_output("\n🔍 Checking project structure...\n")
        required_dirs = ['src', 'data', 'docs', 'reports', 'fixes', 'justifications', 'knowledge_base']
        missing_dirs = []
        for dir_name in required_dirs:
            dir_path = self.project_root / dir_name
            if dir_path.exists():
                self.log_output(f"  ✅ {dir_name}/ folder found\n")
                checks_passed += 1
            else:
                self.log_output(f"  ⚠️ {dir_name}/ folder missing (will be created on use)\n")
                missing_dirs.append(dir_name)
                warnings += 1
        
        # Check 3: Required Python Libraries
        self.log_output("\n🔍 Checking Python dependencies...\n")
        required_libs = {
            'tkinter': 'GUI framework (built-in)',
            'beautifulsoup4': 'HTML parsing',
            'pandas': 'Data processing',
            'openpyxl': 'Excel reports',
            'requests': 'Ollama API communication'
        }
        
        for lib, description in required_libs.items():
            try:
                if lib == 'tkinter':
                    import tkinter
                elif lib == 'beautifulsoup4':
                    import bs4
                elif lib == 'pandas':
                    import pandas
                elif lib == 'openpyxl':
                    import openpyxl
                elif lib == 'requests':
                    import requests
                self.log_output(f"  ✅ {lib} - {description}\n")
                checks_passed += 1
            except ImportError:
                self.log_output(f"  ❌ {lib} - {description} (NOT INSTALLED)\n")
                checks_failed += 1
        
        # Check 4: Optional AI Libraries
        self.log_output("\n🔍 Checking optional AI dependencies...\n")
        optional_libs = {
            'ollama': 'Local LLM support',
            'PIL': 'Image support for logo'
        }
        
        for lib, description in optional_libs.items():
            try:
                if lib == 'ollama':
                    import ollama
                elif lib == 'PIL':
                    from PIL import Image
                self.log_output(f"  ✅ {lib} - {description}\n")
            except ImportError:
                self.log_output(f"  ℹ️ {lib} - {description} (optional, not installed)\n")
        
        # Check 5: Ollama Service
        self.log_output("\n🔍 Checking Ollama AI service...\n")
        try:
            import requests
            response = requests.get('http://localhost:11434/api/version', timeout=3)
            if response.status_code == 200:
                version_data = response.json()
                version = version_data.get('version', 'unknown')
                self.log_output(f"  ✅ Ollama service running (v{version})\n")
                checks_passed += 1
                
                # Check available models
                try:
                    tags_response = requests.get('http://localhost:11434/api/tags', timeout=3)
                    if tags_response.status_code == 200:
                        models_data = tags_response.json()
                        models = models_data.get('models', [])
                        if models:
                            self.log_output(f"  ✅ {len(models)} AI model(s) available:\n")
                            for model in models[:5]:  # Show first 5
                                model_name = model.get('name', 'unknown')
                                size = model.get('size', 0)
                                size_mb = size / (1024 * 1024)
                                self.log_output(f"     • {model_name} ({size_mb:.1f} MB)\n")
                            if len(models) > 5:
                                self.log_output(f"     ... and {len(models) - 5} more\n")
                        else:
                            self.log_output(f"  ⚠️ No models downloaded yet\n")
                            self.log_output(f"     Download a model: ollama pull qwen2.5:latest\n")
                            warnings += 1
                except Exception:
                    pass
            else:
                self.log_output(f"  ⚠️ Ollama service not responding properly\n")
                warnings += 1
        except ImportError:
            self.log_output(f"  ❌ Cannot check Ollama (requests library missing)\n")
            checks_failed += 1
        except Exception as e:
            self.log_output(f"  ⚠️ Ollama service not running\n")
            self.log_output(f"     Start with: ollama serve\n")
            self.log_output(f"     (AI Chat will not work without Ollama)\n")
            warnings += 1
        
        # Check 6: Input Configuration
        self.log_output("\n🔍 Checking input configuration...\n")
        inputs_dir = self.project_root / "Inputs"
        if inputs_dir.exists():
            input_folders = list(inputs_dir.glob("*_Config_*"))
            if input_folders:
                self.log_output(f"  ✅ Found {len(input_folders)} input configuration(s)\n")
                for folder in input_folders[:3]:
                    self.log_output(f"     • {folder.name}\n")
                if len(input_folders) > 3:
                    self.log_output(f"     ... and {len(input_folders) - 3} more\n")
                checks_passed += 1
            else:
                self.log_output(f"  ℹ️ No input configurations found (normal for first run)\n")
        else:
            self.log_output(f"  ℹ️ Inputs/ folder not found (will be created on use)\n")
        
        # Summary
        self.log_output("\n" + "="*70 + "\n")
        self.log_output("📊 STARTUP CHECK SUMMARY\n")
        self.log_output("="*70 + "\n")
        self.log_output(f"  ✅ Passed: {checks_passed}\n")
        if warnings > 0:
            self.log_output(f"  ⚠️ Warnings: {warnings}\n")
        if checks_failed > 0:
            self.log_output(f"  ❌ Failed: {checks_failed}\n")
        
        # Overall status
        if checks_failed == 0:
            self.log_output("\n✅ System ready! All critical components available.\n")
            if warnings > 0:
                self.log_output("ℹ️ Some optional features may be limited (see warnings above).\n")
            self.update_status("✅ System Ready")
        else:
            self.log_output("\n⚠️ Some critical dependencies are missing.\n")
            self.log_output("Install missing packages: pip install -r requirements.txt\n")
            self.update_status("⚠️ Missing Dependencies")
            
            # Show dialog for critical failures
            if checks_failed > 0:
                messagebox.showwarning(
                    "Missing Dependencies",
                    f"Some required libraries are missing!\n\n"
                    f"Failed checks: {checks_failed}\n\n"
                    f"Please install missing dependencies:\n"
                    f"pip install -r requirements.txt\n\n"
                    f"Check the Output Log for details."
                )
        
        self.log_output("="*70 + "\n\n")
        self.log_output("💡 Tip: Use the '🤖 AI Chat' tab to interact with Ollama models\n")
        self.log_output("💡 Tip: Set your Input Path in Main Operations before analysis\n")
        self.log_output("💡 Tip: Check the '📚 Help' tab for documentation\n\n")
    
    def open_folder(self, folder_name):
        """Open folder in file explorer"""
        folder_path = self.project_root / folder_name
        
        if not folder_path.exists():
            folder_path.mkdir(parents=True, exist_ok=True)
        
        if sys.platform == 'win32':
            os.startfile(folder_path)
        elif sys.platform == 'darwin':
            subprocess.run(['open', folder_path])
        else:
            subprocess.run(['xdg-open', folder_path])
    
    def check_dependencies_status(self):
        """Check status of Python dependencies from requirements.txt"""
        self.log_output("\n" + "="*70 + "\n")
        self.log_output("🔍 CHECKING PYTHON DEPENDENCIES STATUS\n")
        self.log_output("="*70 + "\n\n")
        
        requirements_file = self.project_root / "requirements.txt"
        
        if not requirements_file.exists():
            self.log_output("❌ ERROR: requirements.txt not found!\n")
            self.log_output(f"   Expected location: {requirements_file}\n")
            return
        
        self.log_output(f"📄 Reading: {requirements_file}\n\n")
        
        def check_status():
            try:
                import pkg_resources
                
                # Read requirements.txt
                with open(requirements_file, 'r') as f:
                    lines = f.readlines()
                
                required_packages = []
                for line in lines:
                    line = line.strip()
                    # Skip comments and empty lines
                    if line and not line.startswith('#'):
                        # Extract package name (before >= or ==)
                        pkg_name = line.split('>=')[0].split('==')[0].split('<')[0].strip()
                        required_packages.append((pkg_name, line))
                
                self.root.after(0, lambda: self.log_output(f"📦 Found {len(required_packages)} package(s) in requirements.txt\n\n"))
                
                installed = []
                missing = []
                
                for pkg_name, full_spec in required_packages:
                    try:
                        version = pkg_resources.get_distribution(pkg_name).version
                        installed.append((pkg_name, version, full_spec))
                        self.root.after(0, lambda p=pkg_name, v=version: 
                            self.log_output(f"  ✅ {p:20s} v{v}\n"))
                    except pkg_resources.DistributionNotFound:
                        missing.append((pkg_name, full_spec))
                        self.root.after(0, lambda p=pkg_name: 
                            self.log_output(f"  ❌ {p:20s} NOT INSTALLED\n"))
                
                self.root.after(0, lambda: self.log_output("\n" + "-"*70 + "\n"))
                self.root.after(0, lambda: self.log_output(f"📊 SUMMARY:\n"))
                self.root.after(0, lambda i=len(installed), m=len(missing): 
                    self.log_output(f"   ✅ Installed: {i}\n   ❌ Missing: {m}\n"))
                
                if missing:
                    self.root.after(0, lambda: self.log_output("\n💡 Click '📦 Install Python Dependencies' to install missing packages.\n"))
                else:
                    self.root.after(0, lambda: self.log_output("\n🎉 All required packages are installed!\n"))
                
                self.root.after(0, lambda: self.log_output("="*70 + "\n"))
                
            except Exception as e:
                self.root.after(0, lambda err=str(e): self.log_output(f"❌ ERROR checking dependencies: {err}\n"))
        
        # Run check in background thread
        thread = threading.Thread(target=check_status, daemon=True)
        thread.start()
    
    def install_dependencies(self):
        """Install Python dependencies from requirements.txt"""
        requirements_file = self.project_root / "requirements.txt"
        
        if not requirements_file.exists():
            messagebox.showerror(
                "File Not Found",
                f"requirements.txt not found!\n\nExpected location:\n{requirements_file}"
            )
            return
        
        # Show confirmation dialog
        result = messagebox.askyesno(
            "Install Dependencies",
            "This will install/upgrade Python packages from requirements.txt.\n\n"
            "The installation may take 1-5 minutes depending on your internet speed.\n\n"
            "Do you want to continue?",
            icon='question'
        )
        
        if not result:
            self.log_output("[INFO] Dependency installation cancelled by user.\n")
            return
        
        # Show progress dialog
        progress_dialog = tk.Toplevel(self.root)
        progress_dialog.title("📦 Installing Dependencies")
        progress_dialog.geometry("800x500")
        progress_dialog.transient(self.root)
        progress_dialog.grab_set()
        
        # Center dialog
        progress_dialog.update_idletasks()
        x = (progress_dialog.winfo_screenwidth() // 2) - (400)
        y = (progress_dialog.winfo_screenheight() // 2) - (250)
        progress_dialog.geometry(f'+{x}+{y}')
        
        # Dialog content
        dialog_frame = ttk.Frame(progress_dialog, padding="15")
        dialog_frame.pack(fill=tk.BOTH, expand=True)
        
        header_label = ttk.Label(
            dialog_frame,
            text="📦 Installing Python Dependencies",
            font=('Arial', 12, 'bold')
        )
        header_label.pack(pady=(0, 10))
        
        info_label = ttk.Label(
            dialog_frame,
            text=f"Installing packages from: requirements.txt\nPlease wait, this may take a few minutes...",
            font=('Arial', 9),
            foreground='#7F8C8D'
        )
        info_label.pack(pady=(0, 10))
        
        # Progress output
        progress_text = scrolledtext.ScrolledText(
            dialog_frame,
            font=('Consolas', 9),
            bg='#F8F9FA',
            wrap=tk.WORD,
            height=20
        )
        progress_text.pack(fill=tk.BOTH, expand=True, pady=(0, 10))
        progress_text.config(state='disabled')
        
        # Close button (initially disabled)
        close_btn = ttk.Button(
            dialog_frame,
            text="Close",
            command=progress_dialog.destroy,
            state='disabled'
        )
        close_btn.pack()
        
        def update_progress(message):
            """Thread-safe progress update"""
            def _update():
                progress_text.config(state='normal')
                progress_text.insert(tk.END, message)
                progress_text.see(tk.END)
                progress_text.config(state='disabled')
            self.root.after(0, _update)
        
        def install_thread():
            """Background installation thread"""
            update_progress("📦 Starting dependency installation...\n")
            update_progress("="*70 + "\n\n")
            update_progress(f"📄 Using: {requirements_file}\n\n")
            
            try:
                # Get Python executable
                python_exe = sys.executable
                update_progress(f"🐍 Python: {python_exe}\n\n")
                update_progress("="*70 + "\n")
                update_progress("Running: pip install -r requirements.txt\n")
                update_progress("="*70 + "\n\n")
                
                # Run pip install
                process = subprocess.Popen(
                    [python_exe, '-m', 'pip', 'install', '-r', str(requirements_file), '--upgrade'],
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT,
                    text=True,
                    bufsize=1,
                    cwd=str(self.project_root)
                )
                
                # Stream output
                for line in process.stdout:
                    update_progress(line)
                
                process.wait()
                
                update_progress("\n" + "="*70 + "\n")
                
                if process.returncode == 0:
                    update_progress("✅ SUCCESS! All dependencies installed successfully.\n")
                    update_progress("="*70 + "\n\n")
                    update_progress("💡 You can now use all features of the Parasoft Analysis Tool.\n")
                    self.root.after(0, lambda: self.log_output("\n[SUCCESS] ✅ Python dependencies installed successfully!\n"))
                else:
                    update_progress(f"❌ Installation failed with exit code: {process.returncode}\n")
                    update_progress("="*70 + "\n\n")
                    update_progress("⚠️ Some packages may not have been installed correctly.\n")
                    update_progress("💡 Check the output above for specific errors.\n")
                    self.root.after(0, lambda: self.log_output("\n[ERROR] ❌ Dependency installation failed. Check progress dialog.\n"))
                
            except Exception as e:
                update_progress(f"\n❌ ERROR: {str(e)}\n")
                update_progress("\n💡 Try running manually from terminal:\n")
                update_progress(f"   pip install -r requirements.txt\n")
                self.root.after(0, lambda err=str(e): self.log_output(f"\n[ERROR] ❌ Installation failed: {err}\n"))
            
            finally:
                # Enable close button
                self.root.after(0, lambda: close_btn.config(state='normal'))
        
        # Start installation thread
        thread = threading.Thread(target=install_thread, daemon=True)
        thread.start()


def main():
    """Main entry point"""
    root = tk.Tk()
    app = ParasoftGUI(root)
    
    # Center window on screen
    root.update_idletasks()
    width = root.winfo_width()
    height = root.winfo_height()
    x = (root.winfo_screenwidth() // 2) - (width // 2)
    y = (root.winfo_screenheight() // 2) - (height // 2)
    root.geometry(f'{width}x{height}+{x}+{y}')
    
    root.mainloop()


if __name__ == '__main__':
    main()
