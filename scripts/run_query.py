"""
Knowledge Database Query Tool - Root Launcher
Wrapper script to run the query tool from the project root directory
"""

import sys
from pathlib import Path

# Add src directory to path (go up from scripts/ to root, then to src/)
src_dir = Path(__file__).parent.parent / "src"
sys.path.insert(0, str(src_dir))

# Import and run the query tool
from KnowledgeDatabaseQueryTool import main

if __name__ == "__main__":
    main()
