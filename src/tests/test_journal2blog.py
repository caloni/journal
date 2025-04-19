import unittest
import os
import shutil
import tempfile
from pathlib import Path
import subprocess
import sys

# Add the parent directory to the Python path so we can import the modules
sys.path.append(str(Path(__file__).parent.parent))

class TestJournal2Blog(unittest.TestCase):
    def setUp(self):
        # Create a temporary directory
        self.test_dir = tempfile.mkdtemp()
        self.original_dir = os.getcwd()
        
        # Create necessary test directories
        os.makedirs(os.path.join(self.test_dir, 'blog'))
        os.makedirs(os.path.join(self.test_dir, 'img', 'blog'))
        os.makedirs(os.path.join(self.test_dir, 'public', 'blog'))
        
        # Create a sample journal.md file
        with open(os.path.join(self.test_dir, 'journal.md'), 'w') as f:
            f.write("""# Test Blog Entry
[date]:# "2024-01-01"
[tags]:# "test"

This is a test blog entry with some **bold** text and a [link](http://example.com).
""")
        
        # Copy the AWK scripts to the test directory
        script_dir = os.path.dirname(os.path.abspath(__file__))
        parent_dir = os.path.dirname(script_dir)
        for awk_file in ['Util.awk', 'MetadataWriter.awk', 'MarkdownParser.awk', 'BlogWriter.awk']:
            shutil.copy2(
                os.path.join(parent_dir, awk_file),
                os.path.join(self.test_dir, awk_file)
            )
        
        os.chdir(self.test_dir)

    def tearDown(self):
        # Clean up
        os.chdir(self.original_dir)
        shutil.rmtree(self.test_dir)

    def test_blog_generation(self):
        """Test that the blog is generated correctly"""
        # First generate metadata
        process = subprocess.run([
            'gawk',
            '-f', 'Util.awk',
            '-f', 'MetadataWriter.awk',
            'journal.md'
        ], capture_output=True, text=True)
        self.assertEqual(process.returncode, 0, f"Metadata generation failed: {process.stderr}")
        
        # Then generate the blog
        process = subprocess.run([
            'gawk',
            '-f', 'Util.awk',
            '-f', 'MarkdownParser.awk',
            '-f', 'BlogWriter.awk',
            'public/metadata.txt',
            'journal.md'
        ], capture_output=True, text=True)
        
        # Check if the process completed successfully
        self.assertEqual(process.returncode, 0, f"Blog generation failed: {process.stderr}")
        
        # Verify blog output directory exists
        self.assertTrue(os.path.exists(os.path.join(self.test_dir, 'public', 'blog')))
        
        # Check if HTML files were generated
        html_files = [f for f in os.listdir(os.path.join(self.test_dir, 'public', 'blog')) if f.endswith('.html')]
        self.assertTrue(len(html_files) > 0, "No HTML files were generated")
        
        # Check the content of the generated HTML
        with open(os.path.join(self.test_dir, 'public', 'blog', html_files[0]), 'r', encoding='utf-8') as f:
            content = f.read()
            self.assertIn('Test Blog Entry', content)
            self.assertIn('This is a test blog entry', content)

    def test_metadata_generation(self):
        """Test that metadata is generated correctly for blog posts"""
        # Run the metadata generation step
        process = subprocess.run([
            'gawk',
            '-f', 'Util.awk',
            '-f', 'MetadataWriter.awk',
            'journal.md'
        ], capture_output=True, text=True)
        
        self.assertEqual(process.returncode, 0)
        self.assertTrue(os.path.exists('public/metadata.txt'))

if __name__ == '__main__':
    unittest.main() 