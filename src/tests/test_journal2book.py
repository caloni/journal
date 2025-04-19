import unittest
import os
import shutil
import tempfile
from pathlib import Path
import subprocess
import sys

# Add the parent directory to the Python path so we can import the modules
sys.path.append(str(Path(__file__).parent.parent))

class TestJournal2Book(unittest.TestCase):
    def setUp(self):
        # Create a temporary directory
        self.test_dir = tempfile.mkdtemp()
        self.original_dir = os.getcwd()
        
        # Create necessary test directories
        os.makedirs(os.path.join(self.test_dir, 'book'))
        os.makedirs(os.path.join(self.test_dir, 'img', 'book'))
        os.makedirs(os.path.join(self.test_dir, 'img', 'blog'))
        os.makedirs(os.path.join(self.test_dir, 'public', 'book', 'EPUB', 'img'))
        
        # Create a sample journal.md file
        with open(os.path.join(self.test_dir, 'journal.md'), 'w') as f:
            f.write("""# Test Journal Entry
[date]:# "2024-01-01"
[tags]:# "test"

This is a test journal entry.
""")
        
        # Copy the AWK scripts to the test directory
        script_dir = os.path.dirname(os.path.abspath(__file__))
        parent_dir = os.path.dirname(script_dir)
        for awk_file in ['Util.awk', 'MetadataWriter.awk', 'MarkdownParser.awk', 'BookWriter.awk']:
            shutil.copy2(
                os.path.join(parent_dir, awk_file),
                os.path.join(self.test_dir, awk_file)
            )
        
        os.chdir(self.test_dir)

    def tearDown(self):
        # Clean up
        os.chdir(self.original_dir)
        shutil.rmtree(self.test_dir)

    def test_directory_structure_creation(self):
        """Test that the script creates the necessary directory structure"""
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
        
        # Verify directory structure
        self.assertTrue(os.path.exists(os.path.join(self.test_dir, 'public', 'book')))
        self.assertTrue(os.path.exists(os.path.join(self.test_dir, 'public', 'book', 'EPUB', 'img')))

    def test_metadata_generation(self):
        """Test that metadata is generated correctly"""
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