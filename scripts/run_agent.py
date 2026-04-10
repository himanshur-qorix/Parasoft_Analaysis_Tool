"""
Parasoft AI Agent - Root Launcher
Wrapper script to run the agent from the project root directory
"""

import sys
from pathlib import Path

# Add src directory to path (go up from scripts/ to root, then to src/)
src_dir = Path(__file__).parent.parent / "src"
sys.path.insert(0, str(src_dir))

# Import and run the main agent
from ParasoftAIAgent import main

if __name__ == "__main__":
    main()
