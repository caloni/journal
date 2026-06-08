#!/usr/bin/env python3
"""
Wrapper to call the journal publisher.
The script to call is determined by this file's name.
"""

import sys
import os
import subprocess
from pathlib import Path

def call_publisher(script_name):
    """Call Publisher.Console.exe with appropriate arguments."""
    script_dir = Path(__file__).parent.absolute()
    journal_dir = script_dir.parent
    console_exe = "Publisher.Console.exe"

    args = [
        '--mode', 'journal',
        '--base-path', '/journal',
        '--output-path', 'publish/output/caloni.github.io/journal',
        '--journal-path', 'journal.md',
        '--single-post-mode', '0',
        '--use-templates', '1',
        '--external-links', 'external-links.yaml',
        '--post-links', 'post-links.yaml',
        '--external-images', 'external-images.yaml',
    ]
    
    try:
        result = subprocess.run([str(console_exe)] + args, check=True)
        return result.returncode
    except subprocess.CalledProcessError as e:
        print(f"Error running Publisher.Console.exe: {e}", file=sys.stderr)
        return e.returncode
    except FileNotFoundError:
        print(f"Error: Publisher.Console.exe not found at {console_exe}", file=sys.stderr)
        return 1

def main():
    # Get this script's name (without .py extension)
    script_name = Path(__file__).stem
    return call_publisher(script_name)

if __name__ == "__main__":
    sys.exit(main())
