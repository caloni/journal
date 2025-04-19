import unittest
import os
import shutil
import tempfile
from pathlib import Path
import subprocess
import sys

# Add the parent directory to the Python path so we can import the modules
sys.path.append(str(Path(__file__).parent.parent))

class TestFullJournal(unittest.TestCase):
    def setUp(self):
        # Create a temporary directory
        self.test_dir = tempfile.mkdtemp()
        self.original_dir = os.getcwd()
        
        # Create necessary test directories
        os.makedirs(os.path.join(self.test_dir, 'blog'))
        os.makedirs(os.path.join(self.test_dir, 'img', 'blog'))
        os.makedirs(os.path.join(self.test_dir, 'public', 'blog'))
        os.makedirs(os.path.join(self.test_dir, 'book'))
        os.makedirs(os.path.join(self.test_dir, 'img', 'book'))
        os.makedirs(os.path.join(self.test_dir, 'public', 'book', 'EPUB', 'img'))
        
        # Copy the original journal.md to the test directory
        script_dir = os.path.dirname(os.path.abspath(__file__))
        parent_dir = os.path.dirname(script_dir)  # Go up one level to src
        journal_path = os.path.dirname(parent_dir)  # Go up one more level to find journal.md
        shutil.copy2(
            os.path.join(journal_path, 'journal.md'),
            os.path.join(self.test_dir, 'journal.md')
        )
        
        # Copy the AWK scripts to the test directory
        for awk_file in ['Util.awk', 'MetadataWriter.awk', 'MarkdownParser.awk', 'BlogWriter.awk', 'BookWriter.awk']:
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
        """Test that the blog is generated correctly from the full journal"""
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

    def test_book_generation(self):
        """Test that the book is generated correctly from the full journal"""
        # First generate metadata
        process = subprocess.run([
            'gawk',
            '-f', 'Util.awk',
            '-f', 'MetadataWriter.awk',
            'journal.md'
        ], capture_output=True, text=True)
        self.assertEqual(process.returncode, 0, f"Metadata generation failed: {process.stderr}")
        
        # Then generate the book
        process = subprocess.run([
            'gawk',
            '-f', 'Util.awk',
            '-f', 'MarkdownParser.awk',
            '-f', 'BookWriter.awk',
            'public/metadata.txt',
            'journal.md'
        ], capture_output=True, text=True)
        
        # Check if the process completed successfully
        self.assertEqual(process.returncode, 0, f"Book generation failed: {process.stderr}")
        
        # Verify book output directory exists
        self.assertTrue(os.path.exists(os.path.join(self.test_dir, 'public', 'book')))
        self.assertTrue(os.path.exists(os.path.join(self.test_dir, 'public', 'book', 'EPUB')))
        
        # Check if XHTML files were generated
        xhtml_files = [f for f in os.listdir(os.path.join(self.test_dir, 'public', 'book', 'EPUB')) if f.endswith('.xhtml')]
        self.assertTrue(len(xhtml_files) > 0, "No XHTML files were generated")

if __name__ == '__main__':
    unittest.main() 