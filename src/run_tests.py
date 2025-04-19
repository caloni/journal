"""
Run tests for the journal-to-book/blog conversion system.

This script runs all tests in the tests directory using Python's unittest framework.
It provides a simple way to run tests and verify that the conversion system is working correctly.

Usage:
    python run_tests.py [test_name]

If no test_name is provided, all tests will be run.
To run a specific test, provide the test name in the format:
    module.class.test_method
For example:
    python run_tests.py tests.test_markdown_parser.TestMarkdownParser.test_links_and_images

Note: On Windows, you must use 'python run_tests.py' to run this script.
"""

import unittest
import sys
from pathlib import Path

def main():
    # Add the current directory to the Python path
    sys.path.append(str(Path(__file__).parent))
    
    # If a specific test is provided as an argument, run only that test
    if len(sys.argv) > 1:
        test_name = sys.argv[1]
        print(f"Running specific test: {test_name}")
        unittest.main(module=None, argv=[sys.argv[0], test_name])
    else:
        # Otherwise, discover and run all tests
        print("Running all tests...")
        unittest.main(module=None, argv=[sys.argv[0], 'discover'])

if __name__ == '__main__':
    main() 