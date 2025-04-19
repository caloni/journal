import unittest
import os
import shutil
import tempfile
import subprocess
from pathlib import Path

class TestMarkdownParser(unittest.TestCase):
    def setUp(self):
        # Create a temporary directory
        self.test_dir = tempfile.mkdtemp()
        self.original_dir = os.getcwd()
        
        # Create necessary test directories
        os.makedirs(os.path.join(self.test_dir, 'public'))
        os.makedirs(os.path.join(self.test_dir, 'public', 'blog'))
        os.makedirs(os.path.join(self.test_dir, 'public', 'blog', 'img'))
        os.makedirs(os.path.join(self.test_dir, 'blog'))
        os.makedirs(os.path.join(self.test_dir, 'img', 'blog'))
        
        # Copy the AWK scripts to the test directory
        script_dir = os.path.dirname(os.path.abspath(__file__))
        parent_dir = os.path.dirname(script_dir)
        for awk_file in ['Util.awk', 'MarkdownParser.awk', 'BlogWriter.awk', 'MetadataWriter.awk']:
            source = os.path.join(parent_dir, awk_file)
            dest = os.path.join(self.test_dir, awk_file)
            print(f"Copying {source} to {dest}")
            shutil.copy2(source, dest)
        
        os.chdir(self.test_dir)

    def tearDown(self):
        # Clean up
        os.chdir(self.original_dir)
        shutil.rmtree(self.test_dir)

    def run_conversion_test(self, input_markdown, expected_output):
        """Helper function to run the full conversion pipeline"""
        # Create input file
        input_file = os.path.join(self.test_dir, 'journal.md')
        with open(input_file, 'w', encoding='utf-8') as f:
            f.write(input_markdown)
        
        # Print debug information
        print(f"Current directory: {os.getcwd()}")
        print(f"Input file: {input_file}")
        print(f"Input content:\n{input_markdown}")
        
        # First run metadata generation
        metadata_cmd = [
            'gawk',
            '-f', 'Util.awk',
            '-f', 'MetadataWriter.awk',
            'journal.md'
        ]
        print(f"Running metadata command: {' '.join(metadata_cmd)}")
        process = subprocess.run(metadata_cmd, capture_output=True, text=True)
        print(f"Metadata stdout:\n{process.stdout}")
        print(f"Metadata stderr:\n{process.stderr}")
        
        # Then run the full conversion
        convert_cmd = [
            'gawk',
            '-f', 'Util.awk',
            '-f', 'MarkdownParser.awk',
            '-f', 'BlogWriter.awk',
            'public/metadata.txt',
            'journal.md'
        ]
        print(f"Running conversion command: {' '.join(convert_cmd)}")
        process = subprocess.run(convert_cmd, capture_output=True, text=True)
        
        print(f"Return code: {process.returncode}")
        print(f"stdout:\n{process.stdout}")
        print(f"stderr:\n{process.stderr}")
        
        self.assertEqual(process.returncode, 0, f"Conversion failed with error: {process.stderr}")
        
        # Check output by reading the generated HTML file
        html_files = [f for f in os.listdir('public/blog') if f.endswith('.html')]
        self.assertTrue(len(html_files) > 0, "No HTML files were generated")
        
        with open(os.path.join('public/blog', html_files[0]), 'r', encoding='utf-8') as f:
            content = f.read()
            for expected in expected_output:
                self.assertIn(expected, content, 
                             f"Expected '{expected}' not found in output")

    def test_basic_markdown(self):
        """Test basic markdown parsing"""
        input_md = """# Test Entry
[date]: 2024-01-01
[tags]: test

This is a paragraph.

## Heading 2
- List item 1
- List item 2
"""
        expected = [
            'Test Entry',
            'This is a paragraph',
            'Heading 2',
            'List item 1'
        ]
        self.run_conversion_test(input_md, expected)

    def test_links_and_images(self):
        """Test link and image parsing"""
        input_md = """# Test Entry
[date]: 2024-01-01
[tags]: test

[Link text](http://example.com)
![Alt text](image.jpg)
"""
        expected = [
            'Link text',
            'http://example.com',
            'image.jpg'
        ]
        self.run_conversion_test(input_md, expected)

    def test_code_blocks(self):
        """Test code block parsing"""
        input_md = """# Test Entry
[date]: 2024-01-01
[tags]: test

```python
def hello():
    print("Hello, World!")
```"""
        expected = [
            'def hello():',
            'print(&quot;Hello, World!&quot;)'
        ]
        self.run_conversion_test(input_md, expected)

if __name__ == '__main__':
    unittest.main() 