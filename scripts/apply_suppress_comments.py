"""
Apply Suppress Comments to Source Code
Interactive tool to insert Parasoft suppress comments into source files
Version: 1.0.0
Developer: Himanshu R
"""

import sys
import re
from pathlib import Path
from datetime import datetime
import shutil

class SuppressCommentApplicator:
    """Applies Parasoft suppress comments to source code with user confirmation"""
    
    def __init__(self, suppress_file_path, target_repo_path):
        """
        Initialize the applicator
        
        Args:
            suppress_file_path: Path to the suppress comments file
            target_repo_path: Path to the target repository with source code
        """
        self.suppress_file = Path(suppress_file_path)
        self.target_repo = Path(target_repo_path)
        self.applied_count = 0
        self.skipped_count = 0
        self.failed_count = 0
        
        if not self.suppress_file.exists():
            raise FileNotFoundError(f"Suppress comments file not found: {suppress_file_path}")
        
        if not self.target_repo.exists():
            raise FileNotFoundError(f"Target repository not found: {target_repo_path}")
        
        # Create backup folder
        timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
        self.backup_folder = self.target_repo / f"parasoft_backups_{timestamp}"
        self.backup_folder.mkdir(exist_ok=True)
        
        print(f"\n{'='*80}")
        print("  PARASOFT SUPPRESS COMMENTS APPLICATOR")
        print(f"{'='*80}\n")
        print(f"Suppress file: {self.suppress_file}")
        print(f"Target repository: {self.target_repo}")
        print(f"Backup folder: {self.backup_folder}\n")
    
    def parse_suppress_file(self):
        """
        Parse the suppress comments file
        
        Returns:
            List of dictionaries with file, line, begin_comment, end_comment
        """
        suppressions = []
        
        with open(self.suppress_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Pattern to match each suppression block
        pattern = r'File: (.+?), Line: (\d+)\s*-+\s*(/\* parasoft-begin-suppress .+? \*/)\s*.+?\s*(/\* parasoft-end-suppress .+? \*/)'
        
        matches = re.finditer(pattern, content, re.DOTALL)
        
        for match in matches:
            file_name = match.group(1).strip()
            line_num = int(match.group(2))
            begin_comment = match.group(3).strip()
            end_comment = match.group(4).strip()
            
            suppressions.append({
                'file': file_name,
                'line': line_num,
                'begin_comment': begin_comment,
                'end_comment': end_comment
            })
        
        return suppressions
    
    def find_source_file(self, file_name):
        """
        Find a source file in the target repository
        
        Args:
            file_name: Name of the file to find
        
        Returns:
            Path to the file or None if not found
        """
        # Search recursively in target repository
        matches = list(self.target_repo.rglob(file_name))
        
        if len(matches) == 0:
            return None
        elif len(matches) == 1:
            return matches[0]
        else:
            # Multiple matches - let user choose
            print(f"\n[WARNING] Multiple files named '{file_name}' found:")
            for i, path in enumerate(matches, 1):
                print(f"  {i}. {path}")
            
            while True:
                choice = input(f"Select file (1-{len(matches)}) or 's' to skip: ").strip().lower()
                if choice == 's':
                    return None
                try:
                    idx = int(choice) - 1
                    if 0 <= idx < len(matches):
                        return matches[idx]
                except ValueError:
                    pass
                print("Invalid choice. Try again.")
    
    def create_backup(self, file_path):
        """
        Create a backup of the file before modification
        
        Args:
            file_path: Path to the file to backup
        
        Returns:
            Path to the backup file
        """
        # Get relative path from target repo
        try:
            rel_path = file_path.relative_to(self.target_repo)
        except ValueError:
            # If file is not under target_repo, use just the filename
            rel_path = Path(file_path.name)
        
        # Create backup path maintaining directory structure
        backup_path = self.backup_folder / rel_path
        backup_path.parent.mkdir(parents=True, exist_ok=True)
        
        # Copy file to backup location
        shutil.copy2(file_path, backup_path)
        return backup_path
    
    def is_inside_comment_block(self, lines, line_num):
        """
        Check if a line is inside a comment block
        
        Args:
            lines: List of file lines
            line_num: Line number to check (1-indexed)
        
        Returns:
            True if inside comment block, False otherwise
        """
        # Track if we're inside a multi-line comment
        in_comment = False
        
        for i in range(line_num):
            line = lines[i].strip()
            
            # Check for comment start
            if '/*' in line:
                in_comment = True
            
            # Check for comment end
            if '*/' in line:
                in_comment = False
        
        return in_comment
    
    def show_preview(self, file_path, line_num, begin_comment, end_comment):
        """
        Show a preview of the changes
        
        Args:
            file_path: Path to the source file
            line_num: Line number where suppression applies
            begin_comment: Begin suppress comment
            end_comment: End suppress comment
        
        Returns:
            Tuple of (current line content, is_in_comment)
        """
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            lines = f.readlines()
        
        # Check if line is inside a comment block
        is_in_comment = self.is_inside_comment_block(lines, line_num)
        
        # Show context around the line
        start_idx = max(0, line_num - 3)
        end_idx = min(len(lines), line_num + 3)
        
        print(f"\n{'-'*80}")
        print(f"File: {file_path.name}")
        print(f"Line: {line_num}")
        
        if is_in_comment:
            print("[WARNING] This line appears to be inside a comment block!")
        
        print(f"{'-'*80}")
        print("\nCurrent code:")
        for i in range(start_idx, end_idx):
            marker = ">>>" if i == line_num - 1 else "   "
            print(f"{marker} {i+1:4d}: {lines[i].rstrip()}")
        
        if not is_in_comment:
            print("\nProposed changes:")
            print(f"     ADD: {begin_comment}")
            if line_num - 1 < len(lines):
                print(f"     {line_num:4d}: {lines[line_num - 1].rstrip()}")
            print(f"     ADD: {end_comment}")
        else:
            print("\n[SKIP RECOMMENDED] Cannot insert suppress comments inside comment blocks")
        
        current_line = lines[line_num - 1] if line_num - 1 < len(lines) else ""
        return current_line, is_in_comment
    
    def apply_suppression(self, file_path, line_num, begin_comment, end_comment):
        """
        Apply suppression comments to a file
        
        Args:
            file_path: Path to the source file
            line_num: Line number where suppression applies
            begin_comment: Begin suppress comment
            end_comment: End suppress comment
        
        Returns:
            True if successful, False otherwise
        """
        try:
            # Read file
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                lines = f.readlines()
            
            if line_num < 1 or line_num > len(lines):
                print(f"[ERROR] Line number {line_num} is out of range for file with {len(lines)} lines")
                return False
            
            # Create backup
            backup_path = self.create_backup(file_path)
            try:
                rel_backup = backup_path.relative_to(self.backup_folder)
                print(f"[INFO] Backup created: {rel_backup}")
            except ValueError:
                print(f"[INFO] Backup created: {backup_path.name}")
            
            # Insert comments
            # Insert begin comment before the line
            lines.insert(line_num - 1, begin_comment + '\n')
            # Insert end comment after the line (now at line_num + 1 due to insertion)
            lines.insert(line_num + 1, end_comment + '\n')
            
            # Write modified file
            with open(file_path, 'w', encoding='utf-8', newline='') as f:
                f.writelines(lines)
            
            return True
            
        except Exception as e:
            print(f"[ERROR] Failed to apply suppression: {str(e)}")
            return False
    
    def run(self):
        """Run the interactive application process"""
        
        # Parse suppress file
        print("Parsing suppress comments file...")
        suppressions = self.parse_suppress_file()
        
        if not suppressions:
            print("\n[WARNING] No suppressions found in file")
            return
        
        print(f"Found {len(suppressions)} suppression(s) to apply\n")
        
        # Process each suppression
        for i, supp in enumerate(suppressions, 1):
            print(f"\n{'='*80}")
            print(f"Suppression {i} of {len(suppressions)}")
            print(f"{'='*80}")
            
            # Find source file
            source_file = self.find_source_file(supp['file'])
            
            if not source_file:
                print(f"[ERROR] Could not find file: {supp['file']}")
                self.failed_count += 1
                continue
            
            # Show preview
            current_line, is_in_comment = self.show_preview(source_file, supp['line'], supp['begin_comment'], supp['end_comment'])
            
            # Auto-skip if inside comment block
            if is_in_comment:
                print("\n[AUTO-SKIP] This line is inside a comment block - cannot apply suppression here")
                print("[INFO] The violation should be addressed in the actual code, not in comments")
                self.skipped_count += 1
                continue
            
            # Ask user for confirmation
            while True:
                choice = input(f"\nApply this suppression? (y=yes, n=no, a=yes to all, q=quit): ").strip().lower()
                
                if choice == 'y':
                    # Apply this one
                    if self.apply_suppression(source_file, supp['line'], supp['begin_comment'], supp['end_comment']):
                        print("[SUCCESS] Suppression applied successfully")
                        self.applied_count += 1
                    else:
                        self.failed_count += 1
                    break
                
                elif choice == 'n':
                    # Skip this one
                    print("[INFO] Skipped")
                    self.skipped_count += 1
                    break
                
                elif choice == 'a':
                    # Apply all remaining
                    if self.apply_suppression(source_file, supp['line'], supp['begin_comment'], supp['end_comment']):
                        print("[SUCCESS] Suppression applied successfully")
                        self.applied_count += 1
                    else:
                        self.failed_count += 1
                    
                    # Apply rest without asking (but still check for comment blocks)
                    for remaining_supp in suppressions[i:]:
                        remaining_file = self.find_source_file(remaining_supp['file'])
                        if remaining_file:
                            # Read file and check if in comment
                            with open(remaining_file, 'r', encoding='utf-8', errors='ignore') as f:
                                lines = f.readlines()
                            
                            if self.is_inside_comment_block(lines, remaining_supp['line']):
                                print(f"[AUTO-SKIP] {remaining_supp['file']}:{remaining_supp['line']} - inside comment block")
                                self.skipped_count += 1
                            elif self.apply_suppression(remaining_file, remaining_supp['line'], 
                                                     remaining_supp['begin_comment'], remaining_supp['end_comment']):
                                self.applied_count += 1
                            else:
                                self.failed_count += 1
                        else:
                            self.failed_count += 1
                    
                    return  # Exit function
                
                elif choice == 'q':
                    # Quit
                    print("\n[INFO] User quit. Exiting...")
                    return
                
                else:
                    print("Invalid choice. Please enter y, n, a, or q.")
        
        # Show summary
        self.show_summary()
    
    def show_summary(self):
        """Show summary of operations"""
        print(f"\n{'='*80}")
        print("  SUMMARY")
        print(f"{'='*80}")
        print(f"Applied:  {self.applied_count}")
        print(f"Skipped:  {self.skipped_count}")
        print(f"Failed:   {self.failed_count}")
        print(f"Total:    {self.applied_count + self.skipped_count + self.failed_count}")
        print(f"{'='*80}")
        if self.applied_count > 0:
            print(f"\nBackup files saved to: {self.backup_folder}")
            print("Review backups if you need to restore original files\n")
        else:
            print()


def main():
    """Main entry point"""
    
    if len(sys.argv) < 3:
        print("Usage: python apply_suppress_comments.py <suppress_file> <target_repo>")
        print("\nExample:")
        print("  python apply_suppress_comments.py justifications\\Mka_suppress_comments_20260410_143022.txt D:\\MyProject\\src")
        sys.exit(1)
    
    suppress_file = sys.argv[1]
    target_repo = sys.argv[2]
    
    try:
        applicator = SuppressCommentApplicator(suppress_file, target_repo)
        applicator.run()
    except FileNotFoundError as e:
        print(f"\n[ERROR] {str(e)}")
        sys.exit(1)
    except Exception as e:
        print(f"\n[ERROR] Unexpected error: {str(e)}")
        import traceback
        traceback.print_exc()
        sys.exit(1)


if __name__ == "__main__":
    main()
