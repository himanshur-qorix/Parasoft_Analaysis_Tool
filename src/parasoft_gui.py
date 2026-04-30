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
from pathlib import Path

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
        
        # Set project root directory (parent of src)
        self.project_root = Path(__file__).parent.parent.absolute()
        self.scripts_dir = self.project_root / "batch_support"
        
        # Configure style
        self.setup_styles()
        
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
                # Generate code fixes
                script = src_dir / "generate_code_fixes.py"
                ai_mode = self.ai_mode_var.get() if hasattr(self, 'ai_mode_var') else "hybrid"
                cmd = [
                    sys.executable, str(script),
                    module_name,
                    "--ai-mode", ai_mode
                ]
                
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
            from apply_suppress_comments import SuppressCommentApplier
            applier = SuppressCommentApplier(suppress_file, target_repo)
            
            # Parse the file
            suppressions_data = applier.parse_suppress_file()
            
            if not suppressions_data:
                messagebox.showinfo("No Suppressions", "No suppressions found in the selected file.")
                return
            
            # Count total suppressions
            total_suppressions = sum(len(supps) for supps in suppressions_data.values())
            
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
        
        # Stats
        stats = {
            'applied': 0,
            'skipped': 0,
            'already_justified': 0,
            'failed': 0
        }
        
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
        
        stats_text = tk.StringVar(value="Applied: 0 | Skipped: 0 | Already Justified: 0 | Failed: 0")
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
                f"Already Justified: {stats['already_justified']} | Failed: {stats['failed']}"
            )
        
        def load_current_suppression():
            """Load and display current suppression"""
            if current_file_idx[0] >= len(files_list):
                # Done!
                messagebox.showinfo(
                    "Complete",
                    f"All suppressions reviewed!\n\n"
                    f"Applied: {stats['applied']}\n"
                    f"Skipped: {stats['skipped']}\n"
                    f"Already Justified: {stats['already_justified']}\n"
                    f"Failed: {stats['failed']}"
                )
                dialog.destroy()
                return False
            
            file_name = files_list[current_file_idx[0]]
            suppressions = suppressions_data[file_name]
            
            if current_supp_idx[0] >= len(suppressions):
                # Move to next file
                current_file_idx[0] += 1
                current_supp_idx[0] = 0
                return load_current_suppression()
            
            supp = suppressions[current_supp_idx[0]]
            
            # Update progress
            total_processed = sum(
                len(suppressions_data[f]) for f in files_list[:current_file_idx[0]]
            ) + current_supp_idx[0] + 1
            progress_var.set(f"{total_processed} / {total_count}")
            
            # Find source file
            source_file = applier.find_file_in_repo(file_name)
            
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
            supp = suppressions_data[file_name][current_supp_idx[0]]
            source_file = applier.find_file_in_repo(file_name)
            
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
                suppressions = suppressions_data[file_name]
                source_file = applier.find_file_in_repo(file_name)
                
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
            
            messagebox.showinfo(
                "Complete",
                f"All suppressions applied!\n\n"
                f"Applied: {stats['applied']}\n"
                f"Skipped: {stats['skipped']}\n"
                f"Already Justified: {stats['already_justified']}\n"
                f"Failed: {stats['failed']}"
            )
            dialog.destroy()
        
        # Buttons
        ttk.Button(button_frame, text="✅ Apply", command=apply_current, width=15).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="⏭️ Skip", command=skip_current, width=15).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="⚡ Apply All Remaining", command=apply_all_remaining, width=20).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="❌ Cancel", command=dialog.destroy, width=15).pack(side=tk.RIGHT, padx=5)
        
        # Load first suppression
        if not load_current_suppression():
            return
        
        dialog.transient(self.root)
        dialog.grab_set()
    
    
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
