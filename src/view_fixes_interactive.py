"""
Interactive Fix Viewer
Browse code fix suggestions interactively with keyboard navigation

Developer: Himanshu R
Organization: Qorix India Pvt Ltd
Version: 2.3.0
"""

import sys
import os
from pathlib import Path
import json
import re

try:
    import msvcrt  # Windows only
    WINDOWS = True
except ImportError:
    import tty
    import termios
    WINDOWS = False


class InteractiveFixViewer:
    """Interactive viewer for code fix suggestions"""
    
    def __init__(self, fixes_file: Path):
        """Initialize viewer with fixes file"""
        self.fixes_file = fixes_file
        self.fixes = []
        self.current_index = 0
        self.load_fixes()
    
    def load_fixes(self):
        """Load fixes from text file"""
        if not self.fixes_file.exists():
            print(f"[ERROR] Fixes file not found: {self.fixes_file}")
            sys.exit(1)
        
        with open(self.fixes_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Parse fixes from text file
        fix_blocks = re.split(r'^FIX #\d+$', content, flags=re.MULTILINE)[1:]
        
        for block in fix_blocks:
            fix_data = {}
            
            # Extract violation ID
            vid_match = re.search(r'Violation ID: (.+)', block)
            if vid_match:
                fix_data['violation_id'] = vid_match.group(1).strip()
            
            # Extract severity
            sev_match = re.search(r'Severity: (.+)', block)
            if sev_match:
                fix_data['severity'] = sev_match.group(1).strip()
            
            # Extract category
            cat_match = re.search(r'Category: (.+)', block)
            if cat_match:
                fix_data['category'] = cat_match.group(1).strip()
            
            # Extract description
            desc_match = re.search(r'Description:\n(.+?)(?=\n\nFix Type:)', block, re.DOTALL)
            if desc_match:
                fix_data['description'] = desc_match.group(1).strip()
            
            # Extract fix type
            type_match = re.search(r'Fix Type: (.+)', block)
            if type_match:
                fix_data['fix_type'] = type_match.group(1).strip()
            
            # Extract priority
            priority_match = re.search(r'Priority: (.+)', block)
            if priority_match:
                fix_data['priority'] = priority_match.group(1).strip()
            else:
                fix_data['priority'] = 'MEDIUM'
            
            # Extract fix description
            fix_desc_match = re.search(r'Fix Description:\n(.+?)(?=\n\nExample:|\n\nFiles Affected:)', block, re.DOTALL)
            if fix_desc_match:
                fix_data['fix_description'] = fix_desc_match.group(1).strip()
            
            # Extract example
            example_match = re.search(r'Example:\n(.+?)(?=\n\nFiles Affected:)', block, re.DOTALL)
            if example_match:
                fix_data['example'] = example_match.group(1).strip()
            
            # Extract files
            files_match = re.search(r'Files Affected:\n(.+?)(?=\n={80}|$)', block, re.DOTALL)
            if files_match:
                files_text = files_match.group(1).strip()
                fix_data['files'] = [line.strip() for line in files_text.split('\n') if line.strip()]
            
            if 'violation_id' in fix_data:
                self.fixes.append(fix_data)
    
    def clear_screen(self):
        """Clear the console screen"""
        os.system('cls' if os.name == 'nt' else 'clear')
    
    def get_key(self):
        """Get a single key press (cross-platform)"""
        if WINDOWS:
            key = msvcrt.getch()
            if key == b'\\x00' or key == b'\\xe0':  # Arrow keys
                key = msvcrt.getch()
                if key == b'H':  # Up
                    return 'up'
                elif key == b'P':  # Down
                    return 'down'
                elif key == b'K':  # Left
                    return 'left'
                elif key == b'M':  # Right
                    return 'right'
            elif key == b'\\r':
                return 'enter'
            elif key == b'\\x1b':  # Escape
                return 'esc'
            return key.decode('utf-8', errors='ignore')
        else:
            fd = sys.stdin.fileno()
            old_settings = termios.tcgetattr(fd)
            try:
                tty.setraw(sys.stdin.fileno())
                ch = sys.stdin.read(1)
                if ch == '\\x1b':  # Escape sequence
                    ch2 = sys.stdin.read(1)
                    ch3 = sys.stdin.read(1)
                    if ch2 == '[':
                        if ch3 == 'A':
                            return 'up'
                        elif ch3 == 'B':
                            return 'down'
                        elif ch3 == 'C':
                            return 'right'
                        elif ch3 == 'D':
                            return 'left'
                    return 'esc'
                return ch
            finally:
                termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    
    def display_fix(self):
        """Display current fix"""
        self.clear_screen()
        
        if not self.fixes:
            print("\\n[ERROR] No fixes found in file!\\n")
            return
        
        fix = self.fixes[self.current_index]
        
        # Header
        print("\\n" + "="*100)
        print(f"  INTERACTIVE FIX VIEWER - Qorix India Pvt Ltd")
        print(f"  Fix {self.current_index + 1} of {len(self.fixes)}")
        print("="*100 + "\\n")
        
        # Violation Info
        print("\\033[1;36m" + "┌─ VIOLATION INFO " + "─"*82 + "\\033[0m")
        print(f"\\033[1m│ ID:\\033[0m {fix.get('violation_id', 'N/A')}")
        print(f"\\033[1m│ Severity:\\033[0m {fix.get('severity', 'N/A')}  ", end='')
        print(f"\\033[1m│ Category:\\033[0m {fix.get('category', 'N/A')}  ", end='')
        
        # Color-code priority
        priority = fix.get('priority', 'MEDIUM')
        priority_color = {
            'CRITICAL': '\\033[1;31m',  # Red
            'HIGH': '\\033[1;33m',      # Yellow
            'MEDIUM': '\\033[1;34m',    # Blue
            'LOW': '\\033[1;32m'        # Green
        }.get(priority, '\\033[0m')
        print(f"\\033[1m│ Priority:\\033[0m {priority_color}{priority}\\033[0m")
        
        # Color-code fix type
        fix_type = fix.get('fix_type', 'N/A')
        type_color = {
            'parasoft_official': '\\033[1;35m',  # Magenta
            'ai_generated': '\\033[1;36m',       # Cyan
            'rule_based': '\\033[1;37m'          # White
        }.get(fix_type, '\\033[0m')
        print(f"\\033[1m│ Fix Type:\\033[0m {type_color}{fix_type}\\033[0m")
        print("\\033[1;36m" + "└" + "─"*98 + "\\033[0m\\n")
        
        # Description
        print("\\033[1;33m" + "┌─ DESCRIPTION " + "─"*84 + "\\033[0m")
        description = fix.get('description', 'No description available')
        for line in description.split('\\n'):
            print(f"  {line}")
        print("\\033[1;33m" + "└" + "─"*98 + "\\033[0m\\n")
        
        # Fix Suggestion
        if 'fix_description' in fix:
            print("\\033[1;32m" + "┌─ FIX SUGGESTION " + "─"*81 + "\\033[0m")
            fix_desc = fix['fix_description']
            for line in fix_desc.split('\\n')[:10]:  # Limit to 10 lines
                print(f"  {line}")
            if fix_desc.count('\\n') > 10:
                print("  ...")
            print("\\033[1;32m" + "└" + "─"*98 + "\\033[0m\\n")
        
        # Example Code
        if 'example' in fix:
            print("\\033[1;35m" + "┌─ CODE EXAMPLE " + "─"*83 + "\\033[0m")
            example = fix['example']
            
            # Split into violation and repair if present
            if '--- VIOLATION EXAMPLE ---' in example:
                parts = example.split('--- OFFICIAL REPAIR ---')
                violation_part = parts[0].replace('--- VIOLATION EXAMPLE ---', '').strip()
                
                print("  \\033[1;31m❌ VIOLATION:\\033[0m")
                for line in violation_part.split('\\n')[:8]:
                    print(f"    {line}")
                
                if len(parts) > 1:
                    print()
                    repair_part = parts[1].strip()
                    print("  \\033[1;32m✅ REPAIR:\\033[0m")
                    for line in repair_part.split('\\n')[:8]:
                        print(f"    {line}")
            else:
                # Generic example
                for line in example.split('\\n')[:12]:
                    print(f"  {line}")
            
            print("\\033[1;35m" + "└" + "─"*98 + "\\033[0m\\n")
        
        # Affected Files
        if 'files' in fix and fix['files']:
            files = fix['files']
            print("\\033[1;34m" + f"┌─ AFFECTED FILES ({len(files)} total) " + "─"*(60) + "\\033[0m")
            for i, file_line in enumerate(files[:5], 1):
                print(f"  {i}. {file_line}")
            if len(files) > 5:
                print(f"  ... and {len(files) - 5} more files")
            print("\\033[1;34m" + "└" + "─"*98 + "\\033[0m\\n")
        
        # Navigation Help
        print("\\n" + "─"*100)
        print("  \\033[1mNAVIGATION:\\033[0m  ", end='')
        print("\\033[1;32m←→\\033[0m or \\033[1;32mA/D\\033[0m = Prev/Next  │  ", end='')
        print("\\033[1;33mH\\033[0m = HTML Report  │  ", end='')
        print("\\033[1;36mS\\033[0m = Search  │  ", end='')
        print("\\033[1;31mQ\\033[0m or \\033[1;31mESC\\033[0m = Quit")
        print("─"*100)
    
    def search_fixes(self):
        """Search for fixes by violation ID or keyword"""
        self.clear_screen()
        print("\\n" + "="*100)
        print("  SEARCH FIXES")
        print("="*100 + "\\n")
        
        search_term = input("Enter search term (violation ID or keyword): ").strip().lower()
        
        if not search_term:
            return
        
        matches = []
        for i, fix in enumerate(self.fixes):
            violation_id = fix.get('violation_id', '').lower()
            description = fix.get('description', '').lower()
            
            if search_term in violation_id or search_term in description:
                matches.append((i, fix))
        
        if not matches:
            print(f"\\n[INFO] No matches found for '{search_term}'")
            input("\\nPress Enter to continue...")
            return
        
        print(f"\\n[FOUND] {len(matches)} match(es):\\n")
        for idx, (original_idx, fix) in enumerate(matches, 1):
            print(f"  {idx}. {fix.get('violation_id', 'N/A')} - {fix.get('description', '')[:60]}...")
        
        print(f"\\n[0] Cancel")
        
        try:
            choice = int(input("\\nSelect fix number: "))
            if 1 <= choice <= len(matches):
                self.current_index = matches[choice - 1][0]
        except (ValueError, IndexError):
            pass
    
    def open_html_report(self):
        """Open HTML report in default browser"""
        html_file = self.fixes_file.with_suffix('.html')
        
        if html_file.exists():
            import webbrowser
            webbrowser.open(html_file.as_uri())
            print(f"\\n[INFO] Opening HTML report in browser: {html_file.name}")
            input("\\nPress Enter to continue...")
        else:
            print(f"\\n[ERROR] HTML report not found: {html_file}")
            input("\\nPress Enter to continue...")
    
    def run(self):
        """Main interactive loop"""
        while True:
            self.display_fix()
            
            key = self.get_key()
            
            if key in ['q', 'Q', 'esc']:
                self.clear_screen()
                print("\\n[INFO] Exiting Interactive Fix Viewer\\n")
                break
            elif key in ['right', 'd', 'D']:
                if self.current_index < len(self.fixes) - 1:
                    self.current_index += 1
            elif key in ['left', 'a', 'A']:
                if self.current_index > 0:
                    self.current_index -= 1
            elif key in ['h', 'H']:
                self.open_html_report()
            elif key in ['s', 'S']:
                self.search_fixes()


def main():
    """Main entry point"""
    import argparse
    
    parser = argparse.ArgumentParser(
        description='Interactive viewer for code fix suggestions',
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    
    parser.add_argument('fixes_file', help='Path to fixes text file')
    
    args = parser.parse_args()
    
    fixes_file = Path(args.fixes_file)
    
    if not fixes_file.exists():
        print(f"\\n[ERROR] File not found: {fixes_file}\\n")
        return 1
    
    viewer = InteractiveFixViewer(fixes_file)
    
    if not viewer.fixes:
        print("\\n[ERROR] No fixes found in file!\\n")
        return 1
    
    print(f"\\n[OK] Loaded {len(viewer.fixes)} fixes from {fixes_file.name}")
    input("Press Enter to start interactive viewer...")
    
    try:
        viewer.run()
    except KeyboardInterrupt:
        viewer.clear_screen()
        print("\\n\\n[INFO] Interrupted by user\\n")
    
    return 0


if __name__ == '__main__':
    sys.exit(main())
